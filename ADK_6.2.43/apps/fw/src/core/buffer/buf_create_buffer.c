/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Routines dealing with buffer creation.
*/

#include "buffer/buffer_private.h"
#include <assert.h>

/**
 * The main function for creating \c BUFFER structures.
 *
 * This function is not exposed outside this file.  The reason
 * is that the API for creating buffers is likely to be extended
 * to support increasingly complicated uses; to avoid the need
 * to rewrite existing interfaces the complexity is encapsulated
 * here.  Additional front-end functions calling this function
 * should be added to support extra complexity.
 *
 * In principle the front-end functions could be macros.  However,
 * creating buffers should never be a time-critical operation, and
 * code size is saved by having full functions.
 */
static BUFFER *buf_create_buffer_internal(mmu_buffer_size msize, BUFFER *buf_mem)
{
    BUFFER *buf;

    /*
     * 16-bit BUFFER support extends only to MMU_BUFFER_SIZE_64K
     */
    if (msize > MMU_BUFFER_SIZE_64K)
    {
        return NULL;
    }
    if (buf_mem == NULL)
    {
        buf = xzpnew(BUFFER);
        if (NULL == buf)
        {
            return NULL;
        }
    }
    else
    {
        buf = buf_mem;
    }

    if ( !mmu_create_buffer(msize, MMU_HANDLE_NULL, &buf->handle, NULL) )
    {
        buf_destroy_buffer_on_creation(buf, buf_mem == NULL);
        return NULL;
    }

    /*
     * Get the size:  may be 64K, so this has to be 32 bits.
     * However, the mask is (by the definition of the BUFFER API) a
     * 16-bit quantity.
     */
    buf->size_mask = (uint16)((1UL << (MMU_LOG_PAGE_BYTES + msize)) - 1UL);

    return buf;
}

/**
 * Create a buffer of a given size.
 *
 * Don't panic if allocation fails.
 */
BUFFER *buf_create_buffer_simple_caller_check(mmu_buffer_size msize)
{
    return buf_create_buffer_internal(msize, NULL);
}

/**
 * Create a buffer of a given size.
 *
 * Panic if allocation fails.
 */
BUFFER *buf_create_buffer_simple(mmu_buffer_size msize)
{
    BUFFER *buf = buf_create_buffer_simple_caller_check(msize);

    if (buf == NULL)
    {
        panic(PANIC_HYDRA_BUFFER_CREATION_FAILED);
    }
    return buf;
}

/**
 * Create a buffer at a fixed location.
 */
void buf_create_buffer_location(mmu_buffer_size msize, BUFFER *buf)
{
    if (buf_create_buffer_internal(msize, buf) == NULL)
    {
        panic(PANIC_HYDRA_BUFFER_CREATION_FAILED);
    }
}

bool buf_try_create_buffer_location(mmu_buffer_size msize, BUFFER *buf)
{
    if (buf_create_buffer_internal(msize, buf) == NULL)
    {
        return FALSE;
    }
    return TRUE;
}

/**
 * Create a buffer at a fixed location using statically allocated memory
 * for both the page list and the pages.
 */
void buf_create_buffer_static_location(mmu_buffer_size msize, 
                                       BUFFER * buf,
                                       uint16 * page_table,
                                       uint16 * pages)
{
    memset(buf, 0, sizeof(BUFFER));
    buf->size_mask = (uint16)((1UL << (MMU_LOG_PAGE_BYTES + msize)) - 1UL);

    /* If creation fails we will find out when we use it */
    (void)mmu_create_buffer(msize, MMU_HANDLE_NULL, &buf->handle, page_table);
    mmu_fill_static_page_table(msize, page_table, pages);
}

BUFFER *buf_create_buffer_in_octets(uint16 size)
{
    mmu_buffer_size msize = MMU_BUFFER_SIZE_512; /* minimum size to start with */
    uint16 rem = size & 0x1ff; /* Bottom nine bits */
    uint16 osize = size;
    size >>= 9; /* Throw away incoming bottom nine */
    if (0 != rem || 0 == size)
    {
        size += 1; /* Handle small and unaligned requests */
    }
    while (size > 1 && msize < MAX_MMU_BUFFER_SIZE)
    {
        /* In the loop means all ok, but we haven't found the right answer yet */
        rem = size & 1; /* Get new bottom bit */
        size >>= 1; /* Throw away bottom bit */
        assert(0 != size); /* Entry to loop implies size > 1 */
        size = (uint16)(size+rem); /* Ensure that in between sizes round up */
        msize++; /* Next size up */
    }
    /* See if we've succeeded */
    /* size = 1 and msize in range indicates ok */
    /* Note inverted logic for error detection
     * This is because the compiler's don't know that panic
     * doesn't return
     */
    if (!(1 == size && msize < MAX_MMU_BUFFER_SIZE))
    {
        panic(PANIC_HYDRA_BUFFER_CREATION_FAILED);
    }
    /* Postconditions */
    assert(osize <= (1U << (9 + msize))); /* Check buffer big enough */
    assert(osize < 512U || (1U << (8 + msize)) < osize); /* Check buffer not too big */
    UNUSED(osize);
    return buf_create_buffer_simple(msize);
}
