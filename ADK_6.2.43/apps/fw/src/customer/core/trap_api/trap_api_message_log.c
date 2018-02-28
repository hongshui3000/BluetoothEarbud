/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */

#include "trap_api/trap_api_private.h"
#include "pmalloc/pmalloc.h"
#include "longtimer/longtimer.h"

/**
 * Number of elements in circular message log buffer.
 *
 * Must be a power of 2
 */
#define MSG_LOG_BUF_SIZE       2048

#define MSG_LOG_BUF_MASK       (MSG_LOG_BUF_SIZE - 1)

/**
 * A circular message log buffer, its current logging poisition, and its
 * current start position (used to handle wrap around of the circular buffer).
 * These will be accessed from Python.
 */
static uint8  trap_msg_log[MSG_LOG_BUF_SIZE];
static uint16 trap_msg_log_pos = 0;
static uint16 trap_msg_log_start = 0;

/* Arbitrary value used to ensure we get record alignment. */

static uint32 delimiter = 0xc001d00dUL;
static uint32 seq_num = 0;

/**
 * Update the current position within the trap_msg_log buffer.
 *
 * Takes into account wrap-around within the circular buffer, and accounts for
 * wrapping around so that pos becomes equal to trap_msg_log_start. In that
 * situation the value of trap_msg_log_start is moved to the start of the next
 * message in the buffer, discarding the message at the initial
 * trap_msg_log_start location as it is about to be over-written.
 *
 * @param   pos: the original position within the trap_msg_log buffer
 *
 * @return  the new position within the trap_msg_log buffer
 */
static uint16 update_position(uint16 pos)
{
    ++pos;
    pos &= MSG_LOG_BUF_MASK;
    if (pos == trap_msg_log_start)
    {
        /*
         * Caught up with the start of the first valid message in the circular
         * buffer. Move trap_msg_log_start to the start of the next valid
         * message in the buffer. This is so the Python script knows where to
         * start from.
         */
        uint16 rec_len = trap_msg_log[pos];
        uint16 next = (pos + 1) & MSG_LOG_BUF_MASK;
        uint16 new_value;
        rec_len += (uint16) (trap_msg_log[next] << 8);
        /*
         * Add the space occupied by the record plus rec_len itself (2 bytes).
         */
        new_value = (uint16)(trap_msg_log_start + rec_len + 2);
        /*
         * Account for the possibility of trap_msg_log_start wrapping around
         * the end back to the start of the trap_msg_log buffer.
         */
        new_value &= MSG_LOG_BUF_MASK;
        /*
         * Done via new_value to ensure trap_msg_log_start written atomically.
         */
        trap_msg_log_start = new_value;
    }

    return pos;
}

/**
 * Put an item in the trap_msg_log buffer a uint8 at a time.
 *
 * Takes into account wrap-around within the circular buffer.
 *
 * @param   item: pointer to the start of the item to copy
 * @param   len: the length of the item to copy
 * @param   pos: the original position within the trap_msg_log buffer
 *
 * @return  the new position within the trap_msg_log buffer
 */
static uint16 log_item(const uint8 *item, uint16 len, uint16 pos)
{
    uint16 index;
    for (index = 0; index < len; index++)
    {
        trap_msg_log[pos] = item[index];
        pos = update_position(pos);
    }

    return pos;
}

/**
 * Log the state of a message.
 * \param action: what is happening to the message.
 * \param msg: the AppMessage struture for the message.
 */
void trap_api_message_log(
    TRAP_API_LOG_ACTION action,
    AppMessage *        msg)
{
    uint16  pos, rec_len, msg_len;
    uint8   type = (uint8) action;
    uint32  now  = get_milli_time();
    /*
     * With the following constant calculation, give the compiler the
     * opportunity to work it out at compile time, or failing that it
     * should only get calculated once if done at run time.
     */
    static const uint16 fixed_size  = sizeof(delimiter)
                                    + sizeof(seq_num)
                                    + sizeof(now)
                                    + sizeof(type)      /* action */
                                    + sizeof(msg->task)
                                    + sizeof(msg->id)
                                    + sizeof(msg->condition_addr)
                                    + sizeof(msg->due)
                                    + sizeof(msg_len);

    /* Don't bother logging the msg->c_width */

    pos = trap_msg_log_pos;

    if (msg->message == NULL)
    {
        /* There is no message payload, or it may have been freed. */
        msg_len = 0;
    }
    else
    {
        /*
         * The length of the pmalloc block holding the message payload can be
         * determined from the size of the pool that has been allocated. This
         * indicates the maximum possible size for the payload.
         */
        msg_len = (uint16) psizeof((const void *) msg->message);
    }

    /*
     * Calculate the length of the record to be written to the circular buffer,
     * not including the length pf rec_len itself.
     */
    rec_len = (uint16) (fixed_size + msg_len);

    pos = log_item((const uint8 *) &rec_len, (uint16) sizeof(rec_len), pos);
    pos = log_item((const uint8 *) &delimiter, (uint16) sizeof(delimiter), pos);
    pos = log_item((const uint8 *) &seq_num, (uint16) sizeof(seq_num), pos);
    pos = log_item((const uint8 *) &now, (uint16) sizeof(now), pos);
    pos = log_item((const uint8 *) &type, (uint16) sizeof(type), pos);
    pos = log_item((const uint8 *) &msg->task, (uint16) sizeof(msg->task), pos);
    pos = log_item((const uint8 *) &msg->id, (uint16) sizeof(msg->id), pos);
    pos = log_item((const uint8 *) &msg->condition_addr,
            (uint16) sizeof(msg->condition_addr), pos);
    pos = log_item((const uint8 *) &msg->due, (uint16) sizeof(msg->due), pos);
    pos = log_item((const uint8 *) &msg_len, (uint16) sizeof(msg_len), pos);
    if (msg_len > 0)
    {
        pos = log_item((uint8 *) msg->message, msg_len, pos);
    }
    seq_num++;
    trap_msg_log_pos = pos;
}

