/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_avrcp.c
\brief      AVRCP State Machine
*/

/* Only compile if AV defined */
#ifdef INCLUDE_AV

/****************************************************************************
    Header files
*/
#include <avrcp.h>
#include <panic.h>
#include <connection.h>
#include <kalimba.h>
#include <kalimba_standard_messages.h>
#include <ps.h>
#include <string.h>

#include "av_headset.h"
#include "av_headset_power.h"
#include "av_headset_avrcp.h"
#include "av_headset_log.h"

/*! Macro for creating an AV message based on the message name */
#define MAKE_AV_MESSAGE(TYPE) \
    TYPE##_T *message = PanicUnlessNew(TYPE##_T);
/*! Macro for creating a variable length AV message based on the message name */
#define MAKE_AV_MESSAGE_WITH_LEN(TYPE, LEN) \
    TYPE##_T *message = PanicUnlessMalloc(sizeof(TYPE##_T) + (LEN) - 1);

/*! Code assertion that can be checked at run time. This will cause a panic. */
#define assert(x)   PanicFalse(x)

static void appAvrcpFinishAvrcpPassthroughRequest(avInstanceTaskData *theInst, avrcp_status_code status)
{
    MAKE_AV_MESSAGE(AV_AVRCP_VENDOR_PASSTHROUGH_CFM);
    message->av_instance = theInst;
    message->status = status;
    message->opid = theInst->avrcp_vendor_opid;
    MessageSend(theInst->avrcp_vendor_task, AV_AVRCP_VENDOR_PASSTHROUGH_CFM, message);

    DEBUG_LOGF("appAvrcpFinishAvrcpPassthroughRequest(%p), data %p, status %d",
               (void *)theInst,
               (void *)theInst->avrcp_vendor_data, status);

    /* Free allocated memory */
    free(theInst->avrcp_vendor_data);
    theInst->avrcp_vendor_data = NULL;
}

static void appAvrcpSetState(avInstanceTaskData *theInst, avAvrcpState avrcp_state);

/*! \brief Forward AVRCP_PASSTHROUGH_IND from Slave Headset to AV Source 
*/    
static void appAvrcpForwardAvrcpPassthroughIndication(avInstanceTaskData *theInst, const AVRCP_PASSTHROUGH_IND_T *ind)
{
    /* Check if we are the A2DP source */
    if (appA2dpIsSourceCodec(theInst))
    {		
        avInstanceTaskData *av_inst;

        /* Find another connected A2DP instance */
        av_inst = appAvInstanceFindA2dpState(theInst,
                                             A2DP_STATE_MASK_CONNECTED_SIGNALLING,
                                             A2DP_STATE_CONNECTED_SIGNALLING);
                                    
        /* Check of A2DP instance found */
        if (av_inst)
        {
            /* Send AVRCP passthrough request, will be block until AVRCP connected */
            appAvrcpRemoteControl(av_inst, ind->opid, ind->state, FALSE, FALSE);        

            /* Accept passthrough indication */                                             
            AvrcpPassthroughResponse(ind->avrcp, avctp_response_accepted);
            return;
        }
    }

    /* Reject passthrough indication */                                             
    AvrcpPassthroughResponse(ind->avrcp, avctp_response_rejected);
}

/*! \brief Enter 'connecting local' state

    The AVRCP state machine has entered 'connecting local' state, set the
    'connect busy' flag to serialise connect attempts.
*/    
static void appAvrcpEnterConnectingLocal(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appAvrcpEnterConnectingLocal(%p)", (void *)theInst);
}

/*! \brief Exit 'connecting local' state

    The AVRCP state machine has exited 'connecting local' state, clear the
    'connect busy' flag to allow pending connection attempts to proceed.
*/    
static void appAvrcpExitConnectingLocal(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appAvrcpExitConnectingLocal(%p)", (void *)theInst);

    /* We have finished (successfully or not) attempting to connect, so
     * we can relinquish our lock on the ACL.  Bluestack will then close
     * the ACL when there are no more L2CAP connections */
    appConManagerReleaseAcl(&theInst->bd_addr);
}

/*! \brief Enter 'connecting local wait response' state

    The AVRCP state machine has entered 'connecting local response' state, set the
    'connect busy' flag to serialise connect attempts.
*/
static void appAvrcpEnterConnectingLocalWaitResponse(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appAvrcpEnterConnectingLocalWaitResponse(%p)", (void *)theInst);
}

/*! \brief Exit 'connecting local response' state

    The AVRCP state machine has exited 'connecting local response' state, clear the
    'connect busy' flag to allow pending connection attempts to proceed.
*/
static void appAvrcpExitConnectingLocalWaitResponse(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appAvrcpExitConnectingLocalWaitResponse(%p)", (void *)theInst);
}

static void appAvrcpEnterConnectingRemote(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appAvrcpEnterConnectingRemote(%p)", (void *)theInst);
}

static void appAvrcpExitConnectingRemote(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appAvrcpExitConnectingRemote(%p)", (void *)theInst);
}

/*! \brief Enter 'connecting remote wait response' state

    The AVRCP state machine has entered 'connecting remote response' state, set the
    'connect busy' flag to serialise connect attempts.
*/
static void appAvrcpEnterConnectingRemoteWaitResponse(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appAvrcpEnterConnectingRemoteWaitResponse(%p)", (void *)theInst);
}

/*! \brief Exit 'connecting remote response' state

    The AVRCP state machine has exited 'connecting remote response' state, clear the
    'connect busy' flag to allow pending connection attempts to proceed.
*/
static void appAvrcpExitConnectingRemoteWaitResponse(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appAvrcpExitConnectingRemoteWaitResponse(%p)", (void *)theInst);
}

/*! \brief Enter 'connected' state

    The AVRCP state machine has entered 'connected' state, this means
    that the AVRCP control channel has been established.
    
    Kick the link policy manager to make sure this link is configured correctly
    and to maintain the correct link topology.	
*/    
static void appAvrcpEnterConnected(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appAvrcpEnterConnected(%p)", (void *)theInst);

    /* Mark this device as supporting AVRCP */
    appDeviceSetAvrcpIsSupported(&theInst->bd_addr);

    /* Copy volume from attributes */
    appDeviceAttributes attributes;
    if (appDeviceFindBdAddrAttributes(&theInst->bd_addr, &attributes))
    {
        theInst->a2dp_volume = attributes.a2dp_volume;
    }
    /* Attempt to synchronise volumes */
    appAvVolumeSync(NULL, TRUE);

    /* Tell clients we have connected */
    MAKE_AV_MESSAGE(AV_AVRCP_CONNECTED_IND);
    message->av_instance = theInst;
    message->bd_addr = theInst->bd_addr;
    message->sink = AvrcpGetSink(theInst->avrcp);
    appTaskListMessageSend(theInst->av_status_client_list, AV_AVRCP_CONNECTED_IND, message);

    /* Get notified of change in playback status */
    AvrcpRegisterNotificationRequest(theInst->avrcp, avrcp_event_playback_status_changed, 0);

    /* Get notified of change in volume if peer */
    if (appDeviceIsPeer(&theInst->bd_addr))
        AvrcpRegisterNotificationRequest(theInst->avrcp, avrcp_event_volume_changed, 0);
}

/*! \brief Exit 'connected' state

    The AVRCP state machine has exited 'connected' state, this means
    that the AVRCP control channel has closed.	
*/    
static void appAvrcpExitConnected(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appAvrcpExitConnected(%p)", (void *)theInst);

    /* Tell clients we have disconnected */
    MAKE_AV_MESSAGE(AV_AVRCP_DISCONNECTED_IND);
    message->av_instance = theInst;
    message->bd_addr = theInst->bd_addr;
    appTaskListMessageSend(theInst->av_status_client_list, AV_AVRCP_DISCONNECTED_IND, message);
}

/*! \brief Enter 'disconnecting' state

    The AVRCP state machine has entered 'disconnecting' state, this means
    we are about to disconnect the AVRCP connection, set the operation lock
    to block any other operations until the disconnection is completed.
*/    
static void appAvrcpEnterDisconnecting(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appAvrcpEnterDisconnecting(%p)", (void *)theInst);

    /* Close all connections */
    AvrcpDisconnectRequest(theInst->avrcp);
}

/*! \brief Exit 'disconnecting' state

    The AVRCP state machine has exit 'disconnecting' state, this means
    we have now disconnected the AVRCP connection, clear the operation lock
    so that any blocked operations can now proceed.
*/    
static void appAvrcpExitDisconnecting(avInstanceTaskData *theInst)
{
    UNUSED(theInst);
    DEBUG_LOGF("appAvrcpExitDisconnecting(%p)", (void *)theInst);
}

/*! \brief Enter 'disconnected' state

    The AVRCP state machine has entered 'disconnected' state, this means there
    is now no AVRCP connection.  Cancel any request for remote control commands and
    clear the AVRCP profile library instance pointer.  Normally we stay in this
    state until an AV_INTERNAL_AVRCP_DESTROY_REQ or AV_INTERNAL_AVRCP_CONNECT_REQ
    message is received.
*/    
static void appAvrcpEnterDisconnected(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appAvrcpEnterDisconnected(%p)", (void *)theInst);
    
    /* Cancel in-progress vendor passthrough command */
    if (theInst->avrcp_vendor_data)
        appAvrcpFinishAvrcpPassthroughRequest(theInst, avrcp_fail);

    /* Clear any queued up messages */
    MessageCancelAll(&theInst->av_task, AV_INTERNAL_AVRCP_REMOTE_REQ);

    /* Clear AVRCP pointer */
    theInst->avrcp = NULL;

    /* Clear client list and any locks */
    appTaskListDestroy(theInst->avrcp_client_list);
    theInst->avrcp_client_list = appTaskListInit();
    theInst->avrcp_client_lock = 0;

    /* Send ourselves a destroy message so that any other messages waiting on the
       operation lock can be handled */
    MessageSendConditionally(&theInst->av_task, AV_INTERNAL_AVRCP_DESTROY_REQ, NULL, &appAvrcpGetLock(theInst));
}

/*! \brief Exit 'disconnected' state

    The AVRCP state machine has exited 'disconnected' state, this normally
    happend when an AV_INTERNAL_AVRCP_CONNECT_REQ message is received.
    Make sure that any queued AV_INTERNAL_AVRCP_DESTROY_REQ are destroyed.
*/    
static void appAvrcpExitDisconnected(avInstanceTaskData *theInst)
{
    DEBUG_LOGF("appAvrcpExitDisconnected(%p)", (void *)theInst);

    /* Cancel any internal connect/destroy messages */
    MessageCancelFirst(&theInst->av_task, AV_INTERNAL_AVRCP_DESTROY_REQ);
    if (MessageCancelAll(&theInst->av_task, AV_INTERNAL_AVRCP_CONNECT_REQ))
    {
        DEBUG_LOGF("appAvrcpExitDisconnected(%p), cancelling deferred connect req", (void *)theInst);
    }
}

/*! \brief Set AVRCP state

    Called to change state.  Handles calling the state entry and exit functions.
*/
static void appAvrcpSetState(avInstanceTaskData *theInst, avAvrcpState avrcp_state)
{
    avAvrcpState avrcp_old_state = theInst->avrcp_state;
    DEBUG_LOGF("appAvrcpSetState(%p) state %d", (void *)theInst, avrcp_state);

    /* Handle state exit functions */
    switch (avrcp_old_state)
    {
        case AVRCP_STATE_DISCONNECTED:
            appAvrcpExitDisconnected(theInst);
            break;
        case AVRCP_STATE_CONNECTING_LOCAL:
            appAvrcpExitConnectingLocal(theInst);
            break;
        case AVRCP_STATE_CONNECTING_LOCAL_WAIT_RESPONSE:
            appAvrcpExitConnectingLocalWaitResponse(theInst);
            break;
        case AVRCP_STATE_CONNECTING_REMOTE:
            appAvrcpExitConnectingRemote(theInst);
            break;
        case AVRCP_STATE_CONNECTING_REMOTE_WAIT_RESPONSE:
            appAvrcpExitConnectingRemoteWaitResponse(theInst);
            break;
        case AVRCP_STATE_CONNECTED:
            appAvrcpExitConnected(theInst);
            break;
        case AVRCP_STATE_DISCONNECTING:
            appAvrcpExitDisconnecting(theInst);
            break;
        default:
            break;
    }
                    
    /* Set new state */
    theInst->avrcp_state = avrcp_state;

    /* Update lock according to state */
    if (avrcp_state & AVRCP_STATE_LOCK)
        appAvrcpSetLock(theInst, APP_AVRCP_LOCK_STATE);
    else
        appAvrcpClearLock(theInst, APP_AVRCP_LOCK_STATE);

    /* Handle state entry functions */
    switch (avrcp_state)
    {
        case AVRCP_STATE_DISCONNECTED:
            appAvrcpEnterDisconnected(theInst);
            break;
        case AVRCP_STATE_CONNECTING_LOCAL:
            appAvrcpEnterConnectingLocal(theInst);
            break;
        case AVRCP_STATE_CONNECTING_LOCAL_WAIT_RESPONSE:
            appAvrcpEnterConnectingLocalWaitResponse(theInst);
            break;
        case AVRCP_STATE_CONNECTING_REMOTE:
            appAvrcpEnterConnectingRemote(theInst);
            break;
        case AVRCP_STATE_CONNECTING_REMOTE_WAIT_RESPONSE:
            appAvrcpEnterConnectingRemoteWaitResponse(theInst);
            break;
        case AVRCP_STATE_CONNECTED:
            appAvrcpEnterConnected(theInst);
            break;
        case AVRCP_STATE_DISCONNECTING:
            appAvrcpEnterDisconnecting(theInst);
            break;
        default:
            break;
    } 
}

/*! \brief Get the current AVRCP state of this AV instance

    \param[in] theAv    Instance to get the state of

    \returns Current state
*/
avAvrcpState appAvrcpGetState(avInstanceTaskData *theAv)
{
    PanicNull(theAv);
    return theAv->avrcp_state;
}

/*! \brief Handle AVRCP error

    Some error occurred in the AVRCP state machine, to avoid the state machine
    getting stuck, drop connection and move to 'disconnected' state.
*/	
static void appAvrcpError(avInstanceTaskData *theInst, MessageId id, Message message)
{
    UNUSED(message); UNUSED(theInst); UNUSED(id);

#ifdef AV_DEBUG
    DEBUG_LOGF("appAvrcpError(%p) state %u, id %x", (void *)theInst, theInst->avrcp_state, id);
#else
    Panic();
#endif
}

/*! \brief Request AVRCP remote control 

    Handle internal message to send passthrough command, only handle the request
    in the 'connected' state.

    If from_repeat_message is TRUE, this came from a 
    AV_INTERNAL_AVRCP_REMOTE_REPEAT_REQ message.
*/    
static void appAvrcpHandleInternalAvrcpRemoteRequest(avInstanceTaskData *theInst,
                                                     AV_INTERNAL_AVRCP_REMOTE_REQ_T *req,
                                                     bool from_repeat_message)
{
    DEBUG_LOGF("appAvrcpHandleInternalAvrcpRemoteRequest(%p)", (void *)theInst);

    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
            /* UI indication if remote control press */
            if (req->ui)
                appUiAvRemoteControl();

            /* Set operation lock */
            appAvrcpSetLock(theInst, APP_AVRCP_LOCK_PASSTHROUGH_REQ);

            /* Store OPID so that we know what operation the AVRCP_PASSTHROUGH_CFM is for */
            theInst->avrcp_op_id = req->op_id;
            theInst->avrcp_op_state = req->state;
            theInst->avrcp_op_repeat = from_repeat_message;
            
            /* Send remote control */
            AvrcpPassthroughRequest(theInst->avrcp, subunit_panel, 0, req->state, req->op_id, 0, 0);

            /* Repeat message every second */
            MessageCancelFirst(&theInst->av_task, AV_INTERNAL_AVRCP_REMOTE_REPEAT_REQ);
            if (!req->state && req->repeat_ms)
            {
                /* Send internal message */
                MAKE_AV_MESSAGE(AV_INTERNAL_AVRCP_REMOTE_REPEAT_REQ);
                message->op_id = req->op_id;
                message->repeat_ms = req->repeat_ms;
                message->state = req->state;
                message->ui = FALSE;
                MessageSendLater(&theInst->av_task, AV_INTERNAL_AVRCP_REMOTE_REPEAT_REQ, message, req->repeat_ms);
            }
        }
        return;
        
        default:
            /* Ignore in any other states */
            return;
    }
}

static void appAvrcpHandleInternalAvrcpVolumeNotifyRequest(avInstanceTaskData *theInst,
                                                            const AV_INTERNAL_AVRCP_VOLUME_NOTIFY_REQ_T *req)
{
    DEBUG_LOGF("appAvrcpHandleInternalAvrcpVolumeNotifyRequest(%p), volume %d", theInst, req->volume);
    AvrcpEventVolumeChangedResponse(theInst->avrcp, avctp_response_changed, req->volume);
}


static void appAvrcpAddClient(avInstanceTaskData *theInst, Task client)
{
    appTaskListAddTask(theInst->avrcp_client_list, client);
}


static bool appAvrcpRemoveClient(avInstanceTaskData *theInst, Task client_task)
{
    return appTaskListRemoveTask(theInst->avrcp_client_list, client_task);
}

/*! \brief Request outgoing AVRCP connection

    Handle AVRCP connect request from client task, call AvrcpConnectRequest()
    to create AVRCP channel and move into the 'connecting local' state.
*/

static void appAvrcpHandleInternalAvrcpConnectRequest(avInstanceTaskData *theInst, AV_INTERNAL_AVRCP_CONNECT_REQ_T *req)
{
    DEBUG_LOGF("appAvrcpHandleInternalAvrcpConnectRequest(%p) %x,%x,%lx, clients %u, locks %u",
                 (void *)theInst,
                 theInst->bd_addr.nap, theInst->bd_addr.uap, theInst->bd_addr.lap,
                 appTaskListSize(theInst->avrcp_client_list),
                 theInst->avrcp_client_lock);

    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_DISCONNECTED:
        {
            /* Check ACL is connected */
            if (appConManagerIsConnected(&theInst->bd_addr))
            {
                Task client_task = 0;

                if (req->delay_ms)
                {
                    MAKE_AV_MESSAGE(AV_INTERNAL_AVRCP_CONNECT_REQ);
                    /* Send message to newly created AV instance to connect AVRCP */
                    message->client_task = req->client_task;
                    message->delay_ms = 0;
                    MessageSendLater(&theInst->av_task, AV_INTERNAL_AVRCP_CONNECT_REQ, message, req->delay_ms);
                    DEBUG_LOGF("appAvrcpHandleInternalAvrcpConnectRequest(%p) deferring for %ums", theInst, req->delay_ms);
                    return;
                }

                /* Copy avrcp client list to instance */
                appTaskListDestroy(theInst->avrcp_client_list);
                theInst->avrcp_client_list = appTaskListDuplicate(appGetAv()->avrcp_client_list);

                /* Add client to list, increment lock count */
                appAvrcpAddClient(theInst, req->client_task);
                theInst->avrcp_client_lock += 1;

                DEBUG_LOGF("appAvrcpHandleInternalAvrcpConnectRequest(%p) added client %p, clients %u, locks %u",
                             (void *)theInst,
                             req->client_task,
                             appTaskListSize(theInst->avrcp_client_list),
                             theInst->avrcp_client_lock);

                /* Send AV_AVRCP_CONNECT_IND to all clients apart from the one that requested the connection */
                theInst->avrcp_client_responses = 0;
                while (appTaskListIterate(theInst->avrcp_client_list, &client_task))
                {
                    if (client_task != req->client_task)
                    {
                        MAKE_AV_MESSAGE(AV_AVRCP_CONNECT_IND);
                        message->av_instance = theInst;
                        message->connection_id = 0;
                        message->signal_id = 0;
                        message->bd_addr = theInst->bd_addr;
                        MessageSend(client_task, AV_AVRCP_CONNECT_IND, message);
                        theInst->avrcp_client_responses += 1;
                    }
                }

                /* Check if we actually sent any AV_AVRCP_CONNECT_IND messages */
                if (theInst->avrcp_client_responses)
                {
                    /* Move to 'connecting local wait response' state to wait for client reponses */
                    appAvrcpSetState(theInst, AVRCP_STATE_CONNECTING_LOCAL_WAIT_RESPONSE);
                }
                else
                {
                    /* Request outgoing connection */
                    if (AvrcpConnectRequest(&theInst->av_task, &theInst->bd_addr))
                    {
                        /* Move to 'connecting local' state */
                        appAvrcpSetState(theInst, AVRCP_STATE_CONNECTING_LOCAL);
                    }
                    else
                        Panic();
                }
            }
            else
            {
                DEBUG_LOGF("appAvrcpHandleInternalAvrcpConnectRequest(%p) no ACL %x,%x,%lx,", (void *)theInst,
                           theInst->bd_addr.nap, theInst->bd_addr.uap, theInst->bd_addr.lap);

                /* Send AV_AVRCP_CONNECT_CFM to all clients */
                MAKE_AV_MESSAGE(AV_AVRCP_CONNECT_CFM);
                message->av_instance = theInst;
                message->status = avrcp_device_not_connected;
                MessageSend(req->client_task, AV_AVRCP_CONNECT_CFM, message);

                /* Move to 'disconnected' state */
                appAvrcpSetState(theInst, AVRCP_STATE_DISCONNECTED);
            }
        }
        return;

        case AVRCP_STATE_CONNECTED:
        {
            /* Add client to list */
            appAvrcpAddClient(theInst, req->client_task);
            theInst->avrcp_client_lock += 1;

            DEBUG_LOGF("appAvrcpHandleInternalAvrcpConnectRequest(%p) added client %p, clients %u, locks %u",
                         (void *)theInst,
                         req->client_task,
                         appTaskListSize(theInst->avrcp_client_list),
                         theInst->avrcp_client_lock);

            /* Send confirm immediately */
            MAKE_AV_MESSAGE(AV_AVRCP_CONNECT_CFM);
            message->status = avrcp_success;
            message->av_instance = theInst;
            MessageSend(req->client_task, AV_AVRCP_CONNECT_CFM, message);
        }
        return;

        case AVRCP_STATE_CONNECTING_LOCAL:
        case AVRCP_STATE_CONNECTING_LOCAL_WAIT_RESPONSE:
        case AVRCP_STATE_CONNECTING_REMOTE:
        case AVRCP_STATE_CONNECTING_REMOTE_WAIT_RESPONSE:
        case AVRCP_STATE_DISCONNECTING:
        {
            /* Should never receive in these states as AVRCP lock is set */
            Panic();
        }
        return;


        default:
            appAvrcpError(theInst, AV_INTERNAL_AVRCP_CONNECT_REQ, NULL);
            return;
    }
}

/*! \brief Request AVRCP disconnection

    Handle AVRCP disconnect request from AV parent task.  Move into the
    'disconnecting' state, this will initiate the disconnect.
*/
static void appAvrcpHandleInternalAvrcpDisconnectRequest(avInstanceTaskData *theInst, AV_INTERNAL_AVRCP_DISCONNECT_REQ_T *req)
{
    DEBUG_LOGF("appAvrcpHandleInternalAvrcpDisconnectRequest(%p) removing client %p, clients %u, locks %u",
               (void *)theInst,
               req->client_task,
               appTaskListSize(theInst->avrcp_client_list),
               theInst->avrcp_client_lock);

    /* Remove client from list, decrement lock count */
    if (appAvrcpRemoveClient(theInst, req->client_task))
    {
        theInst->avrcp_client_lock -= 1;
        DEBUG_LOGF("appAvrcpHandleInternalAvrcpDisconnectRequest(%p) removed client %p, clients %u, locks %u",
                   (void *)theInst,
                   req->client_task,
                   appTaskListSize(theInst->avrcp_client_list),
                   theInst->avrcp_client_lock);
    }
    else
        DEBUG_LOGF("appAvrcpHandleInternalAvrcpDisconnectRequest(%p) client %p not found",
                   (void *)theInst,
                   req->client_task);

    /* If no locks left we can start disconnection */
    if (theInst->avrcp_client_lock == 0)
    {
        switch (appAvrcpGetState(theInst))
        {
            case AVRCP_STATE_CONNECTED:
            {
                DEBUG_LOGF("appAvrcpHandleInternalAvrcpDisconnectRequest(%p) disconnecting AVRCP",
                           theInst);

                /* Move to 'disconnecting' state */
                appAvrcpSetState(theInst, AVRCP_STATE_DISCONNECTING);
            }
            return;

            case AVRCP_STATE_DISCONNECTED:
                /* Connection cross-over occured, ignore */
                return;

            default:
                appAvrcpError(theInst, AV_INTERNAL_AVRCP_DISCONNECT_REQ, NULL);
                return;
        }
    }
}

/*! \brief Accept incoming AVRCP connection 

    AVRCP Library has indicating an incoming AVRCP connection, if we're currently in the
    'disconnected' state accept the connection and move into the 'connecting remote' state, otherwise
    reject the connection.
*/    
static void appAvrcpHandleInternalAvrcpConnectIndication(avInstanceTaskData *theInst,
                                                         const AV_INTERNAL_AVRCP_CONNECT_IND_T *ind)
{    
    assert(theInst->avrcp == NULL);
    DEBUG_LOGF("appAvrcpHandleInternalAvrcpConnectIndication(%p) clients %d",
                 (void *)theInst,
                 appTaskListSize(theInst->avrcp_client_list));
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_DISCONNECTED:
        {
            /* Set remotely initiated flag, to prevent disconnect when all clients disconnect */
            theInst->avrcp_remotely_initiated = TRUE;

            /* Copy avrcp client list to instance */
            appTaskListDestroy(theInst->avrcp_client_list);
            theInst->avrcp_client_list = appTaskListDuplicate(appGetAv()->avrcp_client_list);

            /* Send AV_AVRCP_CONNECT_IND if we have and clients registered (and wait for their responses),
             * other wise accept connection */
            if (appTaskListSize(theInst->avrcp_client_list))
            {
                /* Send AVRCP_CONNECT_IND to all clients */
                MAKE_AV_MESSAGE(AV_AVRCP_CONNECT_IND);
                message->av_instance = theInst;
                message->connection_id = ind->connection_id;
                message->signal_id = ind->signal_id;
                message->bd_addr = theInst->bd_addr;
                appTaskListMessageSend(theInst->avrcp_client_list, AV_AVRCP_CONNECT_IND, message);

                /* Set counter for the number of responses we're expecting */
                theInst->avrcp_client_responses = appTaskListSize(theInst->avrcp_client_list);

                /* Move to 'connecting remote wait response' state to wait for client responses */
                appAvrcpSetState(theInst, AVRCP_STATE_CONNECTING_REMOTE_WAIT_RESPONSE);
            }
            else
            {
                /* Accept incoming connection */
                AvrcpConnectResponse(&theInst->av_task, ind->connection_id, ind->signal_id, TRUE, &theInst->bd_addr);

                /* Move to 'connecting remote' state */
                appAvrcpSetState(theInst, AVRCP_STATE_CONNECTING_REMOTE);
            }
        }
        return;
        
        case AVRCP_STATE_CONNECTING_LOCAL:
        case AVRCP_STATE_CONNECTING_LOCAL_WAIT_RESPONSE:
        case AVRCP_STATE_CONNECTING_REMOTE:
        case AVRCP_STATE_CONNECTING_REMOTE_WAIT_RESPONSE:
        case AVRCP_STATE_CONNECTED:
        case AVRCP_STATE_DISCONNECTING:
        {
            /* Reject incoming connection */
            AvrcpConnectResponse(&theInst->av_task, ind->connection_id, ind->signal_id, FALSE, &theInst->bd_addr);
        }
        return;
        
        default:
            appAvrcpError(theInst, AV_INTERNAL_AVRCP_CONNECT_IND, ind);
            return;
    }
}


static void appAvrcpHandleInternalAvrcpConnectResponse(avInstanceTaskData *theInst,
                                                       const AV_INTERNAL_AVRCP_CONNECT_RES_T *res)
{
    assert(theInst->avrcp == NULL);
    DEBUG_LOGF("appAvrcpHandleInternalAvrcpConnectResponse(%p), accept %d", (void *)theInst, res->accept);

    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTING_LOCAL_WAIT_RESPONSE:
        {
            /* Remove client from list if connection was rejected */
            if (res->accept == AV_AVRCP_REJECT)
                appAvrcpRemoveClient(theInst, res->client_task);

            /* Increment lock is connection was accepted */
            if (res->accept == AV_AVRCP_ACCEPT)
                theInst->avrcp_client_lock += 1;

            /* Check if we've now got responses from all clients */
            theInst->avrcp_client_responses -= 1;
            if (theInst->avrcp_client_responses == 0)
            {
                DEBUG_LOGF("appAvrcpHandleInternalAvrcpConnectResponse(%p), connect request", (void *)theInst);

                /* Request outgoing connection */
                if (AvrcpConnectRequest(&theInst->av_task, &theInst->bd_addr))
                {
                    /* Move to 'connecting local' state */
                    appAvrcpSetState(theInst, AVRCP_STATE_CONNECTING_LOCAL);
                }
                else
                    Panic();
            }
        }
        return;

        case AVRCP_STATE_CONNECTING_REMOTE_WAIT_RESPONSE:
        {
            /* Remove client from list if connection was rejected */
            if (res->accept == AV_AVRCP_REJECT)
                appAvrcpRemoveClient(theInst, res->client_task);

            /* Increment lock is connection was accepted */
            if (res->accept == AV_AVRCP_ACCEPT)
                theInst->avrcp_client_lock += 1;

            /* Check if we've now got responses from all clients */
            theInst->avrcp_client_responses -= 1;
            if (theInst->avrcp_client_responses == 0)
            {
                DEBUG_LOGF("appAvrcpHandleInternalAvrcpConnectResponse(%p), connect response", (void *)theInst);

                /* Accept incoming connection if more than one client wants the connection, otherwise reject it */
                AvrcpConnectResponse(&theInst->av_task,
                                     res->connection_id, res->signal_id,
                                     appTaskListSize(theInst->avrcp_client_list) ? TRUE : FALSE,
                                     &theInst->bd_addr);

                /* Move to 'connecting remote' state and wait for confirmation message */
                appAvrcpSetState(theInst, AVRCP_STATE_CONNECTING_REMOTE);
            }
        }
        return;

        default:
            appAvrcpError(theInst, AV_INTERNAL_AVRCP_CONNECT_RES, res);
            return;
    }
}



static void appAvrcpHandleInternalAvrcpVendorPassthroughResponse(avInstanceTaskData *theInst,
                                                                 const AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_RES_T *res)
{
    assert(theInst->avrcp != NULL);
    DEBUG_LOGF("appAvrcpHandleInternalAvrcpVendorPassthroughResponse(%p)", (void *)theInst);

    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
            AvrcpPassthroughResponse(theInst->avrcp, res->response);

        default:
            /* We can clear lock now that we've received response */
            appAvrcpClearLock(theInst, APP_AVRCP_LOCK_PASSTHROUGH_IND);
            return;
    }
}

static void appAvrcpHandleInternalAvrcpVendorPassthroughRequest(avInstanceTaskData *theInst,
                                                                AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_REQ_T *req)
{
    DEBUG_LOGF("appAvrcpHandleInternalAvrcpVendorPassthroughRequest(%p)", (void *)theInst);

    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
            const uint16 size_vendor_data = 5 + req->size_payload;
            uint8 *vendor_data = PanicUnlessMalloc(size_vendor_data);

            /* Create vendor unique passthrough command payload */
            vendor_data[0] = (appConfigIeeeCompanyId() >> 16) & 0xFF;
            vendor_data[1] = (appConfigIeeeCompanyId() >>  8) & 0xFF;
            vendor_data[2] = (appConfigIeeeCompanyId() >>  0) & 0xFF;
            vendor_data[3] = (req->op_id >> 8) & 0xFF;
            vendor_data[4] = (req->op_id >> 0) & 0xFF;
            memcpy(&vendor_data[5], req->payload, req->size_payload);

            /* Send passthrough command */
            AvrcpPassthroughRequest(theInst->avrcp, subunit_panel, 0, 0,
                                    opid_vendor_unique,
                                    size_vendor_data,
                                    StreamRegionSource(vendor_data, size_vendor_data));

            /* Set lock to prevent other passthrough requests */
            appAvrcpSetLock(theInst, APP_AVRCP_LOCK_PASSTHROUGH_REQ);

            /* Store memory block, task and op_id for use later when confirm is received */
            theInst->avrcp_vendor_data = vendor_data;
            theInst->avrcp_vendor_task = req->client_task;
            theInst->avrcp_op_id = opid_vendor_unique;
            theInst->avrcp_vendor_opid = req->op_id;
        }
        return;

        case AVRCP_STATE_DISCONNECTED:
        {
            /* No connection, so reject command immediately */
            MAKE_AV_MESSAGE(AV_AVRCP_VENDOR_PASSTHROUGH_CFM);
            message->status = avrcp_device_not_connected;
            message->opid = req->op_id;
            MessageSend(req->client_task, AV_AVRCP_VENDOR_PASSTHROUGH_CFM, message);
        }
        return;

        default:
            appAvrcpError(theInst, AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_REQ, req);
            return;
    }
}


static void appAvrcpHandleInternalPlaybackStatusUpdateIndication(avInstanceTaskData *theInst,
                                                                 AV_INTERNAL_PLAYBACK_STATUS_UPDATE_IND_T *ind)
{
    DEBUG_LOGF("appAvrcpHandleInternalPlaybackStatusUpdateIndication(%p), status %u", (void *)theInst, ind->play_status);

    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
            switch (ind->play_status)
            {
                case avrcp_play_status_stopped:
                case avrcp_play_status_paused:
                case avrcp_play_status_playing:
                    theInst->avrcp_play_status = ind->play_status;
                    break;

                default:
                    break;
            }

            if (appAvrcpIsEventRegistered(theInst, avrcp_event_playback_status_changed))
            {
                /* Only forward playback status if this is A2DP source instance */
                if (appA2dpIsSourceCodec(theInst))
                {
                    DEBUG_LOGF("appAvrcpHandleInternalPlaybackStatusUpdateIndication(%p), send status to TWS sink", theInst);
                    AvrcpEventPlaybackStatusChangedResponse(theInst->avrcp,
                                                            avctp_response_changed,
                                                            ind->play_status);
                }
            }
            else
                DEBUG_LOGF("appAvrcpHandleInternalPlaybackStatusUpdateIndication(%p), playback status not registered", theInst);
        }
        break;

        default:
            break;
    }
}


/*! \brief AVRCP connect confirmation

    AVRCP library has confirmed connection request.
    First of all check if the request was successful, if it was then we should
    store the pointer to the newly created AVRCP instance.  After this move into the
    'connected' state as we now have an active AVRCP channel.
    
    If the request was unsucessful, move back to the 'disconnected' state and
    play an error tone if this connection request was silent.  Note: Moving to
    the 'disconnected' state may result in this AV instance being free'd.
*/		
static void appAvrcpHandleAvrcpConnectConfirm(avInstanceTaskData *theInst,
                                              const AVRCP_CONNECT_CFM_T *cfm)
{
    assert(theInst->avrcp == NULL);
    DEBUG_LOGF("appAvrcpHandleAvrcpConnectConfirm(%p) status %d", (void *)theInst, cfm->status);

    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTING_LOCAL:
        case AVRCP_STATE_CONNECTING_REMOTE:
        {
            /* Send AV_AVRCP_CONNECT_CFM to all clients */
            MAKE_AV_MESSAGE(AV_AVRCP_CONNECT_CFM);
            message->av_instance = theInst;
            message->status = cfm->status;
            appTaskListMessageSend(theInst->avrcp_client_list, AV_AVRCP_CONNECT_CFM, message);

            /* Check if signalling channel created successfully */
            if (cfm->status == avrcp_success)
            {
                /* Store the A2DP task to use in controlling AVRCP library */
                theInst->avrcp = cfm->avrcp;
                
                /* Move to 'connected' state */
                appAvrcpSetState(theInst, AVRCP_STATE_CONNECTED);
            }
            else
            {
                /* Move to 'disconnected' state */
                appAvrcpSetState(theInst, AVRCP_STATE_DISCONNECTED);
            }
        }
        return;
        
        default:
            appAvrcpError(theInst, AVRCP_CONNECT_CFM, cfm);
            return;
    }
}

/*! \brief AVRCP connection disconnected

    AVRCP Library has indicated that the AVRCP connection has been
    disconnected, move to the 'disconnected' state, this will result
    in this AV instance being destroyed.
*/    
static void appAvrcpHandleAvrcpDisconnectIndication(avInstanceTaskData *theInst,
                                                    const AVRCP_DISCONNECT_IND_T *ind)
{
    DEBUG_LOGF("appAvrcpHandleAvrcpDisconnectIndication(%p)", (void *)theInst);
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTING_LOCAL:
        case AVRCP_STATE_CONNECTING_REMOTE:
        case AVRCP_STATE_CONNECTED:
        {
            /* Fall-through to next case */
        }

        case AVRCP_STATE_DISCONNECTING:
        {
            assert(theInst->avrcp == ind->avrcp);

            /* Send AV_AVRCP_DISCONNECT_IND to all clients */
            MAKE_AV_MESSAGE(AV_AVRCP_DISCONNECT_IND);
            message->av_instance = theInst;
            message->status = ind->status;
            appTaskListMessageSend(theInst->avrcp_client_list, AV_AVRCP_DISCONNECT_IND, message);

            /* We're not going to get confirmation for any passthrough request, so clear lock */
            appAvrcpClearLock(theInst, APP_AVRCP_LOCK_PASSTHROUGH_REQ);

            /* Move to 'disconnected' state */
            appAvrcpSetState(theInst, AVRCP_STATE_DISCONNECTED);    
        }
        return;

        case AVRCP_STATE_DISCONNECTED:
        {
            /* Disconnect crossover, do nothing this instance is about to be destroyed */
        }
        return;

        default:
            appAvrcpError(theInst, AVRCP_DISCONNECT_IND, ind);
            return;
    }
}

/*! \brief AVRCP passthrough command received

    An AVRCP_PASSTHROUGH command has been received, normally from the other headset.
    If we're in the connected state check the command ID and handle the special cases, otherwise
    forward the command to the AV source.
    
    The special cases are:
    
        opid_volume_up & opid_volume_down: Attempt to make local volume change, if the volume
        change fails reject the request.
        
        opid_play & opid_pause: If we're not connected to AV source, attempt to reconnect, otherwise pass 
        command through to AV source.        
*/
static void appAvrcpHandleAvrcpPassthroughIndication(avInstanceTaskData *theInst, const AVRCP_PASSTHROUGH_IND_T *ind)
{
    DEBUG_LOGF("appAvrcpHandleAvrcpPassthroughIndication(%p) %d", (void *)theInst, ind->state);
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {        
            /* Handle some operations locally, forward the rest */
            switch (ind->opid)
            {
                case opid_power:
                {
                    AvrcpPassthroughResponse(ind->avrcp, avctp_response_not_implemented);
                }
                break;
                    
                case opid_volume_up:
                    if (ind->state == 0)
                        AvrcpPassthroughResponse(ind->avrcp, appAvVolumeChange(8) ? avctp_response_accepted : avctp_response_rejected);
                    else
                        AvrcpPassthroughResponse(ind->avrcp, avctp_response_accepted);                    
                    break;
                
                case opid_volume_down:
                    if (ind->state == 0)
                        AvrcpPassthroughResponse(ind->avrcp, appAvVolumeChange(-8) ? avctp_response_accepted : avctp_response_rejected);
                    else
                        AvrcpPassthroughResponse(ind->avrcp, avctp_response_accepted);                    
                    break;
                
                /* Translate play and pause from slave into appropriate AVRCP command, also attempt to
                   connect to AV source if not already connected */
                case opid_play:
                case opid_pause:
                {
                    /* Check we are not streaming before attempting to connect to AV source */
                    if (!appAvIsStreaming())
                    {
                        /* Only attempt to connect on button press */
                        if (ind->state == 0)
                        {
                            /* Make sure we are connected to default AV source */                   
                            if (appAvConnectPeer())
                            {
                                /* Connection in process so just accept command and return */
                                AvrcpPassthroughResponse(ind->avrcp, avctp_response_accepted);                    
                                break;
                            }
                        }
                    }

                    /* TODO: Modify command according to AVRCP play state */
                    
                    /* Forward command to AV source */                                        
                    appAvrcpForwardAvrcpPassthroughIndication(theInst, ind);
                }
                break;

                case opid_vendor_unique:
                {
                    if (theInst->avrcp_vendor_task)
                    {
                        /* Check header is valid and contains QTIL company ID */
                        if (ind->size_op_data >= 5)
                        {
                            const uint32 company_id = ind->op_data[0] << 16 | ind->op_data[1] << 8 | ind->op_data[2];
                            if (company_id == appConfigIeeeCompanyId())
                            {
                                const uint16 opid = ind->op_data[3] << 8 | ind->op_data[4];

                                /* Wrap command into message and send to handler task, it will call
                                 * appAvrcpVendorPassthroughResponse once it has handled the command */
                                MAKE_AV_MESSAGE_WITH_LEN(AV_AVRCP_VENDOR_PASSTHROUGH_IND, ind->size_op_data - 5);
                                message->av_instance = theInst;
                                message->opid = opid;
                                message->size_payload = ind->size_op_data - 5;
                                memcpy(message->payload, &ind->op_data[5], message->size_payload);
                                MessageSend(theInst->avrcp_vendor_task, AV_AVRCP_VENDOR_PASSTHROUGH_IND, message);

                                /* Set lock, so we don't destroy instance until we get response */
                                appAvrcpSetLock(theInst, APP_AVRCP_LOCK_PASSTHROUGH_IND);
                            }
                            else
                            {
                                /* Not for us */
                                AvrcpPassthroughResponse(ind->avrcp, avctp_response_not_implemented);
                            }
                        }
                        else
                        {
                            /* Payload is too small to contain company ID and command */
                            AvrcpPassthroughResponse(ind->avrcp, avctp_response_rejected);
                        }
                    }
                    else
                    {
                        /* No task register to handle vendor unique commands, so reject it */
                        AvrcpPassthroughResponse(ind->avrcp, avctp_response_rejected);
                    }
                }
                break;

                    
                default:
                    appAvrcpForwardAvrcpPassthroughIndication(theInst, ind);
                    break;
            }
        }
        return;
       
        default:
            AvrcpPassthroughResponse(ind->avrcp, avctp_response_rejected);
            return;
    }
}

/*! \brief AVRCP passthrough command confirmation
*/
static void appAvrcpHandleAvrcpPassthroughConfirm(avInstanceTaskData *theInst, const AVRCP_PASSTHROUGH_CFM_T *cfm)
{
    DEBUG_LOGF("appAvrcpHandleAvrcpPassthroughConfirm(%p), status %d", (void *)theInst, cfm->status);
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
            /* Clear operation lock */
            appAvrcpClearLock(theInst, APP_AVRCP_LOCK_PASSTHROUGH_REQ);

            /* Clear any pending requests if this one failed, so the rest will probably as well */
            if (cfm->status != avrcp_success)
            {
                MessageCancelAll(&theInst->av_task, AV_INTERNAL_AVRCP_REMOTE_REPEAT_REQ);
                MessageCancelAll(&theInst->av_task, AV_INTERNAL_AVRCP_REMOTE_REQ);
            }

            /* Play specific tone if required */
            switch (theInst->avrcp_op_id)
            {
                case opid_volume_up:
                {
                    if (cfm->status != avrcp_success)
                        appUiAvVolumeLimit();
                    else if (theInst->avrcp_op_state || !theInst->avrcp_op_repeat)
                        appUiAvVolumeUp();
                }
                break;
                
                case opid_volume_down:
                {
                    if (cfm->status != avrcp_success)
                        appUiAvVolumeLimit();
                    else if (theInst->avrcp_op_state || !theInst->avrcp_op_repeat)
                        appUiAvVolumeDown();
                }
                break;

                case opid_vendor_unique:
                {
                    appAvrcpFinishAvrcpPassthroughRequest(theInst, cfm->status);
                }
                break;

                default:
                {
                    /* Play standard AV error tone (only for button press, not for button release) */
                    if ((theInst->avrcp_op_state == 0) && (cfm->status != avrcp_success))
                        appUiAvError(FALSE);
                }
                break;
            }               
        }
        return;
        
            
        case AVRCP_STATE_DISCONNECTED:
        {
            /* Received in invalid state, due to some race condition, we can
               just ignore */
        }
        return;

        default:
            appAvrcpError(theInst, AVRCP_PASSTHROUGH_CFM, cfm);
            return;
    }
}

/*! \brief AVRCP unit info command received
*/
static void appAvrcpHandleAvrcpUnitInfoIndication(avInstanceTaskData *theInst, const AVRCP_UNITINFO_IND_T *ind)
{
    DEBUG_LOGF("appAvrcpHandleAvrcpUnitinfoIndication(%p)", (void *)theInst);
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
            /* Device is a CT and TG, so send the correct response to UnitInfo requests. */
            uint32 company_id = appConfigIeeeCompanyId();
            AvrcpUnitInfoResponse(ind->avrcp, TRUE, subunit_panel, 0, company_id);              
        }
        return;
        
        default:
            appAvrcpError(theInst, AVRCP_UNITINFO_IND, ind);
            return;
    }
}

/*! \brief AVRCP unit info command confirmation
*/
static void appAvrcpHandleAvrcpUnitInfoConfirm(avInstanceTaskData *theInst, const AVRCP_UNITINFO_CFM_T *cfm)
{
    DEBUG_LOGF("appAvrcpHandleAvrcpUnitInfoConfirm(%p)", (void *)theInst);
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
        }
        return;
        
        default:
            appAvrcpError(theInst, AVRCP_UNITINFO_CFM, cfm);
            return;
    }
}

/*! \brief AVRCP sub unit info command received
*/
static void appAvrcpHandleAvrcpSubUnitInfoIndication(avInstanceTaskData *theInst, const AVRCP_SUBUNITINFO_IND_T *ind)
{
    DEBUG_LOGF("appAvrcpHandleAvrcpSubUnitInfoIndication(%p)", (void *)theInst);
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
            /* Device is a CT and TG, so send the correct response to SubUnitInfo requests. */
            uint8 page_data[4];
            page_data[0] = 0x48; /* subunit_type: panel; max_subunit_ID: 0 */
            page_data[1] = 0xff;
            page_data[2] = 0xff;
            page_data[3] = 0xff;
            AvrcpSubUnitInfoResponse(ind->avrcp, TRUE, page_data);    
        }
        return;
        
        default:
            appAvrcpError(theInst, AVRCP_SUBUNITINFO_IND, ind);
            return;
    }
}

/*! \brief AVRCP sub unit info command confirmation
*/
static void appAvrcpHandleAvrcpSubUnitInfoConfirm(avInstanceTaskData *theInst, const AVRCP_SUBUNITINFO_CFM_T *cfm)
{
    DEBUG_LOGF("appAvrcpHandleAvrcpSubUnitInfoConfirm(%p)", (void *)theInst);
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
        }
        return;
        
        default:
            appAvrcpError(theInst, AVRCP_SUBUNITINFO_CFM, cfm);
            return;
    }
}

/*! \brief AVRCP vendor dependent command received
*/
static void appAvrcpHandleAvrcpVendorDependentIndication(avInstanceTaskData *theInst, const AVRCP_VENDORDEPENDENT_IND_T *ind)
{
    DEBUG_LOGF("appAvrcpHandleAvrcpVendordependentIndication(%p)", (void *)theInst);
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
            AvrcpVendorDependentResponse(ind->avrcp, avctp_response_not_implemented);
        }
        return;
        
        default:
            appAvrcpError(theInst, AVRCP_VENDORDEPENDENT_IND, ind);
            return;
    }
}

/*! \brief AVRCP vendor dependent command confirmation
*/
static void appAvrcpHandleVendorDependentConfirm(avInstanceTaskData *theInst, const AVRCP_VENDORDEPENDENT_CFM_T *cfm)
{
    DEBUG_LOGF("appAvrcpHandleVendordependentConfirm(%p)", (void *)theInst);
    
    switch (appAvrcpGetState(theInst))
    {
        case AVRCP_STATE_CONNECTED:
        {
        }
        return;
        
        default:
            appAvrcpError(theInst, AVRCP_VENDORDEPENDENT_CFM, cfm);
            return;
    }
}

/*!
*/
static void appHandleAvrcpGetCapsInd(avInstanceTaskData *theInst, AVRCP_GET_CAPS_IND_T *ind)
{
    UNUSED(theInst);
    DEBUG_LOGF("appHandleAvrcpGetCapsInd(%p), id %d", (void *)theInst, ind->caps);

    switch (ind->caps)
    {
        case avrcp_capability_company_id:
            /* FALLTHROUGH */

        case avrcp_capability_event_supported:
            /* In all cases we just want to respond with only the mandatory
               capabilities, which will be inserted by the AVRCP library. */
            AvrcpGetCapsResponse(ind->avrcp, avctp_response_stable, ind->caps, 0, 0);
            break;

        default:
            /* This should never happen, reject */
            DEBUG_LOGF("appHandleAvrcpGetCapsInd, id %d not handled", ind->caps);
            AvrcpGetCapsResponse(ind->avrcp, avrcp_response_rejected_invalid_param, ind->caps, 0, 0);
            break;
    }
}

/*!
*/
static void appHandleAvrcpRegisterNotificationInd(avInstanceTaskData *theInst, AVRCP_REGISTER_NOTIFICATION_IND_T *ind)
{
    UNUSED(theInst);
    DEBUG_LOGF("appHandleAvrcpRegisterNotificationInd(%p), event_id=%u", (void *)theInst, ind->event_id);

    /* Set bit in supported_events fields */
    theInst->avcrp_supported_events |= 1 << (ind->event_id - 1);

    switch (ind->event_id)
    {
        case avrcp_event_volume_changed:
            AvrcpEventVolumeChangedResponse(ind->avrcp, avctp_response_interim, appAvVolumeGet(theInst));
            break;

        case avrcp_event_playback_status_changed:
            AvrcpEventPlaybackStatusChangedResponse(ind->avrcp, avctp_response_interim, appAvPlayStatus());
            break;

        default:
            DEBUG_LOG("Unhandled avrcp register notification ind");
            break;
    }
}

static void appHandleAvrcpSetAbsoluteVolumeInd(avInstanceTaskData *theInst, AVRCP_SET_ABSOLUTE_VOLUME_IND_T *ind)
{
    DEBUG_LOGF("appHandleAvrcpSetAbsoluteVolumeInd(%p), volume %u", (void *)theInst, ind->volume);

    /* Set the application volume */
    appAvVolumeSet(ind->volume, theInst);

    /* Accept the volume change */
    AvrcpSetAbsoluteVolumeResponse(ind->avrcp, avctp_response_accepted, ind->volume);
}

/*! \brief Confirmation of SetAbsoluteVolume Command (Controller->Target)
*/
static void appHandleAvrcpSetAbsoluteVolumeConfirm(avInstanceTaskData *theInst, AVRCP_SET_ABSOLUTE_VOLUME_CFM_T *cfm)
{
    UNUSED(theInst); UNUSED(cfm);
    DEBUG_LOGF("appHandleAvrcpSetAbsoluteVolumeConfirm(%p), status %u", (void *)theInst, cfm->status);
}


static void appHandleAvrcpEventVolumeChangedInd(avInstanceTaskData *theInst, AVRCP_EVENT_VOLUME_CHANGED_IND_T *ind)
{
    DEBUG_LOGF("appHandleAvrcpEventVolumeChangedInd(%p), volume %u", (void *)theInst, ind->volume);

    switch (ind->response)
    {
        case avctp_response_changed:
        {
            /* re-register to receive notifications */
            AvrcpRegisterNotificationRequest(theInst->avrcp, avrcp_event_volume_changed, 0);

            /* Fall-through to handle status */
        }

        case avctp_response_interim:
        {
            /* Set the application volume */
            appAvVolumeSet(ind->volume, theInst);
        }
        break;

        default:
            break;
    }
}



static void appHandleAvrcpEventPlaybackStatusChangedInd(avInstanceTaskData *theInst, AVRCP_EVENT_PLAYBACK_STATUS_CHANGED_IND_T *ind)
{
    /* Find the other AVRCP instance */
    avInstanceTaskData *theOtherInst = appAvInstanceFindAvrcpOther(theInst);

    DEBUG_LOGF("appHandleAvrcpEventPlaybackStatusChangedInd(%p), response %u, play status %u",
               (void *)theInst, ind->response, ind->play_status);

    switch (ind->response)
    {
        case avctp_response_changed:
        {
            /* Forward to peer */
            if (theOtherInst)
            {
                MAKE_AV_MESSAGE(AV_INTERNAL_PLAYBACK_STATUS_UPDATE_IND);
                message->play_status = ind->play_status;
                MessageSend(&theOtherInst->av_task, AV_INTERNAL_PLAYBACK_STATUS_UPDATE_IND, message);
            }

            /* re-register to receive notifications */
            AvrcpRegisterNotificationRequest(theInst->avrcp, avrcp_event_playback_status_changed, 0);

            /* Fall-through to handle status */
        }

        case avctp_response_interim:
        {
            switch (ind->play_status)
            {
                case avrcp_play_status_stopped:
                case avrcp_play_status_paused:
                case avrcp_play_status_playing:
                    theInst->avrcp_play_status = ind->play_status;
                    break;

                default:
                    theInst->avrcp_play_status = avrcp_play_status_error;
                    break;
            }
        }
        break;

        default:
            break;
    }
}

/*! \brief  Send a remote control request, with optional repetition

    \param  theInst     Instance to send message on
    \param  op_id       The AVRCP operation to send
    \param  rstate      Repeat state. Non-zero indicates cancel.
    \param  ui          Whether to issue a UI indication for this event
    \param  repeat_ms   Delay between repetitions, 0 means no repeat
*/
void appAvrcpRemoteControl(avInstanceTaskData *theInst, avc_operation_id op_id, uint8 rstate, bool ui, uint16 repeat_ms)
{
    PanicFalse(appAvIsValidInst(theInst));
    DEBUG_LOG("appAvrcpRemoteControl");

    /* Cancel repeated operation, exit if it doesn't exist */
    if (rstate && repeat_ms && !MessageCancelFirst(&theInst->av_task, AV_INTERNAL_AVRCP_REMOTE_REPEAT_REQ))
    {
        return;
    }
    else
    {
        MAKE_AV_MESSAGE(AV_INTERNAL_AVRCP_REMOTE_REQ);

        /* Send internal message */
        message->op_id = op_id;
        message->state = rstate;
        message->ui = ui;
        message->repeat_ms = repeat_ms;
        MessageSendConditionally(&theInst->av_task, AV_INTERNAL_AVRCP_REMOTE_REQ,
                                 message, &appAvrcpGetLock(theInst));
    }
}

/*! \brief Check if a task is an AVRCP client for this AV

    \param theInst      The instance for this AV link
    \param client_task  The task to check

    \return TRUE if the client_task has been registered with appAvAvrcpClientRegister(), 
            FALSE otherwise
 */
bool appAvrcpIsValidClient(avInstanceTaskData *theInst, Task client_task)
{
    return appTaskListIsTaskOnList(theInst->avrcp_client_list, client_task);
}

/*! \brief Handle AVRCP messages from the AV module, and also from the AVRCP library

    \param  theInst The AV Instance that this message is for 
    \param  id      ID of message
    \param[in]  message Pointer to received message content, can be NULL.

*/
void appAvrcpInstanceHandleMessage(avInstanceTaskData *theInst, MessageId id, Message message)
{
    /* Handle internal messages */
    switch (id)
    {
        case AV_INTERNAL_AVRCP_CONNECT_IND:
            appAvrcpHandleInternalAvrcpConnectIndication(theInst, (AV_INTERNAL_AVRCP_CONNECT_IND_T *)message);
            return;
        
        case AV_INTERNAL_AVRCP_CONNECT_RES:
            appAvrcpHandleInternalAvrcpConnectResponse(theInst, (AV_INTERNAL_AVRCP_CONNECT_RES_T *)message);
            return;

        case AV_INTERNAL_AVRCP_CONNECT_REQ:
            appAvrcpHandleInternalAvrcpConnectRequest(theInst, (AV_INTERNAL_AVRCP_CONNECT_REQ_T *)message);
            return;
        
        case AV_INTERNAL_AVRCP_DISCONNECT_REQ:
            appAvrcpHandleInternalAvrcpDisconnectRequest(theInst, (AV_INTERNAL_AVRCP_DISCONNECT_REQ_T *)message);
            return;
        
        case AV_INTERNAL_AVRCP_REMOTE_REQ:
            appAvrcpHandleInternalAvrcpRemoteRequest(theInst, (AV_INTERNAL_AVRCP_REMOTE_REQ_T *)message, FALSE);
            return;

        case AV_INTERNAL_AVRCP_REMOTE_REPEAT_REQ:
            appAvrcpHandleInternalAvrcpRemoteRequest(theInst, (AV_INTERNAL_AVRCP_REMOTE_REQ_T *)message, TRUE);
            return;

        case AV_INTERNAL_AVRCP_VOLUME_NOTIFY_REQ:
            appAvrcpHandleInternalAvrcpVolumeNotifyRequest(theInst, (AV_INTERNAL_AVRCP_VOLUME_NOTIFY_REQ_T *)message);
            return;

        case AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_RES:
            appAvrcpHandleInternalAvrcpVendorPassthroughResponse(theInst, (AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_RES_T *)message);
            return;

        case AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_REQ:
            appAvrcpHandleInternalAvrcpVendorPassthroughRequest(theInst, (AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_REQ_T *)message);
            return;

        case AV_INTERNAL_PLAYBACK_STATUS_UPDATE_IND:
            appAvrcpHandleInternalPlaybackStatusUpdateIndication(theInst, (AV_INTERNAL_PLAYBACK_STATUS_UPDATE_IND_T *)message);
            return;

        case AV_INTERNAL_AVRCP_DESTROY_REQ:
            appAvInstanceDestroy(theInst);
            return;
    }

    /* Handle AVRCP library messages */
    switch (id)
    {
        case AVRCP_CONNECT_CFM:
            appAvrcpHandleAvrcpConnectConfirm(theInst, (AVRCP_CONNECT_CFM_T *)message);
            return;
        
        case AVRCP_CONNECT_IND:
            /* Handled by av_headset_av.c so should not receive it here */
            appAvrcpError(theInst, id, message);
            return;
        
        case AVRCP_DISCONNECT_IND:
            appAvrcpHandleAvrcpDisconnectIndication(theInst, (AVRCP_DISCONNECT_IND_T *)message);
            return;
        
        case AVRCP_PASSTHROUGH_CFM:
            appAvrcpHandleAvrcpPassthroughConfirm(theInst, (AVRCP_PASSTHROUGH_CFM_T *)message);
            return;
        
        case AVRCP_PASSTHROUGH_IND:
            appAvrcpHandleAvrcpPassthroughIndication(theInst, (AVRCP_PASSTHROUGH_IND_T *)message);
            return;

        case AVRCP_UNITINFO_CFM:
            appAvrcpHandleAvrcpUnitInfoConfirm(theInst, (AVRCP_UNITINFO_CFM_T *)message);
            return;
        
        case AVRCP_UNITINFO_IND:
            appAvrcpHandleAvrcpUnitInfoIndication(theInst, (AVRCP_UNITINFO_IND_T *)message);
            return;
        
        case AVRCP_SUBUNITINFO_IND:
            appAvrcpHandleAvrcpSubUnitInfoIndication(theInst, (AVRCP_SUBUNITINFO_IND_T *)message);
            return;
        
        case AVRCP_SUBUNITINFO_CFM:
            appAvrcpHandleAvrcpSubUnitInfoConfirm(theInst, (AVRCP_SUBUNITINFO_CFM_T *)message);
            return;
        
        case AVRCP_VENDORDEPENDENT_CFM:
            appAvrcpHandleVendorDependentConfirm(theInst, (AVRCP_VENDORDEPENDENT_CFM_T *)message);
            return;
        
        case AVRCP_VENDORDEPENDENT_IND:
            appAvrcpHandleAvrcpVendorDependentIndication(theInst, (AVRCP_VENDORDEPENDENT_IND_T *)message);
            return;

        case AVRCP_SET_ABSOLUTE_VOLUME_CFM:
            appHandleAvrcpSetAbsoluteVolumeConfirm(theInst, (AVRCP_SET_ABSOLUTE_VOLUME_CFM_T *)message);
            return;

        case AVRCP_SET_ABSOLUTE_VOLUME_IND:
            appHandleAvrcpSetAbsoluteVolumeInd(theInst, (AVRCP_SET_ABSOLUTE_VOLUME_IND_T *) message);
            return;

        case AVRCP_GET_CAPS_IND:
            appHandleAvrcpGetCapsInd(theInst, (AVRCP_GET_CAPS_IND_T *)message);
            return;

        case AVRCP_REGISTER_NOTIFICATION_IND:
            appHandleAvrcpRegisterNotificationInd(theInst, (AVRCP_REGISTER_NOTIFICATION_IND_T *)message);
            return;

        case AVRCP_EVENT_PLAYBACK_STATUS_CHANGED_IND:
            appHandleAvrcpEventPlaybackStatusChangedInd(theInst, (AVRCP_EVENT_PLAYBACK_STATUS_CHANGED_IND_T *)message);
            return;

        case AVRCP_EVENT_VOLUME_CHANGED_IND:
            appHandleAvrcpEventVolumeChangedInd(theInst, (AVRCP_EVENT_VOLUME_CHANGED_IND_T *)message);
            return;

        case AVRCP_REGISTER_NOTIFICATION_CFM:
            return;
    }
    
    /* Unhandled message */
    appAvrcpError(theInst, id, message);
}


/*! \brief Register a task to handle vendor passthrough messages on this link

    \param  theInst     The AV instance to register for
    \param  client_task Task to register

    \returns The previous handling task (if any)
 */
Task appAvrcpVendorPassthroughRegister(avInstanceTaskData *theInst, Task client_task)
{
    Task task = theInst->avrcp_vendor_task;
    PanicFalse(appAvIsValidInst(theInst));
    theInst->avrcp_vendor_task = client_task;
    return task;
}

/*! \brief Send a response status to a received request 

    \param  theInst     The AV instance this applies to
    \param  response    The response status
*/
void appAvrcpVendorPassthroughResponse(avInstanceTaskData *theInst, avrcp_response_type response)
{
    MAKE_AV_MESSAGE(AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_RES);
    PanicFalse(appAvIsValidInst(theInst));
    message->response = response;
    MessageSend(&theInst->av_task, AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_RES, message);
}

/*! \brief Send a passthrough request on the AVRCP link

    \param  theInst       The AV instance this applies to
    \param  op_id         The request code to send
    \param  size_payload  The size of the variable length payload (in octets)
    \param  payload       Pointer to the variable length payload
 */
void appAvrcpVendorPassthroughRequest(avInstanceTaskData *theInst, avc_operation_id op_id, uint16 size_payload, const uint8 *payload)
{
    MAKE_AV_MESSAGE_WITH_LEN(AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_REQ, size_payload);
    PanicFalse(appAvIsValidInst(theInst));
    message->client_task = theInst->avrcp_vendor_task;
    message->op_id = op_id;
    message->size_payload = size_payload;
    memcpy(message->payload, payload, size_payload);
    MessageSendConditionally(&theInst->av_task, AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_REQ, message, &appAvrcpGetLock(theInst));
}

/*! \brief Initialise AV instance

    This function initialises the specified AV instance, all state variables are
    set to defaults.  NOTE: This function should only be called on a newly created
    instance.

    \param theInst  Pointer to instance to initialise
*/
void appAvrcpInstanceInit(avInstanceTaskData *theInst)
{
    /* No profile instance yet */
    theInst->avrcp = NULL;
    
    /* Initialise state */
    theInst->avrcp_state = AVRCP_STATE_DISCONNECTED;
    theInst->avrcp_lock = 0;
    theInst->avcrp_supported_events = 0;
    theInst->avrcp_vendor_task = NULL;
    theInst->avrcp_vendor_data = NULL;
    theInst->avrcp_vendor_opid = 0;
    theInst->avrcp_remotely_initiated = FALSE;
    theInst->avrcp_client_list = appTaskListInit();
    theInst->avrcp_client_lock = 0;
    theInst->avrcp_play_status = avrcp_play_status_error;
    theInst->avrcp_play_hint = avrcp_play_status_error;
}

#else
static const int compiler_happy;
#endif
