/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_av.h
\brief      Header file AV State Machines (A2DP & AVRCP) 

    @startuml

    [*] -down-> NULL : appAvInit()
    NULL : Initialising AV application module
    NULL -down-> INITIALISING_A2DP : A2dpInit()

    INITIALISING_A2DP : Initialising A2DP profile library
    INITIALISING_A2DP -down-> INITIALISING_AVRCP : A2DP_INIT_CFM/AvrcpInit()

    INITIALISING_AVRCP : Initialising AVRCP profile library
    INITIALISING_AVRCP -down-> IDLE : AVRCP_INIT_CFM

    IDLE : Initialised and ready for connections

    @enduml
*/

#ifndef _AV_HEADSET_AV_H_
#define _AV_HEADSET_AV_H_

#include <a2dp.h>
#include <avrcp.h>
#include "av_headset.h"
#include "av_headset_device.h"
#include "av_headset_tasklist.h"

#ifdef INCLUDE_AV

/*! Maximum number of AV connections for TWS */
#define AV_MAX_NUM_TWS (1)
/*! Maximum number of AV connections for audio Sinks */
#define AV_MAX_NUM_SNK (1)

/*! \brief Maximum number of AV connections

    Defines the maximum number of simultaneous AV connections that are allowed.

    This is based on the number of links we want as TWS or as a standard Sink.

    Expectation is a value of 1 for a standard AV headset, or 2 for a True Wireless
    headset.
*/    
#define AV_MAX_NUM_INSTANCES (AV_MAX_NUM_TWS + AV_MAX_NUM_SNK)

/*! \brief Codec types used for instance identification */
typedef enum
{
    AV_CODEC_ANY,
    AV_CODEC_TWS,
    AV_CODEC_NON_TWS
} avCodecType;

/*! \brief A2DP stream endpoint IDs 

    \note Don't changing the ordering of these IDs as the A2DP code
          makes assumptions on the ordering.
*/
enum
{
    AV_SEID_INVALID,

    AV_SEID_SBC_SNK,            /*!< SBC Sink endpoint for standard handsets */
    AV_SEID_AAC_SNK,            /*!< AAC Sink endpoint for standard handsets */
    AV_SEID_APTX_SNK,           /*!< APTX Sink endpoint for standard handsets */

    AV_SEID_APTX_MONO_TWS_SNK,  /*!< Mono TTP APTX Sink endpoint for peer earbud receiving TWS & TWS+ Handsets */
    AV_SEID_SBC_MONO_TWS_SNK,	/*!< Mono TTP SBC Sink endpoint for peer earbud receiving TWS */

    AV_SEID_APTX_MONO_TWS_SRC,	/*!< Mono TTP APTX Source endpoint for earbud forwarding TWS  */
    AV_SEID_SBC_MONO_TWS_SRC,	/*!< Mono TTP SBC Source endpoint for earbud Forwarding TWS */

    AV_SEID_SBC_SRC             /*!< SBC source endpoint for PTS testing */
};

extern const sep_config_type av_sbc_snk_sep;
extern const sep_config_type av_aac_snk_sep;
extern const sep_config_type av_aptx_snk_sep;
extern const sep_config_type av_aptx_mono_tws_snk_sep;
extern const sep_config_type av_sbc_mono_tws_snk_sep;
extern const sep_config_type av_aptx_mono_tws_src;
extern const sep_config_type av_sbc_mono_tws_src;

extern void appAvUpdateSbcMonoTwsCapabilities(uint8 *caps, uint32_t sample_rate);
extern void appAvUpdateAptxMonoTwsCapabilities(uint8 *caps, uint32_t sample_rate);


/*! \brief AV task state machine states */
typedef enum
{
    AV_STATE_NULL,                  /*!< Startup state */
    AV_STATE_INITIALISING_A2DP,     /*!< Initialising A2DP profile library */
    AV_STATE_INITIALISING_AVRCP,    /*!< Initialising AVRCP profile library */
    AV_STATE_IDLE                   /*!< Initialised and ready for connections */
} avState;

/*! \brief AV suspend reasons

    The suspend reasons define the cause for suspending the AV streaming,
    a reason must be specified when calling appAvStreamingSuspend().
*/    
typedef enum
{
    AV_SUSPEND_REASON_SCO    = (1 << 0), /*!< Suspend AV due to active SCO link */
    AV_SUSPEND_REASON_HFP    = (1 << 1), /*!< Suspend AV due to HFP activity */
    AV_SUSPEND_REASON_AV     = (1 << 2), /*!< Suspend AV due to AV activity */
    AV_SUSPEND_REASON_RELAY  = (1 << 3), /*!< Suspend AV due to master suspend request */
    AV_SUSPEND_REASON_REMOTE = (1 << 4)  /*!< Suspend AV due to remote request */
} avSuspendReason;




/*! \brief Application A2DP state machine states */
typedef enum
{
    A2DP_STATE_NULL                                             = 0x00, /*!< Initial state */
    A2DP_STATE_DISCONNECTED                                     = 0x01, /*!< No A2DP connection */
    A2DP_STATE_CONNECTING_LOCAL	                                = 0x02, /*!< Locally initiated connection in progress */
    A2DP_STATE_CONNECTING_REMOTE                                = 0x03, /*!< Remotely initiated connection is progress */
        A2DP_STATE_CONNECTED_SIGNALLING                         = 0x10, /*!< Signalling channel connected */
        A2DP_STATE_CONNECTING_MEDIA_LOCAL                       = 0x11, /*!< Locally initiated media channel connection in progress */
        A2DP_STATE_CONNECTING_MEDIA_REMOTE                      = 0x12, /*!< Remotely initiated media channel connection in progress */
            A2DP_STATE_CONNECTED_MEDIA	                        = 0x30, /*!< Media channel connected (parent-pseudo state) */
                A2DP_STATE_CONNECTED_MEDIA_STREAMING            = 0x31, /*!< Media channel streaming */
                A2DP_STATE_CONNECTED_MEDIA_STREAMING_MUTED      = 0x32, /*!< Media channel streaming but muted (suspend failed) */
                A2DP_STATE_CONNECTED_MEDIA_SUSPENDING_LOCAL     = 0x33, /*!< Locally initiated media channel suspend in progress */
                A2DP_STATE_CONNECTED_MEDIA_SUSPENDED	        = 0x34, /*!< Media channel suspended */
                A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL       = 0x70, /*!< Locally initiated media channel start in progress */
                A2DP_STATE_CONNECTED_MEDIA_STARTING_LOCAL_SYNC  = 0x71, /*!< Locally initiated media channel start in progress, syncing slave */
                A2DP_STATE_CONNECTED_MEDIA_STARTING_REMOTE_SYNC = 0x37, /*!< Remotely initiated media channel start in progress, syncing slave */
            A2DP_STATE_DISCONNECTING_MEDIA                      = 0x13, /*!< Locally initiated media channel disconnection in progress */
    A2DP_STATE_DISCONNECTING                                    = 0x0A  /*!< Disconnecting signalling and media channels */
} avA2dpState;


/*! Flag used on AVRCP states to indicate if the state represents an activity
    that will finish. */
#define AVRCP_STATE_LOCK (0x10)

/*! Application A2DP state machine states */
typedef enum
{
    AVRCP_STATE_NULL = 0,                                               /*!< Initial state */
    AVRCP_STATE_DISCONNECTED = 1,                                       /*!< No AVRCP connection */
    AVRCP_STATE_CONNECTING_LOCAL = 2 + AVRCP_STATE_LOCK,                /*!< Locally initiated connection in progress */
    AVRCP_STATE_CONNECTING_LOCAL_WAIT_RESPONSE = 3 + AVRCP_STATE_LOCK,  /*!< Locally initiated connection in progress, waiting got client task to respond */
    AVRCP_STATE_CONNECTING_REMOTE = 4 + AVRCP_STATE_LOCK,               /*!< Remotely initiated connection is progress */
    AVRCP_STATE_CONNECTING_REMOTE_WAIT_RESPONSE = 5 + AVRCP_STATE_LOCK, /*!< Remotely initiated connection in progress, waiting got client task to respond */
    AVRCP_STATE_CONNECTED = 6,                                          /*!< Control channel connected */
    AVRCP_STATE_DISCONNECTING = 7 + AVRCP_STATE_LOCK                    /*!< Disconnecting control channel */
} avAvrcpState;

/*! \brief AV instance structure

    This structure contains all the information for a AV (A2DP & AVRCP)
    connection.
*/
typedef struct avInstanceData
{
    TaskData        av_task;                    /*!< Task/Message information for this instance */
    
    bdaddr          bd_addr;                    /*!< Bluetooth Address of remote device */
    bool            detach_pending;             /*!< Flag indicating if the instance is about to be detatched */

    TaskList        *av_status_client_list;     /*!< List of tasks to get status indications about this AV instance */

    AVRCP           *avrcp;                     /*!< AVRCP profile library instance */
    avAvrcpState    avrcp_state;                /*!< Current state of AVRCP state machine */
    uint16          avrcp_lock;                 /*!< AVRCP operation lock, used to serialise AVRCP operations */
    unsigned        avrcp_op_id:8;              /*!< Last sent AVRCP operation ID, used to determine which confirmation or error tone to play */
    unsigned        avrcp_op_state:1;           /*!< Last sent AVRCP operation state, used to determine if operation was button press or release */
    unsigned        avrcp_op_repeat:1;          /*!< Last send AVRCP operation was a repeat */
    unsigned        avcrp_supported_events:13;  /*!< Bitmask of events supported. See the avrcp_supported_events enum.*/
    TaskList        *avrcp_client_list;         /*!< List of clients for AVRCP messages for this link */
    uint8           avrcp_client_lock;          /*!< Count of the number of clients registered for this AVRCP link */
    uint8           avrcp_client_responses;     /*!< Count of outstanding response to requests sent to registered clients */
    Task            avrcp_vendor_task;          /*!< Task to receive vendor commands */
    uint8           *avrcp_vendor_data;         /*!< Data for current vendor command */
    uint16          avrcp_vendor_opid;          /*!< Operation identifier of the current vendor command */
    bool            avrcp_remotely_initiated;   /*!< Was this connection initiated by the far end */
                                            /*! Current play status of the AVRCP connection. 
                                                This is not always known. See \ref avrcp_play_hint */
    avrcp_play_status avrcp_play_status;
    avrcp_play_status avrcp_play_hint;          /*!< Our local guess at the play status. Not always accurate. */

    avA2dpState     a2dp_state;                 /*!< Current state of A2DP state machine */
    uint16          a2dp_lock;                  /*!< A2DP operation lock, used to serialise A2DP operations */
    uint8           a2dp_device_id;             /*!< A2DP device identifier from A2DP library */
    uint8           a2dp_stream_id;             /*!< A2DP stream identifier from A2DP library */
    Sink            a2dp_media_sink;            /*!< Sink for A2DP media (streaming) */
    uint8           a2dp_volume;                /*!< Current volume */
    uint8           a2dp_current_seid;          /*!< Currently active SEID */
    unsigned        a2dp_flags:6;               /*!< General connection flags */
    unsigned        a2dp_suspend_state:5;       /*!< Bitmap of active suspend reasons */
    unsigned        a2dp_num_seids:3;           /*!< Number of supported SEIDs */
    unsigned        a2dp_connect_retries:3;     /*!< Number of connection retries */
    unsigned        a2dp_local_initiated:1;     /*!< Flag to indicate if connection was locally initiated */
    unsigned        a2dp_disconnect_reason:4;   /*!< Reason for disconnect */
} avInstanceTaskData;  


/*! When responding to an incoming AVRCP connection, these are the 
    allowed responses */
typedef enum
{
    AV_AVRCP_REJECT,
    AV_AVRCP_ACCEPT,
    AV_AVRCP_ACCEPT_PASSIVE     /*!< The passive means accept connection, but make no attempt to maintain it */
} avAvrcpAccept;

/*! \brief AV task data

    Structure used to hold data relevant only to the AV task. Other modules
    should not access this information directly.
*/
typedef struct
{
    TaskData        task;                   /*!< Task for messages */
    unsigned        state:2;                /*!< Current state of AV state machine */
    unsigned        suspend_state:5;        /*!< Bitmap of active suspend reasons */
    avInstanceTaskData *av_inst[AV_MAX_NUM_INSTANCES];  /*!< AV Instances */

    TaskList        *avrcp_client_list;     /*!< List of tasks registered via \ref appAvAvrcpClientRegister */
    TaskList        *av_status_client_list; /*!< List of tasks registered via \ref appAvStatusClientRegister.
                                                These receive indications for AVRCP, A2DP and A2DP streaming */

} avTaskData;
    
/*! \brief Persistent store data */
typedef struct
{
    unsigned volume:7;  /*!< Current AV volume */
} avPsConfigData;

/*! \brief Internal message IDs */
enum
{
    AV_INTERNAL_REMOTE_IND,
    AV_INTERNAL_REMOTE_REPEAT,
    AV_INTERNAL_VOLUME_UP_REPEAT,
    AV_INTERNAL_VOLUME_DOWN_REPEAT,
};

/*! \brief Message IDs from AV to main application task */
enum av_headset_av_messages
{
    AV_INIT_CFM = AV_MESSAGE_BASE,
    AV_A2DP_CONNECTED_IND,
    AV_A2DP_DISCONNECTED_IND,

    AV_AVRCP_CONNECT_IND,
    AV_AVRCP_CONNECT_CFM,
    AV_AVRCP_DISCONNECT_IND,            /*!< Indication sent when a link starts disconnecting */

    AV_AVRCP_CONNECTED_IND,
    AV_AVRCP_DISCONNECTED_IND,          /*!< Indication sent when a link completed disconnection */
    AV_AVRCP_VENDOR_PASSTHROUGH_IND,
    AV_AVRCP_VENDOR_PASSTHROUGH_CFM,

    AV_CREATE_IND,
    AV_DESTROY_IND,
    AV_STREAMING_ACTIVE_IND,
    AV_STREAMING_INACTIVE_IND
};

/*! \brief Message sent to indicate that an A2DP link has disconnected. 
    This is sent to all tasks registered for messages */
typedef struct
{
    avInstanceTaskData *av_instance;    /*!< The AV instance this applies to */
    bdaddr bd_addr;                     /*!< Bluetooth address of connected device */
    bool local_initiated;               /*!< Whether the connection was requested on this device */
} AV_A2DP_CONNECTED_IND_T;


/*! \brief Reasons for an A2DP link disconnection. 
    This is a reduced list compared to hci disconnect codes available */
typedef enum
{
    AV_A2DP_CONNECT_FAILED,         /*!< A connection attempt failed */
    AV_A2DP_DISCONNECT_LINKLOSS,    /*!< Link dropped */
    AV_A2DP_DISCONNECT_NORMAL,      /*!< A requested disconnect completed */
} avA2dpDisconnectReason;

/*! \brief Message sent to indicate that an A2DP link has disconnected. 
    This is sent to all tasks registered for messages */
typedef struct
{
    avInstanceTaskData *av_instance;    /*!< The AV instance this applies to */
    bdaddr bd_addr;                     /*!< Bluetooth address of disconnected device */
    avA2dpDisconnectReason reason;      /*!< Reason for disconnection */
} AV_A2DP_DISCONNECTED_IND_T;

/*! \brief Message sent to indicate that an AVRCP link has connected. 
    This is sent to all tasks registered for avrcp messages */
typedef struct
{
    avInstanceTaskData *av_instance;    /*!< The AV instance this applies to */
    bdaddr bd_addr;                     /*!< Bluetooth address of connected device */
    uint16 connection_id;               /*!< Connection ID */
    uint16 signal_id;                   /*!< Signalling identifier  */
} AV_AVRCP_CONNECT_IND_T;

/*! Message sent to indicate result of requested AVRCP link connection */
typedef struct
{
    avInstanceTaskData *av_instance;    /*!< The AV instance this applies to */
    avrcp_status_code status;           /*!< Status of the connection request */
} AV_AVRCP_CONNECT_CFM_T;

/*! Message sent to indicate that an AVRCP link connection has disconnected */
typedef struct
{
    avInstanceTaskData *av_instance;    /*!< The AV instance this applies to */
    avrcp_status_code status;           /*!< Reason for the disconnection */
} AV_AVRCP_DISCONNECT_IND_T;

/*! Message sent to indicate an AVRCP link has connected */
typedef struct
{
    avInstanceTaskData *av_instance;    /*!< The AV instance this applies to */
    bdaddr bd_addr;                     /*!< Bluetooth address of the disconnected device */
    Sink sink;                          /*!< The Sink for the link */
} AV_AVRCP_CONNECTED_IND_T;

/*! \brief Message sent to indicate an AVRCP link has completed disconnection process 
    Normally expect a \ref AV_AVRCP_DISCONNECT_IND, followed by \ref AV_AVRCP_DISCONNECTED_IND.
 */
typedef struct
{
    avInstanceTaskData *av_instance;    /*!< The AV instance this applies to */
    bdaddr bd_addr;                     /*!< Bluetooth address of the disconnected device */
} AV_AVRCP_DISCONNECTED_IND_T;

/*! Message sent to report an incoming passthrough message */
typedef struct
{
    avInstanceTaskData *av_instance;    /*!< The AV instance this applies to */
    uint16 opid;                        /*!< The passthrough operation ID */
    uint16 size_payload;                /*!< Size of the variable length payload (in octets) */
    uint8 payload[1];                   /*!< Actual payload. The message is variable length. */
} AV_AVRCP_VENDOR_PASSTHROUGH_IND_T;

/*! Message sent to confirm an outgoing passthrough has been processed */
typedef struct
{
    avInstanceTaskData *av_instance;    /*!< The AV instance this applies to */
    avrcp_status_code status;           /*!< The status of the request. Can be failure. */
    uint16 opid;                        /*!< The passthrough operation ID */
} AV_AVRCP_VENDOR_PASSTHROUGH_CFM_T;

/*! Internal message for a remote control message */
typedef struct
{
    avc_operation_id op_id; /*!< Command to repeat */
    uint8 state;            /*!< State relevant to the command. Often just 0 or 1. */
    bool beep;              /*!< Whether to issue a UI indication when this command is applied */
} AV_INTERNAL_REMOTE_IND_T;

/*! Internal message for a repeated remote control message */
typedef struct
{
    avc_operation_id op_id; /*!< Command to repeat */
    uint8 state;            /*!< State relevant to the command. Often just 0 or 1. */
    bool beep;              /*!< Whether to issue a UI indication when this command is applied */
} AV_INTERNAL_REMOTE_REPEAT_T;

/*! Internal message for a volume repeat */
typedef struct
{
    int16 step;         /*!< Step to adjust volume by +ve or -ve */
} AV_INTERNAL_VOLUME_REPEAT_T;




/*! \brief Internal A2DP & AVRCP message IDs */
enum av_headset_internal_messages
{
    AV_INTERNAL_A2DP_BASE,
    AV_INTERNAL_A2DP_SIGNALLING_CONNECT_IND = AV_INTERNAL_A2DP_BASE,
    AV_INTERNAL_A2DP_CONNECT_REQ,
    AV_INTERNAL_A2DP_DISCONNECT_REQ,
    AV_INTERNAL_A2DP_SUSPEND_MEDIA_REQ,
    AV_INTERNAL_A2DP_RESUME_MEDIA_REQ,
    AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ,
    AV_INTERNAL_A2DP_QUERY_MEDIA_REQ,
    AV_INTERNAL_A2DP_SYNC_MEDIA_IND,
    AV_INTERNAL_A2DP_DISCONNECT_MEDIA_REQ,
    AV_INTERNAL_A2DP_CONFIGURE_MEDIA_REQ,
    AV_INTERNAL_AVRCP_UNLOCK_IND,
    AV_INTERNAL_A2DP_DESTROY_REQ,
    AV_INTERNAL_A2DP_VOLUME_STORE_REQ,
    AV_INTERNAL_A2DP_TOP,

    AV_INTERNAL_AVRCP_BASE,
    AV_INTERNAL_AVRCP_CONNECT_IND = AV_INTERNAL_AVRCP_BASE, /*!< Internal indication of signalling channel connection */
    AV_INTERNAL_AVRCP_CONNECT_RES,
    AV_INTERNAL_AVRCP_CONNECT_REQ,       /*!< Internal request to connect AVRCP */
    AV_INTERNAL_AVRCP_DISCONNECT_REQ,    /*!< Internal request to disconnect AVRCP */
    AV_INTERNAL_AVRCP_DESTROY_REQ,       /*!< Internal request to clean up */
    AV_INTERNAL_AVRCP_REMOTE_REQ,        /*!< Internal request to send AVRCP passthrough command */
    AV_INTERNAL_AVRCP_REMOTE_REPEAT_REQ, /*!< Internal request to send repeat AVRCP passthrough command */
    AV_INTERNAL_AVRCP_VOLUME_NOTIFY_REQ, /*!< Internal request to send volume notification */
    AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_RES,
    AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_REQ,   /*!< Internal request to send a vendor passthrough command */
    AV_INTERNAL_PLAYBACK_STATUS_UPDATE_IND,
    AV_INTERNAL_AVRCP_TOP
};

/*! Internal indication of signalling channel connection */
typedef struct
{
    uint16      a2dp_device_id;         /*!< A2DP Device Identifier */
    unsigned    flags:6;                /*!< Connect flags */
} AV_INTERNAL_A2DP_SIGNALLING_CONNECT_IND_T;

/*! Internal request to connect signalling channel */
typedef struct
{
    unsigned    flags:6;                /*!< Connect flags */
    unsigned    num_retries:3;          /*!< Number of connect retries */
} AV_INTERNAL_A2DP_CONNECT_REQ_T;

/*! Internal request to disconnect */
typedef struct
{
    unsigned    flags:6;                /*!< Disconnect flags */
} AV_INTERNAL_A2DP_DISCONNECT_REQ_T;

/*! Internal request to connect media channel */
typedef struct
{
    uint8 seid;                         /*!< Required SEID, or 0 if no preference */
} AV_INTERNAL_A2DP_CONNECT_MEDIA_REQ_T;

/*! Internal request to suspend streaming */
typedef struct
{
    avSuspendReason reason;             /*!< Suspend reason */
} AV_INTERNAL_A2DP_SUSPEND_MEDIA_REQ_T;

/*! Internal request to resume streaming */
typedef struct
{
    avSuspendReason reason;             /*!< Start reason */
} AV_INTERNAL_A2DP_RESUME_MEDIA_REQ_T;

/*! Internal request to send SEP capabilities to remote device */
typedef struct
{
    /*! A2dp library transaction id that should be passed back to library when calling A2dpGetCapsResponse() */
    uint8  id;
    /*! The seid being queried */
    const sep_config_type *sep_config;
} AV_INTERNAL_A2DP_GET_CAPS_IND_T;

/*! Internal indication of channel connection */
typedef struct
{
    uint16  connection_id;  /*!< The ID for this connection.  Must be returned as part of the AvrcpConnectResponse API. */
    uint16  signal_id;      /*!< Signalling identifier */
} AV_INTERNAL_AVRCP_CONNECT_IND_T;

/*! Internal routing of a response to a connection request, indicating acceptance */
typedef struct
{
    Task            client_task;    /*!< Task responding */
    uint16          connection_id;  /*!< Connection ID */
    uint16          signal_id;      /*!< Signalling identifier  */
    avAvrcpAccept   accept;         /*!< How the connection is accepted/rejected */
} AV_INTERNAL_AVRCP_CONNECT_RES_T;

/*! Internal request to connect AVRCP channel */
typedef struct
{
        /*! Task requesting the connection. */
    Task    client_task;
        /*! The time to wait in milli-seconds before connecting AVRCP,
            if the AVRCP is not created by another means. */
    uint32  delay_ms;
} AV_INTERNAL_AVRCP_CONNECT_REQ_T;

/*! Internal request message to disconnect AVRCP channel */
typedef struct
{
    Task    client_task;    /*!< Task requesting the disconnect.  */
} AV_INTERNAL_AVRCP_DISCONNECT_REQ_T;

/*! Internal message returning the response to a 
    \ref AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_REQ */
typedef struct
{
    avrcp_response_type response;       /*!< Response code */
} AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_RES_T;

/*! Internal message to pass a vendor command over AVRCP.
    Used for TWS signalling to both peer and handset */
typedef struct
{
    Task client_task;           /*!< Task to receive response messages */
    avc_operation_id op_id;     /*!< ID to be sent */
    uint16 size_payload;        /*!< Number of octets in the payload, 0 allowed */
    uint8 payload[1];           /*!< Start of command payload. Message is variable length */
} AV_INTERNAL_AVRCP_VENDOR_PASSTHROUGH_REQ_T;

/*! Internal message to initiate a remote control request, possibly repeating. */
typedef struct
{
    avc_operation_id op_id; /*!< Operation ID */
    uint8 state;            /*!< Button press or release */
    unsigned ui:1;          /*!< Flag when set indicates tone should be played */
    uint16 repeat_ms;       /*!< Period between repeats (0 for none) */
} AV_INTERNAL_AVRCP_REMOTE_REQ_T;

/*! Internal message to trigger a remote control request repeatedly. 
    Same structure as AV_INTERNAL_AVRCP_REMOTE_REQ_T, which is for the first
    request. */
typedef AV_INTERNAL_AVRCP_REMOTE_REQ_T AV_INTERNAL_AVRCP_REMOTE_REPEAT_REQ_T;

/*! Volume notification request */
typedef struct
{
    uint8 volume;   /*!< Volume level 0-127 */
} AV_INTERNAL_AVRCP_VOLUME_NOTIFY_REQ_T;


/*! Internal message communicating play status from other AVRCP instance */
typedef struct
{
    avrcp_play_status play_status;      /*!< The play status */
} AV_INTERNAL_PLAYBACK_STATUS_UPDATE_IND_T;

/*! \brief Get the volume for a specific AV instance */
#define appAvVolumeGet(theInst) \
    ((theInst)->a2dp_volume)

/*! \brief Get Sink for AV instance */
#define appAvGetSink(theInst) \
    (A2dpSignallingGetSink((theInst)->a2dp_device_id))

/*! \brief Check if a AV instance has same Bluetooth Address */
#define appAvIsBdAddr(bd_addr) \
    (appAvInstanceFindFromBdAddr((bd_addr)) != NULL)


/*! A2DP connect/disconnect flags */
typedef enum
{
    A2DP_CONNECT_NOFLAGS      = 0,         /*!< Empty flags set */
    A2DP_CONNECT_SILENT_ERROR = (1 << 0),  /*!< Don't indicate connection error */
    A2DP_CONNECT_SILENT       = (1 << 1),  /*!< Don't indicate connection success */
    A2DP_CONNECT_MEDIA        = (1 << 3),  /*!< Connect media channel automatically */
    A2DP_DISCONNECT_SILENT    = (1 << 4),  /*!< Don't indicate disconnection */
} appAvA2dpConnectFlags;

extern void appAvInit(void);

extern bool appAvA2dpConnectRequest(const bdaddr *bd_addr, appAvA2dpConnectFlags a2dp_flags);
extern bool appAvAvrcpConnectLaterRequest(Task client_task, const bdaddr *bd_addr, uint32 delay_ms);
/*! Specialisation of appAvAvrcpConnectLaterRequest for connecting AVRCP immediately */
#define appAvAvrcpConnectRequest(client_task, bd_addr) appAvAvrcpConnectLaterRequest(client_task, bd_addr, 0)
extern void appAvAvrcpConnectResponse(Task client_task, const bdaddr *bd_addr, uint16 connection_id, uint16 signal_id, avAvrcpAccept accept);
extern void appAvAvrcpClientRegister(Task client_task, uint8 interests);
extern bool appAvA2dpDisconnectRequest(avInstanceTaskData *av_inst);
/*! \brief Request disconnection of AVRCP for specified client. */
extern bool appAvAvrcpDisconnectRequest(Task client_task, avInstanceTaskData* av_inst);


extern bool appAvConnectPeer(void);
extern bool appAvDisconnectPeer(void);
extern bool appAvConnectHandset(void);
extern bool appAvDisconnectHandset(void);
extern bool appAvConnectWithBdAddr(const bdaddr *bd_addr);

extern void appAvStatusClientRegister(Task client_task);

extern void appAvStreamingSuspend(avSuspendReason reason);
extern void appAvStreamingResume(avSuspendReason reason);

extern void appAvVolumeSet(uint16 volume, avInstanceTaskData *from_inst);
extern void appAvVolumeSync(avInstanceTaskData *from_inst, bool on_connect);
extern void appAvVolumeStart(int16 step);
extern void appAvVolumeStop(int16 step);
extern bool appAvVolumeRepeat(int16 step);
extern bool appAvVolumeChange(int16 step);

extern void appAvPause(bool ui);
extern void appAvPlay(bool ui);
extern void appAvPlayToggle(bool ui);
extern void appAvStop(bool ui);
extern void appAvForward(void);
extern void appAvBackward(void);
extern void appAvFastForwardStart(void);
extern void appAvFastForwardStop(void);
extern void appAvRewindStart(void);
extern void appAvRewindStop(void);

extern void appAvVolumeUpRemoteStart(void);
extern void appAvVolumeUpRemoteStop(void);
extern void appAvVolumeDownRemoteStart(void);
extern void appAvVolumeDownRemoteStop(void);

extern avInstanceTaskData *appAvInstanceCreate(const bdaddr *bd_addr);
extern void appAvInstanceDestroy(avInstanceTaskData *theInst);

extern avInstanceTaskData *appAvGetA2dpSink(avCodecType codec_type);
extern avInstanceTaskData *appAvGetA2dpSource(void);
extern avInstanceTaskData *appAvInstanceFindFromBdAddr(const bdaddr *bd_addr);
extern avInstanceTaskData *appAvInstanceFindA2dpState(const avInstanceTaskData *theInst, uint8 mask, uint8 expected);
extern avInstanceTaskData *appAvInstanceFindAvrcpForPassthrough(void);
extern avInstanceTaskData *appAvInstanceFindAvrcpFromBdAddr(const bdaddr *bd_addr);
extern avInstanceTaskData *appAvInstanceFindAvrcpOther(avInstanceTaskData *theInst);

extern bool appAvHasAConnection(void);
extern bool appAvIsConnectedMaster(void);
extern bool appAvIsConnectedSlave(void);

extern bool appAvIsDisconnected(void);
extern bool appAvIsStreaming(void);
extern avrcp_play_status appAvPlayStatus(void);
extern void appAvHintPlayStatus(avrcp_play_status status);

bool appAvIsValidInst(avInstanceTaskData* theInst);

extern bool appAvIsAvrcpConnected(avInstanceTaskData* theInst);

#endif

#endif
