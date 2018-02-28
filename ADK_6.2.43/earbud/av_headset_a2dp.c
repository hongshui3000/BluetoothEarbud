/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_a2dp.c
\brief      A2DP State Machine   
*/

/* Only compile if AV defined */
#ifdef INCLUDE_AV

#include <a2dp.h>
#include <avrcp.h>
#include <panic.h>
#include <connection.h>
#include <kalimba.h>
#include <kalimba_standard_messages.h>
#include <ps.h>
#include <audio.h>
#include <string.h>

#include "av_headset.h"
#include "av_headset_latency.h"

/*! Code assertion that can be checked at run time. This will cause a panic. */
#define assert(x) PanicFalse(x)

/*! Macro for simplifying creating messages */
#define MAKE_AV_MESSAGE(TYPE) \
    TYPE##_T *message = PanicUnlessNew(TYPE##_T);

#include "av_headset_log.h"


#if defined(APP_AUDIO_LEFT)
    /*! Application will always output left channel */
    #define APP_A2DP_AUDIO_MODE AUDIO_MODE_TWS_CONNECT_LEFT
#elif defined(APP_AUDIO_RIGHT)
    /*! Application will always output right channel */
    #define APP_A2DP_AUDIO_MODE AUDIO_MODE_TWS_CONNECT_RIGHT
#else
    /*! Application will output left channel if Master, right channel if Slave */
    #define APP_A2DP_AUDIO_MODE AUDIO_MODE_CONNECTED
#endif

/* Local Function Prototypes */
static void appA2dpSetState(avInstanceTaskData *theInst, avA2dpState a2dp_state);

/*! \brief Convert from Sink SEID to Source SEID

    This function takes a Sink SEID and attempts to convert it
    into the appropriate Source SEID.
*/    
static uint8 appA2dpConvertSeidFromSourceToSink(uint8 seid)
{
    if (seid == AV_SEID_SBC_SNK)
        return AV_SEID_SBC_MONO_TWS_SRC;
    else if (seid == AV_SEID_AAC_SNK)
        return AV_SEID_SBC_MONO_TWS_SRC;
    else if (seid == AV_SEID_APTX_SNK)
        return AV_SEID_APTX_MONO_TWS_SRC;
    else
        return AV_SEID_INVALID;		
}

/*! \brief Update UI to show streaming state

    This function updates the UI when streaming is active, the current SEID
    is checked to differentiate between SBC & MP3 streaming.
*/ 
static void appA2dpStreamingActiveUi(avInstanceTaskData *theInst)
{
    /* Call appropriate UI function */
    switch (theInst->a2dp_current_seid)
    {
        case AV_SEID_SBC_SNK:
        case AV_SEID_AAC_SNK:
        case AV_SEID_SBC_MONO_TWS_SNK:
            appUiAvStreamingActive();
            return;

        case AV_SEID_APTX_SNK:
        case AV_SEID_APTX_MONO_TWS_SNK:
            appUiAvStreamingActiveAptx();
            return;
    }
}

/*! \brief Update UI to show streaming inactive

    This function updates the UI when streaming becomes in active.
    If just calls the appropriate UI module function.
*/ 
static void appA2dpStreamingInactiveUi(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    appUiAvStreamingInactive();
}

/*! \brief Set default attributes

    Populate the supplied attributes structure with default attributes.

    This function should be called when retrieving attributes for a device 
    fails.

    \param[in,out] attributes   Attributes structure to update
*/
void appA2dpSetDefaultAttributes(appDeviceAttributes *attributes)
{
    PanicNull(attributes);

    attributes->supported_profiles &= ~(DEVICE_PROFILE_A2DP | DEVICE_PROFILE_AVRCP);
    attributes->a2dp_num_seids = 0;
    /* Set default volume as set in av_headset_config.h */
    const int rangeDb = appConfigMaxVolumedB() - appConfigMinVolumedB();
    attributes->a2dp_volume = (appConfigDefaultVolumedB() - appConfigMinVolumedB()) * 127 / rangeDb;
}

static void appA2dpVolumeAttributeStore(avInstanceTaskData *theInst)
{
    appDeviceAttributes attributes;

    /* Attempt to retrieve existing attributes */
    if (appDeviceFindBdAddrAttributes(&theInst->bd_addr, &attributes))
    {
        /* Copy volume */
        attributes.a2dp_volume = theInst->a2dp_volume;

        /* Store updated attributes */
        appDeviceSetAttributes(&theInst->bd_addr, &attributes);
    }
    else
    {
        Panic();
    }
}

/*! \brief Start streaming to slave

    This function checks if there is a A2DP source instance connected 
    to a slave, if so then we attempt to connect the media channel and start streaming.
    A lock is returned to allow the master A2DP instance to synchronise with
    the slave.
*/		
static uint16 *appA2dpStartSlave(avInstanceTaskData *theInst)
{
    /* Check if we are the A2DP sink */
    if (appA2dpIsSinkCodec(theInst))
    {		
        avInstanceTaskData *av_inst;
                        
        /* Check if there is another connected A2DP instance */
        av_inst = appAvInstanceFindA2dpState(theInst,
                                             A2DP_STATE_MASK_CONNECTED_SIGNALLING,
                                             A2DP_STATE_CONNECTED_SIGNALLING);

        DEBUG_LOGF("appA2dpStartSlave, other instance(%p)", av_inst);

        if (av_inst != NULL)
        {
            /* Connect media channel and start streaming on other instance */
            MAKE_AV_MESSAGE(AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ);
            message->seid = appA2dpConvertSeidFromSourceToSink(theInst->a2dp_current_seid);
            MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ);
            MessageSendConditionally(&av_inst->av_task, AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ, message,
                                     &appA2dpGetLock(av_inst));

#ifndef AV_DEBUG				
            /* Return lock of other instance */									 
            return &appA2dpGetLock(av_inst);
#else
            /* Debug mode is so slow, the AV source will timeout before the lock has been released
               by the slave instance, so don't return a lock, this will result in the Slave starting
               streaming sometime after the Master */
            return NULL;
#endif			
        }
    }
    
    /* Return with no lock */
    return NULL;
}

/*! \brief Stop streaming to slave 

    This function checks if there is a A2DP source instance connected 
    to a slave, if so then we attempt to stop streaming and  disconnect the media channel.
    A lock is returned to allow the master A2DP instance to synchronise with
    the slave.
*/		
static uint16 *appA2dpStopSlave(avInstanceTaskData *theInst)
{
    /* Check we are a A2DP sink */
    if (theInst && appA2dpIsSinkCodec(theInst))
    {
        /* Check if there is another connected A2DP source instance */
        avInstanceTaskData *theOtherInst;
        theOtherInst = appAvInstanceFindA2dpState(theInst,
                                                  A2DP_STATE_MASK_CONNECTED_MEDIA,
                                                  A2DP_STATE_CONNECTED_MEDIA);
        if (theOtherInst && appA2dpIsSourceCodec(theOtherInst))
        {
            /* Disconnect media channel on other instance */
            MessageCancelFirst(&theOtherInst->av_task, AV_INTERNAL_A2DP_DISCONNECT_MEDIA_REQ);
            MessageSendConditionally(&theOtherInst->av_task, AV_INTERNAL_A2DP_DISCONNECT_MEDIA_REQ, NULL,
                                     &appA2dpGetLock(theOtherInst));

            /* Return lock of other instance */
            return &appA2dpGetLock(theOtherInst);
        }
    }

    /* Return with no lock */
    return NULL;
}

/*! \brief Connect audio

    If this A2DP instance is a Sink then update the UI and connect the appropriate
    audio plug-in, enable relaying if there is another A2DP instance.
    
    If this A2DP instance is a Source then just enable relaying on the current
    audio plug-in.
*/		
static void appA2dpConnectAudio(avInstanceTaskData *theInst)
{    
    avInstanceTaskData *theOtherInst;
    DEBUG_LOGF("appA2dpConnectAudio(%p)", (void *)theInst);

    theInst->a2dp_media_sink = A2dpMediaGetSink(theInst->a2dp_device_id, theInst->a2dp_stream_id);

    /* Check if we are the A2DP sink */
    if (appA2dpIsSinkCodec(theInst))
    {
        /* Call appropriate UI function */
        appA2dpStreamingActiveUi(theInst);

        /* Start Kymera decoder chain */
        if (appKymeraA2dpStart(theInst->a2dp_device_id, theInst->a2dp_stream_id,
                                        theInst->a2dp_current_seid, theInst->a2dp_media_sink))
        {
            /* Get the settings we need from the configured CODEC settings */
            const a2dp_codec_settings *codec_settings = A2dpCodecGetSettings(theInst->a2dp_device_id, theInst->a2dp_stream_id);

            /* Send delay report to audio source if it supports it.  Delay report
             * units are 1/10ms, so multiply TWS_STANDARD_LATENCY_MS by 10 */
            if (codec_settings && codec_settings->codecData.latency_reporting)
                A2dpMediaAvSyncDelayRequest(theInst->a2dp_device_id, theInst->a2dp_current_seid, TWS_STANDARD_LATENCY_MS * 10);

            /* Adjust volume locally */
            appA2dpVolumeSet(theInst, theInst->a2dp_volume);

            /* Check if there is another connected media streaming A2DP instance */
            theOtherInst = appAvInstanceFindA2dpState(theInst,
                                                      A2DP_STATE_CONNECTED_MEDIA_STREAMING,
                                                      A2DP_STATE_CONNECTED_MEDIA_STREAMING);
            if (theOtherInst)
            {
                /* Enable Kymera forwarding chain */
                appKymeraA2dpStartForwarding(theOtherInst->a2dp_device_id, theOtherInst->a2dp_stream_id,
                                                      theOtherInst->a2dp_current_seid, theOtherInst->a2dp_media_sink);
            }
        }
    }
    else
    {
        /* Check if there is another connected media streaming A2DP instance */
        theOtherInst = appAvInstanceFindA2dpState(theInst,
                                                  A2DP_STATE_CONNECTED_MEDIA_STREAMING,
                                                  A2DP_STATE_CONNECTED_MEDIA_STREAMING);
        if (theOtherInst)
        {
            /* Enable Kymera forwarding chain */
            appKymeraA2dpStartForwarding(theInst->a2dp_device_id, theInst->a2dp_stream_id, theInst->a2dp_current_seid, theInst->a2dp_media_sink);
        }
    }
}

/*! \brief Disconnect audio

    If this A2DP instance is a Sink then make sure the Slave has stopped, update
    the UI and disconnect the appropriate audio plug-in, 
    
    If this A2DP instance is a Source then just disable relaying on the audio
    plug-in if it's still connected.
    
*/		
static void appA2dpDisconnectAudio(avInstanceTaskData *theInst)
{    
    DEBUG_LOGF("appA2dpDisconnectAudio(%p)", (void *)theInst);

    /* Check if we are the A2DP sink */
    if (appA2dpIsSinkCodec(theInst))
    {
        /* Stop UI indication */
        appA2dpStreamingInactiveUi(theInst);

        /* Stop streaming, this will also stop forwarding if it's active */
        appKymeraA2dpStop(theInst->a2dp_media_sink);
    }
    else
    {
        /* This is the A2DP source instance, so just stop forwarding */
        appKymeraA2dpStopForwarding();
    }
}

/*! \brief Enter 'connecting local' state

    The A2DP state machine has entered 'connecting local' state, set the
    'connect busy' flag and operation to serialise connect attempts and block
    and other operations on this instance.
*/    
static void appA2dpEnterConnectingLocal(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterConnectingLocal(%p)", (void *)theInst);
    
    /* Set operation lock */
    appA2dpSetLock(theInst, TRUE);

    /* Clear detach pending flag */
    theInst->detach_pending = FALSE;

    /* Set locally initiated flag */
    theInst->a2dp_local_initiated = TRUE;

    /* Request outgoing connection */
    A2dpSignallingConnectRequestWithTask(&theInst->bd_addr, &theInst->av_task);

    /* Play background paging tone */
    if (~theInst->a2dp_flags & A2DP_CONNECT_SILENT)
        appUiPagingStart();
}

/*! \brief Exit 'connecting local' state

    The A2DP state machine has exited 'connecting local' state, clear the
    'connect busy' flag and operaion lock to allow pending connection attempts 
    and any pending operations on this instance to proceed.
*/    
static void appA2dpExitConnectingLocal(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectingLocal(%p)", (void *)theInst);

    /* Clear operation lock */
    appA2dpSetLock(theInst, FALSE);

    /* Stop background paging tone */
    if (~theInst->a2dp_flags & A2DP_CONNECT_SILENT)
        appUiPagingStop();

    /* We have finished (successfully or not) attempting to connect, so
     * we can relinquish our lock on the ACL.  Bluestack will then close
     * the ACL when there are no more L2CAP connections */
    appConManagerReleaseAcl(&theInst->bd_addr);
}

/*! \brief Enter 'connecting remote' state

    The A2DP state machine has entered 'connecting remote' state, set the
    operation lock to block any other operations on this instance.
*/
static void appA2dpEnterConnectingRemote(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterConnectingRemote(%p)", (void *)theInst);

    /* Set operation lock */
    appA2dpSetLock(theInst, TRUE);

    /* Clear detach pending flag */
    theInst->detach_pending = FALSE;

    /* Clear locally initiated flag */
    theInst->a2dp_local_initiated = FALSE;
}

/*! \brief Exit 'connecting remote' state

    The A2DP state machine has exited 'connecting remote' state, clear the
    operation lock to allow any pending operations on this instance to proceed.
*/
static void appA2dpExitConnectingRemote(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectingRemote(%p)", (void *)theInst);

    /* Clear operation lock */
    appA2dpSetLock(theInst, FALSE);
}

/*! \brief  Enter 'connected signalling' state

    The A2DP state machine has entered 'connected signalling' state, this means
    that the A2DP signalling channel has been established.
    
    Kick the link policy manager to make sure this link is configured correctly
    and to maintain the correct link topology.	
    
    Check if we need to create media channel immediately, either because this
    is a locally initiated connection with SEIDs specified or because there
    is already another A2DP sink with media channel established.
*/    
static void appA2dpEnterConnectedSignalling(avInstanceTaskData *theInst)
{   	 
    avInstanceTaskData *theOtherInst;

    DEBUG_LOGF("appA2dpEnterConnectedSignalling(%p)", (void *)theInst);

    /* Mark this device as supporting A2DP */
    appDeviceSetA2dpIsSupported(&theInst->bd_addr);

    /* Copy volume from attributes */
    appDeviceAttributes attributes;
    if (appDeviceFindBdAddrAttributes(&theInst->bd_addr, &attributes))
        theInst->a2dp_volume = attributes.a2dp_volume;

    /* Set link supervision timeout 5 seconds */
    ConnectionSetLinkSupervisionTimeout(appAvGetSink(theInst), 0x1F80);
    
    /* Update most recent connected device */
    ConnectionSmUpdateMruDevice(&theInst->bd_addr);

    appPowerOffTimerRestart();

    /* Clear current SEID */
    theInst->a2dp_current_seid = AV_SEID_INVALID;

    /* Check if there is another connected streaming A2DP instance */
    theOtherInst = appAvInstanceFindA2dpState(theInst,
                                         A2DP_STATE_MASK_CONNECTED_MEDIA_STREAMING,
                                         A2DP_STATE_CONNECTED_MEDIA_STREAMING);
    if (theOtherInst != NULL)
    {
        /* Check instance is configured as a sink */
        if (appA2dpIsSinkCodec(theOtherInst))
        {
            /* Connect media channel, convert SEID of sink into SEID for a source */
            MAKE_AV_MESSAGE(AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ);
            message->seid = appA2dpConvertSeidFromSourceToSink(theOtherInst->a2dp_current_seid);
            MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ);
            MessageSendConditionally(&theInst->av_task, AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ, message,
                                     &appA2dpGetLock(theInst));
        }
    }
    else if (theInst->a2dp_flags & A2DP_CONNECT_MEDIA)
    {
        /* Connect media channel */
        MAKE_AV_MESSAGE(AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ);
        message->seid = AV_SEID_INVALID;
        MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ);
        MessageSendConditionally(&theInst->av_task, AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ, NULL,
                                 &appA2dpGetLock(theInst));
    }
    
    /* Play connected tone */
    appUiAvConnected(theInst->a2dp_flags & A2DP_CONNECT_SILENT);

    /* Clear silent flags */
    theInst->a2dp_flags &= ~(A2DP_CONNECT_SILENT | A2DP_CONNECT_SILENT_ERROR | A2DP_DISCONNECT_SILENT);

    /* Tell clients we have connected */
    MAKE_AV_MESSAGE(AV_A2DP_CONNECTED_IND);
    message->av_instance = theInst;
    message->bd_addr = theInst->bd_addr;
    message->local_initiated = theInst->a2dp_local_initiated;
    appTaskListMessageSend(theInst->av_status_client_list, AV_A2DP_CONNECTED_IND, message);
}

/*! \brief Exit 'connected signalling' state

    The A2DP state machine has exited 'connected signalling' state, this means
    that the A2DP signalling channel has closed.  Make sure AVRCP is disconnected,
    play disconnect tone if required.
*/    
static void appA2dpExitConnectedSignalling(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectedSignalling(%p)", (void *)theInst);

    /* Tell clients we have disconnected */
    MAKE_AV_MESSAGE(AV_A2DP_DISCONNECTED_IND);
    message->av_instance = theInst;
    message->bd_addr = theInst->bd_addr;
    message->reason = theInst->a2dp_disconnect_reason;
    appTaskListMessageSend(theInst->av_status_client_list, AV_A2DP_DISCONNECTED_IND, message);

    /* Clear current SEID */
    theInst->a2dp_current_seid = AV_SEID_INVALID;

    /* Play disconnected tone */
    if (~theInst->a2dp_flags & A2DP_DISCONNECT_SILENT)
    {
        appUiAvDisconnected();
    }

    appPowerOffTimerRestart();
}

/*! \brief Enter 'connecting media local' state

    The A2DP state machine has entered 'connecting media local' state, this means
    that the A2DP media channel is required.  Set the
    operation lock to block any other operations on this instance and initiate
    opening media channel.

*/
static void appA2dpEnterConnectingMediaLocal(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterConnectingMediaLocal(%p)", (void *)theInst);
    
    /* Set operation lock */
    appA2dpSetLock(theInst, TRUE);
    
    /* Open media channel to peer device */
    if (theInst->a2dp_current_seid != AV_SEID_INVALID)
        A2dpMediaOpenRequest(theInst->a2dp_device_id, 1, &theInst->a2dp_current_seid);
    else
        A2dpMediaOpenRequest(theInst->a2dp_device_id, 0, NULL);
}

/*! \brief Exit 'connecting media' state

    The A2DP state machine has exited 'connecting media' state, clear the
    operation lock to allow any pending operations on this instance to proceed.
*/
static void appA2dpExitConnectingMediaLocal(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectingMediaLocal(%p)", (void *)theInst);
    
    /* Clear operation lock */
    appA2dpSetLock(theInst, FALSE);
}

/*! \brief Enter 'connecting media remote' state

    The A2DP state machine has entered 'connecting media remote' state, this means
    that the A2DP media channel is being opened by the remote device.  Set the
    operation lock to block any other operations on this instance.

*/
static void appA2dpEnterConnectingMediaRemote(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterConnectingMediaRemote(%p)", (void *)theInst);

    /* Set operation lock */
    appA2dpSetLock(theInst, TRUE);
}

/*! \brief Exit 'connecting media remote' state

    The A2DP state machine has exited 'connecting media remote' state, clear the
    operation lock to allow any pending operations on this instance to proceed.
*/
static void appA2dpExitConnectingMediaRemote(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectingMediaRemote(%p)", (void *)theInst);

    /* Clear operation lock */
    appA2dpSetLock(theInst, FALSE);
}

/*! \brief Enter 'connected media' state

    The A2DP state machine has entered 'connected media' state, this means
    that the A2DP media channel has been established.
    
    At this point we know the media channel SEID, so we can store the peer
    device Bluetooth Address in the appropriate PSKEY to allow us to connect
    to the device again at a later point.	
*/    
static void appA2dpEnterConnectedMedia(avInstanceTaskData *theInst)
{    
    /* Find A2DP instance where we're the sink */
    avInstanceTaskData *theSinkInst = appAvGetA2dpSink(AV_CODEC_NON_TWS);

    DEBUG_LOGF("appA2dpEnterConnectedMedia(%p)", (void *)theInst);
    
    /* Update volume from sink instance */
    if (theSinkInst && theSinkInst != theInst)
    {
        //TODO: Check & fix
        theInst->a2dp_volume = theSinkInst->a2dp_volume;
    }

    /* Attempt to synchronise volumes */
    appAvVolumeSync(NULL, TRUE);

    appLinkPolicyUpdateRoleFromSink(A2dpMediaGetSink(theInst->a2dp_device_id, theInst->a2dp_stream_id));

    appPowerOffTimerDisable(APP_POWER_EVENT_A2DP);
}

/*! \brief Exit 'connected media' state

    The A2DP state machine has exited 'connected media' state, this means
    that the A2DP media channel has closed.
    
    If this instance is an A2DP sink then check if there is another A2DP
    instance that needs to have it's media channel closed.
*/    
static void appA2dpExitConnectedMedia(avInstanceTaskData *theInst)
{    
    DEBUG_LOGF("appA2dpExitConnectedMedia(%p)", (void *)theInst);

    appPowerOffTimerEnable(APP_POWER_EVENT_A2DP);

    /* Stop streaming on slave */
    if (appA2dpIsSinkCodec(theInst))
    {
        appA2dpStopSlave(theInst);
    }
}

/*! \brief Enter 'disconnecting media' state

    The A2DP state machine has entered 'disconnecting media' state, this means
    that we have initiated disconnecting the A2DP media channel.
    
    Set the operation lock to block any other operations, call
    A2dpClose() to actually request closing of the media channel.
*/
static void appA2dpEnterDisconnectingMedia(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterDisconnectingMedia(%p)", (void *)theInst);

    /* Set operation lock */
    appA2dpSetLock(theInst, TRUE);
    
    /* Close media channel to peer device */
    A2dpMediaCloseRequest(theInst->a2dp_device_id, theInst->a2dp_stream_id);
}

/*! \brief Exit 'disconnecting media' state

    The A2DP state machine has exited 'disconnecting media' state, this means
    that we have completed disconnecting the A2DP media channel.
    
    Clear the operation lock to allow any pending operations on this instance
    to proceed.
*/
static void appA2dpExitDisconnectingMedia(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitDisconnectingMedia(%p)", (void *)theInst);
    
    /* Clear operation lock */
    appA2dpSetLock(theInst, FALSE);
}

/*! \brief Exit 'connected media streaming' state

    The A2DP state machine has exited 'connected media streaming' state, this means
    that the A2DP media channel is now streaming audio and it's time to connect
    to the appropriate audio plug-in.
    
    Tell the main application task that we're streaming so that it can disable
    page scan to prevent audio glitches.
*/    
static void appA2dpEnterConnectedMediaStreaming(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterConnectedMediaStreaming(%p)", (void *)theInst);

    /* Prevent role switch when streaming TWS (standard or plus, source or sink) */
    if (appA2dpIsSeidTws(theInst->a2dp_current_seid))
    {
        appLinkPolicyPreventRoleSwitch(&theInst->bd_addr);
    }

    /* Make sure audio plug-in is connected */
    appA2dpConnectAudio(theInst);

    /* Update hint */
    appAvHintPlayStatus(avrcp_play_status_playing);

    /* Tell clients we are streaming */
    appTaskListMessageSendId(theInst->av_status_client_list, AV_STREAMING_ACTIVE_IND);
}

/*! \brief Exit 'connected media streaming' state

    The A2DP state machine has exited 'connected media streaming' state, this means
    that the A2DP media channel has stopped streaming audio state.
    
    Disconnectthe audio plug-in and the main application task we've finished
    streaming so it can re-enable page scan if required.
*/    
static void appA2dpExitConnectedMediaStreaming(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectedMediaStreaming(%p)", (void *)theInst);

    /* Update hint */
    appAvHintPlayStatus(avrcp_play_status_stopped);

    /* Stop streaming on slave */
    appA2dpStopSlave(theInst);

    /* Disconnect audio plug-in */
    appA2dpDisconnectAudio(theInst);

    /* Allow role switch on exit streaming */
    appLinkPolicyAllowRoleSwitch(&theInst->bd_addr);

    /* Tell clients we are noe streaming */
    appTaskListMessageSendId(theInst->av_status_client_list, AV_STREAMING_INACTIVE_IND);
}

/*! \brief Enter'connected media streaming muted' state

    The A2DP state machine has entered 'connected media streaming muted' state, this means
    that the headset has failed to suspend the audio.
*/    
static void appA2dpEnterConnectedMediaStreamingMuted(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appA2dpEnterConnectedMediaStreamingMuted(%p)", (void *)theInst);
}

/*! \brief Exit 'connected media streaming muted' state

    The A2DP state machine has exited 'connected media streaming muted' state, this means
    that either about to start streaming again or we're disconnecting.
*/    
static void appA2dpExitConnectedMediaStreamingMuted(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectedMediaStreamingMuted(%p)", (void *)theInst);

    /* Cancel any AV_INTERNAL_A2DP_SYNC_MEDIA_IND message just in case we
       exit the 'connected media streaming muted' state without handling the
       message (for example, link-loss) */
    if (MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_SYNC_MEDIA_IND))
        appA2dpStopSlave(theInst);
}

/*! \brief Enter 'connected media suspending' state

    The A2DP state machine has entered 'connected media suspending' state, this means
    that the A2DP media channel needs to be suspended.
    
    SeF the operation lock to block any other operations, call
    A2dpSuspend() to actually request suspension of the stream.
*/    
static void appA2dpEnterConnectedMediaSuspendingLocal(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterConnectedMediaSuspendingLocal(%p)", (void *)theInst);
    
    /* Set operation lock */
    appA2dpSetLock(theInst, TRUE);
    
    /* Suspend A2DP streaming */
    A2dpMediaSuspendRequest(theInst->a2dp_device_id, theInst->a2dp_stream_id);
}

/*! \brief Exit 'connected media suspending' state

    The A2DP state machine has exited 'connected media suspending' state, this could
    be for a number of reasons.	 Clear the operation lock to allow other operations to
    proceed.
*/    
static void appA2dpExitConnectedMediaSuspendingLocal(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectedMediaSuspendingLocal(%p)", (void *)theInst);
    
    /* Clear operation lock */
    appA2dpSetLock(theInst, FALSE);
}

/*! \brief Enter 'connected media suspended' state

    The A2DP state machine has entered 'connected media suspended' state, this means
    the audio streaming has now actually suspended.
*/    
static void appA2dpEnterConnectedMediaSuspended(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appA2dpEnterConnectedMediaSuspended(%p)", (void *)theInst);
}

/*! \brief Exit 'connected media suspended' state

    The A2DP state machine has exited 'connected media suspended' state, this could
    be for a number of reasons.	
*/    
static void appA2dpExitConnectedMediaSuspended(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appA2dpExitConnectedMediaSuspended(%p)", (void *)theInst);
}

/*! \brief Enter 'connected media starting local' state

    The A2DP state machine has entered 'connected media starting local' parent
    state, this means we should attempt to start streaming by sending a
    AVDTP_START to the AV source.

    The operation lock is set to that any other operations are blocked until we
    have exited this state.
*/    
static void appA2dpEnterConnectedMediaStartingLocal(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterConnectedMediaStartingLocal(%p)", (void *)theInst);

    /* Set operation lock */
    appA2dpSetLock(theInst, TRUE);
}
    
/*! \brief Exit 'connected media starting local' state

    The A2DP state machine has exited 'connected media starting local' state,
    this means we have starting streaming or have failed in some way.

    As we are exiting this state we can clear the operation lock to allow any other
    blocked operations to proceed.
*/    
static void appA2dpExitConnectedMediaStartingLocal(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectedMediaStartingLocal(%p)", (void *)theInst);
    
    /* Clear operation lock */
    appA2dpSetLock(theInst, FALSE);
}

/*! \brief Enter 'connected media starting local sync' state

    The A2DP state machine has entered 'connected media starting local sync' state,
    this means we should attempt to start streaming on the Slave.
    
    We send ourselves an internal message which is blocked on the Slave
    starting streaming, once the Slave is ready to stream, the internal message
    is sent which causes us to enter the 'connected media starting local' state.
*/    
static void appA2dpEnterConnectedMediaStartingLocalSync(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterConnectedMediaStartingLocalSync(%p)", (void *)theInst);
    
    /* Send ourselves an AV_INTERNAL_A2DP_SYNC_MEDIA_IND message once
       the other instance has started streaming (or failed in someway) */
    MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_SYNC_MEDIA_IND);
    MessageSendConditionally(&theInst->av_task, AV_INTERNAL_A2DP_SYNC_MEDIA_IND, NULL,
                             appA2dpStartSlave(theInst));
}

/*! \brief Exit 'connected media starting local sync' state

    The A2DP state machine has exited 'connected media starting local sync' state,
    this means we have either sucessfully started streaming to the slace or we
    failed for some reason.
*/    
static void appA2dpExitConnectedMediaStartingLocalSync(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectedMediaStartingLocalSync(%p)", (void *)theInst);

    /* Cancel any AV_INTERNAL_A2DP_SYNC_MEDIA_IND message just in case we
       exit the 'connected media starting local sync' state without handling the
       message (for example, link-loss) */
    if (MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_SYNC_MEDIA_IND))
        appA2dpStopSlave(theInst);
}

/*! \brief Enter 'connected media starting remote sync' state

    The A2DP state machine has entered 'connected media starting remote' state,
    this means the remote device has requested to start streaming.
    
    We sound ourselves an internal message which is blocked on the Slave
    starting streaming, once the Slave is ready to stream, the internal message
    is sent which causes a call to A2dpStartResponse().
    
    The operation lock is set to that any other operations are blocked until we
    have exited this state.
*/    
static void appA2dpEnterConnectedMediaStartingRemoteSync(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterConnectedMediaStartingRemoteSync(%p)", (void *)theInst);
    
    /* Set operation lock */
    appA2dpSetLock(theInst, TRUE);
    
    /* Send ourselves an AV_INTERNAL_A2DP_SYNC_MEDIA_IND message once
       the other instance has started streaming (or failed in someway) */
    MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_SYNC_MEDIA_IND);
    MessageSendConditionally(&theInst->av_task, AV_INTERNAL_A2DP_SYNC_MEDIA_IND, NULL,
                             appA2dpStartSlave(theInst));
}

/*! \brief Exit 'connected media starting remote sync' state

    The A2DP state machine has exited 'connected media starting remote' state,
    this means we have either sucessfully started streaming or we failed for some
    reason.  Check if there is an AV_INTERNAL_A2DP_SYNC_MEDIA_IND message queued
    up, if there is then we are exiting this start due to some abnornal condition
    (for example link-loss), we should make sure that we stop streaming to the Slave.
    
    As we are exiting this state we can clear the operation lock to allow any other
    blocked operations to proceed.
*/    
static void appA2dpExitConnectedMediaStartingRemoteSync(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitConnectedMediaStartingRemoteSync(%p)", (void *)theInst);
    
    /* Cancel any AV_INTERNAL_A2DP_SYNC_MEDIA_IND message just in case we
       exit the 'connected media starting remote sync' state without handling the
       message (for example, link-loss) */
    if (MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_SYNC_MEDIA_IND))
        appA2dpStopSlave(theInst);

    /* Clear operation lock */
    appA2dpSetLock(theInst, FALSE);
}

/*! \brief Enter 'disconnecting' state

    The A2DP state machine has entered 'disconnecting' state, this means that
    we have initiated a disconnect.  Set the operation lock to prevent any other
    operations occuring and call A2dpDisconnectAll() to start the disconnection.
*/    
static void appA2dpEnterDisconnecting(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterDisconnecting(%p)", (void *)theInst);

    /* Set operation lock */
    appA2dpSetLock(theInst, TRUE);
     
    /* Make sure AVRCP isn't doing something important, send internal message blocked on
       AVRCP lock */
    MessageSendConditionally(&theInst->av_task, AV_INTERNAL_AVRCP_UNLOCK_IND, NULL, &appAvrcpGetLock(theInst));
}

/*! \brief Exit 'disconnecting' state

    The A2DP state machine has exited 'disconnect' state, this means we have
    completed the disconnect, clear the operation lock so that any blocked 
    operations can proceed.
*/    
static void appA2dpExitDisconnecting(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitDisconnecting(%p)", (void *)theInst);

    /* Clear operation lock */
    appA2dpSetLock(theInst, FALSE);
}

/*! \brief Enter 'disconnected' state

    The A2DP state machine has entered 'disconnected' state, this means we
    have completely disconnected from the peer device, so we inform the link
    policy manager and main application tasks.  Generally after entering the
    'disconnected' state we'll received a AV_INTERNAL_A2DP_DESTROY_REQ message
    which will destroy this instance.
*/    
static void appA2dpEnterDisconnected(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpEnterDisconnected(%p)", (void *)theInst);

    /* Clear A2DP device ID */
    theInst->a2dp_device_id = INVALID_DEVICE_ID;

    /* Send ourselves a destroy message so that any other messages waiting on the
       operation lock can be handled */
    MessageSendConditionally(&theInst->av_task, AV_INTERNAL_A2DP_DESTROY_REQ, NULL, &appA2dpGetLock(theInst));
}

/*! \brief Exiting 'disconnected' state

    The A2DP state machine has entered 'disconnected' state, this means we
    are about to connect to the peer device, either for a new connection or
    on a reconnect attempt.
*/    
static void appA2dpExitDisconnected(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpExitDisconnected(%p)", (void *)theInst);
    
    /* Reset disconnect reason */
    theInst->a2dp_disconnect_reason = AV_A2DP_CONNECT_FAILED;

    /* Clear any queued AV_INTERNAL_A2DP_DESTROY_REQ messages, as we are exiting the
       'destroyed' state, probably due to a incoming connection */
    MessageCancelAll(&theInst->av_task, AV_INTERNAL_A2DP_DESTROY_REQ);
}

/*! \brief Set A2DP state

    Called to change state.  Handles calling the state entry and exit functions.
    Note: The entry and exit functions will be called regardless of whether or not
    the state actually changes value.
*/
static void appA2dpSetState(avInstanceTaskData *theInst, avA2dpState a2dp_state)
{
    avA2dpState a2dp_old_state = theInst->a2dp_state;
    DEBUG_LOGF("appA2dpSetState(%p)(%02x)", (void *)theInst, a2dp_state);

    /* Handle state exit functions */
    switch (a2dp_old_state)
    {
        case A2DP_STATE_CONNECTING_LOCAL:
            appA2dpExitConnectingLocal(theInst);
            break;
        case A2DP_STATE_CONNECTING_REMOTE:
            appA2dpExitConnectingRemote(theInst);
            break;
        case A2DP_STATE_CONNECTING_MEDIA_LOCAL:
            appA2dpExitConnectingMediaLocal(theInst);
            break;        
        case A2DP_STATE_CONNECTING_MEDIA_REMOTE:
            appA2dpExitConnectingMediaRemote(theInst);
            break;
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
            appA2dpExitConnectedMediaStreaming(theInst);
            break;
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
            appA2dpExitConnectedMediaStreamingMuted(theInst);
            break;
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
            appA2dpExitConnectedMediaSuspendingLocal(theInst);
            break;
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
            appA2dpExitConnectedMediaSuspended(theInst);
            break;
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
            appA2dpExitConnectedMediaStartingLocalSync(theInst);
            break;
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
            appA2dpExitConnectedMediaStartingRemoteSync(theInst);
            break;
        case A2DP_STATE_DISCONNECTING_MEDIA:
            appA2dpExitDisconnectingMedia(theInst);
            break;        
        case A2DP_STATE_DISCONNECTING:
            appA2dpExitDisconnecting(theInst);
            break;
        case A2DP_STATE_DISCONNECTED:
            appA2dpExitDisconnected(theInst);
            break;
        default:
            break;
    }

    /* Check if exiting 'connected media starting local' parent state */
    if (appA2dpIsStateConnectedMediaStartingLocal(a2dp_old_state) && !appA2dpIsStateConnectedMediaStartingLocal(a2dp_state))
        appA2dpExitConnectedMediaStartingLocal(theInst);            

    /* Check if exiting 'connected media' parent state */
    if (appA2dpIsStateConnectedMedia(a2dp_old_state) && !appA2dpIsStateConnectedMedia(a2dp_state))
        appA2dpExitConnectedMedia(theInst);

    /* Check if exiting 'connected signalling' parent state */
    if (appA2dpIsStateConnectedSignalling(a2dp_old_state) && !appA2dpIsStateConnectedSignalling(a2dp_state))
        appA2dpExitConnectedSignalling(theInst);

    /* Set new state */
    theInst->a2dp_state = a2dp_state;
            
    /* Check if entering 'connected signalling' parent state */
    if (!appA2dpIsStateConnectedSignalling(a2dp_old_state) && appA2dpIsStateConnectedSignalling(a2dp_state))
        appA2dpEnterConnectedSignalling(theInst);

    /* Check if entering 'connected media' parent state */
    if (!appA2dpIsStateConnectedMedia(a2dp_old_state) && appA2dpIsStateConnectedMedia(a2dp_state))
        appA2dpEnterConnectedMedia(theInst);

    /* Check if entering 'connected media starting local' parent state */
    if (!appA2dpIsStateConnectedMediaStartingLocal(a2dp_old_state) && appA2dpIsStateConnectedMediaStartingLocal(a2dp_state))
        appA2dpEnterConnectedMediaStartingLocal(theInst);            
                    
    /* Handle state entry functions */
    switch (a2dp_state)
    {
        case A2DP_STATE_CONNECTING_LOCAL:
            appA2dpEnterConnectingLocal(theInst);
            break;
        case A2DP_STATE_CONNECTING_REMOTE:
            appA2dpEnterConnectingRemote(theInst);
            break;
        case A2DP_STATE_CONNECTING_MEDIA_LOCAL:
            appA2dpEnterConnectingMediaLocal(theInst);
            break;        
        case A2DP_STATE_CONNECTING_MEDIA_REMOTE:
            appA2dpEnterConnectingMediaRemote(theInst);
            break;
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
            appA2dpEnterConnectedMediaStreaming(theInst);
            break;            
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
            appA2dpEnterConnectedMediaStreamingMuted(theInst);
            break;
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
            appA2dpEnterConnectedMediaSuspendingLocal(theInst);
            break;            
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
            appA2dpEnterConnectedMediaSuspended(theInst);
            break;            
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
            appA2dpEnterConnectedMediaStartingLocalSync(theInst);
            break;            
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
            appA2dpEnterConnectedMediaStartingRemoteSync(theInst);
            break;            
        case A2DP_STATE_DISCONNECTING_MEDIA:
            appA2dpEnterDisconnectingMedia(theInst);
            break;        
        case A2DP_STATE_DISCONNECTING:
            appA2dpEnterDisconnecting(theInst);
            break;
        case A2DP_STATE_DISCONNECTED:
            appA2dpEnterDisconnected(theInst);
            break;
        default:
            break;
    }               
 
    /* Reflect state change in UI */
    appUiAvState(a2dp_state);

    /* Update link policy following change in state */
    appLinkPolicyUpdatePowerTable(&theInst->bd_addr);
}

/*! \brief Get A2DP state

    \param  theAv   The AV instance for this A2DP link
    
    \return The current A2DP state.
*/
avA2dpState appA2dpGetState(avInstanceTaskData *theAv)
{
    return theAv->a2dp_state;
}

/*! \brief Handle A2DP error

    Some error occurred in the A2DP state machine.
    
    To avoid the state machine getting stuck, if instance is connected then
    drop connection and move to 'disconnecting' state.
*/	
static void appA2dpError(avInstanceTaskData *theInst, MessageId id, Message message)
{
    UNUSED(message); UNUSED(id);

#if defined(AV_DEBUG) || defined(AV_DEBUG_PANIC)
    DEBUG_LOGF("appA2dpError(%p), state=%u, id=%x", (void *)theInst, theInst->a2dp_state, id);
#endif

    /* Check if we are connected */
    if (appA2dpIsStateConnectedSignalling(appA2dpGetState(theInst)))
    {
        /* Move to 'disconnecting' state */
        appA2dpSetState(theInst, A2DP_STATE_DISCONNECTING); 
    }
    
    /* Destroy ourselves */    
    MessageSend(&theInst->av_task, AV_INTERNAL_A2DP_DESTROY_REQ, NULL);
}

/*! \brief Request outgoing A2DP connection 

    Handle A2DP connect request from AV parent task, store connection
    parameters and move into the 'connecting local' state.  The state machine
    will handle creating the connection.  If we are not in the 'disconnected'
    state then just ignore the request as it was probably due to a
    race-condition, this can happen as the AV_INTERNAL_A2DP_CONNECT_REQ can be
    blocked by the ACL lock not the operation lock.
*/    
static void appA2dpHandleInternalA2dpConnectRequest(avInstanceTaskData *theInst,
                                                    const AV_INTERNAL_A2DP_CONNECT_REQ_T *req)
{
    DEBUG_LOGF("appA2dpHandleInternalA2dpConnectRequest(%p), %x,%x,%lx",
                 (void *)theInst, theInst->bd_addr.nap, theInst->bd_addr.uap, theInst->bd_addr.lap);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_DISCONNECTED:
        {
            /* Check ACL is connected */
            if (appConManagerIsConnected(&theInst->bd_addr))
            {
                /* Store connection parameters */
                theInst->a2dp_flags = req->flags;
                theInst->a2dp_connect_retries = req->num_retries;

                /* Debug, show SEIDs */
                DEBUG_LOGF("appA2dpHandleInternalA2dpConnectRequest(%p), %x,%x,%lx", (void *)theInst,
                           theInst->bd_addr.nap, theInst->bd_addr.uap, theInst->bd_addr.lap);

                /* Move to 'connecting local' state */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTING_LOCAL);
            }
            else
            {
                DEBUG_LOGF("appA2dpHandleInternalA2dpConnectRequest(%p), no ACL %x,%x,%lx", (void *)theInst,
                           theInst->bd_addr.nap, theInst->bd_addr.uap, theInst->bd_addr.lap);

                /* Set disconnect reason */
                theInst->a2dp_disconnect_reason = AV_A2DP_CONNECT_FAILED;

                /* Move to 'disconnected' state */
                appA2dpSetState(theInst, A2DP_STATE_DISCONNECTED);
            }
        }
        return;
        
        default:
            return;
    }
}
        
/*! \brief Request A2DP media channel

    Handle A2DP open media channel request from AV parent task, or sibling A2DP instance.
    Only valid in the 'connected signalling' state, should never be received in any of the
    transition states as the operation lock will block the request.
*/
static void appA2dpHandleInternalA2dpConnectMediaRequest(avInstanceTaskData *theInst,
                                                         const AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ_T *req)
{
    DEBUG_LOGF("appA2dpHandleInternalA2dpConnectMediaRequest(%p)", (void *)theInst);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_SIGNALLING:
        {
            /* Store requested SEID */
            theInst->a2dp_current_seid = req->seid;

            /* Move to 'local connecting media' state */
            appA2dpSetState(theInst, A2DP_STATE_CONNECTING_MEDIA_LOCAL);
        }
        return;
                    
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        case A2DP_STATE_DISCONNECTED:
            return;

        default:
            appA2dpError(theInst, AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ, NULL);
            return;
    }
}

/*! \brief Disconnect A2DP media channel

    Handle A2DP close media channel request from AV parent task, or sibling A2DP instance.
    Only valid in the 'connected media' states, should never be received in any of the
    transition states as the operation lock will block the request.
*/
static void appA2dpHandleInternalA2dpDisconnectMediaRequest(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpHandleInternalA2dpDisconnectMediaRequest(%p)", (void *)theInst);
    
    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        {
            /* Move to 'local disconnecting media' state */
            appA2dpSetState(theInst, A2DP_STATE_DISCONNECTING_MEDIA);
        }
        return;

        case A2DP_STATE_CONNECTED_SIGNALLING:
        case A2DP_STATE_DISCONNECTED:
            return;                    

        default:
            appA2dpError(theInst, AV_INTERNAL_A2DP_DISCONNECT_MEDIA_REQ, NULL);
            return;
    }
}
    
/*! \brief Request A2DP disconnection 

    Handle A2DP disconnect request from AV parent task.  Move into the
    'disconnecting' state, this will initiate the disconnect.
*/    
static void appA2dpHandleInternalA2dpDisconnectRequest(avInstanceTaskData *theInst,
                                                       const AV_INTERNAL_A2DP_DISCONNECT_REQ_T *req)
{
    DEBUG_LOGF("appA2dpHandleInternalA2dpDisconnectRequest(%p)", (void *)theInst);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_SIGNALLING:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        {
            /* Store flags */
            theInst->a2dp_flags |= req->flags;
    
            /* Move to 'disconnecting' state */
            appA2dpSetState(theInst, A2DP_STATE_DISCONNECTING);	
        }
        return;

        case A2DP_STATE_DISCONNECTED:
            /* Ignore as instance already disconnected */
            return;
            
        default:
            appA2dpError(theInst, AV_INTERNAL_A2DP_DISCONNECT_REQ, req);
            return;
    }
}

/*! \brief Request suspend A2DP streaming

    Handle A2DP suspend request from AV parent task, move into the
    'suspending local' state, this will initate the suspend.
    
    Record the suspend reason, to prevent resuming if there are outstanding
    suspend reasons.
*/    
static void appA2dpHandleInternalA2dpSuspendRequest(avInstanceTaskData *theInst,
                                                    const AV_INTERNAL_A2DP_SUSPEND_MEDIA_REQ_T *req)
{
    DEBUG_LOGF("appA2dpHandleInternalA2dpSuspendRequest(%p)", (void *)theInst);

    /* Record suspend reason */
    theInst->a2dp_suspend_state |= req->reason;
    
    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTING_LOCAL:
        case A2DP_STATE_CONNECTING_REMOTE:
        case A2DP_STATE_CONNECTED_SIGNALLING:
        case A2DP_STATE_CONNECTING_MEDIA_LOCAL:
        case A2DP_STATE_CONNECTING_MEDIA_REMOTE:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
        case A2DP_STATE_DISCONNECTING_MEDIA:
        case A2DP_STATE_DISCONNECTING:
        case A2DP_STATE_DISCONNECTED:
            return;
            
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        {
            /* Move to suspending state if this instance is an AV sink */
            if (appA2dpIsSeidSink(theInst->a2dp_current_seid))
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL);        
        }
        return;

        default:
            appA2dpError(theInst, AV_INTERNAL_A2DP_SUSPEND_MEDIA_REQ, req);
            return;
    }
}

/*! \brief Request start A2DP streaming

    Handle A2DP resume request from AV parent task.  Clear the suspend reason,
    if there are no suspend reasons left then we can attempt to initiate A2DP
    streaming.
*/    
static void appA2dpHandleInternalA2dpResumeRequest(avInstanceTaskData *theInst,
                                                   const AV_INTERNAL_A2DP_RESUME_MEDIA_REQ_T *req)
{
    DEBUG_LOGF("appA2dpHandleInternalA2dpResumeRequest(%p)", (void *)theInst);

    /* Clear suspend reason */
    theInst->a2dp_suspend_state &= ~req->reason;
    
    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        {
            /* Move to 'connected media starting local sync' state if this instance is an AV
               sink and no reasons to suspend left */
            if (appA2dpIsSeidSink(theInst->a2dp_current_seid) && !theInst->a2dp_suspend_state)
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC);
        }
        return;
                    
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        {
            /* Check if no reasons to suspend left */
            if (!theInst->a2dp_suspend_state)
            {
                /* Attempt to start streaming to Slave, send ourselves an
                   AV_INTERNAL_A2DP_SYNC_MEDIA_IND message once the Slave has
                   started streaming */
                MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_SYNC_MEDIA_IND);
                MessageSendConditionally(&theInst->av_task, AV_INTERNAL_A2DP_SYNC_MEDIA_IND, NULL,
                                         appA2dpStartSlave(theInst));
            }
        }
        return;

        case A2DP_STATE_CONNECTING_LOCAL:
        case A2DP_STATE_CONNECTING_REMOTE:
        case A2DP_STATE_CONNECTED_SIGNALLING:
        case A2DP_STATE_CONNECTING_MEDIA_LOCAL:
        case A2DP_STATE_CONNECTING_MEDIA_REMOTE:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
        case A2DP_STATE_DISCONNECTING_MEDIA:
        case A2DP_STATE_DISCONNECTING:
        case A2DP_STATE_DISCONNECTED:
            return;

        default:
            appA2dpError(theInst, AV_INTERNAL_A2DP_RESUME_MEDIA_REQ, req);
            return;
    }
}

/*! \brief Handle Slave synchronisation indication

    This function is called when the Slave A2DP instance has sent the Master
    A2DP instance a message to indicate that it is ready to stream to the
    Slave headset.
    
    There are 3 states in which this message is handled:
    
    'connected media starting local sync'
        In this state this instance is waiting to initiate streaming, since
        the Slave instance is now ready, we can call A2dpStart() to start streaming
        and move to the 'connected media starting local' state.
        
    'connected media starting remote sync'
        In this state, this instance has received a start request from the AV source
        and is waiting for the Slave instance to be ready.  Just move into the
        'connected media streaming' state, which will load the DSP with the appropriate
        decoder and turn on the DACs.
        
    'connected media streaming muted'
        In this state, this instance has been requested to stream from the AV source but
        we weren't able to start streaming due to some outstanding suspend reason.
        Now the Slave instance is ready when can move into the 'connected media streaming'
        state.
*/    
static void appA2dpHandleInternalA2dpSyncMediaIndication(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appA2dpHandleInternalA2dpSyncMediaIndication(%p)", (void *)theInst);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
        {
            /* Start streaming */
            A2dpMediaStartRequest(theInst->a2dp_device_id, theInst->a2dp_stream_id);
        
            /* Move to 'connected media starting local' state, as the Slave
               headset is now ready */
            appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL);	
        }
        return;

        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
        {
            /* Move to 'connected media streaming' state, the remote device might already be
               streaming */
            appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STREAMING);	
        }
        return;

        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        {
            /* Move to 'connected media streaming' state */
            appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STREAMING);	
        }
        return;
                
        default:
            appA2dpError(theInst, AV_INTERNAL_A2DP_SYNC_MEDIA_IND, NULL);
            return;
    }
}

/*! \brief Accept incoming A2DP connection 

    A2DP Library has indicated an incoming A2DP connection for a new instance,
    the incoming connection was originally handled by the parent AV task which
    created a new A2DP instance and then sent this instance an internal message
    indicating a incoming A2DP connection.  Due to cross-over/race conditions
    it is also possible for this message to arrive on an existing instance, so
    we have to check that the instance is in the 'disconnect' state before
    accepting the connection, otherwise the connection is rejected.
*/    
static void appA2dpHandleInternalA2dpSignallingConnectIndication(avInstanceTaskData *theInst,
                                                                 const AV_INTERNAL_A2DP_SIGNALLING_CONNECT_IND_T *ind)
{
    //avTaskData *theAv = appGetAv();
    DEBUG_LOGF("appA2dpHandleInternalA2dpSignallingConnectIndication(%p)", (void *)theInst);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_DISCONNECTED:
        {
            /* Store A2DP device ID */
            theInst->a2dp_device_id = ind->a2dp_device_id;
    
            /* Store connection flags */
            theInst->a2dp_flags = ind->flags;
    
            /* Accept incoming connection */
            A2dpSignallingConnectResponseWithTask(theInst->a2dp_device_id, TRUE, &theInst->av_task);
    
            /* Move to 'connecting remote' state */
            appA2dpSetState(theInst, A2DP_STATE_CONNECTING_REMOTE);    
        }
        return;
                    
        default:
        {
            /* Reject incoming connection */
            A2dpSignallingConnectResponseWithTask(theInst->a2dp_device_id, FALSE, &theInst->av_task);
        }
        return;
    }
}

/*! \brief Accept incoming A2DP connection 

    A2DP Library has indicated an incoming A2DP connection for an existing
    instance.  Due to cross-over/race conditions it is possible for this
    message to arrive is various states, so we have to check that the instance
    is in the 'disconnect' state before accepting the connection, otherwise
    the connection is rejected.
*/    
static void appA2dpHandleA2dpSignallingConnectIndication(avInstanceTaskData *theInst,
                                                         const A2DP_SIGNALLING_CONNECT_IND_T *ind)
{
    //avTaskData *theAv = appGetAv();
    DEBUG_LOGF("appA2dpHandleA2dpSignallingConnectIndication(%p)", (void *)theInst);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_DISCONNECTED:
        {
            /* Store A2DP device ID */
            theInst->a2dp_device_id = ind->device_id;
        
            /* Accept incoming connection */
            A2dpSignallingConnectResponseWithTask(ind->device_id, TRUE, &theInst->av_task);
    
            /* Move to 'connecting remote' state */
            appA2dpSetState(theInst, A2DP_STATE_CONNECTING_REMOTE);    
        }
        return;

        default:
        {
            /* Reject incoming connection */
            A2dpSignallingConnectResponseWithTask(ind->device_id, FALSE, &theInst->av_task);
        }
        return;
    }
}

/*! \brief A2DP signalling channel confirmation

    A2DP library has confirmed signalling channel connect request.
    First of all check if the request was successful, if it was then we should
    store the pointer to the newly created A2DP instancem, also obtain the
    address of the remote device from the Sink.  After this move into the
    'connect signalling' state as we now have an active A2DP signalling channel.
    
    If the request was unsucessful, move back to the 'disconnected' state and
    play an error tone if this connection request was silent.  Note: Moving to
    the 'disconnected' state may result in this AV instance being free'd.
*/		
static void appA2dpHandleA2dpSignallingConnectConfirm(avInstanceTaskData *theInst,
                                                      const A2DP_SIGNALLING_CONNECT_CFM_T *cfm)
{
    DEBUG_LOGF("appA2dpHandleA2dpSignallingConnectConfirm(%p)(%d)", (void *)theInst, cfm->status);
    
    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTING_LOCAL:
        case A2DP_STATE_CONNECTING_REMOTE:
        case A2DP_STATE_CONNECTED_SIGNALLING:
        case A2DP_STATE_DISCONNECTED:
        {
            /* Check if signalling channel created successfully */
            if (cfm->status == a2dp_success)
            {
                /* Store the A2DP device ID to use in controlling A2DP library */
                theInst->a2dp_device_id = cfm->device_id;
        
                /* Move to 'connected signalling' state */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_SIGNALLING);
            }
            else
            {
                /* Set disconnect reason */
                theInst->a2dp_disconnect_reason = AV_A2DP_CONNECT_FAILED;

                /* Check if we should retry */
                if (theInst->a2dp_connect_retries)
                {                
                    MAKE_AV_MESSAGE(AV_INTERNAL_A2DP_CONNECT_REQ);

                    /* Send message to retry connecting this AV instance */
                    message->num_retries = theInst->a2dp_connect_retries - 1;
                    message->flags = theInst->a2dp_flags;
                    MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_CONNECT_REQ);
                    MessageSendConditionally(&theInst->av_task, AV_INTERNAL_A2DP_CONNECT_REQ, message,
                                             appConManagerCreateAcl(&theInst->bd_addr));

                    /* Move to 'disconnected' state */
                    appA2dpSetState(theInst, A2DP_STATE_DISCONNECTED);
                    return;
                }
                else
                {
                    /* Play error tone */
                    appUiAvError(theInst->a2dp_flags & A2DP_CONNECT_SILENT_ERROR);
            
                    /* Move to 'disconnected' state */
                    appA2dpSetState(theInst, A2DP_STATE_DISCONNECTED);
                }    
            }
        }
        return;
         
        default:
            appA2dpError(theInst, A2DP_SIGNALLING_CONNECT_CFM, cfm);
            return;
    }
}

/*! \brief A2DP connection disconnected

    A2DP Library has indicated that the signalling channel for A2DP
    has been disconnected, move to the 'disconnected' state, this will
    result in this AV instance being destroyed.
*/    
static void appA2dpHandleA2dpSignallingDisconnectInd(avInstanceTaskData *theInst,
                                                     const A2DP_SIGNALLING_DISCONNECT_IND_T *cfm)
{
    assert(theInst->a2dp_device_id == cfm->device_id);
    DEBUG_LOGF("appA2dpHandleA2dpSignallingDisconnectInd(%p)", (void *)theInst);
 
    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTING_LOCAL:
        case A2DP_STATE_CONNECTING_REMOTE:
        case A2DP_STATE_CONNECTED_SIGNALLING:
        case A2DP_STATE_CONNECTING_MEDIA_LOCAL:
        case A2DP_STATE_CONNECTING_MEDIA_REMOTE:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
        {
            /* Check if disconnected due to link-loss */
            if (cfm->status == a2dp_disconnect_link_loss && !theInst->detach_pending)
            {
                /* Check that there are no AV_INTERNAL_A2DP_DISCONNECT_REQ message to cancel before attempting to reconnect */
                if (MessageCancelAll(&theInst->av_task, AV_INTERNAL_A2DP_DISCONNECT_REQ) == 0)
                {
                    /* Attempt to reconenct */
                    appAvA2dpConnectRequest(&theInst->bd_addr, 
                                    A2DP_CONNECT_SILENT_ERROR | A2DP_CONNECT_SILENT);
                }

                /* Inform user of link-loss */
                appUiAvLinkLoss();

                /* Set disconnect reason */
                theInst->a2dp_disconnect_reason = AV_A2DP_DISCONNECT_LINKLOSS;
            }
            else
            {
                /* Play disconnected tone */
                appUiAvDisconnected();

                /* Set disconnect reason */
                theInst->a2dp_disconnect_reason = AV_A2DP_DISCONNECT_NORMAL;
            }

            /* Move to 'disconnected' state */
            appA2dpSetState(theInst, A2DP_STATE_DISCONNECTED);
        }
        break;

        case A2DP_STATE_DISCONNECTING_MEDIA:
        case A2DP_STATE_DISCONNECTING:
        case A2DP_STATE_DISCONNECTED:
        {
            /* Play disconnected tone */
            appUiAvDisconnected();

            /* Set disconnect reason */
            theInst->a2dp_disconnect_reason = AV_A2DP_DISCONNECT_NORMAL;

            /* Move to 'disconnected' state */
            appA2dpSetState(theInst, A2DP_STATE_DISCONNECTED);                    
        }
        return;

        default:
            appA2dpError(theInst, A2DP_SIGNALLING_DISCONNECT_IND, cfm);
            return;
    }                
}

/*! \brief A2DP media channel open indication

    A2DP Library has indicated that the A2DP media channel has been opened by
    peer device.
    
    The SEID for this channel should be stored as this will be required
    later to connect the Kalimba to the A2DP media channel.
    
    Move into the 'connected media suspended' state, the peer device is
    responsible for starting streaming.
*/
static void appA2dpHandleA2dpMediaOpenIndication(avInstanceTaskData *theInst,
                                                 const A2DP_MEDIA_OPEN_IND_T *ind)
{
    assert(theInst->a2dp_device_id == ind->device_id);
    DEBUG_LOGF("appA2dpHandleA2dpMediaOpenIndication(%p)(%d)", (void *)theInst, ind->seid);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_SIGNALLING:
        case A2DP_STATE_CONNECTING_MEDIA_LOCAL:
        case A2DP_STATE_CONNECTING_MEDIA_REMOTE:
        {
            /* Store the SEID */
            theInst->a2dp_current_seid = ind->seid;

            /* Mark media channel as suspended by remote */
            theInst->a2dp_suspend_state |= AV_SUSPEND_REASON_REMOTE;
        
            /* Accept media connection */
            A2dpMediaOpenResponse(ind->device_id, TRUE);

            /* Move to 'connecting media remote' state, wait for confirmation */
            appA2dpSetState(theInst, A2DP_STATE_CONNECTING_MEDIA_REMOTE);
        }
        return;

        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
        case A2DP_STATE_DISCONNECTING_MEDIA:
        case A2DP_STATE_DISCONNECTING:
        case A2DP_STATE_DISCONNECTED:
        {
            /* Reject media connection */
            A2dpMediaOpenResponse(ind->device_id, FALSE);
        }
        return;
        
        default:
            appA2dpError(theInst, A2DP_MEDIA_OPEN_IND, ind);
            return;
    }
}

/*! \brief A2DP media channel open confirmation

    A2DP Library has confirmed opening of the media channel, if the channel we opened
    successfully then store the SEID for use later one.
    
    Check if the channel should be suspended or streaming and move into the appropriate
    state.

    The state entry functions will handle resuming/suspending the channel.
    If the channel open failed then move back to 'connected signalling' state and play
    error tone.
*/
static void appA2dpHandleA2dpMediaOpenConfirm(avInstanceTaskData *theInst,
                                              const A2DP_MEDIA_OPEN_CFM_T *cfm)
{
    DEBUG_LOGF("appA2dpHandleA2dpMediaOpenConfirm(%p)(%d, %d)", (void *)theInst, cfm->status, cfm->seid);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTING_MEDIA_LOCAL:
        {
            assert(theInst->a2dp_device_id == cfm->device_id);

            /* Check if open was successful */
            if (cfm->status == a2dp_success)
            {	
                /* Store the SEID and stream ID */
                theInst->a2dp_current_seid = cfm->seid;
                theInst->a2dp_stream_id = cfm->stream_id;

                /* Check if we should start or suspend streaming */
                if (theInst->a2dp_suspend_state != 0)
                    appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL);
                else
                    appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC);
            }
            else
            {
                /* Move to 'connected signalling' state */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_SIGNALLING);
        
                /* Play error tone */
                appUiAvError(FALSE);
            }
        }
        return;

        case A2DP_STATE_CONNECTING_MEDIA_REMOTE:
        {
            assert(theInst->a2dp_device_id == cfm->device_id);

            /* Check if open was successful */
            if (cfm->status == a2dp_success)
            {
                /* Store the SEID and stream ID */
                theInst->a2dp_current_seid = cfm->seid;
                theInst->a2dp_stream_id = cfm->stream_id;

                /* Remote initiate media channel defaults to suspended */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_SUSPENDED);
            }
            else
            {
                /* Move to 'connected signalling' state */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_SIGNALLING);

                /* Play error tone */
                appUiAvError(FALSE);
            }
        }
        return;

        case A2DP_STATE_DISCONNECTED:
            return;

        default:
            appA2dpError(theInst, A2DP_MEDIA_OPEN_CFM, cfm);
            return;
    }
}

/*! \brief Handle A2DP streaming start indication

    A2DP Library has indicated streaming of the media channel, accept the
    streaming request and move into the appropriate state.  If there is still
    a suspend reason active move into the 'streaming muted'.     
*/
static void appA2dpHandleA2dpMediaStartIndication(avInstanceTaskData *theInst,
                                                  const A2DP_MEDIA_START_IND_T *ind)
{
    assert(theInst->a2dp_device_id == ind->device_id);
    DEBUG_LOGF("appA2dpHandleA2dpMediaStartIndication(%p)", (void *)theInst);

    /* Record the fact that remote device has request start */
    theInst->a2dp_suspend_state &= ~AV_SUSPEND_REASON_REMOTE;

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        {
            /* Check if other instance is in streaming state as an A2DP sink or
               we should still be suspended */
            avInstanceTaskData *theOtherInst;
            theOtherInst = appAvInstanceFindA2dpState(theInst,
                                                      A2DP_STATE_CONNECTED_MEDIA_STREAMING,
                                                      A2DP_STATE_CONNECTED_MEDIA_STREAMING);
            if ((theOtherInst && appA2dpIsSinkCodec(theOtherInst)) ||
                (theInst->a2dp_suspend_state != 0))
            {
                /* Note: We could reject the AVDTP_START at this point, but this
                   seems to upset some AV sources, so instead we'll accept
                   but just drop the audio data */

                /* Accept streaming start request */
                A2dpMediaStartResponse(theInst->a2dp_device_id, ind->stream_id, TRUE);

                /* Move into 'streaming muted' state */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED);
            }
            else
            {
                /* Accept streaming start request immediately, we can't delay
                   until Slave is ready as some AV source timeout very quickly */
                A2dpMediaStartResponse(theInst->a2dp_device_id, ind->stream_id, TRUE);

                /* Move to 'connected media starting remote' state to wait for
                   AV_INTERNAL_A2DP_START_MEDIA_RES message once the other
                   instance has started streaming */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC);
            }
        }
        return;

        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL:
        {
            /* Accept streaming start request */
            A2dpMediaStartResponse(theInst->a2dp_device_id, ind->stream_id, TRUE);

            /* Check if we should still be suspended */
            if (theInst->a2dp_suspend_state != 0)
            {
                /* Not ready to start streaming, so enter the 'streaming muted' state */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED);
            }
            else
            {
                /* Ready to start streaming, so enter the 'streaming' state */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STREAMING);
            }
        }
        return;
        
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
        {
            /* Accept streaming start request */
            A2dpMediaStartResponse(theInst->a2dp_device_id, ind->stream_id, TRUE);

            /* Not ready to start streaming, so enter the 'streaming muted' state */
            appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED);
        }
        return;            

        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        {
            /* Accept streaming start request */
            A2dpMediaStartResponse(theInst->a2dp_device_id, ind->stream_id, TRUE);
        }
        return;
        
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
        {
            /* Received duplicate, just ignore and hope for the best... */
        }
        return;

        default:
            appA2dpError(theInst, A2DP_MEDIA_START_IND, ind);
            return;
    }
}

/*! \brief Handle A2DP streaming start confirmation

    A2DP Library has confirmed streaming of the media channel, if successful
    and in the 'suspended' or 'starting local' state then move into the
    'streaming' state if there is no suspend reasons pending otherwise
    move into the 'suspending local' state.  If streaming failed then move
    into the 'suspended' state and wait for the peer device to start streaming.
*/
static void appA2dpHandleA2dpMediaStartConfirmation(avInstanceTaskData *theInst,
                                                    const A2DP_MEDIA_START_CFM_T *cfm)
{
    assert(theInst->a2dp_device_id == cfm->device_id);
    DEBUG_LOGF("appA2dpHandleA2dpMediaStartConfirmation(%p) status %d", (void *)theInst, cfm->status);
    
    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        {
            /* Ignore, we're already streaming */
        }
        return;
                                                    
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL:
        {
            /* Check confirmation is success */ 
            if (cfm->status == a2dp_success)
            {
                /* Check if we should start or suspend streaming */
                if (theInst->a2dp_suspend_state != 0)
                    appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL);
                else
                    appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STREAMING);
            }
            else
            {
                /* Move to suspended state */
                appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_SUSPENDED);
            }
        }
        return;
        
        default:
            appA2dpError(theInst, A2DP_MEDIA_START_CFM, cfm);
            return;
    }
}

/*! \brief Handle A2DP streaming suspend indication

    The peer device has suspended the media channel.  Move into the
    'connected suspended' state, the state entry function will handle
    turning off the DACs and powering down the DSP.
*/
static void appA2dpHandleA2dpMediaSuspendIndication(avInstanceTaskData *theInst,
                                                    const A2DP_MEDIA_SUSPEND_IND_T *ind)
{
    assert(theInst->a2dp_device_id == ind->device_id);
    DEBUG_LOGF("appA2dpHandleA2dpMediaSuspendIndication(%p)", (void *)theInst);

    /* Record the fact that remote device has request suspend */
    theInst->a2dp_suspend_state |= AV_SUSPEND_REASON_REMOTE;

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
        {
            /* Move to 'connect media suspended' state */	
            appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_SUSPENDED);
        }
        return;
        
        default:
            appA2dpError(theInst, A2DP_MEDIA_SUSPEND_IND, ind);
            return;
    }
}

/*! \brief Handle A2DP streaming suspend confirmation

    Confirmation of request to suspend streaming.  If our request to suspend
    streaming was sucessful move to the 'suspended' or 'resuming' state depending
    on whether a resume was pending.
    
    If the suspend request was rejected then move to the 'streaming muted' or
    'streaming' state depending on whether a resume was pending.  If a resume was
    pending we can go straight to the 'streaming' state and the suspend never
    actually happened.	
*/
static void appA2dpHandleA2dpMediaSuspendConfirmation(avInstanceTaskData *theInst,
                                                      const A2DP_MEDIA_SUSPEND_CFM_T *cfm)
{
    assert(theInst->a2dp_device_id == cfm->device_id);
    DEBUG_LOGF("appA2dpHandleA2dpMediaSuspendConfirmation(%p) status %d", (void *)theInst, cfm->status);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
            /* Already suspended, so just ignore */
            return;
            
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
        {
            /* Check if we suspended */
            if (cfm->status == a2dp_success)
            {
                /* Check if we should start or suspend streaming */
                if (theInst->a2dp_suspend_state != 0)
                    appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_SUSPENDED);
                else
                    appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC);
            }
            else
            {
                /* Check if we should start or mute streaming */
                if (theInst->a2dp_suspend_state != 0)
                    appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED);
                else
                    appA2dpSetState(theInst, A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC);
            }
        }
        return;
        
        default:
            appA2dpError(theInst, A2DP_MEDIA_SUSPEND_CFM, cfm);
            return;
    }
}


/*! \brief Find service category in CODEC capabilities

    Searchs the CODEC capabilities for the required service category.
    caps set to start of category data or NULL if not found.
    size_caps set to size of category data or 0 if not found.
*/
static void appA2dpFindServiceCategory(uint8 service_category, const uint8 **caps_ptr, uint16 *size_caps_ptr)
{
    const uint8 *caps = *caps_ptr;
    uint16 size_caps = *size_caps_ptr;

    while (size_caps != 0)
    {
        if (service_category == caps[0])
        {
            size_caps = caps[1];
            caps += 2;
            break;
        }

        size_caps -= 2 + caps[1];
        caps += 2 + caps[1];
    }

    if (size_caps)
        *caps_ptr = caps;
    else
        *caps_ptr = NULL;

    *size_caps_ptr = size_caps;
}

/*! \brief Copy service category

    Searchs the CODEC capabilities for the required service category and
    copies it from the from_caps capabilities to the to_caps capabilities.
*/
static bool appA2dpCopyServiceCategory(uint8 service_category,
                                       const uint8 *from_caps, uint16 size_from_caps,
                                       uint8 *to_caps, uint16 size_to_caps)
{
    appA2dpFindServiceCategory(service_category, &from_caps, &size_from_caps);
    appA2dpFindServiceCategory(service_category, &to_caps, &size_to_caps);
    if (from_caps && to_caps)
    {
        memcpy(to_caps, from_caps, size_from_caps);
        return TRUE;
    }
    else
        return FALSE;
}

/*! \brief Handle request for CODEC configuration

    The A2DP profile library has requested the media CODEC configuration for
    the specified media codec.
    
    Check if there is another A2DP instance and query it's media CODEC
    configuration to build up a vendor CODEC.	
*/
static void appA2dpHandleA2dpCodecConfigureIndication(avInstanceTaskData *theInst,
                                                      A2DP_CODEC_CONFIGURE_IND_T *ind)
{
    avInstanceTaskData *peerInst;
    
    assert(theInst->a2dp_device_id == ind->device_id);
    DEBUG_LOGF("appA2dpHandleA2dpCodecConfigureIndication(%p)", (void *)theInst);

    /* Check if there is another connected media A2DP instance */
    peerInst = appAvInstanceFindA2dpState(theInst, A2DP_STATE_MASK_CONNECTED_MEDIA,
                                          A2DP_STATE_CONNECTED_MEDIA);
    if (peerInst != NULL)
    {
        /* Check instance is configured as a non-TWS sink */
        if (appA2dpIsSinkCodec(peerInst))
        {
            /* Get CODEC capabilities of other A2DP instance (sink) */
            a2dp_codec_settings *codec_settings = A2dpCodecGetSettings(peerInst->a2dp_device_id, peerInst->a2dp_stream_id);

            DEBUG_LOGF("appA2dpHandleA2dpCodecConfigureIndication(%p), local SEID %d, sink SEID %d",
                      (void *)theInst, ind->local_seid, codec_settings->seid);

            /* Check SEID of source is compatible with non-TWS sink */
            if (codec_settings && appA2dpConvertSeidFromSourceToSink(codec_settings->seid) == ind->local_seid)
            {
                uint8 *sink_caps = codec_settings->configured_codec_caps;
                uint16 size_sink_caps = codec_settings->size_configured_codec_caps;

                /* Get CODEC capabilities of the TWS source */
                uint8 *src_caps = ind->codec_service_caps;
                uint16 size_src_caps = ind->size_codec_service_caps;

                switch (ind->local_seid)
                {
                    case AV_SEID_APTX_MONO_TWS_SRC:
                    {
                        /* Find AVDTP_SERVICE_MEDIA_CODEC service category in TWS source CODEC capabilites */
                        appA2dpFindServiceCategory(AVDTP_SERVICE_MEDIA_CODEC, &src_caps, &size_src_caps);
                        PanicFalse(src_caps);

                        /* Configure sample rate */
                        appAvUpdateAptxMonoTwsCapabilities(src_caps, codec_settings->rate);

                        /* Pass updated CODEC capabilties to A2DP library */
                        PanicFalse(A2dpCodecConfigureResponse(ind->device_id, TRUE, ind->local_seid,
                                                              ind->size_codec_service_caps, ind->codec_service_caps));
                    }
                    break;


                    case AV_SEID_SBC_MONO_TWS_SRC:
                    {
                        /* Find AVDTP_SERVICE_MEDIA_CODEC service category in TWS source CODEC capabilites */
                        appA2dpFindServiceCategory(AVDTP_SERVICE_MEDIA_CODEC, &src_caps, &size_src_caps);
                        PanicFalse(src_caps);

                        /* Configure sample rate */
                        appAvUpdateSbcMonoTwsCapabilities(src_caps, codec_settings->rate);

//                        DEBUG_PRINTF("appA2dpHandleA2dpCodecConfigureIndication(%p)", theInst);
//                        for (int i = 0; i < size_src_caps; i++)
//                            DEBUG_PRINTF(" %02x", src_caps[i]);
//                        DEBUG_PRINT("");

                        /* Pass updated CODEC capabilties to A2DP library */
                        PanicFalse(A2dpCodecConfigureResponse(ind->device_id, TRUE, ind->local_seid,
                                                              ind->size_codec_service_caps, ind->codec_service_caps));
                    }
                    break;

                    case AV_SEID_SBC_SRC:
                    {
                        /* Find AVDTP_SERVICE_MEDIA_CODEC service category in source CODEC capabilites */
                        appA2dpFindServiceCategory(AVDTP_SERVICE_MEDIA_CODEC, &src_caps, &size_src_caps);
                        if (src_caps)
                        {
                            /* Copy AVDTP_SERVICE_MEDIA_CODEC from sink capabilties to source capabilities */
                            appA2dpCopyServiceCategory(AVDTP_SERVICE_MEDIA_CODEC, sink_caps, size_sink_caps, src_caps, size_src_caps);
                        }

                        /* Pass updated CODEC capabilties to A2DP library */
                        PanicFalse(A2dpCodecConfigureResponse(ind->device_id, TRUE, ind->local_seid,
                                                              ind->size_codec_service_caps, ind->codec_service_caps));
                    }
                    break;
                }
            }
            else
            {
                /* SEID of source and sink incompatible, reject and allow A2DP to chose next source SEID */
                A2dpCodecConfigureResponse(theInst->a2dp_device_id, FALSE, ind->local_seid, 0, NULL);
            }

            /* Free structure now that we're done with it */
            free(codec_settings);
        }
        else
        {
            /* No streaming on sink instance, keep rejecting to prevent media channel being setup */
            A2dpCodecConfigureResponse(theInst->a2dp_device_id, FALSE, ind->local_seid, 0, NULL);
        }
    }
    else
    {
        /* No sink instance, keep rejecting to prevent media channel being setup */
        A2dpCodecConfigureResponse(theInst->a2dp_device_id, FALSE, ind->local_seid, 0, NULL);
    }
}

/*! \brief Handle media channel closed remotely

    The peer device has disconnected the media channel, move back to the
    'connected signalling' state, the state machine entry & exit functions
    will automatically disconnect the DACs & DSP if required.
*/	
static void appA2dpHandleA2dpMediaCloseIndication(avInstanceTaskData *theInst,
                                                  const A2DP_MEDIA_CLOSE_IND_T *ind)
{
    assert(theInst->a2dp_device_id == ind->device_id);
    DEBUG_LOGF("appA2dpHandleA2dpMediaCloseIndication(%p), status %d", (void *)theInst, ind->status);

    /* Handle different states */
    switch (appA2dpGetState(theInst))
    {
        case A2DP_STATE_CONNECTED_SIGNALLING:
        case A2DP_STATE_CONNECTING_MEDIA_LOCAL:
        case A2DP_STATE_CONNECTING_MEDIA_REMOTE:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING:
        case A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_SUSPENDED:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC:
        case A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC:
        case A2DP_STATE_DISCONNECTING_MEDIA:
        {
            /* Move to 'connected signalling' state */
            appA2dpSetState(theInst, A2DP_STATE_CONNECTED_SIGNALLING);
        }
        return;
            
        case A2DP_STATE_CONNECTING_LOCAL:
        case A2DP_STATE_CONNECTING_REMOTE:
        case A2DP_STATE_DISCONNECTED:
        {
            /* Probably late message from A2DP profile library, just ignore */
        }
        return;
            
        case A2DP_STATE_DISCONNECTING:
        {
            /* Media channel disconnected, wait for signalling channel to disconnect */
        }
        return;

        default:
            appA2dpError(theInst, A2DP_MEDIA_CLOSE_IND, ind);
            return;
    }
}

/*! \brief Handle media av sync delay indication

    Causes a delay report to be issued to a connected Source.
*/
static void appA2dpHandleA2dpMediaAvSyncDelayIndication(avInstanceTaskData *theInst,
                                                        const A2DP_MEDIA_AV_SYNC_DELAY_IND_T *ind)
{
    assert(theInst->a2dp_device_id == ind->device_id);
    DEBUG_LOGF("appA2dpHandleA2dpMediaAvSyncDelayIndication(%p), seid %d", (void *)theInst, ind->seid);

    if (appA2dpIsConnected(theInst))
        A2dpMediaAvSyncDelayResponse(ind->device_id, ind->seid, TWS_STANDARD_LATENCY_MS*10);
    else
        appA2dpError(theInst, A2DP_MEDIA_AV_SYNC_DELAY_IND, ind);
}


/*! \brief Handle internal indication that AVRCP is unlocked

    This function is called when we have determined that the AVRCP library is no
    longer locked, we need to make sure AVRCP is unlocked in case there is
    a passthrough command in the process of being sent.
*/	
static void appA2dpHandleInternalAvrcpUnlockInd(avInstanceTaskData *theInst)
{
    /* AVRCP is now unlocked, we can proceed with the disconnect */
    A2dpSignallingDisconnectRequest(theInst->a2dp_device_id);
}

/*! \brief Initialise AV instance

    This function initialises the specified AV instance, all state variables are
    set to defaults, with the exception of the streaming state which is 
    initialised with the value supplied. If non-zero this has the effect of
    blocking streaming initially.

    \note This function should only be called on a newly created
    instance.

    \param theAv            The AV that has the A2DP instance to initialise
    \param suspend_state    The initial suspend_state to apply 
 */
void appA2dpInstanceInit(avInstanceTaskData *theAv, uint8 suspend_state)
{
    theAv->a2dp_state = A2DP_STATE_DISCONNECTED;
    theAv->a2dp_current_seid = AV_SEID_INVALID;
    theAv->a2dp_flags = 0;
    theAv->a2dp_lock = 0;
    theAv->a2dp_num_seids = 0;
    theAv->a2dp_volume = 0;
    theAv->a2dp_suspend_state = suspend_state;   
    theAv->a2dp_local_initiated = FALSE;
    theAv->a2dp_disconnect_reason = AV_A2DP_DISCONNECT_NORMAL;

    /* No profile instance yet */
    theAv->a2dp_device_id = INVALID_DEVICE_ID;
}

/*! \brief Set A2DP volume

    Update the playback volume if the supplied AV has an active A2DP Sink.

    Initiate saving of the playback volume for use on subsequent connections
    or after the earbud restarts. The volume is not saved immediately
    as volume is often adjusted several times consecutively.

    \param theInst  The AV instance with an A2DP link
    \param volume   The volume to use
*/
void appA2dpVolumeSet(avInstanceTaskData *theInst, uint16 volume)
{
    PanicFalse(appAvIsValidInst(theInst));

    /* Check if this instance is a A2DP sink */
    if (appA2dpIsSinkCodec(theInst))
    {
        /* We're a sink so just set the CODEC volume immediately */
        appKymeraA2dpSetVolume(volume);
    }

    /* Cancel any pending messages */
    MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_VOLUME_STORE_REQ);

    /* Store configuration after a 5 seconds */
    MessageSendLater(&theInst->av_task, AV_INTERNAL_A2DP_VOLUME_STORE_REQ, 0, D_SEC(5));
}


/*! \brief Message Handler

    This function is the main message handler for an A2DP instance, every
    message is handled in it's own seperate handler function.  The switch
    statement is broken into seperate blocks to reduce code size, if execution
    reaches the end of the function then it is assumed that the message is
    unhandled.

    \param theInst      The instance data for the AV for this A2DP connection
    \param id           Message identifier. For internal messages, see \ref av_headset_internal_messages
    \param[in] message  Message content, potentially NULL.

*/
void appA2dpInstanceHandleMessage(avInstanceTaskData *theInst, MessageId id, Message message)
{
    /* Handle internal messages */
    switch (id)
    {
        case AV_INTERNAL_A2DP_SIGNALLING_CONNECT_IND:           
            appA2dpHandleInternalA2dpSignallingConnectIndication(theInst, (AV_INTERNAL_A2DP_SIGNALLING_CONNECT_IND_T *)message);
            return;
        
        case AV_INTERNAL_A2DP_CONNECT_REQ:
            appA2dpHandleInternalA2dpConnectRequest(theInst, (AV_INTERNAL_A2DP_CONNECT_REQ_T *)message);
            return;

        case AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ:
            appA2dpHandleInternalA2dpConnectMediaRequest(theInst, (AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ_T *)message);
            return;
        
        case AV_INTERNAL_A2DP_DISCONNECT_MEDIA_REQ:
            appA2dpHandleInternalA2dpDisconnectMediaRequest(theInst);
            return;
        
        case AV_INTERNAL_A2DP_DISCONNECT_REQ:
            appA2dpHandleInternalA2dpDisconnectRequest(theInst, (AV_INTERNAL_A2DP_DISCONNECT_REQ_T *)message);
            return;
        
        case AV_INTERNAL_A2DP_SUSPEND_MEDIA_REQ:
            appA2dpHandleInternalA2dpSuspendRequest(theInst, (AV_INTERNAL_A2DP_SUSPEND_MEDIA_REQ_T *)message);
            return;

        case AV_INTERNAL_A2DP_RESUME_MEDIA_REQ:
            appA2dpHandleInternalA2dpResumeRequest(theInst, (AV_INTERNAL_A2DP_RESUME_MEDIA_REQ_T *)message);
            return;

        case AV_INTERNAL_A2DP_SYNC_MEDIA_IND:
            appA2dpHandleInternalA2dpSyncMediaIndication(theInst);
            return;

        case AV_INTERNAL_AVRCP_UNLOCK_IND:
            appA2dpHandleInternalAvrcpUnlockInd(theInst);
            return;

        case AV_INTERNAL_A2DP_DESTROY_REQ:
            appAvInstanceDestroy(theInst);
            return;

        case AV_INTERNAL_A2DP_VOLUME_STORE_REQ:
            appA2dpVolumeAttributeStore(theInst);
            return;
    }
    
    /* Handle A2DP library messages */
    switch (id)
    {
        case A2DP_SIGNALLING_CONNECT_IND:
            appA2dpHandleA2dpSignallingConnectIndication(theInst, (A2DP_SIGNALLING_CONNECT_IND_T *)message);
            return;

        case A2DP_SIGNALLING_CONNECT_CFM:
            appA2dpHandleA2dpSignallingConnectConfirm(theInst, (A2DP_SIGNALLING_CONNECT_CFM_T *)message);
            return;
        
        case A2DP_SIGNALLING_DISCONNECT_IND:
            appA2dpHandleA2dpSignallingDisconnectInd(theInst, (A2DP_SIGNALLING_DISCONNECT_IND_T *)message);
            return;
        
        case A2DP_MEDIA_OPEN_IND:
            appA2dpHandleA2dpMediaOpenIndication(theInst, (A2DP_MEDIA_OPEN_IND_T *)message);
            return;
                        
        case A2DP_MEDIA_OPEN_CFM:
            appA2dpHandleA2dpMediaOpenConfirm(theInst, (A2DP_MEDIA_OPEN_CFM_T *)message);
            return;
                       
        case A2DP_MEDIA_START_IND:
            appA2dpHandleA2dpMediaStartIndication(theInst, (A2DP_MEDIA_START_IND_T *)message);
            return;

        case A2DP_MEDIA_START_CFM:
            appA2dpHandleA2dpMediaStartConfirmation(theInst, (A2DP_MEDIA_START_CFM_T *)message);
            return;
            
        case A2DP_MEDIA_SUSPEND_IND:
            appA2dpHandleA2dpMediaSuspendIndication(theInst, (A2DP_MEDIA_SUSPEND_IND_T *)message);
            return;

        case A2DP_MEDIA_SUSPEND_CFM:
            appA2dpHandleA2dpMediaSuspendConfirmation(theInst, (A2DP_MEDIA_SUSPEND_CFM_T *)message);
            return;

        case A2DP_CODEC_CONFIGURE_IND:
            appA2dpHandleA2dpCodecConfigureIndication(theInst, (A2DP_CODEC_CONFIGURE_IND_T *)message);
            return;
        
        case A2DP_MEDIA_CLOSE_IND:
            appA2dpHandleA2dpMediaCloseIndication(theInst, (A2DP_MEDIA_CLOSE_IND_T *)message);
            return;

        case A2DP_ENCRYPTION_CHANGE_IND:
        case A2DP_MEDIA_RECONFIGURE_IND:
            return;

        case A2DP_MEDIA_AV_SYNC_DELAY_IND:
            appA2dpHandleA2dpMediaAvSyncDelayIndication(theInst, (A2DP_MEDIA_AV_SYNC_DELAY_IND_T *)message);
            return;

        case A2DP_MEDIA_AV_SYNC_DELAY_CFM:
            return;
    }

    /* Unhandled message */
    appA2dpError(theInst, id, message);
}

#else
static const int compiler_happy;
#endif
