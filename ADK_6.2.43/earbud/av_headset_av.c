/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_av.c
\brief      AV State Machines (A2DP & AVRCP)

Main AV task.
*/

/* Only compile if AV defined */
#ifdef INCLUDE_AV

#include <a2dp.h>
#include <avrcp.h>
#include <panic.h>
#include <connection.h>
#include <ps.h>
#include <file.h>
#include <transform.h>
#include <string.h>

#include "av_headset.h"
#include "av_headset_av.h"
#include "av_headset_a2dp.h"
#include "av_headset_avrcp.h"
#include "av_headset_ui.h"
#include "av_headset_link_policy.h"
#include "av_headset_power.h"

#include "av_headset_log.h"

/*! Macro for creating messages */
#define MAKE_AV_MESSAGE(TYPE) \
    TYPE##_T *message = PanicUnlessNew(TYPE##_T);

static void appAvHandleMessage(Task task, MessageId id, Message message);

/*! \brief Handle AV error

    Some error occurred in the Av state machine, to avoid the state machine
    getting stuck, drop connection and move to 'disconnected' state.
*/
static void appAvError(avTaskData *theAv, MessageId id, Message message)
{
    UNUSED(message); UNUSED(theAv);UNUSED(id);

#ifdef AV_DEBUG
    DEBUG_LOGF("appAvError %p, state=%u, id=%x", (void *)theAv, theAv->state, id);
#else
    Panic();
#endif
}

/*! \brief Check if at least one A2DP or AVRCP link is connected

    \return TRUE if either an A2DP or an AVRCP link is connected. FALSE otherwise.
*/
bool appAvHasAConnection(void)
{
    avTaskData *theAv = appGetAv();
    int instance;

    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst)
            if (appA2dpIsConnected(theInst) || appAvrcpIsConnected(theInst))
                return TRUE;
    }

    /* No AV connections */
    return FALSE;
}

/*! \brief Check all A2DP links are disconnected

    \return TRUE if there are no connected links. FALSE if any AV link 
            has either an A2DP or an AVRCP connection.
*/
bool appAvIsDisconnected(void)
{
    avTaskData *theAv = appGetAv();
    int instance;

    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst)
        {
            if (!appA2dpIsDisconnected(theInst) || !appAvrcpIsDisconnected(theInst))
                return FALSE;
        }
    }

    /* No AV connections */
    return TRUE;
}

/*! \brief Check if A2DP is streaming

    \return TRUE if there is an AV that is streaming
*/
bool appAvIsStreaming(void)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* Look in table to find entry that is streaming as sink */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && appA2dpIsSinkCodec(theInst) && appA2dpIsStreaming(theInst))
            return TRUE;
    }

    /* No AV connections */
    return FALSE;
}

/*! \brief Check if AV is connected as an A2DP slave

    \return TRUE If any connected AV has A2DP signalling and is
        a TWS Sink.
*/
bool appAvIsConnectedSlave(void)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* Look in table to find entry with matchin Bluetooth address */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && appA2dpIsStateConnectedSignalling(theInst->a2dp_state))
        {
            if (appA2dpIsSinkTwsCodec(theInst))
                return TRUE;
        }
    }

    /* No AV connections */
    return FALSE;
}

/*! \brief Check if A2DP is streaming as a master

    \return TRUE If any connected AV has A2DP signalling and is
                 a TWS Source.
*/
bool appAvIsConnectedMaster(void)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* Look in table to find entry with matching Bluetooth address */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && appA2dpIsStateConnectedSignalling(theInst->a2dp_state))
        {
            if (appA2dpIsSourceCodec(theInst))
                return TRUE;
        }
    }

    /* No AV connections */
    return FALSE;
}


/*! \brief Get the current play status of the AV

    \return The play status, the status of the stream from a headset, 
            which can include avrcp_play_status_error. 
            avrcp_play_status_error is also returned if there is no
            current link.
*/
avrcp_play_status appAvPlayStatus(void)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* First of all in table to find entry that is streaming as sink */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && appA2dpIsSinkCodec(theInst) && appA2dpIsConnectedMedia(theInst))
        {
            if (theInst->avrcp_play_status != avrcp_play_status_error)
                return theInst->avrcp_play_status;
            else
                return theInst->avrcp_play_hint;
        }
    }

    return avrcp_play_status_error;
}

/*! \brief Set the play status if the real status is not known

    The AV should know whether we are playing audio, based on AVRCP
    status messages. This information can be missing, in which case
    this function allows you to set a status. It won't override
    a known status.
    
    \param status   The status hint to be used
 */
void appAvHintPlayStatus(avrcp_play_status status)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* First of all in table to find entry that is streaming as sink */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && appA2dpIsSinkCodec(theInst) && appA2dpIsConnectedMedia(theInst))
        {
            if (theInst->avrcp_play_status == avrcp_play_status_error)
                theInst->avrcp_play_hint = status;

            return;
        }
    }
}

static void appAvInstanceHandleMessage(Task task, MessageId id, Message message)
{
    avInstanceTaskData *theInst = (avInstanceTaskData *)task;

    if (id >= AV_INTERNAL_AVRCP_BASE && id < AV_INTERNAL_AVRCP_TOP)
        appAvrcpInstanceHandleMessage(theInst, id, message);
    else if (id >= AV_INTERNAL_A2DP_BASE && id < AV_INTERNAL_A2DP_TOP)
        appA2dpInstanceHandleMessage(theInst, id, message);
    else if (id >= AVRCP_MESSAGE_BASE && id < AVRCP_MESSAGE_TOP)
        appAvrcpInstanceHandleMessage(theInst, id, message);
    else if (id >= A2DP_MESSAGE_BASE && id < A2DP_MESSAGE_TOP)
        appA2dpInstanceHandleMessage(theInst, id, message);
    else
    {
        switch (id)
        {
            case AV_AVRCP_CONNECT_CFM:
            case AV_AVRCP_DISCONNECT_IND:
                break;

            default:
                appAvError(appGetAv(), id, message);
                break;
        }
    }
}

/*! \brief Find AV instance with A2DP state

    This function attempts to find the other AV instance with a matching A2DP state.
    The state is selected using a mask and matching state.

    \param  theInst     AV instance that we want to find a match to
    \param  mask        Mask value to be applied to the a2dp state of a connection
    \param  expected    State expected after applying the mask

    \return Pointer to the AV that matches, NULL if no matching AV was found
*/
avInstanceTaskData *appAvInstanceFindA2dpState(const avInstanceTaskData *theInst,
                                               uint8 mask, uint8 expected)
{
    avTaskData *theAv = appGetAv();
    int instance;

    PanicFalse(appAvIsValidInst((avInstanceTaskData*)theInst));

    /* Look in table to find entry with matching A2DP state */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theOtherInst = theAv->av_inst[instance];
        if (theOtherInst != NULL && theInst != theOtherInst)
        {
            if ((theOtherInst->a2dp_state & mask) == expected)
            {
                return theOtherInst;
            }
        }
    }

    /* No match found */
    return NULL;
}

/*! \brief Find AV instance for AVRCP passthrough

    This function finds the AV instance to send a AVRCP passthrough command.
    If an AV instance has a Sink SEID as it's last used SEID, then the
    passthrough command should be sent using that instance, otherwise use
    an AV instance with no last used SEID as this will be for an AV source that
    has just paired but hasn't yet connected the A2DP media channel.

    \return Pointer to the AV to use, NULL if no appropriate AV found
*/
avInstanceTaskData *appAvInstanceFindAvrcpForPassthrough(void)
{
    avTaskData *theAv = appGetAv();
    int instance;

    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && appAvrcpIsConnected(theInst))
        {
            if (appA2dpIsSinkCodec(theInst))
                return theInst;
            else if (appDeviceIsHandset(&theInst->bd_addr))
                return theInst;
        }
    }

    /* No sink A2DP instance, if there's just one AVRCP, send passthrough on that instance */
    avInstanceTaskData *thePassthoughInstance = NULL;
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && appAvrcpIsConnected(theInst))
        {
            /* If no previous instance, remember this one for passthrough */
            if (!thePassthoughInstance)
                thePassthoughInstance = theInst;
            else
            {
                /* Second instance found, can't do passthrough, so exit loop */
                thePassthoughInstance = NULL;
                break;
            }
        }
    }

    return thePassthoughInstance;
}


/*! \brief given a BDADDR, find an AV with a connected AVRCP to that device

    \param bd_addr  Address of bluetooth device to find a connection to

    \return    Pointer to AV data for the AV that is to the requested device
               and has an active avrcp connection.
*/
avInstanceTaskData *appAvInstanceFindAvrcpFromBdAddr(const bdaddr *bd_addr)
{
    avTaskData *theAv = appGetAv();
    int instance;

    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && BdaddrIsSame(&theInst->bd_addr, bd_addr))
            return appAvrcpIsConnected(theInst) ? theInst : NULL;
    }

    return NULL;
}

/*! \brief Find a different connected AV instance

    In TWS cases there may be an AV link to a headset as well as the AV to
    the peer device. Finding the other one, given details of 1 link, can be
    useful for forwarding information.

    \param theInst Instance used in check

    \returns NULL if there is no other instance that is connected, otherwise
             returns an AV instance that is connected and not the same as that 
             supplied.
 */
avInstanceTaskData *appAvInstanceFindAvrcpOther(avInstanceTaskData *theInst)
{
    avTaskData *theAv = appGetAv();
    int instance;

    PanicFalse(appAvIsValidInst(theInst));

    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theOtherInst = theAv->av_inst[instance];
        if (theOtherInst && theInst != theOtherInst)
            if (appAvrcpIsConnected(theOtherInst))
                return theOtherInst;
    }

    return NULL;

}

/*! \brief Find AV instance with Bluetooth Address

    \note This function returns the AV. It does not check for an
            active connection, or whether A2DP/AVRCP exists.

    \param  bd_addr Bluetooth address to search our AV connections for

    \return Pointer to AV data that matches the bd_addr requested. NULL if
            none was found.
*/
avInstanceTaskData *appAvInstanceFindFromBdAddr(const bdaddr *bd_addr)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* Look in table to find entry with matching Bluetooth address */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && BdaddrIsSame(bd_addr, &theInst->bd_addr))
            return theInst;
    }

    /* No match found */
    return NULL;
}


/*! \brief Check if AVRCP is connected for AV usage

    \param[in]  theInst The AV Instance to be checked for an AVRCP connection.

    \return     TRUE if the AV task of this instance is registered for AVRCP messages
*/
bool appAvIsAvrcpConnected(avInstanceTaskData *theInst)
{
    return appAvrcpIsValidClient(theInst, &theInst->av_task);
}

/*! \brief Create AV instance for A2DP sink or source

    Creates an AV instance entry for the bluetooth address supplied (bd_addr).

    \note No check is made for there already being an instance 
    for this address.

    \param bd_addr Address the created instance will represent

    \return Pointer to the created instance, or NULL if it was not
        possible to create

*/
avInstanceTaskData *appAvInstanceCreate(const bdaddr *bd_addr)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* Look in table to find empty entry */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        if (theAv->av_inst[instance] == NULL)
        {
            avInstanceTaskData *theInst;

            /* Allocate new instance */
            theInst = PanicUnlessNew(avInstanceTaskData);
            theAv->av_inst[instance] = theInst;

            DEBUG_LOGF("appAvInstanceCreate %p", theInst);

            /* Initialise instance */
            appA2dpInstanceInit(theInst, theAv->suspend_state);
            appAvrcpInstanceInit(theInst);

            /* Set up task handler */
            theInst->av_task.handler = appAvInstanceHandleMessage;

            /* Set Bluetooth address of remote device */
            theInst->bd_addr = *bd_addr;

            /* Copy the AV status client list to instance */
            theInst->av_status_client_list = appTaskListDuplicate(theAv->av_status_client_list);

            /* Tell main application we have created new instance */
            MessageSend(appGetAppTask(), AV_CREATE_IND, NULL);

            /* Return pointer to new instance */
            return theInst;
        }
    }

    /* No free entry in table */
    return NULL;
}

/*! \brief Destroy AV instance for A2DP sink or source

    This function should only be called if the instance no longer has
    either a connected A2DP or a connected AVRCP.  If either is still 
    connected, the function will silently fail.

    The function will panic if theInst is not valid, for instance
    if already destroyed.

    \param  theInst The instance to destroy

*/
void appAvInstanceDestroy(avInstanceTaskData *theInst)
{
    avTaskData *theAv = appGetAv();
    int instance;

    PanicFalse(appAvIsValidInst(theInst));

    /* Destroy instance only both state machines are disconnected */
    if (appA2dpIsDisconnected(theInst) && appAvrcpIsDisconnected(theInst))
    {
        DEBUG_LOGF("appAvInstanceDestroy %p", theInst);

        /* Check there are no A2DP & AVRCP profile library instances */
        PanicFalse(theInst->a2dp_device_id == INVALID_DEVICE_ID);
        PanicNotNull(theInst->avrcp);

        /* Clear client lists */
        appTaskListDestroy(theInst->avrcp_client_list);
        appTaskListDestroy(theInst->av_status_client_list);
        theInst->avrcp_client_list = NULL;
        theInst->av_status_client_list = NULL;

        /* Flush any messages still pending delivery */
        MessageFlushTask(&theInst->av_task);

        /* Look in table to find matching entry */
        for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
        {
            if (theAv->av_inst[instance] == theInst)
            {
                /* Clear entry and free instance */
                theAv->av_inst[instance] = NULL;
                free(theInst);

                /* Tell main application we have destroyed instance */
                MessageSend(appGetAppTask(), AV_DESTROY_IND, NULL);
                return;
            }
        }

        /* No match found, fatal error! */
        Panic();
    }
    else
    {
        DEBUG_LOGF("appAvInstanceDestroy %p, A2DP (%d) or AVRCP (%d) not disconnected",
                   theInst, !appA2dpIsDisconnected(theInst), !appAvrcpIsDisconnected(theInst));
    }
}

/*! \brief Return AV instance for A2DP sink

    This function walks through the AV instance table looking for the
    first instance which is a connected sink that can use the 
    specified codec.

    \param codec_type   Codec to look for

    \return Pointer to AV information for a connected source,NULL if none
        was found
*/
avInstanceTaskData *appAvGetA2dpSink(avCodecType codec_type)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* Look in table to find connected instance with sink SEID */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && appA2dpIsStateConnectedMedia(theInst->a2dp_state))
        {
            switch (codec_type)
            {
                case AV_CODEC_ANY:
                    if (appA2dpIsSinkCodec(theInst))
                        return theInst;
                    break;

                case AV_CODEC_TWS:
                    if (appA2dpIsSinkTwsCodec(theInst))
                        return theInst;
                    break;

                case AV_CODEC_NON_TWS:
                    if (appA2dpIsSinkNonTwsCodec(theInst))
                        return theInst;
                    break;
            }
        }
    }

    /* No sink found so return NULL */
    return NULL;
}

/*! \brief Return AV instance for A2DP source

    This function walks through the AV instance table looking for the
    first instance which is a connected source.

    \return Pointer to AV information for a connected source,NULL if none
            was found
*/
avInstanceTaskData *appAvGetA2dpSource(void)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* Look in table to find connected instance with source SEID */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *theInst = theAv->av_inst[instance];
        if (theInst && appA2dpIsStateConnectedMedia(theInst->a2dp_state) && appA2dpIsSourceCodec(theInst))
            return theInst;
    }

    /* No sink found so return NULL */
    return NULL;
}

/*! \brief Entering `Initialising A2DP` state

    This function is called when the AV state machine enters
    the 'Initialising A2DP' state, it calls the A2dpInit() function
    to initialise the A2DP profile library and register the SEPs.
*/

static void appAvEnterInitialisingA2dp(avTaskData *theAv)
{
    sep_data_type seps[7];

    DEBUG_LOG("appAvEnterInitialisingA2dp");

    /* Initialise the Stream Endpoints */
    seps[6].sep_config = &av_sbc_snk_sep;
    seps[6].in_use = 0;
    seps[5].sep_config = &av_aac_snk_sep;
    seps[5].in_use = 0;
    seps[4].sep_config = &av_aptx_snk_sep;
    seps[4].in_use = 0;
    seps[3].sep_config = &av_aptx_mono_tws_snk_sep;
    seps[3].in_use = 0;
    seps[2].sep_config = &av_sbc_mono_tws_snk_sep;
    seps[2].in_use = 0;
    seps[1].sep_config = &av_aptx_mono_tws_src;
    seps[1].in_use = 0;
    seps[0].sep_config = &av_sbc_mono_tws_src;
    seps[0].in_use = 0;

    /* Initialise the A2DP Library */
    A2dpInit(&theAv->task, A2DP_INIT_ROLE_SINK | A2DP_INIT_ROLE_SOURCE, 0, 7, seps, 0);
}

/*! \brief Entering `Initialising AVRCP` state

    This function is called when the AV state machine enters
    the 'Initialising AVRCP' state, it calls the AvrcpInit() function
    to initialise the AVRCP profile library.
*/
static void appAvEnterInitialisingAvrcp(avTaskData *theAv)
{
    const avrcp_init_params avrcpConfig =
    {
        avrcp_target_and_controller,
        AVRCP_CATEGORY_1,
        AVRCP_CATEGORY_2 | AVRCP_CATEGORY_1,
        AVRCP_VERSION_1_6
    };

    DEBUG_LOG("appAvEnterInitialisingAvrcp");

    /* Go ahead and initialise the AVRCP library */
    AvrcpInit(&theAv->task, &avrcpConfig);
}

/*! \brief Set AV FSM state

    Called to change state.  Handles calling the state entry and exit
    functions for the new and old states.
*/
static void appAvSetState(avTaskData *theAv, avState state)
{
    DEBUG_LOGF("appAvSetState(%d)", state);

    /* Set new state */
    theAv->state = state;

    /* Handle state entry functions */
    switch (state)
    {
        case AV_STATE_INITIALISING_A2DP:
            appAvEnterInitialisingA2dp(theAv);
            break;

        case AV_STATE_INITIALISING_AVRCP:
            appAvEnterInitialisingAvrcp(theAv);
            break;

        default:
            break;
    }

    /* Set new state */
    theAv->state = state;
}

/*! \brief Set AV FSM state

    Returns current state of the AV FSM.
*/
static avState appAvGetState(avTaskData *theAv)
{
    return theAv->state;
}

/*! \brief Handle A2DP Library initialisation confirmation

    Check status of A2DP Library initialisation if successful store the SEP
    list for later use and move to the 'Initialising AVRCP' state to kick
    of AVRCP Library initialisation.
*/
static void appAvHandleA2dpInitConfirm(avTaskData *theAv, const A2DP_INIT_CFM_T *cfm)
{
    DEBUG_LOG("appAvHandleA2dpInitConfirm");

    /* Check if A2DP initialised successfully */
    if (cfm->status == a2dp_success)
    {
        /* Move to 'Initialising AVRCP' state */
        appAvSetState(theAv, AV_STATE_INITIALISING_AVRCP);
    }
    else
        Panic();
}

/*! \brief Handle AVRCP Library initialisation confirmation

    Check status of AVRCP Library initialisation if successful inform the main
    task that AV initialisation has completed and move into the 'Active' state.
*/
static void appAvHandleAvrcpInitConfirm(avTaskData *theAv, const AVRCP_INIT_CFM_T *cfm)
{
    DEBUG_LOG("appAvHandleAvrcpInitConfirm");

    /* Check if AVRCP successfully initialised */
    if (cfm->status == avrcp_success)
    {
        /* Tell main application task we have initialised */
        MessageSend(appGetAppTask(), AV_INIT_CFM, NULL);

        /* Change to 'idle' state */
        appAvSetState(theAv, AV_STATE_IDLE);
    }
    else
        Panic();
}

/*! \brief Handle incoming A2DP connection

    A2DP Library has indicating an incoming A2DP connection,
    Check if we can create or use an existing AV instance, if so accept the
    incoming connection otherwise reject it.
*/
static void appAvHandleA2dpSignallingConnectIndication(avTaskData *theAv, const A2DP_SIGNALLING_CONNECT_IND_T *ind)
{
    avInstanceTaskData *av_inst;

    UNUSED(theAv);
    DEBUG_LOG("appAvHandleA2dpSignallingConnectIndication");

    /* Check there isn't already an A2DP instance for this device */
    av_inst = appAvInstanceFindFromBdAddr(&ind->addr);
    if (av_inst == NULL)
        av_inst = appAvInstanceCreate(&ind->addr);
    else
    {
        /* Make sure there's no pending destroy message */
        MessageCancelAll(&av_inst->av_task, AV_INTERNAL_A2DP_DESTROY_REQ);
        MessageCancelAll(&av_inst->av_task, AV_INTERNAL_AVRCP_DESTROY_REQ);
    }

    if (av_inst != NULL)
    {
        /* Create message to send to AV instance */
        MAKE_AV_MESSAGE(AV_INTERNAL_A2DP_SIGNALLING_CONNECT_IND);

        /* Make sure instance isn't about to be destroyed */
        MessageCancelAll(&av_inst->av_task, AV_INTERNAL_A2DP_DESTROY_REQ);

        /* Send message to AV instance */
        message->a2dp_device_id = ind->device_id;
        message->flags = 0;
        MessageSend(&av_inst->av_task, AV_INTERNAL_A2DP_SIGNALLING_CONNECT_IND, message);

        /* Finished so return */
        return;
    }

    /* Reject incoming connection, either there is an existing instance, or we failed to create a new instance */
    A2dpSignallingConnectResponse(ind->device_id, FALSE);
}

/*! \brief Reject incoming A2DP connection

    A2DP Library has indicating an incoming A2DP connection, we're not in
    the 'Active' state, so just reject the connection.
*/
static void appAvRejectA2dpSignallingConnectIndication(avTaskData *theAv, const A2DP_SIGNALLING_CONNECT_IND_T *ind)
{
    UNUSED(theAv);
    DEBUG_LOG("appAvRejectA2dpSignallingConnectIndication");

    /* Reject incoming connection */
    A2dpSignallingConnectResponse(ind->device_id, FALSE);
}

/*! \brief Handle incoming AVRCP connection

    AVRCP Library has indicating an incoming AVRCP connection,
    Check if we can create or use an existing AV instance, if so accept the
    incoming connection otherwise reject it.
*/
static void appAvHandleAvrcpConnectIndication(avTaskData *theAv, const AVRCP_CONNECT_IND_T *ind)
{
    avInstanceTaskData *av_inst;
    DEBUG_LOG("appAvHandleAvrcpConnectIndication");

    /* Create task (or find exising one) for the AVRCP connection */
    av_inst = appAvInstanceFindFromBdAddr(&ind->bd_addr);
    if (av_inst == NULL)
        av_inst = appAvInstanceCreate(&ind->bd_addr);
    else
    {
        /* Make sure there's no pending destroy message */
        MessageCancelAll(&av_inst->av_task, AV_INTERNAL_A2DP_DESTROY_REQ);
        MessageCancelAll(&av_inst->av_task, AV_INTERNAL_AVRCP_DESTROY_REQ);
    }

    /* Send message to task if possible, otherwise reject connection */
    if (av_inst != NULL)
    {
        /* Create message to send to AV instance */
        MAKE_AV_MESSAGE(AV_INTERNAL_AVRCP_CONNECT_IND);

        /* Send message to AV instance */
        message->signal_id = ind->signal_id;
        message->connection_id = ind->connection_id;
        MessageSend(&av_inst->av_task, AV_INTERNAL_AVRCP_CONNECT_IND, message);
    }
    else
    {
        /* Reject incoming connection */
        AvrcpConnectResponse(&theAv->task, ind->connection_id, ind->signal_id, FALSE, &ind->bd_addr);
    }
}

/*! \brief Reject incoming AVRCP connection

    AVRCP Library has indicating an incoming AVRCP connection, we're not in
    the 'Active' state, so just reject the connection.
*/
static void appAvRejectAvrcpConnectIndication(avTaskData *theAv, const AVRCP_CONNECT_IND_T *ind)
{
    DEBUG_LOG("appAvRejectAvrcpConnectIndication");

    /* Reject incoming connection */
    AvrcpConnectResponse(&theAv->task, ind->connection_id, ind->signal_id, FALSE, &ind->bd_addr);
}


/*! \brief Indication of successful A2DP connection

    A2DP is now connected, attempt to bring up AVRCP.
*/
static void appAvHandleAvA2dpConnectedIndication(avTaskData *theAv, const AV_A2DP_CONNECTED_IND_T *ind)
{
    avInstanceTaskData *theInst;
    DEBUG_LOG("appAvHandleAvA2dpConnectedIndication");
    UNUSED(theAv);

    theInst = appAvInstanceFindFromBdAddr(&ind->bd_addr);
    if (theInst)
    {
        /* If A2DP was initiated by us, or AVRCP has already been brought up by someone else */
        if (ind->local_initiated || appAvrcpIsConnected(theInst))
        {
            DEBUG_LOG("appAvHandleAvA2dpConnectedIndication, locally initiated, connecting AVRCP");
            appAvAvrcpConnectRequest(&theInst->av_task, &ind->bd_addr);
        }
        else if (appAvrcpIsDisconnected(theInst))
        {
            DEBUG_LOG("appAvHandleAvA2dpConnectedIndication, remotely initiated");
            appAvAvrcpConnectLaterRequest(&theInst->av_task, &ind->bd_addr,
                                          appConfigAvrcpConnectDelayAfterRemoteA2dpConnectMs());
        }
    }
}

/*! \brief Indication of A2DP disconnection

    A2DP is now disconnected, attempt to disconnect AVRCP.
*/
static void appAvHandleAvA2dpDisconnectedIndication(avTaskData *theAv, const AV_A2DP_DISCONNECTED_IND_T *ind)
{
    avInstanceTaskData *theInst;
    DEBUG_LOG("appAvHandleAvA2dpDisconnectedIndication");
    UNUSED(theAv);

    theInst = appAvInstanceFindFromBdAddr(&ind->bd_addr);
    if (theInst)
    {
        DEBUG_LOG("appAvHandleAvA2dpDisconnectedIndication, disconnecting AVRCP");
        appAvAvrcpDisconnectRequest(&theInst->av_task, theInst);
    }
}

static void  appAvHandleAvAvrcpConnectIndication(avTaskData *theAv, const AV_AVRCP_CONNECT_IND_T *ind)
{
    avInstanceTaskData *theInst;
    DEBUG_LOG("appAvHandleAvAvrcpConnectIndication");
    UNUSED(theAv);

    theInst = appAvInstanceFindFromBdAddr(&ind->bd_addr);
    if (theInst)
    {
        /* Reject connection if A2DP is disconnected, accept if A2DP connected or connecting */
        if (appA2dpIsDisconnected(theInst))
        {
            DEBUG_LOG("appAvHandleAvAvrcpConnectIndication, rejecting");

            /* Reject incoming connection */
            appAvAvrcpConnectResponse(&theInst->av_task, &ind->bd_addr, ind->connection_id,
                                      ind->signal_id,  AV_AVRCP_REJECT);
        }
        else
        {
            DEBUG_LOG("appAvHandleAvAvrcpConnectIndication, accepting");

            /* Accept incoming connection */
            appAvAvrcpConnectResponse(&theInst->av_task, &ind->bd_addr, ind->connection_id,
                                      ind->signal_id,  AV_AVRCP_ACCEPT);
        }
    }
}


/*! \brief Handle indication of change in a connection status.

    Some phones will disconnect the ACL without closing any L2CAP/RFCOMM
    connections, so we check the ACL close reason code to determine if this
    has happened.

    If the close reason code was not link-loss and we have an AV profiles
    on that link, mark it as detach pending, so that we can gracefully handle
    the L2CAP or RFCOMM disconnection that will follow shortly.
 */
static void appAvHandleConManagerConnectionInd(CON_MANAGER_CONNECTION_IND_T *ind)
{
    avInstanceTaskData *theInst = appAvInstanceFindFromBdAddr(&ind->bd_addr);
    if (theInst)
    {
        if (!ind->connected)
        {
            if (ind->reason != hci_error_conn_timeout)
            {
                DEBUG_LOG("appAvHandleConManagerConnectionInd, detach pending");
                theInst->detach_pending = TRUE;
            }
        }
    }
}


/*! \brief Initialise AV task

    Called at start up to initialise the main AV task..
    Clear the AV instance table and initialises the state-machine.
*/
void appAvInit(void)
{
    avTaskData *theAv = appGetAv();
    int instance;

    /* Set up task handler */
    theAv->task.handler = appAvHandleMessage;

    /* Clear instance table */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        theAv->av_inst[instance] = NULL;
    }

    /* Initialise state */
    theAv->suspend_state = 0;
    theAv->state = AV_STATE_NULL;
    appAvSetState(theAv, AV_STATE_INITIALISING_A2DP);

    /* Initialise client list */
    theAv->avrcp_client_list = appTaskListInit();
    appAvAvrcpClientRegister(&theAv->task, 0);

    /* Initialse client list for sending AV status indications */
    theAv->av_status_client_list = appTaskListInit();
    appAvStatusClientRegister(&theAv->task);

    /* Register to receive notifications of (dis)connections */
    appConManagerRegisterConnectionsClient(&theAv->task);
}


/*! \brief Register a task to receive avrcp messages from the av module

    \note This function can be called multiple times for the same task. It 
      will only appear once on a list.

    \param  client_task Task to be added to the list of registered clients
    \param  interests   Not used at present
 */
void appAvAvrcpClientRegister(Task client_task, uint8 interests)
{
    avTaskData *theAv = appGetAv();
    UNUSED(interests);

    /* Add client task to list */
    appTaskListAddTask(theAv->avrcp_client_list, client_task);
}


/*! \brief Register a task to receive AV status messages

    \note This function can be called multiple times for the same task. It 
      will only appear once on a list.

    \param  client_task Task to be added to the list of registered clients
 */
void appAvStatusClientRegister(Task client_task)
{
    avTaskData *theAv = appGetAv();

    /* Add client task to list */
    appTaskListAddTask(theAv->av_status_client_list, client_task);
}


/*! \brief Connect A2DP to a specific Bluetooth device 

    This function requests an A2DP connection. If there is no AV entry 
    for the device, it will be created. If the AV already exists any
    pending link destructions for AVRCP and A2DP will be cancelled.

    If there is an existing A2DP connection, then the function will 
    return FALSE.

    \param  bd_addr     Bluetooth address of device to connect to
    \param  a2dp_flags  Flags to apply to connection. Can be combined as a bitmask.

    \return TRUE if the connection has been requested. FALSE otherwise, including
        when a connection already exists.
*/
bool appAvA2dpConnectRequest(const bdaddr *bd_addr, appAvA2dpConnectFlags a2dp_flags)
{
    avInstanceTaskData *theInst;

    /* Check if AV instance to this device already exists */
    theInst = appAvInstanceFindFromBdAddr(bd_addr);
    if (theInst == NULL)
    {
        /* No AV instance for this device, so create new instance */
        theInst = appAvInstanceCreate(bd_addr);
    }
    else
    {
        /* Make sure there's no pending destroy message */
        MessageCancelAll(&theInst->av_task, AV_INTERNAL_A2DP_DESTROY_REQ);
        MessageCancelAll(&theInst->av_task, AV_INTERNAL_AVRCP_DESTROY_REQ);
    }

    /* Now check we have an AV instance */
    if (theInst)
    {
        /* Check A2DP is not already connected */
        if (!appA2dpIsConnected(theInst))
        {
            /* Send AV_INTERNAL_A2DP_CONNECT_REQ to start A2DP connection */
            MAKE_AV_MESSAGE(AV_INTERNAL_A2DP_CONNECT_REQ);

            DEBUG_LOGF("appAvConnectWithBdAddr A2DP, %p, %x %x %lx",
                         (void *)theInst, bd_addr->nap, bd_addr->uap, bd_addr->lap);

            /* Send message to newly created AV instance to connect A2DP */
            message->num_retries = 0;
            message->flags = (unsigned)a2dp_flags;
            MessageCancelFirst(&theInst->av_task, AV_INTERNAL_A2DP_CONNECT_REQ);
            MessageSendConditionally(&theInst->av_task, AV_INTERNAL_A2DP_CONNECT_REQ, message,
                                     appConManagerCreateAcl(&theInst->bd_addr));

            return TRUE;
        }
    }

    return FALSE;
}

/*! \brief Connect AVRCP to a specific Bluetooth device after a delay (possibly 0)

    This function requests an AVRCP connection after a specified delay period.
    If there is no AV entry for the device, it will be created. No check is
    made for an existing AVRCP connection, but if the AV already exists any
    pending link destructions for AVRCP and A2DP will be cancelled.

    If the function returns TRUE, then the client_task should receive an
    AV_AVRCP_CONNECT_CFM message whether the connection succeeds or not. See note.

    \note If there was no existing AV entry for the device, and hence no ACL,
    then the AV_AVRCP_CONNECT_CFM message will not be sent if the ACL could not
    be created.

    \param  client_task Task to receive response messages
    \param  bd_addr     Bluetooth address of device to connect to
    \param  delay_ms    Delay to use before actually performing the connection. This can be zero.

    \return TRUE if the connection has been requested, FALSE otherwise
*/
bool appAvAvrcpConnectLaterRequest(Task client_task, const bdaddr *bd_addr, uint32 delay_ms)
{
    avInstanceTaskData *theInst;

    /* Check if AV instance to this device already exists */
    theInst = appAvInstanceFindFromBdAddr(bd_addr);
    if (theInst == NULL)
    {
        /* No AV instance for this device, so create new instance */
        theInst = appAvInstanceCreate(bd_addr);
    }
    else
    {
        /* Make sure there's no pending destroy message */
        MessageCancelAll(&theInst->av_task, AV_INTERNAL_A2DP_DESTROY_REQ);
        MessageCancelAll(&theInst->av_task, AV_INTERNAL_AVRCP_DESTROY_REQ);
    }

    /* Now check we have an AV instance */
    if (theInst)
    {
        /* Send AV_INTERNAL_AVRCP_CONNECT_REQ to start AVRCP connection */
        MAKE_AV_MESSAGE(AV_INTERNAL_AVRCP_CONNECT_REQ);

        DEBUG_LOGF("appAvConnectWithBdAddr AVRCP, %p, %x %x %lx, %u",
                     (void *)theInst, bd_addr->nap, bd_addr->uap, bd_addr->lap, delay_ms);

        /* Send message to newly created AV instance to connect AVRCP */
        message->client_task = client_task;
        message->delay_ms = delay_ms;
        MessageCancelFirst(&theInst->av_task, AV_INTERNAL_AVRCP_CONNECT_REQ);
        MessageSendConditionally(&theInst->av_task, AV_INTERNAL_AVRCP_CONNECT_REQ, message,
                                 appConManagerCreateAcl(&theInst->bd_addr));

        return TRUE;
    }

    return FALSE;
}


/*! \brief Application response to a connection request

    After a connection request has been received and processed by the
    application, this function should be called to accept or reject the
    request.

    \param[in] client_task    Task responding, that will receive subsequent messages
    \param[in] bd_addr        Bluetooth address of connected device 
    \param     connection_id  Connection ID
    \param     signal_id      Signal ID
    \param     accept         Whether to accept or reject the connection
 */
void appAvAvrcpConnectResponse(Task client_task, const bdaddr *bd_addr,
                               uint16 connection_id, uint16 signal_id, avAvrcpAccept accept)
{
    avInstanceTaskData *av_inst;

    /* Get AV instance for this device */
    av_inst = appAvInstanceFindFromBdAddr(bd_addr);
    if (av_inst)
    {
        MAKE_AV_MESSAGE(AV_INTERNAL_AVRCP_CONNECT_RES);
        message->client_task = client_task;
        message->accept = accept;
        message->connection_id = connection_id;
        message->signal_id = signal_id;
        MessageSend(&av_inst->av_task, AV_INTERNAL_AVRCP_CONNECT_RES, message);
    }
    else
    {
        Panic();
    }
}


/*! \brief Request disconnection of A2DP from specified AV

    \param[in] av_inst  Instance to disconnect A2DPfrom

    \return TRUE if disconnect has been requested
 */
bool appAvA2dpDisconnectRequest(avInstanceTaskData *av_inst)
{
    if (av_inst)
    {
        MAKE_AV_MESSAGE(AV_INTERNAL_A2DP_DISCONNECT_REQ);
        message->flags = 0;
        PanicFalse(appAvIsValidInst(av_inst));
        MessageSendConditionally(&av_inst->av_task, AV_INTERNAL_A2DP_DISCONNECT_REQ,
                                 message, &appA2dpGetLock(av_inst));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*! \brief Request disconnection of AVRCP notifying the specified client. 

    \param[in] client_task  Task to receive disconnect confirmation
    \param[in] av_inst      Instance to disconnect AVRCP from

    \return TRUE if disconnect has been requested
 */
bool appAvAvrcpDisconnectRequest(Task client_task, avInstanceTaskData *av_inst)
{
    if (av_inst)
    {
        MAKE_AV_MESSAGE(AV_INTERNAL_AVRCP_DISCONNECT_REQ);
        message->client_task = client_task;
        PanicFalse(appAvIsValidInst(av_inst));
        MessageSendConditionally(&av_inst->av_task, AV_INTERNAL_AVRCP_DISCONNECT_REQ,
                                 message, &appAvrcpGetLock(av_inst));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*! \brief Connect to peer device

    \return TRUE if a connection was requested, FALSE in all other cases
      including when there is already a connection 
 */
bool appAvConnectPeer(void)
{
    bdaddr bd_addr;

    /* Get peer device address */
    if (appDeviceGetPeerBdAddr(&bd_addr))
    {
        return appAvA2dpConnectRequest(&bd_addr, A2DP_CONNECT_NOFLAGS);
    }
    else
    {
        return FALSE;
    }
}

/*! \brief Disconnect AVRCP and A2DP from the peer earbud. 

    \return TRUE if a disconnection was requested and is pending, FALSE
        otherwise.
*/
bool appAvDisconnectPeer(void)
{
    bdaddr bd_addr;

    /* Get peer device address */
    if (appDeviceGetPeerBdAddr(&bd_addr))
    {
        avInstanceTaskData *av_inst = appAvInstanceFindFromBdAddr(&bd_addr);
        return appAvA2dpDisconnectRequest(av_inst) || appAvAvrcpDisconnectRequest(&av_inst->av_task, av_inst);
    }
    else
    {
        return FALSE;
    }
}


/*! \brief Connect to a handset 

    This connects to the first handset from device manager if that handset
    supports A2DP (which is likely).

    \return TRUE if a connection was requested, FALSE in all other cases
      including when there is already a connection 
 */
bool appAvConnectHandset(void)
{
    bdaddr bd_addr;

    /* TODO: Scan through devices until find one that supports A2DP */

    /* Get handset device address */
    if (appDeviceGetHandsetBdAddr(&bd_addr) && appDeviceIsA2dpSupported(&bd_addr))
    {
        return appAvA2dpConnectRequest(&bd_addr, A2DP_CONNECT_MEDIA);
    }

    return FALSE;
}

/*! \brief Disconnect AVRCP and A2DP from the handset. 

    Disconnect any AVRCP or A2DP connections from the handset.

    \return TRUE if a disconnection was requested and is pending, FALSE
        otherwise.
 */
bool appAvDisconnectHandset(void)
{
    bdaddr bd_addr;

    if (appDeviceGetHandsetBdAddr(&bd_addr))
    {
        avInstanceTaskData *av_inst = appAvInstanceFindFromBdAddr(&bd_addr);
        return appAvA2dpDisconnectRequest(av_inst) || appAvAvrcpDisconnectRequest(&av_inst->av_task, av_inst);
    }
    else
    {
        return FALSE;
    }
}

/*! \brief Initiate an AV connection to a Bluetooth address

    AV connections are started with an A2DP connection.

    \param  bd_addr Address to connect to

    \return TRUE if a connection has been initiated. FALSE in all other cases
            included if there is an existing connection.
 */
bool appAvConnectWithBdAddr(const bdaddr *bd_addr)
{
    /* See if known */
    if (appDeviceFindBdAddrAttributes(bd_addr, NULL))
    {
        return appAvA2dpConnectRequest(bd_addr, A2DP_CONNECT_MEDIA);
    }
    else
    {
        return FALSE;
    }
}

/*! \brief Suspend AV link

    This function is called whenever a module in the headset has a reason to
    suspend AV streaming.  An internal message is sent to every AV
    instance, if the instance is currently streaming it will attempt to
    suspend.

    \note There may be multple reasons that streaming is suspended at any time.

    \param  reason  Why streaming should be suspended. What activity has started.
*/

void appAvStreamingSuspend(avSuspendReason reason)
{
    int instance;
    avTaskData *theAv = appGetAv();
    DEBUG_LOGF("appAvStreamingSuspend(%x)", reason);

    /* Update suspend state for any newly created AV instances */
    theAv->suspend_state |= reason;

    /* Send suspend to all instances in the table that support the specified SEIDs */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *av_inst = theAv->av_inst[instance];
        if (av_inst != NULL)
        {
            MAKE_AV_MESSAGE(AV_INTERNAL_A2DP_SUSPEND_MEDIA_REQ);

            /* Send message to AV instance */
            message->reason = reason;
            MessageSendConditionally(&av_inst->av_task, AV_INTERNAL_A2DP_SUSPEND_MEDIA_REQ,
                                     message, &appA2dpGetLock(av_inst));
        }
    }
}

/*! \brief Resume AV link

    This function is called whenever a module in the headset has cleared it's
    reason to suspend AV streaming.  An internal message is sent to every AV
    instance.

    \note There may be multple reasons why streaming is currently suspended. All
      all suspend reasons have to be cleared before the AV instance will
      attempt to resume streaming.

    \param  reason  Why streaming can now be resumed. What activity has completed.
*/
void appAvStreamingResume(avSuspendReason reason)
{
    int instance;
    avTaskData *theAv = appGetAv();
    DEBUG_LOGF("appAvStreamingResume(%x)", reason);

    /* Update suspend state for any newly created AV instances */
    theAv->suspend_state &= ~reason;

    /* Send suspend to all instances in the table that support the specified SEIDs */
    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        avInstanceTaskData *av_inst = theAv->av_inst[instance];
        if (av_inst != NULL)
        {
            MAKE_AV_MESSAGE(AV_INTERNAL_A2DP_RESUME_MEDIA_REQ);

            /* Send message to AV instance */
            message->reason = reason;
            MessageSendConditionally(&av_inst->av_task, AV_INTERNAL_A2DP_RESUME_MEDIA_REQ,
                                     message, &appA2dpGetLock(av_inst));
        }
    }
}

/*! \brief Send remote control command

    This function is called to send an AVRCP Passthrough (remote control) command, if there are
    multiple AV instances then it will pick the instance which is an A2DP sink.

    If there is no AVRCP connection then it will attempt to connect first before sending the
    passthrough command.
*/
static void appAvRemoteControl(avc_operation_id op_id, uint8 rstate, bool beep, uint16 repeat)
{
    avInstanceTaskData *theInst = appAvInstanceFindAvrcpForPassthrough();
    DEBUG_LOG("appAvRemoteControl");

    /* Check there is an instance to send AVRCP passthrough */
    if (theInst)
    {
        /* Send AVRCP passthrough request, will be block until AVRCP connected */
        appAvrcpRemoteControl(theInst, op_id, rstate, beep, repeat);
    }
}

/*! \brief Send Pause command

    \param  ui  Indicates if there should be a User Interface action as a result
            of the sent pause command (that is should there be a beep).
*/
void appAvPause(bool ui)
{
    DEBUG_LOGF("appAvPause(%d)", ui);

    /* Check if we should send AVRCP_PAUSE */
    if (appAvPlayStatus() != avrcp_play_status_paused)
    {
        appAvRemoteControl(opid_pause, 0, ui, 0);
        appAvRemoteControl(opid_pause, 1, FALSE, 0);

        appAvHintPlayStatus(avrcp_play_status_paused);
    }
}

/*! \brief Send Play command

    \param  ui  Indicates if there should be a User Interface action as a result
            of the sent play command (that is should there be a beep).
*/
void appAvPlay(bool ui)
{
    DEBUG_LOGF("appAvPlay(%d)", ui);

    /* Check if we should send AVRCP_PLAY or if we don't know */
    if (appAvPlayStatus() != avrcp_play_status_playing)
    {
        appAvRemoteControl(opid_play, 0, ui, 0);
        appAvRemoteControl(opid_play, 1, FALSE, 0);

        appAvHintPlayStatus(avrcp_play_status_playing);
    }
}

/*! \brief Send Play or Pause command

    \param  ui  Indicates if there should be a User Interface action as a result
            of the sent command (that is should there be a beep).
*/
void appAvPlayToggle(bool ui)
{
    DEBUG_LOG("appAvPlayToggle");

    /* If we don't know our state, send play */
    if (appAvPlayStatus() != avrcp_play_status_playing)
        appAvPlay(ui);
    else
        appAvPause(ui);
}

/*! \brief Send Stop command

    \param  ui  Indicates if there should be a User Interface action as a result
                of the sent stop command (that is should there be a beep).
*/
void appAvStop(bool ui)
{
    DEBUG_LOGF("appAvStop(%d)", ui);

    appAvRemoteControl(opid_stop, 0, ui, 0);
    appAvRemoteControl(opid_stop, 1, FALSE, 0);

    appAvHintPlayStatus(avrcp_play_status_stopped);
}

/*! \brief Send Forward command
*/
void appAvForward(void)
{
    DEBUG_LOG("appAvForward");
    appAvRemoteControl(opid_forward, 0, TRUE, 0);
    appAvRemoteControl(opid_forward, 1, FALSE, 0);
}

/*! \brief Send Backward command
*/
void appAvBackward(void)
{
    DEBUG_LOG("appAvBackward");
    appAvRemoteControl(opid_backward, 0, TRUE, 0);
    appAvRemoteControl(opid_backward, 1, FALSE, 0);
}

/*! \brief Send Volume Up start command
*/
void appAvVolumeUpRemoteStart(void)
{
    DEBUG_LOG("appAvVolumeUpRemoteStart");
    appAvRemoteControl(opid_volume_up, 0, FALSE, APP_UI_VOLUME_REPEAT_TIME);
}

/*! \brief Send Volume Up stop command
*/
void appAvVolumeUpRemoteStop(void)
{
    DEBUG_LOG("appAvVolumeUpRemoteStop");
    appAvRemoteControl(opid_volume_up, 1, FALSE, APP_UI_VOLUME_REPEAT_TIME);
}

/*! \brief Send Volume Down start command
*/
void appAvVolumeDownRemoteStart(void)
{
    DEBUG_LOG("appAvVolumeDownRemoteStart");
    appAvRemoteControl(opid_volume_down, 0, FALSE, APP_UI_VOLUME_REPEAT_TIME);
}

/*! \brief Send Volume Down stop command
*/
void appAvVolumeDownRemoteStop(void)
{
    DEBUG_LOG("appAvVolumeDownRemoteStop");
    appAvRemoteControl(opid_volume_down, 1, FALSE, APP_UI_VOLUME_REPEAT_TIME);
}

/*! \brief Send Fast Forward start command
*/
void appAvFastForwardStart(void)
{
    DEBUG_LOG("appAvFastForwardStart");
    appAvRemoteControl(opid_fast_forward, 0, TRUE, D_SEC(1));
}

/*! \brief Send Fast Forward stop command
*/
void appAvFastForwardStop(void)
{
    DEBUG_LOG("appAvFastForwardStop");
    appAvRemoteControl(opid_fast_forward, 1, TRUE, 0);
}

/*! \brief Send Rewind start command
*/
void appAvRewindStart(void)
{
    DEBUG_LOG("appAvRewindStart");
    appAvRemoteControl(opid_rewind, 0, TRUE, D_SEC(1));
}

/*! \brief Send Rewind stop command
*/
void appAvRewindStop(void)
{
    DEBUG_LOG("appAvRewindStop");
    appAvRemoteControl(opid_rewind, 1, TRUE, 0);
}


/*! \brief Forward volume to any other instance 

    Synchronise the volume with our other links, whether we are sink
    to a handset or either end of a TWS link to our peer.

    The volume is not sent to a TWS peer on initial connection. 

    \param[in] from_inst AV instance of the connection the volume has come from, 
      or NULL if we just want to synchronise volume
    \param on_connect    Whether this is the initial volume sync
*/
void appAvVolumeSync(avInstanceTaskData *from_inst, bool on_connect)
{
    /* Find A2DP instance where we're the sink */
    avInstanceTaskData *theSinkInst = appAvGetA2dpSink(AV_CODEC_NON_TWS);
    avInstanceTaskData *theTwsSinkInst = appAvGetA2dpSink(AV_CODEC_TWS);

    /* Find A2DP instance where we're the source */
    avInstanceTaskData *theSourceInst = appAvGetA2dpSource();

    /* Forward volume to A2DP source (we're the sink) */
    if (theSinkInst && appAvrcpIsConnected(theSinkInst))
    {
        if (theSinkInst != from_inst)
        {
            DEBUG_LOGF("appAvVolumeSync, send volume %u to source %p", theSinkInst->a2dp_volume, theSinkInst);
            if (appAvrcpIsEventRegistered(theSinkInst, avrcp_event_volume_changed))
                AvrcpEventVolumeChangedResponse(theSinkInst->avrcp, avctp_response_changed, theSinkInst->a2dp_volume);
            appA2dpVolumeSet(theSinkInst, theSinkInst->a2dp_volume);
        }
    }

    /* Forward volume to A2DP TWS source (we're the sink) */
    if (theTwsSinkInst && appAvrcpIsConnected(theTwsSinkInst) && !on_connect)
    {
        if (theTwsSinkInst != from_inst)
        {
            DEBUG_LOGF("appAvVolumeSync, send volume %u to TWS source %p", theTwsSinkInst->a2dp_volume, theTwsSinkInst);
            if (appAvrcpIsEventRegistered(theTwsSinkInst, avrcp_event_volume_changed))
                AvrcpEventVolumeChangedResponse(theTwsSinkInst->avrcp, avctp_response_changed, theTwsSinkInst->a2dp_volume);
            appA2dpVolumeSet(theTwsSinkInst, theTwsSinkInst->a2dp_volume);
        }
    }

    if (theSinkInst && theSourceInst)
    {
        /* Copy volume from sink instance to source instance */
        theSourceInst->a2dp_volume = theSinkInst->a2dp_volume;

        if (appAvrcpIsConnected(theSourceInst))
        {
            if (theSourceInst != from_inst)
            {
                DEBUG_LOGF("appAvVolumeSync, send volume %u to TWS sink %p", theSourceInst->a2dp_volume, theSourceInst);
                AvrcpSetAbsoluteVolumeRequest(theSourceInst->avrcp, theSourceInst->a2dp_volume);
                appA2dpVolumeSet(theSourceInst, theSourceInst->a2dp_volume);
            }
        }
    }
}

/*! \brief Set new volume

    Set a new volume, and synchronise across other AV links if
    required.

    This function only has any effect if we are the sink for an 
    audio stream.

    \param      volume      The new volume
    \param[in]  from_inst   The instance that the volume came from
      or NULL if the volume is from the UI.
*/
void appAvVolumeSet(uint16 volume, avInstanceTaskData *from_inst)
{
    avInstanceTaskData *theInst = appAvGetA2dpSink(AV_CODEC_ANY);
    DEBUG_LOGF("appAvVolumeSet, volume %u, from %p", volume, from_inst);

    /* Check there is AV Sink instance */
    if (theInst)
    {
        /* Store new volume */
        theInst->a2dp_volume = volume;

        /* Synchronise volume with handset and/or peer earbud */
        appAvVolumeSync(from_inst, FALSE);

        /* Set volume locally */
        appA2dpVolumeSet(theInst, theInst->a2dp_volume);
    }
}

/*! \brief Make volume change

    \note A2DP/AVRCP volume range is 0-127. 
    \note Increasing the volume by 100 from 100 will return TRUE as the 
    volume changed but the level set will be the maximum - 127

    \param  step    Relative amount to adjust the volume by. 
                    This can be negative.

    \return TRUE if the volume was changed by at least 1 level, 
            FALSE in all other cases
*/
bool appAvVolumeChange(int16 step)
{
    avInstanceTaskData *theInst = appAvGetA2dpSink(AV_CODEC_ANY);
    DEBUG_LOGF("appAvVolumeChange, step %d", step);

    if (theInst)
    {
        uint8 volume = theInst->a2dp_volume;

        /* Check if increasing volume */
        if (step > 0)
        {
            /* Adjust volume if not at limit */
            if (volume < 127)
                volume = ((volume + step) <= 127) ? volume + step : 127;
            else
                return FALSE;
        }
        else
        {
            /* Adjust volume if not at limit */
            if (volume > 0)
                volume = (volume >= -step) ? volume + step : 0;
            else
                return FALSE;
        }

        appAvVolumeSet(volume, NULL);

        /* Return indicating volume changed successfully */
        return TRUE;
    }
    else
        return FALSE;
}

/*! \brief Start a repeated volume change

    Change the volume by the supplied step, and then request a repeated
    volume increase.

    If the volume is already at a limit (max or min) then a UI tone 
    will be played, and there will not be a repeat.

    \param  step    Relative amount to adjust the volume by. 
                    This can be negative.

    \return TRUE if the volume was changed by at least 1 level, 
            FALSE in all other cases
*/
bool appAvVolumeRepeat(int16 step)
{
    /* Handle volume change locally */
    if (appAvVolumeChange(step))
    {
        MAKE_AV_MESSAGE(AV_INTERNAL_VOLUME_REPEAT);

        /* Send repeat message later */
        message->step = step;
        MessageSendLater(&appGetAv()->task, step > 0 ? AV_INTERNAL_VOLUME_UP_REPEAT : AV_INTERNAL_VOLUME_DOWN_REPEAT,
                         message, APP_UI_VOLUME_REPEAT_TIME);

        /* Return indicating volume changed */
        return TRUE;
    }

    /* Play volume limit tone */
    appUiAvVolumeLimit();

    /* Return indicating volume not changed */
    return FALSE;
}

/*! \brief Start volume change

    Start a repeated volume change either remotely (tell the handset)
    or locally.

    \param step Relative amount to adjust the volume by. 
     This can be negative.
*/
void appAvVolumeStart(int16 step)
{
    avInstanceTaskData *theInst = appAvGetA2dpSink(AV_CODEC_ANY);
    DEBUG_LOGF("appAvVolumeStart(%d)", step);

    /* Check there is AV TWS Sink instance */
    if (theInst && appA2dpIsSeidTws(theInst->a2dp_current_seid))
    {
        /* Use AVRCP to send volume to TWS source */
        if (step > 0)
            appAvVolumeUpRemoteStart();
        else
            appAvVolumeDownRemoteStart();
    }
    else if (appAvVolumeRepeat(step))
    {
        if (step > 0)
            appUiAvVolumeUp();
        else
            appUiAvVolumeDown();
    }
}

/*! \brief Stop volume change

    Stop a repeated volume change either remotely (tell the handset)
    or locally.

    \param step The step is required so that the correct direction of 
    volume can be cancelled at the handset.
*/
void appAvVolumeStop(int16 step)
{
    avInstanceTaskData *theInst = appAvGetA2dpSink(AV_CODEC_ANY);

    /* Check there is AV TWS Sink instance */
    if (theInst && appA2dpIsSeidTws(theInst->a2dp_current_seid))
    {
        if (step > 0)
            appAvVolumeUpRemoteStop();
        else
            appAvVolumeDownRemoteStop();
    }
    else
    {
        if (MessageCancelFirst(&appGetAv()->task, AV_INTERNAL_VOLUME_UP_REPEAT))
            appUiAvVolumeUp();
        if (MessageCancelFirst(&appGetAv()->task, AV_INTERNAL_VOLUME_DOWN_REPEAT))
            appUiAvVolumeDown();
    }
}


/*! \brief Check if an instance is valid 

    Checks if the instance passed is still a valid AV. This allows 
    you to check whether theInst is still valid.

    \param  theInst Instance to check

    \returns TRUE if the instance is valid, FALSE otherwise
 */
bool appAvIsValidInst(avInstanceTaskData* theInst)
{
    avTaskData *theAv = appGetAv();
    int instance;

    if (!theInst)
        return FALSE;

    for (instance = 0; instance < AV_MAX_NUM_INSTANCES; instance++)
    {
        if (theInst == theAv->av_inst[instance])
            return TRUE;
    }
    return FALSE;
}

/*! \brief Message Handler

    This function is the main message handler for the AV module, every
    message is handled in it's own seperate handler function.  The switch
    statement is broken into seperate blocks to reduce code size, if execution
    reaches the end of the function then it is assumed that the message is
    unhandled.
*/
void appAvHandleMessage(Task task, MessageId id, Message message)
{
    avTaskData *theAv = (avTaskData *)task;
    avState state = appAvGetState(theAv);

    /* Handle connection manager messages */
    switch (id)
    {
        case CON_MANAGER_CONNECTION_IND:
            appAvHandleConManagerConnectionInd((CON_MANAGER_CONNECTION_IND_T *)message);
            return;
    }

    /* Handle A2DP messages */
    switch (id)
    {
        case A2DP_INIT_CFM:
        {
            switch (state)
            {
                case AV_STATE_INITIALISING_A2DP:
                    appAvHandleA2dpInitConfirm(theAv, (A2DP_INIT_CFM_T *)message);
                    return;
                default:
                    appHandleUnexpected(id);
                    return;
            }
        }

        case AVRCP_INIT_CFM:
        {
            switch (state)
            {
                case AV_STATE_INITIALISING_AVRCP:
                    appAvHandleAvrcpInitConfirm(theAv, (AVRCP_INIT_CFM_T *)message);
                    return;
                default:
                    appHandleUnexpected(id);
                    return;
            }
        }

        case A2DP_SIGNALLING_CONNECT_IND:
        {
            switch (state)
            {
                case AV_STATE_IDLE:
                    appAvHandleA2dpSignallingConnectIndication(theAv, (A2DP_SIGNALLING_CONNECT_IND_T *)message);
                    return;
                default:
                    appAvRejectA2dpSignallingConnectIndication(theAv, (A2DP_SIGNALLING_CONNECT_IND_T *)message);
                    return;
            }
        }

        case AVRCP_CONNECT_IND:
        {
            switch (state)
            {
                case AV_STATE_IDLE:
                    appAvHandleAvrcpConnectIndication(theAv, (AVRCP_CONNECT_IND_T *)message);
                    return;
                default:
                    appAvRejectAvrcpConnectIndication(theAv, (AVRCP_CONNECT_IND_T *)message);
                    return;
            }
        }

        case AV_INTERNAL_VOLUME_UP_REPEAT:
        case AV_INTERNAL_VOLUME_DOWN_REPEAT:
            appAvVolumeRepeat(((AV_INTERNAL_VOLUME_REPEAT_T *)message)->step);
            return;

        case AV_A2DP_CONNECTED_IND:
            appAvHandleAvA2dpConnectedIndication(theAv, (AV_A2DP_CONNECTED_IND_T *)message);
            return;

        case AV_A2DP_DISCONNECTED_IND:
            appAvHandleAvA2dpDisconnectedIndication(theAv, (AV_A2DP_DISCONNECTED_IND_T *)message);
            return;

        case AV_STREAMING_ACTIVE_IND:
        case AV_STREAMING_INACTIVE_IND:
        case AV_AVRCP_CONNECTED_IND:
        case AV_AVRCP_DISCONNECTED_IND:
            return;

        case AV_AVRCP_CONNECT_IND:
            appAvHandleAvAvrcpConnectIndication(theAv, (AV_AVRCP_CONNECT_IND_T *)message);
            return;

        case AV_AVRCP_CONNECT_CFM:
        case AV_AVRCP_DISCONNECT_IND:
            return;

        default:
            appAvError(theAv, id, message);
            return;
    }
}

#endif
