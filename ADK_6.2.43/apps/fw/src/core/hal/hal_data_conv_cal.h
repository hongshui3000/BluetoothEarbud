/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Header file for the calibration HAL layer of the data_conv driver.
 *
 * \section hal_data_conv_cal_introduction Introduction
 * The self calibration building blocks are implemented here. These are used by
 * the upper layer to perform one or more calibration operations.
 * Please see CS-318675-SP for more details.
 */

#ifndef _HAL_DATA_CONV_CAL_H_
#define _HAL_DATA_CONV_CAL_H_

#if CHIP_HAS_AUX_DATA_CONV

#include "hydra/hydra_types.h"

/**
 * Typical 6: covers +-7mv error range, 7: covers +-14mv error range
 */
#define CALIBRATION_LSB_OFFSET 7

/**
 * Starts the calibration procedure.
 */
extern void hal_data_conv_do_calibration(void);

/**
 * Cleans up after the calibration procedure.
 */
extern void hal_data_conv_stop_calibration(void);

/**
 * Gets the compensated SAR comparator offset. This is to be called after the
 * calibration completes.
 * \return Compensated SAR comparator offset.
 */
extern int8 hal_data_conv_get_compensated_sar_comparator_offset(void);

/**
 * Sets the SAR comparator offset.
 * \param sar_offset_signed SAR comparator offset.
 */
extern void hal_data_conv_set_sar_comparator_offset(int8 sar_offset_signed);

/**
 * Gets the calculated P and N errors.
 * \param use Inidicates which MSBs need correecting: 3 -> MSB-3 to MSB-0,
 * 2 -> MSB-2 to MSB-0, 1 -> MSB-1 to MSB-0, 0 -> MSB-0.
 * \param error_comp_p Pointer to buffer to store the calculated P errors.
 * \param error_comp_n Pointer to buffer to store the calculated N errors.
 */
extern void hal_data_conv_get_error_comp_p_n(uint8 use,
                                             int8 *error_comp_p,
                                             int8 *error_comp_n);

/**
 * Sets the P and N errors.
 * \param error_p Pointer to buffer where the calculated P errors are stored.
 * \param error_n Pointer to buffer where the calculated N errors are stored.
 */
extern void hal_data_conv_set_msb_error_correction(int8 *error_p,
                                                   int8 *error_n);

/**
 * Gets the offset caused by the redundancy bits.
 * \return Redundancy bits offset.
 */
extern int16 hal_data_conv_get_offset_due_to_redundancy(void);

#endif /* CHIP_HAS_AUX_DATA_CONV */

#endif /* _HAL_DATA_CONV_CAL_H_ */
