/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */

#ifndef ISP_ROUTER_H
#define ISP_ROUTER_H

#include "hydra/hydra_types.h"
#include "gen/xap/isp_prim.h"
#include "mmu/mmu.h"
#include "utils/utils.h"
#ifdef SUBSYSTEM_CURATOR
#include "buffer/buffer.h"
#endif

/** The address range that indicates a command comes from or a response is
 * destined for a hostmsg link (through the curator).
 */
#define ADDRESS_IS_ROUTED_VIA_CURATOR(addr)      \
                    ((addr >= ISP_ADDR_HOST) && (addr < ISP_ADDR_TRB_HOST))

/************************************************************************/

/** Strings identifying protocol codes for use in DBG_MSG macros
 * Use as DBG_MSG1("protocol %s", (DBG_STR)isp_protocol_strings[protocol]);
 */
extern const char * isp_protocol_strings[];

/************************************************************************/
/** Forward declaration for the callback function declarations */
struct isp_ss_port_cntx;

/** Callback function prototype for link extablishment/destruction. When a
 * link has been requested by a call to \c isp_router_create_port(),
 * \c isp_router_create_out_band(), \c isp_router_create_host_channel()
 * or any of the macro equivalents, this callback will indicate when the
 * link can be used, or when it has been disconnected (by either end).
 * \param context - a pointer to the context of the link. The macros
 * \c isp_get_src_addr_from_context() and \c isp_get_port_from_context() can
 * be used to get the link details from this.
 * \param connected - TRUE indicates the link has been connected and may be
 * used. FALSE indicates that the link has been disconnected. If this is the
 * first side of a link then the first callback may be received with
 * \c connected = FALSE if something on this subsystem has requested a reset
 * of all links.
 */
typedef void (*link_establish_callback)(struct isp_ss_port_cntx * context,
                                        bool connected);

/** Callback function prototype for the in-out-band data receive
 * It is called to pass the pdu from the remote sub-system.
 * \param context - pointer to the isp context. The client can use accessor
 *              macros to get the connected sub-system or port from this.
 * \param src - the original source address of the pdu. Not necessarily the
 *              same as the subsystem this link is connected to.
 * \param pdu - the pdu itself
 * \param pdu_len_words - length of the pdu
 * \return The callback should return TRUE if it has consumed the data.
 */
typedef bool (*data_callback)(struct isp_ss_port_cntx * context,
                              uint16 src,
                              const uint16 * pdu,
                              uint16 pdu_len_words);

/** Callback function prototype for the out-band data report
 * It is called to indicate the data placed in the mmu by the curator.
 * The callback should return TRUE if it has consumed the data.
 * \param context The ISP context of the link this callback is for
 * \param src The source address of the subsystem sending this data
 * \param mmu MMU handle containing the data
 * \param len_octets Length of data added to the MMU
 * \return TRUE if the data was consumed.
 */
typedef bool (*data_report_callback)(struct isp_ss_port_cntx * context,
                                        uint16 src,
                                        mmu_handle mmu,
                                        uint16 len_octets);

/************************************************************************/

/** Connection states for connecting and disconnecting links */
typedef enum connect_state
{
    /** Initial state of the link */
    CON_STATE_UNINTIALISED,
    /** First side created rx with activate=false */
    CON_STATE_WAIT_TO_ACT_TX,
    /** Second side waiting for either connection */
    CON_STATE_WAIT_FOR_CONNECT,
    /** Out-band uni-directional link */
    CON_STATE_OUT_BAND_WAITING,
    /** Tx has been connected (wait for rx) */
    CON_STATE_TX_CONNECTED,
    /** Rx has been connected (wait for tx) */
    CON_STATE_RX_CONNECTED,
      /** Tx and rx links connected */
    CON_STATE_CONNECTED,
    /** Rx has been disconnected but not tx */
    CON_STATE_RX_DISCONNECTED,
    /** Tx has been disconnected but not rx */
    CON_STATE_TX_DISCONNECTED,
     /** Tx and rx links are disconnected */
    CON_STATE_DISCONNECTED
} connect_state;

/** Enumeration for discriminating between uni and bi-directional links
 * Uni-directional links are assumed to be out-band.
 */
typedef enum isp_link_type
{
    ISP_LINK_TX_RX,
    ISP_LINK_TX_ONLY,
    ISP_LINK_RX_ONLY,
    ISP_LINK_OUT_BAND_TX_RX
} isp_link_type;

/**
 * Convenience for referring to ports for clarity.
 */
typedef uint16 isp_port_num;

/** Private structure for holding the context of a link
 * It is exposed here so that calling modules can allocate the memory for it
 * or embed it within their own link context structures. It is passed as a
 * parameter to the create link and disconnect calls. It is also a
 * parameter passed to the callback functions so the calling modules can
 * dereference it to their own containing structure.
 */
typedef struct isp_ss_port_cntx
{
    utils_SetMember             member;     /** Linked list pointer */
    uint16                      ss_id;      /** Key for this element */
    isp_port_num                port;       /** Key for this element */
    isp_link_type               type;       /** Key for this element */
    link_establish_callback     protocol_link_callback;
    union
    {
        /** Callback for in-out-band links to pass pdu */
        data_callback           protocol_data_callback;

        /** Callback for out-band links to pass report of data received */
        data_report_callback    protocol_data_report_callback;
    } callback;
    union
    {
        /** Buffer size for TX buffer if needed later (for in-out-band links) */
        uint16 size_octets;
        /** Buffer for transmit data needed for second stage of bi-directional
         * out-band link creation (type == ISP_LINK_OUT_BAND_TX_RX).
         */
        mmu_handle  handle;
    } tx_buf;
    connect_state               state;
} isp_ss_port_cntx;

/** Accessor macro for clients to get the source address field value for a link
 * from the private link context structure */
#define isp_get_src_addr_from_context(context)      ((context)->ss_id)

/** Accessor macro for clients to get the port field value for a link
 * from the private link context structure */
#define isp_get_port_from_context(context)          ((context)->port)

/** Accessor macro for clients to get the mmu handle for an out-band  link
 * from the private link context structure */
#define isp_get_mmu_from_context(context)           ((context)->tx_buf.handle)

/************************************************************************/
/** Interface for setting up a bi-directional isp link between sub-systems
 * using an in-out-band submsg link.  This function assumes buffers
 * are to be created symmetrically, with the buffer for receiving each
 * unidirectional link created locally.  For more control, see
 * \c isp_router_create_port_tx_buf().
 *
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param first - TRUE if this is the first side to call the setup function
 *              (usually TRUE on the curator and FALSE on a sub-system).
 * \param ssid - sub-system id of the other side of the link
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param buf_size_octets - a buffer of this size will be created for the
 *      link. Zero may be specified if sharing the link with another protocol
 *      e.g. ftp can do this because ccp has already created a buffer.
 * \param link_callback - a void function taking a context pointer and a
 *      boolean that indicates if the link has been created or destroyed.
 *      May be NULL if not required.
 * \param rx_callback - a function called to pass the received pdu to. Not used
 *      for platform protocol default handlers like ccp and ftp in which case
 *      it should be NULL.
 */
void isp_router_create_port(isp_ss_port_cntx * context,
                            bool first,
                            uint16 ssid,
                            uint16 protocol,
                            uint16 buf_size_octets,
                            link_establish_callback link_callback,
                            data_callback rx_callback);

/** Interface for setting up a bi-directional isp link between sub-systems
 * using an in-out-band submsg link.  This differs from \c
 * isp_router_create_port() in that allows you to specify buffers
 * for for both tx and rx.  This may be used for asymmetric buffer
 * placement, with both tx and rx buffers in one subsystem; note
 * that both sides must call \c isp_router_create_port_tx_buf()
 * in a consistent fashion (exactly one side supplying the tx
 * and one the rx buffer) as there is no negotiation at this level.
 *
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param first - TRUE if this is the first side to call the setup function
 *              (usually TRUE on the curator and FALSE on a sub-system).
 * \param ssid - sub-system id of the other side of the link
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param rx_buf_size_octets - a buffer of this size will be created for the
 *      rx link. Zero may be specified if sharnig the link with another
 *      protocol e.g. ftp can do this because ccp has already created a
 *      buffer, or if the buffer is to be created remotely.
 * \param tx_buf_size_octets - a buffer of this size will be created for
 *      the tx link.  Zero may be specified if the buffer is to be
 *      created remotely.
 * \param link_callback - a void function taking a boolean that indicates if the
 *      link has been created or destroyed. May be NULL if not required.
 * \param rx_callback - a function called to pass the received pdu to. Not used
 *      for platform protocol default handlers like ccp and ftp in which case
 *      it should be NULL.
 */
void isp_router_create_port_tx_buf(
    isp_ss_port_cntx * context,
    bool first,
    uint16 ssid,
    uint16 protocol,
    uint16 rx_buf_size_octets,
    uint16 tx_buf_size_octets,
    link_establish_callback link_callback,
    data_callback rx_callback);


/** Interface for creating an out-band uni-directional link between
 * two subsystems.
 * Fileserv uses this to provide an mmu on the sub-system that the ftp data is
 * written into by the curator.
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param tx - TRUE for tx side of the link, FALSE for rx
 * \param ssid - sub-system id of the other side of the link
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param callback - Valid only for the Rx end of the link. A function taking
 *      an mmu handle and length of data that indicates the data that has been
 *      placed in the mmu by the curator.
 *      The callback returns a bool that is TRUE if the data has been consumed.
 * \param mmu - the handle of an mmu that has been created for this link
 */
extern void isp_router_create_out_band(isp_ss_port_cntx * context,
                                        bool tx,
                                        uint16 ssid,
                                        uint16 protocol,
                                        data_report_callback callback,
                                        mmu_handle mmu);

/**
 * Interface for creating an out-band bi-directional link between
 * two subsystems. The two subsystems need to coordinate the order of
 * starting the link and have arranged which side is supplying which
 * mmu buffers.
 *
 * \param context Pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param first TRUE for the first subsystem of the pair to call this
 *              function for this particular link. FALSE for the other side.
 * \param ssid Sub-system id of the other side of the link
 * \param protocol ISP protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param rx_mmu The handle of an mmu that has been created for this link or
 *               MMU_NO_HANDLE if the other side is providing the buffer.
 * \param tx_mmu The handle of an mmu that has been created for this link or
 *               MMU_NO_HANDLE if the other side is providing the buffer.
 * \param link_callback Called when the link has connected or disconnected.
 *              May be NULL if no callback is required.
 * \param rx_callback Valid only for the Rx end of the link. A function
 *      taking an mmu handle and length of data that indicates the data that
 *      has been placed in the mmu by the other subsystem in the link.
 *      The callback returns a bool that is TRUE if the data has been consumed.
 */
extern void isp_router_create_out_band_bi_dir(
                            isp_ss_port_cntx * context,
                            bool first,
                            uint16 ssid,
                            uint16 protocol,
                            mmu_handle rx_mmu,
                            mmu_handle tx_mmu,
                            link_establish_callback link_callback,
                            data_report_callback rx_callback);

/**
 * Disconnect the isp link of this context
 * The function calls submsg to disconnect both sides of a bi-directional link
 * or the sole direction of a uni-directional link. The link callback function
 * is called when the disconnection is complete.
 * \param context - the private isp link context for the link that needs
 *  disconnecting.
 */
extern void isp_router_disconnect_link(isp_ss_port_cntx * context);

/** Helper macro for subsystems to simplify the interface for creating a
 * link to the curator.
 *
 * Called from a module for which isp is doing the routing to set up a
 * submsg in-out-band channel for getting messages to and from the curator.
 * This function assumes the following sequence:
 * 1. The curator calls the \c isp_router_create_subsys_channel() function.
 * 2. The subsystem calls this function.
 * 3. The link callback passed to this function is called when the link is ready
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param buf_size_octets - a buffer of this size will be created for the
 *      link. Zero may be specified if sharing the link with another protocol
 *      e.g. ftp can do this because ccp has already created a buffer.
 * \param callback - a void function taking a context pointer and a
 *      boolean that indicates if the link has been created or destroyed.
 *      May be NULL if not required.
 */
#define isp_router_create_curator_channel(context,                          \
                                          protocol,                         \
                                          buf_size_octets,                  \
                                          callback)                         \
            isp_router_create_port((context), FALSE, CURATOR_SSID,          \
                                (protocol), (buf_size_octets), (callback), NULL)

/** Helper macro for subsystems to simplify the interface for creating a
 * link to the curator.  Used if the link is to have asymmetric
 * buffer placement.
 *
 * Called from a module for which isp is doing the routing to set up a
 * submsg in-out-band channel for getting messages to and from the curator.
 * This function assumes the following sequence:
 * 1. The curator calls the \c isp_router_create_subsys_channel() function.
 * 2. The subsystem calls this function.
 * 3. The link callback passed to this function is called when the link is ready
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param rx_buf_size_octets - a buffer of this size will be created for the
 *      rx link. Zero may be specified if sharnig the link with another
 *      protocol e.g. ftp can do this because ccp has already created a
 *      buffer, or if the buffer is to be created remotely.
 * \param tx_buf_size_octets - a buffer of this size will be created for
 *      the tx link.  Zero may be specified if the buffer is to be
 *      created remotely.
 * \param callback - a void function taking a boolean that indicates if the
 *      link has been created or destroyed.
 */
#define isp_router_create_curator_channel_tx_buf(                       \
    context,                                                            \
    protocol,                                                           \
    rx_buf_size_octets,                                                 \
    tx_buf_size_octets,                                                 \
    callback)                                                           \
    isp_router_create_port_tx_buf(                                      \
        (context), FALSE, CURATOR_SSID,                                 \
        (protocol), (rx_buf_size_octets), (tx_buf_size_octets),         \
        (callback), NULL)

/** Helper macro for the curator to simplify interface for creating a link.
 *
 * Called from a module for which isp is doing the routing to set up a
 * submsg in-out-band channel for getting messages to and from a sub-system
 * This function assumes the following sequence:
 * 1. The curator calls this function.
 * 2. The subsystem calls \c isp_router_create_curator_channel().
 * 3. The link callback passed to this macro is called when the link is ready.
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param ssid - sub-system id of the other side of the link
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param buf_size_octets - a buffer of this size will be created for the
 *      link. Zero may be specified if sharing the link with another protocol
 *      e.g. ftp can do this because ccp has already created a buffer.
 * \param callback - a void function taking a context pointer and a
 *      boolean that indicates if the link has been created or destroyed.
 *      May be NULL if not required.
 */
#define isp_router_create_subsys_channel(context, ssid, protocol, \
                                            buf_size_octets, callback) \
                isp_router_create_port((context), TRUE, (ssid), (protocol), \
                                        (buf_size_octets), (callback), NULL)

/** Helper macro for the curator to simplify interface for creating a
 *  link.  Used if the link is to have asymmetric buffer placement.
 *
 * Called from a module for which isp is doing the routing to set up a
 * submsg in-out-band channel for getting messages to and from a sub-system
 * This function assumes the following sequence:
 * 1. The curator calls this function.
 * 2. The subsystem calls \c isp_router_create_curator_channel().
 * 3. The link callback passed to this macro is called when the link is ready.
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param ssid - sub-system id of the other side of the link
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param rx_buf_size_octets - a buffer of this size will be created for the
 *      rx link. Zero may be specified if sharnig the link with another
 *      protocol e.g. ftp can do this because ccp has already created a
 *      buffer, or if the buffer is to be created remotely.
 * \param tx_buf_size_octets - a buffer of this size will be created for
 *      the tx link.  Zero may be specified if the buffer is to be
 *      created remotely.
 * \param callback - a void function taking a boolean that indicates if the
 *      link has been created or destroyed.
 */
#define isp_router_create_subsys_channel_tx_buf(                        \
    context, ssid, protocol,                                            \
    rx_buf_size_octets, tx_buf_size_octets, callback)                   \
    isp_router_create_port_tx_buf(                                      \
        (context), TRUE, (ssid), (protocol),                            \
        (rx_buf_size_octets), (tx_buf_size_octets), (callback), NULL)

/** Called from fileserv to prepare the out-band link to a subsystem
 * for passing file data. The mmu is owned by the subsystem.
 * this function must be called before the subsystem creates (and activates)
 * its receive side of the link.
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param callback - a function taking an mmu handle and length of data that
 *      indicates the data that has been placed in the mmu by the curator.
 *      The callback returns a bool that is TRUE if the data has been consumed.
 * \param mmu - the handle of an mmu that has been created for this link
 */
#define isp_router_create_curator_out_band_rx(context, protocol, callback, mmu)\
            isp_router_create_out_band((context), FALSE, CURATOR_SSID,      \
                                        (protocol), (callback), (mmu))

/** Called from fileserv on the curator to prepare the out-band link to a
 * subsystem for passing file data. The mmu is owned by the subsystem.
 * this function must be called before the subsystem creates (and activates)
 * its receive side of the link.
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param ssid - sub-system id of the other side of the link
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 */
#define isp_router_create_subsys_out_band_tx(context, ssid, protocol)    \
                    isp_router_create_out_band((context), TRUE, (ssid), \
                                                (protocol), NULL, MMU_NO_HANDLE)

/** Called from fileserv on a subsystem to complete the out-band link
 * from another subsystem for receiving file data. The mmu is owned by
 * this subsystem. This function must be called after the remote subsystem
 * creates its transmit side of the link.
 * \param context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param ssid - sub-system id of the other side of the link
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param callback - a function taking an mmu handle and length of data that
 *      indicates the data that has been placed in the mmu by the curator.
 *      The callback returns a bool that is TRUE if the data has been consumed.
 * \param mmu - the handle of an mmu that has been created for this link. The
 * mmu will be destroyed when the link with the remote subsystem is
 * disconnected.
 */
#define isp_router_create_subsys_out_band_rx(context, ssid, protocol, callback, mmu)  \
                    isp_router_create_out_band((context), FALSE, (ssid),    \
                                                (protocol), (callback), (mmu))

/** Called from protocol modules when they have finished with a link. They
 * will have either destroyed it themselves or had it destroyed by the other
 * side and had confirmation of this in their link_callback function. This
 * call simply removes the link context (that had been passed into this module
 * in the link_create call) from the internal database of the isp_router module.
 * If it was created from a memory pool then it is the caller's responsibility
 * to free it.
 * \param context The port context to be removed
 */
extern void isp_router_unlink_link_context(isp_ss_port_cntx * context);

/** Allows a subsystem to create a link with the host (via the curator)
 * for an arbitrary isp_port. This creates the link on the subsystem end
 * then sends a ccp message to the curator to ask it to set up its end and
 * activate it.
 * \param link_context - pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param protocol - isp protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param buf_size_octets - a buffer of this size will be created for the
 *      link. Zero may be specified if sharing the link with another protocol
 *      e.g. ftp can do this because ccp has already created a buffer.
 * \param link_callback - a void function taking a context pointer and a
 *      boolean that indicates if the link has been created or destroyed.
 *      May be NULL if not required.
 * \param rx_callback - a function called to pass the received pdu to. Not used
 *      for platform protocol default handlers like ccp and ftp in which case
 *      it should be NULL.
 */
void isp_router_create_host_channel(isp_ss_port_cntx * link_context,
                                    uint16 protocol,
                                    uint16 buf_size_octets,
                                    link_establish_callback link_callback,
                                    data_callback rx_callback);

/**
 * Allows a subsystem to create a link with the host (via the curator)
 * for an arbitrary isp_port using an out-band link. Buffers are created
 * on the subsystem side of the size given by \c buf_size_octets.
 * This function creates the link on the subsystem end
 * then sends a ccp message to the curator to ask it to set up its end and
 * activate it.
 * \param link_context Pointer to a \c isp_ss_port_cntx structure either
 *              allocated by the calling module or within its own structure.
 *              The structure is private to this module and the caller
 *              should not be concerned with its contents.
 * \param protocol ISP protocol to be used (ISP_PORT_MAPPING_ENUM_XXX)
 * \param buf_size_octets Two buffers of this size will be created for the
 *      link (one for each direction).
 * \param link_callback A void function taking a context pointer and a
 *      boolean that indicates if the link has been created or destroyed.
 *      May be NULL if not required.
 * \param rx_callback A function called to pass the received data report to.
 */
void isp_router_create_out_band_bi_dir_host_channel(
                                    isp_ss_port_cntx * link_context,
                                    uint16 protocol,
                                    uint16 buf_size_octets,
                                    link_establish_callback link_callback,
                                    data_report_callback rx_callback);

/** Method of sending a message through ISP.
 * The pdu is copied out of the supplied array so it may be an automatic
 * or freed immediately after this call.
 * \param pdu - the pdu to be sent
 * \param pdu_len_words - the length of the pdu in words
 * \param dest - the subsystem to send it to
 * \param protocol - the isp port to route it to
 * \return TRUE - success. FALSE - failed to send. This will be because
 * there is insufficient room in the submsg buffer.
 */
extern bool isp_router_send(const uint16 * pdu,
                            uint16 pdu_len_words,
                            uint16 dest,
                            uint16 protocol);

/** Called from ccp_router with a received ccp pdu for this module
 * \param pdu - the received pdu
 * \param pdu_len - the length of the pdu in words
 * \param source - the subsystem from which the pdu originated
 * \returns TRUE to say message has been handled
 */
extern bool isp_router_ccp_message(const uint16 *pdu,
                                   uint16 pdu_len,
                                   uint16 source);

#ifdef SUBSYSTEM_CURATOR
/** Entry point for hostmsg messages received from the host. The function
 * routes these to the correct protocol module based on the protocol code
 * in the ISP header.
 * \param msg_buf - the buffer holding the received message
 */
extern void isp_router_host_msg(BUFFER_MSG * msg_buf);

/** Called by protocol modules to get a pointer to a mapped buffer for
 * writing event data to be sent to the host. The function gets a pointer
 * from the hostmsg protocol layer, adds an ISP header according to the
 * parameters passed, and returns a pointer to the ISP payload. When the
 * calling module has written its message, it calls
 * \c isp_router_send_host_evt() to get it sent to the host.
 * \param prot - Protocol of the message
 * \param src - source address
 * \param dest - destination address
 */
extern uint8 * isp_router_get_host_evt_ptr(uint16 prot, uint16 src, uint16 dest);

/** Called by protocol modules to find the maximum size of pdu they can
 * return to the host. It is assumed that all events are returned through
 * calls from the background so the value returned from this function is
 * valid for the point at which \c isp_router_send_host_evt() is called.
 * \returns The number of bytes that can be safely used.
 */
extern uint16 isp_router_get_host_evt_space(void);

/** Called by protocol messages to return a completed event PDU to the host.
 * \param len - the length of data written to the pointer passed by
 * \c isp_router_get_host_evt_ptr(). The function adjusts this for the
 * ISP header size before returning it to the hostmsg protocol layer.
 */
extern void isp_router_send_host_evt(uint16 len);

/**
 * Create the CCP channel between the Curator and itself.
 */
extern void isp_router_ccp_self_init(void);

/** Entry point for curator to send data to a subsystem or forward
 * data from a subsystem to the host.
 * This can route data from a local file system to a subsystem.
 * It will update the destination buffer handle offset
 * \param data      - the file system data
 * \param data_size - the amount of data
 * \param dest      - the destination subsystem
 * \param src       - the source subsystem
 * \param dest_port - the port on the destination subsystem
 */
extern void isp_router_outband_send(const uint8 *data,
                                    uint16 data_size,
                                    uint16 dest,
                                    uint16 src,
                                    uint16 dest_port);

#endif /* SUBSYSTEM_CURATOR */

#endif /* ISP_ROUTER_H */
