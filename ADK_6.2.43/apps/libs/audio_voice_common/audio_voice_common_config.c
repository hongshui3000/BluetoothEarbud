/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_voice_common_config.c

DESCRIPTION
    Implementation of chain construction functions.
*/

#include "audio_voice_common_config.h"

#include <audio_config.h>

#include "audio_voice_common_dsp.h"

#define UNCONNECTED           (255)

#define CVC_OPS_NB \
    MAKE_OPERATOR_CONFIG(capability_id_none, receive_role), \
    MAKE_OPERATOR_CONFIG_PRIORITY_HIGH(capability_id_none, send_role), \
    MAKE_OPERATOR_CONFIG(capability_id_cvc_receive_nb, cvc_receive_role)

#define CVC_OPS_WB \
    MAKE_OPERATOR_CONFIG(capability_id_none, receive_role), \
    MAKE_OPERATOR_CONFIG_PRIORITY_HIGH(capability_id_none, send_role), \
    MAKE_OPERATOR_CONFIG(capability_id_cvc_receive_wb, cvc_receive_role)


static operator_config_t ops_nb_1mic_hs[] =
{
    CVC_OPS_NB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hs_1mic_nb, cvc_send_role)
};

static operator_config_t ops_nb_1mic_speaker[] =
{
    CVC_OPS_NB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_spk_1mic_nb, cvc_send_role)
};

static operator_config_t ops_nb_1mic_handsfree[] =
{
    CVC_OPS_NB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hf_1mic_nb, cvc_send_role)
};

static const operator_config_t ops_wb_1mic_hs[] =
{
    CVC_OPS_WB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hs_1mic_wb, cvc_send_role)
};

static const operator_config_t ops_wb_1mic_speaker[] =
{
    CVC_OPS_WB,
#ifdef DOWNLOAD_CVC_SPK_1MIC_WB
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_download_cvc_spk_1mic_wb, cvc_send_role)
#else
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_spk_1mic_wb, cvc_send_role)
#endif
};

static const operator_config_t ops_wb_1mic_handsfree[] =
{
    CVC_OPS_WB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hf_1mic_wb, cvc_send_role)
};


static const operator_config_t ops_nb_2mic_hs[] =
{
    CVC_OPS_NB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hs_2mic_90deg_nb, cvc_send_role)
};

static const operator_config_t ops_nb_2mic_speaker[] =
{
    CVC_OPS_NB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_spk_2mic_0deg_nb, cvc_send_role)
};

static const operator_config_t ops_nb_2mic_handsfree[] =
{
    CVC_OPS_NB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hf_2mic_nb, cvc_send_role)
};

static const operator_config_t ops_wb_2mic_hs[] =
{
    CVC_OPS_WB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hs_2mic_90deg_wb, cvc_send_role)
};

static const operator_config_t ops_wb_2mic_speaker[] =
{
    CVC_OPS_WB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_spk_2mic_0deg_wb, cvc_send_role)
};

static const operator_config_t ops_wb_2mic_handsfree[] =
{
    CVC_OPS_WB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hf_2mic_wb, cvc_send_role)
};

static const operator_config_t ops_binaural_nb_2mic_hs[] =
{
    CVC_OPS_NB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hs_2mic_0deg_nb, cvc_send_role)
};

static const operator_config_t ops_binaural_wb_2mic_hs[] =
{
    CVC_OPS_WB,
    MAKE_OPERATOR_CONFIG_PRIORITY_LOWEST(capability_id_cvc_hs_2mic_0deg_wb, cvc_send_role)
};

static const operator_config_t ops_nb_no_cvc[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_none, receive_role),
    MAKE_OPERATOR_CONFIG_PRIORITY_HIGH(capability_id_none, send_role)
};

static const operator_config_t ops_wb_no_cvc[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_none, receive_role),
    MAKE_OPERATOR_CONFIG_PRIORITY_HIGH(capability_id_none, send_role)
};

/* --------------------------------------------------------------*/

static const operator_stream_node_t receive[] =
{
    {receive_role, 0, 0},
    {cvc_receive_role, 0, 0},
};

static const operator_stream_node_t send_mic1[] =
{
    {cvc_send_role, 1, 0},
    {send_role, 0, 0}
};

static const operator_stream_node_t send_mic2[] =
{
    {cvc_send_role, 2, UNCONNECTED},
};

static const operator_stream_node_t aec_ref[] =
{
    {cvc_send_role, 0, UNCONNECTED}
};

/* --------------------------------------------------------------*/

static const operator_stream_t streams[] =
{
    {stream_receive, stream_with_in_and_out, ARRAY_DIM((receive)), receive},
    {stream_send_mic1, stream_with_in_and_out, ARRAY_DIM((send_mic1)), send_mic1},
    {stream_send_mic2, stream_with_input, ARRAY_DIM((send_mic2)), send_mic2},
    {stream_aec_ref, stream_with_input, ARRAY_DIM((aec_ref)), aec_ref}
};

/*Headset varaints*/
static const chain_config_t audio_voice_hfp_config_1mic_nb_hs =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_headset, ops_nb_1mic_hs, streams);

static const chain_config_t audio_voice_hfp_config_1mic_wb_hs =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_headset, ops_wb_1mic_hs, streams);

static const chain_config_t audio_voice_hfp_config_2mic_nb_hs =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_headset, ops_nb_2mic_hs, streams);

static const chain_config_t audio_voice_hfp_config_2mic_wb_hs =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_headset, ops_wb_2mic_hs, streams);

static const chain_config_t audio_voice_hfp_config_2mic_binaural_nb_hs =
        MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_headset, ops_binaural_nb_2mic_hs, streams);

static const chain_config_t audio_voice_hfp_config_2mic_binaural_wb_hs =
        MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_headset, ops_binaural_wb_2mic_hs, streams);

/*Handsfree varaints*/
static const chain_config_t audio_voice_hfp_config_1mic_nb_handsfree =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_handsfree, ops_nb_1mic_handsfree, streams);

static const chain_config_t audio_voice_hfp_config_1mic_wb_handsfree =
        MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_handsfree, ops_wb_1mic_handsfree, streams);

static const chain_config_t audio_voice_hfp_config_2mic_nb_handsfree =
        MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_handsfree, ops_nb_2mic_handsfree, streams);

static const chain_config_t audio_voice_hfp_config_2mic_wb_handsfree =
        MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_handsfree, ops_wb_2mic_handsfree, streams);

/*Speaker varaints*/
static const chain_config_t audio_voice_hfp_config_1mic_nb_speaker =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_speaker, ops_nb_1mic_speaker, streams);

static const chain_config_t audio_voice_hfp_config_1mic_wb_speaker =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_speaker, ops_wb_1mic_speaker, streams);

static const chain_config_t audio_voice_hfp_config_2mic_nb_speaker =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_speaker, ops_nb_2mic_speaker, streams);

static const chain_config_t audio_voice_hfp_config_2mic_wb_speaker =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp_cvc_speaker, ops_wb_2mic_speaker, streams);


/* --------------------------------------------------------------*/

static const operator_stream_node_t receive_no_cvc[] =
{
    {receive_role, 0, 0},
};

static const operator_stream_node_t send_mic1_no_cvc[] =
{
    {send_role, 0, 0}
};

static const operator_stream_t streams_no_cvc[] =
{
    {stream_receive, stream_with_in_and_out, ARRAY_DIM((receive_no_cvc)), receive_no_cvc},
    {stream_send_mic1, stream_with_in_and_out, ARRAY_DIM((send_mic1_no_cvc)), send_mic1_no_cvc},
};

static const chain_config_t audio_voice_hfp_config_nb_no_cvc =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp, ops_nb_no_cvc, streams_no_cvc);


static const chain_config_t audio_voice_hfp_config_wb_no_cvc =
    MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_cvc_common, audio_ucid_hfp, ops_wb_no_cvc, streams_no_cvc);



/******************************************************************************/
const chain_config_t* AudioVoiceCommonGetChainConfig(const audio_voice_context_t* ctx)
{
    const unsigned plugin_variant = ctx->variant;

    if(AudioConfigGetQuality(audio_stream_voice) == audio_quality_low_power)
    {
        if(ctx->encoder == link_encoding_msbc)
            return &audio_voice_hfp_config_wb_no_cvc;

        return &audio_voice_hfp_config_nb_no_cvc;
    }

    switch(plugin_variant)
    {
        case cvc_1_mic_headset_cvsd:
            return &audio_voice_hfp_config_1mic_nb_hs;

        case cvc_1_mic_headset_msbc:
            return &audio_voice_hfp_config_1mic_wb_hs;

        case cvc_2_mic_headset_cvsd:
            return &audio_voice_hfp_config_2mic_nb_hs;

        case cvc_2_mic_headset_msbc:
            return &audio_voice_hfp_config_2mic_wb_hs;

        case cvc_2_mic_headset_binaural_nb:
            return &audio_voice_hfp_config_2mic_binaural_nb_hs;

        case cvc_2_mic_headset_binaural_wb:
            return &audio_voice_hfp_config_2mic_binaural_wb_hs;

        case cvc_1_mic_handsfree_cvsd:
            return &audio_voice_hfp_config_1mic_nb_handsfree;

        case cvc_1_mic_handsfree_msbc:
            return &audio_voice_hfp_config_1mic_wb_handsfree;

        case cvc_2_mic_handsfree_cvsd:
            return &audio_voice_hfp_config_2mic_nb_handsfree;

        case cvc_2_mic_handsfree_msbc:
            return &audio_voice_hfp_config_2mic_wb_handsfree;

        case cvc_1_mic_speaker_cvsd:
            return &audio_voice_hfp_config_1mic_nb_speaker;

        case cvc_1_mic_speaker_msbc:
            return &audio_voice_hfp_config_1mic_wb_speaker;

        case cvc_2_mic_speaker_cvsd:
            return &audio_voice_hfp_config_2mic_nb_speaker;

        case cvc_2_mic_speaker_msbc:
            return &audio_voice_hfp_config_2mic_wb_speaker;

        default:
            return NULL;
    }
}
