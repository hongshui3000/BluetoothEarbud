/****************************************************************************
Copyright (c) 2016 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_i2s.c

DESCRIPTION
    Message dispatcher.
*/

#include <stdlib.h>

#include <audio_plugin_if.h>
#include <audio_plugin_music_variants.h>
#include <audio_input_common.h>

#include "audio_input_i2s.h"
#include "audio_input_i2s_connect.h"

static audio_input_context_t *i2s_ctx = 0;

const A2dpPluginTaskdata csr_i2s_decoder_plugin = {{AudioPluginI2sMessageHandler}, I2S_DECODER, BITFIELD_CAST(8, 0)};

static void cleanContextData(void)
{
    free(i2s_ctx);
    i2s_ctx = 0;
}

static void destroyI2sCtx(Task task, Message message, audio_input_context_t* ctx)
{
    UNUSED(task);
    UNUSED(message);
    UNUSED(ctx);
    cleanContextData();
}

static void start(Task task, Message message, audio_input_context_t* ctx)
{
    AudioInputCommonForwardStart(task, message, ctx);
    ChainDestroy(ctx->chain);
    ctx->chain = NULL;
}

static void disconnected(Task task, Message message, audio_input_context_t* ctx)
{
    AudioInputI2sDisconnect(task, message, ctx);
    cleanContextData();
}

static const audio_input_state_table_t i2s_state_table = 
{
    {
        [audio_input_idle] = {[audio_input_connect_req] = AudioInputI2sConnectHandler},
        [audio_input_connecting] = {[audio_input_connect_complete] = AudioInputCommonFadeInCompleteHandler},
        [audio_input_connected] = {[audio_input_disconnect_req] = AudioInputCommonDisconnectHandler, [audio_input_forward_req] = start},
        [audio_input_disconnecting] = {[audio_input_disconnect_complete] = disconnected},
        [audio_input_forwarding_setup] = {[audio_input_forward_created] = AudioInputCommonForwardHandleCreateCfm, [audio_input_forward_started] = AudioInputCommonForwardHandleStartCfm, [audio_input_connect_complete] = AudioInputCommonFadeInCompleteHandler, [audio_input_error] = AudioInputCommonFadeOutCompleteHandler},
        [audio_input_forwarding] = {[audio_input_forward_stop_req] = AudioInputCommonForwardStop, [audio_input_disconnect_req] = AudioInputCommonDisconnectHandler},
        [audio_input_forwarding_tear_down] = {[audio_input_forward_stopped] = AudioInputCommonForwardDestroy, [audio_input_forward_destroyed] = AudioInputI2sConnect, [audio_input_connect_complete] = AudioInputCommonFadeInCompleteHandler},
        [audio_input_forwarding_disconnect] = {[audio_input_disconnect_complete] = AudioInputCommonForwardStop, [audio_input_forward_stopped] = AudioInputCommonForwardDestroy, [audio_input_forward_destroyed] = disconnected},
        [audio_input_error_state] = {[audio_input_disconnect_req] = destroyI2sCtx}
    }
};

static void setUpStateHandlers(void)
{
    i2s_ctx = calloc(1, sizeof(audio_input_context_t));
    AudioInputCommonSetStateTable(&i2s_state_table, i2s_ctx);
}

void AudioPluginI2sMessageHandler(Task task, MessageId id, Message message)
{
    switch(id)
    {
        case AUDIO_PLUGIN_CONNECT_MSG:
        {
            setUpStateHandlers();
            break;
        }
        
        case AUDIO_PLUGIN_TEST_RESET_MSG:
        {
            cleanContextData();
            return;
        }
        
        default:
        {
            break;
        }
    }
    
    AudioInputCommonMessageHandler(task, id, message, i2s_ctx);
}

