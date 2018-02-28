/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file 
 * Clear a pending software interrupt
 *
 */

#include "int/swint_private.h"

/**
 * Clear a pending software interrupt
 */
void clear_sw_interrupt(swint_id id)
{
    /* Determine whether this is a dedicated or multiplexed event */
    if (id < SWINT_HW_SOURCES)
    {
        /* Dedicated hardware interrupt */
        clear_interrupt(SWINT_TO_INT_SOURCE(id));
    }
#if SWINT_SW_SOURCES
    else if (id < SWINT_SOURCES)
    {
        /* Check that the interrupt source is currently configured */
        swint_sw_source *source = &swint_sw_sources[id - SWINT_HW_SOURCES];
        if (!source->handler)
        {
            panic(PANIC_HYDRA_INVALID_SW_INTERRUPT);
        }
        /* Cancel any pending request, but do not touch the hardware event */
        source->req = FALSE;
    }
#endif
    else
        panic(PANIC_HYDRA_INVALID_SW_INTERRUPT);
}
