/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_plugin_mic.c
    
DESCRIPTION
    Implementation of audio plugin microphone functions for the hydracore
    configuration of the audio plugin common library.
*/

#include "audio_plugin_common.h"
#include <audio_processor.h>

/****************************************************************************
DESCRIPTION
    Indicate that a microphone has been connected
*/
void AudioPluginMicConnected(audio_channel channel, const audio_mic_params * params)
{
    UNUSED(channel);
    if (params->is_digital)
    {
        /* Only interested if the microphone is a digital microphone */
        AudioProcessorAddUseCase(audio_ucid_input_digital);
    }
}

/****************************************************************************
DESCRIPTION
    Indicate that a microphone has been disconnected
*/
void AudioPluginMicDisconnected(audio_channel channel, const audio_mic_params * params)
{
    UNUSED(channel);
    if (params->is_digital)
    {
        /* Only interested if the microphone is a digital microphone */
        AudioProcessorRemoveUseCase(audio_ucid_input_digital);
    }
}

