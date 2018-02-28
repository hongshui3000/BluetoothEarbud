/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_input_usb_chain_config.c

DESCRIPTION
    Implementation of function to return the appropriate chain configuration data.
*/

#include <operators.h>
#include <panic.h>
#include "audio_input_usb_chain_config.h"

static const operator_config_t op_config[] =
{
    MAKE_OPERATOR_CONFIG(capability_id_usb_audio_rx, usb_rx_role)
};

static const operator_endpoint_t inputs[] =
{
    {usb_rx_role, audio_input_pcm, 0},
};

static const operator_endpoint_t outputs[] =
{
    {usb_rx_role, audio_output_left, 0},
    {usb_rx_role, audio_output_right, 1},
};

static const chain_config_t usb_chain_config =
    MAKE_CHAIN_CONFIG_NO_CONNECTIONS(chain_id_audio_input_usb,
        audio_ucid_input_digital, op_config, inputs, outputs);

const chain_config_t* AudioInputUsbGetChainConfig(void)
{
    return &usb_chain_config;
}
