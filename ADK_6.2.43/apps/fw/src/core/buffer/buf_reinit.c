/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file 
 *
 * \name buf_reinit_desc DESCRIPTION
 * Implementation of the function \c buf_reinit().
 */

#include "buffer/buffer_private.h"

/**
 * Reinitialise the state of the buffer pointers.
 */
void
buf_reinit(BUFFER *buf)
{
    buf->index = buf->outdex = buf->tail = 0UL;

    mmu_set_handle_offset(buf->handle, 0U);
}
