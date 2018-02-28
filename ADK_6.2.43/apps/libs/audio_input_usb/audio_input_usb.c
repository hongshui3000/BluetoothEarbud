/****************************************************************************
Copyright (c) 2016 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    audio_input_usb.c

DESCRIPTION
    Message dispatcher.
*/
#include <stdlib.h>
#include <audio.h>
#include <audio_plugin_if.h>
#include <audio_input_common.h>
#include <audio_plugin_music_variants.h>
#include <vmtypes.h>
#include <print.h>

#include "audio_input_usb.h"
#include "audio_input_usb_broadcast.h"
#include "audio_input_usb_connect.h"

static audio_input_context_t* usb_ctx = NULL;

const A2dpPluginTaskdata csr_usb_decoder_plugin = {{AudioPluginUsbMessageHandler}, USB_DECODER, BITFIELD_CAST(8, 0)};
const A2dpPluginTaskdata csr_ba_usb_decoder_plugin = {{AudioPluginUsbMessageHandler}, BA_USB_DECODER, BITFIELD_CAST(8, 0)};


static void cleanContextData(void)
{
    free(usb_ctx);
    usb_ctx = NULL;
}

static void destroyUsbCtx(Task task, Message message, audio_input_context_t* ctx)
{
    UNUSED(task);
    UNUSED(message);
    UNUSED(ctx);
    cleanContextData();
}

static void started(Task task, Message message, audio_input_context_t* ctx)
{
    ChainStart(ctx->chain);
    AudioInputCommonForwardHandleStartCfm(task, message, ctx);
}

static void disconnected(Task task, Message message, audio_input_context_t* ctx)
{
    AudioInputCommonFadeOutCompleteHandler(task, message, ctx);
    cleanContextData();
}

static void audioInputUsbHandleForwardingCreate(Task task, Message message, audio_input_context_t* ctx)
{
    if (AudioInputCommonTaskIsBroadcaster(task))
    {
        audioInputUsbBroadcastStart(task, message, ctx);
    }
    else
    {
        AudioInputCommonForwardHandleCreateCfm(task, message, ctx);
    }
}

static void audioInputUsbHandleForwardingStart(Task task, Message message, audio_input_context_t* ctx)
{
    if (AudioInputCommonTaskIsBroadcaster(task))
    {
        audioInputUsbBroadcastStartChain(task, message, ctx);
    }
    else
    {
        started(task, message, ctx);
    }
}

static void audioInputUsbHandleForwardingDestroy(Task task, Message message, audio_input_context_t* ctx)
{
    if (AudioInputCommonTaskIsBroadcaster(task))
    {
        audioInputUsbBroadcastDestroy(task, ctx->ba.plugin, ctx);
    }
    else
    {
        AudioInputCommonForwardDestroy(task, message, ctx);
    }
}

static void audioInputUsbHandleDisconnectReq(Task task, Message message, audio_input_context_t* ctx)
{
    if (AudioInputCommonTaskIsBroadcaster(task))
    {
        audioInputUsbBroadcastStop(task, ctx);
    }
    else
    {
        AudioInputCommonDisconnectHandler(task, message, ctx);
    }
}

static const audio_input_state_table_t usb_state_table = 
{
    {
        [audio_input_idle] = {[audio_input_connect_req] = AudioInputUsbConnectHandler},
        [audio_input_connecting] = {[audio_input_connect_complete] = AudioInputCommonFadeInCompleteHandler, [audio_input_forward_created] = audioInputUsbHandleForwardingCreate, [audio_input_forward_started] = audioInputUsbHandleForwardingStart},
        [audio_input_connected] = {[audio_input_disconnect_req] = audioInputUsbHandleDisconnectReq, [audio_input_forward_req] = AudioInputCommonForwardStart},
        [audio_input_disconnecting] = {[audio_input_disconnect_complete] = disconnected, [audio_input_forward_stopped] = audioInputUsbHandleForwardingDestroy, [audio_input_forward_destroyed] = disconnected},
        [audio_input_forwarding_setup] = {[audio_input_forward_created] = audioInputUsbHandleForwardingCreate, [audio_input_forward_started] = audioInputUsbHandleForwardingStart, [audio_input_connect_complete] = AudioInputCommonFadeInCompleteHandler, [audio_input_error] = AudioInputCommonFadeOutCompleteHandler},
        [audio_input_forwarding] = {[audio_input_forward_stop_req] = AudioInputCommonForwardStop, [audio_input_disconnect_req] = audioInputUsbHandleDisconnectReq},
        [audio_input_forwarding_tear_down] = {[audio_input_forward_stopped] = audioInputUsbHandleForwardingDestroy, [audio_input_forward_destroyed] = audioInputUsbConnectToMixer, [audio_input_connect_complete] = AudioInputCommonFadeInCompleteHandler},
        [audio_input_forwarding_disconnect] = {[audio_input_disconnect_complete] = AudioInputCommonForwardStop, [audio_input_forward_stopped] = audioInputUsbHandleForwardingDestroy, [audio_input_forward_destroyed] = disconnected},
        [audio_input_error_state] = {[audio_input_disconnect_req] = destroyUsbCtx}
    }
};

static void setUpStateHandlers(void)
{
    usb_ctx = calloc(1, sizeof(audio_input_context_t));
    AudioInputCommonSetStateTable(&usb_state_table, usb_ctx);
}

void AudioPluginUsbMessageHandler(Task task, MessageId id, Message message)
{
    switch(id)
    {
        case AUDIO_PLUGIN_CONNECT_MSG:
        {
            PanicNotNull(usb_ctx);
            setUpStateHandlers();
            break;
        }

        case AUDIO_PLUGIN_TEST_RESET_MSG:
        {
            cleanContextData();
            return;
        }

        case AUDIO_PLUGIN_SET_GROUP_VOLUME_MSG:
        {
            if (AudioInputCommonTaskIsBroadcaster(task))
            {
                /* Broadcast the volume change to all receivers. */
                AudioPluginForwardingVolumeChangeInd(usb_ctx->ba.plugin);
            }
            break;
        }

        default:
        {
            break;
        }
    }

    AudioInputCommonMessageHandler(task, id, message, usb_ctx);
}

uint32 AudioGetUsbSampleRate(void)
{
    if(usb_ctx)
        return usb_ctx->sample_rate;
    
    return 0;
}
