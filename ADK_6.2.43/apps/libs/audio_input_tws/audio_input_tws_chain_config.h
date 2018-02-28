/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_tws_chain_config.h

DESCRIPTION
    TWS audio input chain configuration.
*/

#ifndef AUDIO_INPUT_TWS_CHAIN_CONFIG_H_
#define AUDIO_INPUT_TWS_CHAIN_CONFIG_H_

#include <chain.h>

typedef enum
{
    audio_input_tws_sbc,
    audio_input_tws_aptx,
    audio_input_tws_aac
} audio_input_tws_decoder_t;

enum
{
    passthrough_role,
    decoder_role
};

typedef enum
{
    encoded_audio_input,
    decoded_audio_output_left,
    decoded_audio_output_right
} chain_endpoint_role_t;

/****************************************************************************
DESCRIPTION
    Return the chain configuration.
*/
const chain_config_t* AudioInputTwsGetChainConfig(audio_input_tws_decoder_t decoder);


#endif /* AUDIO_INPUT_TWS_CHAIN_CONFIG_H_ */
