/* Copyright (c) 2017 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file 
 * Internal header for the PIO configuration module.
 */

#ifndef PIO_CFG_PRIVATE_H
#define PIO_CFG_PRIVATE_H


#include "pio_cfg/pio_cfg.h"
#include "hydra/hydra_macros.h"
#include "hydra_log/hydra_log.h"
#include "int/int.h"
#include "subres/subres.h"
#include "hal/halauxio.h"
#include "sched/sched.h"
#include "pmalloc/pmalloc.h"
#include "led_cfg/led_cfg.h"
#include "assert.h"

/**
 * Typedef for the panic IDs.
 */
typedef enum pio_cfg_panic_diatribes
{
    PIO_CFG_UNHANDLED_SUBRES_RESPONSE = 0,
    PIO_CFG_UNHANDLED_OP,
    PIO_CFG_OP_NO_CCP_KEY,
    PIO_CFG_BAD_SUBRES_QUERY
} pio_cfg_panic_diatribes;

/**
 * Internal helper function for acquiring and releasing PIO resources.
 * \param bank Bank ID.
 * \param mask Bit mask indicating which PIOs to acquire/release.
 * \param is_request TRUE if acquiring is needed, FALSE otherwise.
 * \return PIO_CFG_SUCCESS if successful.
 */
pio_cfg_status pio_cfg_ccp_acquire_release(uint16 bank,
                                           uint32 mask,
                                           bool is_request);

/**
 * Internal helper function for interrogating the ownership of PIO resources.
 * \param bank Bank ID. All PIOs within the bank will be interrogated.
 * \return PIO_CFG_SUCCESS if successful.
 */
pio_cfg_status pio_cfg_ccp_resource_query(uint16 bank);

/**
 * Internal helper function for configuring a PIO property.
 * \param bank Bank ID.
 * \param mask Bit mask indicating which PIOs to configure.
 * \param value Value to configure the property to. This is not a bit mask.
 * \param op Config operation that's taking place. The PIO property will be
 * deduced from this.
 * \return PIO_CFG_SUCCESS if successful.
 */
pio_cfg_status pio_cfg_ccp_configure(uint16 bank,
                                     uint32 mask,
                                     uint32 value,
                                     pio_cfg_ops op);

/**
 * Internal helper function for configuring a PIO property.
 * \param bank Bank ID.
 * \param mask Bit mask indicating which PIOs to configure.
 * \param value_mask Bit mask indicating the value to configure the property to
 * (per PIO).
 * \param op Config operation that's taking place. The PIO property will be
 * deduced from this.
 * \return PIO_CFG_SUCCESS if successful.
 */
pio_cfg_status pio_cfg_ccp_configure_masked(uint16 bank,
                                            uint32 mask,
                                            uint32 value_mask,
                                            pio_cfg_ops op);

/**
 * Helper function for querying PIO configurations.
 * \param bank Bank ID.
 * \param mask Bit mask indicating which PIOs to query.
 * \param op Config operation that's taking place. The PIO property will be
 * deduced from this.
 * \return PIO_CFG_SUCCESS if successful.
 */
pio_cfg_status pio_cfg_ccp_query(uint16 bank,
                                 uint32 mask,
                                 pio_cfg_ops op);

/**
 * Helper function for populating a CCP resource ID buffer with PIOs indicated
 * by a bank and a mask.
 * \param bank Bank ID.
 * \param mask Bit mask indicating which PIOs to configure.
 * \param res_ids Pointer to the resource ID buffer to populate.
 * \return The number of PIOs found in the mask.
 */
uint8 pio_cfg_get_resource_ids(uint16 bank,
                               uint32 mask,
                               CCP_RESOURCE_ID *res_ids);

/**
 * Helper function for figuring out the CCP key corresponding to a set
 * operation.
 * \param op Operation ID.
 * \return CCP key corresponding to the operation.
 */
CCP_PIO_CONFIG_KEY pio_cfg_set_op_to_ccp_key(pio_cfg_ops op);

/**
 * Helper function for figuring out the CCP key corresponding to a get
 * operation.
 * \param op Operation ID.
 * \return CCP key corresponding to the operation.
 */
CCP_PIO_CONFIG_KEY pio_cfg_get_op_to_ccp_key(pio_cfg_ops op);

/**
 * Helper function for extracting a value from a PIO config array.
 * /param data Configuration data pointer.
 * /param len Length of the configuration data.
 * /param key Key to search for.
 * /param value Pointer to buffer for storing the value if found.
 * /return FALSE if key is not found, TRUE if it is.
 * See \c CCP_PIO_CONFIG_KEY for more information on what the values mean.
 */
bool pio_cfg_ccp_value_get(CCP_PIO_KEY_VALUE_TYPE *data,
                           uint16 len,
                           CCP_PIO_CONFIG_KEY key,
                           uint16 *value);

/**
 * Internal callback used to abstract the CCP interface.
 * \param op Operation that was requested.
 * \param bank PIO bank ID that was requested.
 * \param mask PIO bit mask that was requested.
 * \param value Value or bit mask value that was requested.
 * \param response Response received from the Curator.
 */
void pio_cfg_ccp_cb(pio_cfg_ops op,
                    uint16 bank,
                    uint32 mask,
                    uint32 value,
                    uint32 response);

/**
 * Helper function for keeping track of ongoing operations making sure two
 * operations don't overlap.
 * \return TRUE if the operation can continue, FALSE if there already is an
 * ongoing operation.
 */
bool pio_cfg_start_op(void);

/**
 * Indicates that the ongoing operation has terminated.
 */
void pio_cfg_end_op(void);

/**
 * Helper functions to figure out if PIOs are XIOs.
 * \param bank Bank ID.
 * \param mask PIO bit mask.
 * \return Bit mask representing the XIOs out of the given PIOs.
 */
uint32 pio_cfg_xios(uint16 bank, uint32 mask);


#endif /* PIO_CFG_PRIVATE_H */
