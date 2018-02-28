/* Copyright (c) 2016 - 2017 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Calculate pmalloc pool configuration
 */

#include "pmalloc/pmalloc_private.h"
#include "pmalloc/pmalloc_config_P1.h"
#include "hydra_log/hydra_log.h"

/* Largest size of extra blocks */
#define PM_MAX_SIZE_EXTRA 127
/* Pools with slightly different sizes will be combined
 * if the sizes differ by ratio < (1/POOL_COMBINE_FRAC_ERROR)
 * Keep this power-of-two for efficiency: this is used as a 
 * divisor */
#define POOL_COMBINE_FRAC_ERROR 32

static unsigned add_pmalloc_config(pmalloc_pool_config *combined_pool_config);
static void sort_pmalloc_config(pmalloc_pool_config *pool_config, unsigned config_len);
static void combine_pmalloc_sizes(pmalloc_pool_config *pool_config, unsigned *config_len);

/**
 * Construct fixed pool memory allocation configuration,
 * based on summing the contributions from known sources
 * defined in pmalloc_config_P0 or pmalloc_config_P1.h depending on
 * compile-flags
 * Partition the memory into pools based on "configuration".
 * Then allocate the remaining heap to the given pools 
 * Doesn't set up the pool control blocks at this stage
 */
bool get_pmalloc_config(pmalloc_pool_config *pool_config,unsigned *pool_config_len)
{
    unsigned i;
    size_t config_reqd;
    size_t heap_size;
    size_t config_avail;
    size_t tot_block_size;
    size_t num_extra_blocks;
    pmalloc_pool_config* pool_config_ptr;
    unsigned pool_len;
   
    /* Initialise pool_config array to zero */
    pool_config_ptr = pool_config;    
    for (i = 0; i < MAX_NUM_POOLS; i++)
    {
        pool_config_ptr->size = 0;
        pool_config_ptr->blocks = 0;
        ++pool_config_ptr;        
    }
 
    /* Accumulate a pool-configuration from the configured one */
    pool_len = add_pmalloc_config(pool_config);
   
    /* Sort it into ascending order */
    sort_pmalloc_config(pool_config, pool_len);

#ifndef PMALLOC_DONT_COMBINE_POOLS  
    /* Combine pools whose sizes differ by only a small amount */                       
    combine_pmalloc_sizes(pool_config, &pool_len);
#endif    
   
    /* Calculate the space used by the config */
    config_reqd = 0;
    pool_config_ptr = pool_config;
    tot_block_size = 0;    
    for (i = 0; i < pool_len; ++i)
    {  
        config_reqd += pool_config_ptr->size * pool_config_ptr->blocks;
        if ((pool_config_ptr->size) <= PM_MAX_SIZE_EXTRA)
        {
            tot_block_size += pool_config_ptr->size;
        }
        ++pool_config_ptr;        
    }   
    
    /* Calculates the un-allocated remainder of the heap 
     * that can be given additional blocks spread across the defined pools */
#ifdef NO_PMALLOC_MEMMAP_SYMBOLS
    heap_size = PMALLOC_RAM_SIZE; 
#else    
    heap_size =  (size_t)pmalloc_end - (size_t)pmalloc_begin;
#endif 
    if (heap_size < config_reqd)
    {
        panic(PANIC_HYDRA_INVALID_PRIVATE_MEMORY_CONFIG);
    }
    config_avail = (heap_size - config_reqd);
    /* Subtract space required for pool-control blocks */
    size_t pcb_size = (sizeof(pmalloc_pool) + PMALLOC_ALIGN_BOUNDARY)*pool_len;
    if (config_avail < pcb_size)
    {
        panic(PANIC_HYDRA_INVALID_PRIVATE_MEMORY_CONFIG);
    }
    config_avail -= pcb_size;
   
#ifndef PMALLOC_FIXED_CONFIG_ONLY     
    /* Only consider adding blocks if there are some non-zero pools */
    if (tot_block_size > 0)
    {
        /* Calculate number of blocks per pool, assumed added over the available pools */
        num_extra_blocks = config_avail / tot_block_size;
        
        /* Spread blocks evenly over the available pools */
        pool_config_ptr = pool_config;
        for (i = 0; i < pool_len; ++i)
        {
            if (pool_config_ptr->size <= PM_MAX_SIZE_EXTRA)
            {
                pool_config_ptr->blocks += num_extra_blocks;
                config_avail -= num_extra_blocks * pool_config_ptr->size;
            }
            pool_config_ptr++;
        }
    
        /* Any spare space, add one block per pool until we run out */
        pool_config_ptr = pool_config;
        for (i = 0; i < pool_len; ++i)
        {
            if (pool_config_ptr->size < config_avail)
            {
                pool_config_ptr->blocks++;
                config_avail -=  pool_config_ptr->size;             
            }
            pool_config_ptr++;
        }
        
        /* Finally, fill up the remaining space with blocks in the smallest pool-size */
        num_extra_blocks = config_avail / pool_config->size;
        pool_config->blocks += num_extra_blocks;
    }       
#endif    
     
    /* Re-calculate the space used by the config */
    config_reqd = 0;
    pool_config_ptr = pool_config;  
    for (i = 0; i < pool_len; ++i,++pool_config_ptr)
    {  
        config_reqd += pool_config_ptr->size * pool_config_ptr->blocks;        
    } 
    config_reqd += (sizeof(pmalloc_pool) + PMALLOC_ALIGN_BOUNDARY)*pool_len;
    *pool_config_len = pool_len;

    return(config_reqd <= heap_size); 
    
}


/** Sort a pool_config[] into order of block_size
 * lowest block_size first
 *  
 * \param pool_config The pool-configuration to be sorted
 * \param config_len The length of non-zero entries in pool_config
 */
static void sort_pmalloc_config(pmalloc_pool_config *pool_config, unsigned config_len)
{
    unsigned i;
    unsigned j;
    pmalloc_pool_config tmp_pool;
    pmalloc_pool_config *pool_config_ptr_i;
    pmalloc_pool_config *pool_config_ptr_j;
    size_t min_size;
    unsigned min_pos;          

    /* Trivial bubble-sort, O(n^2), but acceptable because 
     * MAX_NUM_POOLS is small */
    pool_config_ptr_i = pool_config;
    for (i = 0; i < (config_len - 1); i++)
    {
        min_pos = i;
        min_size = pool_config_ptr_i->size;
        pool_config_ptr_j = pool_config_ptr_i;
        
        /* Find the minimum size at and above the i'th
         * position in the list */
        for (j = i; j < config_len; j++)
        {
            if ((pool_config_ptr_j->size) < min_size)
            {
                min_size =  pool_config_ptr_j->size;
                min_pos = j;             
            }
            pool_config_ptr_j++;
        }

        /* Swap the i'th element with the minimum */
        tmp_pool = *pool_config_ptr_i;
        (*pool_config_ptr_i) = pool_config[min_pos];
        pool_config[min_pos] = tmp_pool;
         
        ++pool_config_ptr_i;
    }
}

 /** Select configuration defined in pools_reqd in pmalloc_config_P0.h or  
 * pmalloc_config_P1.h, depending on where it is compiled
 * Copy it into an output "combined_pool_config", but combining pools in
 * this way: where pools have the same size, accumulate them to a single
 * pool adding together the number of blocks.  
 * Length of input configuration is arbitrary, but length of output is
 * limited to MAX_NUM_POOLS, and panics if it doesn't fit. 
 * Also, it clips the pool-size to within the allowed range
 * [PMALLOC_MIN_BLOCK, PMALLOC_MAX_BLOCK]
 * 
 * \param pool_config the configuration we are accumulating into
 * \return The length of combined_pool_config
 */
static unsigned add_pmalloc_config(pmalloc_pool_config *pool_config)
{
    unsigned i;
    unsigned j;
    unsigned config_entries_counter;
    size_t array_len;
    size_t new_size;
    size_t new_blocks;
    size_t current_size;
    bool inserted;
    pmalloc_pool_config *pool_config_ptr;
    pmalloc_pool_config *pools_reqd_ptr;
    
    array_len = sizeof(pools_reqd) / sizeof(pools_reqd[0]);
    pools_reqd_ptr = pools_reqd;

    config_entries_counter = 0;
    for (i = 0; i < array_len; ++i, ++pools_reqd_ptr)
    {   
        /* Attempt to insert a new pool config element */
        inserted = FALSE;

        /* Limit the size of the pool to the allowed range */
        new_size = MIN(MAX(pools_reqd_ptr->size, PMALLOC_MIN_BLOCK), PMALLOC_MAX_BLOCK);
        new_blocks = pools_reqd_ptr->blocks;
        
        /* Ignore the pool-entry if the block-size equals zero */  
        if (new_blocks)
        {
            /* iterate over the locations in pool_config where we
             * might be able to add/accumulate the new pool */
            pool_config_ptr = pool_config;
            for (j = 0;j < config_entries_counter; ++j,++pool_config_ptr)
            {
                current_size = pool_config_ptr->size; 
                /* If the pool-size at this position is the same, accumulate it in */                
                if (current_size == new_size)
                {
                    pool_config_ptr->blocks += new_blocks;
                    inserted = TRUE;
                    break;             
                }              
            }

            if (!inserted)
            {
                /* Add the configuration to the end of the list */      
                if (config_entries_counter < MAX_NUM_POOLS)
                {
                    pool_config_ptr = &pool_config[config_entries_counter];
                    pool_config_ptr->blocks = new_blocks;
                    pool_config_ptr->size = new_size;
                    config_entries_counter++;
                }
                else
                {
                /* If there is no room to insert more configurations (more pools than 
                 * MAX_NUM_POOLS) then PANIC - this is an invalid configuration */
                    panic(PANIC_HYDRA_INVALID_PRIVATE_MEMORY_CONFIG);   
                }            
            }
        }
    }
    if (config_entries_counter == 0)
    {
        /* No non-zero configuration was found */ 
        panic(PANIC_HYDRA_INVALID_PRIVATE_MEMORY_CONFIG);   
    }               
    return(config_entries_counter);
}

/** Combine pools with close sizes, such that combining the pools
 * makes a "small" difference to the memory required, but decreases
 * the number of pools. Decreasing the number of pools will speed up
 * pmalloc(). So this is a speed vs size tradeoff, determined by
 * POOL_COMBINE_FRAC_ERROR: larger val => faster, but more memory
 * \param pool_config The pool-configuration to be sorted
 * \param config_len The length of non-zero entries in pool_config[]
 */
static void combine_pmalloc_sizes(pmalloc_pool_config *pool_config, unsigned *config_len)
{
    unsigned i;
    unsigned j;
    unsigned config_end_index;
    size_t current_reqd_mem;
    size_t reqd_mem_increase;
    pmalloc_pool_config *lower_pool;
    pmalloc_pool_config *higher_pool;   
    
    config_end_index = (*config_len) - 1;
    /* Starting from the largest sizes, to avoid combining multiple 
     * times iteratively, making large overall size change by several 
     * "small" pool-combinations */
    for (i = config_end_index; i > 0;i--)
    {
        higher_pool = &pool_config[i];
        lower_pool = higher_pool - 1;
        current_reqd_mem = (higher_pool->blocks * higher_pool->size) 
                           + (lower_pool->blocks * lower_pool->size);
        reqd_mem_increase = lower_pool->blocks * (higher_pool->size - lower_pool->size);
                    
        /* If the increase in total size required by combining is less than a 
         * small fraction, combine the pools into one with the larger block-size
         * Express the fractional error as a divisor rather than multiplier
         * to prevent overflow issues */
        if (reqd_mem_increase < (current_reqd_mem / POOL_COMBINE_FRAC_ERROR))
        {
            lower_pool->blocks += higher_pool->blocks;
            lower_pool->size = higher_pool->size;
            (*config_len)--;
            /* shuffle-down the larger pools to fill the gap in the list */
            lower_pool = higher_pool;
            higher_pool = lower_pool + 1;
            for (j= i; j < config_end_index; j++)
            {
                lower_pool->blocks = higher_pool->blocks;
                lower_pool->size = higher_pool->size;
                lower_pool++;
                higher_pool++;
            }
        }
    }
}

