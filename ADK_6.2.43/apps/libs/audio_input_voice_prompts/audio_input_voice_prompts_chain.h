/****************************************************************************
Copyright (c) 2016 - 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_voice_prompts_chain.h

DESCRIPTION
    Module talking directly to the Kymera.
    Chain is a set of connected the Kymera operators.

    Sequence of events is:
    1. Turn on DSP
    2. Instantiate a chain
    3. Start a chain
    4. Wait for some event
    5. Stop a chain
    6. Destroy a chain
    7. Turn off DSP
*/

#ifndef LIBS_AUDIO_INPUT_VOICE_PROMPTS_AUDIO_INPUT_VOICE_PROMPTS_CHAIN_H_
#define LIBS_AUDIO_INPUT_VOICE_PROMPTS_AUDIO_INPUT_VOICE_PROMPTS_CHAIN_H_

#include <csrtypes.h>
#include <sink.h>
#include <source.h>
#include <audio_plugin_if.h>


typedef struct tones_data_t* vp_handle_t;

/****************************************************************************
DESCRIPTION
    Create, setup and make internal connections for tone playing chain.
    Task is where tone end notification will be sent.
*/
vp_handle_t VoicePromptsChainConnectTone(const ringtone_note* tone);


/****************************************************************************
DESCRIPTION
    Create, setup and make internal connections for voice prompt playing chain.
*/
vp_handle_t VoicePromptsChainConnectPrompt(vp_context_t *context);

/****************************************************************************
DESCRIPTION
    Disconnect and destroy chain and associated resources.
*/
void VoicePromptsChainDisconnect(vp_handle_t vp_handle);

/****************************************************************************
DESCRIPTION
    Start the chain
*/
void VoicePromptsChainStart(vp_handle_t vp_handle);

/****************************************************************************
DESCRIPTION
    Stop the chain
*/
void VoicePromptsChainStop(vp_handle_t vp_handle);

#endif /* LIBS_AUDIO_INPUT_VOICE_PROMPTS_AUDIO_INPUT_VOICE_PROMPTS_CHAIN_H_ */
