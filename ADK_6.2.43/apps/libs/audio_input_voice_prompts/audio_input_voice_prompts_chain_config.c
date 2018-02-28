/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_input_voice_prompts_chain_config.c

DESCRIPTION
    Implementation of function to return the appropriate chain configuration data.
*/

#include <operators.h>
#include <panic.h>
#include "audio_input_voice_prompts_chain_config.h"

static const operator_config_t tone_op_config[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_tone, tone_role)
};

static const operator_endpoint_t tone_outputs[] =
{
    {tone_role, vp_ep_output_left, 0},
};

static const chain_config_t voice_prompts_tone_chain_config =
    MAKE_CHAIN_CONFIG_NO_INPUTS_NO_CONNECTIONS(chain_id_audio_input_voice_prompts_tone, 
        audio_ucid_input_tone, tone_op_config, tone_outputs);

static const operator_config_t decoder_op_config[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_sbc_decoder, decoder_role)
};

static const operator_endpoint_t decoder_inputs[] =
{
    {decoder_role, vp_ep_input, 0},
};

static const operator_endpoint_t decoder_outputs[] =
{
    {decoder_role, vp_ep_output_left, 0},
    {decoder_role, vp_ep_output_right, 1},
};

static const chain_config_t voice_prompts_decoder_chain_config =
    MAKE_CHAIN_CONFIG_NO_CONNECTIONS(chain_id_audio_input_voice_prompts_decoder,
        audio_ucid_input_voice_prompts_decoder, decoder_op_config, decoder_inputs, decoder_outputs);

static const operator_config_t dummy_op_config[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_passthrough, dummy_role)
};

static const operator_endpoint_t dummy_inputs[] =
{
    {dummy_role, vp_ep_input, 0},
};

static const operator_endpoint_t dummy_outputs[] =
{
    {dummy_role, vp_ep_output_left, 0},
};

static const chain_config_t voice_prompts_dummy_chain_config =
    MAKE_CHAIN_CONFIG_NO_CONNECTIONS(chain_id_audio_input_voice_prompts_decoder,
        audio_ucid_input_voice_prompts_pcm, dummy_op_config, dummy_inputs, dummy_outputs);

const chain_config_t* AudioInputVoicePromptsToneGetChainConfig(void)
{
    return &voice_prompts_tone_chain_config;
}

const chain_config_t* AudioInputVoicePromptsDecoderGetChainConfig(void)
{
    return &voice_prompts_decoder_chain_config;
}

const chain_config_t* AudioInputVoicePromptsDummyGetChainConfig(void)
{
    return &voice_prompts_dummy_chain_config;
}
