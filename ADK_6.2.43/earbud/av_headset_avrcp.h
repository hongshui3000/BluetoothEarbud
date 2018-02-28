/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_avrcp.h
\brief      Header file for AVRCP State Machine

    @startuml

    [*] -down-> NULL
    NULL -down-> DISCONNECTED

    DISCONNECTED : No AVRCP connection
    DISCONNECTED -down-> CONNECTING_LOCAL : ConnectReq
    DISCONNECTED -down-> CONNECTING_LOCAL_WAIT_RESPONSE : ConnectReq/clients to notify
    DISCONNECTED -down-> CONNECTING_REMOTE_WAIT_RESPONSE : ConnectInd/clients to notify
    DISCONNECTED -down-> CONNECTING_REMOTE : ConnectInd

    CONNECTING_LOCAL_WAIT_RESPONSE -down-> CONNECTING_LOCAL : client notification complete
    CONNECTING_REMOTE_WAIT_RESPONSE -down-> CONNECTING_REMOTE : client notification complete

    CONNECTING_LOCAL -down-> CONNECTED : AVRCP_CONNECT_CFM
    CONNECTING_REMOTE -down-> CONNECTED : AVRCP_CONNECT_CFM

    CONNECTED -left-> DISCONNECTING : Disconnect Req/Ind

    DISCONNECTING -right-> DISCONNECTED
    
    @enduml

*/

#ifndef _AV_HEADSET_AVRCP_H_
#define _AV_HEADSET_AVRCP_H_

#include <avrcp.h>
#include "av_headset.h"

struct appDeviceAttributes;

/*! \brief Get operation lock */
#define appAvrcpGetLock(theInst)        ((theInst)->avrcp_lock)

/*!@{ \name Lock reasons 
    The AVRCP instance can be 'locked' for a number of reasons listed here, 
    potentially simultaneously.
    
    The lock can be used in MessageSendConditionally() to hold a message back
    until all locks are released.
*/
#define APP_AVRCP_LOCK_STATE            (1 << 0)
#define APP_AVRCP_LOCK_PASSTHROUGH_REQ  (1 << 1)
#define APP_AVRCP_LOCK_PASSTHROUGH_IND  (1 << 2)
/*!@} */

/*! \brief Set operation lock */
#define appAvrcpSetLock(theInst, lock)	((theInst)->avrcp_lock |= (lock))

/*! \brief Clear operation lock */
#define appAvrcpClearLock(theInst, lock)	((theInst)->avrcp_lock &= ~(lock))

/*! \brief Is AVRCP state is 'disconnected' */
#define appAvrcpIsDisconnected(theInst) \
    ((theInst)->avrcp_state == AVRCP_STATE_DISCONNECTED)

/*! \brief Is AVRCP state is 'connected' */
#define appAvrcpIsConnected(theInst) \
    ((theInst)->avrcp_state == AVRCP_STATE_CONNECTED)

/*! Check if the specified event type is supported */
#define appAvrcpIsEventRegistered(theInst, event) \
    ((theInst)->avcrp_supported_events & (1 << ((event) - 1)))

extern void appAvrcpInstanceInit(avInstanceTaskData *theAv);
extern avAvrcpState appAvrcpGetState(avInstanceTaskData *theAv);
extern void appAvrcpRemoteControl(avInstanceTaskData *theInst, avc_operation_id op_id, uint8 rstate, bool beep, uint16 repeat_ms);
extern bool appAvrcpIsValidClient(avInstanceTaskData *theInst, Task client_task);

extern void appAvrcpVendorPassthroughRequest(avInstanceTaskData *theInst, avc_operation_id op_id, uint16 size_payload, const uint8 *payload);
extern void appAvrcpVendorPassthroughResponse(avInstanceTaskData *theInst, avrcp_response_type);
extern Task appAvrcpVendorPassthroughRegister(avInstanceTaskData *theInst, Task client_task);

extern void appAvrcpInstanceHandleMessage(avInstanceTaskData *theInst, MessageId id, Message message);

#endif
