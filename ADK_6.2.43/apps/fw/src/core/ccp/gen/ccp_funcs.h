/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file ../../fw/src/core/ccp/gen/ccp_funcs.h
*/

#ifndef CCP_FUNCS_H
#define CCP_FUNCS_H

/*
 * The following headers are specified as needed for certain
 * types.  If the type resides elswhere in the subsystem firmware,
 * inclusion of the header can be suppressed by macro and an alternative
 * header included from ccp.h.
 */
#ifndef CCP_DONT_INCLUDE_TIMED_EVENT_TIMED_EVENT_H
#include "timed_event/timed_event.h"
#endif
#ifndef CCP_DONT_INCLUDE_HYDRA_SERVICE_HYDRA_SERVICE_H
#include "hydra_service/hydra_service.h"
#endif

/**
 * Send CCP PDU alive.
 *
 * DIR BOTH
 * Sent by a subsystem to the Curator to indicate that the subsystem
 * is now alive.  The same PDU is returned by the Curator.  The exchange
 * of PDUs completes initialisation of intersubsystem messaging.
 * 
 * DEPRECATED: this PDU is no longer required, as subsystem
 * initialisation is sequenced by subsmsg link control PDUs
 * and version information is passed in OPERATIONAL_IND or via
 * the symbol look-up table.
 * 
 * RESURRECTED: When sent from _host_ to a subsystem with
 * handle == length == 0x0000 this triggers a panic(PANIC_TEST) to
 * support crash recovery testing [B-106470].
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c handle
 * MMU handle of the buffer the sender will use for receiving extended
 * message information.  This is the Hydra native format for the
 * MMU handle as the 16-bit integer understood by the hardware.
 *
 *   * \c length
 * Length of the buffer given in 'handle' in 16-bit words
 * (N.B.: not octets, for consistency with the rest of ISSMSG.)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                 uint16 handle,
                                 uint16 length);

/**
 * Send CCP PDU alive.
 *
 * DIR BOTH
 * Sent by a subsystem to the Curator to indicate that the subsystem
 * is now alive.  The same PDU is returned by the Curator.  The exchange
 * of PDUs completes initialisation of intersubsystem messaging.
 * 
 * DEPRECATED: this PDU is no longer required, as subsystem
 * initialisation is sequenced by subsmsg link control PDUs
 * and version information is passed in OPERATIONAL_IND or via
 * the symbol look-up table.
 * 
 * RESURRECTED: When sent from _host_ to a subsystem with
 * handle == length == 0x0000 this triggers a panic(PANIC_TEST) to
 * support crash recovery testing [B-106470].
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c handle
 * MMU handle of the buffer the sender will use for receiving extended
 * message information.  This is the Hydra native format for the
 * MMU handle as the 16-bit integer understood by the hardware.
 *
 *   * \c length
 * Length of the buffer given in 'handle' in 16-bit words
 * (N.B.: not octets, for consistency with the rest of ISSMSG.)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_alive(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID_ALIVE, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU alive.
 *
 * DIR BOTH
 * Sent by a subsystem to the Curator to indicate that the subsystem
 * is now alive.  The same PDU is returned by the Curator.  The exchange
 * of PDUs completes initialisation of intersubsystem messaging.
 * 
 * DEPRECATED: this PDU is no longer required, as subsystem
 * initialisation is sequenced by subsmsg link control PDUs
 * and version information is passed in OPERATIONAL_IND or via
 * the symbol look-up table.
 * 
 * RESURRECTED: When sent from _host_ to a subsystem with
 * handle == length == 0x0000 this triggers a panic(PANIC_TEST) to
 * support crash recovery testing [B-106470].
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c handle
 * MMU handle of the buffer the sender will use for receiving extended
 * message information.  This is the Hydra native format for the
 * MMU handle as the 16-bit integer understood by the hardware.
 *
 *   * \c length
 * Length of the buffer given in 'handle' in 16-bit words
 * (N.B.: not octets, for consistency with the rest of ISSMSG.)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16(const uint16 *pdu, uint16 pdu_len_words,
                                 uint16 *handle,
                                 uint16 *length);

/**
 * Analyse received CCP PDU alive.
 *
 * DIR BOTH
 * Sent by a subsystem to the Curator to indicate that the subsystem
 * is now alive.  The same PDU is returned by the Curator.  The exchange
 * of PDUs completes initialisation of intersubsystem messaging.
 * 
 * DEPRECATED: this PDU is no longer required, as subsystem
 * initialisation is sequenced by subsmsg link control PDUs
 * and version information is passed in OPERATIONAL_IND or via
 * the symbol look-up table.
 * 
 * RESURRECTED: When sent from _host_ to a subsystem with
 * handle == length == 0x0000 this triggers a panic(PANIC_TEST) to
 * support crash recovery testing [B-106470].
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c handle
 * MMU handle of the buffer the sender will use for receiving extended
 * message information.  This is the Hydra native format for the
 * MMU handle as the 16-bit integer understood by the hardware.
 *
 *   * \c length
 * Length of the buffer given in 'handle' in 16-bit words
 * (N.B.: not octets, for consistency with the rest of ISSMSG.)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_alive(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU resource_req.
 *
 * DIR TO_CURATOR
 * Request the use of a globally managed resource.
 * The response is one of RESOURCE_ASSIGNED_RSP, RESOURCE_DATA_RSP,
 * RESOURCE_UNAVAIL_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource being requested.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_RESOURCE_ID(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                  CCP_RESOURCE_ID res_id);

/**
 * Send CCP PDU resource_req.
 *
 * DIR TO_CURATOR
 * Request the use of a globally managed resource.
 * The response is one of RESOURCE_ASSIGNED_RSP, RESOURCE_DATA_RSP,
 * RESOURCE_UNAVAIL_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource being requested.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_req(arg1, arg2) \
    ccp_send_pdu_CCP_RESOURCE_ID(CCP_SIGNAL_ID_RESOURCE_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU resource_req.
 *
 * DIR TO_CURATOR
 * Request the use of a globally managed resource.
 * The response is one of RESOURCE_ASSIGNED_RSP, RESOURCE_DATA_RSP,
 * RESOURCE_UNAVAIL_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource being requested.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_RESOURCE_ID(const uint16 *pdu, uint16 pdu_len_words,
                                  CCP_RESOURCE_ID *res_id);

/**
 * Analyse received CCP PDU resource_req.
 *
 * DIR TO_CURATOR
 * Request the use of a globally managed resource.
 * The response is one of RESOURCE_ASSIGNED_RSP, RESOURCE_DATA_RSP,
 * RESOURCE_UNAVAIL_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource being requested.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_req(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_RESOURCE_ID(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_RESOURCE_DATA_REQ_DATA \
    CCP_RESOURCE_DATA_REQ_DATA_WORD_OFFSET

/**
 * Send CCP PDU resource_data_req.
 *
 * DIR TO_CURATOR
 * Request the use of a globally managed resource.
 * This version of the PDU passes data to the resource.
 * The response is one of RESOURCE_ASSIGNED_RSP, RESOURCE_DATA_RSP,
 * RESOURCE_UNAVAIL_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c res_id
 * Identifier for the resource being requested.
 *
 *   * \c flags
 * A set of bit flags for the request.
 * The type of each individual flag will be CCP_RESOURCE_FLAGS.
 * Flags may indicate data present in the data array argument; the
 * documentation for the flag will indicate the format of the data.
 * Flag data is in order of the flag's bit (data for bit 0, data
 * for bit 1, etc.) and precedes data to be passed to the resource
 * handler.
 *
 *   * Non-parameter argument \c data:
 * Data to be passed to the resource when acquring it.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_RESOURCE_DATA_REQ_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_RESOURCE_ID__uint16__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        CCP_RESOURCE_ID res_id,
        uint16 flags);

/**
 * Send CCP PDU resource_data_req.
 *
 * DIR TO_CURATOR
 * Request the use of a globally managed resource.
 * This version of the PDU passes data to the resource.
 * The response is one of RESOURCE_ASSIGNED_RSP, RESOURCE_DATA_RSP,
 * RESOURCE_UNAVAIL_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c res_id
 * Identifier for the resource being requested.
 *
 *   * \c flags
 * A set of bit flags for the request.
 * The type of each individual flag will be CCP_RESOURCE_FLAGS.
 * Flags may indicate data present in the data array argument; the
 * documentation for the flag will indicate the format of the data.
 * Flag data is in order of the flag's bit (data for bit 0, data
 * for bit 1, etc.) and precedes data to be passed to the resource
 * handler.
 *
 *   * Non-parameter argument \c data:
 * Data to be passed to the resource when acquring it.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_RESOURCE_DATA_REQ_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_data_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_CCP_RESOURCE_ID__uint16__VARLEN(CCP_SIGNAL_ID_RESOURCE_DATA_REQ, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU resource_data_req.
 *
 * DIR TO_CURATOR
 * Request the use of a globally managed resource.
 * This version of the PDU passes data to the resource.
 * The response is one of RESOURCE_ASSIGNED_RSP, RESOURCE_DATA_RSP,
 * RESOURCE_UNAVAIL_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource being requested.
 *
 *   * \c flags
 * A set of bit flags for the request.
 * The type of each individual flag will be CCP_RESOURCE_FLAGS.
 * Flags may indicate data present in the data array argument; the
 * documentation for the flag will indicate the format of the data.
 * Flag data is in order of the flag's bit (data for bit 0, data
 * for bit 1, etc.) and precedes data to be passed to the resource
 * handler.
 *
 *   Non-parameter argument \c data:
 * Data to be passed to the resource when acquring it.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_RESOURCE_DATA_REQ_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_RESOURCE_ID__uint16__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_RESOURCE_ID *res_id,
        uint16 *flags);

/**
 * Analyse received CCP PDU resource_data_req.
 *
 * DIR TO_CURATOR
 * Request the use of a globally managed resource.
 * This version of the PDU passes data to the resource.
 * The response is one of RESOURCE_ASSIGNED_RSP, RESOURCE_DATA_RSP,
 * RESOURCE_UNAVAIL_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource being requested.
 *
 *   * \c flags
 * A set of bit flags for the request.
 * The type of each individual flag will be CCP_RESOURCE_FLAGS.
 * Flags may indicate data present in the data array argument; the
 * documentation for the flag will indicate the format of the data.
 * Flag data is in order of the flag's bit (data for bit 0, data
 * for bit 1, etc.) and precedes data to be passed to the resource
 * handler.
 *
 *   Non-parameter argument \c data:
 * Data to be passed to the resource when acquring it.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_RESOURCE_DATA_REQ_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_data_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_RESOURCE_ID__uint16__VARLEN(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU resource_assigned_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource has been assigned to
 * it.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource that was assigned.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_assigned_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_RESOURCE_ID(CCP_SIGNAL_ID_RESOURCE_ASSIGNED_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU resource_assigned_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource has been assigned to
 * it.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource that was assigned.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_assigned_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_RESOURCE_ID(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_RESOURCE_DATA_RSP_DATA \
    CCP_RESOURCE_DATA_RSP_DATA_WORD_OFFSET

/**
 * Send CCP PDU resource_data_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource has been assigned to
 * it (possibly temporarily), and passing data from the resource.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c res_id
 * Identifier for the resource that was assigned.
 *
 *   * \c flags
 * A set of bit flags for the request.
 * The flags are the same set as for RESOURCE_DATA_REQ and have the same
 * rules, i.e. the presence of a flag may indicate the presence of data.
 *
 *   * Non-parameter argument \c data:
 * Data passed from the resource.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_RESOURCE_DATA_RSP_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_data_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_CCP_RESOURCE_ID__uint16__VARLEN(CCP_SIGNAL_ID_RESOURCE_DATA_RSP, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU resource_data_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource has been assigned to
 * it (possibly temporarily), and passing data from the resource.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource that was assigned.
 *
 *   * \c flags
 * A set of bit flags for the request.
 * The flags are the same set as for RESOURCE_DATA_REQ and have the same
 * rules, i.e. the presence of a flag may indicate the presence of data.
 *
 *   Non-parameter argument \c data:
 * Data passed from the resource.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_RESOURCE_DATA_RSP_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_data_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_RESOURCE_ID__uint16__VARLEN(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU resource_unavail_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource was unavailable.
 * The arguments detail why the resource was unavailable.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource that was originally requested.
 *
 *   * \c result
 * The reason for the failure.
 *
 *   * \c result_res_id
 * The resource that could not be acquired.
 * This may be a dependency of the requested resource.
 *
 *   * \c status
 * The status of the resource result_res_id.
 *
 *   * \c subsystem
 * A bitmap indicating the subsystem(s) that are currently
 * using result_res_id.  The bits correspond to subsystem bus
 * addresses, identical to ISP addresses.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_RESULT__CCP_RESOURCE_ID__CCP_RESOURCE_STATUS__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_RESOURCE_ID res_id,
        CCP_RESOURCE_RESULT result,
        CCP_RESOURCE_ID result_res_id,
        CCP_RESOURCE_STATUS status,
        uint16 subsystem);

/**
 * Send CCP PDU resource_unavail_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource was unavailable.
 * The arguments detail why the resource was unavailable.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource that was originally requested.
 *
 *   * \c result
 * The reason for the failure.
 *
 *   * \c result_res_id
 * The resource that could not be acquired.
 * This may be a dependency of the requested resource.
 *
 *   * \c status
 * The status of the resource result_res_id.
 *
 *   * \c subsystem
 * A bitmap indicating the subsystem(s) that are currently
 * using result_res_id.  The bits correspond to subsystem bus
 * addresses, identical to ISP addresses.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_unavail_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_send_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_RESULT__CCP_RESOURCE_ID__CCP_RESOURCE_STATUS__uint16(CCP_SIGNAL_ID_RESOURCE_UNAVAIL_RSP, arg1, arg2, arg3, arg4, arg5, arg6)

/**
 * Analyse received CCP PDU resource_unavail_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource was unavailable.
 * The arguments detail why the resource was unavailable.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource that was originally requested.
 *
 *   * \c result
 * The reason for the failure.
 *
 *   * \c result_res_id
 * The resource that could not be acquired.
 * This may be a dependency of the requested resource.
 *
 *   * \c status
 * The status of the resource result_res_id.
 *
 *   * \c subsystem
 * A bitmap indicating the subsystem(s) that are currently
 * using result_res_id.  The bits correspond to subsystem bus
 * addresses, identical to ISP addresses.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_RESULT__CCP_RESOURCE_ID__CCP_RESOURCE_STATUS__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_RESOURCE_ID *res_id,
        CCP_RESOURCE_RESULT *result,
        CCP_RESOURCE_ID *result_res_id,
        CCP_RESOURCE_STATUS *status,
        uint16 *subsystem);

/**
 * Analyse received CCP PDU resource_unavail_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource was unavailable.
 * The arguments detail why the resource was unavailable.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource that was originally requested.
 *
 *   * \c result
 * The reason for the failure.
 *
 *   * \c result_res_id
 * The resource that could not be acquired.
 * This may be a dependency of the requested resource.
 *
 *   * \c status
 * The status of the resource result_res_id.
 *
 *   * \c subsystem
 * A bitmap indicating the subsystem(s) that are currently
 * using result_res_id.  The bits correspond to subsystem bus
 * addresses, identical to ISP addresses.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_unavail_rsp(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    ccp_recv_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_RESULT__CCP_RESOURCE_ID__CCP_RESOURCE_STATUS__uint16(arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/**
 * Send CCP PDU resource_release_req.
 *
 * DIR TO_CURATOR
 * Release a globally managed resource that was previously acquired.
 * The response is one of RESOURCE_RELEASED_RSP or
 * RESOURCE_NOT RELEASED_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource to release.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_release_req(arg1, arg2) \
    ccp_send_pdu_CCP_RESOURCE_ID(CCP_SIGNAL_ID_RESOURCE_RELEASE_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU resource_release_req.
 *
 * DIR TO_CURATOR
 * Release a globally managed resource that was previously acquired.
 * The response is one of RESOURCE_RELEASED_RSP or
 * RESOURCE_NOT RELEASED_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource to release.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_release_req(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_RESOURCE_ID(arg1, arg2, arg3)


/**
 * Send CCP PDU resource_released_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource was released.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource that was released.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_released_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_RESOURCE_ID(CCP_SIGNAL_ID_RESOURCE_RELEASED_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU resource_released_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource was released.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource that was released.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_released_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_RESOURCE_ID(arg1, arg2, arg3)


/**
 * Send CCP PDU resource_not_released_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource could not be released.
 * An argument indicates the reason for the failure.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource that was released.
 *
 *   * \c result
 * The reason for the failure.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_RESULT(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_RESOURCE_ID res_id,
        CCP_RESOURCE_RESULT result);

/**
 * Send CCP PDU resource_not_released_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource could not be released.
 * An argument indicates the reason for the failure.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource that was released.
 *
 *   * \c result
 * The reason for the failure.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_not_released_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_RESULT(CCP_SIGNAL_ID_RESOURCE_NOT_RELEASED_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU resource_not_released_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource could not be released.
 * An argument indicates the reason for the failure.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource that was released.
 *
 *   * \c result
 * The reason for the failure.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_RESULT(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_RESOURCE_ID *res_id,
        CCP_RESOURCE_RESULT *result);

/**
 * Analyse received CCP PDU resource_not_released_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the requester indicating a resource could not be released.
 * An argument indicates the reason for the failure.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource that was released.
 *
 *   * \c result
 * The reason for the failure.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_not_released_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_RESULT(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU resource_query_req.
 *
 * DIR TO_CURATOR
 * Query the status of a global resource.
 * The response is RESOURCE_QUERY_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource to release.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_query_req(arg1, arg2) \
    ccp_send_pdu_CCP_RESOURCE_ID(CCP_SIGNAL_ID_RESOURCE_QUERY_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU resource_query_req.
 *
 * DIR TO_CURATOR
 * Query the status of a global resource.
 * The response is RESOURCE_QUERY_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource to release.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_query_req(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_RESOURCE_ID(arg1, arg2, arg3)


/**
 * Send CCP PDU resource_query_rsp.
 *
 * DIR TO_CURATOR
 * Reply to a query about a global resource.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource in the query.
 *
 *   * \c status
 * The status of the resource. If the resource is not present
 * on the current system, this is \c CCP_RESOURCE_ST_INVALID.
 *
 *   * \c subsystems
 * A bitmap indicating the subsystem(s) that are currently
 * using the resource.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_STATUS__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_RESOURCE_ID res_id,
        CCP_RESOURCE_STATUS status,
        uint16 subsystems);

/**
 * Send CCP PDU resource_query_rsp.
 *
 * DIR TO_CURATOR
 * Reply to a query about a global resource.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource in the query.
 *
 *   * \c status
 * The status of the resource. If the resource is not present
 * on the current system, this is \c CCP_RESOURCE_ST_INVALID.
 *
 *   * \c subsystems
 * A bitmap indicating the subsystem(s) that are currently
 * using the resource.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_query_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_STATUS__uint16(CCP_SIGNAL_ID_RESOURCE_QUERY_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU resource_query_rsp.
 *
 * DIR TO_CURATOR
 * Reply to a query about a global resource.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource in the query.
 *
 *   * \c status
 * The status of the resource. If the resource is not present
 * on the current system, this is \c CCP_RESOURCE_ST_INVALID.
 *
 *   * \c subsystems
 * A bitmap indicating the subsystem(s) that are currently
 * using the resource.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_STATUS__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_RESOURCE_ID *res_id,
        CCP_RESOURCE_STATUS *status,
        uint16 *subsystems);

/**
 * Analyse received CCP PDU resource_query_rsp.
 *
 * DIR TO_CURATOR
 * Reply to a query about a global resource.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource in the query.
 *
 *   * \c status
 * The status of the resource. If the resource is not present
 * on the current system, this is \c CCP_RESOURCE_ST_INVALID.
 *
 *   * \c subsystems
 * A bitmap indicating the subsystem(s) that are currently
 * using the resource.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_query_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_CCP_RESOURCE_ID__CCP_RESOURCE_STATUS__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU resource_revoke_req.
 *
 * DIR FROM_CURATOR
 * The resource indicated, owned by the addressed party, is revoked.
 * This is mandatory: the recipient must tidy up any use of the
 * resource and reply with RESOURCE_REVOKED_RSP.  Only resources
 * with the REVOKE flag are subject to revocation.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource being revoked.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_revoke_req(arg1, arg2) \
    ccp_send_pdu_CCP_RESOURCE_ID(CCP_SIGNAL_ID_RESOURCE_REVOKE_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU resource_revoke_req.
 *
 * DIR FROM_CURATOR
 * The resource indicated, owned by the addressed party, is revoked.
 * This is mandatory: the recipient must tidy up any use of the
 * resource and reply with RESOURCE_REVOKED_RSP.  Only resources
 * with the REVOKE flag are subject to revocation.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource being revoked.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_revoke_req(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_RESOURCE_ID(arg1, arg2, arg3)


/**
 * Send CCP PDU resource_revoked_rsp.
 *
 * DIR TO_CURATOR
 * Response to RESOURCE_REVOKE_REQ.
 * The sender is indicating to the Curator that it has ceased use
 * of the indicated resource and will treat it as relinquished.
 * This is the only valid response to the request.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource being revoked.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_revoked_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_RESOURCE_ID(CCP_SIGNAL_ID_RESOURCE_REVOKED_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU resource_revoked_rsp.
 *
 * DIR TO_CURATOR
 * Response to RESOURCE_REVOKE_REQ.
 * The sender is indicating to the Curator that it has ceased use
 * of the indicated resource and will treat it as relinquished.
 * This is the only valid response to the request.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource being revoked.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_revoked_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_RESOURCE_ID(arg1, arg2, arg3)


/**
 * Send CCP PDU deep_sleep_wake_mask.
 *
 * DIR TO_CURATOR
 * Supply the details of what can wake a subsystem.
 * Sent from subsystem to Curator; no reply.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c wake_mask
 * The mask of wake bits.
 * Use of this is defined in the Curator globsleep module
 * which manages this message.  Currently the lower 16 bits
 * are the hardware wake sources for the chip, the upper
 * 16 bits include extra definitions from the type
 * CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 *
 *   * \c pio_wake_mask
 * The mask of PIOs that should wake the subsystem.
 *
 *   * \c pio_invert_mask
 * The mask of PIOs for which the wake is inverted.
 * Only meaningful if the bit is also set in pio_wake_mask.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint32__uint32__uint32(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint32 wake_mask,
        uint32 pio_wake_mask,
        uint32 pio_invert_mask);

/**
 * Send CCP PDU deep_sleep_wake_mask.
 *
 * DIR TO_CURATOR
 * Supply the details of what can wake a subsystem.
 * Sent from subsystem to Curator; no reply.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c wake_mask
 * The mask of wake bits.
 * Use of this is defined in the Curator globsleep module
 * which manages this message.  Currently the lower 16 bits
 * are the hardware wake sources for the chip, the upper
 * 16 bits include extra definitions from the type
 * CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 *
 *   * \c pio_wake_mask
 * The mask of PIOs that should wake the subsystem.
 *
 *   * \c pio_invert_mask
 * The mask of PIOs for which the wake is inverted.
 * Only meaningful if the bit is also set in pio_wake_mask.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_wake_mask(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_uint32__uint32__uint32(CCP_SIGNAL_ID_DEEP_SLEEP_WAKE_MASK, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU deep_sleep_wake_mask.
 *
 * DIR TO_CURATOR
 * Supply the details of what can wake a subsystem.
 * Sent from subsystem to Curator; no reply.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c wake_mask
 * The mask of wake bits.
 * Use of this is defined in the Curator globsleep module
 * which manages this message.  Currently the lower 16 bits
 * are the hardware wake sources for the chip, the upper
 * 16 bits include extra definitions from the type
 * CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 *
 *   * \c pio_wake_mask
 * The mask of PIOs that should wake the subsystem.
 *
 *   * \c pio_invert_mask
 * The mask of PIOs for which the wake is inverted.
 * Only meaningful if the bit is also set in pio_wake_mask.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint32__uint32__uint32(
        const uint16 *pdu, uint16 pdu_len_words,
        uint32 *wake_mask,
        uint32 *pio_wake_mask,
        uint32 *pio_invert_mask);

/**
 * Analyse received CCP PDU deep_sleep_wake_mask.
 *
 * DIR TO_CURATOR
 * Supply the details of what can wake a subsystem.
 * Sent from subsystem to Curator; no reply.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c wake_mask
 * The mask of wake bits.
 * Use of this is defined in the Curator globsleep module
 * which manages this message.  Currently the lower 16 bits
 * are the hardware wake sources for the chip, the upper
 * 16 bits include extra definitions from the type
 * CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 *
 *   * \c pio_wake_mask
 * The mask of PIOs that should wake the subsystem.
 *
 *   * \c pio_invert_mask
 * The mask of PIOs for which the wake is inverted.
 * Only meaningful if the bit is also set in pio_wake_mask.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_wake_mask(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_uint32__uint32__uint32(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU deep_sleep_ready.
 *
 * DIR TO_CURATOR
 * HEADER timed_event/timed_event.h
 * Reports to the Curator that a subsystem is ready to enter deep sleep.
 * See the Hydra Global Procedures document for the gory detail.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c deadline
 * The deadline by which the subsystem must be woken up.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_TIME(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                       TIME deadline);

/**
 * Send CCP PDU deep_sleep_ready.
 *
 * DIR TO_CURATOR
 * HEADER timed_event/timed_event.h
 * Reports to the Curator that a subsystem is ready to enter deep sleep.
 * See the Hydra Global Procedures document for the gory detail.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c deadline
 * The deadline by which the subsystem must be woken up.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_ready(arg1, arg2) \
    ccp_send_pdu_TIME(CCP_SIGNAL_ID_DEEP_SLEEP_READY, arg1, arg2)

/**
 * Analyse received CCP PDU deep_sleep_ready.
 *
 * DIR TO_CURATOR
 * HEADER timed_event/timed_event.h
 * Reports to the Curator that a subsystem is ready to enter deep sleep.
 * See the Hydra Global Procedures document for the gory detail.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c deadline
 * The deadline by which the subsystem must be woken up.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_TIME(const uint16 *pdu, uint16 pdu_len_words,
                       TIME *deadline);

/**
 * Analyse received CCP PDU deep_sleep_ready.
 *
 * DIR TO_CURATOR
 * HEADER timed_event/timed_event.h
 * Reports to the Curator that a subsystem is ready to enter deep sleep.
 * See the Hydra Global Procedures document for the gory detail.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c deadline
 * The deadline by which the subsystem must be woken up.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_ready(arg1, arg2, arg3) \
    ccp_recv_pdu_TIME(arg1, arg2, arg3)


/**
 * Send CCP PDU deep_sleep_ready_sloppy.
 *
 * DIR TO_CURATOR
 * HEADER timed_event/timed_event.h
 * Reports to the Curator that a subsystem is ready to enter deep sleep.
 * This differs from DEEP_SLEEP_READY in that it supplies an earliest
 * and a latest time for which deep sleep is allowed.  Supplying equal
 * values is equivalent to using DEEP_SLEEP_READY.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c earliest
 * The earliest point at which the subsystem should be woken.
 * If the chip is awake at any time after this point, the subsystem
 * will be woken.
 *
 *   * \c latest
 * The latest point by which the subsystem must be woken.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_TIME__TIME(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                             TIME earliest,
                             TIME latest);

/**
 * Send CCP PDU deep_sleep_ready_sloppy.
 *
 * DIR TO_CURATOR
 * HEADER timed_event/timed_event.h
 * Reports to the Curator that a subsystem is ready to enter deep sleep.
 * This differs from DEEP_SLEEP_READY in that it supplies an earliest
 * and a latest time for which deep sleep is allowed.  Supplying equal
 * values is equivalent to using DEEP_SLEEP_READY.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c earliest
 * The earliest point at which the subsystem should be woken.
 * If the chip is awake at any time after this point, the subsystem
 * will be woken.
 *
 *   * \c latest
 * The latest point by which the subsystem must be woken.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_ready_sloppy(arg1, arg2, arg3) \
    ccp_send_pdu_TIME__TIME(CCP_SIGNAL_ID_DEEP_SLEEP_READY_SLOPPY, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU deep_sleep_ready_sloppy.
 *
 * DIR TO_CURATOR
 * HEADER timed_event/timed_event.h
 * Reports to the Curator that a subsystem is ready to enter deep sleep.
 * This differs from DEEP_SLEEP_READY in that it supplies an earliest
 * and a latest time for which deep sleep is allowed.  Supplying equal
 * values is equivalent to using DEEP_SLEEP_READY.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c earliest
 * The earliest point at which the subsystem should be woken.
 * If the chip is awake at any time after this point, the subsystem
 * will be woken.
 *
 *   * \c latest
 * The latest point by which the subsystem must be woken.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_TIME__TIME(const uint16 *pdu, uint16 pdu_len_words,
                             TIME *earliest,
                             TIME *latest);

/**
 * Analyse received CCP PDU deep_sleep_ready_sloppy.
 *
 * DIR TO_CURATOR
 * HEADER timed_event/timed_event.h
 * Reports to the Curator that a subsystem is ready to enter deep sleep.
 * This differs from DEEP_SLEEP_READY in that it supplies an earliest
 * and a latest time for which deep sleep is allowed.  Supplying equal
 * values is equivalent to using DEEP_SLEEP_READY.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c earliest
 * The earliest point at which the subsystem should be woken.
 * If the chip is awake at any time after this point, the subsystem
 * will be woken.
 *
 *   * \c latest
 * The latest point by which the subsystem must be woken.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_ready_sloppy(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_TIME__TIME(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU deep_sleep_wake_req.
 *
 * DIR BOTH
 * Indicate the subsystem is waking or should wake.
 * When sent from the Curator the arguments indicate the wakeup source;
 * the bits are identical to those in CCP_DEEP_SLEEP_WAKE_MASK,
 * i.e. the lower bits corresponding to chip hardware while
 * upper bits are from the type CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 * Expects a response DEEP_SLEEP_WAKE_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint32(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                         uint32 source);

/**
 * Send CCP PDU deep_sleep_wake_req.
 *
 * DIR BOTH
 * Indicate the subsystem is waking or should wake.
 * When sent from the Curator the arguments indicate the wakeup source;
 * the bits are identical to those in CCP_DEEP_SLEEP_WAKE_MASK,
 * i.e. the lower bits corresponding to chip hardware while
 * upper bits are from the type CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 * Expects a response DEEP_SLEEP_WAKE_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_wake_req(arg1, arg2) \
    ccp_send_pdu_uint32(CCP_SIGNAL_ID_DEEP_SLEEP_WAKE_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU deep_sleep_wake_req.
 *
 * DIR BOTH
 * Indicate the subsystem is waking or should wake.
 * When sent from the Curator the arguments indicate the wakeup source;
 * the bits are identical to those in CCP_DEEP_SLEEP_WAKE_MASK,
 * i.e. the lower bits corresponding to chip hardware while
 * upper bits are from the type CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 * Expects a response DEEP_SLEEP_WAKE_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint32(const uint16 *pdu, uint16 pdu_len_words,
                         uint32 *source);

/**
 * Analyse received CCP PDU deep_sleep_wake_req.
 *
 * DIR BOTH
 * Indicate the subsystem is waking or should wake.
 * When sent from the Curator the arguments indicate the wakeup source;
 * the bits are identical to those in CCP_DEEP_SLEEP_WAKE_MASK,
 * i.e. the lower bits corresponding to chip hardware while
 * upper bits are from the type CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 * Expects a response DEEP_SLEEP_WAKE_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_wake_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint32(arg1, arg2, arg3)


/**
 * Send CCP PDU deep_sleep_wake_rsp.
 *
 * DIR BOTH
 * Mandatory response to \c DEEP_SLEEP_WAKE_REQ.
 * When sent from a subsystem, indicates the subsystem will remain awake
 * until further notice.
 * When sent from the Curator, indicates the Curator has updated its
 * record of the subsystem state.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_NOARGS(CCP_SIGNAL_ID ccp_signal_id, uint16 dest);

/**
 * Send CCP PDU deep_sleep_wake_rsp.
 *
 * DIR BOTH
 * Mandatory response to \c DEEP_SLEEP_WAKE_REQ.
 * When sent from a subsystem, indicates the subsystem will remain awake
 * until further notice.
 * When sent from the Curator, indicates the Curator has updated its
 * record of the subsystem state.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_wake_rsp(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_DEEP_SLEEP_WAKE_RSP, arg1)

/**
 * Analyse received CCP PDU deep_sleep_wake_rsp.
 *
 * DIR BOTH
 * Mandatory response to \c DEEP_SLEEP_WAKE_REQ.
 * When sent from a subsystem, indicates the subsystem will remain awake
 * until further notice.
 * When sent from the Curator, indicates the Curator has updated its
 * record of the subsystem state.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_NOARGS(const uint16 *pdu, uint16 pdu_len_words);

/**
 * Analyse received CCP PDU deep_sleep_wake_rsp.
 *
 * DIR BOTH
 * Mandatory response to \c DEEP_SLEEP_WAKE_REQ.
 * When sent from a subsystem, indicates the subsystem will remain awake
 * until further notice.
 * When sent from the Curator, indicates the Curator has updated its
 * record of the subsystem state.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_wake_rsp(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_ADVERTISEMENT_ADD_REQ_IES_ARRAY \
    CCP_SERVICE_ADVERTISEMENT_ADD_REQ_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_advertisement_add_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Add an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements, may be empty
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_ADVERTISEMENT_ADD_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_SERVICE_RECORD_HEADER__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        const CCP_SERVICE_RECORD_HEADER *srh);

/**
 * Send CCP PDU service_advertisement_add_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Add an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements, may be empty
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_ADVERTISEMENT_ADD_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_advertisement_add_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_SERVICE_RECORD_HEADER__VARLEN(CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_ADD_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_advertisement_add_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Add an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   Non-parameter argument \c ies_array:
 * Information elements, may be empty
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_ADVERTISEMENT_ADD_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_SERVICE_RECORD_HEADER__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_SERVICE_RECORD_HEADER *srh);

/**
 * Analyse received CCP PDU service_advertisement_add_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Add an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   Non-parameter argument \c ies_array:
 * Information elements, may be empty
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_ADVERTISEMENT_ADD_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_advertisement_add_req(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_SERVICE_RECORD_HEADER__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU service_advertisement_add_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to adding an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Status of the add request.
 * type depends on CCP_SERVICE_REASON
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        const CCP_SERVICE_RECORD_HEADER *srh,
        CCP_SERVICE_REASON status);

/**
 * Send CCP PDU service_advertisement_add_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to adding an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Status of the add request.
 * type depends on CCP_SERVICE_REASON
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_advertisement_add_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON(CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_ADD_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU service_advertisement_add_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to adding an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Status of the add request.
 * type depends on CCP_SERVICE_REASON
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_SERVICE_RECORD_HEADER *srh,
        CCP_SERVICE_REASON *status);

/**
 * Analyse received CCP PDU service_advertisement_add_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to adding an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Status of the add request.
 * type depends on CCP_SERVICE_REASON
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_advertisement_add_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU service_advertisement_remove_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Remove an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_SERVICE_RECORD_HEADER(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        const CCP_SERVICE_RECORD_HEADER *srh);

/**
 * Send CCP PDU service_advertisement_remove_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Remove an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_advertisement_remove_req(arg1, arg2) \
    ccp_send_pdu_CCP_SERVICE_RECORD_HEADER(CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_REMOVE_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU service_advertisement_remove_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Remove an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_SERVICE_RECORD_HEADER(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_SERVICE_RECORD_HEADER *srh);

/**
 * Analyse received CCP PDU service_advertisement_remove_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Remove an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_advertisement_remove_req(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_SERVICE_RECORD_HEADER(arg1, arg2, arg3)


/**
 * Send CCP PDU service_advertisement_remove_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to removing an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Status of the remove request.
 * type depends on CCP_SERVICE_REASON
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_advertisement_remove_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON(CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_REMOVE_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU service_advertisement_remove_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to removing an advertisement for a service at run time.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c srh
 * Record header for the advertisement
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Status of the remove request.
 * type depends on CCP_SERVICE_REASON
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_advertisement_remove_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_ADVERTISEMENT_QUERY_REQ_SRH_ARRAY \
    CCP_SERVICE_ADVERTISEMENT_QUERY_REQ_SRH_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_advertisement_query_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Query advertisements for services.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c query_token
 * Arbitrary token to identify query
 *
 *   * \c flags
 * Set of bits from SERVICE_QUERY_FLAGS.
 *
 *   * Non-parameter argument \c srh_array:
 * Array of service record headers for query
 * type depends on CCP_SERVICE_RECORD_HEADER
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_ADVERTISEMENT_QUERY_REQ_SRH_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        uint16 query_token,
        uint16 flags);

/**
 * Send CCP PDU service_advertisement_query_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Query advertisements for services.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c query_token
 * Arbitrary token to identify query
 *
 *   * \c flags
 * Set of bits from SERVICE_QUERY_FLAGS.
 *
 *   * Non-parameter argument \c srh_array:
 * Array of service record headers for query
 * type depends on CCP_SERVICE_RECORD_HEADER
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_ADVERTISEMENT_QUERY_REQ_SRH_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_advertisement_query_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_uint16__uint16__VARLEN(CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_QUERY_REQ, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU service_advertisement_query_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Query advertisements for services.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c query_token
 * Arbitrary token to identify query
 *
 *   * \c flags
 * Set of bits from SERVICE_QUERY_FLAGS.
 *
 *   Non-parameter argument \c srh_array:
 * Array of service record headers for query
 * type depends on CCP_SERVICE_RECORD_HEADER
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_ADVERTISEMENT_QUERY_REQ_SRH_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *query_token,
        uint16 *flags);

/**
 * Analyse received CCP PDU service_advertisement_query_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Query advertisements for services.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c query_token
 * Arbitrary token to identify query
 *
 *   * \c flags
 * Set of bits from SERVICE_QUERY_FLAGS.
 *
 *   Non-parameter argument \c srh_array:
 * Array of service record headers for query
 * type depends on CCP_SERVICE_RECORD_HEADER
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_ADVERTISEMENT_QUERY_REQ_SRH_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_advertisement_query_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16__VARLEN(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_ADVERTISEMENT_QUERY_RSP_DATA \
    CCP_SERVICE_ADVERTISEMENT_QUERY_RSP_DATA_WORD_OFFSET

/**
 * Send CCP PDU service_advertisement_query_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to querying advertisements for services.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c query_token
 * Token sent in original query
 *
 *   * \c status
 * Status of query.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c flags
 * Query flags as set in query
 *
 *   * Non-parameter argument \c data:
 * Data containing the advertisements found by the query.
 * TBD: this will be moved out of bound and this field will become
 * a data reference.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_ADVERTISEMENT_QUERY_RSP_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__CCP_SERVICE_REASON__uint16__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        uint16 query_token,
        CCP_SERVICE_REASON status,
        uint16 flags);

/**
 * Send CCP PDU service_advertisement_query_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to querying advertisements for services.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c query_token
 * Token sent in original query
 *
 *   * \c status
 * Status of query.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c flags
 * Query flags as set in query
 *
 *   * Non-parameter argument \c data:
 * Data containing the advertisements found by the query.
 * TBD: this will be moved out of bound and this field will become
 * a data reference.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_ADVERTISEMENT_QUERY_RSP_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_advertisement_query_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_send_pdu_uint16__CCP_SERVICE_REASON__uint16__VARLEN(CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_QUERY_RSP, arg1, arg2, arg3, arg4, arg5, arg6)

/**
 * Analyse received CCP PDU service_advertisement_query_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to querying advertisements for services.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c query_token
 * Token sent in original query
 *
 *   * \c status
 * Status of query.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c flags
 * Query flags as set in query
 *
 *   Non-parameter argument \c data:
 * Data containing the advertisements found by the query.
 * TBD: this will be moved out of bound and this field will become
 * a data reference.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_ADVERTISEMENT_QUERY_RSP_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__CCP_SERVICE_REASON__uint16__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *query_token,
        CCP_SERVICE_REASON *status,
        uint16 *flags);

/**
 * Analyse received CCP PDU service_advertisement_query_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to querying advertisements for services.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c query_token
 * Token sent in original query
 *
 *   * \c status
 * Status of query.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c flags
 * Query flags as set in query
 *
 *   Non-parameter argument \c data:
 * Data containing the advertisements found by the query.
 * TBD: this will be moved out of bound and this field will become
 * a data reference.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_ADVERTISEMENT_QUERY_RSP_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_advertisement_query_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_uint16__CCP_SERVICE_REASON__uint16__VARLEN(arg1, arg2, arg3, arg4, arg5)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_START_REQ_IES_ARRAY \
    CCP_SERVICE_START_REQ_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_start_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Initial request from a consumer to start a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c srh
 * Record header for service to start.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_START_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_start_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_SERVICE_RECORD_HEADER__VARLEN(CCP_SIGNAL_ID_SERVICE_START_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_start_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Initial request from a consumer to start a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c srh
 * Record header for service to start.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_START_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_start_req(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_SERVICE_RECORD_HEADER__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU service_start_pending_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicates a stag for a service to be started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_STAG__CCP_SERVICE_RECORD_HEADER(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        STAG stag,
        const CCP_SERVICE_RECORD_HEADER *srh);

/**
 * Send CCP PDU service_start_pending_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicates a stag for a service to be started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_start_pending_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_STAG__CCP_SERVICE_RECORD_HEADER(CCP_SIGNAL_ID_SERVICE_START_PENDING_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU service_start_pending_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicates a stag for a service to be started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_STAG__CCP_SERVICE_RECORD_HEADER(
        const uint16 *pdu, uint16 pdu_len_words,
        STAG *stag,
        CCP_SERVICE_RECORD_HEADER *srh);

/**
 * Analyse received CCP PDU service_start_pending_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicates a stag for a service to be started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_start_pending_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_STAG__CCP_SERVICE_RECORD_HEADER(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_ADVICE_IES_ARRAY \
    CCP_SERVICE_ADVICE_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_advice.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicate service to be started when subsystem is running.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c srh
 * Service record header for service to start
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_ADVICE_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_advice(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_SERVICE_RECORD_HEADER__VARLEN(CCP_SIGNAL_ID_SERVICE_ADVICE, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_advice.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicate service to be started when subsystem is running.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c srh
 * Service record header for service to start
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_ADVICE_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_advice(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_SERVICE_RECORD_HEADER__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_STARTING_REQ_IES_ARRAY \
    CCP_SERVICE_STARTING_REQ_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_starting_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Initial request to a provider to start a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_STARTING_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_STAG__CCP_SERVICE_RECORD_HEADER__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        STAG stag,
        const CCP_SERVICE_RECORD_HEADER *srh);

/**
 * Send CCP PDU service_starting_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Initial request to a provider to start a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_STARTING_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_starting_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_STAG__CCP_SERVICE_RECORD_HEADER__VARLEN(CCP_SIGNAL_ID_SERVICE_STARTING_REQ, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU service_starting_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Initial request to a provider to start a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_STARTING_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_STAG__CCP_SERVICE_RECORD_HEADER__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        STAG *stag,
        CCP_SERVICE_RECORD_HEADER *srh);

/**
 * Analyse received CCP PDU service_starting_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Initial request to a provider to start a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_STARTING_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_starting_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_STAG__CCP_SERVICE_RECORD_HEADER__VARLEN(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_STARTING_RSP_IES_ARRAY \
    CCP_SERVICE_STARTING_RSP_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_starting_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_STARTING_REQ.
 * Also a valid rely to SERVICE_STARTING_ACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_STARTING_RSP_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                               uint16 *pdu, uint16 pdu_len_words,
                               STAG stag);

/**
 * Send CCP PDU service_starting_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_STARTING_REQ.
 * Also a valid rely to SERVICE_STARTING_ACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_STARTING_RSP_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_starting_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_STARTING_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_starting_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_STARTING_REQ.
 * Also a valid rely to SERVICE_STARTING_ACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_STARTING_RSP_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_STAG__VARLEN(const uint16 *pdu, uint16 pdu_len_words,
                               STAG *stag);

/**
 * Analyse received CCP PDU service_starting_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_STARTING_REQ.
 * Also a valid rely to SERVICE_STARTING_ACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_STARTING_RSP_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_starting_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_STARTING_ACTIVATION_REQ_IES_ARRAY \
    CCP_SERVICE_STARTING_ACTIVATION_REQ_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_starting_activation_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request to a provider to start and activate a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_STARTING_ACTIVATION_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_starting_activation_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_STAG__CCP_SERVICE_RECORD_HEADER__VARLEN(CCP_SIGNAL_ID_SERVICE_STARTING_ACTIVATION_REQ, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU service_starting_activation_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request to a provider to start and activate a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for service being started.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_STARTING_ACTIVATION_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_starting_activation_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_STAG__CCP_SERVICE_RECORD_HEADER__VARLEN(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_INITIATING_CONSUMER_REQ_IES_ARRAY \
    CCP_SERVICE_INITIATING_CONSUMER_REQ_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_initiating_consumer_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicates values for service start being negotiated.
 * Passed to consumer.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_INITIATING_CONSUMER_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_initiating_consumer_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_INITIATING_CONSUMER_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_initiating_consumer_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicates values for service start being negotiated.
 * Passed to consumer.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_INITIATING_CONSUMER_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_initiating_consumer_req(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_INITIATING_CONSUMER_RSP_IES_ARRAY \
    CCP_SERVICE_INITIATING_CONSUMER_RSP_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_initiating_consumer_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_INITIATING_CONSUMER_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_INITIATING_CONSUMER_RSP_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_initiating_consumer_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_INITIATING_CONSUMER_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_initiating_consumer_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_INITIATING_CONSUMER_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_INITIATING_CONSUMER_RSP_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_initiating_consumer_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_ACTIVATION_REQ_IES_ARRAY \
    CCP_SERVICE_ACTIVATION_REQ_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_activation_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request provider to activate a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_ACTIVATION_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_activation_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_ACTIVATION_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_activation_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request provider to activate a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_ACTIVATION_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_activation_req(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_ACTIVATION_RSP_IES_ARRAY \
    CCP_SERVICE_ACTIVATION_RSP_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_activation_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_ACTIVATION_REQ and SERVICE_STARTING_ACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_ACTIVATION_RSP_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_activation_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_ACTIVATION_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_activation_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_ACTIVATION_REQ and SERVICE_STARTING_ACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_ACTIVATION_RSP_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_activation_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_START_CFM_IES_ARRAY \
    CCP_SERVICE_START_CFM_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_start_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Confirm to the consumer that a service has been started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_START_CFM_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_start_cfm(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_START_CFM, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_start_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Confirm to the consumer that a service has been started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_START_CFM_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_start_cfm(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU service_start_failure_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Explain gently to the consumer that a service has not been started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for failed service.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_STAG__CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        STAG stag,
        const CCP_SERVICE_RECORD_HEADER *srh,
        CCP_SERVICE_REASON status,
        uint16 detail);

/**
 * Send CCP PDU service_start_failure_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Explain gently to the consumer that a service has not been started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for failed service.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_start_failure_cfm(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_STAG__CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_START_FAILURE_CFM, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU service_start_failure_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Explain gently to the consumer that a service has not been started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for failed service.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_STAG__CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        STAG *stag,
        CCP_SERVICE_RECORD_HEADER *srh,
        CCP_SERVICE_REASON *status,
        uint16 *detail);

/**
 * Analyse received CCP PDU service_start_failure_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Explain gently to the consumer that a service has not been started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c srh
 * Service record header for failed service.
 * type depends on CCP_SERVICE_RECORD_HEADER
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_start_failure_cfm(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_recv_pdu_STAG__CCP_SERVICE_RECORD_HEADER__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5, arg6)


/**
 * Send CCP PDU service_starting_failure_inf.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicated to the provider that a service will not after all be started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        STAG stag,
        CCP_SERVICE_REASON status,
        uint16 detail);

/**
 * Send CCP PDU service_starting_failure_inf.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicated to the provider that a service will not after all be started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_starting_failure_inf(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_STARTING_FAILURE_INF, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_starting_failure_inf.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicated to the provider that a service will not after all be started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        STAG *stag,
        CCP_SERVICE_REASON *status,
        uint16 *detail);

/**
 * Analyse received CCP PDU service_starting_failure_inf.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicated to the provider that a service will not after all be started.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_starting_failure_inf(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU service_starting_failure_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to service starting/initiation/activation indicating failure.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_starting_failure_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_STARTING_FAILURE_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_starting_failure_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to service starting/initiation/activation indicating failure.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service couldn't be started.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_starting_failure_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_AUX_CMD_CONSUMER_REQ_DATA \
    CCP_SERVICE_AUX_CMD_CONSUMER_REQ_DATA_WORD_OFFSET

/**
 * Send CCP PDU service_aux_cmd_consumer_req.
 *
 * DIR TO_CURATOR
 * Pass an auxiliary command message from consumer.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c data:
 * Opaque data to be forwarded to the provider.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_AUX_CMD_CONSUMER_REQ_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_aux_cmd_consumer_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_AUX_CMD_CONSUMER_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_aux_cmd_consumer_req.
 *
 * DIR TO_CURATOR
 * Pass an auxiliary command message from consumer.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c data:
 * Opaque data to be forwarded to the provider.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_AUX_CMD_CONSUMER_REQ_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_aux_cmd_consumer_req(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_AUX_CMD_PROVIDER_INF_DATA \
    CCP_SERVICE_AUX_CMD_PROVIDER_INF_DATA_WORD_OFFSET

/**
 * Send CCP PDU service_aux_cmd_provider_inf.
 *
 * DIR FROM_CURATOR
 * Pass an auxiliary command message to provider.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c data:
 * Opaque data from the consumer.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_AUX_CMD_PROVIDER_INF_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_aux_cmd_provider_inf(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_AUX_CMD_PROVIDER_INF, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_aux_cmd_provider_inf.
 *
 * DIR FROM_CURATOR
 * Pass an auxiliary command message to provider.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c data:
 * Opaque data from the consumer.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_AUX_CMD_PROVIDER_INF_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_aux_cmd_provider_inf(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU service_stop_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request from the consumer to stop a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_stop_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_STOP_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_stop_req.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request from the consumer to stop a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_stop_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_DEACTIVATION_REQ_IES_ARRAY \
    CCP_SERVICE_DEACTIVATION_REQ_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_deactivation_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request provider of autostart service to stop it.
 * Do not yet free any resources, however.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements relating to deactivating the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_DEACTIVATION_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        STAG stag,
        CCP_SERVICE_REASON status,
        uint16 detail);

/**
 * Send CCP PDU service_deactivation_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request provider of autostart service to stop it.
 * Do not yet free any resources, however.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements relating to deactivating the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_DEACTIVATION_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_deactivation_req(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16__VARLEN(CCP_SIGNAL_ID_SERVICE_DEACTIVATION_REQ, arg1, arg2, arg3, arg4, arg5, arg6)

/**
 * Analyse received CCP PDU service_deactivation_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request provider of autostart service to stop it.
 * Do not yet free any resources, however.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements relating to deactivating the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_DEACTIVATION_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        STAG *stag,
        CCP_SERVICE_REASON *status,
        uint16 *detail);

/**
 * Analyse received CCP PDU service_deactivation_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request provider of autostart service to stop it.
 * Do not yet free any resources, however.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements relating to deactivating the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_DEACTIVATION_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_deactivation_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16__VARLEN(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU service_deactivation_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_DEACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_STAG(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                       STAG stag);

/**
 * Send CCP PDU service_deactivation_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_DEACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_deactivation_rsp(arg1, arg2) \
    ccp_send_pdu_STAG(CCP_SIGNAL_ID_SERVICE_DEACTIVATION_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU service_deactivation_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_DEACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_STAG(const uint16 *pdu, uint16 pdu_len_words,
                       STAG *stag);

/**
 * Analyse received CCP PDU service_deactivation_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_DEACTIVATION_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_deactivation_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG(arg1, arg2, arg3)


/**
 * Send CCP PDU service_stopping_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request to the provider to stop a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_stopping_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_STOPPING_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_stopping_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request to the provider to stop a service.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_stopping_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU service_stopping_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_STOPPING_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_stopping_rsp(arg1, arg2) \
    ccp_send_pdu_STAG(CCP_SIGNAL_ID_SERVICE_STOPPING_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU service_stopping_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_STOPPING_REQ.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_stopping_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG(arg1, arg2, arg3)


/**
 * Send CCP PDU service_stop_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicate to consumer service is stopped.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_stop_cfm(arg1, arg2) \
    ccp_send_pdu_STAG(CCP_SIGNAL_ID_SERVICE_STOP_CFM, arg1, arg2)

/**
 * Analyse received CCP PDU service_stop_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicate to consumer service is stopped.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_stop_cfm(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG(arg1, arg2, arg3)


/**
 * Send CCP PDU service_stop_failure_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicate to consumer service could not be stopped.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service could not be stopped.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_stop_failure_cfm(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_STOP_FAILURE_CFM, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_stop_failure_cfm.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicate to consumer service could not be stopped.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service could not be stopped.
 * type depends on CCP_SERVICE_REASON
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_stop_failure_cfm(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU service_stopping_failure_inf.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Reply to an invalid stopping message.
 * This means globserv couldn't understand a message about service
 * stopping.  See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service could not be stopped.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_stopping_failure_inf(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_STOPPING_FAILURE_INF, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_stopping_failure_inf.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Reply to an invalid stopping message.
 * This means globserv couldn't understand a message about service
 * stopping.  See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service could not be stopped.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_stopping_failure_inf(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU service_aux_cmd_consumer_failure_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Failure reply to SERVICE_AUX_CMD_CONSUMER_REQ.
 * There is no reply to a successful request; the message is
 * simply forwarded
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why the message failed.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_aux_cmd_consumer_failure_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_AUX_CMD_CONSUMER_FAILURE_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_aux_cmd_consumer_failure_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Failure reply to SERVICE_AUX_CMD_CONSUMER_REQ.
 * There is no reply to a successful request; the message is
 * simply forwarded
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why the message failed.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_aux_cmd_consumer_failure_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU service_stopping_failure_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Failure reply to SERVICE_STOPPING_REQ.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service could not be stopped.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_stopping_failure_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_STOPPING_FAILURE_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_stopping_failure_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Failure reply to SERVICE_STOPPING_REQ.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service could not be stopped.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_stopping_failure_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_INITIATING_PROVIDER_REQ_IES_ARRAY \
    CCP_SERVICE_INITIATING_PROVIDER_REQ_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_initiating_provider_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicates values for service start being negotiated.
 * Passed to provider.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_INITIATING_PROVIDER_REQ_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_initiating_provider_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_INITIATING_PROVIDER_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_initiating_provider_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Indicates values for service start being negotiated.
 * Passed to provider.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_INITIATING_PROVIDER_REQ_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_initiating_provider_req(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_INITIATING_PROVIDER_RSP_IES_ARRAY \
    CCP_SERVICE_INITIATING_PROVIDER_RSP_IES_ARRAY_WORD_OFFSET

/**
 * Send CCP PDU service_initiating_provider_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_INITIATING_PROVIDER_REQ.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_INITIATING_PROVIDER_RSP_IES_ARRAY_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_initiating_provider_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_INITIATING_PROVIDER_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_initiating_provider_rsp.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Response to SERVICE_INITIATING_PROVIDER_REQ.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c ies_array:
 * Information elements for the service.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_INITIATING_PROVIDER_RSP_IES_ARRAY_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_initiating_provider_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU service_terminated_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request to the consumer to stop abnormally terminated service.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_terminated_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_TERMINATED_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_terminated_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request to the consumer to stop abnormally terminated service.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why service is being stopped, or SUCCESS.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_terminated_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU service_terminated_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request to the consumer to stop abnormally terminated service.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_terminated_rsp(arg1, arg2) \
    ccp_send_pdu_STAG(CCP_SIGNAL_ID_SERVICE_TERMINATED_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU service_terminated_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Request to the consumer to stop abnormally terminated service.
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_terminated_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_AUX_CMD_PROVIDER_REQ_DATA \
    CCP_SERVICE_AUX_CMD_PROVIDER_REQ_DATA_WORD_OFFSET

/**
 * Send CCP PDU service_aux_cmd_provider_req.
 *
 * DIR TO_CURATOR
 * Pass an auxiliary command message from provider to consumer.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c data:
 * Opaque data to be forwarded to the consumer.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_AUX_CMD_PROVIDER_REQ_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_aux_cmd_provider_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_AUX_CMD_PROVIDER_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_aux_cmd_provider_req.
 *
 * DIR TO_CURATOR
 * Pass an auxiliary command message from provider to consumer.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c data:
 * Opaque data to be forwarded to the consumer.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_AUX_CMD_PROVIDER_REQ_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_aux_cmd_provider_req(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SERVICE_AUX_CMD_CONSUMER_INF_DATA \
    CCP_SERVICE_AUX_CMD_CONSUMER_INF_DATA_WORD_OFFSET

/**
 * Send CCP PDU service_aux_cmd_consumer_inf.
 *
 * DIR FROM_CURATOR
 * Pass an auxiliary command message from provider to consumer.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * Non-parameter argument \c data:
 * Opaque data from the provider for the consumer.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SERVICE_AUX_CMD_CONSUMER_INF_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_aux_cmd_consumer_inf(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__VARLEN(CCP_SIGNAL_ID_SERVICE_AUX_CMD_CONSUMER_INF, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_aux_cmd_consumer_inf.
 *
 * DIR FROM_CURATOR
 * Pass an auxiliary command message from provider to consumer.
 * See CS-208980-DD for service management interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   Non-parameter argument \c data:
 * Opaque data from the provider for the consumer.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SERVICE_AUX_CMD_CONSUMER_INF_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_aux_cmd_consumer_inf(arg1, arg2, arg3) \
    ccp_recv_pdu_STAG__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU service_aux_cmd_provider_failure_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Failure reply to SERVICE_AUX_CMD_PROVIDER_REQ.
 * There is no reply to a successful request; the message is
 * simply forwarded
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why passing the message failed.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_aux_cmd_provider_failure_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_STAG__CCP_SERVICE_REASON__uint16(CCP_SIGNAL_ID_SERVICE_AUX_CMD_PROVIDER_FAILURE_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU service_aux_cmd_provider_failure_rsp.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Failure reply to SERVICE_AUX_CMD_PROVIDER_REQ.
 * There is no reply to a successful request; the message is
 * simply forwarded
 * See CS-208980-DD for service managment interface.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service.
 *
 *   * \c status
 * Reason why passing the message failed.
 *
 *   * \c detail
 * Any applicable detail for the status.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_aux_cmd_provider_failure_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_STAG__CCP_SERVICE_REASON__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU subsystem_panic.
 *
 * DIR TO_CURATOR
 * Panic report sent unreliably from subsystem when it panics.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c id
 * Panic code private to subsystem.
 * The range 0x1000 - 0x3fff is used by Hydra common panic codes
 *
 *   * \c arg
 * Argument providing extra detail about the panic.
 * Set to zero if no extra detail is provided
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_panic(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID_SUBSYSTEM_PANIC, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU subsystem_panic.
 *
 * DIR TO_CURATOR
 * Panic report sent unreliably from subsystem when it panics.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c id
 * Panic code private to subsystem.
 * The range 0x1000 - 0x3fff is used by Hydra common panic codes
 *
 *   * \c arg
 * Argument providing extra detail about the panic.
 * Set to zero if no extra detail is provided
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_panic(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU subsystem_fault.
 *
 * DIR TO_CURATOR
 * Report from subsystem that a non-fatal error has occurred
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c id
 * Fault code private to subsystem.
 * The range 0x1000 - 0x3fff is used by Hydra common fault codes
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the fault occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the fault recorded since
 * the previous report, or since the earliest in the database
 * as yet unreported.  Not all occurrences of the fault necessary
 * have the same arg value.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first fault reported occurred.
 *
 *   * \c arg
 * Arbitrary extra detail supplied with the fault
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16__uint16__TIME__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 id,
        uint16 cpu,
        uint16 occurrences,
        TIME timestamp,
        uint16 arg);

/**
 * Send CCP PDU subsystem_fault.
 *
 * DIR TO_CURATOR
 * Report from subsystem that a non-fatal error has occurred
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c id
 * Fault code private to subsystem.
 * The range 0x1000 - 0x3fff is used by Hydra common fault codes
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the fault occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the fault recorded since
 * the previous report, or since the earliest in the database
 * as yet unreported.  Not all occurrences of the fault necessary
 * have the same arg value.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first fault reported occurred.
 *
 *   * \c arg
 * Arbitrary extra detail supplied with the fault
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_fault(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_send_pdu_uint16__uint16__uint16__TIME__uint16(CCP_SIGNAL_ID_SUBSYSTEM_FAULT, arg1, arg2, arg3, arg4, arg5, arg6)

/**
 * Analyse received CCP PDU subsystem_fault.
 *
 * DIR TO_CURATOR
 * Report from subsystem that a non-fatal error has occurred
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c id
 * Fault code private to subsystem.
 * The range 0x1000 - 0x3fff is used by Hydra common fault codes
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the fault occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the fault recorded since
 * the previous report, or since the earliest in the database
 * as yet unreported.  Not all occurrences of the fault necessary
 * have the same arg value.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first fault reported occurred.
 *
 *   * \c arg
 * Arbitrary extra detail supplied with the fault
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16__uint16__TIME__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *id,
        uint16 *cpu,
        uint16 *occurrences,
        TIME *timestamp,
        uint16 *arg);

/**
 * Analyse received CCP PDU subsystem_fault.
 *
 * DIR TO_CURATOR
 * Report from subsystem that a non-fatal error has occurred
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c id
 * Fault code private to subsystem.
 * The range 0x1000 - 0x3fff is used by Hydra common fault codes
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the fault occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the fault recorded since
 * the previous report, or since the earliest in the database
 * as yet unreported.  Not all occurrences of the fault necessary
 * have the same arg value.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first fault reported occurred.
 *
 *   * \c arg
 * Arbitrary extra detail supplied with the fault
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_fault(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    ccp_recv_pdu_uint16__uint16__uint16__TIME__uint16(arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SUBSYSTEM_EVENT_REPORT_ARG \
    CCP_SUBSYSTEM_EVENT_REPORT_ARG_WORD_OFFSET

/**
 * Send CCP PDU subsystem_event_report.
 *
 * DIR TO_CURATOR
 * Report from subsystem that an event has occurred.
 * Events are a generalisation of panic (level 0),
 * fault (level 1) and subsystem watchdog (level 2).
 * Note the ordering does not necessarily indicate severity.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c level
 * Severity level of the event
 *
 *   * \c id
 * Identifier for the event in question
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the event occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the event being reported here.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first event reported occurred.
 *
 *   * Non-parameter argument \c arg:
 * Arbitrary extra detail supplied with the event
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SUBSYSTEM_EVENT_REPORT_ARG_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_EVENT_LEVEL__uint16__uint16__uint16__TIME__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        CCP_EVENT_LEVEL level,
        uint16 id,
        uint16 cpu,
        uint16 occurrences,
        TIME timestamp);

/**
 * Send CCP PDU subsystem_event_report.
 *
 * DIR TO_CURATOR
 * Report from subsystem that an event has occurred.
 * Events are a generalisation of panic (level 0),
 * fault (level 1) and subsystem watchdog (level 2).
 * Note the ordering does not necessarily indicate severity.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c level
 * Severity level of the event
 *
 *   * \c id
 * Identifier for the event in question
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the event occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the event being reported here.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first event reported occurred.
 *
 *   * Non-parameter argument \c arg:
 * Arbitrary extra detail supplied with the event
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SUBSYSTEM_EVENT_REPORT_ARG_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_event_report(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    ccp_send_pdu_CCP_EVENT_LEVEL__uint16__uint16__uint16__TIME__VARLEN(CCP_SIGNAL_ID_SUBSYSTEM_EVENT_REPORT, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)

/**
 * Analyse received CCP PDU subsystem_event_report.
 *
 * DIR TO_CURATOR
 * Report from subsystem that an event has occurred.
 * Events are a generalisation of panic (level 0),
 * fault (level 1) and subsystem watchdog (level 2).
 * Note the ordering does not necessarily indicate severity.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c level
 * Severity level of the event
 *
 *   * \c id
 * Identifier for the event in question
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the event occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the event being reported here.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first event reported occurred.
 *
 *   Non-parameter argument \c arg:
 * Arbitrary extra detail supplied with the event
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SUBSYSTEM_EVENT_REPORT_ARG_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_EVENT_LEVEL__uint16__uint16__uint16__TIME__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_EVENT_LEVEL *level,
        uint16 *id,
        uint16 *cpu,
        uint16 *occurrences,
        TIME *timestamp);

/**
 * Analyse received CCP PDU subsystem_event_report.
 *
 * DIR TO_CURATOR
 * Report from subsystem that an event has occurred.
 * Events are a generalisation of panic (level 0),
 * fault (level 1) and subsystem watchdog (level 2).
 * Note the ordering does not necessarily indicate severity.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c level
 * Severity level of the event
 *
 *   * \c id
 * Identifier for the event in question
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the event occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the event being reported here.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first event reported occurred.
 *
 *   Non-parameter argument \c arg:
 * Arbitrary extra detail supplied with the event
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SUBSYSTEM_EVENT_REPORT_ARG_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_event_report(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    ccp_recv_pdu_CCP_EVENT_LEVEL__uint16__uint16__uint16__TIME__VARLEN(arg1, arg2, arg3, arg4, arg5, arg6, arg7)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_HOST_EVENT_REPORT_ARG \
    CCP_HOST_EVENT_REPORT_ARG_WORD_OFFSET

/**
 * Send CCP PDU host_event_report.
 *
 * DIR FROM_CURATOR
 * Report from Curator to host that an event has occurred.
 * Events are a generalisation of panic (level 0),
 * fault (level 1) and subsystem watchdog (level 2).
 * Not the ordering does not necesarily indicate severity.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * The type of the subsystem reporting the event.
 * This is one of the values from the enumeration
 * SYSTEM_SUBSYSTEM_TYPE in the system.xml interface.
 *
 *   * \c level
 * Severity level of the event
 *
 *   * \c id
 * Identifier for the event in question
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the event occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the event being reported here.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first event reported occurred.
 *
 *   * Non-parameter argument \c arg:
 * Arbitrary extra detail supplied with the event
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_HOST_EVENT_REPORT_ARG_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_SYSTEM_SUBSYSTEM_TYPE__CCP_EVENT_LEVEL__uint16__uint16__uint16__TIME__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        SYSTEM_SUBSYSTEM_TYPE subsystem,
        CCP_EVENT_LEVEL level,
        uint16 id,
        uint16 cpu,
        uint16 occurrences,
        TIME timestamp);

/**
 * Send CCP PDU host_event_report.
 *
 * DIR FROM_CURATOR
 * Report from Curator to host that an event has occurred.
 * Events are a generalisation of panic (level 0),
 * fault (level 1) and subsystem watchdog (level 2).
 * Not the ordering does not necesarily indicate severity.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * The type of the subsystem reporting the event.
 * This is one of the values from the enumeration
 * SYSTEM_SUBSYSTEM_TYPE in the system.xml interface.
 *
 *   * \c level
 * Severity level of the event
 *
 *   * \c id
 * Identifier for the event in question
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the event occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the event being reported here.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first event reported occurred.
 *
 *   * Non-parameter argument \c arg:
 * Arbitrary extra detail supplied with the event
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_HOST_EVENT_REPORT_ARG_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_host_event_report(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
    ccp_send_pdu_SYSTEM_SUBSYSTEM_TYPE__CCP_EVENT_LEVEL__uint16__uint16__uint16__TIME__VARLEN(CCP_SIGNAL_ID_HOST_EVENT_REPORT, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

/**
 * Analyse received CCP PDU host_event_report.
 *
 * DIR FROM_CURATOR
 * Report from Curator to host that an event has occurred.
 * Events are a generalisation of panic (level 0),
 * fault (level 1) and subsystem watchdog (level 2).
 * Not the ordering does not necesarily indicate severity.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The type of the subsystem reporting the event.
 * This is one of the values from the enumeration
 * SYSTEM_SUBSYSTEM_TYPE in the system.xml interface.
 *
 *   * \c level
 * Severity level of the event
 *
 *   * \c id
 * Identifier for the event in question
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the event occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the event being reported here.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first event reported occurred.
 *
 *   Non-parameter argument \c arg:
 * Arbitrary extra detail supplied with the event
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_HOST_EVENT_REPORT_ARG_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_SYSTEM_SUBSYSTEM_TYPE__CCP_EVENT_LEVEL__uint16__uint16__uint16__TIME__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        SYSTEM_SUBSYSTEM_TYPE *subsystem,
        CCP_EVENT_LEVEL *level,
        uint16 *id,
        uint16 *cpu,
        uint16 *occurrences,
        TIME *timestamp);

/**
 * Analyse received CCP PDU host_event_report.
 *
 * DIR FROM_CURATOR
 * Report from Curator to host that an event has occurred.
 * Events are a generalisation of panic (level 0),
 * fault (level 1) and subsystem watchdog (level 2).
 * Not the ordering does not necesarily indicate severity.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The type of the subsystem reporting the event.
 * This is one of the values from the enumeration
 * SYSTEM_SUBSYSTEM_TYPE in the system.xml interface.
 *
 *   * \c level
 * Severity level of the event
 *
 *   * \c id
 * Identifier for the event in question
 *
 *   * \c cpu
 * The CPU, numbered from 0, on which the event occurred.
 * Multi-CPU subsystems have their own private numbering.
 * Other subsystems used 0.
 *
 *   * \c occurrences
 * The number of occurrences of the event being reported here.
 *
 *   * \c timestamp
 * The timestamp on the system's microsecond clock when
 * the first event reported occurred.
 *
 *   Non-parameter argument \c arg:
 * Arbitrary extra detail supplied with the event
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_HOST_EVENT_REPORT_ARG_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_host_event_report(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
    ccp_recv_pdu_SYSTEM_SUBSYSTEM_TYPE__CCP_EVENT_LEVEL__uint16__uint16__uint16__TIME__VARLEN(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)


/**
 * Send CCP PDU chip_init_req.
 *
 * DIR TO_CURATOR
 * Request the chip to start. The response is returned once
 * the curator has downloaded the patch and config data
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_chip_init_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_CHIP_INIT_REQ, arg1)

/**
 * Analyse received CCP PDU chip_init_req.
 *
 * DIR TO_CURATOR
 * Request the chip to start. The response is returned once
 * the curator has downloaded the patch and config data
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_chip_init_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU chip_init_rsp.
 *
 * DIR FROM_CURATOR
 * Indicates that the curator has started and completed
 * downloading the patch and config data
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c protocol_version
 * The version of the CCP protocol implemented by the firmware.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_PROTOCOL_VERSION(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                       CCP_PROTOCOL_VERSION protocol_version);

/**
 * Send CCP PDU chip_init_rsp.
 *
 * DIR FROM_CURATOR
 * Indicates that the curator has started and completed
 * downloading the patch and config data
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c protocol_version
 * The version of the CCP protocol implemented by the firmware.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_chip_init_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_PROTOCOL_VERSION(CCP_SIGNAL_ID_CHIP_INIT_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU chip_init_rsp.
 *
 * DIR FROM_CURATOR
 * Indicates that the curator has started and completed
 * downloading the patch and config data
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c protocol_version
 * The version of the CCP protocol implemented by the firmware.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_PROTOCOL_VERSION(const uint16 *pdu, uint16 pdu_len_words,
                                       CCP_PROTOCOL_VERSION *protocol_version);

/**
 * Analyse received CCP PDU chip_init_rsp.
 *
 * DIR FROM_CURATOR
 * Indicates that the curator has started and completed
 * downloading the patch and config data
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c protocol_version
 * The version of the CCP protocol implemented by the firmware.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_chip_init_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_PROTOCOL_VERSION(arg1, arg2, arg3)


/**
 * Send CCP PDU warm_restart_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to request a warm restart
 * (e.g. BT SS will send this after patching is complete).
 * CCP channel will need to be re-established.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_warm_restart_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_WARM_RESTART_REQ, arg1)

/**
 * Analyse received CCP PDU warm_restart_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to request a warm restart
 * (e.g. BT SS will send this after patching is complete).
 * CCP channel will need to be re-established.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_warm_restart_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU self_restart_ind.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate that it is restarting.
 * (e.g. WIFI SS will send this after patching is complete, just before
 * it jumps to the main image it has just loaded).
 * CCP channel will need to be re-established.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_self_restart_ind(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_SELF_RESTART_IND, arg1)

/**
 * Analyse received CCP PDU self_restart_ind.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate that it is restarting.
 * (e.g. WIFI SS will send this after patching is complete, just before
 * it jumps to the main image it has just loaded).
 * CCP channel will need to be re-established.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_self_restart_ind(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU operational_ind.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate that the firmware
 * is now configured (e.g. aftrer patching & rebooting) and it is ready
 * to receive service requests etc.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c patched_fw_version
 * The level to which the firmware being started has been patched.
 * This is the fw version of the subsystem after it has been
 * patched, or 0xFFFFFFFF if the subsystem has not yet been started.
 * Zero indicates the subsystem was started but no patch was
 * available.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_operational_ind(arg1, arg2) \
    ccp_send_pdu_uint32(CCP_SIGNAL_ID_OPERATIONAL_IND, arg1, arg2)

/**
 * Analyse received CCP PDU operational_ind.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate that the firmware
 * is now configured (e.g. aftrer patching & rebooting) and it is ready
 * to receive service requests etc.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c patched_fw_version
 * The level to which the firmware being started has been patched.
 * This is the fw version of the subsystem after it has been
 * patched, or 0xFFFFFFFF if the subsystem has not yet been started.
 * Zero indicates the subsystem was started but no patch was
 * available.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_operational_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint32(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_TUNNEL_DEBUG_CMD_PAYLOAD \
    CCP_TUNNEL_DEBUG_CMD_PAYLOAD_WORD_OFFSET

/**
 * Send CCP PDU tunnel_debug_cmd.
 *
 * DIR BOTH
 * Temporary messages needed as a quick method of setting
 * MIB items and poking values into embedded digits registers
 * for imminent NFC ROM release.
 * file.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * Non-parameter argument \c Payload:
 * A debug command message as defined in debug_cmd.xml
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_TUNNEL_DEBUG_CMD_PAYLOAD_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_VARLEN(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                         uint16 *pdu, uint16 pdu_len_words);

/**
 * Send CCP PDU tunnel_debug_cmd.
 *
 * DIR BOTH
 * Temporary messages needed as a quick method of setting
 * MIB items and poking values into embedded digits registers
 * for imminent NFC ROM release.
 * file.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * Non-parameter argument \c Payload:
 * A debug command message as defined in debug_cmd.xml
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_TUNNEL_DEBUG_CMD_PAYLOAD_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_tunnel_debug_cmd(arg1, arg2, arg3) \
    ccp_send_pdu_VARLEN(CCP_SIGNAL_ID_TUNNEL_DEBUG_CMD, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU tunnel_debug_cmd.
 *
 * DIR BOTH
 * Temporary messages needed as a quick method of setting
 * MIB items and poking values into embedded digits registers
 * for imminent NFC ROM release.
 * file.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   Non-parameter argument \c Payload:
 * A debug command message as defined in debug_cmd.xml
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_TUNNEL_DEBUG_CMD_PAYLOAD_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_VARLEN(const uint16 *pdu, uint16 pdu_len_words);

/**
 * Analyse received CCP PDU tunnel_debug_cmd.
 *
 * DIR BOTH
 * Temporary messages needed as a quick method of setting
 * MIB items and poking values into embedded digits registers
 * for imminent NFC ROM release.
 * file.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   Non-parameter argument \c Payload:
 * A debug command message as defined in debug_cmd.xml
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_TUNNEL_DEBUG_CMD_PAYLOAD_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_tunnel_debug_cmd(arg1, arg2) \
    ccp_recv_pdu_VARLEN(arg1, arg2)


/**
 * Send CCP PDU create_host_isp_link_req.
 *
 * DIR BOTH
 * Chip internal message used by a subsystem to request that the
 * curator create an isp link to the subsystem so it can send pdus
 * to the host. This is needed by protocols such as MIBCMD.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c Isp_Port
 * isp port number for link
 *
 *   * \c Link_Type
 * Whether link is out_band
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__CCP_ISP_LINK_TYPE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 Isp_Port,
        CCP_ISP_LINK_TYPE Link_Type);

/**
 * Send CCP PDU create_host_isp_link_req.
 *
 * DIR BOTH
 * Chip internal message used by a subsystem to request that the
 * curator create an isp link to the subsystem so it can send pdus
 * to the host. This is needed by protocols such as MIBCMD.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c Isp_Port
 * isp port number for link
 *
 *   * \c Link_Type
 * Whether link is out_band
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_create_host_isp_link_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__CCP_ISP_LINK_TYPE(CCP_SIGNAL_ID_CREATE_HOST_ISP_LINK_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU create_host_isp_link_req.
 *
 * DIR BOTH
 * Chip internal message used by a subsystem to request that the
 * curator create an isp link to the subsystem so it can send pdus
 * to the host. This is needed by protocols such as MIBCMD.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c Isp_Port
 * isp port number for link
 *
 *   * \c Link_Type
 * Whether link is out_band
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__CCP_ISP_LINK_TYPE(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *Isp_Port,
        CCP_ISP_LINK_TYPE *Link_Type);

/**
 * Analyse received CCP PDU create_host_isp_link_req.
 *
 * DIR BOTH
 * Chip internal message used by a subsystem to request that the
 * curator create an isp link to the subsystem so it can send pdus
 * to the host. This is needed by protocols such as MIBCMD.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c Isp_Port
 * isp port number for link
 *
 *   * \c Link_Type
 * Whether link is out_band
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_create_host_isp_link_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__CCP_ISP_LINK_TYPE(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU create_host_isp_link_rsp.
 *
 * DIR BOTH
 * Chip internal message sent by the curator to a subsystem in
 * response to the Ccp_Create_Host_Link_Req to indicate that the
 * isp link creation has been initiated and the subsystem can now
 * create and activate its end of the link.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_create_host_isp_link_rsp(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_CREATE_HOST_ISP_LINK_RSP, arg1)

/**
 * Analyse received CCP PDU create_host_isp_link_rsp.
 *
 * DIR BOTH
 * Chip internal message sent by the curator to a subsystem in
 * response to the Ccp_Create_Host_Link_Req to indicate that the
 * isp link creation has been initiated and the subsystem can now
 * create and activate its end of the link.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_create_host_isp_link_rsp(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU watchdog_chip_ping_req.
 *
 * DIR TO_CURATOR
 * Request by the host for the Curator to confirm that the chip is
 * still functioning.  The Curator shall return to its main
 * scheduler and return Ccp_Watchdog_Chip_Ping_Rsp to the host.
 * 
 * This signal may be used by the host as a more efficient
 * alternative to the chip's built-in watchdog circuite.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_watchdog_chip_ping_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_WATCHDOG_CHIP_PING_REQ, arg1)

/**
 * Analyse received CCP PDU watchdog_chip_ping_req.
 *
 * DIR TO_CURATOR
 * Request by the host for the Curator to confirm that the chip is
 * still functioning.  The Curator shall return to its main
 * scheduler and return Ccp_Watchdog_Chip_Ping_Rsp to the host.
 * 
 * This signal may be used by the host as a more efficient
 * alternative to the chip's built-in watchdog circuite.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_watchdog_chip_ping_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU watchdog_chip_ping_rsp.
 *
 * DIR FROM_CURATOR
 * Response from the Curator to the host to
 * Ccp_Watchdog_Chip_Ping_Req.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_watchdog_chip_ping_rsp(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_WATCHDOG_CHIP_PING_RSP, arg1)

/**
 * Analyse received CCP PDU watchdog_chip_ping_rsp.
 *
 * DIR FROM_CURATOR
 * Response from the Curator to the host to
 * Ccp_Watchdog_Chip_Ping_Req.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_watchdog_chip_ping_rsp(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU watchdog_subsystem_ping_req.
 *
 * DIR FROM_CURATOR
 * Request by the Curator for a subsystem to confirm that it is
 * still functioning.  The subsystem shall return to its main
 * scheduler and return Ccp_Watchdog_Subsystem_Ping_Rsp.
 * 
 * In normal operation this signal is not used by the host, however
 * for the purpose of debugging the host may send this to a
 * subsystem, typically using standard ISP routing via the Curator.
 * To this end the subsystem shall ensure the response is sent
 * back to the originator, not necessarily the Curator.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_watchdog_subsystem_ping_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_WATCHDOG_SUBSYSTEM_PING_REQ, arg1)

/**
 * Analyse received CCP PDU watchdog_subsystem_ping_req.
 *
 * DIR FROM_CURATOR
 * Request by the Curator for a subsystem to confirm that it is
 * still functioning.  The subsystem shall return to its main
 * scheduler and return Ccp_Watchdog_Subsystem_Ping_Rsp.
 * 
 * In normal operation this signal is not used by the host, however
 * for the purpose of debugging the host may send this to a
 * subsystem, typically using standard ISP routing via the Curator.
 * To this end the subsystem shall ensure the response is sent
 * back to the originator, not necessarily the Curator.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_watchdog_subsystem_ping_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU watchdog_subsystem_ping_rsp.
 *
 * DIR TO_CURATOR
 * Response from a subsystem to the originator of a
 * Ccp_Watchdog_Subsystem_Ping_Req.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_watchdog_subsystem_ping_rsp(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_WATCHDOG_SUBSYSTEM_PING_RSP, arg1)

/**
 * Analyse received CCP PDU watchdog_subsystem_ping_rsp.
 *
 * DIR TO_CURATOR
 * Response from a subsystem to the originator of a
 * Ccp_Watchdog_Subsystem_Ping_Req.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_watchdog_subsystem_ping_rsp(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU subsystem_crashed_ind.
 *
 * DIR FROM_CURATOR
 * Notification from curator to host that a subsystem crash has been
 * detected.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The crashed subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                         uint16 subsystem);

/**
 * Send CCP PDU subsystem_crashed_ind.
 *
 * DIR FROM_CURATOR
 * Notification from curator to host that a subsystem crash has been
 * detected.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The crashed subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_crashed_ind(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_SUBSYSTEM_CRASHED_IND, arg1, arg2)

/**
 * Analyse received CCP PDU subsystem_crashed_ind.
 *
 * DIR FROM_CURATOR
 * Notification from curator to host that a subsystem crash has been
 * detected.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The crashed subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16(const uint16 *pdu, uint16 pdu_len_words,
                         uint16 *subsystem);

/**
 * Analyse received CCP PDU subsystem_crashed_ind.
 *
 * DIR FROM_CURATOR
 * Notification from curator to host that a subsystem crash has been
 * detected.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The crashed subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_crashed_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU subsystem_recovered_ind.
 *
 * DIR FROM_CURATOR
 * Notification from curator to host that a crashed subsystem
 * has been reset.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The recovered subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_recovered_ind(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_SUBSYSTEM_RECOVERED_IND, arg1, arg2)

/**
 * Analyse received CCP PDU subsystem_recovered_ind.
 *
 * DIR FROM_CURATOR
 * Notification from curator to host that a crashed subsystem
 * has been reset.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The recovered subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_recovered_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU subsystem_version_req.
 *
 * DIR TO_CURATOR
 * Request for version information on a subsystem.  The call is
 * handled within the Curator.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_SYSTEM_SUBSYSTEM_TYPE(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                        SYSTEM_SUBSYSTEM_TYPE subsystem);

/**
 * Send CCP PDU subsystem_version_req.
 *
 * DIR TO_CURATOR
 * Request for version information on a subsystem.  The call is
 * handled within the Curator.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_version_req(arg1, arg2) \
    ccp_send_pdu_SYSTEM_SUBSYSTEM_TYPE(CCP_SIGNAL_ID_SUBSYSTEM_VERSION_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU subsystem_version_req.
 *
 * DIR TO_CURATOR
 * Request for version information on a subsystem.  The call is
 * handled within the Curator.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_SYSTEM_SUBSYSTEM_TYPE(const uint16 *pdu, uint16 pdu_len_words,
                                        SYSTEM_SUBSYSTEM_TYPE *subsystem);

/**
 * Analyse received CCP PDU subsystem_version_req.
 *
 * DIR TO_CURATOR
 * Request for version information on a subsystem.  The call is
 * handled within the Curator.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_version_req(arg1, arg2, arg3) \
    ccp_recv_pdu_SYSTEM_SUBSYSTEM_TYPE(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SUBSYSTEM_VERSION_RSP_ID_STRING \
    CCP_SUBSYSTEM_VERSION_RSP_ID_STRING_WORD_OFFSET

/**
 * Send CCP PDU subsystem_version_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_SUBSYSTEM_VERSION_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c hw_version
 * The subsystem hardware version ID, from SUB_SYS_DEBUGGER_REF_ID.
 * At the time of writing this is the same for all subsystems.
 * For subsystems other than the Curator the address of this
 * register is looked up in the subsystem symbol lookup
 * table.  The value zero here indicates the table could not be
 * accessed; this may be either because no subsystem of the
 * given type was present, or because there was no firmware
 * present in the subsystem.
 *
 *   * \c fw_rom_version
 * The subsystem firmware ROM version.
 * The meaning of this value is specific to the subsystem.
 * A 16-bit version number will be suitably padded.
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 *
 *   * \c patched_fw_version
 * The level to which the firmware being started has been patched.
 * This is the fw version of the subsystem after it has been
 * patched, or 0xFFFFFFFF if the subsystem has not yet been started.
 * This is passed to the Curator in the OPERATIONAL_IND message.
 * If the subsystem has never been started, the value
 * is CCP_PATCH_LEVEL_NOT_YET_PATCHED = 0xFFFFFFFF. This
 * is defined within the firmware as it does not fit within
 * the size of a XAP enumeration.  The value zero indicates the
 * subsystem was started but no patch was available.
 *
 *   * Non-parameter argument \c id_string:
 * An optional identifier string for the firmware.
 * This is an identifier string consisting of octets in
 * the subsystem's usual representation, typically UTF-8 if
 * not US ASCII.  The octets are packed little-endian into
 * 16-bit words.  If the length is an odd number of octets the
 * upper octet of the last word is zero.  If the length is an
 * even number of octets, there is no extra padding (the type
 * "NullTermString" is a barefaced lie; it may be a hack to keep
 * the autogeneration tools happy).
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SUBSYSTEM_VERSION_RSP_ID_STRING_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16__uint32__uint32__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        SYSTEM_SUBSYSTEM_TYPE subsystem,
        uint16 hw_version,
        uint32 fw_rom_version,
        uint32 patched_fw_version);

/**
 * Send CCP PDU subsystem_version_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_SUBSYSTEM_VERSION_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c hw_version
 * The subsystem hardware version ID, from SUB_SYS_DEBUGGER_REF_ID.
 * At the time of writing this is the same for all subsystems.
 * For subsystems other than the Curator the address of this
 * register is looked up in the subsystem symbol lookup
 * table.  The value zero here indicates the table could not be
 * accessed; this may be either because no subsystem of the
 * given type was present, or because there was no firmware
 * present in the subsystem.
 *
 *   * \c fw_rom_version
 * The subsystem firmware ROM version.
 * The meaning of this value is specific to the subsystem.
 * A 16-bit version number will be suitably padded.
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 *
 *   * \c patched_fw_version
 * The level to which the firmware being started has been patched.
 * This is the fw version of the subsystem after it has been
 * patched, or 0xFFFFFFFF if the subsystem has not yet been started.
 * This is passed to the Curator in the OPERATIONAL_IND message.
 * If the subsystem has never been started, the value
 * is CCP_PATCH_LEVEL_NOT_YET_PATCHED = 0xFFFFFFFF. This
 * is defined within the firmware as it does not fit within
 * the size of a XAP enumeration.  The value zero indicates the
 * subsystem was started but no patch was available.
 *
 *   * Non-parameter argument \c id_string:
 * An optional identifier string for the firmware.
 * This is an identifier string consisting of octets in
 * the subsystem's usual representation, typically UTF-8 if
 * not US ASCII.  The octets are packed little-endian into
 * 16-bit words.  If the length is an odd number of octets the
 * upper octet of the last word is zero.  If the length is an
 * even number of octets, there is no extra padding (the type
 * "NullTermString" is a barefaced lie; it may be a hack to keep
 * the autogeneration tools happy).
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SUBSYSTEM_VERSION_RSP_ID_STRING_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_version_rsp(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    ccp_send_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16__uint32__uint32__VARLEN(CCP_SIGNAL_ID_SUBSYSTEM_VERSION_RSP, arg1, arg2, arg3, arg4, arg5, arg6, arg7)

/**
 * Analyse received CCP PDU subsystem_version_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_SUBSYSTEM_VERSION_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c hw_version
 * The subsystem hardware version ID, from SUB_SYS_DEBUGGER_REF_ID.
 * At the time of writing this is the same for all subsystems.
 * For subsystems other than the Curator the address of this
 * register is looked up in the subsystem symbol lookup
 * table.  The value zero here indicates the table could not be
 * accessed; this may be either because no subsystem of the
 * given type was present, or because there was no firmware
 * present in the subsystem.
 *
 *   * \c fw_rom_version
 * The subsystem firmware ROM version.
 * The meaning of this value is specific to the subsystem.
 * A 16-bit version number will be suitably padded.
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 *
 *   * \c patched_fw_version
 * The level to which the firmware being started has been patched.
 * This is the fw version of the subsystem after it has been
 * patched, or 0xFFFFFFFF if the subsystem has not yet been started.
 * This is passed to the Curator in the OPERATIONAL_IND message.
 * If the subsystem has never been started, the value
 * is CCP_PATCH_LEVEL_NOT_YET_PATCHED = 0xFFFFFFFF. This
 * is defined within the firmware as it does not fit within
 * the size of a XAP enumeration.  The value zero indicates the
 * subsystem was started but no patch was available.
 *
 *   Non-parameter argument \c id_string:
 * An optional identifier string for the firmware.
 * This is an identifier string consisting of octets in
 * the subsystem's usual representation, typically UTF-8 if
 * not US ASCII.  The octets are packed little-endian into
 * 16-bit words.  If the length is an odd number of octets the
 * upper octet of the last word is zero.  If the length is an
 * even number of octets, there is no extra padding (the type
 * "NullTermString" is a barefaced lie; it may be a hack to keep
 * the autogeneration tools happy).
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SUBSYSTEM_VERSION_RSP_ID_STRING_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16__uint32__uint32__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        SYSTEM_SUBSYSTEM_TYPE *subsystem,
        uint16 *hw_version,
        uint32 *fw_rom_version,
        uint32 *patched_fw_version);

/**
 * Analyse received CCP PDU subsystem_version_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_SUBSYSTEM_VERSION_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c hw_version
 * The subsystem hardware version ID, from SUB_SYS_DEBUGGER_REF_ID.
 * At the time of writing this is the same for all subsystems.
 * For subsystems other than the Curator the address of this
 * register is looked up in the subsystem symbol lookup
 * table.  The value zero here indicates the table could not be
 * accessed; this may be either because no subsystem of the
 * given type was present, or because there was no firmware
 * present in the subsystem.
 *
 *   * \c fw_rom_version
 * The subsystem firmware ROM version.
 * The meaning of this value is specific to the subsystem.
 * A 16-bit version number will be suitably padded.
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 *
 *   * \c patched_fw_version
 * The level to which the firmware being started has been patched.
 * This is the fw version of the subsystem after it has been
 * patched, or 0xFFFFFFFF if the subsystem has not yet been started.
 * This is passed to the Curator in the OPERATIONAL_IND message.
 * If the subsystem has never been started, the value
 * is CCP_PATCH_LEVEL_NOT_YET_PATCHED = 0xFFFFFFFF. This
 * is defined within the firmware as it does not fit within
 * the size of a XAP enumeration.  The value zero indicates the
 * subsystem was started but no patch was available.
 *
 *   Non-parameter argument \c id_string:
 * An optional identifier string for the firmware.
 * This is an identifier string consisting of octets in
 * the subsystem's usual representation, typically UTF-8 if
 * not US ASCII.  The octets are packed little-endian into
 * 16-bit words.  If the length is an odd number of octets the
 * upper octet of the last word is zero.  If the length is an
 * even number of octets, there is no extra padding (the type
 * "NullTermString" is a barefaced lie; it may be a hack to keep
 * the autogeneration tools happy).
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SUBSYSTEM_VERSION_RSP_ID_STRING_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_version_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_recv_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16__uint32__uint32__VARLEN(arg1, arg2, arg3, arg4, arg5, arg6)


/**
 * Send CCP PDU resource_revoked_cnf.
 *
 * DIR FROM_CURATOR
 * Confirmation following RESOURCE_REVOKED_RSP.
 * The Curator indicates that the revocation has taken effect,
 * including any change of ownership of corresponding hardware,
 * so the recipient is no longer the owner of the resource.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c res_id
 * Identifier for the resource that has been revoked.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_resource_revoked_cnf(arg1, arg2) \
    ccp_send_pdu_CCP_RESOURCE_ID(CCP_SIGNAL_ID_RESOURCE_REVOKED_CNF, arg1, arg2)

/**
 * Analyse received CCP PDU resource_revoked_cnf.
 *
 * DIR FROM_CURATOR
 * Confirmation following RESOURCE_REVOKED_RSP.
 * The Curator indicates that the revocation has taken effect,
 * including any change of ownership of corresponding hardware,
 * so the recipient is no longer the owner of the resource.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c res_id
 * Identifier for the resource that has been revoked.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_resource_revoked_cnf(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_RESOURCE_ID(arg1, arg2, arg3)


/**
 * Send CCP PDU command_failure_ind.
 *
 * DIR FROM_CURATOR
 * Indicates that a ccp command could not be processed.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c ccp_cmd
 * ID of the CCP Command that failed
 *
 *   * \c reason
 * Why the command failed
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_SIGNAL_ID__CCP_COMMAND_FAILURE_REASON(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_SIGNAL_ID ccp_cmd,
        CCP_COMMAND_FAILURE_REASON reason);

/**
 * Send CCP PDU command_failure_ind.
 *
 * DIR FROM_CURATOR
 * Indicates that a ccp command could not be processed.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c ccp_cmd
 * ID of the CCP Command that failed
 *
 *   * \c reason
 * Why the command failed
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_command_failure_ind(arg1, arg2, arg3) \
    ccp_send_pdu_CCP_SIGNAL_ID__CCP_COMMAND_FAILURE_REASON(CCP_SIGNAL_ID_COMMAND_FAILURE_IND, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU command_failure_ind.
 *
 * DIR FROM_CURATOR
 * Indicates that a ccp command could not be processed.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c ccp_cmd
 * ID of the CCP Command that failed
 *
 *   * \c reason
 * Why the command failed
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_SIGNAL_ID__CCP_COMMAND_FAILURE_REASON(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_SIGNAL_ID *ccp_cmd,
        CCP_COMMAND_FAILURE_REASON *reason);

/**
 * Analyse received CCP PDU command_failure_ind.
 *
 * DIR FROM_CURATOR
 * Indicates that a ccp command could not be processed.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c ccp_cmd
 * ID of the CCP Command that failed
 *
 *   * \c reason
 * Why the command failed
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_command_failure_ind(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_SIGNAL_ID__CCP_COMMAND_FAILURE_REASON(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU symbol_lookup_req.
 *
 * DIR TO_CURATOR
 * Request a value from a subsystem's symbol lookup table.
 * For security reasons this command is not available in production
 * builds.
 * 
 * DEPRECATED:
 * See CUR-5255: Remove CCP SYMBOL_LOOKUP_REQ from code,
 * not in our ROMs anyway.
 * Removed since production builds are preferred over
 * development builds.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address).  It may be the Curator or
 * any other subsystem other than the host interface subsystem.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry whose value
 * is to be retrieved.  Identifiers are defined separately for
 * each subsystem.
 *
 *   * \c access_mode
 * The manner in which the value is to be retrieved.  The
 * issuer of the command is required to know how the value
 * behaves.
 *
 *   * \c length
 * The length of the data to be retrieved.  Except in the
 * case of CCP_SYMBOL_LOOKUP_ACCESS_POINTER_NULL, the issuer of
 * the command is required to know the length of data to
 * retrieve; this may come from a previous symbol lookup.
 * In the case of CCP_SYMBOL_LOOKUP_ACCESS_POINTER_NULL the length
 * is determined automatically and this value is ignored.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16__CCP_SYMBOL_LOOKUP_ACCESS_MODE__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 subsystem,
        uint16 identifier,
        CCP_SYMBOL_LOOKUP_ACCESS_MODE access_mode,
        uint16 length);

/**
 * Send CCP PDU symbol_lookup_req.
 *
 * DIR TO_CURATOR
 * Request a value from a subsystem's symbol lookup table.
 * For security reasons this command is not available in production
 * builds.
 * 
 * DEPRECATED:
 * See CUR-5255: Remove CCP SYMBOL_LOOKUP_REQ from code,
 * not in our ROMs anyway.
 * Removed since production builds are preferred over
 * development builds.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address).  It may be the Curator or
 * any other subsystem other than the host interface subsystem.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry whose value
 * is to be retrieved.  Identifiers are defined separately for
 * each subsystem.
 *
 *   * \c access_mode
 * The manner in which the value is to be retrieved.  The
 * issuer of the command is required to know how the value
 * behaves.
 *
 *   * \c length
 * The length of the data to be retrieved.  Except in the
 * case of CCP_SYMBOL_LOOKUP_ACCESS_POINTER_NULL, the issuer of
 * the command is required to know the length of data to
 * retrieve; this may come from a previous symbol lookup.
 * In the case of CCP_SYMBOL_LOOKUP_ACCESS_POINTER_NULL the length
 * is determined automatically and this value is ignored.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_symbol_lookup_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_uint16__uint16__CCP_SYMBOL_LOOKUP_ACCESS_MODE__uint16(CCP_SIGNAL_ID_SYMBOL_LOOKUP_REQ, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU symbol_lookup_req.
 *
 * DIR TO_CURATOR
 * Request a value from a subsystem's symbol lookup table.
 * For security reasons this command is not available in production
 * builds.
 * 
 * DEPRECATED:
 * See CUR-5255: Remove CCP SYMBOL_LOOKUP_REQ from code,
 * not in our ROMs anyway.
 * Removed since production builds are preferred over
 * development builds.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address).  It may be the Curator or
 * any other subsystem other than the host interface subsystem.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry whose value
 * is to be retrieved.  Identifiers are defined separately for
 * each subsystem.
 *
 *   * \c access_mode
 * The manner in which the value is to be retrieved.  The
 * issuer of the command is required to know how the value
 * behaves.
 *
 *   * \c length
 * The length of the data to be retrieved.  Except in the
 * case of CCP_SYMBOL_LOOKUP_ACCESS_POINTER_NULL, the issuer of
 * the command is required to know the length of data to
 * retrieve; this may come from a previous symbol lookup.
 * In the case of CCP_SYMBOL_LOOKUP_ACCESS_POINTER_NULL the length
 * is determined automatically and this value is ignored.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16__CCP_SYMBOL_LOOKUP_ACCESS_MODE__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *subsystem,
        uint16 *identifier,
        CCP_SYMBOL_LOOKUP_ACCESS_MODE *access_mode,
        uint16 *length);

/**
 * Analyse received CCP PDU symbol_lookup_req.
 *
 * DIR TO_CURATOR
 * Request a value from a subsystem's symbol lookup table.
 * For security reasons this command is not available in production
 * builds.
 * 
 * DEPRECATED:
 * See CUR-5255: Remove CCP SYMBOL_LOOKUP_REQ from code,
 * not in our ROMs anyway.
 * Removed since production builds are preferred over
 * development builds.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address).  It may be the Curator or
 * any other subsystem other than the host interface subsystem.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry whose value
 * is to be retrieved.  Identifiers are defined separately for
 * each subsystem.
 *
 *   * \c access_mode
 * The manner in which the value is to be retrieved.  The
 * issuer of the command is required to know how the value
 * behaves.
 *
 *   * \c length
 * The length of the data to be retrieved.  Except in the
 * case of CCP_SYMBOL_LOOKUP_ACCESS_POINTER_NULL, the issuer of
 * the command is required to know the length of data to
 * retrieve; this may come from a previous symbol lookup.
 * In the case of CCP_SYMBOL_LOOKUP_ACCESS_POINTER_NULL the length
 * is determined automatically and this value is ignored.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_symbol_lookup_req(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_recv_pdu_uint16__uint16__CCP_SYMBOL_LOOKUP_ACCESS_MODE__uint16(arg1, arg2, arg3, arg4, arg5, arg6)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SYMBOL_LOOKUP_RSP_VALUE \
    CCP_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET

/**
 * Send CCP PDU symbol_lookup_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_SYMBOL_LOOKUP_REQ.
 * 
 * DEPRECATED:
 * See CUR-5255: Remove CCP SYMBOL_LOOKUP_REQ from code,
 * not in our ROMs anyway.
 * Removed since production builds are preferred over
 * development builds.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address), as given in the
 * request.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry, as in the
 * request.
 *
 *   * \c status
 * The status of the lookup.  If this is not
 * CCP_SYMBOL_LOOKUP_STATUS_SUCCESS no value is returned.
 *
 *   * Non-parameter argument \c value:
 * The value returned, its length as determined by the request.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16__CCP_SYMBOL_LOOKUP_STATUS__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        uint16 subsystem,
        uint16 identifier,
        CCP_SYMBOL_LOOKUP_STATUS status);

/**
 * Send CCP PDU symbol_lookup_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_SYMBOL_LOOKUP_REQ.
 * 
 * DEPRECATED:
 * See CUR-5255: Remove CCP SYMBOL_LOOKUP_REQ from code,
 * not in our ROMs anyway.
 * Removed since production builds are preferred over
 * development builds.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address), as given in the
 * request.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry, as in the
 * request.
 *
 *   * \c status
 * The status of the lookup.  If this is not
 * CCP_SYMBOL_LOOKUP_STATUS_SUCCESS no value is returned.
 *
 *   * Non-parameter argument \c value:
 * The value returned, its length as determined by the request.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_symbol_lookup_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_send_pdu_uint16__uint16__CCP_SYMBOL_LOOKUP_STATUS__VARLEN(CCP_SIGNAL_ID_SYMBOL_LOOKUP_RSP, arg1, arg2, arg3, arg4, arg5, arg6)

/**
 * Analyse received CCP PDU symbol_lookup_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_SYMBOL_LOOKUP_REQ.
 * 
 * DEPRECATED:
 * See CUR-5255: Remove CCP SYMBOL_LOOKUP_REQ from code,
 * not in our ROMs anyway.
 * Removed since production builds are preferred over
 * development builds.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address), as given in the
 * request.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry, as in the
 * request.
 *
 *   * \c status
 * The status of the lookup.  If this is not
 * CCP_SYMBOL_LOOKUP_STATUS_SUCCESS no value is returned.
 *
 *   Non-parameter argument \c value:
 * The value returned, its length as determined by the request.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16__CCP_SYMBOL_LOOKUP_STATUS__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *subsystem,
        uint16 *identifier,
        CCP_SYMBOL_LOOKUP_STATUS *status);

/**
 * Analyse received CCP PDU symbol_lookup_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_SYMBOL_LOOKUP_REQ.
 * 
 * DEPRECATED:
 * See CUR-5255: Remove CCP SYMBOL_LOOKUP_REQ from code,
 * not in our ROMs anyway.
 * Removed since production builds are preferred over
 * development builds.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address), as given in the
 * request.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry, as in the
 * request.
 *
 *   * \c status
 * The status of the lookup.  If this is not
 * CCP_SYMBOL_LOOKUP_STATUS_SUCCESS no value is returned.
 *
 *   Non-parameter argument \c value:
 * The value returned, its length as determined by the request.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_symbol_lookup_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_uint16__uint16__CCP_SYMBOL_LOOKUP_STATUS__VARLEN(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU meta_symbol_lookup_req.
 *
 * DIR TO_CURATOR
 * Request a value from a subsystem's symbol lookup table
 * using a global ID known to the Curator.  This is more
 * convenient where available as the Curator is able to work
 * out the size of the information to return by itself.
 * 
 * This is a low-level interface to the information in question.
 * In most cases it will be more convenient to retrieve the
 * information either via CCP_SIGNAL_ID_SUBSYSTEM_VERSION_REQ
 * or via CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_QUERY_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address).  It may be the Curator or
 * any other subsystem other than the host interface subsystem.
 * Note that not all identifiers may be available on the Curator as
 * it typically retrieves information another way.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry whose value
 * is to be retrieved.  Identifiers are defined separately for
 * each subsystem.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__CCP_META_SYMBOL_ID(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 subsystem,
        CCP_META_SYMBOL_ID identifier);

/**
 * Send CCP PDU meta_symbol_lookup_req.
 *
 * DIR TO_CURATOR
 * Request a value from a subsystem's symbol lookup table
 * using a global ID known to the Curator.  This is more
 * convenient where available as the Curator is able to work
 * out the size of the information to return by itself.
 * 
 * This is a low-level interface to the information in question.
 * In most cases it will be more convenient to retrieve the
 * information either via CCP_SIGNAL_ID_SUBSYSTEM_VERSION_REQ
 * or via CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_QUERY_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address).  It may be the Curator or
 * any other subsystem other than the host interface subsystem.
 * Note that not all identifiers may be available on the Curator as
 * it typically retrieves information another way.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry whose value
 * is to be retrieved.  Identifiers are defined separately for
 * each subsystem.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_meta_symbol_lookup_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__CCP_META_SYMBOL_ID(CCP_SIGNAL_ID_META_SYMBOL_LOOKUP_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU meta_symbol_lookup_req.
 *
 * DIR TO_CURATOR
 * Request a value from a subsystem's symbol lookup table
 * using a global ID known to the Curator.  This is more
 * convenient where available as the Curator is able to work
 * out the size of the information to return by itself.
 * 
 * This is a low-level interface to the information in question.
 * In most cases it will be more convenient to retrieve the
 * information either via CCP_SIGNAL_ID_SUBSYSTEM_VERSION_REQ
 * or via CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_QUERY_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address).  It may be the Curator or
 * any other subsystem other than the host interface subsystem.
 * Note that not all identifiers may be available on the Curator as
 * it typically retrieves information another way.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry whose value
 * is to be retrieved.  Identifiers are defined separately for
 * each subsystem.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__CCP_META_SYMBOL_ID(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *subsystem,
        CCP_META_SYMBOL_ID *identifier);

/**
 * Analyse received CCP PDU meta_symbol_lookup_req.
 *
 * DIR TO_CURATOR
 * Request a value from a subsystem's symbol lookup table
 * using a global ID known to the Curator.  This is more
 * convenient where available as the Curator is able to work
 * out the size of the information to return by itself.
 * 
 * This is a low-level interface to the information in question.
 * In most cases it will be more convenient to retrieve the
 * information either via CCP_SIGNAL_ID_SUBSYSTEM_VERSION_REQ
 * or via CCP_SIGNAL_ID_SERVICE_ADVERTISEMENT_QUERY_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address).  It may be the Curator or
 * any other subsystem other than the host interface subsystem.
 * Note that not all identifiers may be available on the Curator as
 * it typically retrieves information another way.
 *
 *   * \c identifier
 * The identifier of the symbol lookup table entry whose value
 * is to be retrieved.  Identifiers are defined separately for
 * each subsystem.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_meta_symbol_lookup_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__CCP_META_SYMBOL_ID(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_META_SYMBOL_LOOKUP_RSP_VALUE \
    CCP_META_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET

/**
 * Send CCP PDU meta_symbol_lookup_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_META_SYMBOL_LOOKUP_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address), as given in the request.
 *
 *   * \c identifier
 * The identifier of the meta symbol lookup table entry, as in the
 * request.
 *
 *   * \c status
 * The status of the lookup.  If this is not
 * CCP_SYMBOL_LOOKUP_STATUS_SUCCESS no value is returned.
 *
 *   * Non-parameter argument \c value:
 * The value returned.  The length and formatting is as
 * defined for the identifer in question.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_META_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__CCP_META_SYMBOL_ID__CCP_SYMBOL_LOOKUP_STATUS__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        uint16 subsystem,
        CCP_META_SYMBOL_ID identifier,
        CCP_SYMBOL_LOOKUP_STATUS status);

/**
 * Send CCP PDU meta_symbol_lookup_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_META_SYMBOL_LOOKUP_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address), as given in the request.
 *
 *   * \c identifier
 * The identifier of the meta symbol lookup table entry, as in the
 * request.
 *
 *   * \c status
 * The status of the lookup.  If this is not
 * CCP_SYMBOL_LOOKUP_STATUS_SUCCESS no value is returned.
 *
 *   * Non-parameter argument \c value:
 * The value returned.  The length and formatting is as
 * defined for the identifer in question.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_META_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_meta_symbol_lookup_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_send_pdu_uint16__CCP_META_SYMBOL_ID__CCP_SYMBOL_LOOKUP_STATUS__VARLEN(CCP_SIGNAL_ID_META_SYMBOL_LOOKUP_RSP, arg1, arg2, arg3, arg4, arg5, arg6)

/**
 * Analyse received CCP PDU meta_symbol_lookup_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_META_SYMBOL_LOOKUP_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address), as given in the request.
 *
 *   * \c identifier
 * The identifier of the meta symbol lookup table entry, as in the
 * request.
 *
 *   * \c status
 * The status of the lookup.  If this is not
 * CCP_SYMBOL_LOOKUP_STATUS_SUCCESS no value is returned.
 *
 *   Non-parameter argument \c value:
 * The value returned.  The length and formatting is as
 * defined for the identifer in question.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_META_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__CCP_META_SYMBOL_ID__CCP_SYMBOL_LOOKUP_STATUS__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *subsystem,
        CCP_META_SYMBOL_ID *identifier,
        CCP_SYMBOL_LOOKUP_STATUS *status);

/**
 * Analyse received CCP PDU meta_symbol_lookup_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_META_SYMBOL_LOOKUP_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem bus ID (ISP address), as given in the request.
 *
 *   * \c identifier
 * The identifier of the meta symbol lookup table entry, as in the
 * request.
 *
 *   * \c status
 * The status of the lookup.  If this is not
 * CCP_SYMBOL_LOOKUP_STATUS_SUCCESS no value is returned.
 *
 *   Non-parameter argument \c value:
 * The value returned.  The length and formatting is as
 * defined for the identifer in question.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_META_SYMBOL_LOOKUP_RSP_VALUE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_meta_symbol_lookup_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_uint16__CCP_META_SYMBOL_ID__CCP_SYMBOL_LOOKUP_STATUS__VARLEN(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU subsystem_crashed_waiting_ind.
 *
 * DIR FROM_CURATOR
 * This indication is sent from the curator to the host following a
 * CCP_SUBSYSTEM_CRASHED_IND once the subsystem resources and services
 * have been torn-down. It indicates that the subsystem is now held in
 * a "crashed-waiting" state.
 * 
 * The subsystem will remain in the crashed-waiting state until the host
 * sends a CCP_SUBSYSTEM_CRASH_CLEAR_REQ. The subsystem will then
 * be restored to the idle state.
 * 
 * The CCP_SUBSYSTEM_CRASHED_WAITING_IND & CCP_SUBSYSTEM_CRASH_CLEAR_REQ
 * handshake allows the host to scrape a core-dump or debug a
 * crashed subsystem before it is reset.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The crashed subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_crashed_waiting_ind(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_SUBSYSTEM_CRASHED_WAITING_IND, arg1, arg2)

/**
 * Analyse received CCP PDU subsystem_crashed_waiting_ind.
 *
 * DIR FROM_CURATOR
 * This indication is sent from the curator to the host following a
 * CCP_SUBSYSTEM_CRASHED_IND once the subsystem resources and services
 * have been torn-down. It indicates that the subsystem is now held in
 * a "crashed-waiting" state.
 * 
 * The subsystem will remain in the crashed-waiting state until the host
 * sends a CCP_SUBSYSTEM_CRASH_CLEAR_REQ. The subsystem will then
 * be restored to the idle state.
 * 
 * The CCP_SUBSYSTEM_CRASHED_WAITING_IND & CCP_SUBSYSTEM_CRASH_CLEAR_REQ
 * handshake allows the host to scrape a core-dump or debug a
 * crashed subsystem before it is reset.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The crashed subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_crashed_waiting_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU subsystem_crash_clear_req.
 *
 * DIR TO_CURATOR
 * This request must be sent by the host to the curator to release a
 * subsystem from a "crashed-waiting" state after receiving a
 * CCP_SUBSYSTEM_CRASHED_WAITING_IND from the curator.
 * 
 * The subsystem will then be restored to the idle state and a
 * CCP_SUBSYSTEM_RECOVERED_IND sent to the host once ready for
 * re-activation.
 * 
 * The CCP_SUBSYSTEM_CRASHED_WAITING_IND & CCP_SUBSYSTEM_CRASH_CLEAR_REQ
 * handshake allows the host to scrape a core-dump or debug a
 * crashed subsystem before it is reset.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The crashed subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_crash_clear_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_SUBSYSTEM_CRASH_CLEAR_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU subsystem_crash_clear_req.
 *
 * DIR TO_CURATOR
 * This request must be sent by the host to the curator to release a
 * subsystem from a "crashed-waiting" state after receiving a
 * CCP_SUBSYSTEM_CRASHED_WAITING_IND from the curator.
 * 
 * The subsystem will then be restored to the idle state and a
 * CCP_SUBSYSTEM_RECOVERED_IND sent to the host once ready for
 * re-activation.
 * 
 * The CCP_SUBSYSTEM_CRASHED_WAITING_IND & CCP_SUBSYSTEM_CRASH_CLEAR_REQ
 * handshake allows the host to scrape a core-dump or debug a
 * crashed subsystem before it is reset.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The crashed subsystem bus id (ISP address)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_crash_clear_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU bit_config_req.
 *
 * DIR TO_CURATOR
 * Request for bit configuration data stored in non-volatile memory.
 * This may be eFuse or some other mechanism.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c position
 * Bit position of the first bit requested.  The
 * CCP_BIT_CONFIG_POS_* values from CCP_BIT_CONFIG give the
 * location of useful values, however any position within
 * the range may be specified.
 *
 *   * \c length
 * Number of bits required.  The CCP_BIG_CONFIG_LEN_* values
 * from CCP_BIT_CONFIG give the lengths of useful values,
 * however the length is only constrained by the range of
 * available bits and the size of the message.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_bit_config_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID_BIT_CONFIG_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU bit_config_req.
 *
 * DIR TO_CURATOR
 * Request for bit configuration data stored in non-volatile memory.
 * This may be eFuse or some other mechanism.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c position
 * Bit position of the first bit requested.  The
 * CCP_BIT_CONFIG_POS_* values from CCP_BIT_CONFIG give the
 * location of useful values, however any position within
 * the range may be specified.
 *
 *   * \c length
 * Number of bits required.  The CCP_BIG_CONFIG_LEN_* values
 * from CCP_BIT_CONFIG give the lengths of useful values,
 * however the length is only constrained by the range of
 * available bits and the size of the message.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_bit_config_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_BIT_CONFIG_RSP_VALUE \
    CCP_BIT_CONFIG_RSP_VALUE_WORD_OFFSET

/**
 * Send CCP PDU bit_config_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_BIT_CONFIG_REQ containing the requested
 * information.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c position
 * Bit position of the first bit requested, copied from the
 * request.
 *
 *   * \c length
 * Number of bits required, copied from the request.
 *
 *   * \c status
 * Status of request.  If this is not CCP_BIT_CONFIG_STATUS
 * success no data is supplied.
 *
 *   * Non-parameter argument \c value:
 * The data requested.  Bit zero of the first 16-bit word of the
 * value corresponds to the bit at "position", and so on.
 * The last word is zero-filled if the "length" is not a
 * multiple of 16.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_BIT_CONFIG_RSP_VALUE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16__CCP_BIT_CONFIG_STATUS__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        uint16 position,
        uint16 length,
        CCP_BIT_CONFIG_STATUS status);

/**
 * Send CCP PDU bit_config_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_BIT_CONFIG_REQ containing the requested
 * information.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c position
 * Bit position of the first bit requested, copied from the
 * request.
 *
 *   * \c length
 * Number of bits required, copied from the request.
 *
 *   * \c status
 * Status of request.  If this is not CCP_BIT_CONFIG_STATUS
 * success no data is supplied.
 *
 *   * Non-parameter argument \c value:
 * The data requested.  Bit zero of the first 16-bit word of the
 * value corresponds to the bit at "position", and so on.
 * The last word is zero-filled if the "length" is not a
 * multiple of 16.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_BIT_CONFIG_RSP_VALUE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_bit_config_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_send_pdu_uint16__uint16__CCP_BIT_CONFIG_STATUS__VARLEN(CCP_SIGNAL_ID_BIT_CONFIG_RSP, arg1, arg2, arg3, arg4, arg5, arg6)

/**
 * Analyse received CCP PDU bit_config_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_BIT_CONFIG_REQ containing the requested
 * information.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c position
 * Bit position of the first bit requested, copied from the
 * request.
 *
 *   * \c length
 * Number of bits required, copied from the request.
 *
 *   * \c status
 * Status of request.  If this is not CCP_BIT_CONFIG_STATUS
 * success no data is supplied.
 *
 *   Non-parameter argument \c value:
 * The data requested.  Bit zero of the first 16-bit word of the
 * value corresponds to the bit at "position", and so on.
 * The last word is zero-filled if the "length" is not a
 * multiple of 16.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_BIT_CONFIG_RSP_VALUE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16__CCP_BIT_CONFIG_STATUS__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *position,
        uint16 *length,
        CCP_BIT_CONFIG_STATUS *status);

/**
 * Analyse received CCP PDU bit_config_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_BIT_CONFIG_REQ containing the requested
 * information.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c position
 * Bit position of the first bit requested, copied from the
 * request.
 *
 *   * \c length
 * Number of bits required, copied from the request.
 *
 *   * \c status
 * Status of request.  If this is not CCP_BIT_CONFIG_STATUS
 * success no data is supplied.
 *
 *   Non-parameter argument \c value:
 * The data requested.  Bit zero of the first 16-bit word of the
 * value corresponds to the bit at "position", and so on.
 * The last word is zero-filled if the "length" is not a
 * multiple of 16.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_BIT_CONFIG_RSP_VALUE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_bit_config_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_uint16__uint16__CCP_BIT_CONFIG_STATUS__VARLEN(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU smps_8mhz_clock_config.
 *
 * DIR TO_CURATOR
 * Request to set the configuration for the ~8 MHz clock used
 * to drive the switched mode power supply in certain modes,
 * and also the PIO charge pump, on chips with the Amber PMU
 * and PIO arrangement.  There is no response to this signal,
 * although if the feature is unimplemented the chip will generate
 * a fault.  A CCP RESOURCE_QUERY_REQ for the resource smps_8mhz_clock
 * can be used to disover if the feature is present.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c divisor_80mhz
 * The divisor to use for generating the ~8 Mhz clock when the
 * input clock is 80 MHz.  The values 9, 10, 11 are valid.
 *
 *   * \c divisor_240mhz
 * The divisor to use for generating the ~8 Mhz clock when the
 * input clock is 240 MHz.  Values from 27 to 34 inclusive are
 * valid.
 *
 *   * \c phase_clock_1
 * 4-bit phase selection for clock 1, the core SMPS clock.
 *
 *   * \c phase_clock_2
 * 4-bit phase selection for clock 2, the PA SMPS clock.
 *
 *   * \c phase_clock_3
 * 4-bit phase selection for clock 3, the charge pump clock.
 *
 *   * \c invert_clock_1
 * Whether to invert clock 1, the core SMPS clock.
 *
 *   * \c invert_clock_2
 * Whether to invert clock 2, the PA SMPS clock.
 *
 *   * \c invert_clock_3
 * Whether to invert clock 3, the charge pump clock.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint8__uint8__uint4__uint4__uint4__Bit__Bit__Bit(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint8 divisor_80mhz,
        uint8 divisor_240mhz,
        uint4 phase_clock_1,
        uint4 phase_clock_2,
        uint4 phase_clock_3,
        Bit invert_clock_1,
        Bit invert_clock_2,
        Bit invert_clock_3);

/**
 * Send CCP PDU smps_8mhz_clock_config.
 *
 * DIR TO_CURATOR
 * Request to set the configuration for the ~8 MHz clock used
 * to drive the switched mode power supply in certain modes,
 * and also the PIO charge pump, on chips with the Amber PMU
 * and PIO arrangement.  There is no response to this signal,
 * although if the feature is unimplemented the chip will generate
 * a fault.  A CCP RESOURCE_QUERY_REQ for the resource smps_8mhz_clock
 * can be used to disover if the feature is present.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c divisor_80mhz
 * The divisor to use for generating the ~8 Mhz clock when the
 * input clock is 80 MHz.  The values 9, 10, 11 are valid.
 *
 *   * \c divisor_240mhz
 * The divisor to use for generating the ~8 Mhz clock when the
 * input clock is 240 MHz.  Values from 27 to 34 inclusive are
 * valid.
 *
 *   * \c phase_clock_1
 * 4-bit phase selection for clock 1, the core SMPS clock.
 *
 *   * \c phase_clock_2
 * 4-bit phase selection for clock 2, the PA SMPS clock.
 *
 *   * \c phase_clock_3
 * 4-bit phase selection for clock 3, the charge pump clock.
 *
 *   * \c invert_clock_1
 * Whether to invert clock 1, the core SMPS clock.
 *
 *   * \c invert_clock_2
 * Whether to invert clock 2, the PA SMPS clock.
 *
 *   * \c invert_clock_3
 * Whether to invert clock 3, the charge pump clock.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_smps_8mhz_clock_config(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
    ccp_send_pdu_uint8__uint8__uint4__uint4__uint4__Bit__Bit__Bit(CCP_SIGNAL_ID_SMPS_8MHZ_CLOCK_CONFIG, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

/**
 * Analyse received CCP PDU smps_8mhz_clock_config.
 *
 * DIR TO_CURATOR
 * Request to set the configuration for the ~8 MHz clock used
 * to drive the switched mode power supply in certain modes,
 * and also the PIO charge pump, on chips with the Amber PMU
 * and PIO arrangement.  There is no response to this signal,
 * although if the feature is unimplemented the chip will generate
 * a fault.  A CCP RESOURCE_QUERY_REQ for the resource smps_8mhz_clock
 * can be used to disover if the feature is present.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c divisor_80mhz
 * The divisor to use for generating the ~8 Mhz clock when the
 * input clock is 80 MHz.  The values 9, 10, 11 are valid.
 *
 *   * \c divisor_240mhz
 * The divisor to use for generating the ~8 Mhz clock when the
 * input clock is 240 MHz.  Values from 27 to 34 inclusive are
 * valid.
 *
 *   * \c phase_clock_1
 * 4-bit phase selection for clock 1, the core SMPS clock.
 *
 *   * \c phase_clock_2
 * 4-bit phase selection for clock 2, the PA SMPS clock.
 *
 *   * \c phase_clock_3
 * 4-bit phase selection for clock 3, the charge pump clock.
 *
 *   * \c invert_clock_1
 * Whether to invert clock 1, the core SMPS clock.
 *
 *   * \c invert_clock_2
 * Whether to invert clock 2, the PA SMPS clock.
 *
 *   * \c invert_clock_3
 * Whether to invert clock 3, the charge pump clock.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint8__uint8__uint4__uint4__uint4__Bit__Bit__Bit(
        const uint16 *pdu, uint16 pdu_len_words,
        uint8 *divisor_80mhz,
        uint8 *divisor_240mhz,
        uint4 *phase_clock_1,
        uint4 *phase_clock_2,
        uint4 *phase_clock_3,
        Bit *invert_clock_1,
        Bit *invert_clock_2,
        Bit *invert_clock_3);

/**
 * Analyse received CCP PDU smps_8mhz_clock_config.
 *
 * DIR TO_CURATOR
 * Request to set the configuration for the ~8 MHz clock used
 * to drive the switched mode power supply in certain modes,
 * and also the PIO charge pump, on chips with the Amber PMU
 * and PIO arrangement.  There is no response to this signal,
 * although if the feature is unimplemented the chip will generate
 * a fault.  A CCP RESOURCE_QUERY_REQ for the resource smps_8mhz_clock
 * can be used to disover if the feature is present.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c divisor_80mhz
 * The divisor to use for generating the ~8 Mhz clock when the
 * input clock is 80 MHz.  The values 9, 10, 11 are valid.
 *
 *   * \c divisor_240mhz
 * The divisor to use for generating the ~8 Mhz clock when the
 * input clock is 240 MHz.  Values from 27 to 34 inclusive are
 * valid.
 *
 *   * \c phase_clock_1
 * 4-bit phase selection for clock 1, the core SMPS clock.
 *
 *   * \c phase_clock_2
 * 4-bit phase selection for clock 2, the PA SMPS clock.
 *
 *   * \c phase_clock_3
 * 4-bit phase selection for clock 3, the charge pump clock.
 *
 *   * \c invert_clock_1
 * Whether to invert clock 1, the core SMPS clock.
 *
 *   * \c invert_clock_2
 * Whether to invert clock 2, the PA SMPS clock.
 *
 *   * \c invert_clock_3
 * Whether to invert clock 3, the charge pump clock.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_smps_8mhz_clock_config(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
    ccp_recv_pdu_uint8__uint8__uint4__uint4__uint4__Bit__Bit__Bit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)


/**
 * Send CCP PDU deep_sleep_wake_pio_req.
 *
 * DIR FROM_CURATOR
 * This signal is an extended form of CCP_DEEP_SLEEP_WAKE_REQ.
 * It includes the same "source" field as CCP_DEEP_SLEEP_WAKE_REQ,
 * but also indicates a mask of PIOs (0 to 31) that have caused
 * the chip to wake from deep sleep.  The signal is only sent on
 * chips that have hardware to detect PIO events separately in
 * deep sleep.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 * Identical to corresponding field in CCP_DEEP_SLEEP_WAKE_REQ.
 *
 *   * \c pio_mask
 * Bit mask of PIOs that caused the chip to wake from deep sleep.
 * As the signal is only sent when there is PIO information to
 * send, this field should not normally be zero.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint32__uint32(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                 uint32 source,
                                 uint32 pio_mask);

/**
 * Send CCP PDU deep_sleep_wake_pio_req.
 *
 * DIR FROM_CURATOR
 * This signal is an extended form of CCP_DEEP_SLEEP_WAKE_REQ.
 * It includes the same "source" field as CCP_DEEP_SLEEP_WAKE_REQ,
 * but also indicates a mask of PIOs (0 to 31) that have caused
 * the chip to wake from deep sleep.  The signal is only sent on
 * chips that have hardware to detect PIO events separately in
 * deep sleep.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 * Identical to corresponding field in CCP_DEEP_SLEEP_WAKE_REQ.
 *
 *   * \c pio_mask
 * Bit mask of PIOs that caused the chip to wake from deep sleep.
 * As the signal is only sent when there is PIO information to
 * send, this field should not normally be zero.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_wake_pio_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint32__uint32(CCP_SIGNAL_ID_DEEP_SLEEP_WAKE_PIO_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU deep_sleep_wake_pio_req.
 *
 * DIR FROM_CURATOR
 * This signal is an extended form of CCP_DEEP_SLEEP_WAKE_REQ.
 * It includes the same "source" field as CCP_DEEP_SLEEP_WAKE_REQ,
 * but also indicates a mask of PIOs (0 to 31) that have caused
 * the chip to wake from deep sleep.  The signal is only sent on
 * chips that have hardware to detect PIO events separately in
 * deep sleep.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 * Identical to corresponding field in CCP_DEEP_SLEEP_WAKE_REQ.
 *
 *   * \c pio_mask
 * Bit mask of PIOs that caused the chip to wake from deep sleep.
 * As the signal is only sent when there is PIO information to
 * send, this field should not normally be zero.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint32__uint32(const uint16 *pdu, uint16 pdu_len_words,
                                 uint32 *source,
                                 uint32 *pio_mask);

/**
 * Analyse received CCP PDU deep_sleep_wake_pio_req.
 *
 * DIR FROM_CURATOR
 * This signal is an extended form of CCP_DEEP_SLEEP_WAKE_REQ.
 * It includes the same "source" field as CCP_DEEP_SLEEP_WAKE_REQ,
 * but also indicates a mask of PIOs (0 to 31) that have caused
 * the chip to wake from deep sleep.  The signal is only sent on
 * chips that have hardware to detect PIO events separately in
 * deep sleep.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 * Identical to corresponding field in CCP_DEEP_SLEEP_WAKE_REQ.
 *
 *   * \c pio_mask
 * Bit mask of PIOs that caused the chip to wake from deep sleep.
 * As the signal is only sent when there is PIO information to
 * send, this field should not normally be zero.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_wake_pio_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint32__uint32(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU measurement_req.
 *
 * DIR BOTH
 * Request for measurement from host to Curator or from
 * Curator to subsystem
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c type
 * Indication of what measurement has been requested
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_MEASURABLE_QUANTITY(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_MEASURABLE_QUANTITY type);

/**
 * Send CCP PDU measurement_req.
 *
 * DIR BOTH
 * Request for measurement from host to Curator or from
 * Curator to subsystem
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c type
 * Indication of what measurement has been requested
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_measurement_req(arg1, arg2) \
    ccp_send_pdu_CCP_MEASURABLE_QUANTITY(CCP_SIGNAL_ID_MEASUREMENT_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU measurement_req.
 *
 * DIR BOTH
 * Request for measurement from host to Curator or from
 * Curator to subsystem
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c type
 * Indication of what measurement has been requested
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_MEASURABLE_QUANTITY(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_MEASURABLE_QUANTITY *type);

/**
 * Analyse received CCP PDU measurement_req.
 *
 * DIR BOTH
 * Request for measurement from host to Curator or from
 * Curator to subsystem
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c type
 * Indication of what measurement has been requested
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_measurement_req(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_MEASURABLE_QUANTITY(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_MEASUREMENT_RSP_VALUE \
    CCP_MEASUREMENT_RSP_VALUE_WORD_OFFSET

/**
 * Send CCP PDU measurement_rsp.
 *
 * DIR BOTH
 * Measured value passed up to host by Curator or to
 * Curator by subsystem
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c type
 * Indication of what quantity is being supplied
 *
 *   * Non-parameter argument \c value:
 * List of values supplied by subsystem.  When sending
 * from multiple subsystems to the host, this will contain
 * a concatenation of all the subsystem lists in subsystem
 * order.  At this level it is assumed that the sender and
 * receiver will know and agree on the number and
 * meaning of the entries in this array because this is a
 * relatively rarely-changing property of the hardware
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_MEASUREMENT_RSP_VALUE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_MEASURABLE_QUANTITY__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        CCP_MEASURABLE_QUANTITY type);

/**
 * Send CCP PDU measurement_rsp.
 *
 * DIR BOTH
 * Measured value passed up to host by Curator or to
 * Curator by subsystem
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c type
 * Indication of what quantity is being supplied
 *
 *   * Non-parameter argument \c value:
 * List of values supplied by subsystem.  When sending
 * from multiple subsystems to the host, this will contain
 * a concatenation of all the subsystem lists in subsystem
 * order.  At this level it is assumed that the sender and
 * receiver will know and agree on the number and
 * meaning of the entries in this array because this is a
 * relatively rarely-changing property of the hardware
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_MEASUREMENT_RSP_VALUE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_measurement_rsp(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_MEASURABLE_QUANTITY__VARLEN(CCP_SIGNAL_ID_MEASUREMENT_RSP, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU measurement_rsp.
 *
 * DIR BOTH
 * Measured value passed up to host by Curator or to
 * Curator by subsystem
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c type
 * Indication of what quantity is being supplied
 *
 *   Non-parameter argument \c value:
 * List of values supplied by subsystem.  When sending
 * from multiple subsystems to the host, this will contain
 * a concatenation of all the subsystem lists in subsystem
 * order.  At this level it is assumed that the sender and
 * receiver will know and agree on the number and
 * meaning of the entries in this array because this is a
 * relatively rarely-changing property of the hardware
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_MEASUREMENT_RSP_VALUE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_MEASURABLE_QUANTITY__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_MEASURABLE_QUANTITY *type);

/**
 * Analyse received CCP PDU measurement_rsp.
 *
 * DIR BOTH
 * Measured value passed up to host by Curator or to
 * Curator by subsystem
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c type
 * Indication of what quantity is being supplied
 *
 *   Non-parameter argument \c value:
 * List of values supplied by subsystem.  When sending
 * from multiple subsystems to the host, this will contain
 * a concatenation of all the subsystem lists in subsystem
 * order.  At this level it is assumed that the sender and
 * receiver will know and agree on the number and
 * meaning of the entries in this array because this is a
 * relatively rarely-changing property of the hardware
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_MEASUREMENT_RSP_VALUE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_measurement_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_MEASURABLE_QUANTITY__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_MEASUREMENT_IND_VALUE \
    CCP_MEASUREMENT_IND_VALUE_WORD_OFFSET

/**
 * Send CCP PDU measurement_ind.
 *
 * DIR TO_CURATOR
 * Indication from subsystem of a measurement, presumably
 * triggered by some internal event.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c type
 * Indication of what quantity is being supplied
 *
 *   * Non-parameter argument \c value:
 * List of values supplied by subsystem. This must always
 * contain the complete set of the subsystem's sensor readings
 * for the specified quantity.   At this level it is assumed
 * that both the sender and receiver will know and agree on
 * the number and meaning of the entries in this array
 * because this is a relatively rarely-changing property
 * of the hardware
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_MEASUREMENT_IND_VALUE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_measurement_ind(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_MEASURABLE_QUANTITY__VARLEN(CCP_SIGNAL_ID_MEASUREMENT_IND, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU measurement_ind.
 *
 * DIR TO_CURATOR
 * Indication from subsystem of a measurement, presumably
 * triggered by some internal event.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c type
 * Indication of what quantity is being supplied
 *
 *   Non-parameter argument \c value:
 * List of values supplied by subsystem. This must always
 * contain the complete set of the subsystem's sensor readings
 * for the specified quantity.   At this level it is assumed
 * that both the sender and receiver will know and agree on
 * the number and meaning of the entries in this array
 * because this is a relatively rarely-changing property
 * of the hardware
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_MEASUREMENT_IND_VALUE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_measurement_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_MEASURABLE_QUANTITY__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU clock_info_req.
 *
 * DIR TO_CURATOR
 * This signal allows a subsystem to request clock info
 * such as frequencies, dividers etc. Some subsystems need this, some
 * don't, hence the provision of a request. It has no parameters
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_clock_info_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_CLOCK_INFO_REQ, arg1)

/**
 * Analyse received CCP PDU clock_info_req.
 *
 * DIR TO_CURATOR
 * This signal allows a subsystem to request clock info
 * such as frequencies, dividers etc. Some subsystems need this, some
 * don't, hence the provision of a request. It has no parameters
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_clock_info_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_CLOCK_INFO_RSP_DATA \
    CCP_CLOCK_INFO_RSP_DATA_WORD_OFFSET

/**
 * Send CCP PDU clock_info_rsp.
 *
 * DIR FROM_CURATOR
 * This signal provides a subsystem with clock info
 * such as frequencies, dividers etc. Some subsystems need this, some
 * don't. Its parameters are keyword/value pairs
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * Non-parameter argument \c data:
 * Key/value pairs.
 * Keys are each one uint16; value lengths depend on the key
 * so the resulting array needs to be analysed one element at
 * a time.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_CLOCK_INFO_RSP_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_clock_info_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_VARLEN(CCP_SIGNAL_ID_CLOCK_INFO_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU clock_info_rsp.
 *
 * DIR FROM_CURATOR
 * This signal provides a subsystem with clock info
 * such as frequencies, dividers etc. Some subsystems need this, some
 * don't. Its parameters are keyword/value pairs
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   Non-parameter argument \c data:
 * Key/value pairs.
 * Keys are each one uint16; value lengths depend on the key
 * so the resulting array needs to be analysed one element at
 * a time.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_CLOCK_INFO_RSP_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_clock_info_rsp(arg1, arg2) \
    ccp_recv_pdu_VARLEN(arg1, arg2)


/**
 * Send CCP PDU pmu_config_req.
 *
 * DIR TO_CURATOR
 * A request for information about the state of the
 * Power Management Unit (PMU). At least one bit must be
 * set in pmu_config_req_flags for the request to be useful.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pmu_config_req_flags
 * A set of bit flags for the request from the type
 * CCP_PMU_CONFIG_REQ_FLAGS.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_config_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_PMU_CONFIG_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU pmu_config_req.
 *
 * DIR TO_CURATOR
 * A request for information about the state of the
 * Power Management Unit (PMU). At least one bit must be
 * set in pmu_config_req_flags for the request to be useful.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pmu_config_req_flags
 * A set of bit flags for the request from the type
 * CCP_PMU_CONFIG_REQ_FLAGS.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_config_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU pmu_config_rsp.
 *
 * DIR FROM_CURATOR
 * A response to PMU_CONFIG_REQ if the bit was set in
 * pmu_config_req_flags to indicate a response was required.
 * It contains information about the state of the Power Management
 * Unit (PMU).
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pmu_config_rsp_flags
 * A set of bit flags from the type CCP_PMU_CONFIG_RSP_FLAGS
 * indicating the state of the PMU.
 *
 *   * \c use_case
 * The use case for the PMU as documented in the Qualcomm document
 * CS-212104-DD; these are already numeric, so there is no special
 * enumeration.  The value 0xFFFF indicates the PMU is configured
 * in some manner out of the scope of that document or its
 * successors.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_config_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID_PMU_CONFIG_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU pmu_config_rsp.
 *
 * DIR FROM_CURATOR
 * A response to PMU_CONFIG_REQ if the bit was set in
 * pmu_config_req_flags to indicate a response was required.
 * It contains information about the state of the Power Management
 * Unit (PMU).
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pmu_config_rsp_flags
 * A set of bit flags from the type CCP_PMU_CONFIG_RSP_FLAGS
 * indicating the state of the PMU.
 *
 *   * \c use_case
 * The use case for the PMU as documented in the Qualcomm document
 * CS-212104-DD; these are already numeric, so there is no special
 * enumeration.  The value 0xFFFF indicates the PMU is configured
 * in some manner out of the scope of that document or its
 * successors.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_config_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU pmu_supply_voltage_ind.
 *
 * DIR FROM_CURATOR
 * Indication to a subsystem (or the host) that has requested
 * notification of supply voltage changes.  In that case the PDU
 * will always be sent once in response to the original
 * CCP_PMU_CONFIG_REQ in order to indicate the current supply
 * voltage.  Updates will be sent as available until a
 * PMU_CONFIG_REQ signal is received turning indications off.
 * 
 * DEPRECATED: This PDU is no longer sent. See CUR-5368.
 * Power supply monitoring is no longer supported.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c min_supply_voltage_mv
 * A lower bound on the raw supply voltage.  Increases in voltage
 * are reported after a delay in an attempt to detect the
 * minimum voltage that can be relied on over long periods.
 * Note this means that requests for voltage monitoring shortly
 * after system start may not report the current supply voltage
 * immediately, but will do so after a delay.  The delay
 * is configurable by the Curator MIB attribute
 * PmuVoltageMonitoringConfig.
 * Only certain voltages to which the hardware is sensitive can
 * be reported.  If all inputs to the chip are from an already
 * regulated source no supply voltage is reported.  If the chip
 * is connected directly to the raw supply then the values 2200,
 * 2500 and 2700 mV can always be reported.  If the switched
 * mode power supply is also connected to the external supply the
 * supply voltages 2800, 3000, 3200 and 3400 mV can also be
 * reported.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_supply_voltage_ind(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_PMU_SUPPLY_VOLTAGE_IND, arg1, arg2)

/**
 * Analyse received CCP PDU pmu_supply_voltage_ind.
 *
 * DIR FROM_CURATOR
 * Indication to a subsystem (or the host) that has requested
 * notification of supply voltage changes.  In that case the PDU
 * will always be sent once in response to the original
 * CCP_PMU_CONFIG_REQ in order to indicate the current supply
 * voltage.  Updates will be sent as available until a
 * PMU_CONFIG_REQ signal is received turning indications off.
 * 
 * DEPRECATED: This PDU is no longer sent. See CUR-5368.
 * Power supply monitoring is no longer supported.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c min_supply_voltage_mv
 * A lower bound on the raw supply voltage.  Increases in voltage
 * are reported after a delay in an attempt to detect the
 * minimum voltage that can be relied on over long periods.
 * Note this means that requests for voltage monitoring shortly
 * after system start may not report the current supply voltage
 * immediately, but will do so after a delay.  The delay
 * is configurable by the Curator MIB attribute
 * PmuVoltageMonitoringConfig.
 * Only certain voltages to which the hardware is sensitive can
 * be reported.  If all inputs to the chip are from an already
 * regulated source no supply voltage is reported.  If the chip
 * is connected directly to the raw supply then the values 2200,
 * 2500 and 2700 mV can always be reported.  If the switched
 * mode power supply is also connected to the external supply the
 * supply voltages 2800, 3000, 3200 and 3400 mV can also be
 * reported.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_supply_voltage_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SUBSYSTEM_VERSION_IND_ID_STRING \
    CCP_SUBSYSTEM_VERSION_IND_ID_STRING_WORD_OFFSET

/**
 * Send CCP PDU subsystem_version_ind.
 *
 * DIR FROM_CURATOR
 * Sent spontaneously to the host when a subsystem is fully
 * operational.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c hw_version
 * The subsystem hardware version ID, from SUB_SYS_DEBUGGER_REF_ID.
 * At the time of writing this is the same for all subsystems.
 * For subsystems other than the Curator the address of this
 * register is looked up in the subsystem symbol lookup
 * table.  The value zero here indicates the table could not be
 * accessed; this may be either because no subsystem of the
 * given type was present, or because there was no firmware
 * present in the subsystem.
 *
 *   * \c fw_rom_version
 * The subsystem firmware ROM version.
 * The meaning of this value is specific to the subsystem.
 * A 16-bit version number will be suitably padded.
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 *
 *   * \c patched_fw_version
 * The level to which the firmware being started has been patched.
 * This is the fw version of the subsystem after it has been
 * patched, which it has been at the point the current message
 * is sent; the special value 0xFFFFFFFF would only be seen if
 * the subsystem was immediately turned off.  The value zero
 * indicates the subsystem was started but no patch was available.
 *
 *   * Non-parameter argument \c id_string:
 * An optional identifier string for the firmware.
 * This is an identifier string consisting of octets in
 * the subsystem's usual representation, typically UTF-8 if
 * not US ASCII.  The octets are packed little-endian into
 * 16-bit words.  If the length is an odd number of octets the
 * upper octet of the last word is zero.  If the length is an
 * even number of octets, there is no extra padding (the type
 * "NullTermString" is a barefaced lie; it may be a hack to keep
 * the autogeneration tools happy).
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SUBSYSTEM_VERSION_IND_ID_STRING_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsystem_version_ind(arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
    ccp_send_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16__uint32__uint32__VARLEN(CCP_SIGNAL_ID_SUBSYSTEM_VERSION_IND, arg1, arg2, arg3, arg4, arg5, arg6, arg7)

/**
 * Analyse received CCP PDU subsystem_version_ind.
 *
 * DIR FROM_CURATOR
 * Sent spontaneously to the host when a subsystem is fully
 * operational.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c hw_version
 * The subsystem hardware version ID, from SUB_SYS_DEBUGGER_REF_ID.
 * At the time of writing this is the same for all subsystems.
 * For subsystems other than the Curator the address of this
 * register is looked up in the subsystem symbol lookup
 * table.  The value zero here indicates the table could not be
 * accessed; this may be either because no subsystem of the
 * given type was present, or because there was no firmware
 * present in the subsystem.
 *
 *   * \c fw_rom_version
 * The subsystem firmware ROM version.
 * The meaning of this value is specific to the subsystem.
 * A 16-bit version number will be suitably padded.
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 *
 *   * \c patched_fw_version
 * The level to which the firmware being started has been patched.
 * This is the fw version of the subsystem after it has been
 * patched, which it has been at the point the current message
 * is sent; the special value 0xFFFFFFFF would only be seen if
 * the subsystem was immediately turned off.  The value zero
 * indicates the subsystem was started but no patch was available.
 *
 *   Non-parameter argument \c id_string:
 * An optional identifier string for the firmware.
 * This is an identifier string consisting of octets in
 * the subsystem's usual representation, typically UTF-8 if
 * not US ASCII.  The octets are packed little-endian into
 * 16-bit words.  If the length is an odd number of octets the
 * upper octet of the last word is zero.  If the length is an
 * even number of octets, there is no extra padding (the type
 * "NullTermString" is a barefaced lie; it may be a hack to keep
 * the autogeneration tools happy).
 * For subsystems other than the Curator the address of this
 * 16-bit register is looked up in the subsystem symbol lookup
 * table.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SUBSYSTEM_VERSION_IND_ID_STRING_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsystem_version_ind(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_recv_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16__uint32__uint32__VARLEN(arg1, arg2, arg3, arg4, arg5, arg6)


/**
 * Send CCP PDU host_wake_sleep_req.
 *
 * DIR TO_CURATOR
 * Sent by the host to ask the chip permission to go to sleep
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c can_wake
 * Bitmap indicating subsystems that are allowed to wake the host.
 * This is to satisfy a Windows device manager option to prevent
 * devices waking the host
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_host_wake_sleep_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_HOST_WAKE_SLEEP_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU host_wake_sleep_req.
 *
 * DIR TO_CURATOR
 * Sent by the host to ask the chip permission to go to sleep
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c can_wake
 * Bitmap indicating subsystems that are allowed to wake the host.
 * This is to satisfy a Windows device manager option to prevent
 * devices waking the host
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_host_wake_sleep_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU host_wake_sleep_ack_rsp.
 *
 * DIR BOTH
 * Sent (by subsystems to the Curator, and by the Curator to the host)
 * to grant permission for the host to go to sleep
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_host_wake_sleep_ack_rsp(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOST_WAKE_SLEEP_ACK_RSP, arg1)

/**
 * Analyse received CCP PDU host_wake_sleep_ack_rsp.
 *
 * DIR BOTH
 * Sent (by subsystems to the Curator, and by the Curator to the host)
 * to grant permission for the host to go to sleep
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_host_wake_sleep_ack_rsp(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU host_wake_sleep_nak_rsp.
 *
 * DIR BOTH
 * Sent (by subsystems to the Curator, and by the Curator to the host)
 * to deny permission for the host to go to sleep
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_host_wake_sleep_nak_rsp(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOST_WAKE_SLEEP_NAK_RSP, arg1)

/**
 * Analyse received CCP PDU host_wake_sleep_nak_rsp.
 *
 * DIR BOTH
 * Sent (by subsystems to the Curator, and by the Curator to the host)
 * to deny permission for the host to go to sleep
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_host_wake_sleep_nak_rsp(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU host_wake_wake_req.
 *
 * DIR TO_CURATOR
 * Sent by subsystems to request the Curator to wake the host up
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_host_wake_wake_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOST_WAKE_WAKE_REQ, arg1)

/**
 * Analyse received CCP PDU host_wake_wake_req.
 *
 * DIR TO_CURATOR
 * Sent by subsystems to request the Curator to wake the host up
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_host_wake_wake_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU host_wake_awake_ind.
 *
 * DIR BOTH
 * Sent (by the host to the Curator, and by the Curator to subsystems)
 * to indicate that the host is awake
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_host_wake_awake_ind(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOST_WAKE_AWAKE_IND, arg1)

/**
 * Analyse received CCP PDU host_wake_awake_ind.
 *
 * DIR BOTH
 * Sent (by the host to the Curator, and by the Curator to subsystems)
 * to indicate that the host is awake
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_host_wake_awake_ind(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU deep_sleep_ready_no_deadline.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate the subsystem
 * is ready to sleep and does not need to be owken by the timer.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_ready_no_deadline(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_DEEP_SLEEP_READY_NO_DEADLINE, arg1)

/**
 * Analyse received CCP PDU deep_sleep_ready_no_deadline.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate the subsystem
 * is ready to sleep and does not need to be owken by the timer.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_ready_no_deadline(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_DEEP_SLEEP_READY_ANY_DEADLINE \
    CCP_DEEP_SLEEP_READY_ANY_DEADLINE_WORD_OFFSET

/**
 * Send CCP PDU deep_sleep_ready_any.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate the subsystem
 * is ready to sleep.  This PDU is a superset of other
 * DEEP_SLEEP_READY PDUs and also provides a set of bit flags
 * for additional options.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c deep_sleep_ready_flags
 * A set of bit flags drawn from the type
 * CCP_DEEP_SLEEP_READY_FLAGS
 *
 *   * Non-parameter argument \c deadline:
 * This field may appear zero, one or two times in the PDU.
 * 
 * If it does not appear, there is no wake deadline. This
 * mimics CCP_DEEP_SLEEP_READY_NO_DEADLINE.
 * 
 * If it appears once, there is a single hard deadline.
 * This mimics CCP_DEEP_SLEEP_READY.
 * 
 * If it appears twice, the first deadline is the
 * earliest wake time and the second the latest wake
 * time.  This mimics CCP_DEEP_SLEEP_READY_SLOPPY.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_DEEP_SLEEP_READY_ANY_DEADLINE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__VARLEN(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                 uint16 *pdu, uint16 pdu_len_words,
                                 uint16 deep_sleep_ready_flags);

/**
 * Send CCP PDU deep_sleep_ready_any.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate the subsystem
 * is ready to sleep.  This PDU is a superset of other
 * DEEP_SLEEP_READY PDUs and also provides a set of bit flags
 * for additional options.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c deep_sleep_ready_flags
 * A set of bit flags drawn from the type
 * CCP_DEEP_SLEEP_READY_FLAGS
 *
 *   * Non-parameter argument \c deadline:
 * This field may appear zero, one or two times in the PDU.
 * 
 * If it does not appear, there is no wake deadline. This
 * mimics CCP_DEEP_SLEEP_READY_NO_DEADLINE.
 * 
 * If it appears once, there is a single hard deadline.
 * This mimics CCP_DEEP_SLEEP_READY.
 * 
 * If it appears twice, the first deadline is the
 * earliest wake time and the second the latest wake
 * time.  This mimics CCP_DEEP_SLEEP_READY_SLOPPY.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_DEEP_SLEEP_READY_ANY_DEADLINE_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_ready_any(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_uint16__VARLEN(CCP_SIGNAL_ID_DEEP_SLEEP_READY_ANY, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU deep_sleep_ready_any.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate the subsystem
 * is ready to sleep.  This PDU is a superset of other
 * DEEP_SLEEP_READY PDUs and also provides a set of bit flags
 * for additional options.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c deep_sleep_ready_flags
 * A set of bit flags drawn from the type
 * CCP_DEEP_SLEEP_READY_FLAGS
 *
 *   Non-parameter argument \c deadline:
 * This field may appear zero, one or two times in the PDU.
 * 
 * If it does not appear, there is no wake deadline. This
 * mimics CCP_DEEP_SLEEP_READY_NO_DEADLINE.
 * 
 * If it appears once, there is a single hard deadline.
 * This mimics CCP_DEEP_SLEEP_READY.
 * 
 * If it appears twice, the first deadline is the
 * earliest wake time and the second the latest wake
 * time.  This mimics CCP_DEEP_SLEEP_READY_SLOPPY.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_DEEP_SLEEP_READY_ANY_DEADLINE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__VARLEN(const uint16 *pdu, uint16 pdu_len_words,
                                 uint16 *deep_sleep_ready_flags);

/**
 * Analyse received CCP PDU deep_sleep_ready_any.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to the Curator to indicate the subsystem
 * is ready to sleep.  This PDU is a superset of other
 * DEEP_SLEEP_READY PDUs and also provides a set of bit flags
 * for additional options.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c deep_sleep_ready_flags
 * A set of bit flags drawn from the type
 * CCP_DEEP_SLEEP_READY_FLAGS
 *
 *   Non-parameter argument \c deadline:
 * This field may appear zero, one or two times in the PDU.
 * 
 * If it does not appear, there is no wake deadline. This
 * mimics CCP_DEEP_SLEEP_READY_NO_DEADLINE.
 * 
 * If it appears once, there is a single hard deadline.
 * This mimics CCP_DEEP_SLEEP_READY.
 * 
 * If it appears twice, the first deadline is the
 * earliest wake time and the second the latest wake
 * time.  This mimics CCP_DEEP_SLEEP_READY_SLOPPY.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_DEEP_SLEEP_READY_ANY_DEADLINE_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_ready_any(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU curator_temperature_req.
 *
 * DIR TO_CURATOR
 * Request for a reading of the temperature sensor in the chip's PMU
 * macro which the curator can access.  The result is returned in
 * CCP_CURATOR_TEMPERATURE_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_curator_temperature_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_CURATOR_TEMPERATURE_REQ, arg1)

/**
 * Analyse received CCP PDU curator_temperature_req.
 *
 * DIR TO_CURATOR
 * Request for a reading of the temperature sensor in the chip's PMU
 * macro which the curator can access.  The result is returned in
 * CCP_CURATOR_TEMPERATURE_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_curator_temperature_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU curator_temperature_rsp.
 *
 * DIR FROM_CURATOR
 * Reading of the temperature sensor in the chip's PMU macro which the
 * curator can access in response to CCP_CURATOR_TEMPERATURE_REQ.
 * For crescendo, the result is supplied as a raw reading that will
 * need processing; these values require knowledge of the characteristics
 * of the hardware to interpret. The two raw readings are Vbg and Vptat.
 * In addition, an estimate of the actual temperature in degrees Celsius
 * may be supplied using any conversion factors available in the Curator.
 * For aura, the estimated actual temperature in degree Clesius is reported.
 * As the pmu temperature sensor in aura does not provide the measurement of Vbg and
 * Vptat, so all 0s are reported for Vbg and Vptat.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c v_ptat
 * The value of the PTAT voltage reading from the PMU temperature
 * sensor which the curator can access. For Aura, as the temperature
 * sensor in PMU does not provide the measurement result of Vptat, 0 is
 * reported for aura.
 *
 *   * \c v_bg
 * The value of the bandgap voltage reading from the PMU
 * temperature sensor which the curator can access. For Aura, as the
 * temperature sensor in PMU does not provide the measurement result of
 * Vbg, 0 is reported for Aura.
 *
 *   * \c t_curator_deg_c
 * If the curator MIBs to define the conversion factors are supplied,
 * an estimate of the temperature in degrees Celsius is reported.
 * The special value 0x7FFF for temperature in degrees Celsius
 * might be reported if the curator MIBs to define the conversion
 * facotr are not available or the temperature sensor
 * failed to do the measurement. For crescendo, the values of
 * v_ptat and v_bg are still usable in this case.
 * Note that the default conversion factors
 * may not be correct, so this value may not be reliable.
 *
 *   * \c is_new_measurement
 * The flag indicates the reported results are newly measured or
 * they are the old values which are recorded during the periodic
 * chip monitor measurement. For example, in case of crescendo,
 * the PMU ADC is used to measure the temperature and USB etc.
 * If the PMU ADC is busy to do the other measurement when
 * the request ccp_curator_temperature_req is received,
 * the recorded temperature measurement will be reported instead of
 * the new measurement.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16__int16__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 v_ptat,
        uint16 v_bg,
        int16 t_curator_deg_c,
        uint16 is_new_measurement);

/**
 * Send CCP PDU curator_temperature_rsp.
 *
 * DIR FROM_CURATOR
 * Reading of the temperature sensor in the chip's PMU macro which the
 * curator can access in response to CCP_CURATOR_TEMPERATURE_REQ.
 * For crescendo, the result is supplied as a raw reading that will
 * need processing; these values require knowledge of the characteristics
 * of the hardware to interpret. The two raw readings are Vbg and Vptat.
 * In addition, an estimate of the actual temperature in degrees Celsius
 * may be supplied using any conversion factors available in the Curator.
 * For aura, the estimated actual temperature in degree Clesius is reported.
 * As the pmu temperature sensor in aura does not provide the measurement of Vbg and
 * Vptat, so all 0s are reported for Vbg and Vptat.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c v_ptat
 * The value of the PTAT voltage reading from the PMU temperature
 * sensor which the curator can access. For Aura, as the temperature
 * sensor in PMU does not provide the measurement result of Vptat, 0 is
 * reported for aura.
 *
 *   * \c v_bg
 * The value of the bandgap voltage reading from the PMU
 * temperature sensor which the curator can access. For Aura, as the
 * temperature sensor in PMU does not provide the measurement result of
 * Vbg, 0 is reported for Aura.
 *
 *   * \c t_curator_deg_c
 * If the curator MIBs to define the conversion factors are supplied,
 * an estimate of the temperature in degrees Celsius is reported.
 * The special value 0x7FFF for temperature in degrees Celsius
 * might be reported if the curator MIBs to define the conversion
 * facotr are not available or the temperature sensor
 * failed to do the measurement. For crescendo, the values of
 * v_ptat and v_bg are still usable in this case.
 * Note that the default conversion factors
 * may not be correct, so this value may not be reliable.
 *
 *   * \c is_new_measurement
 * The flag indicates the reported results are newly measured or
 * they are the old values which are recorded during the periodic
 * chip monitor measurement. For example, in case of crescendo,
 * the PMU ADC is used to measure the temperature and USB etc.
 * If the PMU ADC is busy to do the other measurement when
 * the request ccp_curator_temperature_req is received,
 * the recorded temperature measurement will be reported instead of
 * the new measurement.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_curator_temperature_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_uint16__uint16__int16__uint16(CCP_SIGNAL_ID_CURATOR_TEMPERATURE_RSP, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU curator_temperature_rsp.
 *
 * DIR FROM_CURATOR
 * Reading of the temperature sensor in the chip's PMU macro which the
 * curator can access in response to CCP_CURATOR_TEMPERATURE_REQ.
 * For crescendo, the result is supplied as a raw reading that will
 * need processing; these values require knowledge of the characteristics
 * of the hardware to interpret. The two raw readings are Vbg and Vptat.
 * In addition, an estimate of the actual temperature in degrees Celsius
 * may be supplied using any conversion factors available in the Curator.
 * For aura, the estimated actual temperature in degree Clesius is reported.
 * As the pmu temperature sensor in aura does not provide the measurement of Vbg and
 * Vptat, so all 0s are reported for Vbg and Vptat.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c v_ptat
 * The value of the PTAT voltage reading from the PMU temperature
 * sensor which the curator can access. For Aura, as the temperature
 * sensor in PMU does not provide the measurement result of Vptat, 0 is
 * reported for aura.
 *
 *   * \c v_bg
 * The value of the bandgap voltage reading from the PMU
 * temperature sensor which the curator can access. For Aura, as the
 * temperature sensor in PMU does not provide the measurement result of
 * Vbg, 0 is reported for Aura.
 *
 *   * \c t_curator_deg_c
 * If the curator MIBs to define the conversion factors are supplied,
 * an estimate of the temperature in degrees Celsius is reported.
 * The special value 0x7FFF for temperature in degrees Celsius
 * might be reported if the curator MIBs to define the conversion
 * facotr are not available or the temperature sensor
 * failed to do the measurement. For crescendo, the values of
 * v_ptat and v_bg are still usable in this case.
 * Note that the default conversion factors
 * may not be correct, so this value may not be reliable.
 *
 *   * \c is_new_measurement
 * The flag indicates the reported results are newly measured or
 * they are the old values which are recorded during the periodic
 * chip monitor measurement. For example, in case of crescendo,
 * the PMU ADC is used to measure the temperature and USB etc.
 * If the PMU ADC is busy to do the other measurement when
 * the request ccp_curator_temperature_req is received,
 * the recorded temperature measurement will be reported instead of
 * the new measurement.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16__int16__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *v_ptat,
        uint16 *v_bg,
        int16 *t_curator_deg_c,
        uint16 *is_new_measurement);

/**
 * Analyse received CCP PDU curator_temperature_rsp.
 *
 * DIR FROM_CURATOR
 * Reading of the temperature sensor in the chip's PMU macro which the
 * curator can access in response to CCP_CURATOR_TEMPERATURE_REQ.
 * For crescendo, the result is supplied as a raw reading that will
 * need processing; these values require knowledge of the characteristics
 * of the hardware to interpret. The two raw readings are Vbg and Vptat.
 * In addition, an estimate of the actual temperature in degrees Celsius
 * may be supplied using any conversion factors available in the Curator.
 * For aura, the estimated actual temperature in degree Clesius is reported.
 * As the pmu temperature sensor in aura does not provide the measurement of Vbg and
 * Vptat, so all 0s are reported for Vbg and Vptat.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c v_ptat
 * The value of the PTAT voltage reading from the PMU temperature
 * sensor which the curator can access. For Aura, as the temperature
 * sensor in PMU does not provide the measurement result of Vptat, 0 is
 * reported for aura.
 *
 *   * \c v_bg
 * The value of the bandgap voltage reading from the PMU
 * temperature sensor which the curator can access. For Aura, as the
 * temperature sensor in PMU does not provide the measurement result of
 * Vbg, 0 is reported for Aura.
 *
 *   * \c t_curator_deg_c
 * If the curator MIBs to define the conversion factors are supplied,
 * an estimate of the temperature in degrees Celsius is reported.
 * The special value 0x7FFF for temperature in degrees Celsius
 * might be reported if the curator MIBs to define the conversion
 * facotr are not available or the temperature sensor
 * failed to do the measurement. For crescendo, the values of
 * v_ptat and v_bg are still usable in this case.
 * Note that the default conversion factors
 * may not be correct, so this value may not be reliable.
 *
 *   * \c is_new_measurement
 * The flag indicates the reported results are newly measured or
 * they are the old values which are recorded during the periodic
 * chip monitor measurement. For example, in case of crescendo,
 * the PMU ADC is used to measure the temperature and USB etc.
 * If the PMU ADC is busy to do the other measurement when
 * the request ccp_curator_temperature_req is received,
 * the recorded temperature measurement will be reported instead of
 * the new measurement.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_curator_temperature_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_recv_pdu_uint16__uint16__int16__uint16(arg1, arg2, arg3, arg4, arg5, arg6)


/**
 * Send CCP PDU service_dependency_start_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Sent to provider when a START_CONFIRM would have been sent to the
 * consumer except that the provider has indicated it has dependency
 * services it needs to start before the service can finally be indicated
 * to the consumer as available.  This works around globserv's inability
 * to handle more than one service start at once in a way that gives more
 * flexibility than the autostart mechanism was designed for.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service that has been started and is waiting
 * for the provider's OK to be confirmed to the consumer.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_dependency_start_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_SERVICE_DEPENDENCY_START_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU service_dependency_start_req.
 *
 * DIR FROM_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Sent to provider when a START_CONFIRM would have been sent to the
 * consumer except that the provider has indicated it has dependency
 * services it needs to start before the service can finally be indicated
 * to the consumer as available.  This works around globserv's inability
 * to handle more than one service start at once in a way that gives more
 * flexibility than the autostart mechanism was designed for.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service that has been started and is waiting
 * for the provider's OK to be confirmed to the consumer.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_dependency_start_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU service_dependency_start_cfm.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Sent to the Curator by a provider which requested leave to start up
 * dependency services before the original service was notified to the
 * conumer as started, indicating that it's now appropriate to send the
 * consumer the START_CFM
 * 
 * Note: in principle it would be possible to send additional IEs in this
 * message for globserv to add in to those supplied in the original
 * START_CONFIRM, but we don't need that at the time of writing, so for
 * the sake of simplicity we'll leave it out
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service whose START_CFM can now be sent.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_dependency_start_cfm(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_SERVICE_DEPENDENCY_START_CFM, arg1, arg2)

/**
 * Analyse received CCP PDU service_dependency_start_cfm.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Sent to the Curator by a provider which requested leave to start up
 * dependency services before the original service was notified to the
 * conumer as started, indicating that it's now appropriate to send the
 * consumer the START_CFM
 * 
 * Note: in principle it would be possible to send additional IEs in this
 * message for globserv to add in to those supplied in the original
 * START_CONFIRM, but we don't need that at the time of writing, so for
 * the sake of simplicity we'll leave it out
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service whose START_CFM can now be sent.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_dependency_start_cfm(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU service_dependency_start_failure_cfm.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Sent to the Curator by a provider which requested leave to start up
 * dependency services before the original service was notified to the
 * consumer as started, indicating that something has gone wrong, and
 * that globserv needs to abort the pending START_CFM
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c stag
 * Service tag for the service which should now be stopped and marked
 * as failing.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_service_dependency_start_failure_cfm(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_SERVICE_DEPENDENCY_START_FAILURE_CFM, arg1, arg2)

/**
 * Analyse received CCP PDU service_dependency_start_failure_cfm.
 *
 * DIR TO_CURATOR
 * HEADER hydra_service/hydra_service.h
 * Sent to the Curator by a provider which requested leave to start up
 * dependency services before the original service was notified to the
 * consumer as started, indicating that something has gone wrong, and
 * that globserv needs to abort the pending START_CFM
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c stag
 * Service tag for the service which should now be stopped and marked
 * as failing.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_service_dependency_start_failure_cfm(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_UPDATE_PIO_CONFIG_REQ_DATA \
    CCP_UPDATE_PIO_CONFIG_REQ_DATA_WORD_OFFSET

/**
 * Send CCP PDU update_pio_config_req.
 *
 * DIR TO_CURATOR
 * Request to update the PIO configurations including drive strength, pull strength,
 * pull direction, pull enable, slew enable, sticky enable
 * and the allocated subsystem user etc.
 * The response is the message CCP_UPDATE_PIO_CONFIG_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   * Non-parameter argument \c data:
 * Configuration key/value pairs.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_UPDATE_PIO_CONFIG_REQ_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_update_pio_config_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_uint16__VARLEN(CCP_SIGNAL_ID_UPDATE_PIO_CONFIG_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU update_pio_config_req.
 *
 * DIR TO_CURATOR
 * Request to update the PIO configurations including drive strength, pull strength,
 * pull direction, pull enable, slew enable, sticky enable
 * and the allocated subsystem user etc.
 * The response is the message CCP_UPDATE_PIO_CONFIG_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   Non-parameter argument \c data:
 * Configuration key/value pairs.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_UPDATE_PIO_CONFIG_REQ_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_update_pio_config_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU update_pio_config_rsp.
 *
 * DIR FROM_CURATOR
 * Response of CCP_UPDATE_PIO_CONFIG_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   * \c status
 * The return status of the request CCP_UPDATE_PIO_CONFIG_REQ.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__CCP_UPDATE_PIO_CONFIG_STATUS(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 pio_no,
        CCP_UPDATE_PIO_CONFIG_STATUS status);

/**
 * Send CCP PDU update_pio_config_rsp.
 *
 * DIR FROM_CURATOR
 * Response of CCP_UPDATE_PIO_CONFIG_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   * \c status
 * The return status of the request CCP_UPDATE_PIO_CONFIG_REQ.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_update_pio_config_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__CCP_UPDATE_PIO_CONFIG_STATUS(CCP_SIGNAL_ID_UPDATE_PIO_CONFIG_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU update_pio_config_rsp.
 *
 * DIR FROM_CURATOR
 * Response of CCP_UPDATE_PIO_CONFIG_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   * \c status
 * The return status of the request CCP_UPDATE_PIO_CONFIG_REQ.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__CCP_UPDATE_PIO_CONFIG_STATUS(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *pio_no,
        CCP_UPDATE_PIO_CONFIG_STATUS *status);

/**
 * Analyse received CCP PDU update_pio_config_rsp.
 *
 * DIR FROM_CURATOR
 * Response of CCP_UPDATE_PIO_CONFIG_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   * \c status
 * The return status of the request CCP_UPDATE_PIO_CONFIG_REQ.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_update_pio_config_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__CCP_UPDATE_PIO_CONFIG_STATUS(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU pio_config_query_req.
 *
 * DIR TO_CURATOR
 * Request to report the PIO configurations including drive strength, pull strength,
 * pull direction, pull enable, slew enable, sticky enable
 * and the allocated subsystem user etc.
 * The response is the message CCP_PIO_CONFIG_QUERY_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pio_config_query_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_PIO_CONFIG_QUERY_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU pio_config_query_req.
 *
 * DIR TO_CURATOR
 * Request to report the PIO configurations including drive strength, pull strength,
 * pull direction, pull enable, slew enable, sticky enable
 * and the allocated subsystem user etc.
 * The response is the message CCP_PIO_CONFIG_QUERY_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pio_config_query_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_PIO_CONFIG_QUERY_RSP_DATA \
    CCP_PIO_CONFIG_QUERY_RSP_DATA_WORD_OFFSET

/**
 * Send CCP PDU pio_config_query_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_PIO_CONFIG_QUERY_REQ.
 * It reports the PIO configurations including drive strength, pull strength,
 * pull direction, pull enable, slew enable, sticky enable
 * and the allocated subsystem user etc.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   * \c status
 * The status of the CCP_PIO_CONFIG_QUERY_REQ.  If this is not
 * CCP_PIO_CONFIG_QUERY_STATUS_SUCCESS no value is returned.
 *
 *   * Non-parameter argument \c data:
 * Configuration key/value pairs. As the different PIO has the different
 * available configuration. If the PIO configuration is not available for that PIO,
 * the value 0xFF is returned.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_PIO_CONFIG_QUERY_RSP_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__CCP_PIO_CONFIG_QUERY_STATUS__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        uint16 pio_no,
        CCP_PIO_CONFIG_QUERY_STATUS status);

/**
 * Send CCP PDU pio_config_query_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_PIO_CONFIG_QUERY_REQ.
 * It reports the PIO configurations including drive strength, pull strength,
 * pull direction, pull enable, slew enable, sticky enable
 * and the allocated subsystem user etc.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   * \c status
 * The status of the CCP_PIO_CONFIG_QUERY_REQ.  If this is not
 * CCP_PIO_CONFIG_QUERY_STATUS_SUCCESS no value is returned.
 *
 *   * Non-parameter argument \c data:
 * Configuration key/value pairs. As the different PIO has the different
 * available configuration. If the PIO configuration is not available for that PIO,
 * the value 0xFF is returned.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_PIO_CONFIG_QUERY_RSP_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pio_config_query_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_uint16__CCP_PIO_CONFIG_QUERY_STATUS__VARLEN(CCP_SIGNAL_ID_PIO_CONFIG_QUERY_RSP, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU pio_config_query_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_PIO_CONFIG_QUERY_REQ.
 * It reports the PIO configurations including drive strength, pull strength,
 * pull direction, pull enable, slew enable, sticky enable
 * and the allocated subsystem user etc.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   * \c status
 * The status of the CCP_PIO_CONFIG_QUERY_REQ.  If this is not
 * CCP_PIO_CONFIG_QUERY_STATUS_SUCCESS no value is returned.
 *
 *   Non-parameter argument \c data:
 * Configuration key/value pairs. As the different PIO has the different
 * available configuration. If the PIO configuration is not available for that PIO,
 * the value 0xFF is returned.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_PIO_CONFIG_QUERY_RSP_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__CCP_PIO_CONFIG_QUERY_STATUS__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *pio_no,
        CCP_PIO_CONFIG_QUERY_STATUS *status);

/**
 * Analyse received CCP PDU pio_config_query_rsp.
 *
 * DIR FROM_CURATOR
 * Response to CCP_PIO_CONFIG_QUERY_REQ.
 * It reports the PIO configurations including drive strength, pull strength,
 * pull direction, pull enable, slew enable, sticky enable
 * and the allocated subsystem user etc.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pio_no
 * PIO number from 0 to (NUMBER_OF_PIOS -1).
 *
 *   * \c status
 * The status of the CCP_PIO_CONFIG_QUERY_REQ.  If this is not
 * CCP_PIO_CONFIG_QUERY_STATUS_SUCCESS no value is returned.
 *
 *   Non-parameter argument \c data:
 * Configuration key/value pairs. As the different PIO has the different
 * available configuration. If the PIO configuration is not available for that PIO,
 * the value 0xFF is returned.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_PIO_CONFIG_QUERY_RSP_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pio_config_query_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__CCP_PIO_CONFIG_QUERY_STATUS__VARLEN(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_DEEP_SLEEP_WAKE_MASK_EXT_DATA \
    CCP_DEEP_SLEEP_WAKE_MASK_EXT_DATA_WORD_OFFSET

/**
 * Send CCP PDU deep_sleep_wake_mask_ext.
 *
 * DIR TO_CURATOR
 * Supply the details of what can wake a subsystem.
 * Sent from subsystem to Curator; no reply.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c wake_mask
 * The mask of wake bits.
 * Use of this is defined in the Curator globsleep module
 * which manages this message.  Currently the lower 16 bits
 * are the hardware wake sources for the chip, the upper
 * 16 bits include extra definitions from the type
 * CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 *
 *   * Non-parameter argument \c data:
 * n lots of uint16 mask, uint16 invert
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_DEEP_SLEEP_WAKE_MASK_EXT_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint32__VARLEN(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                 uint16 *pdu, uint16 pdu_len_words,
                                 uint32 wake_mask);

/**
 * Send CCP PDU deep_sleep_wake_mask_ext.
 *
 * DIR TO_CURATOR
 * Supply the details of what can wake a subsystem.
 * Sent from subsystem to Curator; no reply.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c wake_mask
 * The mask of wake bits.
 * Use of this is defined in the Curator globsleep module
 * which manages this message.  Currently the lower 16 bits
 * are the hardware wake sources for the chip, the upper
 * 16 bits include extra definitions from the type
 * CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 *
 *   * Non-parameter argument \c data:
 * n lots of uint16 mask, uint16 invert
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_DEEP_SLEEP_WAKE_MASK_EXT_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_wake_mask_ext(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_uint32__VARLEN(CCP_SIGNAL_ID_DEEP_SLEEP_WAKE_MASK_EXT, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU deep_sleep_wake_mask_ext.
 *
 * DIR TO_CURATOR
 * Supply the details of what can wake a subsystem.
 * Sent from subsystem to Curator; no reply.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c wake_mask
 * The mask of wake bits.
 * Use of this is defined in the Curator globsleep module
 * which manages this message.  Currently the lower 16 bits
 * are the hardware wake sources for the chip, the upper
 * 16 bits include extra definitions from the type
 * CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 *
 *   Non-parameter argument \c data:
 * n lots of uint16 mask, uint16 invert
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_DEEP_SLEEP_WAKE_MASK_EXT_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint32__VARLEN(const uint16 *pdu, uint16 pdu_len_words,
                                 uint32 *wake_mask);

/**
 * Analyse received CCP PDU deep_sleep_wake_mask_ext.
 *
 * DIR TO_CURATOR
 * Supply the details of what can wake a subsystem.
 * Sent from subsystem to Curator; no reply.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c wake_mask
 * The mask of wake bits.
 * Use of this is defined in the Curator globsleep module
 * which manages this message.  Currently the lower 16 bits
 * are the hardware wake sources for the chip, the upper
 * 16 bits include extra definitions from the type
 * CCP_DEEP_SLEEP_WAKE_EXTRA_MASK.
 *
 *   Non-parameter argument \c data:
 * n lots of uint16 mask, uint16 invert
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_DEEP_SLEEP_WAKE_MASK_EXT_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_wake_mask_ext(arg1, arg2, arg3) \
    ccp_recv_pdu_uint32__VARLEN(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_DEEP_SLEEP_WAKE_PIO_EXT_REQ_DATA \
    CCP_DEEP_SLEEP_WAKE_PIO_EXT_REQ_DATA_WORD_OFFSET

/**
 * Send CCP PDU deep_sleep_wake_pio_ext_req.
 *
 * DIR FROM_CURATOR
 * This signal is an extended form of CCP_DEEP_SLEEP_WAKE_PIO_REQ.
 * It includes the same "source" field as CCP_DEEP_SLEEP_WAKE_REQ,
 * but also indicates a mask of PIOs (0 to however many PIOs)
 * that  have caused the chip to wake from deep sleep.  The
 * signal is only sent on chips that have hardware to detect PIO
 * events separately in deep sleep.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 * Identical to corresponding field in CCP_DEEP_SLEEP_WAKE_REQ.
 *
 *   * Non-parameter argument \c data:
 * n lots of uint16 mask of PIOs that caused wakeup.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_DEEP_SLEEP_WAKE_PIO_EXT_REQ_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_deep_sleep_wake_pio_ext_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_uint32__VARLEN(CCP_SIGNAL_ID_DEEP_SLEEP_WAKE_PIO_EXT_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU deep_sleep_wake_pio_ext_req.
 *
 * DIR FROM_CURATOR
 * This signal is an extended form of CCP_DEEP_SLEEP_WAKE_PIO_REQ.
 * It includes the same "source" field as CCP_DEEP_SLEEP_WAKE_REQ,
 * but also indicates a mask of PIOs (0 to however many PIOs)
 * that  have caused the chip to wake from deep sleep.  The
 * signal is only sent on chips that have hardware to detect PIO
 * events separately in deep sleep.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c source
 * Bits indicating wake sources that triggered.
 * Identical to corresponding field in CCP_DEEP_SLEEP_WAKE_REQ.
 *
 *   Non-parameter argument \c data:
 * n lots of uint16 mask of PIOs that caused wakeup.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_DEEP_SLEEP_WAKE_PIO_EXT_REQ_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_deep_sleep_wake_pio_ext_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint32__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU reset_req.
 *
 * DIR TO_CURATOR
 * Sent to curator to request a reset. Two types of reset are supported:
 * 1) A whole chip reset.
 * 2) A reset of all active subsystems excluding the curator.
 * In the case of a whole chip reset the DFU image bank to use for the
 * reboot is specified.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c full_reset
 * Issue a whole chip reset or subsystem only reset:
 * CCP_FULL_CHIP_RESET - reset the entire chip (using the watchdog)
 * CCP_SUBSYS_ONLY_RESET - reset only subsystems (excluding curator)
 * Note the field dfu_image_info below is ignored for SUBSYS_ONLY resets.
 *
 *   * \c dfu_info
 * If DFU is supported, there are two images banks. This field tells
 * the curator which image bank should be used after the reset, and also
 * other information that is preserved by curator over the reset and may
 * be used by Apps in the DFU process.
 * This field is only valid if the full_reset field is
 * CCP_FULL_CHIP_RESET. Otherwise, it is ignored.
 *
 *   * \c reset_code
 * A 16 bit number that is reported back after the reset
 * in the CCP_REBOOT_REASON_TYPE IE.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_RESET_TYPE__CCP_DFU_INFO__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_RESET_TYPE full_reset,
        const CCP_DFU_INFO *dfu_info,
        uint16 reset_code);

/**
 * Send CCP PDU reset_req.
 *
 * DIR TO_CURATOR
 * Sent to curator to request a reset. Two types of reset are supported:
 * 1) A whole chip reset.
 * 2) A reset of all active subsystems excluding the curator.
 * In the case of a whole chip reset the DFU image bank to use for the
 * reboot is specified.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c full_reset
 * Issue a whole chip reset or subsystem only reset:
 * CCP_FULL_CHIP_RESET - reset the entire chip (using the watchdog)
 * CCP_SUBSYS_ONLY_RESET - reset only subsystems (excluding curator)
 * Note the field dfu_image_info below is ignored for SUBSYS_ONLY resets.
 *
 *   * \c dfu_info
 * If DFU is supported, there are two images banks. This field tells
 * the curator which image bank should be used after the reset, and also
 * other information that is preserved by curator over the reset and may
 * be used by Apps in the DFU process.
 * This field is only valid if the full_reset field is
 * CCP_FULL_CHIP_RESET. Otherwise, it is ignored.
 *
 *   * \c reset_code
 * A 16 bit number that is reported back after the reset
 * in the CCP_REBOOT_REASON_TYPE IE.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_reset_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_RESET_TYPE__CCP_DFU_INFO__uint16(CCP_SIGNAL_ID_RESET_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU reset_req.
 *
 * DIR TO_CURATOR
 * Sent to curator to request a reset. Two types of reset are supported:
 * 1) A whole chip reset.
 * 2) A reset of all active subsystems excluding the curator.
 * In the case of a whole chip reset the DFU image bank to use for the
 * reboot is specified.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c full_reset
 * Issue a whole chip reset or subsystem only reset:
 * CCP_FULL_CHIP_RESET - reset the entire chip (using the watchdog)
 * CCP_SUBSYS_ONLY_RESET - reset only subsystems (excluding curator)
 * Note the field dfu_image_info below is ignored for SUBSYS_ONLY resets.
 *
 *   * \c dfu_info
 * If DFU is supported, there are two images banks. This field tells
 * the curator which image bank should be used after the reset, and also
 * other information that is preserved by curator over the reset and may
 * be used by Apps in the DFU process.
 * This field is only valid if the full_reset field is
 * CCP_FULL_CHIP_RESET. Otherwise, it is ignored.
 *
 *   * \c reset_code
 * A 16 bit number that is reported back after the reset
 * in the CCP_REBOOT_REASON_TYPE IE.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_RESET_TYPE__CCP_DFU_INFO__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_RESET_TYPE *full_reset,
        CCP_DFU_INFO *dfu_info,
        uint16 *reset_code);

/**
 * Analyse received CCP PDU reset_req.
 *
 * DIR TO_CURATOR
 * Sent to curator to request a reset. Two types of reset are supported:
 * 1) A whole chip reset.
 * 2) A reset of all active subsystems excluding the curator.
 * In the case of a whole chip reset the DFU image bank to use for the
 * reboot is specified.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c full_reset
 * Issue a whole chip reset or subsystem only reset:
 * CCP_FULL_CHIP_RESET - reset the entire chip (using the watchdog)
 * CCP_SUBSYS_ONLY_RESET - reset only subsystems (excluding curator)
 * Note the field dfu_image_info below is ignored for SUBSYS_ONLY resets.
 *
 *   * \c dfu_info
 * If DFU is supported, there are two images banks. This field tells
 * the curator which image bank should be used after the reset, and also
 * other information that is preserved by curator over the reset and may
 * be used by Apps in the DFU process.
 * This field is only valid if the full_reset field is
 * CCP_FULL_CHIP_RESET. Otherwise, it is ignored.
 *
 *   * \c reset_code
 * A 16 bit number that is reported back after the reset
 * in the CCP_REBOOT_REASON_TYPE IE.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_reset_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_CCP_RESET_TYPE__CCP_DFU_INFO__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU charger_configure_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to configure the battery charger.
 * Settings always selected, no user control:
 * 
 * PMU_DA_CHG_IMON_EN - internal current monitoring.
 * Enabled (set to 1) when charging, disabled (set to 0) when not charging.
 * 
 * PMU_CHG_CTRL_OVER_VOLT_ENB - battery over voltage protection disable.
 * Disabled (set to 1) briefly to clear overvolt condition on disabling
 * the charger, enabled (set to 0) otherwise.
 * 
 * Notes on External mode min/max currents for Pre and Fast charge:
 * 
 * Max current equates to 100mV across the external resistor, min current 25mV.
 * Eg. 100mOhms gives External max 1000mA, min 250mA
 * 56mOhms gives External max 1785mA, min 446mA (approx)
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pre_external_milliohms
 * External resistor value in milliohms,
 * or 0 to Pre charge in Internal mode.
 *
 *   * \c fast_external_milliohms
 * External resistor value in milliohms,
 * or 0 to Fast charge in Internal mode.
 *
 *   * \c trickle_milliamps
 * Trickle charge max current, max 50mA.
 *
 *   * \c pre_milliamps
 * Pre charge max current, Internal max 200mA,
 * External min/max varies with resistor.
 *
 *   * \c fast_milliamps
 * Fast charge max current, Internal max 200mA,
 * External min/max varies with resistor.
 *
 *   * \c pre_fast_threshold
 * Threshold to switch from Pre to Fast charge,
 * 2 bits PMU_CHG_VFAST_CTRL values:
 * 0 2V9, 1 3V0, 2 3V1 (2V8 hysteresis to Pre),
 * 3 2V5 (2V4 hysteresis back to Pre)
 *
 *   * \c float_constant_voltage
 * Fast charge float constant voltage,
 * 4 bits PMU_CHG_VFLOAT_CTRL values:
 * 0 3V65, 1 3V70, .. 14 4V35, 15 4V40
 *
 *   * \c termination_current
 * Charge termination at % of Fast current,
 * 2 bits PMU_CHG_ITERM_CTRL values:
 * 0 10%, 1 20%, 2 30%, 3 40%
 *
 *   * \c termination_debounce
 * Charge termination debounce timer,
 * 2 bits PMU_CHG_DB_CTRL values:
 * 0 Off, 1 4ms, 2 32ms, 3 128ms
 *
 *   * \c standby_fast_hysteresis
 * Standby to Fast charge hysteresis,
 * 2 bits PMU_CHG_HYST_CTRL values:
 * 0 100mV, 1 150mV, 2 200mV, 3 250mV
 *
 *   * \c configure_type
 * Type of configuration being performed:
 * CCP_CHARGER_CONFIGURE_FULL to fully configure the charger.
 * CCP_CHARGER_CONFIGURE_CURRENTS to configure charge currents only.
 *
 *   * \c unused_expansion2
 * Unused space for expansion/patching,
 * set this to zero if not used.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__CCP_CHARGER_CONFIGURE_TYPE__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 pre_external_milliohms,
        uint16 fast_external_milliohms,
        uint16 trickle_milliamps,
        uint16 pre_milliamps,
        uint16 fast_milliamps,
        uint16 pre_fast_threshold,
        uint16 float_constant_voltage,
        uint16 termination_current,
        uint16 termination_debounce,
        uint16 standby_fast_hysteresis,
        CCP_CHARGER_CONFIGURE_TYPE configure_type,
        uint16 unused_expansion2);

/**
 * Send CCP PDU charger_configure_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to configure the battery charger.
 * Settings always selected, no user control:
 * 
 * PMU_DA_CHG_IMON_EN - internal current monitoring.
 * Enabled (set to 1) when charging, disabled (set to 0) when not charging.
 * 
 * PMU_CHG_CTRL_OVER_VOLT_ENB - battery over voltage protection disable.
 * Disabled (set to 1) briefly to clear overvolt condition on disabling
 * the charger, enabled (set to 0) otherwise.
 * 
 * Notes on External mode min/max currents for Pre and Fast charge:
 * 
 * Max current equates to 100mV across the external resistor, min current 25mV.
 * Eg. 100mOhms gives External max 1000mA, min 250mA
 * 56mOhms gives External max 1785mA, min 446mA (approx)
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pre_external_milliohms
 * External resistor value in milliohms,
 * or 0 to Pre charge in Internal mode.
 *
 *   * \c fast_external_milliohms
 * External resistor value in milliohms,
 * or 0 to Fast charge in Internal mode.
 *
 *   * \c trickle_milliamps
 * Trickle charge max current, max 50mA.
 *
 *   * \c pre_milliamps
 * Pre charge max current, Internal max 200mA,
 * External min/max varies with resistor.
 *
 *   * \c fast_milliamps
 * Fast charge max current, Internal max 200mA,
 * External min/max varies with resistor.
 *
 *   * \c pre_fast_threshold
 * Threshold to switch from Pre to Fast charge,
 * 2 bits PMU_CHG_VFAST_CTRL values:
 * 0 2V9, 1 3V0, 2 3V1 (2V8 hysteresis to Pre),
 * 3 2V5 (2V4 hysteresis back to Pre)
 *
 *   * \c float_constant_voltage
 * Fast charge float constant voltage,
 * 4 bits PMU_CHG_VFLOAT_CTRL values:
 * 0 3V65, 1 3V70, .. 14 4V35, 15 4V40
 *
 *   * \c termination_current
 * Charge termination at % of Fast current,
 * 2 bits PMU_CHG_ITERM_CTRL values:
 * 0 10%, 1 20%, 2 30%, 3 40%
 *
 *   * \c termination_debounce
 * Charge termination debounce timer,
 * 2 bits PMU_CHG_DB_CTRL values:
 * 0 Off, 1 4ms, 2 32ms, 3 128ms
 *
 *   * \c standby_fast_hysteresis
 * Standby to Fast charge hysteresis,
 * 2 bits PMU_CHG_HYST_CTRL values:
 * 0 100mV, 1 150mV, 2 200mV, 3 250mV
 *
 *   * \c configure_type
 * Type of configuration being performed:
 * CCP_CHARGER_CONFIGURE_FULL to fully configure the charger.
 * CCP_CHARGER_CONFIGURE_CURRENTS to configure charge currents only.
 *
 *   * \c unused_expansion2
 * Unused space for expansion/patching,
 * set this to zero if not used.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_charger_configure_req(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13) \
    ccp_send_pdu_uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__CCP_CHARGER_CONFIGURE_TYPE__uint16(CCP_SIGNAL_ID_CHARGER_CONFIGURE_REQ, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13)

/**
 * Analyse received CCP PDU charger_configure_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to configure the battery charger.
 * Settings always selected, no user control:
 * 
 * PMU_DA_CHG_IMON_EN - internal current monitoring.
 * Enabled (set to 1) when charging, disabled (set to 0) when not charging.
 * 
 * PMU_CHG_CTRL_OVER_VOLT_ENB - battery over voltage protection disable.
 * Disabled (set to 1) briefly to clear overvolt condition on disabling
 * the charger, enabled (set to 0) otherwise.
 * 
 * Notes on External mode min/max currents for Pre and Fast charge:
 * 
 * Max current equates to 100mV across the external resistor, min current 25mV.
 * Eg. 100mOhms gives External max 1000mA, min 250mA
 * 56mOhms gives External max 1785mA, min 446mA (approx)
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pre_external_milliohms
 * External resistor value in milliohms,
 * or 0 to Pre charge in Internal mode.
 *
 *   * \c fast_external_milliohms
 * External resistor value in milliohms,
 * or 0 to Fast charge in Internal mode.
 *
 *   * \c trickle_milliamps
 * Trickle charge max current, max 50mA.
 *
 *   * \c pre_milliamps
 * Pre charge max current, Internal max 200mA,
 * External min/max varies with resistor.
 *
 *   * \c fast_milliamps
 * Fast charge max current, Internal max 200mA,
 * External min/max varies with resistor.
 *
 *   * \c pre_fast_threshold
 * Threshold to switch from Pre to Fast charge,
 * 2 bits PMU_CHG_VFAST_CTRL values:
 * 0 2V9, 1 3V0, 2 3V1 (2V8 hysteresis to Pre),
 * 3 2V5 (2V4 hysteresis back to Pre)
 *
 *   * \c float_constant_voltage
 * Fast charge float constant voltage,
 * 4 bits PMU_CHG_VFLOAT_CTRL values:
 * 0 3V65, 1 3V70, .. 14 4V35, 15 4V40
 *
 *   * \c termination_current
 * Charge termination at % of Fast current,
 * 2 bits PMU_CHG_ITERM_CTRL values:
 * 0 10%, 1 20%, 2 30%, 3 40%
 *
 *   * \c termination_debounce
 * Charge termination debounce timer,
 * 2 bits PMU_CHG_DB_CTRL values:
 * 0 Off, 1 4ms, 2 32ms, 3 128ms
 *
 *   * \c standby_fast_hysteresis
 * Standby to Fast charge hysteresis,
 * 2 bits PMU_CHG_HYST_CTRL values:
 * 0 100mV, 1 150mV, 2 200mV, 3 250mV
 *
 *   * \c configure_type
 * Type of configuration being performed:
 * CCP_CHARGER_CONFIGURE_FULL to fully configure the charger.
 * CCP_CHARGER_CONFIGURE_CURRENTS to configure charge currents only.
 *
 *   * \c unused_expansion2
 * Unused space for expansion/patching,
 * set this to zero if not used.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__CCP_CHARGER_CONFIGURE_TYPE__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *pre_external_milliohms,
        uint16 *fast_external_milliohms,
        uint16 *trickle_milliamps,
        uint16 *pre_milliamps,
        uint16 *fast_milliamps,
        uint16 *pre_fast_threshold,
        uint16 *float_constant_voltage,
        uint16 *termination_current,
        uint16 *termination_debounce,
        uint16 *standby_fast_hysteresis,
        CCP_CHARGER_CONFIGURE_TYPE *configure_type,
        uint16 *unused_expansion2);

/**
 * Analyse received CCP PDU charger_configure_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to configure the battery charger.
 * Settings always selected, no user control:
 * 
 * PMU_DA_CHG_IMON_EN - internal current monitoring.
 * Enabled (set to 1) when charging, disabled (set to 0) when not charging.
 * 
 * PMU_CHG_CTRL_OVER_VOLT_ENB - battery over voltage protection disable.
 * Disabled (set to 1) briefly to clear overvolt condition on disabling
 * the charger, enabled (set to 0) otherwise.
 * 
 * Notes on External mode min/max currents for Pre and Fast charge:
 * 
 * Max current equates to 100mV across the external resistor, min current 25mV.
 * Eg. 100mOhms gives External max 1000mA, min 250mA
 * 56mOhms gives External max 1785mA, min 446mA (approx)
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pre_external_milliohms
 * External resistor value in milliohms,
 * or 0 to Pre charge in Internal mode.
 *
 *   * \c fast_external_milliohms
 * External resistor value in milliohms,
 * or 0 to Fast charge in Internal mode.
 *
 *   * \c trickle_milliamps
 * Trickle charge max current, max 50mA.
 *
 *   * \c pre_milliamps
 * Pre charge max current, Internal max 200mA,
 * External min/max varies with resistor.
 *
 *   * \c fast_milliamps
 * Fast charge max current, Internal max 200mA,
 * External min/max varies with resistor.
 *
 *   * \c pre_fast_threshold
 * Threshold to switch from Pre to Fast charge,
 * 2 bits PMU_CHG_VFAST_CTRL values:
 * 0 2V9, 1 3V0, 2 3V1 (2V8 hysteresis to Pre),
 * 3 2V5 (2V4 hysteresis back to Pre)
 *
 *   * \c float_constant_voltage
 * Fast charge float constant voltage,
 * 4 bits PMU_CHG_VFLOAT_CTRL values:
 * 0 3V65, 1 3V70, .. 14 4V35, 15 4V40
 *
 *   * \c termination_current
 * Charge termination at % of Fast current,
 * 2 bits PMU_CHG_ITERM_CTRL values:
 * 0 10%, 1 20%, 2 30%, 3 40%
 *
 *   * \c termination_debounce
 * Charge termination debounce timer,
 * 2 bits PMU_CHG_DB_CTRL values:
 * 0 Off, 1 4ms, 2 32ms, 3 128ms
 *
 *   * \c standby_fast_hysteresis
 * Standby to Fast charge hysteresis,
 * 2 bits PMU_CHG_HYST_CTRL values:
 * 0 100mV, 1 150mV, 2 200mV, 3 250mV
 *
 *   * \c configure_type
 * Type of configuration being performed:
 * CCP_CHARGER_CONFIGURE_FULL to fully configure the charger.
 * CCP_CHARGER_CONFIGURE_CURRENTS to configure charge currents only.
 *
 *   * \c unused_expansion2
 * Unused space for expansion/patching,
 * set this to zero if not used.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_charger_configure_req(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14) \
    ccp_recv_pdu_uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__uint16__CCP_CHARGER_CONFIGURE_TYPE__uint16(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14)


/**
 * Send CCP PDU charger_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by curator to say whether the request to configure the battery
 * charger was successful or not.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c result
 * Indication of success or failure reason.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_CHARGER_CONFIGURE_RESULT(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_CHARGER_CONFIGURE_RESULT result);

/**
 * Send CCP PDU charger_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by curator to say whether the request to configure the battery
 * charger was successful or not.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c result
 * Indication of success or failure reason.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_charger_configure_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_CHARGER_CONFIGURE_RESULT(CCP_SIGNAL_ID_CHARGER_CONFIGURE_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU charger_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by curator to say whether the request to configure the battery
 * charger was successful or not.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c result
 * Indication of success or failure reason.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_CHARGER_CONFIGURE_RESULT(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_CHARGER_CONFIGURE_RESULT *result);

/**
 * Analyse received CCP PDU charger_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by curator to say whether the request to configure the battery
 * charger was successful or not.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c result
 * Indication of success or failure reason.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_charger_configure_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_CHARGER_CONFIGURE_RESULT(arg1, arg2, arg3)


/**
 * Send CCP PDU charger_enable_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to enable/disable the battery charger.
 * On disable also clear any PMU_CHG_STATUS_CHARGER_VBAT_OVERVOLT_ERROR
 * condition.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c enable
 * Whether to enable or disable the charger:
 * CCP_CHARGER_ENABLE_DISABLE to disable the charger.
 * CCP_CHARGER_ENABLE_ENABLE to (re)enable the changer.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_CHARGER_ENABLE_TYPE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_CHARGER_ENABLE_TYPE enable);

/**
 * Send CCP PDU charger_enable_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to enable/disable the battery charger.
 * On disable also clear any PMU_CHG_STATUS_CHARGER_VBAT_OVERVOLT_ERROR
 * condition.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c enable
 * Whether to enable or disable the charger:
 * CCP_CHARGER_ENABLE_DISABLE to disable the charger.
 * CCP_CHARGER_ENABLE_ENABLE to (re)enable the changer.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_charger_enable_req(arg1, arg2) \
    ccp_send_pdu_CCP_CHARGER_ENABLE_TYPE(CCP_SIGNAL_ID_CHARGER_ENABLE_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU charger_enable_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to enable/disable the battery charger.
 * On disable also clear any PMU_CHG_STATUS_CHARGER_VBAT_OVERVOLT_ERROR
 * condition.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c enable
 * Whether to enable or disable the charger:
 * CCP_CHARGER_ENABLE_DISABLE to disable the charger.
 * CCP_CHARGER_ENABLE_ENABLE to (re)enable the changer.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_CHARGER_ENABLE_TYPE(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_CHARGER_ENABLE_TYPE *enable);

/**
 * Analyse received CCP PDU charger_enable_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to enable/disable the battery charger.
 * On disable also clear any PMU_CHG_STATUS_CHARGER_VBAT_OVERVOLT_ERROR
 * condition.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c enable
 * Whether to enable or disable the charger:
 * CCP_CHARGER_ENABLE_DISABLE to disable the charger.
 * CCP_CHARGER_ENABLE_ENABLE to (re)enable the changer.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_charger_enable_req(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_CHARGER_ENABLE_TYPE(arg1, arg2, arg3)


/**
 * Send CCP PDU charger_enable_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by curator to say whether the request to enable/disable the
 * battery charger was successful or not.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c result
 * Indication of success or failure reason.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_CHARGER_ENABLE_RESULT(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_CHARGER_ENABLE_RESULT result);

/**
 * Send CCP PDU charger_enable_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by curator to say whether the request to enable/disable the
 * battery charger was successful or not.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c result
 * Indication of success or failure reason.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_charger_enable_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_CHARGER_ENABLE_RESULT(CCP_SIGNAL_ID_CHARGER_ENABLE_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU charger_enable_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by curator to say whether the request to enable/disable the
 * battery charger was successful or not.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c result
 * Indication of success or failure reason.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_CHARGER_ENABLE_RESULT(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_CHARGER_ENABLE_RESULT *result);

/**
 * Analyse received CCP PDU charger_enable_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by curator to say whether the request to enable/disable the
 * battery charger was successful or not.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c result
 * Indication of success or failure reason.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_charger_enable_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_CHARGER_ENABLE_RESULT(arg1, arg2, arg3)


/**
 * Send CCP PDU charger_status_ind.
 *
 * DIR FROM_CURATOR
 * Sent by curator whenever it gets a CURATOR_PMU_INT_BATTERY_CHARGER
 * interrupt.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pmu_chg_status
 * Contents of the PMU_CHG_STATUS register after the interrupt and
 * settling delay.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_charger_status_ind(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_CHARGER_STATUS_IND, arg1, arg2)

/**
 * Analyse received CCP PDU charger_status_ind.
 *
 * DIR FROM_CURATOR
 * Sent by curator whenever it gets a CURATOR_PMU_INT_BATTERY_CHARGER
 * interrupt.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pmu_chg_status
 * Contents of the PMU_CHG_STATUS register after the interrupt and
 * settling delay.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_charger_status_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU dormant_req.
 *
 * DIR TO_CURATOR
 * Command from Apps to switch the chip into dormant
 * mode.
 * Also supply the details of what can wake the chip.
 * Sent from Apps subsys to Curator.
 * 
 * DEPRECATED: This PDU is being kept for compatibility with curator
 * Crescendo A04 ROM. All code talking to a newer curator should use
 * the new CCP_DORMANT_ENTER_REQ message.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c led_wake_mask
 * The mask of LEDs that should wake the chip.
 * Bit 0 corresponds to LED0
 *
 *   * \c led_wake_invert_mask
 * The mask of LEDs for which the wake input is inverted.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_wake_mask.
 *
 *   * \c led_output_drive_mask
 * The mask of LEDs for which the output needs to be set during dormant.
 * Bit 0 corresponds to LED0
 *
 *   * \c led_output_drive
 * The driven output on the LED pads.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_output_drive_mask.
 * Note: An LED pin cannot be a wakeup source and a output driver at
 * the same time.
 * Also an LED pin can only be driven low. A request to drive it high
 * will result in the LED being configured in HighZ state.
 *
 *   * \c led_output_invert_mask
 * The mask of LEDs for which the driven output is inverted.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_output_drive_mask.
 *
 *   * \c led_pull_down_en_mask
 * The mask of LEDs to which a pull down needs to be applied.
 * Bit 0 corresponds to LED0
 * Only meaningful if the corresponding bit is also set in
 * led_output_drive_mask or led_wake_mask.
 *
 *   * \c xio_wake_mask
 * The mask of XIOs that should wake the chip.
 * Bit 0 corresponds to XIO0
 *
 *   * \c nfc_wake_enable_mask
 * Enable dormant wake on NFC field detect and/or NFC overload.
 *
 *   * \c deadline_valid
 * If set then the deadline time will be used to wake-up
 * the chip from dormant. Otherwise the deadline will be ignored.
 *
 *   * \c deadline
 * The deadline by which the chip must be woken up from dormant.
 * Only valid when deadline_valid is set. Otherwise, it will be ignored.
 * Note: The wakeup timer in dormant runs from a 8KHz slow clock.
 * Therefore, the time duration to stay in dormant is obtained by converting
 * the deadline time (which is based on the system clock) to the slow clock
 * ticks. If the slow clock is un-calibrated then the dormant duration
 * can be off by +/- 50%.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16__uint16__uint16__uint16__uint16__uint16__CCP_NFC_WAKE_ENABLE_MASK_TYPE__CCP_DORMANT_REQ_DEADLINE_TYPE__uint32(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 led_wake_mask,
        uint16 led_wake_invert_mask,
        uint16 led_output_drive_mask,
        uint16 led_output_drive,
        uint16 led_output_invert_mask,
        uint16 led_pull_down_en_mask,
        uint16 xio_wake_mask,
        CCP_NFC_WAKE_ENABLE_MASK_TYPE nfc_wake_enable_mask,
        CCP_DORMANT_REQ_DEADLINE_TYPE deadline_valid,
        uint32 deadline);

/**
 * Send CCP PDU dormant_req.
 *
 * DIR TO_CURATOR
 * Command from Apps to switch the chip into dormant
 * mode.
 * Also supply the details of what can wake the chip.
 * Sent from Apps subsys to Curator.
 * 
 * DEPRECATED: This PDU is being kept for compatibility with curator
 * Crescendo A04 ROM. All code talking to a newer curator should use
 * the new CCP_DORMANT_ENTER_REQ message.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c led_wake_mask
 * The mask of LEDs that should wake the chip.
 * Bit 0 corresponds to LED0
 *
 *   * \c led_wake_invert_mask
 * The mask of LEDs for which the wake input is inverted.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_wake_mask.
 *
 *   * \c led_output_drive_mask
 * The mask of LEDs for which the output needs to be set during dormant.
 * Bit 0 corresponds to LED0
 *
 *   * \c led_output_drive
 * The driven output on the LED pads.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_output_drive_mask.
 * Note: An LED pin cannot be a wakeup source and a output driver at
 * the same time.
 * Also an LED pin can only be driven low. A request to drive it high
 * will result in the LED being configured in HighZ state.
 *
 *   * \c led_output_invert_mask
 * The mask of LEDs for which the driven output is inverted.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_output_drive_mask.
 *
 *   * \c led_pull_down_en_mask
 * The mask of LEDs to which a pull down needs to be applied.
 * Bit 0 corresponds to LED0
 * Only meaningful if the corresponding bit is also set in
 * led_output_drive_mask or led_wake_mask.
 *
 *   * \c xio_wake_mask
 * The mask of XIOs that should wake the chip.
 * Bit 0 corresponds to XIO0
 *
 *   * \c nfc_wake_enable_mask
 * Enable dormant wake on NFC field detect and/or NFC overload.
 *
 *   * \c deadline_valid
 * If set then the deadline time will be used to wake-up
 * the chip from dormant. Otherwise the deadline will be ignored.
 *
 *   * \c deadline
 * The deadline by which the chip must be woken up from dormant.
 * Only valid when deadline_valid is set. Otherwise, it will be ignored.
 * Note: The wakeup timer in dormant runs from a 8KHz slow clock.
 * Therefore, the time duration to stay in dormant is obtained by converting
 * the deadline time (which is based on the system clock) to the slow clock
 * ticks. If the slow clock is un-calibrated then the dormant duration
 * can be off by +/- 50%.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_dormant_req(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) \
    ccp_send_pdu_uint16__uint16__uint16__uint16__uint16__uint16__uint16__CCP_NFC_WAKE_ENABLE_MASK_TYPE__CCP_DORMANT_REQ_DEADLINE_TYPE__uint32(CCP_SIGNAL_ID_DORMANT_REQ, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11)

/**
 * Analyse received CCP PDU dormant_req.
 *
 * DIR TO_CURATOR
 * Command from Apps to switch the chip into dormant
 * mode.
 * Also supply the details of what can wake the chip.
 * Sent from Apps subsys to Curator.
 * 
 * DEPRECATED: This PDU is being kept for compatibility with curator
 * Crescendo A04 ROM. All code talking to a newer curator should use
 * the new CCP_DORMANT_ENTER_REQ message.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c led_wake_mask
 * The mask of LEDs that should wake the chip.
 * Bit 0 corresponds to LED0
 *
 *   * \c led_wake_invert_mask
 * The mask of LEDs for which the wake input is inverted.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_wake_mask.
 *
 *   * \c led_output_drive_mask
 * The mask of LEDs for which the output needs to be set during dormant.
 * Bit 0 corresponds to LED0
 *
 *   * \c led_output_drive
 * The driven output on the LED pads.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_output_drive_mask.
 * Note: An LED pin cannot be a wakeup source and a output driver at
 * the same time.
 * Also an LED pin can only be driven low. A request to drive it high
 * will result in the LED being configured in HighZ state.
 *
 *   * \c led_output_invert_mask
 * The mask of LEDs for which the driven output is inverted.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_output_drive_mask.
 *
 *   * \c led_pull_down_en_mask
 * The mask of LEDs to which a pull down needs to be applied.
 * Bit 0 corresponds to LED0
 * Only meaningful if the corresponding bit is also set in
 * led_output_drive_mask or led_wake_mask.
 *
 *   * \c xio_wake_mask
 * The mask of XIOs that should wake the chip.
 * Bit 0 corresponds to XIO0
 *
 *   * \c nfc_wake_enable_mask
 * Enable dormant wake on NFC field detect and/or NFC overload.
 *
 *   * \c deadline_valid
 * If set then the deadline time will be used to wake-up
 * the chip from dormant. Otherwise the deadline will be ignored.
 *
 *   * \c deadline
 * The deadline by which the chip must be woken up from dormant.
 * Only valid when deadline_valid is set. Otherwise, it will be ignored.
 * Note: The wakeup timer in dormant runs from a 8KHz slow clock.
 * Therefore, the time duration to stay in dormant is obtained by converting
 * the deadline time (which is based on the system clock) to the slow clock
 * ticks. If the slow clock is un-calibrated then the dormant duration
 * can be off by +/- 50%.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16__uint16__uint16__uint16__uint16__uint16__CCP_NFC_WAKE_ENABLE_MASK_TYPE__CCP_DORMANT_REQ_DEADLINE_TYPE__uint32(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *led_wake_mask,
        uint16 *led_wake_invert_mask,
        uint16 *led_output_drive_mask,
        uint16 *led_output_drive,
        uint16 *led_output_invert_mask,
        uint16 *led_pull_down_en_mask,
        uint16 *xio_wake_mask,
        CCP_NFC_WAKE_ENABLE_MASK_TYPE *nfc_wake_enable_mask,
        CCP_DORMANT_REQ_DEADLINE_TYPE *deadline_valid,
        uint32 *deadline);

/**
 * Analyse received CCP PDU dormant_req.
 *
 * DIR TO_CURATOR
 * Command from Apps to switch the chip into dormant
 * mode.
 * Also supply the details of what can wake the chip.
 * Sent from Apps subsys to Curator.
 * 
 * DEPRECATED: This PDU is being kept for compatibility with curator
 * Crescendo A04 ROM. All code talking to a newer curator should use
 * the new CCP_DORMANT_ENTER_REQ message.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c led_wake_mask
 * The mask of LEDs that should wake the chip.
 * Bit 0 corresponds to LED0
 *
 *   * \c led_wake_invert_mask
 * The mask of LEDs for which the wake input is inverted.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_wake_mask.
 *
 *   * \c led_output_drive_mask
 * The mask of LEDs for which the output needs to be set during dormant.
 * Bit 0 corresponds to LED0
 *
 *   * \c led_output_drive
 * The driven output on the LED pads.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_output_drive_mask.
 * Note: An LED pin cannot be a wakeup source and a output driver at
 * the same time.
 * Also an LED pin can only be driven low. A request to drive it high
 * will result in the LED being configured in HighZ state.
 *
 *   * \c led_output_invert_mask
 * The mask of LEDs for which the driven output is inverted.
 * Bit 0 corresponds to LED0
 * Only meaningful if the bit is also set in led_output_drive_mask.
 *
 *   * \c led_pull_down_en_mask
 * The mask of LEDs to which a pull down needs to be applied.
 * Bit 0 corresponds to LED0
 * Only meaningful if the corresponding bit is also set in
 * led_output_drive_mask or led_wake_mask.
 *
 *   * \c xio_wake_mask
 * The mask of XIOs that should wake the chip.
 * Bit 0 corresponds to XIO0
 *
 *   * \c nfc_wake_enable_mask
 * Enable dormant wake on NFC field detect and/or NFC overload.
 *
 *   * \c deadline_valid
 * If set then the deadline time will be used to wake-up
 * the chip from dormant. Otherwise the deadline will be ignored.
 *
 *   * \c deadline
 * The deadline by which the chip must be woken up from dormant.
 * Only valid when deadline_valid is set. Otherwise, it will be ignored.
 * Note: The wakeup timer in dormant runs from a 8KHz slow clock.
 * Therefore, the time duration to stay in dormant is obtained by converting
 * the deadline time (which is based on the system clock) to the slow clock
 * ticks. If the slow clock is un-calibrated then the dormant duration
 * can be off by +/- 50%.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_dormant_req(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12) \
    ccp_recv_pdu_uint16__uint16__uint16__uint16__uint16__uint16__uint16__CCP_NFC_WAKE_ENABLE_MASK_TYPE__CCP_DORMANT_REQ_DEADLINE_TYPE__uint32(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12)


/**
 * Send CCP PDU dormant_rsp.
 *
 * DIR FROM_CURATOR
 * This gets sent to Apps only when Curator fails to
 * put the chip into dormant.
 * 
 * Note: This message is returned on failure to enter dormant for either
 * CCP_DORMANT_REQ or CCP_DORMANT_ENTER_REQ.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c reason_code
 * The reason why Curator failed to put the chip into dormant.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_DORMANT_REASON_CODE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_DORMANT_REASON_CODE reason_code);

/**
 * Send CCP PDU dormant_rsp.
 *
 * DIR FROM_CURATOR
 * This gets sent to Apps only when Curator fails to
 * put the chip into dormant.
 * 
 * Note: This message is returned on failure to enter dormant for either
 * CCP_DORMANT_REQ or CCP_DORMANT_ENTER_REQ.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c reason_code
 * The reason why Curator failed to put the chip into dormant.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_dormant_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_DORMANT_REASON_CODE(CCP_SIGNAL_ID_DORMANT_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU dormant_rsp.
 *
 * DIR FROM_CURATOR
 * This gets sent to Apps only when Curator fails to
 * put the chip into dormant.
 * 
 * Note: This message is returned on failure to enter dormant for either
 * CCP_DORMANT_REQ or CCP_DORMANT_ENTER_REQ.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c reason_code
 * The reason why Curator failed to put the chip into dormant.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_DORMANT_REASON_CODE(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_DORMANT_REASON_CODE *reason_code);

/**
 * Analyse received CCP PDU dormant_rsp.
 *
 * DIR FROM_CURATOR
 * This gets sent to Apps only when Curator fails to
 * put the chip into dormant.
 * 
 * Note: This message is returned on failure to enter dormant for either
 * CCP_DORMANT_REQ or CCP_DORMANT_ENTER_REQ.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c reason_code
 * The reason why Curator failed to put the chip into dormant.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_dormant_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_DORMANT_REASON_CODE(arg1, arg2, arg3)


/**
 * Send CCP PDU chip_off_req.
 *
 * DIR TO_CURATOR
 * Command from Apps to switch the chip into OFF state
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_chip_off_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_CHIP_OFF_REQ, arg1)

/**
 * Analyse received CCP PDU chip_off_req.
 *
 * DIR TO_CURATOR
 * Command from Apps to switch the chip into OFF state
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_chip_off_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU chip_off_rsp.
 *
 * DIR FROM_CURATOR
 * This gets sent to Apps only when Curator fails to
 * put the chip into OFF state.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c reason_code
 * The reason why Curator failed to put the chip into OFF.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_CHIP_OFF_REASON_CODE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_CHIP_OFF_REASON_CODE reason_code);

/**
 * Send CCP PDU chip_off_rsp.
 *
 * DIR FROM_CURATOR
 * This gets sent to Apps only when Curator fails to
 * put the chip into OFF state.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c reason_code
 * The reason why Curator failed to put the chip into OFF.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_chip_off_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_CHIP_OFF_REASON_CODE(CCP_SIGNAL_ID_CHIP_OFF_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU chip_off_rsp.
 *
 * DIR FROM_CURATOR
 * This gets sent to Apps only when Curator fails to
 * put the chip into OFF state.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c reason_code
 * The reason why Curator failed to put the chip into OFF.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_CHIP_OFF_REASON_CODE(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_CHIP_OFF_REASON_CODE *reason_code);

/**
 * Analyse received CCP PDU chip_off_rsp.
 *
 * DIR FROM_CURATOR
 * This gets sent to Apps only when Curator fails to
 * put the chip into OFF state.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c reason_code
 * The reason why Curator failed to put the chip into OFF.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_chip_off_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_CHIP_OFF_REASON_CODE(arg1, arg2, arg3)


/**
 * Send CCP PDU pmu_mux_to_gmux_req.
 *
 * DIR TO_CURATOR
 * Request to set connection between PMU_MUX, LEDs and GMUX.
 * The message allows
 * 1: Any PMU_MUX output to be routed to GMUX (PMU_MUX = #, LED = NOT_USED)
 * 2: Any LED to be routed to GMUX (PMU_MUX = Off, LED = #)
 * 3: PMU_MUX connected to LED and GMUX at the same time (PMU_MUX = #, LED = *) this allows
 * a: 'scope to monitor analogue lines)
 * b: PMU_MUX to provide current source for single pin thermistor measurement
 * 
 * Multiple connections can be made to the MUX at the same time. ADC measurments
 * will be done on the signal that was connected last.
 * 
 * The requestor of the MUX configuration must release it when finished or before
 * measuring a different input.
 * 
 * The MUX and LED settings can be released by sending a CCP_MUX_TO_GMUX_REQ
 * with OFF in both PMU_MUX and LED, (PMU_MUX = OFF, LED = NOT_USED). The PMU_MUX
 * connection to GMUX will be disabled and the pull resistor on the PMU_MUX side of
 * the interconencting line will be enabled.
 * 
 * The response is the message CCP_PMU_MUX_TO_GMUX_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pmu_mux_cfg
 * Analogue input to select from PMU
 *
 *   * \c pmu_mux_led_cfg
 * Selected LED for input / output note this forces a specific PMU MuxBus
 * to use for pmu_mux_cfg
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_PMU_MUX_CFG__CCP_PMU_MUX_LED_CFG(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_PMU_MUX_CFG pmu_mux_cfg,
        CCP_PMU_MUX_LED_CFG pmu_mux_led_cfg);

/**
 * Send CCP PDU pmu_mux_to_gmux_req.
 *
 * DIR TO_CURATOR
 * Request to set connection between PMU_MUX, LEDs and GMUX.
 * The message allows
 * 1: Any PMU_MUX output to be routed to GMUX (PMU_MUX = #, LED = NOT_USED)
 * 2: Any LED to be routed to GMUX (PMU_MUX = Off, LED = #)
 * 3: PMU_MUX connected to LED and GMUX at the same time (PMU_MUX = #, LED = *) this allows
 * a: 'scope to monitor analogue lines)
 * b: PMU_MUX to provide current source for single pin thermistor measurement
 * 
 * Multiple connections can be made to the MUX at the same time. ADC measurments
 * will be done on the signal that was connected last.
 * 
 * The requestor of the MUX configuration must release it when finished or before
 * measuring a different input.
 * 
 * The MUX and LED settings can be released by sending a CCP_MUX_TO_GMUX_REQ
 * with OFF in both PMU_MUX and LED, (PMU_MUX = OFF, LED = NOT_USED). The PMU_MUX
 * connection to GMUX will be disabled and the pull resistor on the PMU_MUX side of
 * the interconencting line will be enabled.
 * 
 * The response is the message CCP_PMU_MUX_TO_GMUX_RSP.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pmu_mux_cfg
 * Analogue input to select from PMU
 *
 *   * \c pmu_mux_led_cfg
 * Selected LED for input / output note this forces a specific PMU MuxBus
 * to use for pmu_mux_cfg
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_mux_to_gmux_req(arg1, arg2, arg3) \
    ccp_send_pdu_CCP_PMU_MUX_CFG__CCP_PMU_MUX_LED_CFG(CCP_SIGNAL_ID_PMU_MUX_TO_GMUX_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU pmu_mux_to_gmux_req.
 *
 * DIR TO_CURATOR
 * Request to set connection between PMU_MUX, LEDs and GMUX.
 * The message allows
 * 1: Any PMU_MUX output to be routed to GMUX (PMU_MUX = #, LED = NOT_USED)
 * 2: Any LED to be routed to GMUX (PMU_MUX = Off, LED = #)
 * 3: PMU_MUX connected to LED and GMUX at the same time (PMU_MUX = #, LED = *) this allows
 * a: 'scope to monitor analogue lines)
 * b: PMU_MUX to provide current source for single pin thermistor measurement
 * 
 * Multiple connections can be made to the MUX at the same time. ADC measurments
 * will be done on the signal that was connected last.
 * 
 * The requestor of the MUX configuration must release it when finished or before
 * measuring a different input.
 * 
 * The MUX and LED settings can be released by sending a CCP_MUX_TO_GMUX_REQ
 * with OFF in both PMU_MUX and LED, (PMU_MUX = OFF, LED = NOT_USED). The PMU_MUX
 * connection to GMUX will be disabled and the pull resistor on the PMU_MUX side of
 * the interconencting line will be enabled.
 * 
 * The response is the message CCP_PMU_MUX_TO_GMUX_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pmu_mux_cfg
 * Analogue input to select from PMU
 *
 *   * \c pmu_mux_led_cfg
 * Selected LED for input / output note this forces a specific PMU MuxBus
 * to use for pmu_mux_cfg
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_PMU_MUX_CFG__CCP_PMU_MUX_LED_CFG(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_PMU_MUX_CFG *pmu_mux_cfg,
        CCP_PMU_MUX_LED_CFG *pmu_mux_led_cfg);

/**
 * Analyse received CCP PDU pmu_mux_to_gmux_req.
 *
 * DIR TO_CURATOR
 * Request to set connection between PMU_MUX, LEDs and GMUX.
 * The message allows
 * 1: Any PMU_MUX output to be routed to GMUX (PMU_MUX = #, LED = NOT_USED)
 * 2: Any LED to be routed to GMUX (PMU_MUX = Off, LED = #)
 * 3: PMU_MUX connected to LED and GMUX at the same time (PMU_MUX = #, LED = *) this allows
 * a: 'scope to monitor analogue lines)
 * b: PMU_MUX to provide current source for single pin thermistor measurement
 * 
 * Multiple connections can be made to the MUX at the same time. ADC measurments
 * will be done on the signal that was connected last.
 * 
 * The requestor of the MUX configuration must release it when finished or before
 * measuring a different input.
 * 
 * The MUX and LED settings can be released by sending a CCP_MUX_TO_GMUX_REQ
 * with OFF in both PMU_MUX and LED, (PMU_MUX = OFF, LED = NOT_USED). The PMU_MUX
 * connection to GMUX will be disabled and the pull resistor on the PMU_MUX side of
 * the interconencting line will be enabled.
 * 
 * The response is the message CCP_PMU_MUX_TO_GMUX_RSP.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c pmu_mux_cfg
 * Analogue input to select from PMU
 *
 *   * \c pmu_mux_led_cfg
 * Selected LED for input / output note this forces a specific PMU MuxBus
 * to use for pmu_mux_cfg
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_mux_to_gmux_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_PMU_MUX_CFG__CCP_PMU_MUX_LED_CFG(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU pmu_mux_to_gmux_rsp.
 *
 * DIR FROM_CURATOR
 * Response of CCP_PMU_MUX_TO_GMUX_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c status
 * The return status of the request CCP_PMU_MUX_TO_GMUX_REQ.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_PMU_MUX_CONFIG_STATUS(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_PMU_MUX_CONFIG_STATUS status);

/**
 * Send CCP PDU pmu_mux_to_gmux_rsp.
 *
 * DIR FROM_CURATOR
 * Response of CCP_PMU_MUX_TO_GMUX_REQ.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c status
 * The return status of the request CCP_PMU_MUX_TO_GMUX_REQ.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_mux_to_gmux_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_PMU_MUX_CONFIG_STATUS(CCP_SIGNAL_ID_PMU_MUX_TO_GMUX_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU pmu_mux_to_gmux_rsp.
 *
 * DIR FROM_CURATOR
 * Response of CCP_PMU_MUX_TO_GMUX_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c status
 * The return status of the request CCP_PMU_MUX_TO_GMUX_REQ.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_PMU_MUX_CONFIG_STATUS(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_PMU_MUX_CONFIG_STATUS *status);

/**
 * Analyse received CCP PDU pmu_mux_to_gmux_rsp.
 *
 * DIR FROM_CURATOR
 * Response of CCP_PMU_MUX_TO_GMUX_REQ.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c status
 * The return status of the request CCP_PMU_MUX_TO_GMUX_REQ.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_mux_to_gmux_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_PMU_MUX_CONFIG_STATUS(arg1, arg2, arg3)


/**
 * Send CCP PDU pmu_power_source_select_req.
 *
 * DIR TO_CURATOR
 * Select the input power source (battery or charger for
 * the SMPS and Bypass LDO. These need not be the same.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c smps_source
 * Select Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Select Battery OR external supply for BYPASS LDO power source
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_PMU_POWER_SOURCE smps_source,
        CCP_PMU_POWER_SOURCE bypass_ldo_source);

/**
 * Send CCP PDU pmu_power_source_select_req.
 *
 * DIR TO_CURATOR
 * Select the input power source (battery or charger for
 * the SMPS and Bypass LDO. These need not be the same.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c smps_source
 * Select Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Select Battery OR external supply for BYPASS LDO power source
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_power_source_select_req(arg1, arg2, arg3) \
    ccp_send_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE(CCP_SIGNAL_ID_PMU_POWER_SOURCE_SELECT_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU pmu_power_source_select_req.
 *
 * DIR TO_CURATOR
 * Select the input power source (battery or charger for
 * the SMPS and Bypass LDO. These need not be the same.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c smps_source
 * Select Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Select Battery OR external supply for BYPASS LDO power source
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_PMU_POWER_SOURCE *smps_source,
        CCP_PMU_POWER_SOURCE *bypass_ldo_source);

/**
 * Analyse received CCP PDU pmu_power_source_select_req.
 *
 * DIR TO_CURATOR
 * Select the input power source (battery or charger for
 * the SMPS and Bypass LDO. These need not be the same.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c smps_source
 * Select Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Select Battery OR external supply for BYPASS LDO power source
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_power_source_select_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU pmu_power_source_select_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the POWER_SOURCE_SELECT
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c results
 * Status of the power mode change request
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_PMU_POWER_SOURCE_REQ_RESULT(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_PMU_POWER_SOURCE_REQ_RESULT results);

/**
 * Send CCP PDU pmu_power_source_select_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the POWER_SOURCE_SELECT
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c results
 * Status of the power mode change request
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_power_source_select_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_PMU_POWER_SOURCE_REQ_RESULT(CCP_SIGNAL_ID_PMU_POWER_SOURCE_SELECT_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU pmu_power_source_select_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the POWER_SOURCE_SELECT
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c results
 * Status of the power mode change request
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_PMU_POWER_SOURCE_REQ_RESULT(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_PMU_POWER_SOURCE_REQ_RESULT *results);

/**
 * Analyse received CCP PDU pmu_power_source_select_rsp.
 *
 * DIR FROM_CURATOR
 * Response to the POWER_SOURCE_SELECT
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c results
 * Status of the power mode change request
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_power_source_select_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_PMU_POWER_SOURCE_REQ_RESULT(arg1, arg2, arg3)


/**
 * Send CCP PDU pmu_power_source_status_req.
 *
 * DIR TO_CURATOR
 * Request the current power source used for SMPS and BYP_LDO
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_power_source_status_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_PMU_POWER_SOURCE_STATUS_REQ, arg1)

/**
 * Analyse received CCP PDU pmu_power_source_status_req.
 *
 * DIR TO_CURATOR
 * Request the current power source used for SMPS and BYP_LDO
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_power_source_status_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU pmu_power_source_status_rsp.
 *
 * DIR FROM_CURATOR
 * Report the current input power source (battery or charger) for
 * the SMPS and Bypass LDO. These need not be the same.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c smps_source
 * Select Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Select Battery OR external supply for BYPASS LDO power source
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_power_source_status_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE(CCP_SIGNAL_ID_PMU_POWER_SOURCE_STATUS_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU pmu_power_source_status_rsp.
 *
 * DIR FROM_CURATOR
 * Report the current input power source (battery or charger) for
 * the SMPS and Bypass LDO. These need not be the same.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c smps_source
 * Select Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Select Battery OR external supply for BYPASS LDO power source
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_power_source_status_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_VCHG_DISCONNECT_EMERGENCY_IND_DATA \
    CCP_VCHG_DISCONNECT_EMERGENCY_IND_DATA_WORD_OFFSET

/**
 * Send CCP PDU vchg_disconnect_emergency_ind.
 *
 * DIR FROM_CURATOR
 * Indication from Curator to a subsystem about the charger disconnect
 * emergency event i.e. a charger getting unplugged while the chip was
 * running from the charger. When such an event occurs then the
 * Janitor/Curator switches the chip to VBAT from VCHG automatically
 * and informs other subsystems (most likely the Apps) about the same
 * via this indication message.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * Non-parameter argument \c data:
 * Spaceholder for any data that the Curator might want to pass along
 * with this indication.
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_VCHG_DISCONNECT_EMERGENCY_IND_DATA_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_vchg_disconnect_emergency_ind(arg1, arg2, arg3) \
    ccp_send_pdu_VARLEN(CCP_SIGNAL_ID_VCHG_DISCONNECT_EMERGENCY_IND, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU vchg_disconnect_emergency_ind.
 *
 * DIR FROM_CURATOR
 * Indication from Curator to a subsystem about the charger disconnect
 * emergency event i.e. a charger getting unplugged while the chip was
 * running from the charger. When such an event occurs then the
 * Janitor/Curator switches the chip to VBAT from VCHG automatically
 * and informs other subsystems (most likely the Apps) about the same
 * via this indication message.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   Non-parameter argument \c data:
 * Spaceholder for any data that the Curator might want to pass along
 * with this indication.
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_VCHG_DISCONNECT_EMERGENCY_IND_DATA_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_vchg_disconnect_emergency_ind(arg1, arg2) \
    ccp_recv_pdu_VARLEN(arg1, arg2)


/**
 * Send CCP PDU nfc_configure_req.
 *
 * DIR TO_CURATOR
 * Request from a subsys to configure the NFC HW. The Apps can use this
 * message to enable/disable the NFC HW block, NFC charge pump and also
 * to clear any NFC overload HW events that may have triggered.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c nfc_hw_enable
 * Enable or Disable the NFC HW block.
 *
 *   * \c charge_pump_enable
 * Enable or Disable the NFC charge pump.
 *
 *   * \c nfc_overload_clear
 * Clear any NFC overload HW events.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_NFC_HW_ENABLE_TYPE__CCP_NFC_CHARGE_PUMP_ENABLE_TYPE__CCP_NFC_OVERLOAD_CLEAR_TYPE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_NFC_HW_ENABLE_TYPE nfc_hw_enable,
        CCP_NFC_CHARGE_PUMP_ENABLE_TYPE charge_pump_enable,
        CCP_NFC_OVERLOAD_CLEAR_TYPE nfc_overload_clear);

/**
 * Send CCP PDU nfc_configure_req.
 *
 * DIR TO_CURATOR
 * Request from a subsys to configure the NFC HW. The Apps can use this
 * message to enable/disable the NFC HW block, NFC charge pump and also
 * to clear any NFC overload HW events that may have triggered.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c nfc_hw_enable
 * Enable or Disable the NFC HW block.
 *
 *   * \c charge_pump_enable
 * Enable or Disable the NFC charge pump.
 *
 *   * \c nfc_overload_clear
 * Clear any NFC overload HW events.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_nfc_configure_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_NFC_HW_ENABLE_TYPE__CCP_NFC_CHARGE_PUMP_ENABLE_TYPE__CCP_NFC_OVERLOAD_CLEAR_TYPE(CCP_SIGNAL_ID_NFC_CONFIGURE_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU nfc_configure_req.
 *
 * DIR TO_CURATOR
 * Request from a subsys to configure the NFC HW. The Apps can use this
 * message to enable/disable the NFC HW block, NFC charge pump and also
 * to clear any NFC overload HW events that may have triggered.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c nfc_hw_enable
 * Enable or Disable the NFC HW block.
 *
 *   * \c charge_pump_enable
 * Enable or Disable the NFC charge pump.
 *
 *   * \c nfc_overload_clear
 * Clear any NFC overload HW events.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_NFC_HW_ENABLE_TYPE__CCP_NFC_CHARGE_PUMP_ENABLE_TYPE__CCP_NFC_OVERLOAD_CLEAR_TYPE(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_NFC_HW_ENABLE_TYPE *nfc_hw_enable,
        CCP_NFC_CHARGE_PUMP_ENABLE_TYPE *charge_pump_enable,
        CCP_NFC_OVERLOAD_CLEAR_TYPE *nfc_overload_clear);

/**
 * Analyse received CCP PDU nfc_configure_req.
 *
 * DIR TO_CURATOR
 * Request from a subsys to configure the NFC HW. The Apps can use this
 * message to enable/disable the NFC HW block, NFC charge pump and also
 * to clear any NFC overload HW events that may have triggered.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c nfc_hw_enable
 * Enable or Disable the NFC HW block.
 *
 *   * \c charge_pump_enable
 * Enable or Disable the NFC charge pump.
 *
 *   * \c nfc_overload_clear
 * Clear any NFC overload HW events.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_nfc_configure_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_CCP_NFC_HW_ENABLE_TYPE__CCP_NFC_CHARGE_PUMP_ENABLE_TYPE__CCP_NFC_OVERLOAD_CLEAR_TYPE(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU nfc_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Response from Curator to the NFC_CONFIGURE_REQ from the subsys
 * confirming whether configuring the NFC HW succeeded or failed.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c result
 * Enable or Disable the NFC HW block.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_NFC_CONFIGURE_RSP_TYPE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_NFC_CONFIGURE_RSP_TYPE result);

/**
 * Send CCP PDU nfc_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Response from Curator to the NFC_CONFIGURE_REQ from the subsys
 * confirming whether configuring the NFC HW succeeded or failed.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c result
 * Enable or Disable the NFC HW block.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_nfc_configure_rsp(arg1, arg2) \
    ccp_send_pdu_CCP_NFC_CONFIGURE_RSP_TYPE(CCP_SIGNAL_ID_NFC_CONFIGURE_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU nfc_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Response from Curator to the NFC_CONFIGURE_REQ from the subsys
 * confirming whether configuring the NFC HW succeeded or failed.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c result
 * Enable or Disable the NFC HW block.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_NFC_CONFIGURE_RSP_TYPE(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_NFC_CONFIGURE_RSP_TYPE *result);

/**
 * Analyse received CCP PDU nfc_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Response from Curator to the NFC_CONFIGURE_REQ from the subsys
 * confirming whether configuring the NFC HW succeeded or failed.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c result
 * Enable or Disable the NFC HW block.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_nfc_configure_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_NFC_CONFIGURE_RSP_TYPE(arg1, arg2, arg3)


/**
 * Send CCP PDU subsys_speed_configure_req.
 *
 * DIR TO_CURATOR
 * A request to limit the speed of a subsystem.
 * Must be sent before the subsystem is started, must be a valid speed
 * for that subsystem, and must be for a subsystem that curator supports
 * starting at different speeds. If any of these constraints are not
 * satisifed, the request will be rejected.
 * Note that the valid speeds of the subsystem can be limited by eFuse or
 * by MIB keys and any request must be compatiable with these limits.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c speed_in_mhz
 * The speed in the MHz to run the subsystem at.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        SYSTEM_SUBSYSTEM_TYPE subsystem,
        uint16 speed_in_mhz);

/**
 * Send CCP PDU subsys_speed_configure_req.
 *
 * DIR TO_CURATOR
 * A request to limit the speed of a subsystem.
 * Must be sent before the subsystem is started, must be a valid speed
 * for that subsystem, and must be for a subsystem that curator supports
 * starting at different speeds. If any of these constraints are not
 * satisifed, the request will be rejected.
 * Note that the valid speeds of the subsystem can be limited by eFuse or
 * by MIB keys and any request must be compatiable with these limits.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c speed_in_mhz
 * The speed in the MHz to run the subsystem at.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsys_speed_configure_req(arg1, arg2, arg3) \
    ccp_send_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16(CCP_SIGNAL_ID_SUBSYS_SPEED_CONFIGURE_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU subsys_speed_configure_req.
 *
 * DIR TO_CURATOR
 * A request to limit the speed of a subsystem.
 * Must be sent before the subsystem is started, must be a valid speed
 * for that subsystem, and must be for a subsystem that curator supports
 * starting at different speeds. If any of these constraints are not
 * satisifed, the request will be rejected.
 * Note that the valid speeds of the subsystem can be limited by eFuse or
 * by MIB keys and any request must be compatiable with these limits.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c speed_in_mhz
 * The speed in the MHz to run the subsystem at.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        SYSTEM_SUBSYSTEM_TYPE *subsystem,
        uint16 *speed_in_mhz);

/**
 * Analyse received CCP PDU subsys_speed_configure_req.
 *
 * DIR TO_CURATOR
 * A request to limit the speed of a subsystem.
 * Must be sent before the subsystem is started, must be a valid speed
 * for that subsystem, and must be for a subsystem that curator supports
 * starting at different speeds. If any of these constraints are not
 * satisifed, the request will be rejected.
 * Note that the valid speeds of the subsystem can be limited by eFuse or
 * by MIB keys and any request must be compatiable with these limits.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * The subsystem type identifier.  This is one of the values
 * from the enumeration SYSTEM_SUBSYSTEM_TYPE in the
 * system.xml interface.
 *
 *   * \c speed_in_mhz
 * The speed in the MHz to run the subsystem at.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsys_speed_configure_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_SYSTEM_SUBSYSTEM_TYPE__uint16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU subsys_speed_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Response from Curator to a CCP_SUBSYS_SPEED_CONFIGURE_REQ
 * confirming whether the configuration has succeeded.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c result
 * Result: 0 indicates failure, 1 indicates success
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_subsys_speed_configure_rsp(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_SUBSYS_SPEED_CONFIGURE_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU subsys_speed_configure_rsp.
 *
 * DIR FROM_CURATOR
 * Response from Curator to a CCP_SUBSYS_SPEED_CONFIGURE_REQ
 * confirming whether the configuration has succeeded.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c result
 * Result: 0 indicates failure, 1 indicates success
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_subsys_speed_configure_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU pmu_weak_battery_ind.
 *
 * DIR FROM_CURATOR
 * Indication from Curator to the Apps subsystem indicating that
 * the battery is too weak (<2.6V). On reception of this message
 * the Apps can decide to either turn off the chip or move the chip to
 * run from the charger supply (if present). This message also includes
 * the information about whether there is charger supply (VCHG) available
 * and also the power source that the BYP LDO and SMPSs are running from
 * (i.e. VBAT or VCHG).
 * 
 * DEPRECATED: This PDU is no longer sent. Crescendo a04 curator PB V12
 * patches it out, and the code has been removed from curator mainline.
 * It is intended Apps will poll VBAT periodically using an ADC and
 * switch off below a suitable voltage.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c smps_source
 * Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Battery OR external supply for BYPASS LDO power source
 *
 *   * \c charger_connected
 * Indicates whether a charger (VCHG supply) is connected or not.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE__CCP_CHARGER_CONNECTED_TYPE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_PMU_POWER_SOURCE smps_source,
        CCP_PMU_POWER_SOURCE bypass_ldo_source,
        CCP_CHARGER_CONNECTED_TYPE charger_connected);

/**
 * Send CCP PDU pmu_weak_battery_ind.
 *
 * DIR FROM_CURATOR
 * Indication from Curator to the Apps subsystem indicating that
 * the battery is too weak (<2.6V). On reception of this message
 * the Apps can decide to either turn off the chip or move the chip to
 * run from the charger supply (if present). This message also includes
 * the information about whether there is charger supply (VCHG) available
 * and also the power source that the BYP LDO and SMPSs are running from
 * (i.e. VBAT or VCHG).
 * 
 * DEPRECATED: This PDU is no longer sent. Crescendo a04 curator PB V12
 * patches it out, and the code has been removed from curator mainline.
 * It is intended Apps will poll VBAT periodically using an ADC and
 * switch off below a suitable voltage.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c smps_source
 * Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Battery OR external supply for BYPASS LDO power source
 *
 *   * \c charger_connected
 * Indicates whether a charger (VCHG supply) is connected or not.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_pmu_weak_battery_ind(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE__CCP_CHARGER_CONNECTED_TYPE(CCP_SIGNAL_ID_PMU_WEAK_BATTERY_IND, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU pmu_weak_battery_ind.
 *
 * DIR FROM_CURATOR
 * Indication from Curator to the Apps subsystem indicating that
 * the battery is too weak (<2.6V). On reception of this message
 * the Apps can decide to either turn off the chip or move the chip to
 * run from the charger supply (if present). This message also includes
 * the information about whether there is charger supply (VCHG) available
 * and also the power source that the BYP LDO and SMPSs are running from
 * (i.e. VBAT or VCHG).
 * 
 * DEPRECATED: This PDU is no longer sent. Crescendo a04 curator PB V12
 * patches it out, and the code has been removed from curator mainline.
 * It is intended Apps will poll VBAT periodically using an ADC and
 * switch off below a suitable voltage.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c smps_source
 * Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Battery OR external supply for BYPASS LDO power source
 *
 *   * \c charger_connected
 * Indicates whether a charger (VCHG supply) is connected or not.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE__CCP_CHARGER_CONNECTED_TYPE(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_PMU_POWER_SOURCE *smps_source,
        CCP_PMU_POWER_SOURCE *bypass_ldo_source,
        CCP_CHARGER_CONNECTED_TYPE *charger_connected);

/**
 * Analyse received CCP PDU pmu_weak_battery_ind.
 *
 * DIR FROM_CURATOR
 * Indication from Curator to the Apps subsystem indicating that
 * the battery is too weak (<2.6V). On reception of this message
 * the Apps can decide to either turn off the chip or move the chip to
 * run from the charger supply (if present). This message also includes
 * the information about whether there is charger supply (VCHG) available
 * and also the power source that the BYP LDO and SMPSs are running from
 * (i.e. VBAT or VCHG).
 * 
 * DEPRECATED: This PDU is no longer sent. Crescendo a04 curator PB V12
 * patches it out, and the code has been removed from curator mainline.
 * It is intended Apps will poll VBAT periodically using an ADC and
 * switch off below a suitable voltage.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c smps_source
 * Battery OR external supply for SMPS power source
 *
 *   * \c bypass_ldo_source
 * Battery OR external supply for BYPASS LDO power source
 *
 *   * \c charger_connected
 * Indicates whether a charger (VCHG supply) is connected or not.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_pmu_weak_battery_ind(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_CCP_PMU_POWER_SOURCE__CCP_PMU_POWER_SOURCE__CCP_CHARGER_CONNECTED_TYPE(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU usb_host_charging_req.
 *
 * DIR TO_CURATOR
 * Request to curator to set the value of the Curator "USB_CHARGING"
 * register. The lower 3 bits of this registers are used by USB
 * charger detection, and are discarded from the request.
 * Handler for this message is not aware of specific fields in the
 * register and writes the value directly to hardware (after masking
 * off the lower 3 bits), the user must construct the value written to
 * match the USB_CHARGING register bits.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c usb_charging_reg
 * Register value to write.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_usb_host_charging_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_USB_HOST_CHARGING_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU usb_host_charging_req.
 *
 * DIR TO_CURATOR
 * Request to curator to set the value of the Curator "USB_CHARGING"
 * register. The lower 3 bits of this registers are used by USB
 * charger detection, and are discarded from the request.
 * Handler for this message is not aware of specific fields in the
 * register and writes the value directly to hardware (after masking
 * off the lower 3 bits), the user must construct the value written to
 * match the USB_CHARGING register bits.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c usb_charging_reg
 * Register value to write.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_usb_host_charging_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU usb_host_charging_rsp.
 *
 * DIR FROM_CURATOR
 * Confirmation the USB_CHARGING register has been written,
 * allowing the APPS to measure anything it needs.
 * This is always succesful, and is reported for timing reasons.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c error
 * Set to 1 if error
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_usb_host_charging_rsp(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_USB_HOST_CHARGING_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU usb_host_charging_rsp.
 *
 * DIR FROM_CURATOR
 * Confirmation the USB_CHARGING register has been written,
 * allowing the APPS to measure anything it needs.
 * This is always succesful, and is reported for timing reasons.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c error
 * Set to 1 if error
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_usb_host_charging_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_DORMANT_ENTER_REQ_WAKE_LIST \
    CCP_DORMANT_ENTER_REQ_WAKE_LIST_WORD_OFFSET

/**
 * Send CCP PDU dormant_enter_req.
 *
 * DIR TO_CURATOR
 * Configure something to wake the chip from Dormant.
 * The response message is CCP_DORMANT_RSP which will only be sent if
 * the request fails.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c wake_list_count
 * Number of items in the wake source list
 *
 *   * Non-parameter argument \c wake_list:
 * List of chip wake options
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_DORMANT_ENTER_REQ_WAKE_LIST_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_dormant_enter_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_uint16__VARLEN(CCP_SIGNAL_ID_DORMANT_ENTER_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU dormant_enter_req.
 *
 * DIR TO_CURATOR
 * Configure something to wake the chip from Dormant.
 * The response message is CCP_DORMANT_RSP which will only be sent if
 * the request fails.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c wake_list_count
 * Number of items in the wake source list
 *
 *   Non-parameter argument \c wake_list:
 * List of chip wake options
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_DORMANT_ENTER_REQ_WAKE_LIST_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_dormant_enter_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU adc_read_req.
 *
 * DIR TO_CURATOR
 * Request to curator to do an ADC reading using the ADC it has under
 * control. The source is the MUX BUS that was chosen by the last
 * CCP_PMU_MUX_TO_GMUX_REQ request
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c adc_config
 * ADC configuration bit mask (see \c CCP_ADC_READ_CONFIG).
 *
 *   * \c sample_no
 * Number of samples to accumulate.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint8(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                uint16 adc_config,
                                uint8 sample_no);

/**
 * Send CCP PDU adc_read_req.
 *
 * DIR TO_CURATOR
 * Request to curator to do an ADC reading using the ADC it has under
 * control. The source is the MUX BUS that was chosen by the last
 * CCP_PMU_MUX_TO_GMUX_REQ request
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c adc_config
 * ADC configuration bit mask (see \c CCP_ADC_READ_CONFIG).
 *
 *   * \c sample_no
 * Number of samples to accumulate.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_adc_read_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint8(CCP_SIGNAL_ID_ADC_READ_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU adc_read_req.
 *
 * DIR TO_CURATOR
 * Request to curator to do an ADC reading using the ADC it has under
 * control. The source is the MUX BUS that was chosen by the last
 * CCP_PMU_MUX_TO_GMUX_REQ request
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c adc_config
 * ADC configuration bit mask (see \c CCP_ADC_READ_CONFIG).
 *
 *   * \c sample_no
 * Number of samples to accumulate.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint8(const uint16 *pdu, uint16 pdu_len_words,
                                uint16 *adc_config,
                                uint8 *sample_no);

/**
 * Analyse received CCP PDU adc_read_req.
 *
 * DIR TO_CURATOR
 * Request to curator to do an ADC reading using the ADC it has under
 * control. The source is the MUX BUS that was chosen by the last
 * CCP_PMU_MUX_TO_GMUX_REQ request
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c adc_config
 * ADC configuration bit mask (see \c CCP_ADC_READ_CONFIG).
 *
 *   * \c sample_no
 * Number of samples to accumulate.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_adc_read_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint8(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU adc_read_rsp.
 *
 * DIR FROM_CURATOR
 * Response for the ADC read request. This contains the ADC conversion
 * value if conversion was successful.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c status
 * The status of the request, see \c CCP_ADC_READ_STATUS.
 *
 *   * \c reading
 * The ADC accumulated reading. This should be ignored if the request
 * was unsuccessful.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_ADC_READ_STATUS__int16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_ADC_READ_STATUS status,
        int16 reading);

/**
 * Send CCP PDU adc_read_rsp.
 *
 * DIR FROM_CURATOR
 * Response for the ADC read request. This contains the ADC conversion
 * value if conversion was successful.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c status
 * The status of the request, see \c CCP_ADC_READ_STATUS.
 *
 *   * \c reading
 * The ADC accumulated reading. This should be ignored if the request
 * was unsuccessful.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_adc_read_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_CCP_ADC_READ_STATUS__int16(CCP_SIGNAL_ID_ADC_READ_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU adc_read_rsp.
 *
 * DIR FROM_CURATOR
 * Response for the ADC read request. This contains the ADC conversion
 * value if conversion was successful.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c status
 * The status of the request, see \c CCP_ADC_READ_STATUS.
 *
 *   * \c reading
 * The ADC accumulated reading. This should be ignored if the request
 * was unsuccessful.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_ADC_READ_STATUS__int16(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_ADC_READ_STATUS *status,
        int16 *reading);

/**
 * Analyse received CCP PDU adc_read_rsp.
 *
 * DIR FROM_CURATOR
 * Response for the ADC read request. This contains the ADC conversion
 * value if conversion was successful.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c status
 * The status of the request, see \c CCP_ADC_READ_STATUS.
 *
 *   * \c reading
 * The ADC accumulated reading. This should be ignored if the request
 * was unsuccessful.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_adc_read_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_CCP_ADC_READ_STATUS__int16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU clear_sosc_calibration_req.
 *
 * DIR TO_CURATOR
 * Request to curator to clear the SOSC calibration table.
 * No response or confirmation is required
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_clear_sosc_calibration_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_CLEAR_SOSC_CALIBRATION_REQ, arg1)

/**
 * Analyse received CCP PDU clear_sosc_calibration_req.
 *
 * DIR TO_CURATOR
 * Request to curator to clear the SOSC calibration table.
 * No response or confirmation is required
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_clear_sosc_calibration_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU hostio_sdio_enable_fn_ind.
 *
 * DIR BOTH
 * Sent from curator to a sub system to indicate that the host has
 * enabled its function.
 * The curator expects a \c CCP_SIGNAL_ID_HOSTIO_SDIO_FN_READY_IND
 * response from the sub system once it has initialised its own data
 * structures (and particularly its to host scratch registers).
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_sdio_enable_fn_ind(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOSTIO_SDIO_ENABLE_FN_IND, arg1)

/**
 * Analyse received CCP PDU hostio_sdio_enable_fn_ind.
 *
 * DIR BOTH
 * Sent from curator to a sub system to indicate that the host has
 * enabled its function.
 * The curator expects a \c CCP_SIGNAL_ID_HOSTIO_SDIO_FN_READY_IND
 * response from the sub system once it has initialised its own data
 * structures (and particularly its to host scratch registers).
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_sdio_enable_fn_ind(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU hostio_sdio_disable_fn_ind.
 *
 * DIR BOTH
 * Sent from the curator to a sub system to indicate that the host has
 * disabled its sdio function. No response is expected.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_sdio_disable_fn_ind(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOSTIO_SDIO_DISABLE_FN_IND, arg1)

/**
 * Analyse received CCP PDU hostio_sdio_disable_fn_ind.
 *
 * DIR BOTH
 * Sent from the curator to a sub system to indicate that the host has
 * disabled its sdio function. No response is expected.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_sdio_disable_fn_ind(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU hostio_sdio_fn_ready_ind.
 *
 * DIR BOTH
 * Sent by a sub system to the curator in response to the
 * \c CCP_SIGNAL_ID_HOSTIO_SDIO_ENABLE_FN_IND message. The sub system
 * sends this when its internal data structures and the to-host scratch
 * registers have been initialised. The curator sets the sdio function
 * ready bit on reception of this message.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_sdio_fn_ready_ind(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOSTIO_SDIO_FN_READY_IND, arg1)

/**
 * Analyse received CCP PDU hostio_sdio_fn_ready_ind.
 *
 * DIR BOTH
 * Sent by a sub system to the curator in response to the
 * \c CCP_SIGNAL_ID_HOSTIO_SDIO_ENABLE_FN_IND message. The sub system
 * sends this when its internal data structures and the to-host scratch
 * registers have been initialised. The curator sets the sdio function
 * ready bit on reception of this message.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_sdio_fn_ready_ind(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU hostio_sdio_fn_running_ind.
 *
 * DIR BOTH
 * Sent by the curator to a sub system to indicate when the running bit
 * has been set. The sub system can use this as a cue for initiating
 * transfers to the host.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_sdio_fn_running_ind(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOSTIO_SDIO_FN_RUNNING_IND, arg1)

/**
 * Analyse received CCP PDU hostio_sdio_fn_running_ind.
 *
 * DIR BOTH
 * Sent by the curator to a sub system to indicate when the running bit
 * has been set. The sub system can use this as a cue for initiating
 * transfers to the host.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_sdio_fn_running_ind(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU hostio_sdio_fifo_error_ind.
 *
 * DIR TO_CURATOR
 * Sent by a sub system to the curator to indicate that a fifo error
 * interrupt has occurred. The curator reads the type of fifo error
 * and returns it in the response hostio_sdio_fifo_error_rsp. It also
 * clears the error.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_sdio_fifo_error_ind(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOSTIO_SDIO_FIFO_ERROR_IND, arg1)

/**
 * Analyse received CCP PDU hostio_sdio_fifo_error_ind.
 *
 * DIR TO_CURATOR
 * Sent by a sub system to the curator to indicate that a fifo error
 * interrupt has occurred. The curator reads the type of fifo error
 * and returns it in the response hostio_sdio_fifo_error_rsp. It also
 * clears the error.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_sdio_fifo_error_ind(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU hostio_sdio_fifo_error_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the curator to a sub system in response to the
 * hostio_sdio_fifo_error_ind pdu to indicate the type of fifo
 * error that has occurred.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c fifo_error
 * For Amber D02 this value has bits arranged the
 * same as the DEBUG_OUT1 signal from sdio_buffer.v
 * shifted right by 9 bits - i.e. fifo errors are in bits 0 to 3.
 * For later chips with B-106118 fixed the value here
 * will be zero since the subsystems can read the FIFO reason for
 * themselves. In that case this message is sent by the curator just
 * in case the subsystem needs to know when the FIFO error has been
 * cleared.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_sdio_fifo_error_rsp(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_HOSTIO_SDIO_FIFO_ERROR_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU hostio_sdio_fifo_error_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the curator to a sub system in response to the
 * hostio_sdio_fifo_error_ind pdu to indicate the type of fifo
 * error that has occurred.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c fifo_error
 * For Amber D02 this value has bits arranged the
 * same as the DEBUG_OUT1 signal from sdio_buffer.v
 * shifted right by 9 bits - i.e. fifo errors are in bits 0 to 3.
 * For later chips with B-106118 fixed the value here
 * will be zero since the subsystems can read the FIFO reason for
 * themselves. In that case this message is sent by the curator just
 * in case the subsystem needs to know when the FIFO error has been
 * cleared.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_sdio_fifo_error_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU nbcsp_channel_alloc_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem (possibly the Curator itself) to the Curator
 * requesting that an NBCSP channel be allocated.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID, set to 0 when making an initial request
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c reliable
 * Is the channel to be sequenced?
 *
 *   * \c use_crc
 * Is the channel to use checksum?
 *
 *   * \c tx_winsize
 * Window size for transmit (from subsystem's point of view)
 *
 *   * \c rx_winsize
 * Window size for receive (from subsystem's point of view)
 *
 *   * \c tx_max_msgsize
 * Largest allowable transmitted message
 *
 *   * \c rx_max_msgsize
 * Largest receivable message
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint4__uint4__uint12__Bit__Bit__uint2__uint2__uint11__uint11(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint4 reqid,
        uint4 channel_id,
        uint12 subchannel_id,
        Bit reliable,
        Bit use_crc,
        uint2 tx_winsize,
        uint2 rx_winsize,
        uint11 tx_max_msgsize,
        uint11 rx_max_msgsize);

/**
 * Send CCP PDU nbcsp_channel_alloc_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem (possibly the Curator itself) to the Curator
 * requesting that an NBCSP channel be allocated.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID, set to 0 when making an initial request
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c reliable
 * Is the channel to be sequenced?
 *
 *   * \c use_crc
 * Is the channel to use checksum?
 *
 *   * \c tx_winsize
 * Window size for transmit (from subsystem's point of view)
 *
 *   * \c rx_winsize
 * Window size for receive (from subsystem's point of view)
 *
 *   * \c tx_max_msgsize
 * Largest allowable transmitted message
 *
 *   * \c rx_max_msgsize
 * Largest receivable message
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_nbcsp_channel_alloc_req(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
    ccp_send_pdu_uint4__uint4__uint12__Bit__Bit__uint2__uint2__uint11__uint11(CCP_SIGNAL_ID_NBCSP_CHANNEL_ALLOC_REQ, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)

/**
 * Analyse received CCP PDU nbcsp_channel_alloc_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem (possibly the Curator itself) to the Curator
 * requesting that an NBCSP channel be allocated.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID, set to 0 when making an initial request
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c reliable
 * Is the channel to be sequenced?
 *
 *   * \c use_crc
 * Is the channel to use checksum?
 *
 *   * \c tx_winsize
 * Window size for transmit (from subsystem's point of view)
 *
 *   * \c rx_winsize
 * Window size for receive (from subsystem's point of view)
 *
 *   * \c tx_max_msgsize
 * Largest allowable transmitted message
 *
 *   * \c rx_max_msgsize
 * Largest receivable message
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint4__uint4__uint12__Bit__Bit__uint2__uint2__uint11__uint11(
        const uint16 *pdu, uint16 pdu_len_words,
        uint4 *reqid,
        uint4 *channel_id,
        uint12 *subchannel_id,
        Bit *reliable,
        Bit *use_crc,
        uint2 *tx_winsize,
        uint2 *rx_winsize,
        uint11 *tx_max_msgsize,
        uint11 *rx_max_msgsize);

/**
 * Analyse received CCP PDU nbcsp_channel_alloc_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem (possibly the Curator itself) to the Curator
 * requesting that an NBCSP channel be allocated.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID, set to 0 when making an initial request
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c reliable
 * Is the channel to be sequenced?
 *
 *   * \c use_crc
 * Is the channel to use checksum?
 *
 *   * \c tx_winsize
 * Window size for transmit (from subsystem's point of view)
 *
 *   * \c rx_winsize
 * Window size for receive (from subsystem's point of view)
 *
 *   * \c tx_max_msgsize
 * Largest allowable transmitted message
 *
 *   * \c rx_max_msgsize
 * Largest receivable message
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_nbcsp_channel_alloc_req(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) \
    ccp_recv_pdu_uint4__uint4__uint12__Bit__Bit__uint2__uint2__uint11__uint11(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11)


/**
 * Send CCP PDU nbcsp_channel_alloc_rsp.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem (possibly the Curator itself) by the Curator
 * indicating whether an NBCSP channel was allocated or not.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID, assigned by the Curator CAP layer
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c allocated
 * Whether the channel was allocated or not
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint4__uint4__uint12__Bit(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint4 reqid,
        uint4 channel_id,
        uint12 subchannel_id,
        Bit allocated);

/**
 * Send CCP PDU nbcsp_channel_alloc_rsp.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem (possibly the Curator itself) by the Curator
 * indicating whether an NBCSP channel was allocated or not.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID, assigned by the Curator CAP layer
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c allocated
 * Whether the channel was allocated or not
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_nbcsp_channel_alloc_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_uint4__uint4__uint12__Bit(CCP_SIGNAL_ID_NBCSP_CHANNEL_ALLOC_RSP, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU nbcsp_channel_alloc_rsp.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem (possibly the Curator itself) by the Curator
 * indicating whether an NBCSP channel was allocated or not.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID, assigned by the Curator CAP layer
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c allocated
 * Whether the channel was allocated or not
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint4__uint4__uint12__Bit(
        const uint16 *pdu, uint16 pdu_len_words,
        uint4 *reqid,
        uint4 *channel_id,
        uint12 *subchannel_id,
        Bit *allocated);

/**
 * Analyse received CCP PDU nbcsp_channel_alloc_rsp.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem (possibly the Curator itself) by the Curator
 * indicating whether an NBCSP channel was allocated or not.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID, assigned by the Curator CAP layer
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c allocated
 * Whether the channel was allocated or not
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_nbcsp_channel_alloc_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_recv_pdu_uint4__uint4__uint12__Bit(arg1, arg2, arg3, arg4, arg5, arg6)


/**
 * Send CCP PDU nbcsp_channel_refine_rsp.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem (possibly the Curator itself) by the Curator
 * to pass on the host's request for refinements to the channel parameters.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c reliable
 * Is the channel to be sequenced?
 *
 *   * \c use_crc
 * Is the channel to use checksum?
 *
 *   * \c tx_winsize
 * Window size for transmit (from subsystem's point of view)
 *
 *   * \c rx_winsize
 * Window size for receive (from subsystem's point of view)
 *
 *   * \c tx_max_msgsize
 * Largest allowable transmitted message
 *
 *   * \c rx_max_msgsize
 * Largest receivable message
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_nbcsp_channel_refine_rsp(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
    ccp_send_pdu_uint4__uint4__uint12__Bit__Bit__uint2__uint2__uint11__uint11(CCP_SIGNAL_ID_NBCSP_CHANNEL_REFINE_RSP, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)

/**
 * Analyse received CCP PDU nbcsp_channel_refine_rsp.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem (possibly the Curator itself) by the Curator
 * to pass on the host's request for refinements to the channel parameters.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c reqid
 * Request ID, generated by the requesting subsystem to distinguish
 * requests
 *
 *   * \c channel_id
 * Channel ID
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 *   * \c reliable
 * Is the channel to be sequenced?
 *
 *   * \c use_crc
 * Is the channel to use checksum?
 *
 *   * \c tx_winsize
 * Window size for transmit (from subsystem's point of view)
 *
 *   * \c rx_winsize
 * Window size for receive (from subsystem's point of view)
 *
 *   * \c tx_max_msgsize
 * Largest allowable transmitted message
 *
 *   * \c rx_max_msgsize
 * Largest receivable message
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_nbcsp_channel_refine_rsp(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) \
    ccp_recv_pdu_uint4__uint4__uint12__Bit__Bit__uint2__uint2__uint11__uint11(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11)


/**
 * Send CCP PDU nbcsp_channel_dealloc_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem (possibly the Curator itself) to the Curator
 * requesting that an open NBCSP channel be deallocated. There is no
 * corresponding response because this will always succeed and the
 * requesting subsystem can assume the channel is deallocated immediately.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c channel_id
 * Channel ID
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint4__uint12(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                uint4 channel_id,
                                uint12 subchannel_id);

/**
 * Send CCP PDU nbcsp_channel_dealloc_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem (possibly the Curator itself) to the Curator
 * requesting that an open NBCSP channel be deallocated. There is no
 * corresponding response because this will always succeed and the
 * requesting subsystem can assume the channel is deallocated immediately.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c channel_id
 * Channel ID
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_nbcsp_channel_dealloc_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint4__uint12(CCP_SIGNAL_ID_NBCSP_CHANNEL_DEALLOC_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU nbcsp_channel_dealloc_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem (possibly the Curator itself) to the Curator
 * requesting that an open NBCSP channel be deallocated. There is no
 * corresponding response because this will always succeed and the
 * requesting subsystem can assume the channel is deallocated immediately.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c channel_id
 * Channel ID
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint4__uint12(const uint16 *pdu, uint16 pdu_len_words,
                                uint4 *channel_id,
                                uint12 *subchannel_id);

/**
 * Analyse received CCP PDU nbcsp_channel_dealloc_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem (possibly the Curator itself) to the Curator
 * requesting that an open NBCSP channel be deallocated. There is no
 * corresponding response because this will always succeed and the
 * requesting subsystem can assume the channel is deallocated immediately.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c channel_id
 * Channel ID
 *
 *   * \c subchannel_id
 * Subchannel can be anything, really.  Not sure this should be CAP's
 * business.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_nbcsp_channel_dealloc_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint4__uint12(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU hostio_usb_attach_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request attachment of a USB device.
 * CCP_HOSTIO_USB_ATTACH_RSP indicates whether the attachment was
 * successful or not.  The subsystem sends a home-made device ID
 * that is a temporary identifier until a port is allocated.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c device_id
 * Temporary request identifier; won't be referred to again once the
 * port_is made available by response.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_attach_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_HOSTIO_USB_ATTACH_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU hostio_usb_attach_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request attachment of a USB device.
 * CCP_HOSTIO_USB_ATTACH_RSP indicates whether the attachment was
 * successful or not.  The subsystem sends a home-made device ID
 * that is a temporary identifier until a port is allocated.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c device_id
 * Temporary request identifier; won't be referred to again once the
 * port_is made available by response.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_attach_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU hostio_usb_attach_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by a subsystem in response to a request for attachment of a device.
 * If successful, the port field contains the portID the device is attached
 * to.  This is the identifier by which all future hub messages will be
 * parametrised. If unsuccessful, it contains 0.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c device_id
 * Temporary request identifier; won't be referred to again now that the
 * port_id is available
 *
 *   * \c port_id
 * Port number: (0 => couldn't attach)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_attach_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID_HOSTIO_USB_ATTACH_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU hostio_usb_attach_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by a subsystem in response to a request for attachment of a device.
 * If successful, the port field contains the portID the device is attached
 * to.  This is the identifier by which all future hub messages will be
 * parametrised. If unsuccessful, it contains 0.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c device_id
 * Temporary request identifier; won't be referred to again now that the
 * port_id is available
 *
 *   * \c port_id
 * Port number: (0 => couldn't attach)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_attach_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU hostio_usb_detach_ind.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to indicate that it is detaching the device on the given
 * port.  There is no need for a response; the subsystem can start ignoring the
 * USB bus as soon as it likes.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_detach_ind(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_HOSTIO_USB_DETACH_IND, arg1, arg2)

/**
 * Analyse received CCP PDU hostio_usb_detach_ind.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to indicate that it is detaching the device on the given
 * port.  There is no need for a response; the subsystem can start ignoring the
 * USB bus as soon as it likes.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_detach_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU hostio_usb_endpoint_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a certain number of in and out endpoints.
 * Note that we use the USB standard's meaning of "in" and "out", i.e. from the
 * host's point of view. This message can also be used to release endpoints by
 * setting "allocate_control_eps" to "CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE_RELEASE".
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 *   * \c allocate_control_eps
 * Whether to allocate a control endpoints pair
 *
 *   * \c num_out_eps
 * Required number of out endpoints
 *
 *   * \c num_in_eps
 * Required number of in endpoints
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE__uint16__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 port_id,
        CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE allocate_control_eps,
        uint16 num_out_eps,
        uint16 num_in_eps);

/**
 * Send CCP PDU hostio_usb_endpoint_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a certain number of in and out endpoints.
 * Note that we use the USB standard's meaning of "in" and "out", i.e. from the
 * host's point of view. This message can also be used to release endpoints by
 * setting "allocate_control_eps" to "CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE_RELEASE".
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 *   * \c allocate_control_eps
 * Whether to allocate a control endpoints pair
 *
 *   * \c num_out_eps
 * Required number of out endpoints
 *
 *   * \c num_in_eps
 * Required number of in endpoints
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_endpoint_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_uint16__CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE__uint16__uint16(CCP_SIGNAL_ID_HOSTIO_USB_ENDPOINT_REQ, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU hostio_usb_endpoint_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a certain number of in and out endpoints.
 * Note that we use the USB standard's meaning of "in" and "out", i.e. from the
 * host's point of view. This message can also be used to release endpoints by
 * setting "allocate_control_eps" to "CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE_RELEASE".
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 *   * \c allocate_control_eps
 * Whether to allocate a control endpoints pair
 *
 *   * \c num_out_eps
 * Required number of out endpoints
 *
 *   * \c num_in_eps
 * Required number of in endpoints
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE__uint16__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *port_id,
        CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE *allocate_control_eps,
        uint16 *num_out_eps,
        uint16 *num_in_eps);

/**
 * Analyse received CCP PDU hostio_usb_endpoint_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a certain number of in and out endpoints.
 * Note that we use the USB standard's meaning of "in" and "out", i.e. from the
 * host's point of view. This message can also be used to release endpoints by
 * setting "allocate_control_eps" to "CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE_RELEASE".
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 *   * \c allocate_control_eps
 * Whether to allocate a control endpoints pair
 *
 *   * \c num_out_eps
 * Required number of out endpoints
 *
 *   * \c num_in_eps
 * Required number of in endpoints
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_endpoint_req(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_recv_pdu_uint16__CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE__uint16__uint16(arg1, arg2, arg3, arg4, arg5, arg6)


/**
 * Send CCP PDU hostio_usb_endpoint_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem the endpoints it has been
 * allocated.  This may not be as many as requested; it's up to the recipient
 * to decide whether it can live with that.  If not, it should send a DETACH_IND.
 * There are 32 endpoints of each type in the Hydra USB block, so we use bitmaps
 * to indicate which have been allocated
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 *   * \c allocated_control_eps
 * Indicates that a control endpoints pair has been allocated
 *
 *   * \c out_ep_bitmap
 * Bitmap of assigned out endpoints
 *
 *   * \c in_ep_bitmap
 * Bitmap of assigned in endpoints
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE__uint32__uint32(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 port_id,
        CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE allocated_control_eps,
        uint32 out_ep_bitmap,
        uint32 in_ep_bitmap);

/**
 * Send CCP PDU hostio_usb_endpoint_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem the endpoints it has been
 * allocated.  This may not be as many as requested; it's up to the recipient
 * to decide whether it can live with that.  If not, it should send a DETACH_IND.
 * There are 32 endpoints of each type in the Hydra USB block, so we use bitmaps
 * to indicate which have been allocated
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 *   * \c allocated_control_eps
 * Indicates that a control endpoints pair has been allocated
 *
 *   * \c out_ep_bitmap
 * Bitmap of assigned out endpoints
 *
 *   * \c in_ep_bitmap
 * Bitmap of assigned in endpoints
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_endpoint_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_uint16__CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE__uint32__uint32(CCP_SIGNAL_ID_HOSTIO_USB_ENDPOINT_RSP, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU hostio_usb_endpoint_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem the endpoints it has been
 * allocated.  This may not be as many as requested; it's up to the recipient
 * to decide whether it can live with that.  If not, it should send a DETACH_IND.
 * There are 32 endpoints of each type in the Hydra USB block, so we use bitmaps
 * to indicate which have been allocated
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 *   * \c allocated_control_eps
 * Indicates that a control endpoints pair has been allocated
 *
 *   * \c out_ep_bitmap
 * Bitmap of assigned out endpoints
 *
 *   * \c in_ep_bitmap
 * Bitmap of assigned in endpoints
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE__uint32__uint32(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *port_id,
        CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE *allocated_control_eps,
        uint32 *out_ep_bitmap,
        uint32 *in_ep_bitmap);

/**
 * Analyse received CCP PDU hostio_usb_endpoint_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem the endpoints it has been
 * allocated.  This may not be as many as requested; it's up to the recipient
 * to decide whether it can live with that.  If not, it should send a DETACH_IND.
 * There are 32 endpoints of each type in the Hydra USB block, so we use bitmaps
 * to indicate which have been allocated
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c port_id
 * Port number: (0 => invalid)
 *
 *   * \c allocated_control_eps
 * Indicates that a control endpoints pair has been allocated
 *
 *   * \c out_ep_bitmap
 * Bitmap of assigned out endpoints
 *
 *   * \c in_ep_bitmap
 * Bitmap of assigned in endpoints
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_endpoint_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_recv_pdu_uint16__CCP_HOSTIO_USB_ENDPOINT_REQ_TYPE__uint32__uint32(arg1, arg2, arg3, arg4, arg5, arg6)


/**
 * Send CCP PDU hostio_usb_reset_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem that a reset event has
 * been seen on the bus (if port_id is 0) or a selective reset has been
 * requested by the host (if port_id is non-zero)
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c port_id
 * Port number (0 => all ports)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_reset_ind(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_HOSTIO_USB_RESET_IND, arg1, arg2)

/**
 * Analyse received CCP PDU hostio_usb_reset_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem that a reset event has
 * been seen on the bus (if port_id is 0) or a selective reset has been
 * requested by the host (if port_id is non-zero)
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c port_id
 * Port number (0 => all ports)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_reset_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU hostio_usb_suspend_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem that a suspend event has
 * been seen on the bus (if port_id is 0) or a selective suspend has been
 * requested by the host (if port_id is non-zero)
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c port_id
 * Port number (0 => all ports)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_suspend_ind(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_HOSTIO_USB_SUSPEND_IND, arg1, arg2)

/**
 * Analyse received CCP PDU hostio_usb_suspend_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem that a suspend event has
 * been seen on the bus (if port_id is 0) or a selective suspend has been
 * requested by the host (if port_id is non-zero)
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c port_id
 * Port number (0 => all ports)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_suspend_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU hostio_usb_resume_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem that a resume event has
 * been seen on the bus (if port_id is 0) or a selective resume has been
 * requested by the host (if port_id is non-zero)
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c port_id
 * Port number (0 => all ports)
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_resume_ind(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_HOSTIO_USB_RESUME_IND, arg1, arg2)

/**
 * Analyse received CCP PDU hostio_usb_resume_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to indicate to a subsystem that a resume event has
 * been seen on the bus (if port_id is 0) or a selective resume has been
 * requested by the host (if port_id is non-zero)
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c port_id
 * Port number (0 => all ports)
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_resume_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU hostio_usb_resume_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to indicate that it is asserting remote wakeup.
 * It doesn't matter which device is asserting wakeup; the hub will
 * send RESUME_INDs to everybody.
 * 
 * In reality it's likely this signal won't be used because remote wakeup
 * will be handled by a different on-chip mechanism.  However, we add this in
 * now for completeness.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_resume_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_HOSTIO_USB_RESUME_REQ, arg1)

/**
 * Analyse received CCP PDU hostio_usb_resume_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to indicate that it is asserting remote wakeup.
 * It doesn't matter which device is asserting wakeup; the hub will
 * send RESUME_INDs to everybody.
 * 
 * In reality it's likely this signal won't be used because remote wakeup
 * will be handled by a different on-chip mechanism.  However, we add this in
 * now for completeness.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_resume_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU siflash_config_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a particular serial flash chip
 * configuration parameters.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint8__uint8(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                               uint8 subsystem,
                               uint8 bank);

/**
 * Send CCP PDU siflash_config_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a particular serial flash chip
 * configuration parameters.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_siflash_config_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint8__uint8(CCP_SIGNAL_ID_SIFLASH_CONFIG_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU siflash_config_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a particular serial flash chip
 * configuration parameters.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint8__uint8(const uint16 *pdu, uint16 pdu_len_words,
                               uint8 *subsystem,
                               uint8 *bank);

/**
 * Analyse received CCP PDU siflash_config_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a particular serial flash chip
 * configuration parameters.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_siflash_config_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint8__uint8(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SIFLASH_CONFIG_RSP_STATUS_AND_RESET_VECTORS \
    CCP_SIFLASH_CONFIG_RSP_STATUS_AND_RESET_VECTORS_WORD_OFFSET

/**
 * Send CCP PDU siflash_config_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to return to a subsystem the serial flash
 * device configuration parameters it has requested.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 *   * \c result
 * The result of the request.
 *
 *   * \c device_size
 * Serial flash device size in 4KB blocks.
 *
 *   * \c erase_block_size
 * Erase block size, in 4KB blocks.
 *
 *   * \c erase_command
 * Block erase command
 *
 *   * \c write_command
 * Page program command
 *
 *   * \c write_enable_command
 * Write enable command
 *
 *   * \c status_reg_size
 * Size of status register in octets
 *
 *   * \c busy_status_mask
 * Mask busy status in the status register
 *
 *   * \c busy_status_value
 * Value indicating busy status in the status register (after
 * applying "busy_status_mask")
 *
 *   * \c write_enable_latch_mask
 * Mask write enable latch status in the status register
 *
 *   * Non-parameter argument \c status_and_reset_vectors:
 * Two vectors one following after another:
 * Busy status read vector: 4 words
 * Reset vector: 16 words
 * 
 * Each vector is an array of octets with command(s) packed
 * into uint16 words in big-endian and padded at the end.
 * 
 * A command vector can be described by the following structure:
 * 
 * struct cmd_vector {
 * uint8 total_commands_count;
 * struct cmd
 * {
 * uint8 cmd_length;
 * uint8 cmd[cmd_length];
 * } cmds[total_commands_count];
 * }
 * 
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SIFLASH_CONFIG_RSP_STATUS_AND_RESET_VECTORS_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint8__uint8__CCP_SIFLASH_RESULT__uint32__uint16__uint8__uint8__uint8__uint8__uint16__uint16__uint16__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        uint8 subsystem,
        uint8 bank,
        CCP_SIFLASH_RESULT result,
        uint32 device_size,
        uint16 erase_block_size,
        uint8 erase_command,
        uint8 write_command,
        uint8 write_enable_command,
        uint8 status_reg_size,
        uint16 busy_status_mask,
        uint16 busy_status_value,
        uint16 write_enable_latch_mask);

/**
 * Send CCP PDU siflash_config_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to return to a subsystem the serial flash
 * device configuration parameters it has requested.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 *   * \c result
 * The result of the request.
 *
 *   * \c device_size
 * Serial flash device size in 4KB blocks.
 *
 *   * \c erase_block_size
 * Erase block size, in 4KB blocks.
 *
 *   * \c erase_command
 * Block erase command
 *
 *   * \c write_command
 * Page program command
 *
 *   * \c write_enable_command
 * Write enable command
 *
 *   * \c status_reg_size
 * Size of status register in octets
 *
 *   * \c busy_status_mask
 * Mask busy status in the status register
 *
 *   * \c busy_status_value
 * Value indicating busy status in the status register (after
 * applying "busy_status_mask")
 *
 *   * \c write_enable_latch_mask
 * Mask write enable latch status in the status register
 *
 *   * Non-parameter argument \c status_and_reset_vectors:
 * Two vectors one following after another:
 * Busy status read vector: 4 words
 * Reset vector: 16 words
 * 
 * Each vector is an array of octets with command(s) packed
 * into uint16 words in big-endian and padded at the end.
 * 
 * A command vector can be described by the following structure:
 * 
 * struct cmd_vector {
 * uint8 total_commands_count;
 * struct cmd
 * {
 * uint8 cmd_length;
 * uint8 cmd[cmd_length];
 * } cmds[total_commands_count];
 * }
 * 
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SIFLASH_CONFIG_RSP_STATUS_AND_RESET_VECTORS_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_siflash_config_rsp(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15) \
    ccp_send_pdu_uint8__uint8__CCP_SIFLASH_RESULT__uint32__uint16__uint8__uint8__uint8__uint8__uint16__uint16__uint16__VARLEN(CCP_SIGNAL_ID_SIFLASH_CONFIG_RSP, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15)

/**
 * Analyse received CCP PDU siflash_config_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to return to a subsystem the serial flash
 * device configuration parameters it has requested.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 *   * \c result
 * The result of the request.
 *
 *   * \c device_size
 * Serial flash device size in 4KB blocks.
 *
 *   * \c erase_block_size
 * Erase block size, in 4KB blocks.
 *
 *   * \c erase_command
 * Block erase command
 *
 *   * \c write_command
 * Page program command
 *
 *   * \c write_enable_command
 * Write enable command
 *
 *   * \c status_reg_size
 * Size of status register in octets
 *
 *   * \c busy_status_mask
 * Mask busy status in the status register
 *
 *   * \c busy_status_value
 * Value indicating busy status in the status register (after
 * applying "busy_status_mask")
 *
 *   * \c write_enable_latch_mask
 * Mask write enable latch status in the status register
 *
 *   Non-parameter argument \c status_and_reset_vectors:
 * Two vectors one following after another:
 * Busy status read vector: 4 words
 * Reset vector: 16 words
 * 
 * Each vector is an array of octets with command(s) packed
 * into uint16 words in big-endian and padded at the end.
 * 
 * A command vector can be described by the following structure:
 * 
 * struct cmd_vector {
 * uint8 total_commands_count;
 * struct cmd
 * {
 * uint8 cmd_length;
 * uint8 cmd[cmd_length];
 * } cmds[total_commands_count];
 * }
 * 
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SIFLASH_CONFIG_RSP_STATUS_AND_RESET_VECTORS_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint8__uint8__CCP_SIFLASH_RESULT__uint32__uint16__uint8__uint8__uint8__uint8__uint16__uint16__uint16__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        uint8 *subsystem,
        uint8 *bank,
        CCP_SIFLASH_RESULT *result,
        uint32 *device_size,
        uint16 *erase_block_size,
        uint8 *erase_command,
        uint8 *write_command,
        uint8 *write_enable_command,
        uint8 *status_reg_size,
        uint16 *busy_status_mask,
        uint16 *busy_status_value,
        uint16 *write_enable_latch_mask);

/**
 * Analyse received CCP PDU siflash_config_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to return to a subsystem the serial flash
 * device configuration parameters it has requested.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 *   * \c result
 * The result of the request.
 *
 *   * \c device_size
 * Serial flash device size in 4KB blocks.
 *
 *   * \c erase_block_size
 * Erase block size, in 4KB blocks.
 *
 *   * \c erase_command
 * Block erase command
 *
 *   * \c write_command
 * Page program command
 *
 *   * \c write_enable_command
 * Write enable command
 *
 *   * \c status_reg_size
 * Size of status register in octets
 *
 *   * \c busy_status_mask
 * Mask busy status in the status register
 *
 *   * \c busy_status_value
 * Value indicating busy status in the status register (after
 * applying "busy_status_mask")
 *
 *   * \c write_enable_latch_mask
 * Mask write enable latch status in the status register
 *
 *   Non-parameter argument \c status_and_reset_vectors:
 * Two vectors one following after another:
 * Busy status read vector: 4 words
 * Reset vector: 16 words
 * 
 * Each vector is an array of octets with command(s) packed
 * into uint16 words in big-endian and padded at the end.
 * 
 * A command vector can be described by the following structure:
 * 
 * struct cmd_vector {
 * uint8 total_commands_count;
 * struct cmd
 * {
 * uint8 cmd_length;
 * uint8 cmd[cmd_length];
 * } cmds[total_commands_count];
 * }
 * 
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SIFLASH_CONFIG_RSP_STATUS_AND_RESET_VECTORS_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_siflash_config_rsp(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14) \
    ccp_recv_pdu_uint8__uint8__CCP_SIFLASH_RESULT__uint32__uint16__uint8__uint8__uint8__uint8__uint16__uint16__uint16__VARLEN(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14)


/**
 * Send CCP PDU charger_detect_ind.
 *
 * DIR FROM_CURATOR
 * Sent by curator to provide a subsystem with information
 * about the charger.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c charger_detect_ind_type
 * An enum for the type of charger, see
 * CCP_CHARGER_DETECT_IND_TYPE.
 *
 *   * \c charger_dp_millivolts
 * The voltage (in mV) measured on D+ of a proprietary charger.
 * This measurement is taken when the charger is connected,
 * therefore it won't change until a charger is plugged/
 * unplugged. This will only be non-zero if charger_detect_ind_type
 * is CCP_CHARGER_DETECT_IND_TYPE_USB_NON_COMPLIANT or
 * CCP_CHARGER_DETECT_IND_TYPE_USB_FLOATING.
 *
 *   * \c charger_dm_millivolts
 * The voltage (in mV) measured on D- of a proprietary charger.
 * This measurement is taken when the charger is connected,
 * therefore it won't change until a charger is plugged/
 * unplugged. This will only be non-zero if charger_detect_ind_type
 * is CCP_CHARGER_DETECT_IND_TYPE_USB_NON_COMPLIANT or
 * CCP_CHARGER_DETECT_IND_TYPE_USB_FLOATING.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint16__uint16__uint16(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 charger_detect_ind_type,
        uint16 charger_dp_millivolts,
        uint16 charger_dm_millivolts);

/**
 * Send CCP PDU charger_detect_ind.
 *
 * DIR FROM_CURATOR
 * Sent by curator to provide a subsystem with information
 * about the charger.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c charger_detect_ind_type
 * An enum for the type of charger, see
 * CCP_CHARGER_DETECT_IND_TYPE.
 *
 *   * \c charger_dp_millivolts
 * The voltage (in mV) measured on D+ of a proprietary charger.
 * This measurement is taken when the charger is connected,
 * therefore it won't change until a charger is plugged/
 * unplugged. This will only be non-zero if charger_detect_ind_type
 * is CCP_CHARGER_DETECT_IND_TYPE_USB_NON_COMPLIANT or
 * CCP_CHARGER_DETECT_IND_TYPE_USB_FLOATING.
 *
 *   * \c charger_dm_millivolts
 * The voltage (in mV) measured on D- of a proprietary charger.
 * This measurement is taken when the charger is connected,
 * therefore it won't change until a charger is plugged/
 * unplugged. This will only be non-zero if charger_detect_ind_type
 * is CCP_CHARGER_DETECT_IND_TYPE_USB_NON_COMPLIANT or
 * CCP_CHARGER_DETECT_IND_TYPE_USB_FLOATING.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_charger_detect_ind(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_uint16__uint16__uint16(CCP_SIGNAL_ID_CHARGER_DETECT_IND, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU charger_detect_ind.
 *
 * DIR FROM_CURATOR
 * Sent by curator to provide a subsystem with information
 * about the charger.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c charger_detect_ind_type
 * An enum for the type of charger, see
 * CCP_CHARGER_DETECT_IND_TYPE.
 *
 *   * \c charger_dp_millivolts
 * The voltage (in mV) measured on D+ of a proprietary charger.
 * This measurement is taken when the charger is connected,
 * therefore it won't change until a charger is plugged/
 * unplugged. This will only be non-zero if charger_detect_ind_type
 * is CCP_CHARGER_DETECT_IND_TYPE_USB_NON_COMPLIANT or
 * CCP_CHARGER_DETECT_IND_TYPE_USB_FLOATING.
 *
 *   * \c charger_dm_millivolts
 * The voltage (in mV) measured on D- of a proprietary charger.
 * This measurement is taken when the charger is connected,
 * therefore it won't change until a charger is plugged/
 * unplugged. This will only be non-zero if charger_detect_ind_type
 * is CCP_CHARGER_DETECT_IND_TYPE_USB_NON_COMPLIANT or
 * CCP_CHARGER_DETECT_IND_TYPE_USB_FLOATING.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint16__uint16__uint16(
        const uint16 *pdu, uint16 pdu_len_words,
        uint16 *charger_detect_ind_type,
        uint16 *charger_dp_millivolts,
        uint16 *charger_dm_millivolts);

/**
 * Analyse received CCP PDU charger_detect_ind.
 *
 * DIR FROM_CURATOR
 * Sent by curator to provide a subsystem with information
 * about the charger.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c charger_detect_ind_type
 * An enum for the type of charger, see
 * CCP_CHARGER_DETECT_IND_TYPE.
 *
 *   * \c charger_dp_millivolts
 * The voltage (in mV) measured on D+ of a proprietary charger.
 * This measurement is taken when the charger is connected,
 * therefore it won't change until a charger is plugged/
 * unplugged. This will only be non-zero if charger_detect_ind_type
 * is CCP_CHARGER_DETECT_IND_TYPE_USB_NON_COMPLIANT or
 * CCP_CHARGER_DETECT_IND_TYPE_USB_FLOATING.
 *
 *   * \c charger_dm_millivolts
 * The voltage (in mV) measured on D- of a proprietary charger.
 * This measurement is taken when the charger is connected,
 * therefore it won't change until a charger is plugged/
 * unplugged. This will only be non-zero if charger_detect_ind_type
 * is CCP_CHARGER_DETECT_IND_TYPE_USB_NON_COMPLIANT or
 * CCP_CHARGER_DETECT_IND_TYPE_USB_FLOATING.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_charger_detect_ind(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_uint16__uint16__uint16(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU hostio_usb_ext_token_handshakes_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to configure how to respond to Protocol Extension Tokens
 * (including LPM requests).
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subpid
 * Ext token SubPID. Requests for invalid SubPIDs will be ignored.
 *
 *   * \c response
 * Response.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint8__CCP_HOSTIO_USB_EXT_TOKEN_HANDSHAKE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint8 subpid,
        CCP_HOSTIO_USB_EXT_TOKEN_HANDSHAKE response);

/**
 * Send CCP PDU hostio_usb_ext_token_handshakes_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to configure how to respond to Protocol Extension Tokens
 * (including LPM requests).
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subpid
 * Ext token SubPID. Requests for invalid SubPIDs will be ignored.
 *
 *   * \c response
 * Response.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_hostio_usb_ext_token_handshakes_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint8__CCP_HOSTIO_USB_EXT_TOKEN_HANDSHAKE(CCP_SIGNAL_ID_HOSTIO_USB_EXT_TOKEN_HANDSHAKES_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU hostio_usb_ext_token_handshakes_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to configure how to respond to Protocol Extension Tokens
 * (including LPM requests).
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subpid
 * Ext token SubPID. Requests for invalid SubPIDs will be ignored.
 *
 *   * \c response
 * Response.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint8__CCP_HOSTIO_USB_EXT_TOKEN_HANDSHAKE(
        const uint16 *pdu, uint16 pdu_len_words,
        uint8 *subpid,
        CCP_HOSTIO_USB_EXT_TOKEN_HANDSHAKE *response);

/**
 * Analyse received CCP PDU hostio_usb_ext_token_handshakes_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to configure how to respond to Protocol Extension Tokens
 * (including LPM requests).
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subpid
 * Ext token SubPID. Requests for invalid SubPIDs will be ignored.
 *
 *   * \c response
 * Response.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_hostio_usb_ext_token_handshakes_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint8__CCP_HOSTIO_USB_EXT_TOKEN_HANDSHAKE(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU register_local_fileserver_req.
 *
 * DIR TO_CURATOR
 * Sent to curator to indicate that the sending subsystem has a
 * local fileserver that should be used for local file requests
 * in preference to the curator one (if it exists).
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_register_local_fileserver_req(arg1) \
    ccp_send_pdu_NOARGS(CCP_SIGNAL_ID_REGISTER_LOCAL_FILESERVER_REQ, arg1)

/**
 * Analyse received CCP PDU register_local_fileserver_req.
 *
 * DIR TO_CURATOR
 * Sent to curator to indicate that the sending subsystem has a
 * local fileserver that should be used for local file requests
 * in preference to the curator one (if it exists).
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_register_local_fileserver_req(arg1, arg2) \
    ccp_recv_pdu_NOARGS(arg1, arg2)


/**
 * Send CCP PDU audio_pwm_pio_info.
 *
 * DIR TO_CURATOR
 * Sent to curator with information about the 2 PIOs needed
 * for the PWM audio outputs.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c audio_pwm_power_pio_number
 * The PIO number of the PWM external amp power pin.
 *
 *   * \c audio_pwm_power_pio_low_active
 * 0 means active high, 1 means active low.
 *
 *   * \c audio_pwm_assert_pio_number
 * The PIO number of the PWM external amp assert pin.
 *
 *   * \c audio_pwm_assert_pio_low_active
 * 0 means active high, 1 means active low.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint7__Bit__uint7__Bit(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint7 audio_pwm_power_pio_number,
        Bit audio_pwm_power_pio_low_active,
        uint7 audio_pwm_assert_pio_number,
        Bit audio_pwm_assert_pio_low_active);

/**
 * Send CCP PDU audio_pwm_pio_info.
 *
 * DIR TO_CURATOR
 * Sent to curator with information about the 2 PIOs needed
 * for the PWM audio outputs.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c audio_pwm_power_pio_number
 * The PIO number of the PWM external amp power pin.
 *
 *   * \c audio_pwm_power_pio_low_active
 * 0 means active high, 1 means active low.
 *
 *   * \c audio_pwm_assert_pio_number
 * The PIO number of the PWM external amp assert pin.
 *
 *   * \c audio_pwm_assert_pio_low_active
 * 0 means active high, 1 means active low.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_audio_pwm_pio_info(arg1, arg2, arg3, arg4, arg5) \
    ccp_send_pdu_uint7__Bit__uint7__Bit(CCP_SIGNAL_ID_AUDIO_PWM_PIO_INFO, arg1, arg2, arg3, arg4, arg5)

/**
 * Analyse received CCP PDU audio_pwm_pio_info.
 *
 * DIR TO_CURATOR
 * Sent to curator with information about the 2 PIOs needed
 * for the PWM audio outputs.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c audio_pwm_power_pio_number
 * The PIO number of the PWM external amp power pin.
 *
 *   * \c audio_pwm_power_pio_low_active
 * 0 means active high, 1 means active low.
 *
 *   * \c audio_pwm_assert_pio_number
 * The PIO number of the PWM external amp assert pin.
 *
 *   * \c audio_pwm_assert_pio_low_active
 * 0 means active high, 1 means active low.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint7__Bit__uint7__Bit(
        const uint16 *pdu, uint16 pdu_len_words,
        uint7 *audio_pwm_power_pio_number,
        Bit *audio_pwm_power_pio_low_active,
        uint7 *audio_pwm_assert_pio_number,
        Bit *audio_pwm_assert_pio_low_active);

/**
 * Analyse received CCP PDU audio_pwm_pio_info.
 *
 * DIR TO_CURATOR
 * Sent to curator with information about the 2 PIOs needed
 * for the PWM audio outputs.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c audio_pwm_power_pio_number
 * The PIO number of the PWM external amp power pin.
 *
 *   * \c audio_pwm_power_pio_low_active
 * 0 means active high, 1 means active low.
 *
 *   * \c audio_pwm_assert_pio_number
 * The PIO number of the PWM external amp assert pin.
 *
 *   * \c audio_pwm_assert_pio_low_active
 * 0 means active high, 1 means active low.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_audio_pwm_pio_info(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_recv_pdu_uint7__Bit__uint7__Bit(arg1, arg2, arg3, arg4, arg5, arg6)


/**
 * Send CCP PDU audio_pwm_mute.
 *
 * DIR TO_CURATOR
 * Sent to curator to mute/unmute the PWM audio outputs.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c audio_pwm_mute
 * If not zero Curator will mute PWM outputs, if zero Curator will
 * unmute PWM outputs.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_audio_pwm_mute(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_AUDIO_PWM_MUTE, arg1, arg2)

/**
 * Analyse received CCP PDU audio_pwm_mute.
 *
 * DIR TO_CURATOR
 * Sent to curator to mute/unmute the PWM audio outputs.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c audio_pwm_mute
 * If not zero Curator will mute PWM outputs, if zero Curator will
 * unmute PWM outputs.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_audio_pwm_mute(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU audio_pwm_event_ind.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem to report an Audio PWM event.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c audio_pwm_events
 * A report of which event has occurred, there will only ever
 * be 1 event reported as the message will be sent straight away.
 * See the type CCP_AUDIO_PWM_EVENT_TYPE.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_AUDIO_PWM_EVENT_TYPE(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_AUDIO_PWM_EVENT_TYPE audio_pwm_events);

/**
 * Send CCP PDU audio_pwm_event_ind.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem to report an Audio PWM event.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c audio_pwm_events
 * A report of which event has occurred, there will only ever
 * be 1 event reported as the message will be sent straight away.
 * See the type CCP_AUDIO_PWM_EVENT_TYPE.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_audio_pwm_event_ind(arg1, arg2) \
    ccp_send_pdu_CCP_AUDIO_PWM_EVENT_TYPE(CCP_SIGNAL_ID_AUDIO_PWM_EVENT_IND, arg1, arg2)

/**
 * Analyse received CCP PDU audio_pwm_event_ind.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem to report an Audio PWM event.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c audio_pwm_events
 * A report of which event has occurred, there will only ever
 * be 1 event reported as the message will be sent straight away.
 * See the type CCP_AUDIO_PWM_EVENT_TYPE.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_AUDIO_PWM_EVENT_TYPE(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_AUDIO_PWM_EVENT_TYPE *audio_pwm_events);

/**
 * Analyse received CCP PDU audio_pwm_event_ind.
 *
 * DIR FROM_CURATOR
 * Sent to a subsystem to report an Audio PWM event.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c audio_pwm_events
 * A report of which event has occurred, there will only ever
 * be 1 event reported as the message will be sent straight away.
 * See the type CCP_AUDIO_PWM_EVENT_TYPE.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_audio_pwm_event_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_AUDIO_PWM_EVENT_TYPE(arg1, arg2, arg3)


/**
 * Send CCP PDU mclk_pll_adjust_freq_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator asking to make a small change to the MCLK PLL
 * frequency.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c mclk_pll_adjustment
 * The adjustment to be made to the mclk pll frequency. The frequency
 * will be moved up or down depending upon if this value is positive
 * or negative. This value is the number of units to move the
 * frequency up/down.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_int16(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                        int16 mclk_pll_adjustment);

/**
 * Send CCP PDU mclk_pll_adjust_freq_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator asking to make a small change to the MCLK PLL
 * frequency.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c mclk_pll_adjustment
 * The adjustment to be made to the mclk pll frequency. The frequency
 * will be moved up or down depending upon if this value is positive
 * or negative. This value is the number of units to move the
 * frequency up/down.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_mclk_pll_adjust_freq_req(arg1, arg2) \
    ccp_send_pdu_int16(CCP_SIGNAL_ID_MCLK_PLL_ADJUST_FREQ_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU mclk_pll_adjust_freq_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator asking to make a small change to the MCLK PLL
 * frequency.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c mclk_pll_adjustment
 * The adjustment to be made to the mclk pll frequency. The frequency
 * will be moved up or down depending upon if this value is positive
 * or negative. This value is the number of units to move the
 * frequency up/down.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_int16(const uint16 *pdu, uint16 pdu_len_words,
                        int16 *mclk_pll_adjustment);

/**
 * Analyse received CCP PDU mclk_pll_adjust_freq_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator asking to make a small change to the MCLK PLL
 * frequency.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c mclk_pll_adjustment
 * The adjustment to be made to the mclk pll frequency. The frequency
 * will be moved up or down depending upon if this value is positive
 * or negative. This value is the number of units to move the
 * frequency up/down.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_mclk_pll_adjust_freq_req(arg1, arg2, arg3) \
    ccp_recv_pdu_int16(arg1, arg2, arg3)


/**
 * Send CCP PDU measure_freq_on_pio_req.
 *
 * DIR TO_CURATOR
 * Request to measure the frequency on an arbitrary PIO. This will
 * measure frequencies up to 100MHz with less than 1% error. The
 * time range is 0ms-100ms, if the time is set to 0ms a default
 * of 100us will be used.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c measure_freq_pio
 * The PIO number that the input frequency is to be measured on.
 *
 *   * \c measure_freq_time_ms
 * The number of milliseconds to measure the frequency for.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_measure_freq_on_pio_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID_MEASURE_FREQ_ON_PIO_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU measure_freq_on_pio_req.
 *
 * DIR TO_CURATOR
 * Request to measure the frequency on an arbitrary PIO. This will
 * measure frequencies up to 100MHz with less than 1% error. The
 * time range is 0ms-100ms, if the time is set to 0ms a default
 * of 100us will be used.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c measure_freq_pio
 * The PIO number that the input frequency is to be measured on.
 *
 *   * \c measure_freq_time_ms
 * The number of milliseconds to measure the frequency for.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_measure_freq_on_pio_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU measure_freq_on_pio_rsp.
 *
 * DIR FROM_CURATOR
 * The response to a PIO frequency measurement request.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c measure_freq_hz
 * The frequency that has been measured on the chosen PIO in Hz.
 *
 *   * \c measure_freq_success
 * If 0, the measurement has failed. Otherwise it has been successful.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint32__uint16(CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
                                 uint32 measure_freq_hz,
                                 uint16 measure_freq_success);

/**
 * Send CCP PDU measure_freq_on_pio_rsp.
 *
 * DIR FROM_CURATOR
 * The response to a PIO frequency measurement request.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c measure_freq_hz
 * The frequency that has been measured on the chosen PIO in Hz.
 *
 *   * \c measure_freq_success
 * If 0, the measurement has failed. Otherwise it has been successful.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_measure_freq_on_pio_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_uint32__uint16(CCP_SIGNAL_ID_MEASURE_FREQ_ON_PIO_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU measure_freq_on_pio_rsp.
 *
 * DIR FROM_CURATOR
 * The response to a PIO frequency measurement request.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c measure_freq_hz
 * The frequency that has been measured on the chosen PIO in Hz.
 *
 *   * \c measure_freq_success
 * If 0, the measurement has failed. Otherwise it has been successful.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint32__uint16(const uint16 *pdu, uint16 pdu_len_words,
                                 uint32 *measure_freq_hz,
                                 uint16 *measure_freq_success);

/**
 * Analyse received CCP PDU measure_freq_on_pio_rsp.
 *
 * DIR FROM_CURATOR
 * The response to a PIO frequency measurement request.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c measure_freq_hz
 * The frequency that has been measured on the chosen PIO in Hz.
 *
 *   * \c measure_freq_success
 * If 0, the measurement has failed. Otherwise it has been successful.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_measure_freq_on_pio_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint32__uint16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU event_register_req.
 *
 * DIR TO_CURATOR
 * Sent to the curator to register interest in a type of event.
 * The curator will either send REGISTERED_EVENT_IND PDUs or
 * a separate specific CCP PDU for that type to
 * the subsystem sending this request.
 * The enable parameter allows a subsytem to register or
 * de-register its interest in every change of the event.
 * When the immediate parameter is TRUE the subsytem will
 * receive an immediate REGISTERED_EVENT_IND PDU or specific
 * CCP PDU giving the current state or value of the requested
 * event type.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c event_type
 * Type of event this registration is for
 *
 *   * \c enable
 * Enable or clear the registration of this subsystem
 * for this event type
 *
 *   * \c immediate
 * Return an immediate indication of the current value.
 * This is only relevant for certain event types
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_REGISTER_EVENT_TYPE__uint8__uint8(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        CCP_REGISTER_EVENT_TYPE event_type,
        uint8 enable,
        uint8 immediate);

/**
 * Send CCP PDU event_register_req.
 *
 * DIR TO_CURATOR
 * Sent to the curator to register interest in a type of event.
 * The curator will either send REGISTERED_EVENT_IND PDUs or
 * a separate specific CCP PDU for that type to
 * the subsystem sending this request.
 * The enable parameter allows a subsytem to register or
 * de-register its interest in every change of the event.
 * When the immediate parameter is TRUE the subsytem will
 * receive an immediate REGISTERED_EVENT_IND PDU or specific
 * CCP PDU giving the current state or value of the requested
 * event type.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c event_type
 * Type of event this registration is for
 *
 *   * \c enable
 * Enable or clear the registration of this subsystem
 * for this event type
 *
 *   * \c immediate
 * Return an immediate indication of the current value.
 * This is only relevant for certain event types
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_event_register_req(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_REGISTER_EVENT_TYPE__uint8__uint8(CCP_SIGNAL_ID_EVENT_REGISTER_REQ, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU event_register_req.
 *
 * DIR TO_CURATOR
 * Sent to the curator to register interest in a type of event.
 * The curator will either send REGISTERED_EVENT_IND PDUs or
 * a separate specific CCP PDU for that type to
 * the subsystem sending this request.
 * The enable parameter allows a subsytem to register or
 * de-register its interest in every change of the event.
 * When the immediate parameter is TRUE the subsytem will
 * receive an immediate REGISTERED_EVENT_IND PDU or specific
 * CCP PDU giving the current state or value of the requested
 * event type.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c event_type
 * Type of event this registration is for
 *
 *   * \c enable
 * Enable or clear the registration of this subsystem
 * for this event type
 *
 *   * \c immediate
 * Return an immediate indication of the current value.
 * This is only relevant for certain event types
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_REGISTER_EVENT_TYPE__uint8__uint8(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_REGISTER_EVENT_TYPE *event_type,
        uint8 *enable,
        uint8 *immediate);

/**
 * Analyse received CCP PDU event_register_req.
 *
 * DIR TO_CURATOR
 * Sent to the curator to register interest in a type of event.
 * The curator will either send REGISTERED_EVENT_IND PDUs or
 * a separate specific CCP PDU for that type to
 * the subsystem sending this request.
 * The enable parameter allows a subsytem to register or
 * de-register its interest in every change of the event.
 * When the immediate parameter is TRUE the subsytem will
 * receive an immediate REGISTERED_EVENT_IND PDU or specific
 * CCP PDU giving the current state or value of the requested
 * event type.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c event_type
 * Type of event this registration is for
 *
 *   * \c enable
 * Enable or clear the registration of this subsystem
 * for this event type
 *
 *   * \c immediate
 * Return an immediate indication of the current value.
 * This is only relevant for certain event types
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_event_register_req(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_CCP_REGISTER_EVENT_TYPE__uint8__uint8(arg1, arg2, arg3, arg4, arg5)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_REGISTERED_EVENT_IND_PAYLOAD \
    CCP_REGISTERED_EVENT_IND_PAYLOAD_WORD_OFFSET

/**
 * Send CCP PDU registered_event_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the curator to subsytems that have registered interest
 * in a type of event. Sent either in response to the
 * EVENT_REGISTER_REQ PDU or on each change in the event
 * value or state.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c event_type
 * Type of event this indication is for
 *
 *   * Non-parameter argument \c payload:
 * Optional extra values to the indication
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_REGISTERED_EVENT_IND_PAYLOAD_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_CCP_REGISTER_EVENT_TYPE__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        CCP_REGISTER_EVENT_TYPE event_type);

/**
 * Send CCP PDU registered_event_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the curator to subsytems that have registered interest
 * in a type of event. Sent either in response to the
 * EVENT_REGISTER_REQ PDU or on each change in the event
 * value or state.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c event_type
 * Type of event this indication is for
 *
 *   * Non-parameter argument \c payload:
 * Optional extra values to the indication
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_REGISTERED_EVENT_IND_PAYLOAD_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_registered_event_ind(arg1, arg2, arg3, arg4) \
    ccp_send_pdu_CCP_REGISTER_EVENT_TYPE__VARLEN(CCP_SIGNAL_ID_REGISTERED_EVENT_IND, arg1, arg2, arg3, arg4)

/**
 * Analyse received CCP PDU registered_event_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the curator to subsytems that have registered interest
 * in a type of event. Sent either in response to the
 * EVENT_REGISTER_REQ PDU or on each change in the event
 * value or state.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c event_type
 * Type of event this indication is for
 *
 *   Non-parameter argument \c payload:
 * Optional extra values to the indication
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_REGISTERED_EVENT_IND_PAYLOAD_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_CCP_REGISTER_EVENT_TYPE__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        CCP_REGISTER_EVENT_TYPE *event_type);

/**
 * Analyse received CCP PDU registered_event_ind.
 *
 * DIR FROM_CURATOR
 * Sent by the curator to subsytems that have registered interest
 * in a type of event. Sent either in response to the
 * EVENT_REGISTER_REQ PDU or on each change in the event
 * value or state.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c event_type
 * Type of event this indication is for
 *
 *   Non-parameter argument \c payload:
 * Optional extra values to the indication
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_REGISTERED_EVENT_IND_PAYLOAD_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_registered_event_ind(arg1, arg2, arg3) \
    ccp_recv_pdu_CCP_REGISTER_EVENT_TYPE__VARLEN(arg1, arg2, arg3)


/**
 * Send CCP PDU siflash_vectors_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a particular serial flash chip
 * configuration parameters.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_siflash_vectors_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint8__uint8(CCP_SIGNAL_ID_SIFLASH_VECTORS_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU siflash_vectors_req.
 *
 * DIR TO_CURATOR
 * Sent by a subsystem to request a particular serial flash chip
 * configuration parameters.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_siflash_vectors_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint8__uint8(arg1, arg2, arg3, arg4)


/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_SIFLASH_VECTORS_RSP_CONFIG_VECTORS \
    CCP_SIFLASH_VECTORS_RSP_CONFIG_VECTORS_WORD_OFFSET

/**
 * Send CCP PDU siflash_vectors_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to return to a subsystem the serial flash
 * device configuration parameters it has requested.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 *   * \c result
 * The result of the request.
 *
 *   * Non-parameter argument \c config_vectors:
 * Three configuration vectors one following after another:
 * spi_config_vector:     16 uint16 words
 * non_ddr_config_vector: 16 uint16 words
 * normal_config_vector:  16 uint16 words
 * 
 * Each vector is an array of octets with command(s) packed
 * into uint16 words in big-endian and padded at the end.
 * 
 * A command vector can be described by the following structure:
 * 
 * struct cmd_vector {
 * uint8 total_commands_count;
 * struct cmd
 * {
 * uint8 cmd_length;
 * uint8 cmd[cmd_length];
 * } cmds[total_commands_count];
 * }
 * 
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SIFLASH_VECTORS_RSP_CONFIG_VECTORS_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
bool ccp_send_pdu_uint8__uint8__CCP_SIFLASH_RESULT__VARLEN(
        CCP_SIGNAL_ID ccp_signal_id, uint16 dest,
        uint16 *pdu, uint16 pdu_len_words,
        uint8 subsystem,
        uint8 bank,
        CCP_SIFLASH_RESULT result);

/**
 * Send CCP PDU siflash_vectors_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to return to a subsystem the serial flash
 * device configuration parameters it has requested.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 *   * \c result
 * The result of the request.
 *
 *   * Non-parameter argument \c config_vectors:
 * Three configuration vectors one following after another:
 * spi_config_vector:     16 uint16 words
 * non_ddr_config_vector: 16 uint16 words
 * normal_config_vector:  16 uint16 words
 * 
 * Each vector is an array of octets with command(s) packed
 * into uint16 words in big-endian and padded at the end.
 * 
 * A command vector can be described by the following structure:
 * 
 * struct cmd_vector {
 * uint8 total_commands_count;
 * struct cmd
 * {
 * uint8 cmd_length;
 * uint8 cmd[cmd_length];
 * } cmds[total_commands_count];
 * }
 * 
 * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \c CCP_SIFLASH_VECTORS_RSP_CONFIG_VECTORS_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_siflash_vectors_rsp(arg1, arg2, arg3, arg4, arg5, arg6) \
    ccp_send_pdu_uint8__uint8__CCP_SIFLASH_RESULT__VARLEN(CCP_SIGNAL_ID_SIFLASH_VECTORS_RSP, arg1, arg2, arg3, arg4, arg5, arg6)

/**
 * Analyse received CCP PDU siflash_vectors_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to return to a subsystem the serial flash
 * device configuration parameters it has requested.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 *   * \c result
 * The result of the request.
 *
 *   Non-parameter argument \c config_vectors:
 * Three configuration vectors one following after another:
 * spi_config_vector:     16 uint16 words
 * non_ddr_config_vector: 16 uint16 words
 * normal_config_vector:  16 uint16 words
 * 
 * Each vector is an array of octets with command(s) packed
 * into uint16 words in big-endian and padded at the end.
 * 
 * A command vector can be described by the following structure:
 * 
 * struct cmd_vector {
 * uint8 total_commands_count;
 * struct cmd
 * {
 * uint8 cmd_length;
 * uint8 cmd[cmd_length];
 * } cmds[total_commands_count];
 * }
 * 
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SIFLASH_VECTORS_RSP_CONFIG_VECTORS_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
bool ccp_recv_pdu_uint8__uint8__CCP_SIFLASH_RESULT__VARLEN(
        const uint16 *pdu, uint16 pdu_len_words,
        uint8 *subsystem,
        uint8 *bank,
        CCP_SIFLASH_RESULT *result);

/**
 * Analyse received CCP PDU siflash_vectors_rsp.
 *
 * DIR FROM_CURATOR
 * Sent by the Curator to return to a subsystem the serial flash
 * device configuration parameters it has requested.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c subsystem
 * Subsystem ID to return serial flash configuration for. Some
 * subsystems might be intereset in other subsystem's serial flash
 * configuration (e.g. Apps for doing Audio DFU).
 *
 *   * \c bank
 * Particular serial flash bank attached to target subsystem,
 * starting from "0". Some subsystems have more than one
 * serial flash attached (e.g. Apps).
 *
 *   * \c result
 * The result of the request.
 *
 *   Non-parameter argument \c config_vectors:
 * Three configuration vectors one following after another:
 * spi_config_vector:     16 uint16 words
 * non_ddr_config_vector: 16 uint16 words
 * normal_config_vector:  16 uint16 words
 * 
 * Each vector is an array of octets with command(s) packed
 * into uint16 words in big-endian and padded at the end.
 * 
 * A command vector can be described by the following structure:
 * 
 * struct cmd_vector {
 * uint8 total_commands_count;
 * struct cmd
 * {
 * uint8 cmd_length;
 * uint8 cmd[cmd_length];
 * } cmds[total_commands_count];
 * }
 * 
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \c CCP_SIFLASH_VECTORS_RSP_CONFIG_VECTORS_WORD_OFFSET
 * to find the argument in the PDU and its length.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_siflash_vectors_rsp(arg1, arg2, arg3, arg4, arg5) \
    ccp_recv_pdu_uint8__uint8__CCP_SIFLASH_RESULT__VARLEN(arg1, arg2, arg3, arg4, arg5)


/**
 * Send CCP PDU apps_cpu_clock_ctrl_variant_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator requesting to change the Apps CPU clock.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c cpu_clock
 * Values defined as follows:
 * 0 - Select XTAL clock control variant
 * 1 - Select PLL clock control variant
 * 255 - Query the current clock frequency only. Does not alter the
 * clock control variant.
 * Other values are reserved for future use.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_apps_cpu_clock_ctrl_variant_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_APPS_CPU_CLOCK_CTRL_VARIANT_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU apps_cpu_clock_ctrl_variant_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator requesting to change the Apps CPU clock.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c cpu_clock
 * Values defined as follows:
 * 0 - Select XTAL clock control variant
 * 1 - Select PLL clock control variant
 * 255 - Query the current clock frequency only. Does not alter the
 * clock control variant.
 * Other values are reserved for future use.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_apps_cpu_clock_ctrl_variant_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU apps_cpu_clock_ctrl_variant_rsp.
 *
 * DIR FROM_CURATOR
 * Confirmation the APPS clock control variant request has been
 * completed.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c cpu_frequency_mhz
 * Contains the current CPU clock frequency in MHz
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_apps_cpu_clock_ctrl_variant_rsp(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_APPS_CPU_CLOCK_CTRL_VARIANT_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU apps_cpu_clock_ctrl_variant_rsp.
 *
 * DIR FROM_CURATOR
 * Confirmation the APPS clock control variant request has been
 * completed.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c cpu_frequency_mhz
 * Contains the current CPU clock frequency in MHz
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_apps_cpu_clock_ctrl_variant_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU audio_cpu_eng_clock_ctrl_variant_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator requesting to change the Audio CPU and/or Engine
 * clocks.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c cpu_clock
 * Values defined as follows:
 * 0 - Select XTAL clock control variant
 * 1 - Select PLL clock control variant
 * 2 - Select PLL_TURBO control clock variant
 * 3 - Select AOV XTAL/2 control clock variant
 * 4 - Select AOV XTAL/4 control clock variant
 * 5 - Select AOV XTAL/8 control clock variant
 * 6 - Select AOV XTAL/16 control clock variant
 * 255 - Query the current clock frequency only. Does not alter the
 * clock control variant.
 * Other values are reserved for future use.
 *
 *   * \c engine_clock
 * Values defined as follows:
 * 0 - Select XTAL clock control variant
 * 1 - Select AOV XTAL/2 clock control variant
 * 2 - Select AOV XTAL/4 clock control variant
 * 3 - Select AOV XTAL/8 clock control variant
 * 4 - Select AOV XTAL/16 clock control variant
 * 254 - Turn clock off
 * 255 - Query the current clock frequency only. Does not alter the
 * clock control variant.
 * Other values are reserved for future use.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_audio_cpu_eng_clock_ctrl_variant_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID_AUDIO_CPU_ENG_CLOCK_CTRL_VARIANT_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU audio_cpu_eng_clock_ctrl_variant_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator requesting to change the Audio CPU and/or Engine
 * clocks.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c cpu_clock
 * Values defined as follows:
 * 0 - Select XTAL clock control variant
 * 1 - Select PLL clock control variant
 * 2 - Select PLL_TURBO control clock variant
 * 3 - Select AOV XTAL/2 control clock variant
 * 4 - Select AOV XTAL/4 control clock variant
 * 5 - Select AOV XTAL/8 control clock variant
 * 6 - Select AOV XTAL/16 control clock variant
 * 255 - Query the current clock frequency only. Does not alter the
 * clock control variant.
 * Other values are reserved for future use.
 *
 *   * \c engine_clock
 * Values defined as follows:
 * 0 - Select XTAL clock control variant
 * 1 - Select AOV XTAL/2 clock control variant
 * 2 - Select AOV XTAL/4 clock control variant
 * 3 - Select AOV XTAL/8 clock control variant
 * 4 - Select AOV XTAL/16 clock control variant
 * 254 - Turn clock off
 * 255 - Query the current clock frequency only. Does not alter the
 * clock control variant.
 * Other values are reserved for future use.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_audio_cpu_eng_clock_ctrl_variant_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU audio_cpu_eng_clock_ctrl_variant_rsp.
 *
 * DIR FROM_CURATOR
 * Confirmation the Audio and/or Engine clock control variant request
 * has been completed.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c cpu_frequency_mhz
 * Contains the current CPU clock frequency in MHz
 *
 *   * \c eng_frequency_mhz
 * Contains the current Engine clock frequency in MHz
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_audio_cpu_eng_clock_ctrl_variant_rsp(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID_AUDIO_CPU_ENG_CLOCK_CTRL_VARIANT_RSP, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU audio_cpu_eng_clock_ctrl_variant_rsp.
 *
 * DIR FROM_CURATOR
 * Confirmation the Audio and/or Engine clock control variant request
 * has been completed.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c cpu_frequency_mhz
 * Contains the current CPU clock frequency in MHz
 *
 *   * \c eng_frequency_mhz
 * Contains the current Engine clock frequency in MHz
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_audio_cpu_eng_clock_ctrl_variant_rsp(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16(arg1, arg2, arg3, arg4)


/**
 * Send CCP PDU audio_analogue_clock_control_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator requesting to enable or disable the Audio differential
 * analogue clock.
 * 
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c clock_enable
 * Values defined as follows:
 * 0 - disable the analogue clock
 * 1 - enable the analogue clock
 * Other values are reserved for future use.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_audio_analogue_clock_control_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_AUDIO_ANALOGUE_CLOCK_CONTROL_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU audio_analogue_clock_control_req.
 *
 * DIR TO_CURATOR
 * Sent to Curator requesting to enable or disable the Audio differential
 * analogue clock.
 * 
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c clock_enable
 * Values defined as follows:
 * 0 - disable the analogue clock
 * 1 - enable the analogue clock
 * Other values are reserved for future use.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_audio_analogue_clock_control_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU audio_analogue_clock_control_rsp.
 *
 * DIR FROM_CURATOR
 * Confirmation that the Audio differential analogue clock control
 * request has been completed.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c clock_status
 * Values defined as follows:
 * 0 - analogue clock currently disabled
 * 1 - analogue clock currently enabled
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_audio_analogue_clock_control_rsp(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_AUDIO_ANALOGUE_CLOCK_CONTROL_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU audio_analogue_clock_control_rsp.
 *
 * DIR FROM_CURATOR
 * Confirmation that the Audio differential analogue clock control
 * request has been completed.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c clock_status
 * Values defined as follows:
 * 0 - analogue clock currently disabled
 * 1 - analogue clock currently enabled
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_audio_analogue_clock_control_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU usb_device_attach_req.
 *
 * DIR TO_CURATOR
 * Tell Curator to raise the D+ pull-up to initiate enumeration, this
 * message should also be used to disable the pull-up when USB is
 * detached.
 * 
 * The main use case for this message is when VBUS is connected
 * to a PIO instead of VCHG. In this scenario Apps will need to tell Curator
 * when VBUS goes high/low.
 * 
 * Curator will not only raise the D+ pull-up, it will also turn on the
 * required USB hardware and disable deep sleep until we are suspended.
 * 
 * CCP_USB_DEVICE_ATTACH_RSP will be sent by Curator in response.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c enable
 * Tell Curator whether to enable (enable = 1) or disable (enable = 0) the
 * D+ pull-up.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_usb_device_attach_req(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_USB_DEVICE_ATTACH_REQ, arg1, arg2)

/**
 * Analyse received CCP PDU usb_device_attach_req.
 *
 * DIR TO_CURATOR
 * Tell Curator to raise the D+ pull-up to initiate enumeration, this
 * message should also be used to disable the pull-up when USB is
 * detached.
 * 
 * The main use case for this message is when VBUS is connected
 * to a PIO instead of VCHG. In this scenario Apps will need to tell Curator
 * when VBUS goes high/low.
 * 
 * Curator will not only raise the D+ pull-up, it will also turn on the
 * required USB hardware and disable deep sleep until we are suspended.
 * 
 * CCP_USB_DEVICE_ATTACH_RSP will be sent by Curator in response.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c enable
 * Tell Curator whether to enable (enable = 1) or disable (enable = 0) the
 * D+ pull-up.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_usb_device_attach_req(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU usb_device_attach_rsp.
 *
 * DIR FROM_CURATOR
 * Curator's response to CCP_USB_DEVICE_ATTACH_REQ to inform
 * the requesting subsystem if the request has been successful.
 * 
 * The request will fail in the following two cases:
 * 1. The Curator MIB key ChargerIsUsb is set to TRUE, in this
 * case USB attachment is controlled by Curator.
 * 2. If the USB device is in the wrong state to carry out the
 * req eg. if the device is attached and is asked to attach
 * again.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c success
 * success = 1 if the request has been successful, otherwise
 * success = 0.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_usb_device_attach_rsp(arg1, arg2) \
    ccp_send_pdu_uint16(CCP_SIGNAL_ID_USB_DEVICE_ATTACH_RSP, arg1, arg2)

/**
 * Analyse received CCP PDU usb_device_attach_rsp.
 *
 * DIR FROM_CURATOR
 * Curator's response to CCP_USB_DEVICE_ATTACH_REQ to inform
 * the requesting subsystem if the request has been successful.
 * 
 * The request will fail in the following two cases:
 * 1. The Curator MIB key ChargerIsUsb is set to TRUE, in this
 * case USB attachment is controlled by Curator.
 * 2. If the USB device is in the wrong state to carry out the
 * req eg. if the device is attached and is asked to attach
 * again.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c success
 * success = 1 if the request has been successful, otherwise
 * success = 0.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_usb_device_attach_rsp(arg1, arg2, arg3) \
    ccp_recv_pdu_uint16(arg1, arg2, arg3)


/**
 * Send CCP PDU additional_current_load_req.
 *
 * DIR TO_CURATOR
 * Configure the chip to draw additional current.
 * The values supplied are programmed into a register to cause increased
 * current draw. How these values map to current is for the P0 application
 * to determine.
 * No response is sent for this.
 *
 * Arguments:
 *
 *   * \c dest
 * Destination ISP address on port \c ISP_PROTOCOL_CCP
 *
 *   * \c active_mode_current
 * The value to use when awake.
 *
 *   * \c deep_sleep_mode_current
 * The value to use when in deep sleep mode.
 *
 * \return \c TRUE if the PDU was successfully sent, else \c FALSE.
 */
#define ccp_send_pdu_additional_current_load_req(arg1, arg2, arg3) \
    ccp_send_pdu_uint16__uint16(CCP_SIGNAL_ID_ADDITIONAL_CURRENT_LOAD_REQ, arg1, arg2, arg3)

/**
 * Analyse received CCP PDU additional_current_load_req.
 *
 * DIR TO_CURATOR
 * Configure the chip to draw additional current.
 * The values supplied are programmed into a register to cause increased
 * current draw. How these values map to current is for the P0 application
 * to determine.
 * No response is sent for this.
 *
 * Arguments:
 *
 *   * \c pdu
 * The received PDU.
 *
 *   * \c pdu_len_words
 * The length of the received PDU in words.
 *
 *   * \c active_mode_current
 * The value to use when awake.
 *
 *   * \c deep_sleep_mode_current
 * The value to use when in deep sleep mode.
 *
 * \return \c TRUE if the PDU was successfully analysed, else \c FALSE.
 */
#define ccp_recv_pdu_additional_current_load_req(arg1, arg2, arg3, arg4) \
    ccp_recv_pdu_uint16__uint16(arg1, arg2, arg3, arg4)


#endif /* CCP_FUNCS_H */
