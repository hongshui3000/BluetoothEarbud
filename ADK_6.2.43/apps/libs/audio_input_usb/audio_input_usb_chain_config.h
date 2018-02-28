/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_usb_chain_config.h

DESCRIPTION
    USB audio input chain configuration.
*/

#ifndef AUDIO_INPUT_USB_CHAIN_CONFIG_H_
#define AUDIO_INPUT_USB_CHAIN_CONFIG_H_

#include <chain.h>

enum
{
    usb_rx_role,
};

typedef enum
{
    audio_input_pcm,
    audio_output_left,
    audio_output_right,
} chain_endpoint_role_t;

/****************************************************************************
DESCRIPTION
    Return the chain configuration.
*/
const chain_config_t* AudioInputUsbGetChainConfig(void);


#endif /* AUDIO_INPUT_USB_CHAIN_CONFIG_H_ */
