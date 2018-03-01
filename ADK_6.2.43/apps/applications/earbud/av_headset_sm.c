/*!
\copyright  Copyright (c) 2005 - 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_sm.c
\brief	    Application state machine
*/

#include "av_headset.h"
#include "av_headset_sm.h"
#include "av_headset_ui.h"
#include "av_headset_gatt.h"
#include "av_headset_scan_manager.h"
#include "av_headset_con_manager.h"
#include "av_headset_log.h"
#include "av_headset_conn_rules.h"
#include "av_headset_config.h"
#include "av_headset_handset_signalling.h"

#include <panic.h>
#include <connection.h>
#include <ps.h>
#include <boot.h>

/*! \brief Application state machine internal message IDs */
enum sm_internal_message_ids
{
    SM_INTERNAL_PAIR_HANDSET,               /*!< Start pairing with a handset. */
    SM_INTERNAL_DELETE_HANDSETS,            /*!< Delete all paired handsets. */
    SM_INTERNAL_FACTORY_RESET,              /*!< Reset device to factory defaults. */
    SM_INTERNAL_POWER_OFF_DISCONNECT,       /*!< Power off in progress. Disconnect any links. */
    SM_INTERNAL_ENTER_DFU_UI,               /*!< Button for DFU has been pressed */
    SM_INTERNAL_ENTER_DFU_GAIA,             /*!< We have been connected to over GAIA */
    SM_INTERNAL_ENTER_DFU_UPGRADED,         /*!< We have restarted after an upgrade requested reboot */
    SM_INTERNAL_ENTER_DFU_STARTUP,          /*!< We have restarted while a DFU was in progress */
    SM_INTERNAL_DFU_ENDED,                  /*!< DFU / GAIA has ended */
    SM_INTERNAL_START_UPGRADE,              /*!< The upgrade library has commenced an upgrade */
    SM_INTERNAL_NO_DFU,                     /*!< DFU/Upgrade is not supported, move to startup */

    SM_INTERNAL_TIMEOUT_DFU_ENTRY = 0x80,   /*!< Timeout occurred because no DFU activity after entering GAIA */
    SM_INTERNAL_TIMEOUT_DFU_ACTIVITY,       /*!< Timeout for no DFU activity during an update */
    SM_INTERNAL_TIMEOUT_FACTORY_RESET,      /*!< Timer to force reset if link disconnection fails. */
    SM_INTERNAL_TIMEOUT_OUT_OF_EAR_A2DP,    /*!< Timeout to pause A2DP when earbud removed from ear while streaming . */
    SM_INTERNAL_TIMEOUT_OUT_OF_EAR_SCO,     /*!< Timeout to transfer SCO to AG when earbud removed from ear while call active. */
    SM_INTERNAL_TIMEOUT_IN_EAR_A2DP_START,  /*!< Timeout within which restart audio if earbud put back in ear. */
};

static void appSmHandleInternalDeleteHandsets(void);

/*****************************************************************************
 * SM utility functions
 *****************************************************************************/
static void appSmCancelDfuTimers(void)
{
    MessageCancelAll(appGetSmTask(),SM_INTERNAL_TIMEOUT_DFU_ENTRY);
    MessageCancelAll(appGetSmTask(),SM_INTERNAL_TIMEOUT_DFU_ACTIVITY);
}

/*! \brief Delete all peer and handset pairing and reboot device. */
static void appSmDeletePairingAndReset(void)
{
    bdaddr bd_addr;

    DEBUG_LOG("appSmDeletePairingAndReset");

    /* cancel the factory reset timer, may already be gone if it fired to get us here */
    MessageCancelFirst(appGetSmTask(), SM_INTERNAL_TIMEOUT_FACTORY_RESET);

    /* delete all paired handsets */
    appSmHandleInternalDeleteHandsets();

    /* delete paired peer earbud */
    if (appDeviceGetPeerBdAddr(&bd_addr))
        appDeviceDelete(&bd_addr);

    appPowerReboot();
}

/*! \brief Force disconnection of any links with peer or a handset.
    \return bool TRUE all links are disconnected
                 FALSE caller needs to wait for link disconnection to complete
 * */
static bool appSmDisconnectAllLinks(void)
{
    bool all_links_disconnected = TRUE;

    if (appDeviceIsPeerConnected())
    {
        DEBUG_LOG("appSmDisconnectAllLinks PEER IS CONNECTED");
        appPeerSigForceDisconnect();
        appAvDisconnectPeer();
        all_links_disconnected = FALSE;
    }
    if (appDeviceIsHandsetHfpConnected())
    {
        DEBUG_LOG("appSmDisconnectAllLinks HANDSET HFP IS CONNECTED");
        appHfpDisconnectInternal();
        all_links_disconnected = FALSE;
    }
    if (appDeviceIsHandsetA2dpConnected() || appDeviceIsHandsetAvrcpConnected())
    {
        DEBUG_LOG("appSmDisconnectAllLinks HANDSET AV IS CONNECTED");
        appAvDisconnectHandset();
        all_links_disconnected = FALSE;
    }

    return all_links_disconnected;
}

/*! \brief Update page scan according to the connections that exist between
           devices.
    @startuml
        (handset1)
        (handset2)
        (earbud1)
        (earbud2)
        earbud1 <-> earbud2 : A
        earbud1 <--> handset1 : B
        earbud2 <--> handset2 : C
    @enduml
    A = link between earbuds
    B = link from earbud1 to handset1
    C = link from earbud2 to handset2
    Links B and C are mutually exclusive.

    Page scan is controlled as defined in the following truth table (X=Don't care).

    A | B | C | Page Scan On
    - | - | - | ------------
    0 | X | X | 1
    1 | 0 | 0 | 1
    1 | 0 | 1 | 0
    1 | 1 | X | 0
*/
static void appSmPageScanUpdate(void)
{
    bool connected_peer = appDeviceIsPeerConnected();
    bool connected_handset = appDeviceIsHandsetConnected();
    bool peer_connected_handset = appSmIsPeerHandsetA2dpConnected() ||
                                  appSmIsPeerHandsetAvrcpConnected() ||
                                  appSmIsPeerHandsetHfpConnected();
    /* Logic derived from the truth table above */
    bool ps_on = !connected_peer || (!connected_handset && !peer_connected_handset);
    if (ps_on)
    {
        appScanManagerEnablePageScan(SCAN_MAN_USER_SM, SCAN_MAN_PARAMS_TYPE_SLOW);
    }
    else
    {
        appScanManagerDisablePageScan(SCAN_MAN_USER_SM);
    }
    DEBUG_LOGF("appSmPageScanUpdate, Peer=%u Handset=%u PeerHandset=%u, PS=%u",
               connected_peer, connected_handset, peer_connected_handset, ps_on);
}

/*! \brief Determine which idle state to transition to based on physical state.
    \return appState One of the IDLE states that correspond to a physical earbud state.
*/
static appState appSmCalcCoreState(void)
{
    switch (appSmGetPhyState())
    {
        case PHY_STATE_IN_CASE:
            return APP_STATE_IN_CASE_IDLE;

        case PHY_STATE_OUT_OF_EAR:
            if (appAvIsStreaming() || appHfpIsScoActive())
                return APP_STATE_OUT_OF_CASE_BUSY;
            else
                return APP_STATE_OUT_OF_CASE_IDLE;

        case PHY_STATE_OUT_OF_EAR_AT_REST:
            if (appAvIsStreaming() || appHfpIsScoActive())
                return APP_STATE_OUT_OF_CASE_BUSY;
            else
                return APP_STATE_OUT_OF_CASE_IDLE;
#if 0
            else
                /*! \todo need to control going into soporific after timeout not immediately... */
                return APP_STATE_OUT_OF_CASE_SOPORIFIC;
#endif

        case PHY_STATE_IN_EAR:
            if (appAvIsStreaming() || appHfpIsScoActive())
                return APP_STATE_IN_EAR_BUSY;
            else
                return APP_STATE_IN_EAR_IDLE;

        /* Physical state is unknown, default to in-case state */
        case PHY_STATE_UNKNOWN:
            return APP_STATE_IN_CASE_IDLE;

        default:
            Panic();
    }

    return APP_STATE_IN_EAR_IDLE;
}

static void appSmSetCoreState(void)
{
    appState state = appSmCalcCoreState();
    if (state != appGetState())
        appSetState(state);

    appSmPageScanUpdate();
}


static void appSmSetInitialCoreState(void)
{
    smTaskData *sm = appGetSm();

    /* Register with physical state manager to get notification of state
     * changes such as 'in-case', 'in-ear' etc */
    appPhyStateRegisterClient(&sm->task);

    /* Get latest physical state */
    sm->phy_state = appPhyStateGetState();

    /* Generate physical state events */
    switch (sm->phy_state)
    {
        case PHY_STATE_IN_CASE:
            appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_IN_CASE);
            break;

        case PHY_STATE_OUT_OF_EAR_AT_REST:
        case PHY_STATE_OUT_OF_EAR:
            appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_OUT_CASE);
            appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_OUT_EAR);
            break;

        case PHY_STATE_IN_EAR:
            appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_OUT_CASE);
            appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_IN_EAR);
            break;

        default:
            Panic();
    }

    /* Update core state */
    appSmSetCoreState();
}

/*****************************************************************************
 * SM state transition handler functions
 *****************************************************************************/
/*! \brief Enter initialising state

    This function is called whenever the state changes to
    APP_STATE_INITIALISING.
    It is reponsible for initialising global aspects of the application,
    i.e. non application module related state.
*/
static void appEnterInitialising(void)
{
    DEBUG_LOG("appEnterInitialising");
}

/*! \brief Exit initialising state.
 */
static void appExitInitialising(void)
{
    DEBUG_LOG("appExitInitialising");
}

/*! \brief Enter
 */
static void appEnterDfuCheck(void)
{
    DEBUG_LOG("appEnterDfuCheck");

    /* We only get into DFU check, if there wasn't a pending DFU
       So send a message to go to startup. */
    MessageSend(appGetSmTask(), SM_INTERNAL_NO_DFU, NULL);
}

/*! \brief Exit
 */
static void appExitDfuCheck(void)
{
    DEBUG_LOG("appExitDfuCheck");
}

/*! \brief Enter actions when we enter the factory reset state.
    The application will drop all links, delete all pairing and reboot.
 */
static void appEnterFactoryReset(void)
{
    /* if all links are now disconnected, continue immediately to
     * delete pairing and reset.
     * otherwise wait for disconnections to complete first. */
    if (appSmDisconnectAllLinks())
    {
        appSmDeletePairingAndReset();
    }
    else
    {
        /* waiting for link disconnect completion,
         * start a timer to force reset if we fail to complete for some reason */
        MessageSendLater(appGetSmTask(), SM_INTERNAL_TIMEOUT_FACTORY_RESET,
                         NULL, appConfigFactoryResetTimeoutMs());
    }
}

/*! \brief Exit factory reset. */
static void appExitFactoryReset(void)
{
}

/*! \brief Actions to take on entering startup state
 */
static void appEnterStartup(void)
{
    DEBUG_LOG("appEnterStartup");

    /* Initial kick to the connection rules engine */
    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_STARTUP);

    /* Enable fast page scan, to allow peer to connect to us */
    appScanManagerEnablePageScan(SCAN_MAN_USER_SM, SCAN_MAN_PARAMS_TYPE_FAST);
}

static void appExitStartup(void)
{
    DEBUG_LOG("appExitStartup");

    /* Clear event now we're exiting startup state */
    appConnRulesResetEvent(RULE_EVENT_STARTUP);

    /* Disable scanning for now, may get re-enabled on new state entry function */
    appScanManagerDisablePageScan(SCAN_MAN_USER_SM);
}

/*! \brief Start process to pair with peer earbud.
 */
static void appEnterPeerPairing(void)
{
    DEBUG_LOG("appEnterPeerPairing");

    appPairingPeerPair(appGetSmTask(), FALSE);
}

/*! \brief Exit actions when peer pairing completed.
 */
static void appExitPeerPairing(void)
{
    DEBUG_LOG("appExitPeerPairing");

    /* No actions currently required. */
    appPairingPeerPairCancel();
}

/*! \brief Start process to pairing with handset.
 */
static void appEnterHandsetPairing(void)
{
    bool wait_for_disconnects = FALSE;
    DEBUG_LOG("appEnterHandsetPairing");

    /* drop any existing handset connections before starting handset pairing */
    if (appDeviceIsHandsetHfpConnected())
    {
        DEBUG_LOG("appEnterHandsetPairing, disconnecting HFP");
        appHfpDisconnectInternal();
        wait_for_disconnects = TRUE;
    }
    if (appDeviceIsHandsetA2dpConnected() || appDeviceIsHandsetAvrcpConnected())
    {
        DEBUG_LOG("appEnterHandsetPairing, disconnecting AV");
        appAvDisconnectHandset();
        wait_for_disconnects = TRUE;
    }

    /* if nothing to disconnect start handset pairing immediately */
    if (!wait_for_disconnects)
    {
        appPairingHandsetPair(appGetSmTask(), TRUE);
    }
}

/*! \brief Exit actions when handset pairing completed.
 */
static void appExitHandsetPairing(void)
{
    DEBUG_LOG("appExitHandsetPairing");

    appConnRulesSetRuleComplete(CONN_RULES_HANDSET_PAIR);
    appPairingHandsetPairCancel();
}

/*! \brief Enter
 */
static void appEnterInCaseIdle(void)
{
    DEBUG_LOG("appEnterInCaseIdle");
}

/*! \brief Exit
 */
static void appExitInCaseIdle(void)
{
    DEBUG_LOG("appExitInCaseIdle");
}

/*! \brief Enter
 */
static void appEnterInCaseDfu(void)
{
    DEBUG_LOG("appEnterInCaseDfu");

    appUpgradeEnteredDfuMode();
}

/*! \brief Exit
 */
static void appExitInCaseDfu(void)
{
    DEBUG_LOG("appExitInCaseDfu");
    appSmCancelDfuTimers();
}

/*! \brief Enter
 */
static void appEnterOutOfCaseIdle(void)
{
    DEBUG_LOG("appEnterOutOfCaseIdle");

    /* Show idle on LEDs */
    appUiIdleActive();

    /* \todo think about this one, has the potential to break stuff easily */
    /* Turn slow page scan on to allow connections to us */
    appScanManagerEnablePageScan(SCAN_MAN_USER_SM, SCAN_MAN_PARAMS_TYPE_SLOW);

    /* \todo start dormant timer here or handled in power control somewhere... */
}

/*! \brief Exit
 */
static void appExitOutOfCaseIdle(void)
{
    DEBUG_LOG("appExitOutOfCaseIdle");

    /* Stop idle on LEDs */
    appUiIdleInactive();

    /* \todo think about this one, has the potential to break stuff easily */
    /* Disable scanning for now, may get re-enabled on new state entry function */
    appScanManagerDisablePageScan(SCAN_MAN_USER_SM);
}

/*! \brief Enter
 */
static void appEnterOutOfCaseBusy(void)
{
    DEBUG_LOG("appEnterOutOfCaseBusy");
}

/*! \brief Cancel the audio pause timer.

    May have already fired, which is causing us to exit this state, but
    safe to do so.
 */
static void appExitOutOfCaseBusy(void)
{
    DEBUG_LOG("appExitOutOfCaseBusy");

}

/*! \brief Enter actions when the soporific state is entered.
 */
static void appEnterOutOfCaseSoporific(void)
{
    DEBUG_LOG("appEnterOutOfCaseSoporific");

#ifdef INCLUDE_POWER_CONTROL
    {
        bool disconnecting_links = appSmDisconnectAllLinks();

        /* Tell the power module to power off, letting it know
           whether there are links in the process of being
           disconnected. */
        appPowerEnterSoporific(disconnecting_links);
    }
#endif
}

/*! \brief Exit actions when the soporific state exited
 */
static void appExitOutOfCaseSoporific(void)
{
    DEBUG_LOG("appExitOutOfCaseSoporific");

    appPowerCancelInProgressDormant();
}

/*! \brief Enter actions when we enter the asleep state.
 * The application is expected to be shutting down imminently.
 */
static void appEnterOutOfCaseSleeping(void)
{
    DEBUG_LOG("appEnterOutOfCaseSleeping");
     /* Delegate actions to the power control module */
     appPowerEnterAsleep();
}

/*! \brief Exit actions when the asleep state ends (unexpected)
 */
static void appExitOutOfCaseSleeping(void)
{
    DEBUG_LOG("appExitOutOfCaseSleeping");
    appChargerRestoreState();
}

/*! \brief Enter
 */
static void appEnterInEarIdle(void)
{
    DEBUG_LOG("appEnterInEarIdle");
}

/*! \brief Exit
 */
static void appExitInEarIdle(void)
{
    DEBUG_LOG("appExitInEarIdle");
}

/*! \brief Enter
 */
static void appEnterInEarBusy(void)
{
    DEBUG_LOG("appEnterInEarBusy");
}

/*! \brief Exit
 */
static void appExitInEarBusy(void)
{
    DEBUG_LOG("appExitInEarBusy");
}

/*! \brief Exit APP_STATE_IN_CASE parent function actions. */
static void appExitInCase(void)
{
    DEBUG_LOG("appExitInCase");

    /* run rules for being taken out of the case */
    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_OUT_CASE);
}

/*! \brief Exit APP_STATE_OUT_OF_CASE parent function actions. */
static void appExitOutOfCase(void)
{
    DEBUG_LOG("appExitOutOfCase");

    MessageCancelFirst(appGetSmTask(), SM_INTERNAL_TIMEOUT_OUT_OF_EAR_A2DP);
    MessageCancelFirst(appGetSmTask(), SM_INTERNAL_TIMEOUT_OUT_OF_EAR_SCO);
}

/*! \brief Exit APP_STATE_IN_EAR parent function actions. */
static void appExitInEar(void)
{
    DEBUG_LOG("appExitInEar");

    /* Run rules for out of ear event */
    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_OUT_EAR);
}

/*! \brief Enter APP_STATE_IN_CASE parent function actions. */
static void appEnterInCase(void)
{
    DEBUG_LOG("appEnterInCase");

    /* request handset signalling module send current state to handset. */
    appHandsetSigSendEarbudStateReq(PHY_STATE_IN_CASE);

    /* Run rules for in case event */
    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_IN_CASE);
}

/*! \brief Enter APP_STATE_OUT_OF_CASE parent function actions. */
static void appEnterOutOfCase(void)
{
    DEBUG_LOG("appEnterOutOfCase");

    /* request handset signalling module send current state to handset. */
    appHandsetSigSendEarbudStateReq(PHY_STATE_OUT_OF_EAR);
}

/*! \brief Enter APP_STATE_IN_EAR parent function actions. */
static void appEnterInEar(void)
{
    DEBUG_LOG("appEnterInEar");

    /* request handset signalling module send current state to handset. */
    appHandsetSigSendEarbudStateReq(PHY_STATE_IN_EAR);

    /* Run rules for in ear event */
    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_IN_EAR);

    /* try and cancel the A2DP auto restart timer, if there was one
     * then we're inside the time required to automatically restart
     * audio due to earbud being put back in the ear, so we need to
     * send a play request to the handset */
    if (MessageCancelFirst(appGetSmTask(), SM_INTERNAL_TIMEOUT_IN_EAR_A2DP_START))
    {
        appAvPlay(FALSE);
    }

    if (appHfpIsScoActive())
        appHfpTransferToHeadset();
    else
    {
        if (appHfpIsCallIncoming())
            appHfpCallAccept();
    }

}

/*****************************************************************************
 * End of SM state transition handler functions
 *****************************************************************************/

/* This function is called to change the applications state, it automatically
   calls the entry and exit functions for the new and old states.
*/
void appSetState(appState new_state)
{
    appState previous_state = appGetSm()->state;

    DEBUG_LOGF("appSetState, state 0x%02x to 0x%02x", previous_state, new_state);

    /* Handle state exit functions */
    switch (previous_state)
    {
        case APP_STATE_NULL:
            /* This can occur when DFU is entered during INIT. */
            break;

        case APP_STATE_INITIALISING:
            appExitInitialising();
            break;

        case APP_STATE_DFU_CHECK:
            appExitDfuCheck();
            break;

        case APP_STATE_FACTORY_RESET:
            appExitFactoryReset();
            break;

        case APP_STATE_STARTUP:
            appExitStartup();
            break;

        case APP_STATE_PEER_PAIRING:
            appExitPeerPairing();
            break;

        case APP_STATE_HANDSET_PAIRING:
            appExitHandsetPairing();
            break;

        case APP_STATE_IN_CASE_IDLE:
            appExitInCaseIdle();
            break;

        case APP_STATE_IN_CASE_DFU:
            appExitInCaseDfu();
            break;

        case APP_STATE_OUT_OF_CASE_IDLE:
            appExitOutOfCaseIdle();
            break;

        case APP_STATE_OUT_OF_CASE_BUSY:
            appExitOutOfCaseBusy();
            break;

        case APP_STATE_OUT_OF_CASE_SOPORIFIC:
            appExitOutOfCaseSoporific();
            break;

        case APP_STATE_OUT_OF_CASE_SLEEPING:
            appExitOutOfCaseSleeping();
            break;

        case APP_STATE_IN_EAR_IDLE:
            appExitInEarIdle();
            break;

        case APP_STATE_IN_EAR_BUSY:
            appExitInEarBusy();
            break;

        default:
            DEBUG_LOGF("Attempted to exit unsupported state %d", appGetSm()->state);
            Panic();
            break;
    }

    /* if exiting the APP_STATE_IN_CASE parent state */
    if ((previous_state & APP_STATE_IN_CASE) && !(new_state & APP_STATE_IN_CASE))
        appExitInCase();

    /* if exiting the APP_STATE_OUT_OF_CASE parent state */
    if ((previous_state & APP_STATE_OUT_OF_CASE) && !(new_state & APP_STATE_OUT_OF_CASE))
        appExitOutOfCase();

    /* if exiting the APP_STATE_IN_EAR parent state */
    if ((previous_state & APP_STATE_IN_EAR) && !(new_state & APP_STATE_IN_EAR))
        appExitInEar();

    /* Set new state */
    appGetSm()->state = new_state;

    /* if entering the APP_STATE_IN_CASE parent state */
    if (!(previous_state & APP_STATE_IN_CASE) && (new_state & APP_STATE_IN_CASE))
        appEnterInCase();

    /* if entering the APP_STATE_OUT_OF_CASE parent state */
    if (!(previous_state & APP_STATE_OUT_OF_CASE) && (new_state & APP_STATE_OUT_OF_CASE))
        appEnterOutOfCase();

    /* if entering the APP_STATE_IN_EAR parent state */
    if (!(previous_state & APP_STATE_IN_EAR) && (new_state & APP_STATE_IN_EAR))
        appEnterInEar();

    /* Handle state entry functions */
    switch (new_state)
    {
        case APP_STATE_INITIALISING:
            appEnterInitialising();
            break;

        case APP_STATE_DFU_CHECK:
            appEnterDfuCheck();
            break;

        case APP_STATE_FACTORY_RESET:
            appEnterFactoryReset();
            break;

        case APP_STATE_STARTUP:
            appEnterStartup();
            break;

        case APP_STATE_PEER_PAIRING:
            appEnterPeerPairing();
            break;

        case APP_STATE_HANDSET_PAIRING:
            appEnterHandsetPairing();
            break;

        case APP_STATE_IN_CASE_IDLE:
            appEnterInCaseIdle();
            break;

        case APP_STATE_IN_CASE_DFU:
            appEnterInCaseDfu();
            break;

        case APP_STATE_OUT_OF_CASE_IDLE:
            appEnterOutOfCaseIdle();
            break;

        case APP_STATE_OUT_OF_CASE_BUSY:
            appEnterOutOfCaseBusy();
            break;

        case APP_STATE_OUT_OF_CASE_SOPORIFIC:
            appEnterOutOfCaseSoporific();
            break;

        case APP_STATE_OUT_OF_CASE_SLEEPING:
            appEnterOutOfCaseSleeping();
            break;

        case APP_STATE_IN_EAR_IDLE:
            appEnterInEarIdle();
            break;

        case APP_STATE_IN_EAR_BUSY:
            appEnterInEarBusy();
            break;

        default:
            DEBUG_LOGF("Attempted to enter unsupported state %d", new_state);
            Panic();
            break;
    }

    /* Changing state is a good clue that we have done some work,
     * so reset the power off timers.
     * This also ensures that we start a power-off timer on boot.
     */
    if (   (new_state != previous_state)
        && !appSmIsSleepy())
    {
        appPowerOffTimerRestart();
    }
}

appState appGetState(void)
{
    return appGetSm()->state;
}


/*! \brief Handle notification of (dis)connections. */
static void appSmHandleConManagerConnectionInd(CON_MANAGER_CONNECTION_IND_T* ind)
{
    DEBUG_LOGF("appSmHandleConManagerConnectionInd connected:%d", ind->connected);

    switch (appGetState())
    {
        case APP_STATE_FACTORY_RESET:
            /* all links disconnected and profiles? */
            if (!appConManagerAnyLinkConnected() && appHfpIsDisconnected() && appAvIsDisconnected())
            {
                appSmDeletePairingAndReset();
            }
            break;

#ifdef INCLUDE_POWER_CONTROL
        case APP_STATE_OUT_OF_CASE_SOPORIFIC:
            /* Let power module know link state has changed, in case affects powerdown */
            appPowerLinkStateUpdated();
            break;
#endif

        default:
            /* Check if we've disconnected to peer */
            if (!ind->connected && appDeviceIsPeer(&ind->bd_addr))
            {
                smTaskData *sm = appGetSm();

                DEBUG_LOG("appSmHandleConManagerConnectionInd, disconnected from peer");

                /* Peer sync information is now out of date */
                sm->peer_sync_state &= ~(SM_PEER_SYNC_SENT | SM_PEER_SYNC_RECEIVED);
            }
            break;
    }
}

/*! \brief Handle completion of application module initialisation. */
static void appSmHandleInitConfirm(void)
{
    DEBUG_LOG("appSmHandleInitConfirm");
    switch (appGetState())
    {
        case APP_STATE_INITIALISING:
#ifdef INCLUDE_DFU
            appSetState(APP_STATE_DFU_CHECK);
#endif
            appPowerOn();
            break;

        case APP_STATE_IN_CASE_DFU:
            appPowerOn();
            break;

        default:
            Panic();
    }
}

/*! \brief Handle completion of peer pairing. */
static void appSmHandlePairingPeerPairConfirm(PAIRING_PEER_PAIR_CFM_T *cfm)
{
    DEBUG_LOGF("appSmHandlePairingPeerPairConfirm, status %d", cfm->status);

    switch (appGetState())
    {
        case APP_STATE_PEER_PAIRING:
        {
            /* only mark as complete if succeeds as we want the rules to
             * still consider this task outstanding and request a retry */
            if (cfm->status == pairingSuccess)
            {
                appConnRulesSetRuleComplete(CONN_RULES_PEER_PAIR);
            }
            /* peer pairing operation completed, always moved
             * back to STARTUP, if not successful the rules
             * will try again */
            appSetState(APP_STATE_STARTUP);
        }
        break;

#if 0
            /*! \todo handling in here if we cancelled peer
             * pairing as part of attempt to factory reset */
        case APP_STATE_FACTORY_RESET:
            break;
#endif

        default:
            Panic();
    }
}

/*! \brief Handle completion of handset pairing. */
static void appSmHandlePairingHandsetPairConfirm(PAIRING_HANDSET_PAIR_CFM_T *cfm)
{
    UNUSED(cfm);
    DEBUG_LOGF("appSmHandlePairingHandsetPairConfirm, status %d", cfm->status);

    switch (appGetState())
    {
        case APP_STATE_HANDSET_PAIRING:
            appSmSetCoreState();
            break;

#if 0
            /*! \todo handling in here if we cancelled handset
             * pairing as part of attempt to factory reset */
        case APP_STATE_FACTORY_RESET:
            break;
#endif

        default:
            /* Ignore, paired with handset with known address as requested by peer */
            break;
    }
}

/*! \brief Handle state machine transitions when earbud removed from the ear. */
static void appSmHandlePhyStateOutOfEarEvent(void)
{
    DEBUG_LOG("appSmHandlePhyStateOutOfEarEvent");

    if (appSmIsCoreState())
        appSmSetCoreState();
}

/*! \brief Handle state machine transitions when earbud motionless out of the ear. */
static void appSmHandlePhyStateOutOfEarAtRestEvent(void)
{
    DEBUG_LOG("appSmHandlePhyStateOutOfEarAtRestEvent");
#if 0
    if (appSmIsCoreState())
        appSmSetCoreState();
#endif

    /*! \todo start dormant timer */
}

/*! \brief Handle state machine transitions when earbud is put in the ear. */
static void appSmHandlePhyStateInEarEvent(void)
{
    DEBUG_LOG("appSmHandlePhyStateInEarEvent");
    if (appSmIsCoreState())
        appSmSetCoreState();
}

/*! \brief Handle state machine transitions when earbud is put in the case. */
static void appSmHandlePhyStateInCaseEvent(void)
{
    DEBUG_LOG("appSmHandlePhyStateInCaseEvent");

    /*! \todo Need to add other non-core states to this conditional from which we'll
     * permit a transition back to a core state, such as...peer pairing? sleeping? */
    if (appSmIsCoreState() ||
        (appGetState() == APP_STATE_HANDSET_PAIRING))
    {
        appSmSetCoreState();
    }
}

/*! \brief Handle changes in physical state of the earbud. */
static void appSmHandlePhyStateChangedInd(smTaskData* sm, PHY_STATE_CHANGED_IND_T *ind)
{
    UNUSED(sm);

    DEBUG_LOGF("appSmHandlePhyStateChangedInd new phy state %d", ind->new_state);

    sm->phy_state = ind->new_state;

    switch (ind->new_state)
    {
        case PHY_STATE_IN_CASE:
            appSmHandlePhyStateInCaseEvent();
            return;
        case PHY_STATE_OUT_OF_EAR:
            appSmHandlePhyStateOutOfEarEvent();
            return;
        case PHY_STATE_OUT_OF_EAR_AT_REST:
            appSmHandlePhyStateOutOfEarAtRestEvent();
            return;
        case PHY_STATE_IN_EAR:
            appSmHandlePhyStateInEarEvent();
            return;
        default:
            break;
    }
}


/*! \brief Start pairing with peer earbud. */
static void appSmHandleConnRulesPeerPair(void)
{
    DEBUG_LOG("appSmHandleConnRulesPeerPair");

    switch (appGetState())
    {
        case APP_STATE_STARTUP:
            DEBUG_LOG("appSmHandleConnRulesPeerPair, rule said pair with peer");
            appSetState(APP_STATE_PEER_PAIRING);
            break;

    default:
            break;
    }
}

static void appSmHandleConnRulesHandsetPair(void)
{
    DEBUG_LOG("appSmHandleConnRulesHandsetPair");

    switch (appGetState())
    {
        case APP_STATE_OUT_OF_CASE_IDLE:
        case APP_STATE_IN_EAR_IDLE:
            DEBUG_LOG("appSmHandleConnRulesHandsetPair, rule said pair with handset");
            appSetState(APP_STATE_HANDSET_PAIRING);
            break;
        default:
            break;
    }
}

static void appSmHandleConnRulesEnterDfu(void)
{
    DEBUG_LOG("appSmHandleConnRulesEnterDfu");

    switch (appGetState())
    {
        case APP_STATE_IN_CASE_IDLE:
        case APP_STATE_IN_CASE_DFU:
            DEBUG_LOG("appSmHandleConnRulesEnterDfu, rule said enter DFU");
            appSmEnterDfuMode(TRUE);
            break;

        default:
            break;
    }
    appConnRulesSetRuleComplete(CONN_RULES_ENTER_DFU);
}

static void appSmHandleConnRulesAllowHandsetConnect(void)
{
    DEBUG_LOG("appSmHandleConnRulesAllowHandsetConnect");

    appConManagerAllowHandsetConnect(TRUE);
    appConnRulesSetRuleComplete(CONN_RULES_ALLOW_HANDSET_CONNECT);
}

static void appSmHandleConnRulesRejectHandsetConnect(void)
{
    DEBUG_LOG("appSmHandleConnRulesRejectHandsetConnect");

    appConManagerAllowHandsetConnect(FALSE);
    appConnRulesSetRuleComplete(CONN_RULES_REJECT_HANDSET_CONNECT);
}

static void appSmSendPeerSync(void)
{
    bdaddr peer_addr;
    bdaddr handset_addr;
    uint16 tws_version = DEVICE_TWS_UNKNOWN;
    uint16 battery_level = appBatteryGetVoltage();
    smTaskData *sm = appGetSm();

    DEBUG_LOG("appSmHandleConnRulesPeerSync");

    /* Can only send this if we have a peer earbud */
    if (appDeviceGetPeerBdAddr(&peer_addr))
    {
        /* Try and find last connected handset address, may not exist */
        if (!appDeviceGetHandsetBdAddr(&handset_addr))
            BdaddrSetZero(&handset_addr);
        else
            tws_version = appDeviceTwsVersion(&handset_addr);

        /* Store battery level we sent, so we can compare with peer */
        sm->sync_battery_level = battery_level;

        /* Attempt to send sync message to peer */
        appPeerSigSyncRequest(&sm->task, &peer_addr, &handset_addr,
                              sm->sync_battery_level, tws_version,
                              appDeviceIsHandsetA2dpConnected(), appDeviceIsHandsetA2dpStreaming(),
                              appDeviceIsHandsetAvrcpConnected(), appDeviceIsHandsetHfpConnected(),
                              appGetState() == APP_STATE_STARTUP,
                              appSmIsInCase(), appSmIsInEar(),
                              appGetState() == APP_STATE_HANDSET_PAIRING,
                              appDeviceGetHandsetBdAddr(&handset_addr));

        appConnRulesResetEvent(RULE_EVENT_PEER_SYNC_VALID);
    }
}

static void appSmHandleConnRulesPeerSync(void)
{
    /* Send peer sync */
    appSmSendPeerSync();

    /* In all cases mark rule as done */
    appConnRulesSetRuleComplete(CONN_RULES_SEND_PEER_SYNC);
}

static void appSmHandleConnRulesForwardLinkKeys(void)
{
    bdaddr peer_addr;

    DEBUG_LOG("appSmHandleConnRulesForwardLinkKeys");

    /* Can only send this if we have a peer earbud */
    if (appDeviceGetPeerBdAddr(&peer_addr))
    {
        /* Attempt to send handset link keys to peer device */
        appPairingTransmitHandsetLinkKeysReq();
    }
    else
    {
        /* We should never attempt to send link keys message if there's no peer device */
    }

    /* In all cases mark rule as done */
    appConnRulesSetRuleComplete(CONN_RULES_PEER_SEND_LINK_KEYS);
}


/*! \brief Connect HFP, A2DP and AVRCP to last connected handset. */
static void appSmHandleConnRulesConnectHandset(void)
{
    DEBUG_LOG("appSmHandleConnRulesConnectHandset");

    /* Connect HFP to handset */
    appHfpConnectHandset();

    /* Connect AVRCP and A2DP to handset */
    appAvConnectHandset();

    /* Mark rule as done */
    appConnRulesSetRuleComplete(CONN_RULES_CONNECT_HANDSET);
}


/*! \brief Connect A2DP and AVRCP to peer. */
static void appSmHandleConnRulesConnectPeer(void)
{
    DEBUG_LOG("appSmHandleConnRulesConnectPeer");

    /* Connect AVRCP and A2DP to peer */
    appAvConnectPeer();

    /* Mark rule as done */
    appConnRulesSetRuleComplete(CONN_RULES_CONNECT_PEER);
}


/*! \brief Connect HFP, A2DP and AVRCP to peer's connected handset. */
static void appSmHandleConnRulesConnectPeerHandset(void)
{
    smTaskData *sm = appGetSm();
    DEBUG_LOG("appSmHandleConnRulesConnectPeerHandset");

    /* Connect to peer's handset */
    appAvConnectWithBdAddr(&sm->peer_handset_addr);
    appHfpConnectWithBdAddr(&sm->peer_handset_addr, hfp_handsfree_107_profile);

    /* Mark rule as done */
    appConnRulesSetRuleComplete(CONN_RULES_CONNECT_PEER_HANDSET);
}

/*! \brief Connect HFP, A2DP and AVRCP to peer's connected handset. */
static void appSmHandleConnRulesUpdateMruPeerHandset(void)
{
    smTaskData *sm = appGetSm();
    DEBUG_LOG("appSmHandleConnRulesUpdateMruPeerHandset");

    /* Update most recent connected device */
    ConnectionSmUpdateMruDevice(&sm->peer_handset_addr);

    /* Mark rule as done */
    appConnRulesSetRuleComplete(CONN_RULES_UPDATE_MRU_PEER_HANDSET);

}

/*! \brief Send Earbud state and role messages to handset. */
static void appSmHandleConnRulesSendStatusToHandset(void)
{
    DEBUG_LOG("appSmHandleConnRulesSendStatusToHandset");

    /* request handset signalling module send current state to handset. */
    appHandsetSigSendEarbudStateReq(appSmGetPhyState());

    /* request handset signalling module sends current role to handset. */
    appHandsetSigSendEarbudRoleReq(appConfigIsLeft() ? EARBUD_ROLE_LEFT : EARBUD_ROLE_RIGHT);

    /* Mark rule as done */
    appConnRulesSetRuleComplete(CONN_RULES_SEND_STATE_TO_HANDSET);
}

/*! \brief Start timer to stop A2DP if earbud stays out of the ear. */
static void appSmHandleConnRulesA2dpTimeout(void)
{
    DEBUG_LOG("appSmHandleConnRulesA2dpTimeout");

    if (appSmIsOutOfCase() && appConfigOutOfEarA2dpTimeoutSecs())
    {
        DEBUG_LOGF("appSmHandleConnRulesA2dpTimeout, out of case/ear, so starting %u second timer", appConfigOutOfEarA2dpTimeoutSecs());
        MessageSendLater(appGetSmTask(), SM_INTERNAL_TIMEOUT_OUT_OF_EAR_A2DP,
                         NULL, D_SEC(appConfigOutOfEarA2dpTimeoutSecs()));
    }

    appConnRulesSetRuleComplete(CONN_RULES_A2DP_TIMEOUT);
}

/*! \brief Start timer to transfer SCO to AG if earbud stays out of the ear. */
static void appSmHandleConnRulesScoTimeout(void)
{
    DEBUG_LOG("appSmHandleConnRulesA2dpTimeout");

    if (appSmIsOutOfCase() && appConfigOutOfEarScoTimeoutSecs())
    {
        DEBUG_LOGF("appSmHandleConnRulesScoTimeout, out of case/ear, so starting %u second timer", appConfigOutOfEarScoTimeoutSecs());
        MessageSendLater(appGetSmTask(), SM_INTERNAL_TIMEOUT_OUT_OF_EAR_SCO,
                         NULL, D_SEC(appConfigOutOfEarScoTimeoutSecs()));
    }

    appConnRulesSetRuleComplete(CONN_RULES_SCO_TIMEOUT);
}

/*! \brief Turns LEDs on */
static void appSmHandleConnRulesLedEnable(void)
{
    DEBUG_LOG("appSmHandleConnRulesLedEnable");

    appLedEnable(TRUE);
    appConnRulesSetRuleComplete(CONN_RULES_LED_ENABLE);
}

/*! \brief Turns LEDs off */
static void appSmHandleConnRulesLedDisable(void)
{
    DEBUG_LOG("appSmHandleConnRulesLedDisable");

    appLedEnable(FALSE);
    appConnRulesSetRuleComplete(CONN_RULES_LED_DISABLE);
}

/*! \brief Pause A2DP as earbud is out of the ear for too long. */
static void appSmHandleInternalTimeoutOutOfEarA2dp(void)
{
    DEBUG_LOG("appSmHandleInternalTimeoutOutOfEarA2dp out of ear pausing audio");

    /* Double check we're still out of case when the timeout occurs */
    if (appSmIsOutOfCase())
    {
        /* request the handset pauses the AV, indicate not a UI initiated
         * request with FALSE */
        appAvPause(FALSE);

        /* start the timer to autorestart the audio if earbud is placed
         * back in the ear. */
        if (appConfigInEarA2dpStartTimeoutSecs())
        {
            MessageSendLater(appGetSmTask(), SM_INTERNAL_TIMEOUT_IN_EAR_A2DP_START,
                             NULL, D_SEC(appConfigInEarA2dpStartTimeoutSecs()));
        }
    }
}

/*! \brief Nothing to do. */
static void appSmHandleInternalTimeoutInEarA2dpStart(void)
{
    DEBUG_LOG("appSmHandleInternalTimeoutInEarA2dpStart");

    /* no action needed, we're just using the presence/absence of an active
     * timer when going into InEar to determine if we need to restart audio */
}


/*! \brief Transfer SCO to AG as earbud is out of the ear for too long. */
static void appSmHandleInternalTimeoutOutOfEarSco(void)
{
    DEBUG_LOG("appSmHandleInternalTimeoutOutOfEarSco out of ear transferring audio");

    /* Double check we're still out of case when the timeout occurs */
    if (appSmIsOutOfCase())
    {
        /* TODO: Check peer Earbud is out of case */

        /* Transfer SCO to handset */
        appHfpTransferToAg();
    }
}


/*! \brief Earbud put in case disconnect link to handset */
static void appSmHandleConnRulesHandsetDisconnect(void)
{
    DEBUG_LOG("appSmHandleConnRulesHandsetDisconnect");

    if (appDeviceIsHandsetHfpConnected())
    {
        DEBUG_LOG("appSmHandleConnRulesHandsetDisconnect, handset HFP is connected");
        appHfpDisconnectInternal();
    }
    if (appDeviceIsHandsetA2dpConnected() || appDeviceIsHandsetAvrcpConnected())
    {
        DEBUG_LOG("appSmHandleConnRulesHandsetDisconnect, handset AV is connected");
        appAvDisconnectHandset();
    }

    appConnRulesSetRuleComplete(CONN_RULES_HANDSET_DISCONNECT);
}


/*! \brief Earbud put in case disconnect link to peer */
static void appSmHandleConnRulesPeerDisconnect(void)
{
    DEBUG_LOG("appSmHandleConnRulesPeerDisconnect");

    if (appDeviceIsPeerA2dpConnected() || appDeviceIsPeerAvrcpConnectedForAv())
    {
        DEBUG_LOG("appSmHandleConnRulesPeerDisconnect, handset AV is connected");
        appAvDisconnectPeer();
    }

    appConnRulesSetRuleComplete(CONN_RULES_PEER_DISCONNECT);
}


/*! \brief Handle command to pair with a handset. */
static void appSmHandlePeerSigPairHandsetIndication(PEER_SIG_PAIR_HANDSET_IND_T *ind)
{
    smTaskData *sm = appGetSm();
    appDeviceAttributes attr;

    DEBUG_LOGF("appSmHandlePeerSigPairHandsetIndication, bdaddr %04x,%02x,%06lx",
               ind->handset_addr.nap, ind->handset_addr.uap, ind->handset_addr.lap);

    /* Check if we already know about this handset */
    if (appDeviceFindBdAddrAttributes(&ind->handset_addr, &attr))
    {
        /* Attempt to delete it, this will fail if we're connected */
        if (appDeviceDelete(&ind->handset_addr))
        {
            DEBUG_LOG("appSmHandlePeerSigPairHandsetIndication, known handset deleting and re-pairing");
            appPairingHandsetPairAddress(&sm->task, &ind->handset_addr);
        }
        else
            DEBUG_LOG("appSmHandlePeerSigPairHandsetIndication, known handset, current connected so ignore pairing request");
    }
    else
    {
        DEBUG_LOG("appSmHandlePeerSigPairHandsetIndication, pairing with handset");
        appPairingHandsetPairAddress(&sm->task, &ind->handset_addr);
    }
}

static void appSmHandlePeerSigSyncIndication(PEER_SIG_SYNC_IND_T *ind)
{
    smTaskData *sm = appGetSm();

    DEBUG_LOGF("appSmHandlePeerSigSyncIndication, battery %u, bdaddr %04x,%02x,%06lx, version %u.%02u, startup %u",
               ind->battery_level, ind->handset_addr.nap, ind->handset_addr.uap, ind->handset_addr.lap,
               ind->tws_version >> 8, ind->tws_version & 0xFF, ind->startup);
    DEBUG_LOGF("appSmHandlePeerSigSyncIndication, peer_a2dp_connected %u, peer_avrcp_connected %u, peer_hfp_conected %u, peer_a2dp_streaming %u",
               ind->peer_a2dp_connected, ind->peer_avrcp_connected, ind->peer_hfp_connected,
               ind->peer_a2dp_streaming);
    DEBUG_LOGF("appSmHandlePeerSigSyncIndication, peer_is_pairing %u, peer_has_handset_pairing %u",
               ind->peer_is_pairing, ind->peer_has_handset_pairing);

    /* Store peer's info */
    sm->peer_battery_level = ind->battery_level;
    sm->peer_handset_addr = ind->handset_addr;
    sm->peer_handset_tws = ind->tws_version;
    sm->peer_a2dp_connected = ind->peer_a2dp_connected;
    sm->peer_a2dp_streaming = ind->peer_a2dp_streaming;
    sm->peer_avrcp_connected = ind->peer_avrcp_connected;
    sm->peer_hfp_connected = ind->peer_hfp_connected;
    sm->peer_startup = ind->startup;
    sm->peer_in_case = ind->peer_in_case;
    sm->peer_in_ear = ind->peer_in_ear;
    sm->peer_is_pairing = ind->peer_is_pairing;
    sm->peer_has_handset_pairing = ind->peer_has_handset_pairing;

    /* Update peer sync state */
    sm->peer_sync_state |= SM_PEER_SYNC_RECEIVED;

    /* Record what profiles the peer is connected (and therefore supported) */
    appDeviceSetA2dpWasConnected(&sm->peer_handset_addr, sm->peer_a2dp_connected);
    appDeviceSetHfpWasConnected(&sm->peer_handset_addr, sm->peer_hfp_connected);
    if (sm->peer_a2dp_connected)
        appDeviceSetA2dpIsSupported(&sm->peer_handset_addr);
    if (sm->peer_avrcp_connected)
        appDeviceSetAvrcpIsSupported(&sm->peer_handset_addr);
    if (sm->peer_hfp_connected)
        appDeviceSetHfpIsSupported(&sm->peer_handset_addr, hfp_handsfree_107_profile);  // TODO: Get HFP profile version from peer

    /* Set peer sync valid event if we've received and successfully send peer sync messages */
    if (appSmIsPeerSyncComplete())
    {
        DEBUG_LOG("appSmHandlePeerSigSyncIndication, peer sync complete");

        appConnRulesResetEvent(RULE_EVENT_PEER_SYNC_FAILED);

        /* Check if we're in the startup state */
        if (appGetState() == APP_STATE_STARTUP)
        {
            DEBUG_LOG("appSmHandlePeerSigSyncIndication, peer sync complete, set initial state");

            /* Run peer sync valid rules */
            appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_PEER_SYNC_VALID);

            /* Clear peer startup flag, as we've set startup connect event */
            sm->peer_startup = 0;

            /* Move to core state state */
            appSmSetInitialCoreState();
        }
        else if (sm->peer_startup)
        {
            /* Peer is in startup state, send peer sync so that it complete startup */
            appSmSendPeerSync();

            /* Clear peer startup flag, as we've set startup connect event */
            sm->peer_startup = 0;
        }
        else
        {
            /* Run peer sync valid rules */
            appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_PEER_SYNC_VALID);
        }
    }
    else
    {
        DEBUG_LOG("appSmHandlePeerSigSyncIndication, peer sync not complete");

        /* Peer sync isn't complete, we've received one but haven't sent one successfully */
        appSmSendPeerSync();
    }
}

static void appSmHandlePeerSigSyncConfirm(PEER_SIG_SYNC_CFM_T *cfm)
{
    smTaskData *sm = appGetSm();

    if (cfm->status == peerSigStatusSuccess)
    {
        DEBUG_LOG("appSmHandlePeerSigStartupSyncConfirm, success");

        /* Update peer sync state */
        sm->peer_sync_state |= SM_PEER_SYNC_SENT;

        /* Set peer sync valid event if we've received and successfully send peer sync messages */
        if (appSmIsPeerSyncComplete())
        {
            /* Clear any lingering peer sync failed event */
            appConnRulesResetEvent(RULE_EVENT_PEER_SYNC_FAILED);

            /* Check if we're in the startup state, if so apply startup connect rules */
            if (appGetState() == APP_STATE_STARTUP)
            {
                DEBUG_LOG("appSmHandlePeerSigStartupSyncConfirm, startup sync complete, set initial state");

                /* Move to core state state, as startup is complete (we've exchange peer sync messages with peer) */
                appSmSetInitialCoreState();

                /* Clear peer startup flag, as we've set startup connect event */
                sm->peer_startup = 0;
            }
            else
            {
                /* We're not in the startup state but the peer is, so we should apply startup connect rules */
                if (sm->peer_startup)
                {
                    DEBUG_LOG("appSmHandlePeerSigStartupSyncConfirm, sync complete, peer startup");

                    /* Clear peer startup flag, as we've set startup connect event */
                    sm->peer_startup = 0;
                }
                else
                {
                    DEBUG_LOG("appSmHandlePeerSigStartupSyncConfirm, sync complete");

                    /* Generate peer sync valid event, this will in turn cause us to setup any connections if required */
                    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_PEER_SYNC_VALID);
                }
            }
        }
    }
    else
    {
        DEBUG_LOGF("appSmHandlePeerSigStartupSyncConfirm, failed, status %u", cfm->status);

        /* Failed to send, so clear both bits of peer sync state */
        sm->peer_sync_state &= ~(SM_PEER_SYNC_SENT | SM_PEER_SYNC_RECEIVED);

        /* Check if we're in the startup state, if so apply startup connect rules */
        if (appGetState() == APP_STATE_STARTUP)
        {
            DEBUG_LOG("appSmHandlePeerSigStartupSyncConfirm, startup sync failed, set initial state");

            /* Move to core state, as startup is complete even though failed to exchange peer sync messages with peer */
            appSmSetInitialCoreState();
        }
    }
}

static void appSmHandleAvA2dpConnectedInd(const AV_A2DP_CONNECTED_IND_T *ind)
{
    smTaskData *sm = appGetSm();

    DEBUG_LOG("appSmHandleAvA2dpConnectedInd");

    if (appDeviceIsHandset(&ind->bd_addr))
    {
        /* Peer sync information we sent is now out of date */
        sm->peer_sync_state &= ~SM_PEER_SYNC_SENT;

        appConnRulesResetEvent(RULE_EVENT_HANDSET_A2DP_DISCONNECTED);
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_HANDSET_A2DP_CONNECTED);

        /* Record that we're connected with A2DP to handset */
        appDeviceSetA2dpWasConnected(&ind->bd_addr, TRUE);
    }
    appSmPageScanUpdate();
}

static void appSmHandleAvA2dpDisconnectedInd(const AV_A2DP_DISCONNECTED_IND_T *ind)
{
    smTaskData *sm = appGetSm();

    DEBUG_LOGF("appSmHandleAvA2dpDisconnectedInd, reason %u inst %x", ind->reason, ind->av_instance);

    switch (appGetState())
    {
        case APP_STATE_FACTORY_RESET:
            if (appHfpIsDisconnected() && appAvIsDisconnected())
            {
                appSmDeletePairingAndReset();
            }
            break;

#ifdef INCLUDE_POWER_CONTROL
        case APP_STATE_OUT_OF_CASE_SOPORIFIC:
            /* Let power module know link state has changed, in case affects powerdown */
            appPowerLinkStateUpdated();
            break;
#endif

        case APP_STATE_HANDSET_PAIRING:
            if (!appDeviceIsHandsetAnyProfileConnected())
            {
                appPairingHandsetPair(appGetSmTask(), TRUE);
            }
            break;

        default:
            if (appDeviceIsHandset(&ind->bd_addr))
            {
                /* Peer sync information we sent is now out of date */
                sm->peer_sync_state &= ~SM_PEER_SYNC_SENT;

                /* Clear connected and set disconnected events */
                appConnRulesResetEvent(RULE_EVENT_HANDSET_A2DP_CONNECTED);
                appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_HANDSET_A2DP_DISCONNECTED);

                /* If it was a normal disconnect and we're not going asleep, record that
                 * we're not connected with A2DP to handset */
                if (ind->reason == AV_A2DP_DISCONNECT_NORMAL && !appSmIsSoporific())
                    appDeviceSetA2dpWasConnected(&ind->bd_addr, FALSE);
            }
            break;
    }
    appSmPageScanUpdate();
}

static void appSmHandleAvAvrcpConnectedInd(const AV_AVRCP_CONNECTED_IND_T *ind)
{
    smTaskData *sm = appGetSm();

    DEBUG_LOG("appSmHandleAvAvrcpConnectedInd");

    if (appDeviceIsHandset(&ind->bd_addr))
    {
        /* Peer sync information we sent is now out of date */
        sm->peer_sync_state &= ~SM_PEER_SYNC_SENT;

        appConnRulesResetEvent(RULE_EVENT_HANDSET_AVRCP_DISCONNECTED);
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_HANDSET_AVRCP_CONNECTED);
    }

    /* Check if connected to TWS+ device (handset or earbud) */
    if (appDeviceIsTwsPlusHandset(&ind->bd_addr) || appDeviceIsPeer(&ind->bd_addr))
    {
        DEBUG_LOG("appSmHandleAvAvrcpConnectedInd, using wallclock for UI synchronisation");

        /* Use wallclock on this connection to synchronise LEDs */
        appLedSetWallclock(ind->sink);
    }

    if (appDeviceIsPeer(&ind->bd_addr))
    {
        appConnRulesResetEvent(RULE_EVENT_PEER_DISCONNECTED);
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_PEER_CONNECTED);
    }
    appSmPageScanUpdate();
}

static void appSmHandleAvAvrcpDisconnectedInd(const AV_AVRCP_DISCONNECTED_IND_T *ind)
{
    smTaskData *sm = appGetSm();

    DEBUG_LOG("appSmHandleAvAvrcpDisconnectedInd");

    switch (appGetState())
    {
        case APP_STATE_FACTORY_RESET:
            if (appHfpIsDisconnected() && appAvIsDisconnected())
            {
                appSmDeletePairingAndReset();
            }
            break;

#ifdef INCLUDE_POWER_CONTROL
        case APP_STATE_OUT_OF_CASE_SOPORIFIC:
            /* Let power module know link state has changed, in case affects powerdown */
            appPowerLinkStateUpdated();
            break;
#endif

        case APP_STATE_HANDSET_PAIRING:
            if (!appDeviceIsHandsetAnyProfileConnected())
            {
                appPairingHandsetPair(appGetSmTask(), TRUE);
            }
            break;

        default:
            {
                if (appDeviceIsHandset(&ind->bd_addr))
                {
                    /* Peer sync information we sent is now out of date */
                    sm->peer_sync_state &= ~SM_PEER_SYNC_SENT;

                    appConnRulesResetEvent(RULE_EVENT_HANDSET_AVRCP_CONNECTED);
                    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_HANDSET_AVRCP_DISCONNECTED);
                }

                /* Check if disconnected from TWS+ device (handset or earbud) */
                if (appDeviceIsTwsPlusHandset(&ind->bd_addr) || appDeviceIsPeer(&ind->bd_addr))
                {
                    /* Don't use wallclock */
                    appLedSetWallclock(0);
                }

                if (appDeviceIsPeer(&ind->bd_addr))
                {
                    appConnRulesResetEvent(RULE_EVENT_PEER_CONNECTED);
                    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_PEER_DISCONNECTED);
                }
            }
            break;
    }
    appSmPageScanUpdate();
}

static void appSmHandleAvStreamingActiveInd(void)
{
    DEBUG_LOG("appSmHandleAvStreamingActiveInd");

    /* We only care about this event if we're in a core state,
     * and it could be dangerous if we're not */
    if (appSmIsCoreState())
        appSmSetCoreState();
}

static void appSmHandleAvStreamingInactiveInd(void)
{
    DEBUG_LOG("appSmHandleAvStreamingInactiveInd");

    /* We only care about this event if we're in a core state,
     * and it could be dangerous if we're not */
    if (appSmIsCoreState())
        appSmSetCoreState();
}

static void appSmHandleHfpConnectedInd(APP_HFP_CONNECTED_IND_T *ind)
{
    smTaskData *sm = appGetSm();

    DEBUG_LOG("appSmHandleHfpConnectedInd");

    if (appDeviceIsHandset(&ind->bd_addr))
    {
        /* Peer sync information we sent is now out of date */
        sm->peer_sync_state &= ~SM_PEER_SYNC_SENT;

        appConnRulesResetEvent(RULE_EVENT_HANDSET_HFP_DISCONNECTED);
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_HANDSET_HFP_CONNECTED);

        /* Record that we're connected with HFP to handset */
        appDeviceSetHfpWasConnected(&ind->bd_addr, TRUE);
    }
    appSmPageScanUpdate();
}

static void appSmHandleHfpDisconnectedInd(APP_HFP_DISCONNECTED_IND_T *ind)
{
    smTaskData *sm = appGetSm();

    DEBUG_LOGF("appSmHandleHfpDisconnectedInd, reason %u", ind->reason);

    switch (appGetState())
    {
        case APP_STATE_FACTORY_RESET:
            if (appHfpIsDisconnected() && appAvIsDisconnected())
            {
                appSmDeletePairingAndReset();
            }
            break;

#ifdef INCLUDE_POWER_CONTROL
        case APP_STATE_OUT_OF_CASE_SOPORIFIC:
            /* Let power module know link state has changed, in case affects powerdown */
            appPowerLinkStateUpdated();
            break;
#endif

        case APP_STATE_HANDSET_PAIRING:
            if (!appDeviceIsHandsetAnyProfileConnected())
            {
                appPairingHandsetPair(appGetSmTask(), TRUE);
            }
            break;

        default:
            if (appDeviceIsHandset(&ind->bd_addr))
            {
                /* Peer sync information we sent is now out of date */
                sm->peer_sync_state &= ~SM_PEER_SYNC_SENT;

                appConnRulesResetEvent(RULE_EVENT_HANDSET_HFP_CONNECTED);
                appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_HANDSET_HFP_DISCONNECTED);

                /* If it was a normal disconnect and we're not going asleep, record that
                 * we're not connected with HFP to handset */
                if (ind->reason == APP_HFP_DISCONNECT_NORMAL && !appSmIsSoporific())
                    appDeviceSetHfpWasConnected(&ind->bd_addr, FALSE);
            }
            break;
    }
    appSmPageScanUpdate();
}

static void appSmHandleHfpScoConnectedInd(void)
{
    DEBUG_LOG("appSmHandleHfpScoConnectedInd");

    /* We only care about this event if we're in a core state,
     * and it could be dangerous if we're not */
    if (appSmIsCoreState())
        appSmSetCoreState();
}

static void appSmHandleHfpScoDisconnectedInd(void)
{
    DEBUG_LOG("appSmHandleHfpScoDisconnectedInd");

    /* We only care about this event if we're in a core state,
     * and it could be dangerous if we're not */
    if (appSmIsCoreState())
        appSmSetCoreState();
}

static void appSmHandleInternalPairHandset(void)
{
    if (appSmIsIdle())
        appSetState(APP_STATE_HANDSET_PAIRING);
    else
        DEBUG_LOG("appSmHandleInternalPairHandset can only pair in IDLE state");
}

static void appSmHandleInternalDeleteHandsets(void)
{
    switch (appGetState())
    {
        case APP_STATE_IN_CASE_IDLE:
        case APP_STATE_OUT_OF_CASE_IDLE:
        case APP_STATE_IN_EAR_IDLE:
        case APP_STATE_FACTORY_RESET:
        {
            bdaddr bd_addr;
            while (appDeviceGetHandsetBdAddr(&bd_addr))
            {
                if (!appDeviceDelete(&bd_addr))
                    return;
            }
            break;
        }
        default:
            /* TODO error indication? */
            break;
    }
}

/*! \brief Handle request to start factory reset. */
static void appSmHandleInternalFactoryReset(void)
{
    if (appSmIsIdle())
        appSetState(APP_STATE_FACTORY_RESET);
    else
        DEBUG_LOG("appSmHandleInternalFactoryReset can only factory reset in IDLE state");
}

/*! \brief Handle failure to successfully disconnect links within timeout.
    Manually tear down the ACL and once profiles have completed their
    disconnect handling we'll go ahead and factory reset anyway.
*/
static void appSmHandleInternalFactoryResetTimeout(void)
{
    bdaddr addr;

    DEBUG_LOG("appSmHandleInternalFactoryResetTimeout");

    if (appDeviceGetPeerBdAddr(&addr) && appConManagerIsConnected(&addr))
    {
        DEBUG_LOG("appSmHandleInternalFactoryResetTimeout PEER IS STILL CONNECTED");
        appConManagerSendCloseAclRequest(&addr, TRUE);
    }
    if (appDeviceGetHandsetBdAddr(&addr) && appConManagerIsConnected(&addr))
    {
        DEBUG_LOG("appSmHandleInternalFactoryResetTimeout HANDSET IS STILL CONNECTED");
        appConManagerSendCloseAclRequest(&addr, TRUE);
    }
}

#ifdef INCLUDE_POWER_CONTROL
/*! \brief Handle request to disconnect links for power down. */
static void appSmHandleInternalPowerOffDisconnect(void)
{
    appSetState(APP_STATE_OUT_OF_CASE_SOPORIFIC);
}
#endif


#ifdef INCLUDE_DFU
static void appSmHandleEnterDfuWithTimeout(uint32 timeout)
{
    DEBUG_LOGF("appSmHandleEnterDfuWithTimeout(%d)",timeout);

    appSetState(APP_STATE_IN_CASE_DFU);

    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_DFU_CONNECT);

    MessageSendLater(appGetSmTask(), SM_INTERNAL_TIMEOUT_DFU_ENTRY, NULL, timeout);
}

static void appSmHandleDfuEnded(bool error)
{
    DEBUG_LOGF("appSmHandleDfuEnded(%d)",error);

    if (appGetState() == APP_STATE_IN_CASE_DFU)
    {
        appSetState(APP_STATE_IN_CASE_IDLE);
        if (error)
        {
            appGaiaDisconnect();
        }
    }
}


static void appSmHandleStartUpgrade(void)
{
    switch (appGetState())
    {
        case APP_STATE_IN_CASE_IDLE:
        case APP_STATE_IN_CASE_DFU:
            DEBUG_LOG("appSmHandleStartUpgrade, valid state to enter DFU");
            appSmEnterDfuMode(FALSE);
            break;

        default:
            DEBUG_LOG("appSmHandleStartUpgrade, asked upgrade to disconnect its transport");
            UpgradeTransportDisconnectRequest();
            break;
    }
}


/*! \brief Respond to a DFU request from Gaia

    Only accept if we are in DFU state, using a
    #APP_GAIA_CONNECT_ALLOWED_IND message, otherwise reject.
 */
static void appSmHandleEnterDfuFromGaia(void)
{
    APP_GAIA_CONNECT_ALLOWED_IND_T *msg = calloc(1,sizeof(APP_GAIA_CONNECT_ALLOWED_IND_T));
    bool allowed = (appGetState() == APP_STATE_IN_CASE_DFU);

    DEBUG_LOGF("appSmHandleEnterDfuFromGaia. Gaia connected allowed %d",allowed);
    if (allowed)
    {
        appSmHandleEnterDfuWithTimeout(APP_DFU_TIMEOUT_ENTERED_FROM_GAIA_MS);
    }

    msg->in_dfu_mode = allowed;
    MessageSend(appGetGaiaTask(),APP_GAIA_CONNECT_ALLOWED_IND, msg);
}
#endif


static void appSmHandleNoDfu(void)
{

    if (appGetState() != APP_STATE_IN_CASE_DFU)
    {
        DEBUG_LOG("appSmHandleNoDfu. Not gone into DFU, so safe to assume we can continue");

        appSetState(APP_STATE_STARTUP);
    }
    else
    {
        DEBUG_LOG("appSmHandleNoDfu. We are in DFU mode !");
    }
}


/*! \brief Application state machine message handler. */
void appSmHandleMessage(Task task, MessageId id, Message message)
{
    smTaskData* sm = (smTaskData*)task;

    switch (id)
    {
        case INIT_CFM:
            appSmHandleInitConfirm();
            break;

        /* Pairing completion confirmations */
        case PAIRING_PEER_PAIR_CFM:
            appSmHandlePairingPeerPairConfirm((PAIRING_PEER_PAIR_CFM_T *)message);
            break;
        case PAIRING_HANDSET_PAIR_CFM:
            appSmHandlePairingHandsetPairConfirm((PAIRING_HANDSET_PAIR_CFM_T *)message);
            break;

        /* Connection manager status indications */
        case CON_MANAGER_CONNECTION_IND:
            appSmHandleConManagerConnectionInd((CON_MANAGER_CONNECTION_IND_T*)message);
            break;

        /* AV status change indications */
        case AV_A2DP_CONNECTED_IND:
            appSmHandleAvA2dpConnectedInd((AV_A2DP_CONNECTED_IND_T *)message);
            break;
        case AV_A2DP_DISCONNECTED_IND:
            appSmHandleAvA2dpDisconnectedInd((AV_A2DP_DISCONNECTED_IND_T *)message);
            break;
        case AV_AVRCP_CONNECTED_IND:
            appSmHandleAvAvrcpConnectedInd((AV_AVRCP_CONNECTED_IND_T *)message);
            break;
        case AV_AVRCP_DISCONNECTED_IND:
            appSmHandleAvAvrcpDisconnectedInd((AV_AVRCP_DISCONNECTED_IND_T *)message);
            break;
        case AV_STREAMING_ACTIVE_IND:
            appSmHandleAvStreamingActiveInd();
            break;
        case AV_STREAMING_INACTIVE_IND:
            appSmHandleAvStreamingInactiveInd();
            break;

        /* HFP status change indications */
        case APP_HFP_CONNECTED_IND:
            appSmHandleHfpConnectedInd((APP_HFP_CONNECTED_IND_T *)message);
            break;
        case APP_HFP_DISCONNECTED_IND:
            appSmHandleHfpDisconnectedInd((APP_HFP_DISCONNECTED_IND_T *)message);
            break;
        case APP_HFP_SCO_CONNECTED_IND:
            appSmHandleHfpScoConnectedInd();
            break;
        case APP_HFP_SCO_DISCONNECTED_IND:
            appSmHandleHfpScoDisconnectedInd();
            break;

        /* Physical state changes */
        case PHY_STATE_CHANGED_IND:
            appSmHandlePhyStateChangedInd(sm, (PHY_STATE_CHANGED_IND_T*)message);
            break;

        /* Connection rules */
        case CONN_RULES_PEER_PAIR:
            appSmHandleConnRulesPeerPair();
            break;
        case CONN_RULES_SEND_PEER_SYNC:
            appSmHandleConnRulesPeerSync();
            break;
        case CONN_RULES_PEER_SEND_LINK_KEYS:
            appSmHandleConnRulesForwardLinkKeys();
            break;
        case CONN_RULES_CONNECT_HANDSET:
            appSmHandleConnRulesConnectHandset();
            break;
        case CONN_RULES_CONNECT_PEER:
            appSmHandleConnRulesConnectPeer();
            break;
        case CONN_RULES_CONNECT_PEER_HANDSET:
            appSmHandleConnRulesConnectPeerHandset();
            break;
        case CONN_RULES_UPDATE_MRU_PEER_HANDSET:
            appSmHandleConnRulesUpdateMruPeerHandset();
            break;
        case CONN_RULES_SEND_STATE_TO_HANDSET:
            appSmHandleConnRulesSendStatusToHandset();
            break;
        case CONN_RULES_A2DP_TIMEOUT:
            appSmHandleConnRulesA2dpTimeout();
            break;
        case CONN_RULES_SCO_TIMEOUT:
            appSmHandleConnRulesScoTimeout();
            break;
        case CONN_RULES_LED_ENABLE:
            appSmHandleConnRulesLedEnable();
            break;
        case CONN_RULES_LED_DISABLE:
            appSmHandleConnRulesLedDisable();
            break;
        case CONN_RULES_HANDSET_DISCONNECT:
            appSmHandleConnRulesHandsetDisconnect();
            break;
        case CONN_RULES_PEER_DISCONNECT:
            appSmHandleConnRulesPeerDisconnect();
            break;
        case CONN_RULES_HANDSET_PAIR:
            appSmHandleConnRulesHandsetPair();
            break;
        case CONN_RULES_ENTER_DFU:
            appSmHandleConnRulesEnterDfu();
            break;
        case CONN_RULES_ALLOW_HANDSET_CONNECT:
            appSmHandleConnRulesAllowHandsetConnect();
            break;
        case CONN_RULES_REJECT_HANDSET_CONNECT:
            appSmHandleConnRulesRejectHandsetConnect();
            break;

        /* Peer signalling messages */
        case PEER_SIG_PAIR_HANDSET_IND:
            appSmHandlePeerSigPairHandsetIndication((PEER_SIG_PAIR_HANDSET_IND_T*)message);
            break;
        case PEER_SIG_SYNC_IND:
            appSmHandlePeerSigSyncIndication((PEER_SIG_SYNC_IND_T*)message);
            break;
        case PEER_SIG_SYNC_CFM:
            appSmHandlePeerSigSyncConfirm((PEER_SIG_SYNC_CFM_T*)message);
            break;

        /* SM internal messages */
        case SM_INTERNAL_PAIR_HANDSET:
            appSmHandleInternalPairHandset();
            break;
        case SM_INTERNAL_DELETE_HANDSETS:
            appSmHandleInternalDeleteHandsets();
            break;
        case SM_INTERNAL_FACTORY_RESET:
            appSmHandleInternalFactoryReset();
            break;
        case SM_INTERNAL_TIMEOUT_FACTORY_RESET:
            appSmHandleInternalFactoryResetTimeout();
            break;

#ifdef INCLUDE_POWER_CONTROL
        case SM_INTERNAL_POWER_OFF_DISCONNECT:
            appSmHandleInternalPowerOffDisconnect();
            break;
#endif

#ifdef INCLUDE_DFU
        case SM_INTERNAL_ENTER_DFU_UI:
            appSmHandleEnterDfuWithTimeout(APP_DFU_TIMEOUT_ENTERED_FROM_UI_MS );
            break;

        case SM_INTERNAL_ENTER_DFU_GAIA:
            appSmHandleEnterDfuFromGaia();
            break;

        case SM_INTERNAL_ENTER_DFU_UPGRADED:
            appSmHandleEnterDfuWithTimeout(APP_DFU_TIMEOUT_REBOOT_TO_CONFIRM_MS);
            break;

        case SM_INTERNAL_ENTER_DFU_STARTUP:
            appSmHandleEnterDfuWithTimeout(APP_DFU_TIMEOUT_REBOOT_IN_DFU_MS);
            break;

        case SM_INTERNAL_START_UPGRADE:
            appSmHandleStartUpgrade();
            break;

        case SM_INTERNAL_DFU_ENDED:
            appSmHandleDfuEnded(FALSE);
            break;

        case SM_INTERNAL_TIMEOUT_DFU_ENTRY:
        case SM_INTERNAL_TIMEOUT_DFU_ACTIVITY:
            /* May be a case for kicking GAIA if no activity */
            appSmHandleDfuEnded(TRUE);
            break;
#endif

        case SM_INTERNAL_NO_DFU:
            appSmHandleNoDfu();
            break;

        case SM_INTERNAL_TIMEOUT_OUT_OF_EAR_A2DP:
            appSmHandleInternalTimeoutOutOfEarA2dp();
            break;

        case SM_INTERNAL_TIMEOUT_IN_EAR_A2DP_START:
            appSmHandleInternalTimeoutInEarA2dpStart();
            break;

        case SM_INTERNAL_TIMEOUT_OUT_OF_EAR_SCO:
            appSmHandleInternalTimeoutOutOfEarSco();
            break;

        default:
            appHandleUnexpected(id);
            break;
    }
}

bool appSmIsPeerSyncComplete(void)
{
    smTaskData *sm = appGetSm();
    return sm->peer_sync_state == SM_PEER_SYNC_COMPLETE;
}

void appSmGetPeerBatteryLevel(uint16 *battery_level, uint16 *peer_battery_level)
{
    smTaskData *sm = appGetSm();

    *battery_level = sm->sync_battery_level;
    *peer_battery_level = sm->peer_battery_level;
}

void appSmGetPeerHandsetAddr(bdaddr *peer_handset_addr)
{
    smTaskData *sm = appGetSm();
    *peer_handset_addr = sm->peer_handset_addr;
}

bool appSmIsPeerHandsetA2dpConnected(void)
{
    smTaskData *sm = appGetSm();
    return appSmIsPeerSyncComplete() && sm->peer_a2dp_connected;
}

bool appSmIsPeerHandsetA2dpStreaming(void)
{
    smTaskData *sm = appGetSm();
    return appSmIsPeerSyncComplete() && sm->peer_a2dp_streaming;
}

bool appSmIsPeerHandsetAvrcpConnected(void)
{
    smTaskData *sm = appGetSm();
    return appSmIsPeerSyncComplete() && sm->peer_avrcp_connected;
}

bool appSmIsPeerHandsetHfpConnected(void)
{
    smTaskData *sm = appGetSm();
    return appSmIsPeerSyncComplete() && sm->peer_hfp_connected;
}

bool appSmIsPeerHandsetTws(void)
{
    smTaskData *sm = appGetSm();
    return appSmIsPeerSyncComplete() && (sm->peer_handset_tws == DEVICE_TWS_VERSION);
}

bool appSmIsPeerInCase(void)
{
    smTaskData *sm = appGetSm();
    if (appSmIsPeerSyncComplete())
        return sm->peer_in_case;
    else
        return PHY_STATE_UNKNOWN;
}

bool appSmIsPeerInEar(void)
{
    smTaskData *sm = appGetSm();
    if (appSmIsPeerSyncComplete())
        return sm->peer_in_ear;
    else
        return PHY_STATE_UNKNOWN;
}

bool appSmIsPeerPairing(void)
{
    smTaskData *sm = appGetSm();
    if (appSmIsPeerSyncComplete())
        return FALSE;
    else
        return sm->peer_is_pairing;
}

bool appSmHasPeerHandsetPairing(void)
{
    smTaskData *sm = appGetSm();
    if (appSmIsPeerSyncComplete())
        return FALSE;
    else
        return sm->peer_has_handset_pairing;
}

bool appSmIsInEar(void)
{
    smTaskData *sm = appGetSm();
    return sm->phy_state == PHY_STATE_IN_EAR;
}

bool appSmIsOutOfEar(void)
{
    smTaskData *sm = appGetSm();
    return (sm->phy_state >= PHY_STATE_IN_CASE) && (sm->phy_state <= PHY_STATE_OUT_OF_EAR_AT_REST);
}

bool appSmIsInCase(void)
{
    smTaskData *sm = appGetSm();
    return (sm->phy_state == PHY_STATE_IN_CASE) || (sm->phy_state == PHY_STATE_UNKNOWN);
}

bool appSmIsOutOfCase(void)
{
    smTaskData *sm = appGetSm();
    return (sm->phy_state >= PHY_STATE_OUT_OF_EAR) && (sm->phy_state <= PHY_STATE_IN_EAR);
}


void appSmPairHandset(void)
{
    smTaskData *sm = appGetSm();
    MessageSend(&sm->task, SM_INTERNAL_PAIR_HANDSET, NULL);
}

void appSmDeleteHandsets(void)
{
    smTaskData *sm = appGetSm();
    MessageSend(&sm->task, SM_INTERNAL_DELETE_HANDSETS, NULL);
}

#if INCLUDE_DFU
void appSmEnterDfuMode(bool from_ui_request)
{
    MessageSend(appGetSmTask(),
                from_ui_request ? SM_INTERNAL_ENTER_DFU_UI
                                : SM_INTERNAL_ENTER_DFU_GAIA,
                NULL);
}

void appSmEnterDfuOnStartup(bool upgrade_reboot)
{
    MessageSend(appGetSmTask(),
                upgrade_reboot ? SM_INTERNAL_ENTER_DFU_UPGRADED
                               : SM_INTERNAL_ENTER_DFU_STARTUP,
                NULL);
}

void appSmEndDfuMode(void)
{
    MessageSend(appGetSmTask(), SM_INTERNAL_DFU_ENDED, NULL);
}


void appSmUpgradeStartedByGaia(void)
{
    MessageSend(appGetSmTask(), SM_INTERNAL_START_UPGRADE, NULL);
}
#endif

/*! \brief Initialise the main application state machine.
 */
void appSmInit(void)
{
    smTaskData* sm = appGetSm();
    memset(sm, 0, sizeof(*sm));
    sm->task.handler = appSmHandleMessage;
    sm->state = APP_STATE_NULL;
    sm->peer_sync_state = SM_PEER_SYNC_NONE;
    sm->phy_state = appPhyStateGetState();

    /* register with connection manager to get notification of (dis)connections */
    appConManagerRegisterConnectionsClient(&sm->task);

    /* register with HFP for changes in state */
    appHfpStatusClientRegister(&sm->task);

    /* register with AV to receive notifications of A2DP and AVRCP activity */
    appAvStatusClientRegister(&sm->task);

    /* register with peer signalling to receive PEER_SIG_PAIR_HANDSET_IND */
    appPeerSigPairHandsetTaskRegister(&sm->task);

    /* register with peer signalling to receive PEER_SIG_STARTUP_SYNC_IND */
    appPeerSigStartupSyncTaskRegister(&sm->task);

    appSetState(APP_STATE_INITIALISING);
}

void appSmConnectHandset(void)
{
    /* Generate event that will run rules to connect to handset */
    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_USER_CONNECT);
}

/*! \brief Request a factory reset. */
void appSmFactoryReset(void)
{
    MessageSend(appGetSmTask(), SM_INTERNAL_FACTORY_RESET, NULL);
}

#ifdef INCLUDE_POWER_CONTROL
/*! \brief Power off is in progress. */
void appSmPowerDownDisconnect(void)
{
    MessageSend(appGetSmTask(), SM_INTERNAL_POWER_OFF_DISCONNECT, NULL);
}
#endif



#ifdef INCLUDE_DFU
void appSmNotifyDfuActivity(void)
{
    appSmCancelDfuTimers();
    if (appGetState() == APP_STATE_IN_CASE_DFU)
    {
        MessageSendLater(appGetSmTask(), SM_INTERNAL_TIMEOUT_DFU_ACTIVITY, NULL, APP_DFU_TIMEOUT_NO_ACTIVITY_MS);
    }
}
#endif /* INCLUDE_DFU */

