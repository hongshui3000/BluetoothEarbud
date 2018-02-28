/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_voice_prompts_chain_config.h

DESCRIPTION
    Voice prompts audio input chain configuration.
*/

#ifndef AUDIO_INPUT_VOICE_PROMPTS_CHAIN_CONFIG_H_
#define AUDIO_INPUT_VOICE_PROMPTS_CHAIN_CONFIG_H_

#include <chain.h>

typedef enum
{
    tone_role,
    decoder_role,
    dummy_role
} usb_operator_roles_t;

typedef enum
{
    vp_ep_input,
    vp_ep_output_left,
    vp_ep_output_right,
} vp_endpoint_role_t;

/****************************************************************************
DESCRIPTION
    Return the chain configuration.
*/
const chain_config_t* AudioInputVoicePromptsToneGetChainConfig(void);
const chain_config_t* AudioInputVoicePromptsDecoderGetChainConfig(void);
const chain_config_t* AudioInputVoicePromptsDummyGetChainConfig(void);

#endif /* AUDIO_INPUT_VOICE_PROMPTS_CHAIN_CONFIG_H_ */
