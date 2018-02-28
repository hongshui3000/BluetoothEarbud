/* Copyright (c) 2017 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * CCP interface for PIO configuration module.
 */

#include "pio_cfg/pio_cfg_private.h"

/**
 * Stores the response PIO mask for situations where we need to perform multiple
 * subres requests and wait for the response. We then construct this mask as
 * subres resposes arrive and send it to back once the last expected subres
 * response is received. */
static struct result_pio
{
    uint16 bank;
    uint32 mask;
    uint32 value;
    uint32 response;
    pio_cfg_ops op;
} result_pio;

/**
 * Keeps track of how many group ops we have going on. We need to receive this
 * many responses.
 */
static uint8 count_group_ops = 0;

/** Forward reference for the subres callback */
static SUBRES_RETURN pio_cfg_ccp_subres_group_cb(SUBRES_CALLBACK_DATA *cdata);


pio_cfg_status pio_cfg_ccp_acquire_release(uint16 bank,
                                           uint32 mask,
                                           bool is_request)
{
    CCP_RESOURCE_ID res_ids[PIOS_PER_BANK];
    uint16 num_resource_ids = pio_cfg_get_resource_ids(bank, mask, res_ids);

    result_pio.bank = bank;
    result_pio.mask = mask;
    result_pio.value = 0; /* not used */
    result_pio.response = mask; /* Initialise with fail indicator */
    result_pio.op = is_request?PIO_CFG_OP_ACQUIRE:PIO_CFG_OP_RELEASE;

    ATOMIC_BLOCK_START {
        count_group_ops++;
    } ATOMIC_BLOCK_END;
    if (is_request)
    {
        L4_DBG_MSG2("pio_cfg_ccp: initiating PIO acquisition for bank %d "
                    "mask 0x%08x", bank, mask);
        subres_group_request(res_ids,
                             num_resource_ids,
                             pio_cfg_ccp_subres_group_cb);
    }
    else
    {
        L4_DBG_MSG2("pio_cfg_ccp: initiating PIO release for bank %d "
                    "mask 0x%08x", bank, mask);
        /* Callback is remembered from the subres_group_request() call */
        subres_group_release(res_ids, num_resource_ids);
    }
    return PIO_CFG_SUCCESS;
}

pio_cfg_status pio_cfg_ccp_resource_query(uint16 bank)
{
    CCP_RESOURCE_ID res_ids[PIOS_PER_BANK];
    uint16 num_res_ids = pio_cfg_get_resource_ids(bank, ALL_PIOS, res_ids);

    L4_DBG_MSG1("pio_cfg_ccp: querying PIO bank %d", bank);

    result_pio.bank = bank;
    result_pio.mask = ALL_PIOS;
    result_pio.value = 0; /* not used */
    result_pio.response = 0;
    result_pio.op = PIO_CFG_OP_RESOURCE_QUERY;

    ATOMIC_BLOCK_START {
        count_group_ops++;
    } ATOMIC_BLOCK_END;
    subres_group_resource_query(res_ids,
                                num_res_ids,
                                pio_cfg_ccp_subres_group_cb);

    return PIO_CFG_SUCCESS;
}

pio_cfg_status pio_cfg_ccp_configure_masked(uint16 bank,
                                            uint32 mask,
                                            uint32 value_mask,
                                            pio_cfg_ops op)
{
    pio_cfg_status status = PIO_CFG_SUCCESS;
    uint32 to_enable = mask & value_mask;
    uint32 to_disable = mask & (~value_mask);

    L4_DBG_MSG4("pio_cfg_ccp: configure_masked: op %d for bank %d, mask 0x%08x"
                " and value_mask 0x%08x", op, bank, mask, value_mask);

    if (to_enable)
    {
        status = pio_cfg_ccp_configure(bank, to_enable, 1, op);
    }
    if (to_disable && (status == PIO_CFG_SUCCESS))
    {
        status = pio_cfg_ccp_configure(bank, to_disable, 0, op);
    }
    return status;
}

pio_cfg_status pio_cfg_ccp_configure(uint16 bank,
                                     uint32 mask,
                                     uint32 value,
                                     pio_cfg_ops op)
{
    CCP_PIO_CONFIG_KEY ccp_key;
    CCP_RESOURCE_ID res_ids[PIOS_PER_BANK];
    CCP_PIO_KEY_VALUE_TYPE config;
    uint16 num_res_ids;

    L4_DBG_MSG4("pio_cfg_ccp: configure: op %d for bank %d, mask 0x%08x and "
                "value 0x%08x", op, bank, mask, value);

    ccp_key = pio_cfg_set_op_to_ccp_key(op);

    num_res_ids = pio_cfg_get_resource_ids(bank, mask, res_ids);
    CCP_PIO_KEY_VALUE_TYPE_KEY_SET(&config, ccp_key);
    CCP_PIO_KEY_VALUE_TYPE_VALUE_SET(&config, value);

    result_pio.bank = bank;
    result_pio.mask = mask;
    result_pio.value = value;
    result_pio.response = mask; /* Initialise with fail indicator */
    result_pio.op = op;

    ATOMIC_BLOCK_START {
        count_group_ops++;
    } ATOMIC_BLOCK_END;
    subres_group_pio_configure(res_ids, num_res_ids, &config, 1);

    return PIO_CFG_SUCCESS;
}

pio_cfg_status pio_cfg_ccp_query(uint16 bank,
                                 uint32 mask,
                                 pio_cfg_ops op)
{
    CCP_RESOURCE_ID res_ids[PIOS_PER_BANK];
    uint16 num_res_ids;

    L4_DBG_MSG3("pio_cfg_ccp: query: op %d for bank %d and mask 0x%08x",
                op, bank, mask);

    num_res_ids = pio_cfg_get_resource_ids(bank, mask, res_ids);

    result_pio.bank = bank;
    result_pio.mask = mask;
    result_pio.value = 0; /* not used */
    result_pio.response = 0;
    result_pio.op = op;

    ATOMIC_BLOCK_START {
        count_group_ops++;
    } ATOMIC_BLOCK_END;
    subres_group_pio_config_query(
        res_ids, num_res_ids, pio_cfg_ccp_subres_group_cb);

    return PIO_CFG_SUCCESS;
}

static SUBRES_RETURN pio_cfg_ccp_subres_group_cb(SUBRES_CALLBACK_DATA *cdata)
{
    uint16 query_response;
    uint32 pio = (uint32)cdata->res_id - (uint32)CCP_RESOURCE_ID_PIO0;
    uint32 pio_mask = POFFM(pio);
    uint32 bank = PBANK(pio);

    L4_DBG_MSG2("pio_cfg_ccp: Received subres response %d for pio %d.",
                cdata->op, pio);

    if (cdata->op != SUBRES_OP_GROUP_COMPLETED)
    {
        assert(bank < NUMBER_OF_PIO_BANKS);
    }

    switch (cdata->op)
    {
        case SUBRES_OP_RESOURCE_ASSIGNED:
            result_pio.response &= ~pio_mask;
            break;
        case SUBRES_OP_PIO_CONFIGURED:
            result_pio.response &= ~pio_mask;
            break;
        case SUBRES_OP_GROUP_COMPLETED:
            ATOMIC_BLOCK_START {
                count_group_ops--;
            } ATOMIC_BLOCK_END;
            if (count_group_ops == 0)
            {
                L4_DBG_MSG4("pio_cfg_ccp: Subres group ops completed, "
                            "responding with bank %d, mask 0x%08x, "
                            "value 0x%08x, response 0x%08x.",
                            result_pio.bank, result_pio.mask,
                            result_pio.value, result_pio.response);
                pio_cfg_ccp_cb(result_pio.op,
                               result_pio.bank,
                               result_pio.mask,
                               result_pio.value,
                               result_pio.response);
            }
            break;
        case SUBRES_OP_RESOURCE_UNAVAIL:
            L2_DBG_MSG1("pio_cfg_ccp: PIO %d unavailable", pio);
            result_pio.response |= pio_mask;
            break;
        case SUBRES_OP_RESOURCE_RELEASED:
            result_pio.response &= ~pio_mask;
            break;
        case SUBRES_OP_QUERY_RESPONSE:
            if (cdata->ops.query.status == CCP_RESOURCE_ST_AVAILABLE)
            {
                result_pio.response |= pio_mask;
            }
            else
            {
                result_pio.response &= ~pio_mask;
            }
            break;
        case SUBRES_OP_PIO_CONFIG_QUERIED:
            if (!pio_cfg_ccp_value_get(
                     cdata->ops.config_query.data,
                     cdata->ops.config_query.len,
                     pio_cfg_get_op_to_ccp_key(result_pio.op),
                     &query_response))
            {
                L2_DBG_MSG1("pio_cfg_ccp: Bad subres query; did not find key "
                            "for op %d in response", cdata->op);
                panic_diatribe(PANIC_PIO_CFG, PIO_CFG_BAD_SUBRES_QUERY);
            }
            switch (result_pio.op)
            {
                case PIO_CFG_OP_GET_PIO_MUX:
                case PIO_CFG_OP_GET_DRIVE_STRENGTH:
                case PIO_CFG_OP_GET_PAD_MUX:
                    result_pio.response = query_response;
                    break;
                default:
                    result_pio.response |= query_response?pio_mask:0;
                    break;
            }
            pfree(cdata->ops.config_query.data);
            break;
        default:
            L2_DBG_MSG1("pio_cfg_ccp: Unhandled subres group response: %d",
                        cdata->op);
            panic_diatribe(PANIC_PIO_CFG, PIO_CFG_UNHANDLED_SUBRES_RESPONSE);
    }

    return SUBRES_RETURN_SUCCESS;
}
