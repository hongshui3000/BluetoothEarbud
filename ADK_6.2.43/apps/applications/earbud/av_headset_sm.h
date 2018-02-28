/*!
\copyright  Copyright (c) 2005 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_sm.h
\brief	    Header file for the application state machine
*/

#ifndef _AV_HEADSET_SM_H_
#define _AV_HEADSET_SM_H_

#include "av_headset_conn_rules.h"
#include "av_headset_phy_state.h"

/*!
    @startuml

    note "For clarity not all state transitions shown" as N1
    note top of STARTUP
      _IDLE is any of the idle states
      * IN_CASE_IDLE
      * OUT_OF_CASE_IDLE
      * IN_EAR_IDLE
    end note

    [*] -down-> INITIALISING : Power On
    INITIALISING : App module and library init
    INITIALISING --> DFU_CHECK : Init Complete

    DFU_CHECK : Is DFU in progress?
    DFU_CHECK --> STARTUP : No DFU in progress

    STARTUP : Check for paired peer earbud
    STARTUP : Attempt peer synchronisation
    STARTUP : After attempt go to _IDLE
    STARTUP --> PEER_PAIRING : No paired peer

    FACTORY_RESET : Disconnect links, deleting all pairing, reboot
    FACTORY_RESET : Only entered from _IDLE
    FACTORY_RESET -u-> INITIALISING

    PEER_PAIRING : Pair with peer earbud
    PEER_PAIRING --> STARTUP
    HANDSET_PAIRING : Pair with handset
    HANDSET_PAIRING : Return to _IDLE state

    state IN_CASE #LightBlue {
        IN_CASE : Charger Active
        IN_CASE : Buttons Disabled
        DFU : Device Upgrade
        DFU --> IN_CASE_IDLE #LightGreen : DFU Complete
        DFU_CHECK --> DFU : DFU in progress
        IN_CASE_IDLE : May have BT connection(s)
        IN_CASE_IDLE -up-> DFU : Start DFU
    }

    state OUT_OF_CASE #LightBlue {
        OUT_OF_CASE_IDLE : May have BT connection(s)
        OUT_OF_CASE_IDLE : Start dormant timer
        SOPORIFIC : Prepare to sleep
        SOPORIFIC : Disconnect links
        SLEEPING : Entering dormant
        SLEEPING -l-> INITIALISING : Movement from dormant
        SLEEPING -u-> [*] : Power Off
        OUT_OF_CASE_IDLE #LightGreen --> IN_CASE_IDLE : In Case
        IN_CASE_IDLE --> OUT_OF_CASE_IDLE : Out of Case
        OUT_OF_CASE_IDLE -u-> SOPORIFIC : Dormant timer expire
        SOPORIFIC -u-> SLEEPING : Powering down
        SOPORIFIC -u-> SLEEPING : Attempt enter dormant
        OUT_OF_CASE_IDLE --> HANDSET_PAIRING : User or Auto pairing
        OUT_OF_CASE_BUSY : Earbud removed from ear audio still playing
        OUT_OF_CASE_BUSY #LightGreen --> OUT_OF_CASE_IDLE : Out of ear audio timeout
        OUT_OF_CASE_BUSY --> OUT_OF_CASE_IDLE : Audio Inactive
        OUT_OF_CASE_BUSY -[HIDDEN]l->SOPORIFIC
    }

    state IN_EAR #LightBlue {
        IN_EAR_IDLE : May have BT connection(s)
        IN_EAR_IDLE #LightGreen -l-> OUT_OF_CASE_IDLE : Out of Ear
        IN_EAR_IDLE -u-> BUSY : Audio Active
        OUT_OF_CASE_IDLE -r-> IN_EAR_IDLE : In Ear
        BUSY : Streaming Audio Active (A2DP or SCO)
        BUSY : Tones audio available in other states
        BUSY #LightGreen -d-> IN_EAR_IDLE : Audio Inactive
        BUSY -l-> OUT_OF_CASE_BUSY : Out of Ear
        OUT_OF_CASE_BUSY -l-> BUSY : In Ear
        IN_EAR_IDLE --> HANDSET_PAIRING : User or Auto pairing
    }

    @enduml
*/

/*! \brief Application states */
typedef enum sm_application_states
{
    APP_STATE_NULL                      = 0x00, /*!< Initial state before state machine is running. */

    APP_STATE_INITIALISING              = 0x01, /*!< App module and library initialisation in progress. */
    APP_STATE_DFU_CHECK                 = 0x02, /*!< Interim state, to see if DFU is in progress. */
    APP_STATE_FACTORY_RESET             = 0x03, /*!< Resetting the earbud to factory defaults. */
    APP_STATE_STARTUP                   = 0x04, /*!< Startup, syncing with peer. */

    APP_STATE_PEER_PAIRING              = 0x05, /*!< Pairing with peer earbud */
    APP_STATE_HANDSET_PAIRING           = 0x06, /*!< Pairing with a handset */

    APP_STATE_IN_CASE                   = 0x10, /*!< Earbud is in the case, parent state */ 
        APP_STATE_IN_CASE_IDLE          = 0x11, /*!< In the case and idle */
        APP_STATE_IN_CASE_DFU           = 0x12, /*!< In the case and upgrading firmware. */

    APP_STATE_OUT_OF_CASE               = 0x20, /*!< Earbud is out of the case, parent state */
        APP_STATE_OUT_OF_CASE_IDLE      = 0x21, /*!< Out of the case and idle. */
        APP_STATE_OUT_OF_CASE_BUSY      = 0x22, /*!< Out of the case and audio is active. */
        APP_STATE_OUT_OF_CASE_SOPORIFIC = 0x23, /*!< Out of the case and going to a low power state. */
        APP_STATE_OUT_OF_CASE_SLEEPING  = 0x24, /*!< Out of the case and completing low power transition. */

    APP_STATE_IN_EAR                    = 0x40, /*!< Earbud is in the ear, parent state. */
        APP_STATE_IN_EAR_IDLE           = 0x41, /*!< In the ear and idle. */
        APP_STATE_IN_EAR_BUSY           = 0x42, /*!< In the ear and audio is active. */
} appState;

/*! \brief Check if the application is in an IDLE state.
    \note Warning, ensure this macro is updated if enum #sm_application_states is changed.
*/
#define appSmIsIdle() \
    ((appGetState() == APP_STATE_IN_CASE_IDLE) || \
     (appGetState() == APP_STATE_OUT_OF_CASE_IDLE) || \
     (appGetState() == APP_STATE_OUT_OF_CASE_SOPORIFIC) || \
     (appGetState() == APP_STATE_IN_EAR_IDLE))

/*! \brief Check if the application is in a core state.
    \note Warning, ensure this macro is updated if enum #sm_application_states is changed.
*/
#define appSmIsCoreState() \
    (  ((APP_STATE_IN_CASE <= appGetState()) && (appGetState() < APP_STATE_OUT_OF_CASE_SOPORIFIC) \
                                             && (appGetState() != APP_STATE_IN_CASE_DFU)) \
    || ((APP_STATE_IN_EAR  <= appGetState()) && (appGetState() <= APP_STATE_IN_EAR_BUSY)))

/*! \brief The state of synchronisation with the peer.
@startuml
participant Earbud1 as EB1
participant Earbud2 as EB2
participant Handset as HS
EB1 -> EB2: Peer Sync
EB1 <- EB2: Peer Sync
EB1 -> EB2: Connect A2DP/AVRCP
EB1 -> HS: Connect A2DP/AVRCP
note left: Peer Sync information of\nEarbud1 is now out of date
EB1 -> EB2: Peer Sync
==  ==
EB1 -> HS: Disconnect A2DP/AVRCP
note left: Peer Sync information of\nEarbud1 is now out of date
EB1 -> EB2: Peer Sync
@enduml */
typedef enum sm_peer_sync_states
{
    SM_PEER_SYNC_NONE   = 0,
    SM_PEER_SYNC_RECEIVED = 1,
    SM_PEER_SYNC_SENT = 2,
    SM_PEER_SYNC_COMPLETE = 3,
} smPeerSyncState;

/*! \brief Main application state machine task data. */
typedef struct
{
    TaskData task;                      /*!< SM task */
    appState state;                     /*!< Application state */
    phyState phy_state;                 /*!< Cache the current physical state */
    uint16 sync_battery_level;          /*!< Battery level that was sent in sync message */
    uint16 peer_battery_level;          /*!< Battery level of peer that was received in sync message */
    uint16 peer_handset_tws;            /*!< The peer's handset's TWS version */
    bdaddr peer_handset_addr;           /*!< The peer's handset's address */
    bool peer_a2dp_connected:1;         /*!< The peer has A2DP connected */
    bool peer_a2dp_streaming:1;         /*!< The peer has A2DP streaming */
    bool peer_avrcp_connected:1;        /*!< The peer has AVRCP connected */
    bool peer_hfp_connected:1;          /*!< The peer has HFP connected */
    bool peer_startup:1;                /*!< The peer is starting-up */
    bool peer_in_case:1;                /*!< The peer is in the case */
    bool peer_in_ear:1;                 /*!< The peer is in the ear */
    bool peer_is_pairing:1;             /*!< The peer is pairing */
    bool peer_has_handset_pairing:1;    /*!< The peer is paired with a handset */
    smPeerSyncState peer_sync_state:2;  /*!< The state of synchronisation with the peer */
} smTaskData;

/*! \brief Change application state.

    \param new_state [IN] State to move to.
 */
extern void appSetState(appState new_state);

/*! \brief Get current application state.

    \return appState Current application state.
 */
extern appState appGetState(void);

/*! \brief Initialise the main application state machine.
 */
extern void appSmInit(void);

/*! \brief Application state machine message handler.
    \param task The SM task.
    \param id The message ID to handle.
    \param message The message content (if any).
*/
extern void appSmHandleMessage(Task task, MessageId id, Message message);

/*! \brief Update application state. */
extern void appSmStateUpdate(void);

/* FUNCTIONS THAT CHECK THE STATE OF THE SM
 *******************************************/

/*! @brief Query if peer sync is complete.
    @return TRUE if complete, otherwise FALSE. */
extern bool appSmIsPeerSyncComplete(void);
/*! @brief Query if the peer's handset supports TWS+.
    @return TRUE if supported, otherwise FALSE. */
extern bool appSmIsPeerHandsetTws(void);
/*! @brief Query if the peer has an A2DP connection to its handset.
    @return TRUE if connected, otherwise FALSE. */
extern bool appSmIsPeerHandsetA2dpConnected(void);
/*! @brief Query if the peer is A2DP streaming from its handset.
    @return TRUE if streaming, otherwise FALSE. */
extern bool appSmIsPeerHandsetA2dpStreaming(void);
/*! @brief Query if the peer has an AVRCP connection to its handset.
    @return TRUE if connected, otherwise FALSE. */
extern bool appSmIsPeerHandsetAvrcpConnected(void);
/*! @brief Query if the peer has an HFP connection to its handset.
    @return TRUE if connected, otherwise FALSE. */
extern bool appSmIsPeerHandsetHfpConnected(void);
/*! @brief Query if the peer is in-case.
    @return TRUE if in-case, otherwise FALSE. */
extern bool appSmIsPeerInCase(void);
/*! @brief Query if the peer is in-ear.
    @return TRUE if in-ear, otherwise FALSE. */
extern bool appSmIsPeerInEar(void);
/*! @brief Query if the peer is pairing.
    @return TRUE if pairing, otherwise FALSE. */
extern bool appSmIsPeerPairing(void);
/*! @brief Query if the peer is paired with a handset.
    @return TRUE if paired, otherwise FALSE. */
extern bool appSmHasPeerHandsetPairing(void);
/*! @brief Query if this earbud is in-ear.
    @return TRUE if in-ear, otherwise FALSE. */
extern bool appSmIsInEar(void);
/*! @brief Query if this earbud is out-of-ear.
    @return TRUE if out-of-ear, otherwise FALSE. */
extern bool appSmIsOutOfEar(void);
/*! @brief Query if this earbud is in-case.
    @return TRUE if in-case, otherwise FALSE. */
extern bool appSmIsInCase(void);
/*! @brief Query if this earbud is out-of-case.
    @return TRUE if out-of-case, otherwise FALSE. */
extern bool appSmIsOutOfCase(void);
/*! @brief Query if this earbud is soporific. */
#define appSmIsSoporific() \
    (appGetState() == APP_STATE_OUT_OF_CASE_SOPORIFIC)
/*! @brief Query if this earbud is pairing. */
#define appSmIsPairing() \
    (appGetState() == APP_STATE_HANDSET_PAIRING)
/*! @brief Query if this earbud is in an active state. */
#define appSmIsActive() \
    (appGetState() >= APP_STATE_FACTORY_RESET && appGetState() <= APP_STATE_IN_EAR_BUSY)
/*! @brief Query if this earbud is in a sleep state. */
#define appSmIsSleepy() \
    (appGetState() == APP_STATE_OUT_OF_CASE_SOPORIFIC || \
     appGetState() == APP_STATE_OUT_OF_CASE_SLEEPING)
/*! @brief Get the physical state as received from last update message. */
#define appSmGetPhyState() (appGetSm()->phy_state)

/* FUNCTIONS TO RETURN INFORMATION
 **********************************/
/*! \brief Query synchronised battery levels.
    \param battery_level [OUT] This earbud's battery_level (sent in last sync).
    \param peer_battery_level [OUT] The peer earbud's battery level (received in the last sync).
*/
extern void appSmGetPeerBatteryLevel(uint16 *battery_level, uint16 *peer_battery_level);
/*! \brief Query the peer handset's address.
    \param peer_handset_addr [OUT] The address.
*/
extern void appSmGetPeerHandsetAddr(bdaddr *peer_handset_addr);

/* FUNCTIONS TO INITIATE AN ACTION IN THE SM
 ********************************************/
/*! \brief Initiate pairing with a handset. */
extern void appSmPairHandset(void);
/*! \brief Delete paired handsets. */
extern void appSmDeleteHandsets(void);
/*! \brief Connect to paired handset. */
extern void appSmConnectHandset(void);
/*! \brief Request a factory reset. */
extern void appSmFactoryReset(void);
/*! \brief Disconnect links in preparation to power-down */
extern void appSmPowerDownDisconnect(void);

#ifdef INCLUDE_DFU
/*! \brief Tell the state machine to enter DFU mode.
    \param from_ui_request Set to TRUE if the DFU request was made using the UI,
           as opposed to remotely, for example, using GAIA.
 */
extern void appSmEnterDfuMode(bool from_ui_request);
/*! \brief Tell the state machine to enter DFU mode following a reboot.
    \param upgrade_reboot If TRUE, indicates that DFU triggered the reboot.
                          If FALSE, indicates the device was rebooted whilst
                          an upgrade was in progress.
*/
extern void appSmEnterDfuOnStartup(bool upgrade_reboot);
/*! \brief Tell the state machine that DFU is ended. */
extern void appSmEndDfuMode(void);
/*! \brief Tell the state machine that DFU was start by GAIA */
extern void appSmUpgradeStartedByGaia(void);
/*! \brief Notify the state machine of DFU activity */
extern void appSmNotifyDfuActivity(void);

#else
/*! \brief Not implemented.
    \param from_ui_request Unused.
*/
#define appSmEnterDfuMode(from_ui_request) UNUSED(from_ui_request)

#endif

#endif
