/*!
\copyright  Copyright (c) 2005 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_conn_rules.h
\brief      Connection Rules Module Interface
*/

#ifndef _AV_HEADSET_CONN_RULES_H_
#define _AV_HEADSET_CONN_RULES_H_

#include "av_headset_message.h"

/*! Enumeration of types of action that the connection rules engine can send
    to the state machine. */
enum    av_headset_conn_rules_messages
{
    /*! Make the device connectable. */
    CONN_RULES_ENABLE_CONNECTABLE = CONN_RULES_MESSAGE_BASE,

    /*! Make the device not connectable. */
    CONN_RULES_DISABLE_CONNECTABLE,

    /*! Start peer pairing. */
    CONN_RULES_PEER_PAIR,

    /*! Use Peer Signalling (AVRCP) to send sync message. */
    CONN_RULES_SEND_PEER_SYNC,

    /*! Use Peer Signalling (AVRCP) to send link-keys message. */
    CONN_RULES_PEER_SEND_LINK_KEYS,

    /*! Start handset pairing. */
    CONN_RULES_HANDSET_PAIR,

    /*! Connect HFP, A2DP and AVRCP to last connected handset. */
    CONN_RULES_CONNECT_HANDSET,

    /*! Connect A2DP and AVRCP to handset that peer is connected to (only TWS+). */
    CONN_RULES_CONNECT_PEER_HANDSET,

    /*! Connect A2DP and AVRCP for audio forwarding to peer earbud. */
    CONN_RULES_CONNECT_PEER,

    /*! Update TDL MRU for peer's handset */
    CONN_RULES_UPDATE_MRU_PEER_HANDSET,

    /*! Send status and role to handset */
    CONN_RULES_SEND_STATE_TO_HANDSET,

    /*! Start timer to pause A2DP streaming. */
    CONN_RULES_A2DP_TIMEOUT,

    /*! Start timer to pause A2DP streaming. */
    CONN_RULES_SCO_TIMEOUT,

    /*! Enable LEDs when out of ear. */
    CONN_RULES_LED_ENABLE,

    /*! Disable LEDs when in ear. */
    CONN_RULES_LED_DISABLE,

    /*! Disconnect links to handset and DFU not pending */
    CONN_RULES_HANDSET_DISCONNECT,

    /*! Disconnect link to peer. */
    CONN_RULES_PEER_DISCONNECT,

    /*! Enter DFU mode as we have entered the case with DFU pending */
    CONN_RULES_ENTER_DFU,

    /*! Allow connections from handset */
    CONN_RULES_ALLOW_HANDSET_CONNECT,

    /*! Reject connections from handset */
    CONN_RULES_REJECT_HANDSET_CONNECT,

    CONN_RULES_NOP
};

/*! \brief Current rule status */
typedef enum
{
    /*! rule has not been processed at all */
    RULE_STATUS_NOT_DONE,
    /*! rule has been called, but did not generated an action */
    RULE_STATUS_IGNORED,
    /*! rule has been called, generated an action, but has not yet been completed */
    RULE_STATUS_IN_PROGRESS,
    /*! rule has been called, generated an action and completed */
    RULE_STATUS_COMPLETE
} ruleStatus;

/*! Type for all the events that may have rules associated with them */
typedef enum
{
    RULE_EVENT_STARTUP                      = 1UL << 0,     /*!< Startup */

    RULE_EVENT_HANDSET_A2DP_CONNECTED       = 1UL << 1,     /*!< Handset connected */
    RULE_EVENT_HANDSET_A2DP_DISCONNECTED    = 1UL << 2,     /*!< Handset disconnected */
    RULE_EVENT_HANDSET_AVRCP_CONNECTED      = 1UL << 3,     /*!< Handset connected */
    RULE_EVENT_HANDSET_AVRCP_DISCONNECTED   = 1UL << 4,     /*!< Handset disconnected */
    RULE_EVENT_HANDSET_HFP_CONNECTED        = 1UL << 5,     /*!< Handset connected */
    RULE_EVENT_HANDSET_HFP_DISCONNECTED     = 1UL << 6,     /*!< Handset disconnected */

    RULE_EVENT_PEER_CONNECTED               = 1UL << 7,     /*!< Peer connected */
    RULE_EVENT_PEER_DISCONNECTED            = 1UL << 8,     /*!< Peer disconnected */
    RULE_EVENT_PEER_SYNC_VALID              = 1UL << 9,     /*!< Peer sync exchanged */
    RULE_EVENT_PEER_SYNC_FAILED             = 1UL << 10,    /*!< Peer sync failed */
    RULE_EVENT_PEER_UPDATE_LINKKEYS         = 1UL << 11,    /*!< Linkey for handset updated, potentially need to forward to peer */
    RULE_EVENT_RX_HANDSET_LINKKEY           = 1UL << 12,    /*!< Receive derived TWS linkkey from peer device */

    RULES_EVENT_BATTERY_CRITICAL            = 1UL << 13,    /*!< Battery voltage is critical */
    RULES_EVENT_BATTERY_LOW                 = 1UL << 14,    /*!< Battery voltage is low */
    RULES_EVENT_BATTERY_OK                  = 1UL << 15,    /*!< Battery voltage is OK */
    RULES_EVENT_CHARGER_CONNECTED           = 1UL << 16,    /*!< Charger is connected */
    RULES_EVENT_CHARGER_DISCONNECTED        = 1UL << 17,    /*!< Charger is disconnected */
    RULES_EVENT_CHARGING_COMPLETED          = 1UL << 18,    /*!< Charging is complete */

    RULE_EVENT_IN_EAR                       = 1UL << 19,    /*!< Earbud put in ear. */
    RULE_EVENT_OUT_EAR                      = 1UL << 20,    /*!< Earbud taken out of ear. */
    RULE_EVENT_IN_CASE                      = 1UL << 21,    /*!< Earbud put in the case. */
    RULE_EVENT_OUT_CASE                     = 1UL << 22,    /*!< Earbud taken out of case. */

    RULE_EVENT_MOTION_DETECTED              = 1UL << 23,    /*!< Earbud started moving. */
    RULE_EVENT_NO_MOTION_DETECTED           = 1UL << 24,    /*!< Earbud stopped moving. */

    RULE_EVENT_USER_CONNECT                 = 1UL << 25,    /*!< User has requested connect */
    RULE_EVENT_DFU_CONNECT                  = 1UL << 27,    /*!< Connect for DFU purposes */
} ruleEvent;

/*! Action required by the rules engine. This type is used as the 
    return value from a rule. */
typedef enum
{
    /*! Let the rule engine know that it should send the message defined for the rule */
    RULE_ACTION_RUN,
    /*! Let the rule engine know that no message is needed, as the rule has been completed */
    RULE_ACTION_COMPLETE,
    /*! Let the rule engine know that no message is needed, as the rule not required  */
    RULE_ACTION_IGNORE
} ruleAction;

/*! Type to hold a bitmask of active connection rules */
typedef uint32 connRulesEvents;

/*! \brief Connection Rules task data. */
typedef struct
{
    /*! Connection rules module task. */
    TaskData task;

    /*! Set of active events */
    connRulesEvents events;

    /*! Set of tasks registered for event actions */
    Task event_tasks[32];
} connRulesTaskData; 

/*! \brief Initialise the connection rules module. */
extern void appConnRulesInit(void);

/*! \brief Set an event or events
    \param client_task The client task to receive rule actions from this event
    \param event Events to set that will trigger rules to run
    This function is called to set an event or events that will cause the relevant
    rules in the rules table to run.  Any actions generated will be sent as message
    to the client_task
*/    
extern void appConnRulesSetEvent(Task client_task, connRulesEvents event);

/*! \brief Reset/clear an event or events 
    \param event Events to clear
    This function is called to clear an event or set of events that was previously
    set by appConnRulesSetEvent().  Clear event will reset any rule that was run for event.
*/    
extern void appConnRulesResetEvent(connRulesEvents event);

/*! \brief Get set of active events 
    \return The set of active events.
*/
extern connRulesEvents appConnRulesGetEvents(void);

/*! \brief Mark rules as complete from messaage ID 
    \param message Message ID that rule(s) generated
    This function is called to mark rules as completed, the message parameter
    is used to determine which rules can be marked as completed.
*/    
extern void appConnRulesSetRuleComplete(MessageId message);

/*! \brief Mark rules as complete from message ID and set of events
    \param message Message ID that rule(s) generated
    \param event Event or set of events that trigger the rule(s)
    This function is called to mark rules as completed, the message and event parameter
    is used to determine which rules can be marked as completed.
*/    
extern void appConnRulesSetRuleWithEventComplete(MessageId message, connRulesEvents event);

#endif /* _AV_HEADSET_CONN_RULES_H_ */

