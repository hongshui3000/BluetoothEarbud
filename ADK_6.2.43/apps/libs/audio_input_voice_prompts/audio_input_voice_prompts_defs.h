/****************************************************************************
Copyright (c) 2016 Qualcomm Technologies International, Ltd.
Part of 6.2
 
FILE NAME
    audio_input_voice_prompts_defs.h
 
DESCRIPTION
    Definitaion of Voice Prompts Context Structure 
*/

#ifndef LIBS_AUDIO_INPUT_VOICE_PROMPTS_AUDIO_INPUT_VOICE_PROMPTS_DEFS_H_
#define LIBS_AUDIO_INPUT_VOICE_PROMPTS_AUDIO_INPUT_VOICE_PROMPTS_DEFS_H_

#include "audio_plugin_if.h"

typedef struct
{
    const ringtone_note*    tone;
    FILE_INDEX              prompt_index;
    FILE_INDEX              prompt_header_index;
    voice_prompts_codec_t   codec_type;
    bool                    stereo;
    uint16                  playback_rate;
    int16                   prompt_volume;
    AudioPluginFeatures     features;
    Source                  source;
    Task                    app_task;
} vp_context_t ;

#endif /* LIBS_AUDIO_INPUT_VOICE_PROMPTS_AUDIO_INPUT_VOICE_PROMPTS_DEFS_H_ */
