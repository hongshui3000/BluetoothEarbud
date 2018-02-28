/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_input_a2dp_chain_config.c

DESCRIPTION
    Implementation of function to return the appropriate chain configuration data.
*/

#include <operators.h>
#include <panic.h>
#include "audio_input_a2dp_chain_config.h"

static const operator_config_t op_config[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_rtp, rtp_role),
    MAKE_OPERATOR_CONFIG(capability_id_splitter, splitter_role),
    MAKE_OPERATOR_CONFIG(capability_id_sbc_decoder, decoder_role)
};

static const operator_stream_node_t encoded_channel[] =
{
    {rtp_role, 0, 0},
    {splitter_role, 0, 0},
    {decoder_role, 0, 0}
};

static const operator_stream_node_t forwarding_channel[] =
{
    {splitter_role, 0, 1}
};

static const operator_stream_node_t pcm_left_channel[] =
{
    {decoder_role, 0, 0}
};

static const operator_stream_node_t pcm_right_channel[] =
{
    {decoder_role, 0, 1}
};

static const operator_stream_t streams[] =
{
    {stream_encoded, stream_with_input, ARRAY_DIM((encoded_channel)), encoded_channel},
    {stream_forwarding, stream_with_output, ARRAY_DIM((forwarding_channel)), forwarding_channel},
    {stream_pcm_left, stream_with_output, ARRAY_DIM((pcm_left_channel)), pcm_left_channel},
    {stream_pcm_right, stream_with_output, ARRAY_DIM((pcm_right_channel)), pcm_right_channel},
};

static const chain_config_t a2dp_chain_config =
        MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_audio_input_a2dp, audio_ucid_input_a2dp, op_config, streams);

static const chain_config_t a2dp_hq_chain_config =
        MAKE_CHAIN_CONFIG_WITH_STREAMS(chain_id_audio_input_a2dp, audio_ucid_input_a2dp_hq_codec, op_config, streams);

#define NUMBER_OF_FILTERS 2

static operator_config_t filter_list[NUMBER_OF_FILTERS];
static const operator_filters_t a2dp_filters =
{
    .num_operator_filters = NUMBER_OF_FILTERS,
    .operator_filters = filter_list
};

const chain_config_t *AudioInputA2dpGetChainConfig(audio_codec_t decoder)
{
    switch(decoder)
    {
        case audio_codec_aptx:
        case audio_codec_aptx_ll:
        case audio_codec_aptx_hd:
            return &a2dp_hq_chain_config;
        
        default:
            return &a2dp_chain_config;
    }
}

static capability_id_t getCapIdFromCodecType(audio_codec_t decoder)
{
    capability_id_t cap_id = capability_id_none;

    switch(decoder)
    {
        case audio_codec_sbc:
            cap_id = capability_id_sbc_decoder;
            break;
        case audio_codec_aptx:
            cap_id = capability_id_aptx_decoder;
            break;
        case audio_codec_aptx_ll:
            cap_id = capability_id_aptx_ll_decoder;
            break;
        case audio_codec_aac:
            cap_id = capability_id_aac_decoder;
            break;
        case audio_codec_aptx_hd:
            cap_id = capability_id_aptx_hd_decoder;
            break;
        default:
            Panic();
    }

    return cap_id;
}

static void setFilter(operator_config_t *filter, const operator_config_t *base_config, capability_id_t cap_id)
{
    /* First copy all fields form base configuration,
       otherwise those fields will be 0 or random, which is not good. */
    memcpy(filter, base_config, sizeof(operator_config_t));

    filter->capability_id = cap_id;
}

static const operator_config_t *findConfigForRole(unsigned role)
{
    unsigned i;

    for(i = 0; i < ARRAY_DIM(op_config); ++i)
    {
        if(op_config[i].role == role)
        {
            return &op_config[i];
        }
    }

    Panic();
    return NULL;
}

const operator_filters_t *AudioInputA2dpGetChainConfigFilter(audio_codec_t decoder, chain_variant_t variant)
{
    setFilter(&filter_list[0], findConfigForRole(decoder_role), getCapIdFromCodecType(decoder));
    setFilter(&filter_list[1], findConfigForRole(splitter_role),
            variant == chain_variant_with_splitter ? capability_id_splitter : capability_id_none);

    return &a2dp_filters;
}
