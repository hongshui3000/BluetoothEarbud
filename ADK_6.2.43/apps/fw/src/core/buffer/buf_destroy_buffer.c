/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Functions dealing with buffer destruction.
*/

#include "buffer/buffer_private.h"

/**
 * Destroy a \c BUFFER's internals.
 *
 * Does not free the buffer structure.  Does free any associated
 * metadata.
 */
void buf_destroy_buffer_no_free(BUFFER *buf)
{
    if ( !mmu_handle_is_null(buf->handle) )
    {
        mmu_destroy_buffer(buf->handle, TRUE);
    }
    buf->handle = 0;
}


/**
 * Destroy a \c BUFFER, freeing associated resources.
 */
void buf_destroy_buffer(BUFFER *buf)
{
    buf_destroy_buffer_no_free(buf);
    pfree(buf);
}


/**
 * Destroy a newly created buffer.
 *
 * This is private to the buffer module.
 */
void buf_destroy_buffer_on_creation(BUFFER *buf, bool do_free)
{
    if (do_free)
    {
        buf_destroy_buffer(buf);
    }
    else
    {
        buf_destroy_buffer_no_free(buf);
    }
}
