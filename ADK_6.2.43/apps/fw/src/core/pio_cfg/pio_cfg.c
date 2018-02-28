/* Copyright (c) 2017 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Manage PIO configuration.
 */

#include "pio_cfg/pio_cfg_private.h"

/**
 * Stores the Apps owned PIOs.
 */
static pio_size_bits pios_owned[NUMBER_OF_PIO_BANKS];

/**
 * Stores the callback.
 */
static pio_cfg_cb pio_cfg_stored_cb;


pio_cfg_status pio_cfg_op(pio_cfg_ops op,
                          uint16 bank,
                          uint32 mask,
                          uint32 value,
                          pio_cfg_cb cb)
{
    pio_cfg_status status = PIO_CFG_SUCCESS;
    uint32 to_acquire = (~pios_owned[bank]) & mask;
    uint32 to_release = pios_owned[bank] & mask;
    uint32 tmp_mask;

    L4_DBG_MSG2("pio_cfg: op %d, cb 0x%08x", op, cb);
    L4_DBG_MSG3("pio_cfg: op: bank %d, mask 0x%08x, value 0x%08x",
                bank, mask, value);

    if (pio_cfg_start_op())
    {
        pio_cfg_stored_cb = cb;

        switch (op)
        {
            case PIO_CFG_OP_ACQUIRE:
            case PIO_CFG_OP_RELEASE:
                UNUSED(value);
                if (((op == PIO_CFG_OP_ACQUIRE) && to_acquire) ||
                    ((op == PIO_CFG_OP_RELEASE) && to_release))
                {
                    status = pio_cfg_ccp_acquire_release(
                                 bank,
                                 (op == PIO_CFG_OP_ACQUIRE)?to_acquire:
                                                            to_release,
                                 (op == PIO_CFG_OP_ACQUIRE)?TRUE:FALSE);
                }
                else
                {
                    /* already done */
                    pio_cfg_ccp_cb(op, bank, mask, value, 0);
                }
                break;
            case PIO_CFG_OP_RESOURCE_QUERY:
                UNUSED(mask);
                UNUSED(value);
                status = pio_cfg_ccp_resource_query(bank);
                break;
            case PIO_CFG_OP_GET_XIO_MODE_ENABLE:
                tmp_mask = mask & pio_cfg_xios(bank, mask);
                if (tmp_mask)
                {
                    status = pio_cfg_ccp_query(bank, tmp_mask, op);
                }
                else
                {
                    /* nothing to do */
                    pio_cfg_ccp_cb(op, bank, mask, value, 0);
                }
                break;
            case PIO_CFG_OP_GET_PIO_MUX:
            case PIO_CFG_OP_GET_DRIVE_STRENGTH:
            case PIO_CFG_OP_GET_PAD_MUX:
                if (BIT_IS_ONLY_ONE_BIT_SET(mask))
                {
                    status = pio_cfg_ccp_query(bank, mask, op);
                }
                else
                {
                    if (mask == 0)
                    {
                        /* nothing to do */
                        pio_cfg_ccp_cb(op, bank, mask, value, 0);
                    }
                    else
                    {
                        status = pio_cfg_ccp_query(bank, mask, op);
                    }
                }
                break;
            case PIO_CFG_OP_GET_PULL_ENABLE:
            case PIO_CFG_OP_GET_PULL_DIRECTION:
            case PIO_CFG_OP_GET_PULL_STRENGTH:
            case PIO_CFG_OP_GET_SLEW_ENABLE:
            case PIO_CFG_OP_GET_STICKY_ENABLE:
                if (mask)
                {
                    status = pio_cfg_ccp_query(bank, mask, op);
                }
                else
                {
                    /* nothing to do */
                    pio_cfg_ccp_cb(op, bank, mask, value, 0);
                }
                break;
            default:
                if (!to_acquire)
                {
                    switch (op)
                    {
                        case PIO_CFG_OP_SET_PIO_MUX:
                        case PIO_CFG_OP_SET_DRIVE_STRENGTH:
                        case PIO_CFG_OP_SET_PAD_MUX:
                            if (mask)
                            {
                                status = pio_cfg_ccp_configure(
                                             bank, mask, value, op);
                            }
                            else
                            {
                                /* nothing to do */
                                pio_cfg_ccp_cb(op, bank, mask, value, 0);
                            }
                            break;
                        case PIO_CFG_OP_SET_XIO_MODE_ENABLE:
                            /* mask of non XIOs that enable XIO mode */
                            tmp_mask = mask &
                                       (~pio_cfg_xios(bank, mask)) &
                                       value;
                            if (tmp_mask)
                            {
                                status = PIO_CFG_NON_XIOS_IN_MASK;
                            }
                            else
                            {
                                /* mask of XIOs that enable XIO mode */
                                tmp_mask = mask &
                                           (pio_cfg_xios(bank, mask)) &
                                           value;
                                if (tmp_mask)
                                {
                                    status = pio_cfg_ccp_configure_masked(
                                                 bank, tmp_mask, value, op);
                                }
                                else
                                {
                                    /* nothing to do */
                                    pio_cfg_ccp_cb(op, bank, mask, value, 0);
                                }
                            }
                            break;
                        case PIO_CFG_OP_SET_PULL_ENABLE:
                        case PIO_CFG_OP_SET_PULL_DIRECTION:
                        case PIO_CFG_OP_SET_PULL_STRENGTH:
                        case PIO_CFG_OP_SET_SLEW_ENABLE:
                        case PIO_CFG_OP_SET_STICKY_ENABLE:
                            if (mask)
                            {
                                status = pio_cfg_ccp_configure_masked(
                                             bank, mask, value, op);
                            }
                            else
                            {
                                /* nothing to do */
                                pio_cfg_ccp_cb(op, bank, mask, value, 0);
                            }
                            break;
                        default:
                            L2_DBG_MSG1("pio_cfg: op: Unhandled op %d", op);
                            panic_diatribe(PANIC_PIO_CFG,
                                           PIO_CFG_UNHANDLED_OP);
                    }
                }
                else
                {
                    /* we must own the PIO to change any parameters
                       (this is an error) */
                    pio_cfg_ccp_cb(op, bank, mask, value, to_acquire);
                }
                break;
        }
    }
    else
    {
        L4_DBG_MSG1("pio_cfg: op %d: busy", op);
        status = PIO_CFG_BUSY;
    }
    if ((status != PIO_CFG_BUSY) && (status != PIO_CFG_SUCCESS))
    {
        /* any other status is an error that stops the callback from being
           called */
        pio_cfg_end_op();
    }
    return status;
}

void pio_cfg_ccp_cb(pio_cfg_ops op,
                    uint16 bank,
                    uint32 mask,
                    uint32 value,
                    uint32 response)
{
    pio_cfg_cb cb = pio_cfg_stored_cb;

    L4_DBG_MSG("pio_cfg: cpp_cb: CCP op %d done");
    L4_DBG_MSG4("pio_cfg: cpp_cb: bank %d, mask 0x%08x, value 0x%08x, "
                "response 0x%08x", bank, mask, value, response);

    switch (op)
    {
        case PIO_CFG_OP_ACQUIRE:
        case PIO_CFG_OP_RELEASE:
            if (response == 0)
            {
                if (op == PIO_CFG_OP_ACQUIRE)
                {
                    pios_owned[bank] |= mask;
                }
                else
                {
                    pios_owned[bank] &= ~mask;
                    led_cfg_pio_mux_mask_update(bank, mask, FALSE);
                }
            }
            break;
        case PIO_CFG_OP_SET_PIO_MUX:
                if (response == 0)
                {
                    if (value == PIO_CFG_PIO_MUX_LED_CTRL)
                    {
                        led_cfg_pio_mux_mask_update(bank, mask, TRUE);
                    }
                }
            break;
        case PIO_CFG_OP_RESOURCE_QUERY:
        case PIO_CFG_OP_SET_DRIVE_STRENGTH:
        case PIO_CFG_OP_SET_PAD_MUX:
        case PIO_CFG_OP_SET_PULL_ENABLE:
        case PIO_CFG_OP_SET_PULL_DIRECTION:
        case PIO_CFG_OP_SET_PULL_STRENGTH:
        case PIO_CFG_OP_SET_SLEW_ENABLE:
        case PIO_CFG_OP_SET_STICKY_ENABLE:
        case PIO_CFG_OP_SET_XIO_MODE_ENABLE:
        case PIO_CFG_OP_GET_PIO_MUX:
        case PIO_CFG_OP_GET_DRIVE_STRENGTH:
        case PIO_CFG_OP_GET_PAD_MUX:
        case PIO_CFG_OP_GET_PULL_ENABLE:
        case PIO_CFG_OP_GET_PULL_DIRECTION:
        case PIO_CFG_OP_GET_PULL_STRENGTH:
        case PIO_CFG_OP_GET_SLEW_ENABLE:
        case PIO_CFG_OP_GET_STICKY_ENABLE:
        case PIO_CFG_OP_GET_XIO_MODE_ENABLE:
            break;
        default:
            L2_DBG_MSG1("pio_cfg: ccp_cb %d: Unhandled op", op);
            panic_diatribe(PANIC_PIO_CFG, PIO_CFG_UNHANDLED_OP);
    }

    pio_cfg_end_op();

    if (cb)
    {
        L4_DBG_MSG("pio_cfg: cpp_cb: calling user callback");
        cb(op, bank, mask, response);
    }
    else
    {
        L4_DBG_MSG("pio_cfg: cpp_cb: no user callback");
    }
}
