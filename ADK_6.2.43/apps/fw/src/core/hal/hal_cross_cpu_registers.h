/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 *
 * Allow Processor 0 to access P1 registers.
 *
 * The P0 memory map contains a region in which P0 can access P1's registers.
 * This region got moved and resized when the memory was re-organised for
 * Crescendo D01.
*/
#ifndef HAL_CROSS_REG_H_
#define HAL_CROSS_REG_H_
/**
 * Macro to convert a P1 register address into its equivalent in P0 memory map.
 *
 * The start of the region PxD_CROSS_CPU_REGS_LOWER and 
 * the end of the region PxD_CROSS_CPU_REGS_UPPER gets imported from digits.
 * the size of the region PxD_CROSS_CPU_REGS_ADDR_MASK is either imported from the
 * digits when available or calculated.
 *
 */
#endif /* HAL_CROSS_REG_H_ */

