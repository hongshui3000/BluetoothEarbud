/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Routine for initialising a buffer with the mmu handles of a previously 
 * created one.
*/

#include "buffer/buffer_private.h"

void
buf_create_clone_location(BUFFER *clone_buf, const BUFFER *source_buf)
{
    clone_buf->index = 0;
    clone_buf->outdex = 0;
    clone_buf->tail = 0;
    clone_buf->size_mask = source_buf->size_mask;
    clone_buf->handle = source_buf->handle;
}

