/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_input_analogue_chain_config.c

DESCRIPTION
    Implementation of function to return the appropriate chain configuration data.
*/

#include <operators.h>
#include <panic.h>
#include "audio_input_analogue_chain_config.h"

static const operator_config_t op_config[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_ttp_passthrough, ttp_pass_role)
};

static const operator_endpoint_t inputs[] =
{
    {ttp_pass_role, audio_input_left, 0},
    {ttp_pass_role, audio_input_right, 1}
};

static const operator_endpoint_t outputs[] =
{
    {ttp_pass_role, local_audio_output_left, 0},
    {ttp_pass_role, local_audio_output_right, 1},
};

static const chain_config_t analogue_chain_config =
    MAKE_CHAIN_CONFIG_NO_CONNECTIONS(chain_id_audio_input_analogue,
        audio_ucid_input_analogue, op_config, inputs, outputs);

const chain_config_t* AudioInputAnalogueGetChainConfig(void)
{
    return &analogue_chain_config;
}
