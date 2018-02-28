/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_output_tws_chain.c

DESCRIPTION
    TWS chain functionality
*/
#include <panic.h>
#include <stream.h>

#include <chain.h>
#include <rtime.h>
#include <audio_plugin_forwarding.h>

#include "sbc_encoder_params.h"
#include "audio_output_tws.h"
#include "audio_output_tws_chain.h"
#include "audio_input_common.h"

#define SPLITTER_BUFFER_SIZE    (4096)
#define size_array(x) (sizeof(x)/sizeof((x)[0]))

typedef enum
{
    tws_splitter_role,
    tws_decoder_role,
    tws_encoder_role,
    tws_ttp_role
} tws_operator_role_t;

typedef enum
{
    tws_transcode_pcm_left_input,
    tws_transcode_pcm_right_input
} tws_input_role_t;

typedef enum
{
    tws_transcode_left,
    tws_transcode_right,
    tws_transcode_forwarding
} tws_output_role_t;

/* Configuration */

static const operator_config_t transcode_ops[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_ttp_passthrough, tws_ttp_role),
    MAKE_OPERATOR_CONFIG(capability_id_sbc_encoder, tws_encoder_role),
    MAKE_OPERATOR_CONFIG(capability_id_splitter, tws_splitter_role),
    MAKE_OPERATOR_CONFIG(capability_id_sbc_decoder, tws_decoder_role)
};

static const operator_endpoint_t transcode_inputs[] =
{
    {tws_ttp_role, tws_transcode_pcm_left_input,      0},
    {tws_ttp_role, tws_transcode_pcm_right_input,     1},
};

static const operator_endpoint_t transcode_outputs[] =
{
    {tws_decoder_role, tws_transcode_left,        0},
    {tws_decoder_role, tws_transcode_right,       1},
    {tws_splitter_role, tws_transcode_forwarding, 1},
};

static const operator_connection_t transcode_connections[] =
{
    {tws_ttp_role,      0, tws_encoder_role,  0, 2},
    {tws_encoder_role,  0, tws_splitter_role, 0, 1},
    {tws_splitter_role, 0, tws_decoder_role,  0, 1}
};

static const chain_config_t transcode_config_pcm_to_sbc = 
    MAKE_CHAIN_CONFIG(chain_id_audio_output_tws,
                      audio_ucid_output_tws,
                      transcode_ops, transcode_inputs, transcode_outputs,
                      transcode_connections);

/* Core chain instance */
static kymera_chain_handle_t transcode_chain;

static void configureSplitterIfPresent(void)
{
    Operator splitter = ChainGetOperatorByRole(transcode_chain, tws_splitter_role);
    if(splitter)
    {
        OperatorsStandardSetBufferSize(splitter, SPLITTER_BUFFER_SIZE);
        OperatorsSplitterSetDataFormat(splitter, operator_data_format_encoded);
        OperatorsSplitterEnableSecondOutput(splitter, TRUE);
    }
}

static void configureTtpPassthroughIfPresent(const audio_plugin_forwarding_params_t* params)
{
    Operator ttp = ChainGetOperatorByRole(transcode_chain, tws_ttp_role);
    if(ttp)
    {
        OperatorsStandardSetTimeToPlayLatency(ttp, TTP_LATENCY_IN_US(params->ttp_latency.target_in_ms));
        OperatorsStandardSetLatencyLimits(ttp, TTP_LATENCY_IN_US(params->ttp_latency.min_in_ms),
                                               TTP_LATENCY_IN_US(params->ttp_latency.max_in_ms));
        OperatorsStandardSetSampleRate(ttp, params->sample_rate);
        OperatorsSetPassthroughDataFormat(ttp, operator_data_format_pcm);
    }
}

static void configureSbcEncoderIfPresent(const audio_plugin_forwarding_params_t* params)
{
    if(params->transcode.codec == audio_codec_sbc)
    {
        Operator sbc_encoder = ChainGetOperatorByRole(transcode_chain, tws_encoder_role);
        if(sbc_encoder)
        {
            unsigned bitpool = params->transcode.params.sbc.bitpool;
            unsigned settings = params->transcode.params.sbc.settings;
            sbc_encoder_params_t sbc_params = ConvertSbcEncoderParams(bitpool, settings);
            OperatorsSbcEncoderSetEncodingParams(sbc_encoder, &sbc_params);
        }
    }
}

static const chain_config_t* getConfig(audio_codec_t source_codec, audio_codec_t dest_codec)
{
    if(source_codec == audio_codec_pcm && dest_codec == audio_codec_sbc)
        return &transcode_config_pcm_to_sbc;
    
    Panic();
    return NULL;
}

static void connectPcmInputs(const audio_plugin_forwarding_params_t* params)
{
    Sink pcm_left_sink = ChainGetInput(transcode_chain, tws_transcode_pcm_left_input);
    Sink pcm_right_sink = ChainGetInput(transcode_chain, tws_transcode_pcm_right_input);
    
    PanicNull(StreamConnect(params->forwarding.pcm.left_source, pcm_left_sink));
    PanicNull(StreamConnect(params->forwarding.pcm.right_source, pcm_right_sink));
}

void AudioOutputTwsChainCreate(const audio_plugin_forwarding_params_t* params)
{
    const chain_config_t* config = getConfig(params->source_codec, params->transcode.codec);
    
    transcode_chain = PanicNull(ChainCreate(config));
    
    configureSplitterIfPresent();
    configureTtpPassthroughIfPresent(params);
    configureSbcEncoderIfPresent(params);
    
    ChainConnect(transcode_chain);
    connectPcmInputs(params);
}

void AudioOutputTwsChainStart(void)
{
    if(transcode_chain)
        ChainStart(transcode_chain);
}

void AudioOutputTwsChainStop(void)
{
    if(transcode_chain)
        ChainStop(transcode_chain);
}

void AudioOutputTwsChainDestroy(void)
{
    if(transcode_chain)
    {
        ChainStop(transcode_chain);
        ChainDestroy(transcode_chain);
        transcode_chain = NULL;
    }
}

Source AudioOutputTwsChainGetLeftSource(void)
{
    return ChainGetOutput(transcode_chain, tws_transcode_left);
}

Source AudioOutputTwsChainGetRightSource(void)
{
    return ChainGetOutput(transcode_chain, tws_transcode_right);
}

Source AudioOutputTwsChainGetForwardingSource(void)
{
    return ChainGetOutput(transcode_chain, tws_transcode_forwarding);
}

void AudioOutputTwsChainReset(void)
{
    AudioOutputTwsChainDestroy();
}
