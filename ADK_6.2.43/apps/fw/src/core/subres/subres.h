/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 *
 * Main public header for the subsystem resource agent.
 */

#ifndef SUBRES_H
#define SUBRES_H

#include "hydra/hydra_types.h"
#include "hydra/hydra_macros.h"

#include "ccp/ccp.h"

/**
 * Operation referred to by callback.
 */
typedef enum SUBRES_OP
{
    /**
     * A response to a query about a resource has been received.
     */
    SUBRES_OP_QUERY_RESPONSE,
    /**
     * A resource has been successfully assigned.
     *
     * If there is data with the resource, it is passed within
     * the \c SUBRES_CALLBACK_DATA.  Note this is different from
     * the CCP PDUs which indicate the presence of data by a different
     * signal.
     */
    SUBRES_OP_RESOURCE_ASSIGNED,
    /**
     * A resource could not be successfully assigned.
     */
    SUBRES_OP_RESOURCE_UNAVAIL,
    /**
     * A resource has been successfully released.
     */
    SUBRES_OP_RESOURCE_RELEASED,
    /**
     * A resource could not be successfully released.
     *
     * As a valid resource release should always be honoured, \c subres
     * will usually panic when this happens.  Currently the only
     * exception is when the result code for the operation is
     * \c CCP_RESOURCE_RESULT_ALLOC_FAILURE, which is retriable.
     */
    SUBRES_OP_RESOURCE_NOT_RELEASED,
    /**
     * The grant of the resource is being revoked.
     *
     * This is mandatory, however the owner may perform tasks to clear
     * up before indicating it has ceased using the resource, in which
     * case it should return \c SUBRES_RETURN_PENDING from the
     * callback.
     *
     * After returning \c SUBRES_RETURN_SUCCESS or calling
     * \c subres_revoked() the client should not use the resource
     * any more.  However, it may call subres to request it again.
     * It should not do this until after the revocation.
     */
    SUBRES_OP_RESOURCE_REVOKE,
    /**
     * The revocation is complete.
     *
     * This indicates that \c globres has now completed the switch
     * of the resource ownership.  This is significant in the
     * case of resources such as PIO where the transfer of ownership
     * takes place in hardware: following this \c op, the local subsystem
     * may alter its local hardware registers (e.g. PIO direction and
     * levels) without these having an affect on the external hardware.
     */
    SUBRES_OP_RESOURCE_REVOKED,
    /**
     * The resource has been released by \c subres.
     *
     * This differs from \c SUBRES_OP_RESOURCE_RELEASED only in that
     * the request to release the resource came from a call to
     * \c subres_clear() to indicate that all resources from
     * the subsystem should be removed, rather than from the local
     * handler for the resource.
     */
    SUBRES_OP_RESOURCE_CLEARED,
    /**
     * A PIO resource has been successfully configured.
     *
     * This is an ancillary operation provided by subres primarily for
     * convenience of response handling
     */
    SUBRES_OP_PIO_CONFIGURED,
    /**
     * A PIO resource has been successfully acquired and configured
     */
    SUBRES_OP_PIO_ASSIGNED_AND_CONFIGURED,
    /**
     * A PIO resource configuration has been successfully queried.
     *
     * This is an ancillary operation provided by subres primarily for
     * convenience of response handling
     */
    SUBRES_OP_PIO_CONFIG_QUERIED,
    /**
     * A group operation (request, release or PIO configuration) has now
     * completed, successfully or otherwise.
     */
    SUBRES_OP_GROUP_COMPLETED
} SUBRES_OP;

/**
 * Result returned from a resource callback.
 */
typedef enum SUBRES_RETURN
{
    /** Operation was successful */
    SUBRES_RETURN_SUCCESS,
    /**
     * Operation failed.
     *
     * Currently this is not a useful return code: only revocation
     * requires action by the client, and it is mandatory, so must
     * either succeed or be pending.
     */
    SUBRES_RETURN_FAILURE,
    /**
     * Operation is pending.
     *
     * This is used when a "revoke" cannot be implemented immediately.
     * \c subres_revoked() will be called when the revocation
     * has finished.
     */
    SUBRES_RETURN_PENDING
} SUBRES_RETURN;

/**
 * Data passed to \c SUBRES_CALLBACK.
 */
typedef struct SUBRES_CALLBACK_DATA
{
    /**
     * Identifier for the resource referred to.
     */
    CCP_RESOURCE_ID res_id;
    /**
     * Operation being performed on \c res_id
     */
    SUBRES_OP op;
    /**
     * Variant data for different operations
     */
    union {
        /** \c SUBRES_OP_QUERY */
        struct {
            /** Status of the resource */
            CCP_RESOURCE_STATUS status;
            /** Bitmap of systems using the resource */
            uint16 subsystems;
        } query; /**< Data for query */
        /** \c SUBRES_OP_RESOURCE_ASSIGNED */
        struct {
            /**
             * Any flags associated with the resource response.
             * They are bitmask from \c CCP_RESOURCE_FLAGS.
             */
            uint16 flags;
            /**
             * Pointer to data returned by \c globres, \c NULL if none.
             */
            const uint16 *data_ptr;
            /**
             * Length in uint16's of data in \c data_ptr.
             */
            uint16 data_len;
        } assigned; /**< Data on resource assignment */
        /**
         * \c SUBRES_OP_RESOURCE_UNAVAIL
         */
        struct {
            /** Reason for failure of request */
            CCP_RESOURCE_RESULT result;
            /**
             * The resource that caused the failure.  This may
             * be a dependency of the requested resource.
             */
            CCP_RESOURCE_ID result_res_id;
            /**
             * The status of \c result_res_id, i.e. the resource
             * that caused the failure, not necessarily the requested
             * resource.
             */
            CCP_RESOURCE_STATUS status;
            /**
             * Bitmap of subsystems currently using the resource in \c
             * result_res_id.
             */
            uint16 subsystems;
        } unavail; /**< Data on failure of request*/
        /**
         * \c SUBRES_OP_RESOURCE_NOT_RELEASED
         */
        struct {
            /** Reason for failure of release */
            CCP_RESOURCE_RESULT result;
        } not_released; /**< Data on failure of release */
        /**
         * \c SUBRES_OP_GROUP_COMPLETED
         */
        struct {
            /** The operation that was being applied to the group */
            SUBRES_OP op;
        } group;
        /**
         * \c SUBRES_OP_PIO_CONFIG_QUERIED
         */
        struct {
            /** Array of configuration data. This should be freed by the
                callback. */
            CCP_PIO_KEY_VALUE_TYPE *data;
            /** Length of configuration data */
            uint16 len;
        } config_query;
    } ops; /**< Variant data for resource operations */
} SUBRES_CALLBACK_DATA;

/**
 * Callback used for returning information from \c globres.
 *
 * Currently, no element of the structure can usefully be modified by
 * the called function.  However, in order to retain forward
 * compatibility with the callback type if it should become useful for
 * the function to change a value in the structure it is left mutable.
 */
typedef SUBRES_RETURN (*SUBRES_CALLBACK)(SUBRES_CALLBACK_DATA *cdata);

/*@}*/


/**
 * @name Requests for resources and to release resources
 */
/*@{*/

/**
 * Query status of a resource.
 *
 * \param res_id is the resource whose status is to be queried.
 * \param callback will be used for reporting the result.  In this
 *        case it is \e not retained for future use by \c subres.
 *
 * \return \c TRUE if the query could be sent, else \c FALSE, in which
 * case no subsequent callback will be made.  A failure here only
 * indicates a failure of messaging.
 */
extern bool subres_query(
    CCP_RESOURCE_ID res_id,
    SUBRES_CALLBACK callback);

/**
 * Request use of a resource.
 *
 * If the function returns \c TRUE, the request for resource \c res_id
 * is forwarded to \c globres and \c callback called to indicate the
 * result.
 *
 * This function is equivalent to \c subres_data_request()
 * with flags zero and no data; \c subres automatically optimises
 * calls to \c subres_data_request() to use the shorter interface over
 * the wire where appropriate.
 *
 * \param res_id is to be acquired by the local subsystem.
 * \param callback will be called to indicate the result.  \c subres
 *        will retain \c callback for calling in case the resource
 *        needs to be revoked or removed on subsystem stop.
 *
 * \return \c TRUE if the request could be sent, else \c FALSE, in which
 * case no subsequent callback will be made.  A failure here only
 * indicates a failure of messaging; no resource handling has yet been
 * done.
 */
extern bool subres_request(
    CCP_RESOURCE_ID res_id,
    SUBRES_CALLBACK callback);

/**
 * Request use of a resource, maybe passing additional information.
 *
 * This is a superset of \c subres_request().
 *
 * \param res_id is the resource being requested.
 * \param flags is a bitmask of flags from \c CCP_RESOURCE_FLAGS to
 *        be passed.
 * \param data_ptr is a pointer to an array of \c uint16 data,
 *        which may be \c NULL if \c data_len is zero.  The
 *        data, if used, is copied immediately so the caller
 *        can free it on return.
 * \param data_len is the number of uint16's in the array pointed
 *        to by \c data_ptr.
 * \param callback will be called to indicate the result.  \c subres
 *        will retain \c callback for calling in case the resource
 *        needs to be revoked or removed on subsystem stop.
 *
 * \return \c TRUE if the request could be sent, else \c FALSE, in which
 * case no subsequent callback will be made.  A failure here only
 * indicates a failure of messaging; no resource handling has yet been
 * done.
 */
extern bool subres_data_request(
    CCP_RESOURCE_ID res_id,
    uint16 flags,
    const uint16 *data_ptr,
    uint16 data_len,
    SUBRES_CALLBACK callback);

/**
 * A shorthand for cancelling a resource request.
 *
 * This macro sets the appropriate flags in \c subres_data_request()
 * to ensure that any pending request is cancelled, while if the request
 * has already been processed no further action is taken.  Note that
 * if multiple requests for a resource are in the queue they need
 * to be cancelled separately.
 */
#define subres_cancel(res_id, callback)                                 \
    subres_data_request(                                                \
        res_id,                                                         \
        CCP_RESOURCE_FLAGS_UPDATE|CCP_RESOURCE_FLAGS_NO_QUEUE,          \
        NULL, 0U, callback)

/**
 * Request a group of resources. If they are not all available, none will be
 * allocated.  No other operations on these resources should be attempted by
 * the client while this call is ongoing; this is not explicitly checked by
 * subres.
 *
 * @param res_ids Array of resource IDs. This array is copied by subres and so
 * may be freed on return.
 * @param num_res_ids Number of resource IDs
 * @param callback will be called on each resource individually to indicate the
 * result, and again to indicate the group operation completion.
 */
extern void subres_group_request(const CCP_RESOURCE_ID *res_ids,
                                 uint16 num_res_ids,
                                 SUBRES_CALLBACK callback);

/**
 * Release previously requested or owned resource \c res_id.
 *
 * If the resource has been requested but not yet granted, \c subres
 * will attempt to cancel the request and look after any crossover
 * conditions resulting from \c globres processing.  In this
 * case no further response are passed to the requester; it may
 * consider the request immediately cancelled (and obviously will
 * not be able to use the resource without a further request for it).
 *
 * Otherwise, if the resource is already owned by the caller, when a
 * response is received from \c globres a response will be generated via
 * the \c callback registered when the resource was requested .  The
 * owner may choose to ignore a successful response (\c
 * SUBRES_OP_RESOURCE_RELEASED) unless it has some state to clear up at
 * this point.  An unsuccessful release for a previously granted
 * resource is treated as a fatal error unless the error indicates
 * a retry is possible.
 *
 * It is a fatal error to attempt to release a resource that
 * is not currently owned or requested; \c subres will panic immediately.
 *
 * \return \c TRUE if the request could be sent, else \c FALSE, in which
 * case no subsequent callback will be made.  A failure here only
 * indicates a failure of messaging; no resource handling has yet been
 * done.
 */
extern bool subres_release(CCP_RESOURCE_ID res_id);

/**
 * Release a group of previously requested or owned resources
 *
 * These need not have been allocated as a group; however, they should all be
 * associated with the same callback.
 *
 * @param res_ids Array of resource IDs. This array is copied by subres and so
 * may be freed on return.
 * @param num_res_ids Number of resource IDs
 */
extern void subres_group_release(const CCP_RESOURCE_ID *res_ids,
                                 uint16 num_res_ids);

/**
 * Client confirms revocation of resource \c res_id.
 *
 * This is only used
 * - after the owner has received SUBRES_OP_RESOURCE_REVOKE
 * - and returned SUBRES_RETURN_PENDING to indicate that the revocation
 *   could not be completed immediately.
 * .
 * \c subres will panic otherwise.
 */
extern void subres_revoked(CCP_RESOURCE_ID res_id);

/**
 * Update the configuration of a PIO resource.  This resource must already be
 * owned by the subsystem.
 *
 * @param res_id ID of the resource: this must be a PIO!
 * @param config Array of configuration data. This array is copied by subres
 * and so may be freed on return.
 * @param num_config_keys Number of elements in configuration array
 * @return \c TRUE if the request could be sent, else \c FALSE, in which
 * case no subsequent callback will be made.  A failure here only
 * indicates a failure of messaging; no resource handling has yet been
 * done.
 */
extern bool subres_pio_configure(CCP_RESOURCE_ID res_id,
                                 const CCP_PIO_KEY_VALUE_TYPE *config,
                                 uint16 num_config_keys);


/**
 * Request and configure a PIO resource.
 * @param res_id ID of the resource: this must be a PIO!
 * @param config Array of configuration data. This array is copied by subres
 * and so may be freed on return.
 * @param num_config_keys Number of elements in the configuration array
 * @param callback will be called to indicate the result.  \c subres
 *        will retain \c callback for calling in case the resource
 *        needs to be revoked or removed on subsystem stop (although revokable
 *        resources should not actually be allocated in groups to avoid subres
 *        getting into an inconsistent state).
 */
extern void subres_pio_request_configure(CCP_RESOURCE_ID res_id,
                                         const CCP_PIO_KEY_VALUE_TYPE *config,
                                         uint16 num_config_keys,
                                         SUBRES_CALLBACK callback);

/**
 * Query the configuration of a PIO resource.
 *
 * @param res_id ID of the resource: this must be a PIO!
 * @param callback This will be called when the result comes back.
 */
extern void subres_pio_config_query(CCP_RESOURCE_ID res_id,
                                    SUBRES_CALLBACK callback);

/**
 * Update the PIO configuration of a set of PIOs with identical configurations
 *
 * @param res_ids Array of resource IDs: these must be PIOs!  This array is
 * copied by subres and so may be freed on return.
 * @param num_res_ids Number of IDs in the array
 * @param config Common configuration to set in the PIOs
 * @param num_config_keys Number of key/value pairs in the configuration array
 */
extern void subres_group_pio_configure(const CCP_RESOURCE_ID *res_ids,
                                       uint16 num_res_ids,
                                       const CCP_PIO_KEY_VALUE_TYPE *config,
                                       uint16 num_config_keys);

/**
 * Request and configure a set of PIO resources with identical configurations
 *
 * @param res_ids Array of resource IDs: these must be PIOs!  This array is
 * copied by subres and so may be freed on return.
 * @param num_res_ids Number of resource IDs
 * @param config Array of configuration data. This array is copied by subres
 * and so may be freed on return.
 * @param num_config_keys Number of elements in the configuration array
 * @param callback will be called on each resource individually to indicate the
 * result of both the request and the configuration operations, and again
 * (once) to indicate the overall group operation completion.
 */
extern void subres_group_pio_request_configure(const CCP_RESOURCE_ID *res_ids,
                                           uint16 num_res_ids,
                                           const CCP_PIO_KEY_VALUE_TYPE *config,
                                           uint16 num_config_keys,
                                           SUBRES_CALLBACK callback);

/**
 * Query the PIO configuration of a set of PIOs.
 *
 * @param res_ids Array of resource IDs: these must be PIOs!  This array is
 * copied by subres and so may be freed on return.
 * @param num_res_ids Number of IDs in the array
 * @param callback will be called on each resource individually to indicate the
 * result of both the request and the configuration operations, and again
 * (once) to indicate the overall group operation completion.
 */
extern void subres_group_pio_config_query(const CCP_RESOURCE_ID *res_ids,
                                          uint16 num_res_ids,
                                          SUBRES_CALLBACK callback);

/**
 * Query the status of a group of resources.
 *
 * @param res_ids Array of resource IDs: copied by subres and so may be freed
 *                on return.
 * @param num_res_ids Number of IDs in the array
 * @param callback will be called on each resource individually to indicate the
 * result of both the request and the configuration operations, and again
 * (once) to indicate the overall group operation completion.
 */
void subres_group_resource_query(const CCP_RESOURCE_ID *res_ids,
                                 uint16 num_res_ids,
                                 SUBRES_CALLBACK callback);

/*@}*/


/**
 * @name Interface for top-level management
 */
/*@{*/

/** Callback following clear */
typedef void (*SUBRES_CLEAR_CALLBACK)(void);

/**
 * Release all resource owned by the subsystem.
 *
 * The resource owner will receive \c SUBRES_OP_RESOURCE_CLEARED.
 *
 * When the \c subres database is empty, a call will be made
 * to clear_callback.
 */
extern void subres_clear(SUBRES_CLEAR_CALLBACK clear_callback);

/*@}*/


/**
 * @name Communication interface
 */
/*@{*/

/**
 * Handler for received CCP messages dealing with resources.
 *
 * \param pdu is the received CCP PDU.
 * \param pdu_len is its length in 16-bit words.
 * \param remote_ss_id is the sender of the message, always the Curator.
 *
 * \return \c TRUE, as the caller does not currently have the capability for
 * dealing with failed receptions.
 */
extern bool subres_ccp_rx(
    const uint16 *pdu,
    uint16 pdu_len,
    uint16 remote_ss_id);

/*@}*/

#endif /* SUBRES_H */
