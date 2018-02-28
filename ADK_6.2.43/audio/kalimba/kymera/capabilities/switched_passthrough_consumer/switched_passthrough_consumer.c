/****************************************************************************
 * Copyright (c) 2017 Qualcomm Technologies International, Ltd 
****************************************************************************/
/**
 * \file  switched_passthrough_consumer.c
 * \ingroup  capabilities
 *
 *  Switch between consuming input data and passing though input data to
 *  the output.
 *
 */

#include "capabilities.h"
#include "switched_passthrough_consumer.h"
#include "common/interface/util.h"
#include "platform/pl_assert.h"

enum spc_state
{
    /* Note the special ordering: incrementing a _TRANSITION_ state results in
       moving to the non-transition state */
    STATE_TRANSITION_TO_CONSUME,
    STATE_CONSUME,
    STATE_TRANSITION_TO_PASSTHROUGH,
    STATE_PASSTHROUGH
};

typedef struct SPC_OP_DATA
{
    /** Consume data from input buffer. */
    tCbuffer*   ip_buffer;

    /** Passthrough data to output buffer. */
    tCbuffer*   op_buffer;

    /** Current state */
    enum spc_state state;

    /** The type of data consumed/passed by the capability.
        This capability doesn't care about the data type, but 'kymera'
        complains about unresolvable data formats if this doesn't match the
        format of the operator to which this operator is connected */
    AUDIO_DATA_FORMAT data_format;

    /** The requested size of the output buffer, or 0 for the default size. */
    unsigned output_buffer_size;

} SPC_OP_DATA;

/****************************************************************************
Private Function Definitions
*/
static void spc_process_data(OPERATOR_DATA*, TOUCHED_TERMINALS*);
static bool spc_connect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
static bool spc_disconnect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
static bool spc_buffer_details(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
static bool spc_get_sched_info(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
static bool spc_get_data_format(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data);
static bool spc_opmsg_transition_request(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);
static bool spc_opmsg_set_data_format(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);
static bool spc_opmsg_set_buffer_size(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data);

/****************************************************************************
Private Constant Declarations
*/
#ifdef CAPABILITY_DOWNLOAD_BUILD
#define SWITCHED_PASSTHROUGH_CONSUMER_ID CAP_ID_DOWNLOAD_SWITCHED_PASSTHROUGH_CONSUMER
#else
#define SWITCHED_PASSTHROUGH_CONSUMER_ID CAP_ID_SWITCHED_PASSTHROUGH_CONSUMER
#endif

#define SPC_DEFAULT_BLOCK_SIZE 1

/** The stub capability function handler table */
const handler_lookup_struct spc_handler_table =
{
    base_op_create,       /* OPCMD_CREATE */
    base_op_destroy,      /* OPCMD_DESTROY */
    base_op_start,        /* OPCMD_START */
    base_op_stop,         /* OPCMD_STOP */
    base_op_reset,        /* OPCMD_RESET */
    spc_connect,          /* OPCMD_CONNECT */
    spc_disconnect,       /* OPCMD_DISCONNECT */
    spc_buffer_details,   /* OPCMD_BUFFER_DETAILS */
    spc_get_data_format,  /* OPCMD_DATA_FORMAT */
    spc_get_sched_info    /* OPCMD_GET_SCHED_INFO */
};

/* Null terminated operator message handler table - this is the set of operator
 * messages that the capability understands and will attempt to service. */
const opmsg_handler_lookup_table_entry spc_opmsg_handler_table[] =
{
    {OPMSG_COMMON_ID_GET_CAPABILITY_VERSION, base_op_opmsg_get_capability_version},
    {OPMSG_SPC_ID_TRANSITION, spc_opmsg_transition_request},
    {OPMSG_SPC_ID_SET_DATA_FORMAT, spc_opmsg_set_data_format},
    {OPMSG_COMMON_ID_SET_BUFFER_SIZE, spc_opmsg_set_buffer_size},
    {0, NULL}
};


/* Capability data - This is the definition of the capability that Opmgr uses to
 * create the capability from. */
const CAPABILITY_DATA switched_passthrough_consumer_cap_data =
{
    SWITCHED_PASSTHROUGH_CONSUMER_ID,             /* Capability ID */
    1, 0,                           /* Version information - hi and lo parts */
    1, 1,                           /* Max number of sinks/inputs and sources/outputs */
    &spc_handler_table,             /* Pointer to message handler function table */
    spc_opmsg_handler_table,        /* Pointer to operator message handler function table */
    spc_process_data,               /* Pointer to data processing function */
    0,                              /* Reserved */
    sizeof(SPC_OP_DATA)             /* Size of capability-specific per-instance data */
};

/* Data processing function */
static void spc_process_data(OPERATOR_DATA *op_data, TOUCHED_TERMINALS *touched)
{
    SPC_OP_DATA *opx_data = (SPC_OP_DATA *)(op_data->extra_op_data);
    tCbuffer *ip_buffer = opx_data->ip_buffer;
    unsigned data_in_words = cbuffer_calc_amount_data_in_words(ip_buffer);
    unsigned usable_octets = buff_metadata_get_usable_octets(ip_buffer);
    unsigned b4idx, afteridx;
    metadata_tag *mtag;

    if ((opx_data->state == STATE_TRANSITION_TO_PASSTHROUGH) ||
        (opx_data->state == STATE_TRANSITION_TO_CONSUME))
    {
        /* Consume data 'b4' the current metadata index, then transition */
        if (buff_metadata_peek_ex(ip_buffer, &b4idx))
        {
            unsigned words_to_consume;
            unsigned words_b4idx = b4idx / usable_octets;
            words_to_consume = MIN(data_in_words, words_b4idx);
            if (words_to_consume != 0)
            {
                cbuffer_discard_data(ip_buffer, words_to_consume);
                buff_metadata_align_to_buff_read_ptr(ip_buffer);
                data_in_words -= words_to_consume;
                if (data_in_words == 0)
                {
                    touched->sinks |= 1;
                }
            }
            if (words_to_consume == words_b4idx)
            {
                buff_metadata_peek_ex(ip_buffer, &b4idx);
                PL_ASSERT(b4idx == 0);
                /* Incrementing moves to non-transition state */
                opx_data->state++;
            }
        }
    }

    if (opx_data->state == STATE_PASSTHROUGH)
    {
        tCbuffer *op_buffer = opx_data->op_buffer;
        unsigned space_in_words = cbuffer_calc_amount_space_in_words(op_buffer);
        unsigned to_copy = MIN(data_in_words, space_in_words);
        if (to_copy != 0)
        {
            unsigned copied = cbuffer_copy(op_buffer, ip_buffer, to_copy);
            if (copied != 0)
            {
                touched->sources |= 1;
                metadata_strict_transport(ip_buffer, op_buffer, copied * usable_octets);
            }
            if (copied == data_in_words)
            {
                touched->sinks |= 1;
            }
        }
    }
    else if (opx_data->state == STATE_CONSUME)
    {
        if (data_in_words != 0)
        {
            cbuffer_discard_data(ip_buffer, data_in_words);
            mtag = buff_metadata_remove(ip_buffer, data_in_words * usable_octets, &b4idx, &afteridx);
            buff_metadata_tag_list_delete(mtag);
            touched->sinks |= 1;
        }
    }
}

static bool spc_connect_disconnect_common(OPERATOR_DATA *op_data, void *message_data, tCbuffer *buffer, void **response_data)
{
    SPC_OP_DATA *opx_data = (SPC_OP_DATA *)(op_data->extra_op_data);
    unsigned terminal_id = ((unsigned*)message_data)[0];
    if (terminal_id & TERMINAL_SINK_MASK)
    {
        opx_data->ip_buffer = buffer;
    }
    else
    {
        opx_data->op_buffer = buffer;
        if (buffer == NULL)
        {
            if (opx_data->ip_buffer)
            {
                /* Since the output is no longer present to kick, self-kick to consume input data */
                opmgr_kick_operator(op_data);
            }
        }
        else
        {
            unsigned usable_octets = get_octets_per_word(opx_data->data_format);
            buff_metadata_set_usable_octets(opx_data->op_buffer, usable_octets);
        }
    }
    /* Any connect/disconnect should reset the state to consume */
    opx_data->state = STATE_CONSUME;
    return TRUE;
}

static bool spc_connect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    tCbuffer *buffer = (tCbuffer*)(((uintptr_t *)message_data)[1]);
    return base_op_connect(op_data, message_data, response_id, response_data) &&
           spc_connect_disconnect_common(op_data, message_data, buffer, response_data);
}

static bool spc_disconnect(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    return base_op_disconnect(op_data, message_data, response_id, response_data) &&
           spc_connect_disconnect_common(op_data, message_data, NULL, response_data);
}

static bool spc_buffer_details(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    bool result = base_op_buffer_details(op_data, message_data, response_id, response_data);
    if (result)
    {
        unsigned terminal_id = ((unsigned*)message_data)[0];
        OP_BUF_DETAILS_RSP *resp = *response_data;
        SPC_OP_DATA *opx_data = (SPC_OP_DATA *)(op_data->extra_op_data);

        if (0 == (terminal_id & TERMINAL_SINK_MASK))
        {
            /* Output, use the output buffer size */
            resp->b.buffer_size = opx_data->output_buffer_size;
        }
        resp->metadata_buffer = NULL;
        resp->supports_metadata = TRUE;
    }
    return result;
}

static bool spc_get_sched_info(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    bool result = base_op_get_sched_info(op_data, message_data, response_id, response_data);
    if (result)
    {
        OP_SCHED_INFO_RSP *resp = *response_data;
        resp->op_id = op_data->id;
        resp->status = STATUS_OK;
        resp->block_size = SPC_DEFAULT_BLOCK_SIZE;
        resp->run_period = 0;
    }
    return result;
}

static bool spc_get_data_format(OPERATOR_DATA *op_data, void *message_data, unsigned *response_id, void **response_data)
{
    bool result = base_op_get_data_format(op_data, message_data, response_id, response_data);
    if (result)
    {
        SPC_OP_DATA *opx_data = (SPC_OP_DATA *)(op_data->extra_op_data);
        ((OP_STD_RSP*)*response_data)->resp_data.data = opx_data->data_format;
    }
    return result;
}

static bool spc_opmsg_transition_request(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    SPC_OP_DATA *opx_data = (SPC_OP_DATA *)(op_data->extra_op_data);
    OPMSG_SPC_MODE new_mode = (OPMSG_SPC_MODE) OPMSG_FIELD_GET(message_data, OPMSG_SPC_CHANGE_MODE, NEW_MODE);
    
    if (new_mode == OPMSG_SPC_MODE_PASSTHROUGH)
    {
        opx_data->state = STATE_TRANSITION_TO_PASSTHROUGH;
        return TRUE;
    } else if (new_mode == OPMSG_SPC_MODE_CONSUMER)
    {
        opx_data->state = STATE_TRANSITION_TO_CONSUME;
        return TRUE;       
    }
    else
    {
        return FALSE;
    }
}

static bool spc_opmsg_set_data_format(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    SPC_OP_DATA *opx_data = (SPC_OP_DATA *)(op_data->extra_op_data);
    opx_data->data_format = OPMSG_FIELD_GET(message_data, OPMSG_SPC_SET_DATA_FORMAT, DATA_TYPE);
    return TRUE;
}

static bool spc_opmsg_set_buffer_size(OPERATOR_DATA *op_data, void *message_data, unsigned *resp_length, OP_OPMSG_RSP_PAYLOAD **resp_data)
{
    SPC_OP_DATA *opx_data = (SPC_OP_DATA *)(op_data->extra_op_data);
    opx_data->output_buffer_size = OPMSG_FIELD_GET(message_data, OPMSG_COMMON_SET_BUFFER_SIZE, BUFFER_SIZE);
    return TRUE;
}
