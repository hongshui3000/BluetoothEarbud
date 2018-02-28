/*******************************************************************************
Copyright (c) 2016 Qualcomm Technologies International, Ltd.

FILE NAME
    audio_mixer_chain_interconnections.c
DESCRIPTION
    Defines the interconnections between the various audio mixer chains.
NOTES
*/

#include <audio_output.h>
#include <chain.h>
#include <panic.h>
#include <stream.h>
#include <transform.h>

#include "audio_mixer.h"
#include "audio_mixer_core.h"
#include "audio_mixer_input.h"
#include "audio_mixer_resampler.h"
#include "audio_mixer_processing.h"
#include "audio_mixer_speaker.h"
#include "audio_mixer_chain_interconnections.h"
#include "audio_mixer_channel_roles.h"

/*******************************************************************************/
bool mixerChainJoin(kymera_chain_handle_t src_chain, kymera_chain_handle_t dest_chain)
{
    audio_mixer_channel_role_t channel;
    bool connection_made = FALSE;
    
    PanicNull(src_chain);
    PanicNull(dest_chain);

    for (channel = left_channel; channel <= prompt_right_channel; channel++)
    {
        Source source = ChainGetOutput(src_chain, channel);
        Sink sink = ChainGetInput(dest_chain, channel);
        if(source && sink)
        {
            if(StreamConnect(source, sink))
                connection_made = TRUE;
        }
    }
    
    return connection_made;
}

/******************************************************************************/
static void getInputChains(audio_mixer_input_t input, kymera_chain_handle_t* resampler_chain, kymera_chain_handle_t* processing_chain)
{
    mixer_resampler_context_t resampler_context   = mixerInputGetResampler(input);
    mixer_processing_context_t processing_context = mixerInputGetProcessing(input);
    
    *resampler_chain  = mixerResamplerGetChain(resampler_context);
    *processing_chain = mixerProcessingGetChain(processing_context);
}

/******************************************************************************/
static kymera_chain_handle_t getInputChainForInput(audio_mixer_input_t input)
{
    kymera_chain_handle_t resampler_chain;
    kymera_chain_handle_t processing_chain;
    getInputChains(input, &resampler_chain, &processing_chain);
    
    if(resampler_chain)
        return resampler_chain;
    
    return processing_chain;
}

/******************************************************************************/
static kymera_chain_handle_t getMidChainForInput(audio_mixer_input_t input)
{
    kymera_chain_handle_t resampler_chain;
    kymera_chain_handle_t processing_chain;
    getInputChains(input, &resampler_chain, &processing_chain);
    
    if(resampler_chain)
        return processing_chain;
    
    return NULL;
}

/******************************************************************************/
static kymera_chain_handle_t getOutputChainForInput(audio_mixer_input_t input)
{
    kymera_chain_handle_t output_chain = mixerCoreGetChain();

    if(input == audio_mixer_input_tones)
        output_chain = mixerSpeakerGetChain();
    
    return output_chain;
}

/******************************************************************************/
static audio_mixer_channel_role_t getRoleForInput(audio_mixer_input_t input, audio_mixer_channel_role_t channel)
{
    switch(input)
    {
        case audio_mixer_input_music:
        {
            if(channel == left_channel)
                return music_left_channel;
            return music_right_channel;
        }
        
        case audio_mixer_input_voice:
        {
            return voice_channel;
        }
        
        case audio_mixer_input_tones:
        {
            if(channel == left_channel)
                return prompt_left_channel;
            return prompt_right_channel;
        }
        
        default:
        {
            Panic();
            return channel;
        }
    }
}

/******************************************************************************/
static void mixerChainConnectSource(kymera_chain_handle_t input_chain, audio_mixer_input_t input, audio_mixer_channel_role_t channel, Source source)
{
    PanicNull(StreamConnect(source, ChainGetInput(input_chain, getRoleForInput(input, channel))));
}

/******************************************************************************/
bool mixerChainConnectInput(audio_mixer_input_t input, Source left, Source right)
{
    kymera_chain_handle_t input_chain  = getInputChainForInput(input);
    kymera_chain_handle_t mid_chain    = getMidChainForInput(input);
    kymera_chain_handle_t output_chain = getOutputChainForInput(input);
    
    if(mid_chain)
    {
        /* Two input chains (resampler and processing) */
        if(!mixerChainJoin(input_chain, mid_chain))
            return FALSE;
        
        if(!mixerChainJoin(mid_chain, output_chain))
            return FALSE;
    }
    else if(input_chain)
    {
        /* Only one input chain (resampler or processing) */
        if(!mixerChainJoin(input_chain, output_chain))
            return FALSE;
    }
    else
    {
        /* No input chains (no resampler or processing) */
        input_chain = output_chain;
    }
    
    mixerChainConnectSource(input_chain, input, left_channel, left);
    
    if(right)
        mixerChainConnectSource(input_chain, input, right_channel, right);
    
    return TRUE;
}
