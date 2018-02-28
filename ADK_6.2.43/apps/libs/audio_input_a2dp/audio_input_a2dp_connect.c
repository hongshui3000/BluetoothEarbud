/****************************************************************************
Copyright (c) 2016 - 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_a2dp_connect.c

DESCRIPTION
    Implementation of AUDIO_PLUGIN_CONNECT_MSG message for a2dp source.
    Remaining message handlers are implemented in audio_input_common library.
*/

#include <stdlib.h>
#include <operators.h>
#include <audio_mixer.h>
#include <audio.h>
#include <audio_data_types.h>
#include <audio_plugin_music_params.h>
#include <audio_plugin_music_variants.h>
#include <audio_input_common.h>
#include <panic.h>
#include <audio_config.h>

#include "audio_input_a2dp.h"
#include "audio_input_a2dp_chain_config.h"
#include "audio_input_a2dp_connect.h"
#include "audio_input_a2dp_task.h"
#include "audio_input_a2dp_broadcast.h"
#include "audio_input_common.h"

/* Make sure buffer is large enough to hold number of samples corresponding to the latency */
#define RTP_BUFFER_SIZE (8000)
#define BA_RTP_BUFFER_SIZE (1000)
#define SPLITTER_BUFFER_SIZE (2048)

#define APTX_LL_SAMPLES_PER_CODEWORD    ((unsigned)4)
#define APTX_LL_CHANNELS                    ((unsigned)2)
#define APTX_LL_RTP_BUFFER_RATIO            ((unsigned)2)
#define APTX_LL_MINIMUM_LATENCY_IN_US       (0)
#define APTX_LL_MAXIMUM_LATENCY_DELTA_IN_US (10000)
#define US_PER_SECOND                   (1000000)

/* MTU? (895) - max tws packet header (5) = 890, and then round down to a multiple of 4 := 888 */
#define RTP_PACKET_LIMITED_LENGTH           (888)

/* MTU? (895) - max share me packet header (13) = 882, and then round down to a multiple of 4 := 880 */
#define RTP_PACKET_LIMITED_LENGTH_SHAREME   (880)

static bool getContentProtection(A2dpPluginConnectParams *params);
static kymera_chain_handle_t createChain(audio_codec_t decoder, chain_variant_t variant,
        bool content_protection, uint32 sample_rate, A2dpPluginConnectParams *a2dp_connect_params);
static rtp_codec_type_t decoderToRtpCodecType(audio_codec_t decoder);
static rtp_working_mode_t workOutRtpMode(audio_codec_t decoder, bool content_protection);
static void configureRtp(Operator rtp_op, audio_codec_t decoder, bool content_protection, uint32 sample_rate,
                         A2dpPluginConnectParams *a2dp_connect_params,
						 ttp_latency_t *ttp_latency);
static void configureRtpBasedOnDecoder(audio_codec_t decoder, kymera_chain_handle_t chain,
                             A2dpPluginConnectParams *a2dp_connect_params, uint32 sample_rate);
static void setupAptxLowLatency(Operator rtp_op, A2dpPluginConnectParams *a2dp_connect_params, uint32 sample_rate);
static void setupRtpAacCodec(Operator rtp_op, Operator aac_op);
static void setupRtpBroadcasterLatency(Operator rtp_op);

static uint16 aptx_ll_latency_ms = 0;

static unsigned getKickPeriodFromTask(Task task)
{
    if(AudioPluginA2dpTaskIsBroadcaster(task))
        return BA_KICK_PERIOD;
    
    return AudioInputCommonGetKickPeriodFromCodec(AudioPluginA2dpTaskGetCodec(task));
}

/****************************************************************************
DESCRIPTION
    AUDIO_PLUGIN_CONNECT_MSG message handler.
    It creates Kymera chain and connect it to source and audio_mixer.
*/
void AudioInputA2dpConnectHandler(audio_input_context_t *ctx, Task task,
        const AUDIO_PLUGIN_CONNECT_MSG_T *msg)
{
    bool content_protection = getContentProtection(msg->params);
    A2dpPluginConnectParams* a2dpParams = (A2dpPluginConnectParams *)msg->params;
    audio_codec_t codec = AudioPluginA2dpTaskGetCodec(task);


    AudioInputCommonDspPowerOn();
    OperatorsFrameworkSetKickPeriod(getKickPeriodFromTask(task));

    ctx->left_source = StreamSourceFromSink(msg->audio_sink);
    AudioInputCommonSetMusicProcessingContext(ctx, a2dpParams);

    ctx->tws.mute_until_start_forwarding = a2dpParams->peer_is_available;
    ctx->sample_rate = msg->rate;
    ctx->app_task = msg->app_task;

    if(!AudioPluginA2dpTaskIsBroadcaster(task))
    {
        audio_mixer_connect_t connect_data;

        ctx->chain = createChain(codec, chain_variant_with_splitter, content_protection,
                ctx->sample_rate, a2dpParams);

        connect_data.left_src = ChainGetOutput(ctx->chain, stream_pcm_left);
        connect_data.right_src = ChainGetOutput(ctx->chain, stream_pcm_right);
        connect_data.connection_type = CONNECTION_TYPE_MUSIC_A2DP;
        connect_data.sample_rate = ctx->sample_rate;
        connect_data.channel_mode = CHANNEL_MODE_STEREO;
        connect_data.variable_rate = FALSE;

        ctx->mixer_input = AudioMixerConnect(&connect_data);

        PanicFalse(ctx->mixer_input != audio_mixer_input_error_none);
        
        /* Disconnect A2DP source as close to start as possible */
        StreamDisconnect(ctx->left_source, 0);
        StreamConnect(ctx->left_source, ChainGetInput(ctx->chain, stream_encoded));
        ChainStart(ctx->chain);

        AudioInputCommonConnect(ctx, task);
    }
    else
    {
        ctx->chain = createChain(codec, chain_variant_without_splitter, content_protection,
                ctx->sample_rate, a2dpParams);
        ctx->ba.plugin = a2dpParams->ba_output_plugin;

        audioInputA2dpBroadcastCreate(task, ctx->ba.plugin, ctx);

        SetAudioBusy(task);
    }
}

/****************************************************************************
DESCRIPTION
    Read the TTP latency used for A2DP
*/
uint16 audioInputA2dpGetLatency(audio_input_context_t *ctx, Task audio_plugin, bool *estimated)
{
    *estimated = FALSE;

    if(((A2dpPluginTaskdata*)audio_plugin)->a2dp_plugin_variant == APTX_ACL_SPRINT_DECODER)
    {
        if(ctx)
        {
            return aptx_ll_latency_ms;
        }
        else
        {
            *estimated = TRUE;
            return TTP_APTX_LL_ESTIMATED_LATENCY_IN_MS;
        }
    }
    else
    {
        ttp_latency_t ttp_latency = AudioConfigGetA2DPTtpLatency();
        return ttp_latency.target_in_ms;
    }
}

/****************************************************************************
DESCRIPTION
    Configures the RTP time to play latency
*/
void audioInputConfigureRTPLatency(Operator rtp_op, ttp_latency_t *ttp_latency)
{
    OperatorsStandardSetTimeToPlayLatency(rtp_op, TTP_LATENCY_IN_US(ttp_latency->target_in_ms));
    OperatorsStandardSetLatencyLimits(rtp_op,
                                          TTP_LATENCY_IN_US(ttp_latency->min_in_ms),
                                          TTP_LATENCY_IN_US(ttp_latency->max_in_ms));
}

/****************************************************************************
DESCRIPTION
    Extracts content protection bit from connect parameters.
*/
static bool getContentProtection(A2dpPluginConnectParams *params)
{
    return params->content_protection;
}

/****************************************************************************
DESCRIPTION
    Create Kymera chain for decoder.
*/
static kymera_chain_handle_t createChain(audio_codec_t decoder, chain_variant_t variant,
                                  bool content_protection, uint32 sample_rate,
                                  A2dpPluginConnectParams *a2dp_connect_params)
{
    kymera_chain_handle_t chain;
    Operator rtp_op;
    Operator splitter_op;
    const chain_config_t *chain_config = AudioInputA2dpGetChainConfig(decoder);
    const operator_filters_t *config_filters = AudioInputA2dpGetChainConfigFilter(decoder, variant);
    ttp_latency_t ttp_latency = AudioConfigGetA2DPTtpLatency();

    chain = PanicNull(ChainCreateWithFilter(chain_config, config_filters));

    rtp_op = ChainGetOperatorByRole(chain, rtp_role);
    configureRtp(rtp_op, decoder, content_protection, sample_rate, a2dp_connect_params, &ttp_latency);

    splitter_op = ChainGetOperatorByRole(chain, splitter_role);
    if(splitter_op)
    {
        OperatorsConfigureSplitter(splitter_op, SPLITTER_BUFFER_SIZE, FALSE, operator_data_format_encoded);
    }

    configureRtpBasedOnDecoder(decoder, chain, a2dp_connect_params, sample_rate);

    ChainConnect(chain);
    return chain;
}

/****************************************************************************
DESCRIPTION
    Determines whether RTP packet size requires limiting
*/
static bool rtpPacketLengthNeedsLimiting(audio_codec_t decoder)
{
    return (decoder == audio_codec_aptx);
}

static uint16 rtpPacketLength(void)
{
    if(AudioConfigGetPeerMode() == peer_mode_shareme) 
    {
        return RTP_PACKET_LIMITED_LENGTH_SHAREME;
    }
    else
    {
        return RTP_PACKET_LIMITED_LENGTH;
    }
}

/****************************************************************************
DESCRIPTION
    Create rtp operator.
*/
static void configureRtp(Operator rtp_op, audio_codec_t decoder,
                                  bool content_protection,
                                  uint32 sample_rate,
                                  A2dpPluginConnectParams *a2dp_connect_params, 
                                  ttp_latency_t *ttp_latency)
{
    rtp_codec_type_t rtp_codec_type;
    rtp_working_mode_t rtp_mode;

    rtp_mode = workOutRtpMode(decoder, content_protection);
    rtp_codec_type = decoderToRtpCodecType(decoder);

    OperatorsConfigureRtp(rtp_op, rtp_codec_type, content_protection, rtp_mode);

    if(rtpPacketLengthNeedsLimiting(decoder))
    {
        OperatorsRtpSetMaximumPacketLength(rtp_op, rtpPacketLength());
    }

    /* Additional time-to-play setup messages */
    OperatorsStandardSetSampleRate(rtp_op, sample_rate);

    /*! @todo: This is a temporary hack. We should perhaps refactor
     *  setting the rtp latency into a separate function. */
    if (a2dp_connect_params->ba_output_plugin)
    {
        setupRtpBroadcasterLatency(rtp_op);
        OperatorsStandardSetBufferSize(rtp_op, BA_RTP_BUFFER_SIZE);
    }
    else
    {
        audioInputConfigureRTPLatency(rtp_op, ttp_latency);
        OperatorsStandardSetBufferSize(rtp_op, RTP_BUFFER_SIZE);
    }
}

/****************************************************************************
DESCRIPTION
    Convert local decoder type to one understood by RTP operator.
*/
static rtp_codec_type_t decoderToRtpCodecType(audio_codec_t decoder)
{
    rtp_codec_type_t rtp_codec_type = rtp_codec_type_sbc;

    switch(decoder)
    {
        case audio_codec_sbc:
            rtp_codec_type = rtp_codec_type_sbc;
            break;
        case audio_codec_aptx:
        case audio_codec_aptx_ll:
            rtp_codec_type = rtp_codec_type_aptx;
            break;
        case audio_codec_aptx_hd:
            rtp_codec_type = rtp_codec_type_aptx_hd;
            break;
        case audio_codec_aac:
            rtp_codec_type = rtp_codec_type_aac;
            break;
        default:
            Panic();
    }

    return rtp_codec_type;
}

/****************************************************************************
DESCRIPTION
    Determine RTP working mode.
*/
static rtp_working_mode_t workOutRtpMode(audio_codec_t decoder, bool content_protection)
{
    rtp_working_mode_t rtp_mode;

    if(((decoder == audio_codec_aptx) || (decoder == audio_codec_aptx_ll)) &&
         content_protection == FALSE)
    {
        rtp_mode = rtp_ttp_only;
    }
    else
    {
        rtp_mode = rtp_decode;
    }

    return rtp_mode;
}

/****************************************************************************
DESCRIPTION
    Configuration of decoder specific parameters.
*/
static void configureRtpBasedOnDecoder(audio_codec_t decoder,
                            kymera_chain_handle_t chain,
                            A2dpPluginConnectParams *a2dp_connect_params,
                            uint32 sample_rate)
{
    switch(decoder)
    {
    case audio_codec_aac:
        setupRtpAacCodec(ChainGetOperatorByRole(chain, rtp_role), ChainGetOperatorByRole(chain, decoder_role));
        break;
    case audio_codec_aptx_ll:
        setupAptxLowLatency(ChainGetOperatorByRole(chain, rtp_role), a2dp_connect_params, sample_rate);
        break;
    default:
        break;
    }
}

/****************************************************************************
DESCRIPTION
    Set-up APTX LL additional parameters:
    TTP latency and RTP decoder buffer size.
*/
static void setupAptxLowLatency(Operator rtp_op, A2dpPluginConnectParams *a2dp_connect_params, uint32 sample_rate)
{
    unsigned ttp_latency_us = ((a2dp_connect_params->aptx_sprint_params.target_codec_level *
                                APTX_LL_SAMPLES_PER_CODEWORD * APTX_LL_CHANNELS * US_PER_SECOND)
                            / sample_rate);
    unsigned buffer_size = a2dp_connect_params->aptx_sprint_params.initial_codec_level * APTX_LL_RTP_BUFFER_RATIO;

    OperatorsStandardSetBufferSize(rtp_op, buffer_size);
    OperatorsStandardSetTimeToPlayLatency(rtp_op, ttp_latency_us);
    OperatorsStandardSetLatencyLimits(rtp_op,
                                      APTX_LL_MINIMUM_LATENCY_IN_US,
                                      ttp_latency_us + APTX_LL_MAXIMUM_LATENCY_DELTA_IN_US);

    aptx_ll_latency_ms = (uint16_t)(ttp_latency_us / 1000);
}

/****************************************************************************
DESCRIPTION
    Associate Aac decoder with the rtp capability
*/
static void setupRtpAacCodec(Operator rtp_op, Operator aac_op)
{
    OperatorsRtpSetAacCodec(rtp_op, aac_op);
}

/****************************************************************************
DESCRIPTION
    Set RTP latencies for running in the broadcaster role
*/
static void setupRtpBroadcasterLatency(Operator rtp_op)
{
    OperatorsStandardSetTimeToPlayLatency(rtp_op, TTP_BA_LATENCY_IN_US);
    OperatorsStandardSetLatencyLimits(rtp_op,
                                      TTP_BA_MIN_LATENCY_LIMIT_US,
                                      TTP_BA_MAX_LATENCY_LIMIT_US);
}

void AudioInputA2dpTestReset(void)
{
#ifndef AUDIO_TEST_BUILD
    Panic();
#else
    AudioMixerTestReset();
#endif
}
