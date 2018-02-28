/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_plugin_mic.c
    
DESCRIPTION
    Implementation of audio plugin microphone functions for the default
    configuration of the audio plugin common library.
*/

#include "audio_plugin_common.h"

/****************************************************************************
DESCRIPTION
    Indicate that a microphone has been connected
*/
void AudioPluginMicConnected(audio_channel channel, const audio_mic_params * params)
{
    /* Nothing to be done for the default configuration */
    UNUSED(channel);
    UNUSED(params);
}

/****************************************************************************
DESCRIPTION
    Indicate that a microphone has been disconnected
*/
void AudioPluginMicDisconnected(audio_channel channel, const audio_mic_params * params)
{
    /* Nothing to be done for the default configuration */
    UNUSED(channel);
    UNUSED(params);
}

