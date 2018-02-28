/*!
\copyright  Copyright (c) 2015 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_peer_signalling.h
\brief	    Interface to module providing signalling to headset peer device.
*/

#ifndef AV_HEADSET_PEER_SIGNALLING_H
#define AV_HEADSET_PEER_SIGNALLING_H

#include "av_headset_message.h"
#include "av_headset_tasklist.h"
#include "av_headset_phy_state.h"

/*! Flag used on peer signalling states to indicate if the state represents 
    an activity that will finish. This is reflected in the lock member of 
    \ref peerSigTaskData. */
#define PEER_SIG_STATE_LOCK (0x10)

/*! Peer signalling state machine states */
typedef enum
{
    PEER_SIG_STATE_NULL = 0,                                       /*!< Initial state */
    PEER_SIG_STATE_DISCONNECTED = 1,                               /*!< No connection */
    PEER_SIG_STATE_CONNECTING_ACL = 2 + PEER_SIG_STATE_LOCK,       /*!< Connecting ACL */
    PEER_SIG_STATE_CONNECTING_LOCAL = 3 + PEER_SIG_STATE_LOCK,     /*!< Locally initiated connection in progress */
    PEER_SIG_STATE_CONNECTING_REMOTE = 4 + PEER_SIG_STATE_LOCK,    /*!< Remotely initiated connection is progress */
    PEER_SIG_STATE_CONNECTED = 5,                                  /*!< Connnected */
} appPeerSigState;


/*! Peer signalling module state. */
typedef struct
{

    TaskData task;                  /*!< Peer Signalling module task */
    appPeerSigState state:5;        /*!< Current state */
    uint16 lock;                    /*!< State machine lock */
    bdaddr peer_addr;               /*!< Bluetooth address of the peer we are signalling */

    avInstanceTaskData *av_inst;    /*!< Reference used to communicate using AVRCP with peer via the AV module */

    Task client_task;               /*!< Task to respond with result of current peer signalling operation */
    uint16 current_op;              /*!< Type of in progress operation. */

    Task link_key_task;             /*!< Task to send handset link key to when received from peer */
    Task pair_handset_task;         /*!< Task to send pair handset command when received from peer */
    Task sync_task;                 /*!< Task to sync data to when received from peer */

    bdaddr handset_addr;            /*!< Address of the handset for current operation */

    TaskList *client_tasks;         /*!< List of client tasks to which channel indications are sent */

    uint16 peer_battery;            /*!< Last level of peer battery received */
    bdaddr peer_handset_addr;       /*!< Address of handset that the peer is connected to */
    uint16 peer_tws_version;        /*!< The TWS version of the handset that the peer is connected to */

    uint8 attempts;                 /*!< Number of attempts to make at connecting to peer */

} peerSigTaskData;

/*! Enumeration of peer signalling status codes. */
typedef enum
{
    /*! Operation success. */
    peerSigStatusSuccess,

    /*! Failed to send link key to peer. */
    peerSigStatusLinkKeyTxFail,

    /*! Signalling channel with peer earbud established. */
    peerSigStatusChannelConnected,

    /*! Signalling channel with peer earbud disconnected. */
    peerSigStatusChannelDisconnected,

    /*! Failed to send #AVRCP_PEER_CMD_PAIR_HANDSET_ADDRESS. */
    peerSigStatusPairHandsetTxFail,

    /*! Failed to send #AVRCP_PEER_CMD_STARTUP_SYNC. */
    peerSigStatusStartupSyncTxFail
} peerSigStatus;

/*! Messages that can be sent by peer signalling to client tasks. */

enum av_headset_peer_signalling_messages
{
    /*! Result of operation to send link key to peer. */
    PEER_SIG_LINK_KEY_TX_CFM = PEER_SIG_MESSAGE_BASE,

    /*! Peer link key received. */
    PEER_SIG_LINK_KEY_RX_IND,

    /*! Signalling link to peer established. */
    PEER_SIG_CHANNEL_IND,

    /*! Pair Handset command received. */
    PEER_SIG_PAIR_HANDSET_IND,

    /*! Result of operation to send AVRCP_PEER_CMD_PAIR_HANDSET_ADDRESS to peer. */
    PEER_SIG_PAIR_HANDSET_CFM,

    /*! Result of operation to send AVRCP_PEER_CMD_STARTUP_SYNC to peer. */ 
    PEER_SIG_SYNC_CFM,

    /*! Startup sync command received. */
    PEER_SIG_SYNC_IND
};

/*! Message sent to client task with result of operation to send link key to peer. */
typedef struct
{
    peerSigStatus status;           /*!< Status of request */
    bdaddr handset_addr;            /*!< Handset that this response refers to */
} PEER_SIG_LINK_KEY_TX_CFM_T;

/*! Message sent to client task with handset link key received from peer. */
typedef struct
{
    peerSigStatus status;           /*!< Status of request */
    bdaddr handset_addr;            /*!< Handset that this response refers to */
    uint16 key_len;                 /*!< Length of the link key in <B>uint16s, not octets</B> */
    uint16 key[1];                  /*!< Link key. This will be the full length, \ref key_len */
} PEER_SIG_LINK_KEY_RX_IND_T;

/*! Message sent to clients registered to receive notification of peer signalling
    channel connection and disconnection events.

    The sstatus can be either #peerSigStatusChannelConnected or
    #peerSigStatusChannelDisconnected.
 */
typedef struct
{
    peerSigStatus status;           /*!< Connected / disconected status (see message description) */
} PEER_SIG_CHANNEL_IND_T;

/*! Message sent to pairing module to pair with a handset. */
typedef struct
{
    bdaddr handset_addr;            /*!< Address of handset to pair with */
} PEER_SIG_PAIR_HANDSET_IND_T;

/*! Message sent to client task with result of operation to send pair handset command to peer. */
typedef struct
{
    peerSigStatus status;           /*!< Status of pairing message */
    bdaddr handset_addr;            /*!< Address of handset the status applies to */
} PEER_SIG_PAIR_HANDSET_CFM_T;

/*! Message sent to client task with result of operation to send startup sync message to peer. */
typedef struct
{
    peerSigStatus status;           /*!< Status of startup sync message */
} PEER_SIG_SYNC_CFM_T;

/*! Message sent to clients with startup sync info from peer earbud. */
typedef struct
{ 
    uint16 battery_level;           /*!< Peers battery level */
    bdaddr handset_addr;            /*!< Current handset connected to peer */
    uint16 tws_version;             /*!< The TWS version for current handset */
    bool peer_a2dp_connected:1;     /*!< Is a2dp currently connected */
    bool peer_a2dp_streaming:1;     /*!< Is music being streamed */
    bool peer_avrcp_connected:1;    /*!< Is avrcp connected */
    bool peer_hfp_connected:1;      /*!< Is hfp connected */
    bool startup:1;                 /*!< Is the peer still in a startup phase */
    bool peer_in_case:1;            /*!< Is the peer physically in case */
    bool peer_in_ear:1;             /*!< Is the peer physically in ear */
    bool peer_is_pairing:1;         /*!< Is the peer in pairing mode */
    bool peer_has_handset_pairing:1;/*!< Does the peer have any handset pairing */
} PEER_SIG_SYNC_IND_T;

/*! Internal messages used by peer signalling. */
typedef enum
{
    /*! Message to bring up link to peer */
    PEER_SIG_INTERNAL_STARTUP_REQ,

    /*! Message to shut down link to peer */
    PEER_SIG_INTERNAL_SHUTDOWN_REQ,

    /*! Message to send Link Key to peer */
    PEER_SIG_INTERNAL_LINK_KEY_REQ,

    /*! Message to teardown peer signalling channel due to inactivity */
    PEER_SIG_INTERNAL_INACTIVITY_TIMER,

    /*! Message to send AVRCP_PEER_CMD_PAIR_HANDSET_ADDRESS to peer. */
    PEER_SIG_INTERNAL_PAIR_HANDSET_REQ,

    /*! Message to synchronise to peer, sending the information supplied */
    PEER_SIG_INTERNAL_SYNC_REQ
};

/*! Internal message sent to start signalling to a peer */
typedef struct
{
    bdaddr peer_addr;           /*!< Address of peer */
    uint8 attempts;             /*!< Number of attempts to contact the peer */
} PEER_SIG_INTERNAL_STARTUP_REQ_T;

/*! Message definition for action to send link key to peer. */
typedef struct
{
    Task client_task;           /*!< Task to receive any response */
    bdaddr handset_addr;        /*!< Handset that link key is for  */
    uint16 key_len;             /*!< Length of link key, in octets */
    uint8 key[1];               /*!< Link key. This will be the full length, key_len */
} PEER_SIG_INTERNAL_LINK_KEY_REQ_T;

/*! Message definition for action to send pair handset command. */
typedef struct
{
    Task client_task;           /*!< Task to receive any response */
    bdaddr handset_addr;        /*!< Handset that peer should try to pair with */
} PEER_SIG_INTERNAL_PAIR_HANDSET_REQ_T;

/*! Structure used to request synchronisation of peer information 
    carried in a PEER_SIG_INTERNAL_SYNC_REQ message */
typedef struct
{
    Task client_task;           /*!< Task to receive the sync result */
    uint16 battery_level;       /*!< Battery level to send */
    bdaddr handset_addr;        /*!< Address of the current handset */
    uint16 handset_tws_version; /*!< The TWS version for current handset */
    bool a2dp_connected:1;      /*!< Is a2dp currently connected */
    bool a2dp_streaming:1;      /*!< Are we currently streaming music */
    bool avrcp_connected:1;     /*!< Is avrcp currently connected */
    bool hfp_connected:1;       /*!< Is the HFP protocol connected */
    bool is_startup:1;          /*!< Are we still in startup mode */
    bool in_case:1;             /*!< Are we physically in case */
    bool in_ear:1;              /*!< Are we physically in ear */
    bool is_pairing:1;          /*!< Are we in a pairing mode */
    bool have_handset_pairing:1;/*!< Are we paired with any handsets */
} PEER_SIG_INTERNAL_SYNC_REQ_T;

/*! @brief Initialise the peer signalling module. 
 */
void appPeerSigInit(void);

/*! @brief Send handset link key to peer headset. 

    \param      task         Task to send confirmation message to.
    \param[in]  peer_addr    Address of peer earbud.
    \param[in]  handset_addr Address of handset to pair with.
    \param[in]  key          Pointer to the link key to send
    \param      key_len      Length of the link key <B>in uint16's (not octets)</B>
*/
void appPeerSigLinkKeyToPeerRequest(Task task, const bdaddr *peer_addr, const bdaddr *handset_addr,
                                  const uint16 *key, uint16 key_len);

/*! \brief Inform peer earbud of address of handset with which it should pair.
    \param task [IN] Task to send confirmation message to.
    \param peer_addr [IN] Address of peer earbud.
    \param handset_addr [IN] Address of handset to pair with.
*/
void appPeerSigTxPairHandsetRequest(Task task, const bdaddr* peer_addr, const bdaddr* handset_addr);

/*! \brief Send startup sync message to peer earbud.
    \param task [IN] Task to send confirmation message to.
    \param peer_addr [IN] Address of peer earbud.
    \param handset_addr [IN] Address of last connected handset.
    \param battery_level [IN] Voltage of battery.
    \param handset_tws_version [IN] TWS version of last connection handset.
    \param a2dp_connected [IN] Is A2DP connected.
    \param a2dp_streaming [IN] Is A2DP streaming.
    \param avrcp_connected [IN] Is AVRCP connected.
    \param hfp_connected [IN] Is HFP connected.
    \param is_startup [IN] Is in startup state.
    \param in_case [IN] Is in case.
    \param in_ear [IN] Is in ear.
    \param is_pairing [IN] Is in handset pairing state.
    \param have_handset_pairing [IN] Has pairing with at least one handset.
*/
void appPeerSigSyncRequest(Task task, const bdaddr *peer_addr, const bdaddr *handset_addr,
                           uint16 battery_level, uint16 handset_tws_version,
                           bool a2dp_connected, bool a2dp_streaming,
                           bool avrcp_connected, bool hfp_connected,
                           bool is_startup,
                           bool in_case, bool in_ear, bool is_pairing, bool have_handset_pairing);

/*! @brief Register task with peer signalling for Link Key TX/RX operations.
 
    @param client_task Task to send messages to regarding link key operations.
 */
void appPeerSigLinkKeyTaskRegister(Task client_task);

/*! @brief Unregister task with peer signalling for Link Key TX/RX operations.
 */
void appPeerSigLinkKeyTaskUnregister(void);

/*! @brief Try and connect peer signalling channel with specified peer earbud.
 
    @param peer_addr [IN] BT address of peer earbud to connect.
 */
void appPeerSigConnect(bdaddr* peer_addr);

/*! @brief Register to receive peer signalling notifications.

    \param  client_task Task to register to receive peer signalling messages.
 */
void appPeerSigClientRegister(Task client_task);

/*! @brief Unregister task that is currently receiving peer signalling notifications.

    \param  client_task Task that was registered for peer signalling messages.
 */
void appPeerSigClientUnregister(Task client_task);

/*! \brief Register task to receive AVRCP_PEER_CMD_PAIR_HANDSET_ADDRESS. 

    \param pair_handset_task    Task to receive AVRCP_PEER_CMD_PAIR_HANDSET_ADDRESS messages
  */
void appPeerSigPairHandsetTaskRegister(Task pair_handset_task);

/*! \brief Register task to receive AVRCP_PEER_CMD_STARTUP_SYNC. 

    \param[in] startup_sync_task    The task to be sent the message when the 
                        signalling link is created.
 */
void appPeerSigStartupSyncTaskRegister(Task startup_sync_task);

/*! \brief Force peer signalling to disconnect AVRCP if it is up. 

    \note This function should only be called if the intention
    is to restart the device. The device state is not guaranteed to
    be consistent following this.
*/
void appPeerSigForceDisconnect(void);

/* ****************************************************************************
 * TEST FUNCTIONS
 ******************************************************************************/
/*! \brief TEST FUNCTION to force a link to peer device */
void appPeerSigForceLinkToPeer(void);

#endif /* AV_HEADSET_PEER_SIGNALLING_H */
