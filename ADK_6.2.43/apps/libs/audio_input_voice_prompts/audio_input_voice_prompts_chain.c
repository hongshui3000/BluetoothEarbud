/****************************************************************************
Copyright (c) 2016 - 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_voice_prompts_chain.c

DESCRIPTION
    Module talking directly to the Kymera.
*/
#include <operator.h>
#include <stream.h>
#include <panic.h>
#include <vmal.h>
#include <stdlib.h>

#include <print.h>
#include <operators.h>
#include <chain.h>
#include <audio_mixer.h>
#include <audio_plugin_if.h>

#include "audio_input_voice_prompts_defs.h"
#include "audio_input_voice_prompts_chain.h"
#include "audio_input_voice_prompts_utils.h"
#include "audio_input_voice_prompts_chain_config.h"

#define TONE_SAMPLE_RATE 8000

typedef struct vp_data_t
{
    kymera_chain_handle_t chain;
    audio_mixer_input_t mixer_input;
} vp_data_t;

extern const TaskData csr_voice_prompts_plugin;

/****************************************************************************
DESCRIPTION
    Determine if a decoder is required to decode the voice prompt
*/
static bool isDecoderRequired(voice_prompts_codec_t codec_type)
{
    bool decoderRequired = FALSE;

    switch(codec_type)
    {
        case voice_prompts_codec_pcm:
        case voice_prompts_codec_pcm_8khz:
            decoderRequired = FALSE;
        break;

        case voice_prompts_codec_sbc:
            decoderRequired = TRUE;
        break;

        case voice_prompts_codec_ima_adpcm:
        case voice_prompts_codec_mp3:
        case voice_prompts_codec_aac:
        case voice_prompts_codec_tone:
        default:
            Panic();
    }

    return decoderRequired;
}

/****************************************************************************
DESCRIPTION
    Create the chain required to play tones
*/
static kymera_chain_handle_t createToneChain(const ringtone_note* tone)
{
    kymera_chain_handle_t chain;
    Operator tone_op;
    const chain_config_t *config = AudioInputVoicePromptsToneGetChainConfig();
    chain = PanicNull(ChainCreate(config));

    tone_op = ChainGetOperatorByRole(chain, tone_role);
    OperatorsConfigureToneGenerator(tone_op, tone, (Task)&csr_voice_prompts_plugin);

    return chain;
}

/****************************************************************************
DESCRIPTION
    Create the chain required to play prompts that require decoding
*/
static kymera_chain_handle_t createPromptDecoderChain(void)
{
    const chain_config_t *config = AudioInputVoicePromptsDecoderGetChainConfig();
    kymera_chain_handle_t chain = PanicNull(ChainCreate(config));

    return chain;
}

/****************************************************************************
DESCRIPTION
    Create the dummy chain required to play PCM prompts
*/
static kymera_chain_handle_t createPromptDummyChain(void)
{
    const chain_config_t *config = AudioInputVoicePromptsDummyGetChainConfig();
    kymera_chain_handle_t chain = PanicNull(ChainCreate(config));

    return chain;
}

/****************************************************************************
DESCRIPTION
    Connect supplied sources to audio mixer
*/
static audio_mixer_input_t connectToMixer(vp_data_t* vp_data, unsigned input_sample_rate, bool stereo)
{
    audio_mixer_connect_t connect_data;
    audio_mixer_input_t input;

    connect_data.left_src = ChainGetOutput(vp_data->chain, vp_ep_output_left);
    connect_data.right_src = stereo ? ChainGetOutput(vp_data->chain, vp_ep_output_right) : NULL;
    connect_data.connection_type = CONNECTION_TYPE_TONES;
    connect_data.sample_rate = input_sample_rate;
    connect_data.channel_mode = CHANNEL_MODE_STEREO;
    connect_data.variable_rate = FALSE;

    input = AudioMixerConnect(&connect_data);

    PanicFalse(input != audio_mixer_input_error_none);

    return input;
}

/****************************************************************************
DESCRIPTION
    Create, setup and make internal connections for tone playing chain.
    Task is where tone end notification will be sent.
*/
vp_handle_t VoicePromptsChainConnectTone(const ringtone_note* tone)
{
    vp_data_t* vp_data = PanicUnlessNew(vp_data_t);

    vp_data->chain = createToneChain(tone);

    vp_data->mixer_input = connectToMixer(vp_data, TONE_SAMPLE_RATE, FALSE);

    return (vp_handle_t)vp_data;
}

/****************************************************************************
DESCRIPTION
    Create the required voice prompt chain
*/
static void voicePromptsChainCreate(vp_data_t* vp_data, voice_prompts_codec_t codec_type)
{
    if (isDecoderRequired(codec_type))
    {
        vp_data->chain = createPromptDecoderChain();
    }
    else
    {
        /* Need a dummy chain to register for message for when prompt completes */
        /* Can we register for message from file source instead? Then passthrough operator
         * will not be required. */
        vp_data->chain = createPromptDummyChain();
    }
}

/****************************************************************************
DESCRIPTION
    Create, setup and make internal connections for voice prompt playing chain.
*/
vp_handle_t VoicePromptsChainConnectPrompt(vp_context_t *context)
{
    Sink sink;
    vp_data_t* vp_data = PanicUnlessNew(vp_data_t);

    PRINT(("VoicePromptsChainInstantiateForType 0x%x, %u\n", context->codec_type, context->playback_rate));

    voicePromptsChainCreate(vp_data, context->codec_type);

    vp_data->mixer_input = connectToMixer(vp_data, context->playback_rate, context->stereo);

    sink = ChainGetInput(vp_data->chain, vp_ep_input);
    VoicePromptsRegisterForMessagesFromSink(sink);
    PanicNull(StreamConnect(context->source, sink));

    return (vp_handle_t)vp_data;
}

/****************************************************************************
DESCRIPTION
    Disconnect and destroy chain and associated resources.
*/
void VoicePromptsChainDisconnect(vp_handle_t vp_handle)
{
    vp_data_t* vp_data = (vp_data_t*)vp_handle;
    Sink input_sink;

    if(vp_data && vp_data->chain)
    {
        input_sink = ChainGetInput(vp_data->chain, vp_ep_input);

        if(input_sink)
        {
            /* Cancel all the messages relating to VP that have been sent */
            VoicePromptsDeregisterForMessagesFromSink(input_sink);
        }

        AudioMixerDisconnect(vp_data->mixer_input);
        ChainDestroy(vp_data->chain);
    }

    free(vp_data);

    MessageCancelAll((TaskData*) &csr_voice_prompts_plugin, MESSAGE_STREAM_DISCONNECT);
}

/****************************************************************************
DESCRIPTION
    Start the vp chain
*/
void VoicePromptsChainStart(vp_handle_t vp_handle)
{
    vp_data_t* tones_data = (vp_data_t*)vp_handle;

    if (tones_data && tones_data->chain)
        ChainStart(tones_data->chain);
}


/****************************************************************************
DESCRIPTION
    Stop the vp chain
*/
void VoicePromptsChainStop(vp_handle_t vp_handle)
{
    vp_data_t* tones_data = (vp_data_t*)vp_handle;

    if (tones_data && tones_data->chain)
        ChainStop(tones_data->chain);
}

