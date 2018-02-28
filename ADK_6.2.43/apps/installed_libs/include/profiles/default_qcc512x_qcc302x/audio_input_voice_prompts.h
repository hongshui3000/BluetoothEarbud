/****************************************************************************
Copyright (c) 2016 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_audio_prompts.h

DESCRIPTION
    Audio plugin for voice prompts source.
*/

#ifndef LIBS_AUDIO_INPUT_VOICE_PROMPTS_AUDIO_INPUT_VOICE_PROMPTS_H_
#define LIBS_AUDIO_INPUT_VOICE_PROMPTS_AUDIO_INPUT_VOICE_PROMPTS_H_

#include <message.h>

/*the task message handler*/
void AudioInputVoicePromptsMessageHandler (Task task, MessageId id, Message message);

void AudioInputVoicePromptsPluginTestReset(void);

#endif /* LIBS_AUDIO_INPUT_VOICE_PROMPTS_AUDIO_INPUT_VOICE_PROMPTS_H_ */
