/* Copyright (c) 2017 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * PIO configuration helper functions.
 */

#include "pio_cfg/pio_cfg_private.h"

/** Flag to keep track of pio_cfg activity */
static bool is_busy = FALSE;

/**
 * Helper macro which generates the PIO resource IDs from the PIO number.
 * \param pio PIO number
 */
#define CCP_RESOURCE_ID_PIO(pio) \
    ((CCP_RESOURCE_ID)((uint32)pio + (uint32)CCP_RESOURCE_ID_PIO0))


uint8 pio_cfg_get_resource_ids(uint16 bank,
                               uint32 mask,
                               CCP_RESOURCE_ID *res_ids)
{
    uint8 pio, count = 0;
    
    /* Set pio to first PIO in bank */
    pio = (uint8)(bank * PIOS_PER_BANK);

    while ((mask) && (pio < NUMBER_OF_PIOS))
    {
        if (mask & 0x01)
        {
            res_ids[count++] = CCP_RESOURCE_ID_PIO(pio);
        }
        mask >>= 1;
        pio++;
    }

    assert(count > 0);
    
    return count;
}

bool pio_cfg_start_op(void)
{
    bool ret = FALSE;

    ATOMIC_BLOCK_START {
        if (!is_busy)
        {
            is_busy = TRUE;
            ret = TRUE;
        }
    } ATOMIC_BLOCK_END;

    return ret;
}

void pio_cfg_end_op(void)
{
    ATOMIC_BLOCK_START {
        assert(is_busy == TRUE);
        is_busy = FALSE;
    } ATOMIC_BLOCK_END;
}

uint32 pio_cfg_xios(uint16 bank, uint32 mask)
{
    return mask & ((bank == PIO_CFG_XIO_BANK)?PIO_CFG_XIO_MASK:0);
}

CCP_PIO_CONFIG_KEY pio_cfg_set_op_to_ccp_key(pio_cfg_ops op)
{
    CCP_PIO_CONFIG_KEY ccp_key;

    switch (op)
    {
        case PIO_CFG_OP_SET_PIO_MUX:
            ccp_key = CCP_PIO_CONFIG_KEY_PIO_MUX;
            break;
        case PIO_CFG_OP_SET_PULL_ENABLE:
            ccp_key = CCP_PIO_CONFIG_KEY_PULL_ENABLE;
            break;
        case PIO_CFG_OP_SET_PULL_DIRECTION:
            ccp_key = CCP_PIO_CONFIG_KEY_PULL_DIRECTION;
            break;
        case PIO_CFG_OP_SET_PULL_STRENGTH:
            ccp_key = CCP_PIO_CONFIG_KEY_PULL_STRENGTH;
            break;
        case PIO_CFG_OP_SET_DRIVE_STRENGTH:
            ccp_key = CCP_PIO_CONFIG_KEY_DRIVE_STRENGTH;
            break;
        case PIO_CFG_OP_SET_SLEW_ENABLE:
            ccp_key = CCP_PIO_CONFIG_KEY_SLEW_ENABLE;
            break;
        case PIO_CFG_OP_SET_STICKY_ENABLE:
            ccp_key = CCP_PIO_CONFIG_KEY_STICKY_ENABLE;
            break;
        case PIO_CFG_OP_SET_PAD_MUX:
            ccp_key = CCP_PIO_CONFIG_KEY_PAD_MUX;
            break;
        case PIO_CFG_OP_SET_XIO_MODE_ENABLE:
            ccp_key = CCP_PIO_CONFIG_KEY_XIO_MODE;
            break;
        default:
            L2_DBG_MSG1("pio_cfg: utils: set op %d has no corresponding "
                        "CCP key", op);
            panic_diatribe(PANIC_PIO_CFG, PIO_CFG_OP_NO_CCP_KEY);
    }

    return ccp_key;
}

CCP_PIO_CONFIG_KEY pio_cfg_get_op_to_ccp_key(pio_cfg_ops op)
{
    CCP_PIO_CONFIG_KEY ccp_key;

    switch (op)
    {
        case PIO_CFG_OP_GET_PIO_MUX:
            ccp_key = CCP_PIO_CONFIG_KEY_PIO_MUX;
            break;
        case PIO_CFG_OP_GET_PULL_ENABLE:
            ccp_key = CCP_PIO_CONFIG_KEY_PULL_ENABLE;
            break;
        case PIO_CFG_OP_GET_PULL_DIRECTION:
            ccp_key = CCP_PIO_CONFIG_KEY_PULL_DIRECTION;
            break;
        case PIO_CFG_OP_GET_PULL_STRENGTH:
            ccp_key = CCP_PIO_CONFIG_KEY_PULL_STRENGTH;
            break;
        case PIO_CFG_OP_GET_DRIVE_STRENGTH:
            ccp_key = CCP_PIO_CONFIG_KEY_DRIVE_STRENGTH;
            break;
        case PIO_CFG_OP_GET_SLEW_ENABLE:
            ccp_key = CCP_PIO_CONFIG_KEY_SLEW_ENABLE;
            break;
        case PIO_CFG_OP_GET_STICKY_ENABLE:
            ccp_key = CCP_PIO_CONFIG_KEY_STICKY_ENABLE;
            break;
        case PIO_CFG_OP_GET_PAD_MUX:
            ccp_key = CCP_PIO_CONFIG_KEY_PAD_MUX;
            break;
        case PIO_CFG_OP_GET_XIO_MODE_ENABLE:
            ccp_key = CCP_PIO_CONFIG_KEY_XIO_MODE;
            break;
        default:
            L2_DBG_MSG1("pio_cfg: utils: get op %d has no corresponding "
                        "CCP key", op);
            panic_diatribe(PANIC_PIO_CFG, PIO_CFG_OP_NO_CCP_KEY);
    }

    return ccp_key;
}

bool pio_cfg_ccp_value_get(CCP_PIO_KEY_VALUE_TYPE *data,
                           uint16 len,
                           CCP_PIO_CONFIG_KEY key,
                           uint16 *value)
{
    uint16 v, i;
    CCP_PIO_CONFIG_KEY k;
    
    for (i = 0; i < len; i++)
    {
        v = CCP_PIO_KEY_VALUE_TYPE_VALUE_GET(data);
        k = CCP_PIO_KEY_VALUE_TYPE_KEY_GET(data);
        data++;
        if (k == key)
        {
            *value = v;
            return TRUE;
        }
    }
    return FALSE;
}
