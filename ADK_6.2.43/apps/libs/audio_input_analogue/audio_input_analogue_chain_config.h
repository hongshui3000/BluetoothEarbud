/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_analogue_chain_config.h

DESCRIPTION
    ANALOGUE audio input chain configuration.
*/

#ifndef AUDIO_INPUT_ANALOGUE_CHAIN_CONFIG_H_
#define AUDIO_INPUT_ANALOGUE_CHAIN_CONFIG_H_

#include <chain.h>

enum
{
    ttp_pass_role,
};

typedef enum
{
    audio_input_left,
    audio_input_right,
    local_audio_output_left,
    local_audio_output_right,
} chain_endpoint_role_t;

/****************************************************************************
DESCRIPTION
    Return the chain configuration.
*/
const chain_config_t* AudioInputAnalogueGetChainConfig(void);


#endif /* AUDIO_INPUT_ANALOGUE_CHAIN_CONFIG_H_ */
