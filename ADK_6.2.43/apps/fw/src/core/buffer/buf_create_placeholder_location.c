/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Routine for initialising a buffer with a guveb mmu handle abd iffset,
 */

#include "buffer/buffer_private.h"

void
buf_create_placeholder_location(BUFFER *clone_buf, mmu_handle handle,
                                uint16 offset)
{
    clone_buf->index = offset;
    clone_buf->outdex = offset;
    clone_buf->tail = offset;
    clone_buf->size_mask = (uint16)(/*lint -e{834}*/MMU_PROC_BUF_WIN_SIZE-1);
    clone_buf->handle = handle;
}

