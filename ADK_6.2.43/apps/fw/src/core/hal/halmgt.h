/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file 
 * Hardware abstraction layer for chip management
 *
 * Control the CSR chip's management hardware. This is related to figuring out 
 * if firmware is running on a chip or on an emulator.
 */

#ifndef HALMGT_H
#define HALMGT_H

#include "io/io.h"

/**
 * Checks if formware is running on an emulator or on a chip.
 */
#define hal_running_on_emulator() (hal_get_chip_version_variant() == 0xF)

#endif /* HALMGT_H */
