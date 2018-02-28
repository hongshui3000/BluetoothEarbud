/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file buf_raw_restore_mapping_state.c
 * Contains the corresponding function.
 */
#include "buffer/buffer_private.h"

void
buf_raw_restore_mapping_state(const buf_mapping_state *save_state)
{
    buf_restore_state(save_state);
}
