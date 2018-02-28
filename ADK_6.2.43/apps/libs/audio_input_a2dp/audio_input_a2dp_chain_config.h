/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_a2dp_chain_config.h

DESCRIPTION
    Types used for chain configuration
*/

#ifndef AUDIO_INPUT_A2DP_CHAIN_CONFIG_H_
#define AUDIO_INPUT_A2DP_CHAIN_CONFIG_H_

#include <chain.h>
#include <audio_data_types.h>


enum
{
    rtp_role,
    splitter_role,
    decoder_role
};

typedef enum
{
    stream_encoded,
    stream_forwarding,
    stream_pcm_left,
    stream_pcm_right
} stream_channel_t;

typedef enum
{
    chain_variant_with_splitter,
    chain_variant_without_splitter
} chain_variant_t;

/****************************************************************************
DESCRIPTION
    Return the chain configuration.
*/
const chain_config_t* AudioInputA2dpGetChainConfig(audio_codec_t decoder);
const operator_filters_t *AudioInputA2dpGetChainConfigFilter(audio_codec_t decoder, chain_variant_t variant);

#endif /* AUDIO_INPUT_A2DP_CHAIN_CONFIG_H_ */
