/****************************************************************************
Copyright (c) 2015 - 2017 Qualcomm Technologies International, Ltd.
  Part of 6.2

*/

#ifndef NFC_PRIM_H
#define NFC_PRIM_H

#ifdef __cplusplus
extern "C" {
#endif
/* PROJECT INCLUDES **********************************************************/

#ifdef SUBSYSTEM_APPS
#include "hydra/hydra_types.h"
#else
#include <hydra_types.h>
#endif

#include "nfc/nfc_prim_defs.h"

/* PUBLIC MACROS ************************************************************/
#define VAR_ARR (1)

/* PUBLIC TYPES DEFINITIONS *************************************************/

/* @brief the NFC message base
   @warning this base should not clash with other base values in defined
   vm/libs/library/library.h if the handler is main sink app task.*/
#define NFC_MESSAGE_BASE      (0x7E00)

#ifdef SUBSYSTEM_APPS
/* @brief NFC CL base. It must be defined in vm/libs/library/library.h for ADK
   build and here for appss P1 builds */
#define NFC_CL_MESSAGE_BASE   (0x7E40)
#endif /* SUBSYSTEM_APPS */

#define NFC_MESSAGE_ID_MAX    (0x00FF)
#define NFC_MESSAGE_ID_MASK   (0x00FF)
#define NFC_MESSAGE_BASE_MASK (0xFF00)

typedef enum nfc_vm_status_enum
{
    NFC_VM_SUCCESS, /*!< Request was successful */
    NFC_VM_FAIL, /*!< Request has failed */
    NFC_VM_WRONG_NDEF_FORMAT, /*!< Wrong NDEF format used in request */
    NFC_VM_WRONG_ARGS, /*!< Wrong argument used in the request */
    NFC_VM_WRONG_HAL_STATE, /*!< Wrong NFC HAL to process the request */
    NFC_VM_SAP_DISABLED, /*!< SAP disabled - The request is not available */
    NFC_VM_UNSUPPORTED, /*!< Feature not supported */
    NFC_VM_DISABLED, /*!< NFC HW has been disabled via MIB */
    NFC_VM_WRONG_STATE, /*!< NFC Configuration not completed  */
    NFC_VM_NO_HW, /*!< NO NFC hardware present (e.g. e-fuse has been used) */
    /* CH SAP **********************/
    /*!< Module is performing a Connection Handover procedure and parameters can't
     * be updated for consistency. New parameters will be accepted at the end
     * of the procedure
     */
    NFC_VM_CH_STORE_CARRIER_BUSY,
    /*!< Received carrier configuration is corrupted. This may prevent an
     * ongoing procedure to be completed
     */
    NFC_VM_CH_CARRIER_CORRUPTED,
    /*!< None of the received carriers match stored configuration */
    NFC_VM_CH_CARRIER_MISMATCH,

    /* NFC Internal ****************/
    /*!< Last value available on P1 app */
    NFC_VM_LAST_STATUS = 0xFF,
    NFC_VM_CH_MODULE_BUSY
} NFC_VM_STATUS;

/*!< Message IDs for the NFC subsystem API. The IDs are meant to be unique to
 * the application and should not overlap with other subsystems.
 * Message IDs are set explicitly to their value on purpose. This is to avoid
 * any accidental modifications to the enumeration. The scope of this approach
 * is to maintain the backward compatibility of new versions of the NFC
 * firmware/libraries with previous versions for message IDs
 */
typedef enum nfc_vm_msg_id_enum
{
    /* CTRL SAP **********************/
    NFC_CTRL_RESET_REQ_ID            =  NFC_MESSAGE_BASE,   /*!< Reset NFC susbsystem */
    NFC_CTRL_RESET_CNF_ID            = (NFC_MESSAGE_BASE+1), /*!< Reset request result */
    NFC_CTRL_CONFIG_REQ_ID           = (NFC_MESSAGE_BASE+2), /*!< Configuration of NFC susbsystem */
    NFC_CTRL_CONFIG_CNF_ID           = (NFC_MESSAGE_BASE+3), /*!< Config request result */
    NFC_CTRL_CARRIER_ON_IND_ID       = (NFC_MESSAGE_BASE+4), /*!< NFC HW detected the NFC carrier from a NFC reader */
    NFC_CTRL_CARRIER_LOSS_IND_ID     = (NFC_MESSAGE_BASE+5), /*!< NFC HW has lost the carrier from a NFC reader */
    NFC_CTRL_SELECTED_IND_ID         = (NFC_MESSAGE_BASE+6), /*!< NFC HW target selected by a NFC reader */
    NFC_CTRL_READY_IND_ID            = (NFC_MESSAGE_BASE+7), /*!< NFC HW is up and running (ONLY received ONCE after NFC_CTRL_REGISTER_REQ_ID) */
    NFC_CTRL_REGISTER_REQ_ID         = (NFC_MESSAGE_BASE+8), /*!< Register client for NFC service */

    /* TAG SAP **********************/
    NFC_TAG_READ_UID_REQ_ID          = (NFC_MESSAGE_BASE+10), /*!< Read the current UID */
    NFC_TAG_READ_UID_CNF_ID          = (NFC_MESSAGE_BASE+11), /*!< Read UID request result */
    NFC_TAG_WRITE_UID_REQ_ID         = (NFC_MESSAGE_BASE+12), /*!< Write the TAG UID */
    NFC_TAG_WRITE_UID_CNF_ID         = (NFC_MESSAGE_BASE+13), /*!< Write UID request result */
    NFC_TAG_WRITE_NDEF_REQ_ID        = (NFC_MESSAGE_BASE+14), /*!< Write the data content of the TAG */
    NFC_TAG_WRITE_NDEF_CNF_ID        = (NFC_MESSAGE_BASE+15), /*!< Write NDEF request result */
    NFC_TAG_READ_NDEF_REQ_ID         = (NFC_MESSAGE_BASE+16), /*!< Read the data content of the TAG */
    NFC_TAG_READ_NDEF_CNF_ID         = (NFC_MESSAGE_BASE+17), /*!< Read NDEF request result */
    NFC_TAG_READ_STARTED_IND_ID      = (NFC_MESSAGE_BASE+18), /*!< The tag data content is activily being read by the reader */
    /* Add/Delete a carrier to the tag for an NFC static handover. */
    NFC_TAG_WRITE_CH_CARRIERS_REQ_ID = (NFC_MESSAGE_BASE+19), /*!< Write Static Handover carriers to the tag */
    NFC_TAG_WRITE_CH_CARRIERS_CNF_ID = (NFC_MESSAGE_BASE+20), /*!< Write request result */

/* \cond */
    /* CH SAP **********************/
    NFC_CH_MSG_HANDOVER_COMPLETE_IND = (NFC_MESSAGE_BASE+30), /*!< Negotiated Connection Handover procedure complete */
    NFC_CH_MSG_CARRIER_CONFIG_CNF    = (NFC_MESSAGE_BASE+31), /*!< Carrier data received by CH module */
    NFC_VM_LAST_MSG_ID               = (NFC_MESSAGE_BASE+32)

    /* SNEP SAP **********************/
    /* To be defined */
/* \endcond */
} NFC_VM_MSG_ID;

#define IS_VALID_NFC_VM_MSG_ID(id) ((NFC_MESSAGE_BASE)<=(id) && (id)<(NFC_VM_LAST_MSG_ID))

/* The NFC CL base must be defined in at least one of the application header
 * file.  This is usually done in vm/libs/library/library.h.
 * Please update the following test if the expected value has changed. */

#define IS_VALID_CTRL_REQ(id) \
(NFC_CTRL_RESET_REQ_ID==(id)|| \
 NFC_CTRL_CONFIG_REQ_ID==(id)|| \
 NFC_CTRL_REGISTER_REQ_ID==(id))

#define IS_VALID_TAG_REQ(id) \
(NFC_TAG_READ_UID_REQ_ID==(id) || \
 NFC_TAG_WRITE_UID_REQ_ID==(id) || \
 NFC_TAG_WRITE_NDEF_REQ_ID==(id) || \
 NFC_TAG_READ_NDEF_REQ_ID==(id) || \
 NFC_TAG_WRITE_CH_CARRIERS_REQ_ID==(id))

/* Connection Handover - Technical Specification - NFC ForumTM  CH 1.4 Draft 6
   5.1.1 CPS (Carrier Power State)
   The CPS (Carrier Power State) field is a 2-bit field that indicates the
   carrier power state. Possible values are described in Table 3.

   Table 3: Carrier Power State Values
   -----------------------------------
   Value    Carrier Power State
   0x00     INACTIVE: the carrier is currently off
   0x01     ACTIVE: the carrier is currently on
   0x02     ACTIVATING: the device is in the process of activating the carrier,
            but it is not yet active
   0x03     UNKNOWN: the device is only reachable via the carrier through a
            router, and it does not directly support an interface for the
            carrier.
*/
typedef enum power_state_tag
{
    PS_INACTIVE,   /*!< the carrier is currently off */
    PS_ACTIVE,     /*!< the carrier is currently on */
    PS_ACTIVATING, /*!< the device is in the process of activating the carrier
            but it is not yet active */
    PS_UNKNOWN     /*!< the device is only reachable via the carrier through a
            router, and it does not directly support an interface for the
            carrier. */
} POWER_STATE;

#define IS_VALID_POWER_STATE(id) \
((id)<=PS_UNKNOWN)

/* NDEF Carrier Record identification.

   BT_EP_OOB    Bluetooth Carrier Configuration Record will be identified by
                the mime-type "application/vnd.bluetooth.ep.oob".
   BT_LE_OOB    Bluetooth Low Energy Carrier Configuration Record will be identified by
                the mime-type "application/vnd.bluetooth.le.oob".
   WFA_WSC      Wifi carrier Configuration Record will be identified by
                the mime-type "application/vnd.wfa.wsc".
   WFA_P2P      WiFi P2P Device Configuration Record will be identified by
                use the mime-type "application/vnd.wfa.p2p".
   */
typedef enum carrier_technology_tag
{
    BT_EP_OOB = 20, /*!< Bluetooth Carrier Configuration Record */
    BT_LE_OOB = 21, /*!< Bluetooth Low Energy Carrier Configuration Record */
    WFA_WSC   = 22, /*!< Wifi carrier Configuration record */
    WFA_P2P   = 23  /*!< WiFi P2P Device Configuration Record */
} CARRIER_TECH;

/* Carrier header type for channel handover */
typedef struct nfc_ch_carrier_header_struct
{
    uint8 cps;            /*!< Carrier power state */
    uint8 carrier_tech;   /*!< Carrier technology type */
    uint8 data_length[2]; /*!< Carrier data length */
} NFC_CH_CARRIER_HEADER;

/* This type contains the data information associated to a particular carrier */
typedef struct nfc_ch_carrier_struct
{
    NFC_CH_CARRIER_HEADER carrier_header; /*!< Carrier header information */
    uint8 data[VAR_ARR]; /*!<  Out of band handover data */
}NFC_CH_CARRIER;

#define IS_VALID_CARRIER_TECH(id) \
(BT_EP_OOB<=(id)&&(id)<=WFA_P2P)

/* Generic structure that return the status of an NFC command */
typedef struct nfc_vm_status_cnf_struct
{
    NFC_VM_STATUS status;
} NFC_VM_STATUS_CNF;

/* NFC CTRL SAP **************************************************************/
typedef enum nfc_vm_mode_enum
{
    NFC_VM_NONE = 0, /*!< HW is not configured and is disabled */
    NFC_VM_P2P  = 1, /*!< NOT available on P1 - NFC HW in Peer to Peer configuration */
    NFC_VM_TT2  = 2, /*!< NFC HW in TAG Type 2 configuration */
    NFC_VM_DTA  = 3, /*!< NOT available on P1 - NFC forum qualification mode */
    NFC_VM_TEST = 4  /*!< NOT available on P1 - Configure for internal testing */
} NFC_VM_MODE;
#define IS_VALID_NFC_VM_MODE(mode) \
    ((mode)<=NFC_VM_TT2)

/* LLCP configuration type of NFC module, P2P configuration */
typedef enum nfc_vm_service_type_enum
{
    NFC_VM_LLCP_NONE = 0,   /*!< LLCP service disabled  */
    NFC_VM_LLCP_SERVER = 1, /*!< LLCP service, server configuration  */
    NFC_VM_LLCP_CLIENT = 2, /*!< LLCP service, client configuration  */
    NFC_VM_LLCP_SERVER_CLIENT = 3 /*!< LLCP service, both server and client configuration  */
} NFC_VM_LLCP_SERVICE_TYPE;

typedef struct nfc_ctrl_config_struct
{
    NFC_VM_MODE mode; /*!< NFC control configuration mode */
    NFC_VM_LLCP_SERVICE_TYPE ch_service; /*!< P2P Mode ONLY NFC - negotiated CH configuration */
    NFC_VM_LLCP_SERVICE_TYPE snep_service; /*!< P2P Mode ONLY - NFC SNEP configuration */
} NFC_CTRL_CONFIG;

/* NFC_CTRL_RESET
 *
 * NFC_CTRL_RESET_REQ is used to reset the NFC hardware and release it resources.
 * NFC_CTRL_RESET_CNF is used to confirm the processing of the NFC_CTRL_RESET_REQ.
 */
typedef NFC_VM_STATUS_CNF NFC_CTRL_RESET_CNF;

/* NFC_CTRL_CONFIG
 *
 * NFC_CTRL_CONFIG_REQ is used to configure the NFC Hardware.
 *
 * NFC_CTRL_CONFIG_CNF confirms that the NFC hardware is up and running, and
 * indicates the selected configuration - TT2 or PP2.
 */
typedef NFC_CTRL_CONFIG NFC_CTRL_CONFIG_REQ;
typedef NFC_VM_STATUS_CNF NFC_CTRL_CONFIG_CNF;

/* NFC_CTRL_CARRIER_ON
 *
 * NFC_CTRL_CARRIER_ON_IND is used to indicate that the NFC hardware is
 * receiving the NFC carrier from an NFC reader.
<p><tt>
\code{.unparsed}
 *      [ NFC Phone and NFC targets are brought together]
 *
 * +--------------+                          +-------+
 * |  NFC Target  |<------- Phone Carrier -- | Phone |
 * +--------------+                          +-------+
\endcode
</tt></p>
 */

/* NFC_CTRL_CARRIER_LOSS
 *
 * NFC_CTRL_CARRIER_LOSS_IND is used to indicate that the NFC hardware has
 * lost the carrier from a NFC reader.
<p><tt>
\code{.unparsed}
 *
 *      [ NFC Phone and NFC target move apart]
 *
 * +--------------+                               +-------+
 * |  NFC Target  |  TOO FAR  <- Phone Carrier -- | Phone |
 * +--------------+                               +-------+
\endcode
</tt></p>
 */

/* NFC_CTRL_SELECTED
 *
 * NFC_CTRL_SELECTED_IND is used to indicate that the NFC hardware has been
 * selected as a target.  This can be the TAG target or the P2P target.
 *
<p><tt>
\code{.unparsed}
 * +--------------+                      +-------+
 * |  NFC Target  |  <- Communicating -> |       |
 * +--------------+                      +-------+
 *
\endcode
</tt></p>
 *  see full sequence below for NFC_TAG_READ_STARTED_IND
 */
typedef struct nfc_ctrl_selected_ind_struct
{
    NRC_SERVICE_TYPE service_type;
} NFC_CTRL_SELECTED_IND;


/* NFC_CTRL_READY
 *
 * Indicates that the NFC hardware is ready to be used and the current running
 * configuration.
 * One indication is sent from P0 for each NFC_CTRL_REGISTER_REQ received on P0.
 *<p><tt>
\code{.unparsed}
 * +------+                    +-------+
 * |  P1  |                    |  P0   |
 * +------+                    +-------+
 *    |                            |
 *    +----NFC_CTR_REGISTER_REQ--->|
 *    |<---NFC_CTRL_READY_IND------+
 *
\endcode
</tt></p>
 *
 * The current running configuration can be NONE or CONFIGURED (e.g. TT2/P2P).
 * The status will be SUCCESS in these cases.
 *
 * If the NFC hardware has been disabled (e.g. using MIB or E-FUSE) and application
 * tries to register for NFC services then the status will be set to the
 * corresponding error (NFC_VM_DISABLED or NFC_VM_NO_HW)
 */
typedef struct nfc_ctrl_ready_ind_struct
{
    NFC_CTRL_CONFIG current_config;
    NFC_VM_STATUS   status;
    bool is_tt2_empty; /*!< TT2 Mode ONLY - Is the TT2 TAG memory empty (Read ONLY)  */
} NFC_CTRL_READY_IND;

/* NFC TAG MGR SAP ***********************************************************/

/* NFC_TAG_READ_UID
 *
 * The NFC_TAG_READ_UID_REQ is used to read the NFC TAG Unique Identifier.
 * NFC_TAG_READ_UID_CNF confirms the processing of NFC_TAG_READ_UID_REQ.
 */
typedef struct nfc_tag_read_uid_cnf_struct
{
    NFC_VM_STATUS status;
    uint8 nfcid_size; /* valid values 0,4,7,10 */
    uint8 nfcid[MAX_NFCID_SIZE];
} NFC_TAG_READ_UID_CNF;

/* NFC_TAG_WRITE_UID
 *
 * The NFC_TAG_WRITE_UID_REQ is used to write the NFC TAG Unique Identifier.
 * NFC_TAG_WRITE_UID_CNF confirms the processing of NFC_TAG_WRITE_UID_REQ.
 *
 * A valid NFC UID length can be either 4bytes (SINGLE), 7bytes (DOUBLE) or
 * 10bytes (TRIPLE).
 *
 * UID Requirements
 *******************
 *
 * [Ref ISO-1444-3] The UID consists of 4, 7 or 10 UID bytes. Consequently,
 * the PICC shall handle up to 3 cascade levels to get all UID bytes.
 * Within each cascade level, a part of UID consisting of 5 data bytes shall
 * be transmitted to the PCD. According to the maximum cascade level, three
 * types of UID size are defined.
 *
 * Single size UIDs
 * ================
 * [Ref NFCForum-TS-DigitalProtocol-1.1 Table 16: nfcid1-0 for Single-size NFCID1]
 * nfcid1-0             Description
 * ---------------------------------------------------------------------------------
 * 08h                  nfcid1-1 to nfcid1-3 are dynamically generated
 *
 *                      PLEASE NOTE THAT DYNAMICALLY GENERATED UID ARE NOT SUPPORTED
 *                      IN THIS SOFTWARE RELEASE.
 * ---------------------------------------------------------------------------------
 * x0h to x7h           Fixed diversified number (the assignment is out of
 * x9h to xEh           scope of this specification)
 * 18h, 28h, 38h, 48h,
 * 58h, 68h, 78h, 98h,
 * A8h, B8h, C8h, D8h,
 * E8h
 * ---------------------------------------------------------------------------------
 * xFh                  Fixed-number, non-unique
 * ---------------------------------------------------------------------------------
 * F8h                  RFU
 *
 * [Ref NFCForum-TS-DigitalProtocol-1.1]
 * Requirements 26: Handling of NFCID1-0 values for Single-size NFCID1
 *
 * 6.7.2.5 - The NFC Forum Device SHALL set nfcid1-0 of a single-size
 * NFCID1 and nfcid1-3 of a double-size NFCID1 to a value different
 * from 88h (The Cascade Tag)
 *
 * Double and triple size UIDs
 * ===========================
 * [Ref ISO-1444-3]
 *    UID0  - Manufacturer ID according to ISO/IEC 7816-6/AM1
 *    Each manufacturer is responsible for the uniqueness of the value of
 *    the other bytes of the unique number.
 *
 *    PLEASE NOTE THAT IT IS RECOMMENDED TO SET NFCID1-0 TO 0 TO INDICATE
 *    A GENERIC MANUFACTURER.
 *
 * The values '81' to 'FE', which are marked for 'Private use' in
 * ISO/IEC 7816-6/AM1 shall not be allowed in this context.
 *
 */
typedef struct nfc_tag_write_uid_req_struct
{
    uint8 nfcid_size; /* valid values  - 0,4,7,10 */
    uint8 nfcid[MAX_NFCID_SIZE];
} NFC_TAG_WRITE_UID_REQ;
typedef NFC_VM_STATUS_CNF NFC_TAG_WRITE_UID_CNF;


/* Tag Type 2 MAX_CARRIERS_SIZE Calculation Details

 Only a single carrier is supported for this API release.

 1) Calculate the space available on the NFC TT2.
    TT2_MAX_NDEF=TT2_MAX_MEMORY_LIMIT-4BLOCK-TLV: 256-16-2=238 bytes

    IOP shows that various phone stacks expect the last 3bytes to be used.
    One of these bytes must be a terminator TT2_MAX_NDEF=235

 2) Calculate space required for the BT static handover select headers

    Record 1 * Hdr-size=15 bytes ***************************
    0x91, 0x02, 0x0a,
    0x48, 0x73, -------Hs
    0x12, 0xd1, 0x02,
    0x04,
    0x61, 0x63, -------ac
    0x01, 0x01,
    0x31, -------------ID
    0x00,

    Record 2 * Hdr-Size=37 bytes ***************************
    0x5a,
    0x20,
    0x28, -------------BT Payload length
    0x01, 0x61, 0x70, 0x70, 0x6c, 0x69, -------------6bytes
    0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x76, -8bytes
    0x6e, 0x64, 0x2e, 0x62, 0x6c, 0x75, 0x65, 0x74, -8bytes
    0x6f, 0x6f, 0x74, 0x68, 0x2e, 0x65, 0x70, 0x2e, -8bytes
    0x6f, 0x6f, 0x62,
    0x31, -------------ID

    1+1+1+(6+3x8+3)+1=52 bytes

    <-- SPACE AVAILABLE FOR THE CARRIER DATA -->

  3) 235-52= 183 bytes
*/
#define MAX_CARRIERS_SIZE (183)
/* NOTE: the number of carriers is limited to one in this version of the API */

/* NFC_TAG_WRITE_CH_CARRIERS
 *
 * NFC_TAG_WRITE_CH_CARRIERS_REQ is used to add alternative carriers to the
 * NDEF message stored on the NFC TAG.
 *
 * NFC_TAG_WRITE_CH_CARRIERS_CNF confirms the completion of NFC_TAG_WRITE_CH_CARRIERS
 * processing.
 */
typedef struct nfc_tag_write_ch_carriers_req_struct
{
    uint8 n_carriers;
    NFC_CH_CARRIER **carriers;
} NFC_TAG_WRITE_CH_CARRIERS_REQ;

typedef NFC_VM_STATUS_CNF NFC_TAG_WRITE_CH_CARRIERS_CNF;

/* NFC_TAG_WRITE_NDEF
 *
 * NFC_TAG_WRITE_NDEF_REQ is used to write the NDEF message on the NFC TAG.
 * The NDEF message must be compliant to the NFC Forum NDEF specification -
 * NFCForum-TS-NDEF_1.0
 *
 * NFC_TAG_WRITE_NDEF_CNF confirms the processing of NFC_TAG_WRITE_NDEF_REQ.
 */
typedef struct nfc_tag_write_ndef_req_struct
{
    uint8 ndef_size;
    uint8 ndef[VAR_ARR];
} NFC_TAG_WRITE_NDEF_REQ;
typedef NFC_VM_STATUS_CNF NFC_TAG_WRITE_NDEF_CNF;

/* NFC_TAG_READ_NDEF
 *
 * NFC_TAG_READ_NDEF_REQ is used to read the NDEF message on the NFC TAG.
 * It is only for debugging purpose.
 * NFC_TAG_READ_NDEF_CNF returns the NDEF message.
 */
typedef struct nfc_tag_read_ndef_cnf_struct
{
    NFC_VM_STATUS status;
    uint8 ndef_size;
    uint8 ndef[VAR_ARR];
} NFC_TAG_READ_NDEF_CNF;

/* NFC_TAG_READ_STARTED_IND
 *
 *
 * Typical reading sequence of a Tag.
 *
 * 1 - RF Field On
 * 2 - Anti-collision Algorithm
 * 3 - Tag selection/activation
 * 4 - Tag reading sequence
 * 5 - RF Field Off
 *
 * These indications are useful to solve false detection of a NFC reader
 * (such as NFC phone) during the NFC handover.
 *
 * The NFC receiver can detect the RF field from the NFC reader earlier than
 * the reader can detect the reply from the tag. Moreover the RF indication
 * only indicates that a reader is close by, but doesn't necessarily mean that
 * the reader is interested to read the Tag and perform an NFC handover.
 *
 * The other problem occurs when the phone is waved too quickly to read the Tag.
 * This sometimes give sufficient time to activate the Tag, but not enough time
 * to completely read the tag. I.e. the phone is missing the necessary information
 * to connect.
 *
 * This could impact the user application and degrade the user experience.
 *
 * The following primitives are introduced to improve the user experience in
 * the above scenarios.
 *
 * 1) Tap-to-pair or tap-to-play scenario.
 *
 * NFC_CTRL_SELECTED_IND(TT2) -  It indicates that the reader detected the TAG
 * and selected it. The sub-systems for the alternative carriers (BT/WiFi)
 * should be informed that they are likely to receive a connection request
 * soon.
 *
 * NFC_TAG_READ_STARTED_IND - It indicates that the reader activated the Tag
 * and is actively reading it. When this primitive is received it is MORE
 * likely that the phone (reader) is trying for a handover. This indication
 * could be used by the user application to prepare the device to play the
 * music.
 *
 * 2) Tap-to-stop
 *
 * NFC_CTRL_SELECTED_IND(TT2) -  It indicates that the phone (reader) detected the
 * TAG and activated it. If the device is currently connected on the alternative
 * carrier, it is likely to receive a request such as 'stop music' from the phone.
 *
 * NFC_TAG_READ_STARTED_IND - It indicates that the phone (reader) is actively
 * reading the content of the tag. In this case, the primitive can be used to
 * signal the user application that it is MORE likely to receive a request such
 * as 'stop music' from the phone.
 */

/* NFC SNEP SAP **************************************************************/
/* This SAP is not exposed in the current API */

/* NFC CH SAP ****************************************************************/

/* NFC_MAX_NOF_CARRIER
 *
 * NFC_MAX_NOF_CARRIER defines the maximum number of carriers that we expect to
 * exchange with P0. This limits both the number of carriers in the carrier
 * configuration message and the number of carriers that can be received from
 * the phone
 */
#define NFC_MAX_NOF_CARRIER     (8)

/* Define this object's header size */
#define NFC_STREAM_CONTENT_SIZE (4 * sizeof(uint8))
/* This type describes the NFC stream content. The starting point of the
 * consequent carriers is not known as the carrier data length is not fixed
 */
typedef struct nfc_stream_content_struct
{
    /* Number of carriers carried in the message */
    uint8 n_carriers;
    /* 32 bit alignment */
    uint8 rfu[3];
    /* Carrier information array */
    NFC_CH_CARRIER carrier_info[VAR_ARR];
} NFC_STREAM_CONTENT;

/*
 * NOT AVAILABLE - to be enabled in the future.
 *
 * IMPORTANT NOTE: the negotiated handover is used for
 * transferring pictures on Android, blackberry and Windows phones at the time
 * of writing this document (02/10/2015).
 *
 * The market may develop some new use cases relevant to this product. Software
 * upgrade will be possible.
 *
 * The feature uses two primitives: NFC_CH_MSG_HANDOVER_COMPLETE_IND and
 * NFC_CH_COMPLETE_IND.
 *
 * The negotiated handover can work as a client, server or both.
 */

/* NFC_CH_HANDOVER_COMPLETE_IND
 *
 * NFC_CH_HANDOVER_IND is used to signal the P1 the completion of the data
 * send procedure with the Negotiated Connection Handover module
 */
typedef struct nfc_ch_handover_complete_ind_struct
{
    /* Procedure result */
    NFC_VM_STATUS status;
} NFC_CH_HANDOVER_COMPLETE_IND;

/* NFC_CH_CARRIER_CONFIG_CNF
 *
 * NFC_CH_CARRIER_CONFIG_CNF is used to signal the P1 the result of the carrier
 * storing procedure
 */
typedef struct nfc_ch_carrier_config_cnf_struct
{
    /* Procedure result */
    NFC_VM_STATUS status;
} NFC_CH_CARRIER_CONFIG_CNF;

/** @}*/

/* NFC_CH_MSG_RESET_REQ
 *
 * INTERNAL API - Not available to user application on APPS P1
 *
 * NFC_CH_MSG_RESET_CNF is returned on completing the NFC_CH_MSG_RESET_REQ.
 */

/* NFC stream SAP ************************************************************/

/* NFC_STREAM_HANDOVER_TYPE identifies the handover record type for Connection
 * Handover module
 */
typedef enum nfc_stream_handover_type_enum
{
    /* Stream carries an handover request record */
    nfc_stream_handover_request,
    /* Stream carries an handover select record */
    nfc_stream_handover_select,
    /* Stream carries a configuration message */
    nfc_stream_handover_config
} NFC_STREAM_HANDOVER_TYPE;

/* NFC_STREAM_TYPE_ID identifies the type of data carried in the stream */
typedef enum nfc_stream_type_enum
{
    /* The attached data is to be handled by the CH */
    nfc_stream_type_ch
} NFC_STREAM_TYPE_ID;

/* NFC_STREAM_CH contains information about the stream data referred to the
 * Connection Handover
 */
typedef struct nfc_stream_ch_struct
{
    NFC_STREAM_HANDOVER_TYPE handover_record_type;
} NFC_STREAM_CH;

/** @{*/
/* NFC PRIMITIVES ************************************************************/
typedef struct nfc_prim_msg_struct NFC_PRIM;

struct nfc_prim_msg_struct
{
    uint16  id;  /* message type */
    union
    {
     /* NFC_CTRL_RESET_REQ - No arguments */
        NFC_CTRL_RESET_CNF  reset_cnf;
        NFC_CTRL_CONFIG_REQ config_req;
        NFC_CTRL_CONFIG_CNF config_cnf;
     /* NFC_CTRL_CARRIER_ON_IND - No arguments */
     /* NFC_CTRL_CARRIER_LOSS_IND - No arguments */
        NFC_CTRL_SELECTED_IND selected_ind;
        NFC_CTRL_READY_IND ready_ind;

        NFC_TAG_WRITE_NDEF_REQ write_ndef_req;
        NFC_TAG_WRITE_NDEF_CNF write_ndef_cnf;
     /* NFC_TAG_READ_NDEF_REQ - No arguments */
        NFC_TAG_READ_NDEF_CNF read_ndef_cnf;
     /* NFC_TAG_READ_UID_REQ - No arguments */
        NFC_TAG_READ_UID_CNF read_uid_cnf;
        NFC_TAG_WRITE_UID_REQ write_uid_req;
        NFC_TAG_WRITE_UID_CNF write_uid_cnf;
     /* NFC_TAG_READ_STARTED_IND - No arguments */
        NFC_TAG_WRITE_CH_CARRIERS_REQ write_carriers_req;
        NFC_TAG_WRITE_CH_CARRIERS_CNF write_carriers_cnf;

        NFC_CH_HANDOVER_COMPLETE_IND ch_handover_complete_ind;
        NFC_CH_CARRIER_CONFIG_CNF ch_carrier_config_cnf;
    } m;
};
/** @}*/

/* NFC STREAM PRIMITIVES *****************************************************/
/* NFC_STREAM_INF type is associated to the NFC stream and carries information
 * about the type of data contained in the stream
 */
typedef struct nfc_stream_inf_struct NFC_STREAM_INF;

struct nfc_stream_inf_struct
{
    /* Identify the stream type */
    NFC_STREAM_TYPE_ID stream_id;
    union
    {
        NFC_STREAM_CH nfc_ch_info;
    } m;
};

#ifdef __cplusplus
#endif

#endif /* NFC_PRIM_H */
