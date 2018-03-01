/*!
\copyright  Copyright (c) 2005 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_conn_rules.c
\brief	    Connection Rules Module
*/

#include "av_headset.h"
#include "av_headset_conn_rules.h"
#include "av_headset_device.h"
#include "av_headset_log.h"
#include "av_headset_av.h"
#include "av_headset_phy_state.h"

#include <bdaddr.h>
#include <panic.h>

#pragma unitsuppress Unused

/*! \{
    Macros for diagnostic output that can be suppressed.
    Allows debug of the rules module at two levels. */
#define CONNRULES_LOG(x)       DEBUG_LOG(x)
#define CONNRULES_LOGF(x, ...) DEBUG_LOGF(x, __VA_ARGS__)

#define RULE_LOG(x)         DEBUG_LOG(x)
#define RULE_LOGF(x, ...)   DEBUG_LOGF(x, __VA_ARGS__)
/*! \} */

/*! \brief Flags to control rule calling.

    NOT USED YET.
*/
typedef enum
{
    RULE_FLAG_NO_FLAGS  = 0x00,
} ruleFlags;

/*! \brief Function pointer definition for a rule */
typedef ruleStatus (*ruleFunc)(void);

/*! \brief Definition of a rule entry. */
typedef struct
{
    /*! Events that trigger this rule */
    connRulesEvents events;

    /*! Current state of the rule. */
    ruleStatus status;

    ruleFlags flags;

    /*! Pointer to the function to evaluate the rule. */
    ruleFunc rule;

    /*! Message to send when rule determines action to be run. */
    MessageId message;
} ruleEntry;

/*! Macro to declare a function, based on the name of a rule */
#define DEFINE_RULE(name) \
    static ruleAction name(void)

/*! Macro used to create an entry in the rules table */
#define RULE(event, name, message) \
    { event, RULE_STATUS_NOT_DONE, RULE_FLAG_NO_FLAGS, name, message }

/*! \{
    Rule function prototypes, so we can build the rule tables below. */
DEFINE_RULE(rulePeerPair);
DEFINE_RULE(ruleAutoHandsetPair);
DEFINE_RULE(rulePeerSync);
DEFINE_RULE(ruleForwardLinkKeys);

DEFINE_RULE(ruleSyncConnectHandset);
DEFINE_RULE(ruleSyncConnectPeerHandset);
DEFINE_RULE(ruleSyncConnectPeer);
DEFINE_RULE(ruleDisconnectPeer);

DEFINE_RULE(ruleNoSyncConnectHandset);

DEFINE_RULE(ruleUserConnectHandset);
DEFINE_RULE(ruleUserConnectPeerHandset);
DEFINE_RULE(ruleUserConnectPeer);

DEFINE_RULE(ruleOutOfCaseConnectHandset);
DEFINE_RULE(ruleOutOfCaseConnectPeerHandset);
DEFINE_RULE(ruleOutOfCaseConnectPeer);

DEFINE_RULE(ruleUpdateMruHandset);
DEFINE_RULE(ruleSendStatusToHandset);
DEFINE_RULE(ruleOutOfEarA2dpActive);
DEFINE_RULE(ruleOutOfEarScoActive);
DEFINE_RULE(ruleOutOfEarLedsEnable);
DEFINE_RULE(ruleInEarLedsDisable);
DEFINE_RULE(ruleInCaseDisconnectHandset);
DEFINE_RULE(ruleInCaseDisconnectPeer);
DEFINE_RULE(ruleInCaseEnterDfu);
DEFINE_RULE(ruleOutOfCaseAllowHandsetConnect);
DEFINE_RULE(ruleInCaseRejectHandsetConnect);
DEFINE_RULE(ruleDfuAllowHandsetConnect);
/*! \} */

/*! \brief Set of rules to run on Earbud startup. */
ruleEntry appConnRules[] =
{
    /*! \{
        Startup (power on) rules */
    RULE(RULE_EVENT_STARTUP,                    rulePeerPair,               CONN_RULES_PEER_PAIR),
    RULE(RULE_EVENT_STARTUP,                    rulePeerSync,               CONN_RULES_SEND_PEER_SYNC),
    /*! \} */

    RULE(RULE_EVENT_PEER_UPDATE_LINKKEYS,       ruleForwardLinkKeys,        CONN_RULES_PEER_SEND_LINK_KEYS),
    RULE(RULE_EVENT_PEER_CONNECTED,             ruleForwardLinkKeys,        CONN_RULES_PEER_SEND_LINK_KEYS),

    /*! \{
        Rules that are run when handset connects */
    RULE(RULE_EVENT_HANDSET_A2DP_CONNECTED,     rulePeerSync,               CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_HANDSET_A2DP_CONNECTED,     ruleSendStatusToHandset,    CONN_RULES_SEND_STATE_TO_HANDSET),
    RULE(RULE_EVENT_HANDSET_AVRCP_CONNECTED,    rulePeerSync,               CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_HANDSET_AVRCP_CONNECTED,    ruleSendStatusToHandset,    CONN_RULES_SEND_STATE_TO_HANDSET),
    RULE(RULE_EVENT_HANDSET_HFP_CONNECTED,      rulePeerSync,               CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_HANDSET_HFP_CONNECTED,      ruleSendStatusToHandset,    CONN_RULES_SEND_STATE_TO_HANDSET),
    /*! \} */

    /*! \{
        Rules that are run when handset disconnects */
    RULE(RULE_EVENT_HANDSET_A2DP_DISCONNECTED,  rulePeerSync,               CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_HANDSET_AVRCP_DISCONNECTED, rulePeerSync,               CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_HANDSET_HFP_DISCONNECTED,   rulePeerSync,               CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_HANDSET_A2DP_DISCONNECTED,  ruleDisconnectPeer,         CONN_RULES_PEER_DISCONNECT),
    RULE(RULE_EVENT_HANDSET_AVRCP_DISCONNECTED, ruleDisconnectPeer,         CONN_RULES_PEER_DISCONNECT),

    /*! \{
        Receive handset link-key from peer */
    RULE(RULE_EVENT_RX_HANDSET_LINKKEY,         rulePeerSync,               CONN_RULES_SEND_PEER_SYNC),
    /*! \} */

    /*! \{
        Rules that are run when peer synchronisation is successful */
    RULE(RULE_EVENT_PEER_SYNC_VALID,            ruleSyncConnectPeer,        CONN_RULES_CONNECT_PEER),
    RULE(RULE_EVENT_PEER_SYNC_VALID,            ruleSyncConnectHandset,     CONN_RULES_CONNECT_HANDSET),
    RULE(RULE_EVENT_PEER_SYNC_VALID,            ruleSyncConnectPeerHandset, CONN_RULES_CONNECT_PEER_HANDSET),
    RULE(RULE_EVENT_PEER_SYNC_VALID,            ruleUpdateMruHandset,       CONN_RULES_UPDATE_MRU_PEER_HANDSET),
    /*! \} */

    /*! \{
        Rules that are run when peer synchronisation failed */
    RULE(RULE_EVENT_PEER_SYNC_FAILED,           ruleNoSyncConnectHandset,   CONN_RULES_CONNECT_HANDSET),
    /*! \} */

    /*! \{
        Rules that are run when user has request a connect */
    RULE(RULE_EVENT_USER_CONNECT,               ruleUserConnectPeer,        CONN_RULES_CONNECT_PEER),
    RULE(RULE_EVENT_USER_CONNECT,               ruleUserConnectHandset,     CONN_RULES_CONNECT_HANDSET),
    RULE(RULE_EVENT_USER_CONNECT,               ruleUserConnectPeerHandset, CONN_RULES_CONNECT_PEER_HANDSET),
    /*! \} */

    /*! \{
        Rules that are run on physical state changes */
    RULE(RULE_EVENT_OUT_CASE,                   rulePeerSync,                       CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_OUT_CASE,                   ruleOutOfCaseConnectPeer,           CONN_RULES_CONNECT_PEER),
    RULE(RULE_EVENT_OUT_CASE,                   ruleOutOfCaseConnectHandset,        CONN_RULES_CONNECT_HANDSET),
    RULE(RULE_EVENT_OUT_CASE,                   ruleOutOfCaseAllowHandsetConnect,   CONN_RULES_ALLOW_HANDSET_CONNECT),
    RULE(RULE_EVENT_OUT_CASE,                   ruleAutoHandsetPair,                CONN_RULES_HANDSET_PAIR),

    RULE(RULE_EVENT_IN_CASE,                    rulePeerSync,                       CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_IN_CASE,                    ruleInCaseDisconnectHandset,        CONN_RULES_HANDSET_DISCONNECT),
    RULE(RULE_EVENT_IN_CASE,                    ruleInCaseDisconnectPeer,           CONN_RULES_PEER_DISCONNECT),
    RULE(RULE_EVENT_IN_CASE,                    ruleInCaseEnterDfu,                 CONN_RULES_ENTER_DFU),
    RULE(RULE_EVENT_IN_CASE,                    ruleInCaseRejectHandsetConnect,     CONN_RULES_REJECT_HANDSET_CONNECT),

    RULE(RULE_EVENT_OUT_EAR,                    rulePeerSync,                       CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_OUT_EAR,                    ruleOutOfEarA2dpActive,             CONN_RULES_A2DP_TIMEOUT),
    RULE(RULE_EVENT_OUT_EAR,                    ruleOutOfEarScoActive,              CONN_RULES_SCO_TIMEOUT),
    RULE(RULE_EVENT_OUT_EAR,                    ruleOutOfEarLedsEnable,             CONN_RULES_LED_ENABLE),

    RULE(RULE_EVENT_IN_EAR,                     rulePeerSync,                       CONN_RULES_SEND_PEER_SYNC),
    RULE(RULE_EVENT_IN_EAR,                     ruleInEarLedsDisable,               CONN_RULES_LED_DISABLE),
    /*! \} */

    RULE(RULE_EVENT_DFU_CONNECT,                ruleDfuAllowHandsetConnect,         CONN_RULES_ALLOW_HANDSET_CONNECT),
};

/*! \brief Types of event that can cause connect rules to run. */
typedef enum
{
    RULE_CONNECT_USER,          /*!< User initiated connection */
    RULE_CONNECT_STARTUP,       /*!< Connect on startup */
    RULE_CONNECT_PEER_SYNC,     /*!< Peer sync complete initiated connection */
    RULE_CONNECT_OUT_OF_CASE,   /*!< Out of case initiated connection */
} ruleConnectReason;

/*****************************************************************************
 * RULES FUNCTIONS
 *****************************************************************************/

/*! @brief Rule to determine if Earbud should start automatic peer pairing
    This rule determins if automatic peer pairing should start, it is triggered
    by the startup event.
    @startuml

    start
        if (IsPairedWithPeer()) then (no)
            :Start peer pairing;
            end
        else (yes)
            :Already paired;
            stop
    @enduml 
*/
static ruleAction rulePeerPair(void)
{
    if (!appDeviceGetPeerBdAddr(NULL))
    {
        RULE_LOG("ruleStartupPeerPaired, run");
        return RULE_ACTION_RUN;
    }
    else
    {
        RULE_LOG("ruleStartupPeerPaired, done");
        return RULE_ACTION_COMPLETE;
    }
}

/*! @brief Rule to determine if Earbud should start automatic handset pairing
    @startuml

    start
        if (IsInCase()) then (yes)
            :Earbud is in case, do nothing;
            end
        endif
        if (IsPairedWithHandset()) then (yes)
            :Already paired with handset, do nothing;
            end
        endif
        if (IsPeerSyncComplete()) then (no)
            :Not sync'ed with peer, do nothing;
            end
        endif
        if (IsPeerPairing()) then (yes)
            :Peer is already pairing, do nothing;
            end
        endif
        if (IsPeerPairWithHandset()) then (yes)
            :Peer is already paired with handset, do nothing;
            end
        endif
        if (IsPeerInCase()) then (yes)
            :Start pairing, peer is in case;
            stop
        endif

        :Both Earbuds are out of case;
        if (IsPeerLeftEarbud) then (yes)
            stop
        else (no)
            end
        endif
    @enduml 
*/
static ruleAction ruleAutoHandsetPair(void)
{
    /* NOTE: Ordering of these checks is important */

    if (appSmIsInCase())
    {
        DEBUG_LOG("ruleAutoHandsetPair, ignore, we're in the case");
        return RULE_ACTION_IGNORE;
    }

    if (appDeviceGetHandsetBdAddr(NULL))
    {
        DEBUG_LOG("ruleAutoHandsetPair, complete, already paired with handset");
        return RULE_ACTION_COMPLETE;
    }

    if (!appSmIsPeerSyncComplete())
    {
        DEBUG_LOG("ruleAutoHandsetPair, ignore, not synced with peer");
        return RULE_ACTION_IGNORE;
    }

    if (appSmIsPeerPairing())
    {
        DEBUG_LOG("ruleAutoHandsetPair, ignore, peer is already in pairing mode");
        return RULE_ACTION_IGNORE;
    }

    if (appSmIsPairing())
    {
        DEBUG_LOG("ruleAutoHandsetPair, ignore, already in pairing mode");
        return RULE_ACTION_IGNORE;
    }

    if (appSmHasPeerHandsetPairing())
    {
        DEBUG_LOG("ruleAutoHandsetPair, complete, peer is already paired with handset");
        return RULE_ACTION_COMPLETE;
    }

    if (appSmIsPeerInCase())
    {
        DEBUG_LOG("ruleAutoHandsetPair, run, no paired handset, we're out of case, peer is in case");
        return RULE_ACTION_RUN;
    }
    else
    {
        /* Both out of case, neither pairing or paired.  Left wins, right loses */
        if (appConfigIsLeft())
        {
            DEBUG_LOG("ruleAutoHandsetPair, run, no paired handset, we're out of case, peer is out of case, we're left earbud");
            return RULE_ACTION_RUN;
        }
        else
        {
            DEBUG_LOG("ruleAutoHandsetPair, ignore, no paired handset, we're out of case, peer is out of case, but we're right earbud");
            return RULE_ACTION_IGNORE;
        }
    }
}

/*! @brief Rule to determine if Earbud should attempt to synchronize with peer Earbud
    @startuml

    start
        if (IsPairedWithPeer() and !IsInCaseDfu()) then (yes)
            :Start peer sync;
            stop
        else (no)
            :Already paired;
            end
    @enduml 
*/
static ruleAction rulePeerSync(void)
{
    if (appDeviceGetPeerBdAddr(NULL) && appGetState() != APP_STATE_IN_CASE_DFU)
    {
        RULE_LOGF("rulePeerSync, run (state x%x)",appGetState());
        return RULE_ACTION_RUN;
    }
    else
    {
        RULE_LOG("rulePeerSync, ignore as there's no peer - or in DFU");
        return RULE_ACTION_IGNORE;
    }
}

/*! @brief Rule to determine if Earbud should attempt to forward handset link-key to peer
    @startuml

    start
        if (IsPairedWithPeer()) then (yes)
            :Forward any link-keys to peer;
            stop
        else (no)
            :Not paired;
            end
    @enduml 
*/
static ruleAction ruleForwardLinkKeys(void)
{
    if (appDeviceGetPeerBdAddr(NULL))
    {
        RULE_LOG("ruleForwardLinkKeys, run");
        return RULE_ACTION_RUN;
    }
    else
    {
        RULE_LOG("ruleForwardLinkKeys, ignore as there's no peer");
        return RULE_ACTION_IGNORE;
    }
}

/*! @brief Sub-rule to determine if Earbud should connect to standard handset

    @startuml

    start
        if (Reason is 'User Connect') then (yes)
            :Attempt to connect as user requested it;
            stop
        else (no)
            if (Reason is 'Peer Sync' or 'Out of Case') then (yes)
                if (IsOutOfCase() and PeerIsInCase()) then (yes)
                    :Earbud is out of case, peer Earbud is in case;
                    stop
                else (no)
                    :Earbud is in case or peer Earbud is out of case;
                    end
                endif
            else
                 if (Battery Voltage > Peer Battery Voltage) then (yes)
                    :Our battery voltage is higher, connect;
                    end
                else
                    if (Battery Voltage = Peer Battery Voltage) then (yes)
                        if (IsLeftEarbud()) then (yes)
                            :Left takes preference;
                            end
                        else (no)
                            :Right doesn't connect;
                            stop
                        endif
                    else
                        :Our battery voltage is lower, don't connect;
                        stop
                    endif
                endif
            endif    
        endif
    @enduml 
*/
static ruleAction ruleConnectHandsetStandard(ruleConnectReason reason)
{
    if (reason == RULE_CONNECT_USER)
    {     
        RULE_LOG("ruleConnectHandsetStandard, run as standard handset and user requested connection");
        return RULE_ACTION_RUN;
    }
    else if ((reason == RULE_CONNECT_PEER_SYNC) || (reason == RULE_CONNECT_OUT_OF_CASE))
    {
        if (appSmIsOutOfCase() && appSmIsPeerInCase())
        {
            RULE_LOG("ruleConnectHandsetStandard, run as standard handset and not in case but peer is in case");
            return RULE_ACTION_RUN;
        }
        else
        {
            RULE_LOG("ruleConnectHandsetStandard, ignore as standard handset and in case or peer is out of case");
            return RULE_ACTION_IGNORE;
        }
    }
    else
    {
        uint16 battery_level, peer_battery_level;

        appSmGetPeerBatteryLevel(&battery_level, &peer_battery_level);
        RULE_LOGF("ruleConnectHandsetStandard, battery %u, peer battery %u", battery_level, peer_battery_level);
        if (battery_level > peer_battery_level)
        {
            RULE_LOG("ruleConnectHandsetStandard, run as standard handset and our battery is higher and we're out of case");
            return RULE_ACTION_RUN;
        }
        else if (battery_level == peer_battery_level)
        {
            if (appConfigIsLeft())
            {
                RULE_LOG("ruleConnectHandsetStandard, run as standard handset and left earbud");
                return RULE_ACTION_RUN;
            }
            else
            {
                RULE_LOG("ruleConnectHandsetStandard, ignore as standard handset and right earbud");
                return RULE_ACTION_IGNORE;
            }
        }
        else
        {
             RULE_LOG("ruleConnectHandsetStandard, ignore as standard handset and our battery is lower");
             return RULE_ACTION_IGNORE;
        }
    }
}

/*! @brief Rule to determine if Earbud should connect to Handset
    @startuml

    start
    if (IsInCase()) then (yes)
        :Never connect when in case;
        end
    endif

    if (IsPeerSyncComplete()) then (yes)
        if (Not IsPairedWithHandset()) then (yes)
            :Not paired with handset, don't connect;
            end
        endif        
        if (IsHandsetA2dpConnected() and IsHandsetAvrcpConnected() and IsHandsetHfpConnected()) then (yes)
            :Already connected;
            end
        endif

        if (IsTwsPlusHandset()) then (yes)
            :Handset is a TWS+ device;
            if (WasConnected() or Reason is 'User', 'Start-up' or 'Out of Case') then (yes)
                if (not just paired) then (yes)
                    :Connect to handset;
                    end
                else
                    :Just paired, handset will connect to us;
                    stop
                endif
            else (no)
                :Wasn't connected before;
                stop
            endif
        else (no)
            if (IsPeerConnectedA2dp() or IsPeerConnectedAvrcp() or IsPeerConnectedHfp()) then (yes)
                :Peer already has profile(s) connected, don't connect;
                stop
            else (no)
                if (WasConnected() or Reason is 'User', 'Start-up' or 'Out of Case') then (yes)
                    :run RuleConnectHandsetStandard();
                    end
                else (no)
                    :Wasn't connected before;
                    stop
                endif
            endif
        endif
    else (no)
        :Not sync'ed with peer;
        if (IsPairedWithHandset() and IsHandsetTwsPlus() and WasConnected()) then (yes)
            :Connect to handset, it is TWS+ handset;
            stop
        else (no)
            :Don't connect, not TWS+ handset;
            end
        endif
    endif

    @enduml 
*/
static ruleAction ruleConnectHandset(ruleConnectReason reason)
{
    bdaddr handset_addr;

    RULE_LOGF("ruleConnectHandset, reason %u", reason);

    /* Don't attempt to connect if we're in the case */
    if (appSmIsInCase())
    {
        RULE_LOG("ruleConnectHandset, ignore as in case");
        return RULE_ACTION_IGNORE;
    }

    /* Check we have sync'ed with peer */
    if (appSmIsPeerSyncComplete())
    {
        /* If we're not paired with handset then don't connect */
        if (!appDeviceGetHandsetBdAddr(&handset_addr))
        {
            RULE_LOG("ruleConnectHandset, ignore as not paired with handset");
            return RULE_ACTION_IGNORE;
        }

        /* If we're already connected to handset then don't connect */
        if (appDeviceIsHandsetA2dpConnected() && appDeviceIsHandsetAvrcpConnected() && appDeviceIsHandsetHfpConnected())
        {
            RULE_LOG("ruleConnectHandset, ignore as already connected to handset");
            return RULE_ACTION_IGNORE;
        }

        /* Peer is not connected to handset, so we should connect to our handset if it's a TWS+ handset or
           it's a standard handset and our battery level is higer */

        /* Check if TWS+ handset, if so just connect, otherwise compare battery levels
         * if we have higher battery level connect to handset */
        if (appDeviceIsTwsPlusHandset(&handset_addr))
        {
            /* Check if device was connected before, or we connecting due to user request */
            if (appDeviceWasConnected(&handset_addr) ||
                (reason == RULE_CONNECT_USER) ||
                (reason == RULE_CONNECT_STARTUP) ||
                (reason == RULE_CONNECT_OUT_OF_CASE))
            {
                if (!appDeviceHasJustPaired(&handset_addr))
                {
                    RULE_LOG("ruleConnectHandset, run as TWS+ handset");
                    return RULE_ACTION_RUN;
                }
                else
                {
                    RULE_LOG("ruleConnectHandset, ignore as just paired with TWS+ handset");
                    return RULE_ACTION_IGNORE;
                }
            }
            else
            {
                RULE_LOG("ruleConnectHandset, ignore as TWS+ handset but wasn't connected before");
                return RULE_ACTION_IGNORE;
            }
        }
        else
        {
            /* If peer is connected to handset then we shouldn't connect using this rule, use ruleConnectPeerHandset. */
            if (appSmIsPeerHandsetA2dpConnected() || appSmIsPeerHandsetHfpConnected())
            {
                RULE_LOG("ruleConnectHandset, ignore as peer has already connected");
                return RULE_ACTION_IGNORE;
            }

            /* Check if device was connected before, or we connecting due to user request or startup */
            if (appDeviceWasConnected(&handset_addr) ||
                (reason == RULE_CONNECT_USER) ||
                (reason == RULE_CONNECT_STARTUP) ||
                (reason == RULE_CONNECT_OUT_OF_CASE))
            {
                RULE_LOG("ruleConnectHandset, calling ruleConnectHandsetStandard()");
                return ruleConnectHandsetStandard(reason);
            }
            else
            {
                RULE_LOG("ruleConnectHandset, ignore as standard handset but wasn't connected before");
                return RULE_ACTION_IGNORE;
            }
        }
    }
    else
    {
        if (appDeviceGetHandsetBdAddr(&handset_addr) &&
            appDeviceIsTwsPlusHandset(&handset_addr) &&
            appDeviceWasConnected(&handset_addr))
        {
            RULE_LOG("ruleConnectHandset, run as TWS+ handset, as connected before, despite peer sync fail");
            return RULE_ACTION_RUN;
        }
        else
        {
            RULE_LOG("ruleConnectHandset, ignore as not sync'ed with peer");
            return RULE_ACTION_IGNORE;
        }
    }
}

/*! @brief Wrapper around ruleConnectHandset() that calls it with 'Peer sync' connect reason
*/
static ruleAction ruleSyncConnectHandset(void)
{
    return ruleConnectHandset(RULE_CONNECT_PEER_SYNC);
}

/*! @brief Wrapper around ruleConnectHandset() that calls it with 'User' connect reason
*/
static ruleAction ruleUserConnectHandset(void)
{
    return ruleConnectHandset(RULE_CONNECT_USER);
}

/*! @brief Wrapper around ruleConnectHandset() that calls it with 'Out of case' connect reason
*/
static ruleAction ruleOutOfCaseConnectHandset(void)
{
    RULE_LOG("ruleOutOfCaseConnectHandset");
    return ruleConnectHandset(RULE_CONNECT_OUT_OF_CASE);
}

/*! @brief Rule to determine if Earbud should connect to Handset even when synchronisation with peer Earbud failed
    @startuml

    start
    if (IsInCase()) then (yes)
        :Never connect when in case;
        stop
    endif

    if (Not IsPeerSyncComplete()) then (yes)
        :Not sync'ed with peer;
        if (IsPairedWithHandset() and Not IsHandsetConnected()) then (yes)
            if (IsTwsPlusHandset()) then (yes)
                :Connect to handset, it is TWS+ handset;
                stop
            else (no)
                :Don't connect, not TWS+ handset;
                end
            endif
        else (no)
            :Don't connected, not paired or already connected;
            end
        endif
    else
        :Do nothing as not sync'ed with peer Earbud;
        end
    endif
    @enduml
*/
static ruleAction ruleNoSyncConnectHandset(void)
{
    bdaddr handset_addr;

    /* Don't attempt to connect if we're in the case */
    if (appSmIsInCase())
    {
        RULE_LOG("ruleConnectHandset, ignore as nin case");
        return RULE_ACTION_IGNORE;
    }

    /* Check we haven't sync'ed with peer */
    if (!appSmIsPeerSyncComplete())
    {
        /* Not sync'ed with peer, so connect to handset if it's a TWS+ handset */

        /* Check we're paired with handset and not already connected */
        if (appDeviceGetHandsetBdAddr(&handset_addr) && !appDeviceIsHandsetConnected())
        {
            /* Check if TWS+ handset, if so just connect */
            if (appDeviceIsTwsPlusHandset(&handset_addr))
            {
                RULE_LOG("ruleConnectHandsetNoSync, run as not sync'ed but TWS+ handset");
                return RULE_ACTION_RUN;
            }
            else
            {
                RULE_LOG("ruleConnectHandsetNoSync, ignore as not sync'ed but standard handset");
                return RULE_ACTION_IGNORE;
            }
        }
        else
        {
            RULE_LOG("ruleConnectHandsetNoSync, ignore as not paired or already connected to handset");
            return RULE_ACTION_IGNORE;
        }
    }
    else
    {
        RULE_LOG("ruleConnectHandsetNoSync, ignore as sync'ed with peer");
        return RULE_ACTION_IGNORE;
    }
}

/*! @brief Rule to determine if Earbud should connect to peer's Handset
    @startuml

    start
    if (IsInCase()) then (yes)
        :Never connect when in case;
        stop
    endif

    if (Not IsPeerSyncComplete()) then (yes)
        :Not sync'ed with peer;
        end
    endif

    if (IsPeerHandsetA2dpConnected() or IsPeerHandsetAvrcpConnected() or IsPeerHandsetHfpConnected()) then (yes)
        if (IsPeerHandsetTws()) then (yes)
            if (IsPairedWithHandset())) then (yes)
                if (Not JustPaired()) then (yes)
                    if (Reason is 'User' or 'Start-up' or 'Out of case') then (yes)
                        :Connect to peer's handset;
                        stop
                    else (no)
                        :Don't connect to peer's handset;
                        end
                    endif
                else (no)
                    :Don't connect as just paired;
                    end
                endif
            else (no)
                :Not paired with peer's handset;
                end
            endif
        else (no)
            :Peer is connected to standard handset;
            end
        endif
    else (no)
        :Don't connect as peer is not connected to handset;
        end
    endif
    @enduml 
*/
static ruleAction ruleConnectPeerHandset(ruleConnectReason reason)
{
    /* Don't attempt to connect if we're in the case */
    if (appSmIsInCase())
    {
        RULE_LOG("ruleConnectHandset, ignore as nin case");
        return RULE_ACTION_IGNORE;
    }

    /* Don't run rule if we haven't sync'ed with peer */
    if (!appSmIsPeerSyncComplete())
    {
        RULE_LOG("ruleConnectPeerHandset, ignore as not sync'ed with peer");
        return RULE_ACTION_IGNORE;
    }

    /* If peer is connected to handset then we should also connect to this handset if it's TWS+ */
    if (appSmIsPeerHandsetA2dpConnected() || appSmIsPeerHandsetHfpConnected())
    {
        /*  Check peer's handset is TWS+ */
        if (appSmIsPeerHandsetTws())
        {
            bdaddr handset_addr;
            appSmGetPeerHandsetAddr(&handset_addr);

            /* Check we paired with this handset */
            if (appDeviceIsHandset(&handset_addr))
            {
#ifndef ALLOW_CONNECT_AFTER_PAIRING
                if (!appDeviceHasJustPaired(&handset_addr))
                {
#endif
                    if ((reason == RULE_CONNECT_USER) ||
                        (reason == RULE_CONNECT_STARTUP) ||
                        (reason == RULE_CONNECT_OUT_OF_CASE)
#ifdef ALLOW_CONNECT_AFTER_PAIRING
                        || (appDeviceHasJustPaired(&handset_addr))
#endif
                        )
                    {
                        RULE_LOG("ruleConnectPeerHandset, run as peer is connected to TWS+ handset");
                        return RULE_ACTION_RUN;
                    }
                    else
                    {
                        RULE_LOG("ruleConnectPeerHandset, ignore as peer is connected to TWS+ handset but not user or startup connect and not just paired");
                        return RULE_ACTION_IGNORE;
                    }
#ifndef ALLOW_CONNECT_AFTER_PAIRING
                }
                else
                {
                    RULE_LOG("ruleConnectPeerHandset, ignore as just paired with peer's TWS+ handset");
                    return RULE_ACTION_IGNORE;
                }
#endif
            }
            else
            {
                RULE_LOG("ruleConnectPeerHandset, ignore as peer is connected to TWS+ handset but we're not paired with it");
                return RULE_ACTION_IGNORE;
            }
        }
        else
        {
            RULE_LOG("ruleConnectPeerHandset, ignore as peer is connected to standard handset");
            return RULE_ACTION_IGNORE;
        }
    }
    else
    {
        /* Peer is not connected to handset, don't connect as ruleConnectHandset handles this case */
        RULE_LOG("ruleConnectPeerHandset, done as peer is not connected");
        return RULE_ACTION_COMPLETE;
    }
}

/*! @brief Wrapper around ruleSyncConnectPeerHandset() that calls it with 'Peer sync' connect reason
*/
static ruleAction ruleSyncConnectPeerHandset(void)
{
    return ruleConnectPeerHandset(RULE_CONNECT_PEER_SYNC);
}

/*! @brief Wrapper around ruleSyncConnectPeerHandset() that calls it with 'User' connect reason
*/
static ruleAction ruleUserConnectPeerHandset(void)
{
    return ruleConnectPeerHandset(RULE_CONNECT_USER);
}

/*! @brief Wrapper around ruleSyncConnectPeerHandset() that calls it with 'Out of case' connect reason
*/
static ruleAction ruleOutOfCaseConnectPeerHandset(void)
{
    RULE_LOG("ruleOutOfCaseConnectPeerHandset");
    return ruleConnectPeerHandset(RULE_CONNECT_OUT_OF_CASE);
}

/*! @brief Rule to determine if Earbud should connect A2DP & AVRCP to peer Earbud
    @startuml

    start
    if (IsPeerA2dpConnected()) then (yes)
        :Already connected;
        end
    endif

    if (IsPeerSyncComplete()) then (yes)
        if (IsPeerInCase()) then (yes)
            :Peer is in case, so don't connect to it;
            end
        endif

        if (IsPeerHandsetA2dpConnected() or IsPeerHandsetHfpConnected()) then (yes)
            if (IsPeerHandsetTws()) then (yes)
                :Don't need to connect to peer, as peer is connected to TWS+ handset;
                end
            else (no)
                :Don't need to connect, peer will connect to us;
                end
            endif
        else (no)    
            :Peer is not connected to handset yet;
            if (IsPairedWithHandset()) then (yes)
                if (Not IsTwsHandset()) then (yes)
                    if (IsHandsetA2dpConnected() or IsHandsetHfpConnected()) then (yes)
                        :Connect to peer as  connected to standard handset, peer won't be connected;
                        stop
                    else (no)
                        :Run RuleConnectHandsetStandard() to determine if we're going to connect to handset;
                        if (RuleConnectHandsetStandard()) then (yes)
                            :Will connect to handset, so should also connect to peer;
                            stop
                        else (no)
                            :Won't connect to handset, so don't connect to peer;
                            end
                        endif
                    endif
                else (no)
                    :Don't connect to peer, as connected to TWS+ handset;
                    end
                endif
            else (no)
                :Don't connect to peer, as not paired with handset;
                end
            endif
        endif
    else (no)
        :Not sync'ed with peer;
        end
    endif

    @enduml 
*/
static ruleAction ruleConnectPeer(ruleConnectReason reason)
{
    bdaddr handset_addr;

    /* Don't run rule if we're connected to peer */
    if (appDeviceIsPeerA2dpConnected())
    {
        RULE_LOG("ruleConnectPeer, ignore as already connected to peer");
        return RULE_ACTION_IGNORE;
    }

    /* Check we have sync'ed with peer */
    if (appSmIsPeerSyncComplete())
    {
        /* Check if peer is in case */
        if (appSmIsPeerInCase())
        {
            RULE_LOG("ruleConnectPeer, ignore as peer is in case");
            return RULE_ACTION_IGNORE;
        }

        /* Check if peer is connected to handset */
        if (appSmIsPeerHandsetA2dpConnected() || appSmIsPeerHandsetHfpConnected())
        {
            /* Don't connect to peer if handset is TWS+ */
            if (appSmIsPeerHandsetTws())
            {
                RULE_LOG("ruleConnectPeer, ignore as peer is connected to TWS+ handset");
                return RULE_ACTION_IGNORE;
            }
            else
            {
                RULE_LOG("ruleConnectPeer, ignore as peer is connected to standard handset and peer will connect to us");
                return RULE_ACTION_IGNORE;
            }
        }
        else
        {
            /* Peer is not connected to handset yet */
            /* Get handset address */
            if (appDeviceGetHandsetBdAddr(&handset_addr))
            {
                /* Check if the handset we would connect to is a standard handset */
                if (!appDeviceIsTwsPlusHandset(&handset_addr))
                {
                    /* Check if we're already connected to handset */
                    if (appDeviceIsHandsetA2dpConnected() || appDeviceIsHandsetHfpConnected())
                    {
                        RULE_LOG("ruleConnectPeer, run as connected to standard handset, peer won't be connected");
                        return RULE_ACTION_RUN;
                    }
                    else
                    {
                        /* Not connected to handset, if we are going to connect to standard handset, we should also connect to peer */
                        RULE_LOG("ruleConnectPeer, calling ruleConnectHandsetStandard() to determine if we're going to connect to handset");
                        if (ruleConnectHandsetStandard(reason) == RULE_ACTION_RUN)
                        {
                            RULE_LOG("ruleConnectPeer, run as connected/ing to standard handset");
                            return RULE_ACTION_RUN;
                        }
                        else
                        {
                            RULE_LOG("ruleConnectPeer, ignore as not connected/ing to standard handset");
                            return RULE_ACTION_IGNORE;
                        }
                    }
                }
                else
                {
                    RULE_LOG("ruleConnectPeer, ignore as connected/ing to TWS+ handset");
                    return RULE_ACTION_IGNORE;
                }
            }
            else
            {
                RULE_LOG("ruleConnectPeer, ignore as no handset, so no need to connect to peer");
                return RULE_ACTION_IGNORE;
            }
        }
    }
    else
    {
        /* Peer sync is not complete */
        RULE_LOG("ruleConnectPeer, ignore as peer sync not complete");
        return RULE_ACTION_IGNORE;
    }
}

/*! @brief Wrapper around ruleSyncConnectPeer() that calls it with 'Peer sync' connect reason
*/
static ruleAction ruleSyncConnectPeer(void)
{
    return ruleConnectPeer(RULE_CONNECT_PEER_SYNC);
}

/*! @brief Wrapper around ruleSyncConnectPeer() that calls it with 'User' connect reason
*/
static ruleAction ruleUserConnectPeer(void)
{
    return ruleConnectPeer(RULE_CONNECT_USER);
}

/*! @brief Wrapper around ruleSyncConnectPeer() that calls it with 'Out of case' connect reason
*/
static ruleAction ruleOutOfCaseConnectPeer(void)
{
    RULE_LOG("ruleOutOfCaseConnectPeer");
    return ruleConnectPeer(RULE_CONNECT_OUT_OF_CASE);
}

/*! @brief Rule to determine if most recently used handset should be updated
    @startuml

    start
    if (Not IsPeerSyncComplete()) then (yes)
        :Peer sync not completed;
        end 
    endif

    if (IsPeerHandsetA2dpConnected() or IsPeerHandsetHfpConnected()) then (yes)
        if (IsPairedPeerHandset()) then (yes)
            :Update MRU handzset as peer is connected to handset;
            stop
        else (no)
            :Do nothing as not paired to peer's handset;
            end
        endif
    else
        :Do nothing as peer is not connected to handset;
        end
    endif
    @enduml
*/
static ruleAction ruleUpdateMruHandset(void)
{
    /* Don't run rule if we haven't sync'ed with peer */
    if (!appSmIsPeerSyncComplete())
    {
        RULE_LOG("ruleConnectPeerHandset, ignore as not sync'ed with peer");
        return RULE_ACTION_IGNORE;
    }

    /* If peer is connected to handset then we should mark this handset as most recently used */
    if (appSmIsPeerHandsetA2dpConnected() || appSmIsPeerHandsetHfpConnected())
    {
        /* Check we paired with this handset */
        bdaddr handset_addr;
        appSmGetPeerHandsetAddr(&handset_addr);
        if (appDeviceIsHandset(&handset_addr))
        {
            RULE_LOG("ruleUpdateMruHandset, run as peer is connected to handset");
            return RULE_ACTION_RUN;
        }
        else
        {
            RULE_LOG("ruleUpdateMruHandset, ignore as not paired with peer's handset");
            return RULE_ACTION_IGNORE;
        }
    }
    else
    {
        /* Peer is not connected to handset */
        RULE_LOG("ruleUpdateMruHandset, ignore as peer is not connected");
        return RULE_ACTION_IGNORE;
    }

}

/*! @brief Rule to determine if Earbud should send status to handset over HFP and/or AVRCP
    @startuml

    start
    if (IsPairedHandset() and IsTwsPlusHandset()) then (yes)
        if (IsHandsetHfpConnected() or IsHandsetAvrcpConnected()) then (yes)
            :HFP and/or AVRCP connected, send status update;
            stop
        endif
    endif

    :Not connected with AVRCP or HFP to handset;
    end
    @enduml
*/
static ruleAction ruleSendStatusToHandset(void)
{
    bdaddr handset_addr;

    if (appDeviceGetHandsetBdAddr(&handset_addr) && appDeviceIsTwsPlusHandset(&handset_addr))
    {
        if (appDeviceIsHandsetHfpConnected() || appDeviceIsHandsetAvrcpConnected())
        {
            RULE_LOG("ruleSendStatusToHandset, run as TWS+ handset");
            return RULE_ACTION_RUN;
        }
    }

    RULE_LOG("ruleSendStatusToHandset, ignore as not connected to TWS+ handset");
    return RULE_ACTION_IGNORE;
}

/*! @brief Rule to determine if A2DP streaming when out of ear
    Rule is triggered by the 'out of ear' event
    @startuml

    start
    if (IsAvStreaming()) then (yes)
        :Run rule, as out of ear with A2DP streaming;
        stop
    endif
    end
    @enduml
*/
static ruleAction ruleOutOfEarA2dpActive(void)
{
    if (appAvIsStreaming())
    {
        RULE_LOG("ruleOutOfEarA2dpActive, run as A2DP is active and earbud out of ear");
        return RULE_ACTION_RUN;
    }

    RULE_LOG("ruleOutOfEarA2dpActive, ignore as A2DP not active out of ear");
    return RULE_ACTION_IGNORE;
}

/*! @brief Rule to determine if SCO active when out of ear
    Rule is triggered by the 'out of ear' event
    @startuml

    start
    if (IsScoActive()) then (yes)
        :Run rule, as out of ear with SCO active;
        stop
    endif
    end
    @enduml
*/
static ruleAction ruleOutOfEarScoActive(void)
{
    if (appHfpIsScoActive())
    {
        RULE_LOG("ruleOutOfEarScoActive, run as SCO is active and earbud out of ear");
        return RULE_ACTION_RUN;
    }

    RULE_LOG("ruleOutOfEarScoActive, ignore as SCO not active out of ear");
    return RULE_ACTION_IGNORE;
}

/*! @brief Rule to determine if LED should be enabled when out of ear
    Rule is triggered by the 'out of ear' event
    @startuml

    start
    if (Not IsLedsInEarEnabled()) then (yes)
        :Run rule, as out of ear and LEDs were disabled in ear;
        stop
    endif
    end
    @enduml
*/
static ruleAction ruleOutOfEarLedsEnable(void)
{
    if (!appConfigInEarLedsEnabled())
    {
        RULE_LOG("ruleOutOfEarLedsEnable, run as out of ear");
        return RULE_ACTION_RUN;
    }
    else
    {
        RULE_LOG("ruleOutOfEarLedsEnable, run as out of ear but in ear LEDs enabled");
        return RULE_ACTION_IGNORE;
    }
}

/*! @brief Rule to determine if LED should be disabled when in ear
    Rule is triggered by the 'in ear' event
    @startuml

    start
    if (Not IsLedsInEarEnabled()) then (yes)
        :Run rule, as in ear and LEDs are disabled in ear;
        stop
    endif
    end
    @enduml
*/
static ruleAction ruleInEarLedsDisable(void)
{
    if (!appConfigInEarLedsEnabled())
    {
        RULE_LOG("ruleOutOfEarLedsEnable, run as in ear");
        return RULE_ACTION_RUN;
    }
    else
    {
        RULE_LOG("ruleOutOfEarLedsEnable, run as out of ear but in ear LEDs enabled");
        return RULE_ACTION_IGNORE;
    }
}

/*! @brief Rule to determine if Earbud should disconnect from handset when put in case
    Rule is triggered by the 'in case' event
    @startuml

    start
    if (IsInCase() and IsHandsetConnected() and Not DfuUpgradePending()) then (yes)
        :Disconnect from handset as now in case;
        stop
    endif
    end
    @enduml
*/
static ruleAction ruleInCaseDisconnectHandset(void)
{
    if (   appSmIsInCase() 
        && appDeviceIsHandsetConnected() 
        && !appUpgradeDfuPending())
    {
        RULE_LOG("ruleInCaseDisconnectHandset, run as in case and handset connected");
        return RULE_ACTION_RUN;
    }
    else
    {
        RULE_LOG("ruleInCaseDisconnectHandset, ignore as not in case or handset not connected");
        return RULE_ACTION_IGNORE;
    }
}

/*! @brief Rule to determine if Earbud should disconnect A2DP/AVRCP from peer when put in case
    Rule is triggered by the 'in case' event
    @startuml

    start
    if (IsInCase() and IsPeerA2dpConnected() and IsPeerAvrcpConnectedForAv()) then (yes)
        :Disconnect from peer as now in case;
        stop
    endif
    end
    @enduml
*/
static ruleAction ruleInCaseDisconnectPeer(void)
{
    if (appSmIsInCase() && (appDeviceIsPeerA2dpConnected() || appDeviceIsPeerAvrcpConnectedForAv()))
    {
        RULE_LOG("ruleInCaseDisconnectPeer, run as in case and peer connected");
        return RULE_ACTION_RUN;
    }
    else
    {
        RULE_LOG("ruleInCaseDisconnectPeer, ignore as not in case or peer not connected");
        return RULE_ACTION_IGNORE;
    }
}

/*! @brief Rule to determine if Earbud should start DFU  when put in case
    Rule is triggered by the 'in case' event
    @startuml

    start
    if (IsInCase() and DfuUpgradePending()) then (yes)
        :DFU upgrade can start as it was pending and now in case;
        stop
    endif
    end
    @enduml
*/
static ruleAction ruleInCaseEnterDfu(void)
{
#ifdef INCLUDE_DFU
    if (appSmIsInCase() && appUpgradeDfuPending())
    {
        RULE_LOG("ruleInCaseCheckDfu, run as still in case & DFU pending/active");
        return RULE_ACTION_RUN;
    }
    else
    {
        RULE_LOG("ruleInCaseCheckDfu, ignore as not in case or no DFU pending");
        return RULE_ACTION_IGNORE;
    }
#else
    return RULE_ACTION_IGNORE;
#endif
}


static ruleAction ruleDfuAllowHandsetConnect(void)
{
#ifdef INCLUDE_DFU
    bdaddr handset_addr;

    /* If we're already connected to handset then don't connect */
    if (appDeviceIsHandsetConnected())
    {
        RULE_LOG("ruleDfuAllowHandsetConnect, ignore as already connected to handset");
        return RULE_ACTION_IGNORE;
    }

    RULE_LOG("ruleDfuAllowHandsetConnect - just run it");
    return RULE_ACTION_RUN;

#else
    return RULE_ACTION_IGNORE;
#endif
}

/*! @brief Rule to determine if Earbud should disconnect A2DP/AVRCP from peer Earbud
    @startuml

    start
    if (Not IsPeerA2dpConnected() and Not IsPeerAvrcoConnectedForAv()) then (yes)
        :Not connected, do nothing;
        stop
    endif

    if (Not IsHandsetPaired()) then (yes)
        :Not paired with handset, disconnect from peer;
        stop
    endif

    if (IsHandsetA2dpConnected()) then (yes)
        if (IsTwsPlusHandset()) then (yes)
            :Connected to TWS+ handset, no need for A2DP/AVCRP to peer;
            stop
        else
            :Connected to standard handset, still require A2DP/AVRCP to peer;
            end
        endif
    else
        :Not connected with A2DP to handset;
        end
    endif    
    @enduml
*/
static ruleAction ruleDisconnectPeer(void)
{
    bdaddr handset_addr;

    /* Don't run rule if we're not connected to peer */
    if (!appDeviceIsPeerA2dpConnected() && !appDeviceIsPeerAvrcpConnectedForAv())
    {
        RULE_LOG("ruleSyncDisconnectPeer, ignore as not connected to peer");
        return RULE_ACTION_IGNORE;
    }

    /* If we're not paired with handset then disconnect */
    if (!appDeviceGetHandsetBdAddr(&handset_addr))
    {
        RULE_LOG("ruleSyncDisconnectPeer, run and not paired with handset");
        return RULE_ACTION_IGNORE;
    }

    /* If we're connected to a handset, but it's a TWS+ handset then we don't need connction to peer */
    if (appDeviceIsHandsetA2dpConnected())
    {
        if (appDeviceIsTwsPlusHandset(&handset_addr))
        {
            RULE_LOG("ruleSyncDisconnectPeer, run as connected to TWS+ handset");
            return RULE_ACTION_RUN;
        }
        else
        {
            RULE_LOG("ruleSyncDisconnectPeer, ignore as connected to standard handset");
            return RULE_ACTION_IGNORE;
        }
    }
    else
    {
        RULE_LOG("ruleSyncDisconnectPeer, run as not connected handset");
        return RULE_ACTION_RUN;
    }
}

static ruleAction ruleOutOfCaseAllowHandsetConnect(void)
{
    RULE_LOG("ruleOutOfCaseAllowHandsetConnect, run as out of case");
    return RULE_ACTION_RUN;
}

static ruleAction ruleInCaseRejectHandsetConnect(void)
{
#ifdef INCLUDE_DFU
    if (appUpgradeDfuPending())
    {
        RULE_LOG("ruleInCaseRejectHandsetConnect, ignored as DFU pending");
        return RULE_ACTION_IGNORE;
    }
#endif

    RULE_LOG("ruleInCaseRejectHandsetConnect, run as in case and no DFU");
    return RULE_ACTION_RUN;
}

/*****************************************************************************
 * END RULES FUNCTIONS
 *****************************************************************************/

static void appConRulesSetRuleStatus(MessageId message, ruleStatus status, ruleStatus new_status, connRulesEvents event)
{
    int rule_index;
    int num_rules = sizeof(appConnRules) / sizeof(ruleEntry);
    connRulesEvents event_mask = 0;

    for (rule_index = 0; rule_index < num_rules; rule_index++)
    {
        ruleEntry *rule = &appConnRules[rule_index];
        if ((rule->message == message) && (rule->status == status) && (rule->events & event))
        {
            CONNRULES_LOGF("appConnRulesSetStatus, rule %d, status %d", rule_index, new_status);
            rule->status = new_status;

            /* Build up set of events where rules are complete */
            event_mask |= rule->events;
        }
    }

    /* Check if all rules for an event are now complete, if so clear event */
    for (rule_index = 0; rule_index < num_rules; rule_index++)
    {
        ruleEntry *rule = &appConnRules[rule_index];
        if (rule->events & event)
        {
            /* Clear event if this rule is not complete */
            if (rule->status != RULE_STATUS_COMPLETE)
                event_mask &= ~rule->events;
        }
    }

    if (event_mask)
    {
        CONNRULES_LOGF("appConnRulesSetStatus, event %x complete", event_mask);
        appConnRulesResetEvent(event_mask);
    }
}

static void appConnRulesCheck(void)
{
    connRulesTaskData *conn_rules = appGetConnRules();
    int rule_index;
    int num_rules = sizeof(appConnRules) / sizeof(ruleEntry);
    connRulesEvents events = conn_rules->events;

    CONNRULES_LOGF("appConnRulesCheck, starting events %x", events);

    for (rule_index = 0; rule_index < num_rules; rule_index++)
    {
        ruleEntry *rule = &appConnRules[rule_index];
        ruleAction action;

        /* On check rules that match event */
        if ((rule->events & events) == rule->events)
        {
            /* Skip rules that are now complete */
            if (rule->status == RULE_STATUS_COMPLETE)
                continue;

            /* Stop checking rules for this event if rule is in progress */
            if (rule->status == RULE_STATUS_IN_PROGRESS)
            {
                events &= ~rule->events;
                CONNRULES_LOGF("appConnRulesCheck, in progress, filtered events %x", events);
                continue;
            }

            /* Call the rule */
            CONNRULES_LOGF("appConnRulesCheck, running rule %d, status %d, events %x", rule_index, rule->status, events);
            action = rule->rule();
            if (action == RULE_ACTION_RUN)
            {
                TaskList *rule_tasks = appTaskListInit();

                CONNRULES_LOG("appConnRulesCheck, rule in progress");
                rule->status = RULE_STATUS_IN_PROGRESS;

                /*  Build list of tasks to send message to */
                for (int event = 0; event < ARRAY_DIM(conn_rules->event_tasks); event++)
                {
                    const connRulesEvents event_mask = 1UL << event;
                    Task task = conn_rules->event_tasks[event];

                    if ((rule->events & event_mask) && task)
                        appTaskListAddTask(rule_tasks, task);
                }

                /* Send rule message to tasks in list */
                appTaskListMessageSendId(rule_tasks, rule->message);
                appTaskListDestroy(rule_tasks);

                /* Stop checking rules for this event */
                events &= ~rule->events;
                continue;
            }
            else if (action == RULE_ACTION_COMPLETE)
            {
                CONNRULES_LOG("appConnRulesCheck, rule complete");
                appConRulesSetRuleStatus(rule->message, rule->status, RULE_STATUS_COMPLETE, rule->events);
            }
            else if (action == RULE_ACTION_IGNORE)
            {
                CONNRULES_LOG("appConnRulesCheck, rule ignored");
                appConRulesSetRuleStatus(rule->message, rule->status, RULE_STATUS_COMPLETE, rule->events);
            }
        }
    }
}



/*! \brief Initialise the connection rules module. */
void appConnRulesInit(void)
{
    connRulesTaskData *conn_rules = appGetConnRules();
    conn_rules->events = 0;
}

void appConnRulesSetEvent(Task client_task, connRulesEvents event_mask)
{
    connRulesTaskData *conn_rules = appGetConnRules();

    conn_rules->events |= event_mask;
    CONNRULES_LOGF("appConnRulesSetEvent, event %x, new events %x", event_mask, conn_rules->events);

    for (int event = 0; event < ARRAY_DIM(conn_rules->event_tasks); event++)
    {
        if (event_mask & (1UL << event))
            conn_rules->event_tasks[event] = client_task;
    }

    appConnRulesCheck();
}

void appConnRulesResetEvent(connRulesEvents event)
{
    int rule_index;
    int num_rules = sizeof(appConnRules) / sizeof(ruleEntry);

    connRulesTaskData *conn_rules = appGetConnRules();
    conn_rules->events &= ~event;
    //CONNRULES_LOGF("appConnRulesResetEvent, event %x, new events %x", event, conn_rules->events);

    /* Walk through matching rules resetting the status */
    for (rule_index = 0; rule_index < num_rules; rule_index++)
    {
        ruleEntry *rule = &appConnRules[rule_index];

        if (rule->events & event)
        {
            //CONNRULES_LOGF("appConnRulesResetEvent, resetting rule %d", rule_index);
            rule->status = RULE_STATUS_NOT_DONE;
        }
    }
}

extern connRulesEvents appConnRulesGetEvents(void)
{
    connRulesTaskData *conn_rules = appGetConnRules();
    return conn_rules->events;
}

void appConnRulesSetRuleComplete(MessageId message)
{
    appConRulesSetRuleStatus(message, RULE_STATUS_IN_PROGRESS, RULE_STATUS_COMPLETE, 0xFFFFFFFFUL);
    appConnRulesCheck();
}

void appConnRulesSetRuleWithEventComplete(MessageId message, connRulesEvents event)
{
    appConRulesSetRuleStatus(message, RULE_STATUS_IN_PROGRESS, RULE_STATUS_COMPLETE, event);
    appConnRulesCheck();
}


