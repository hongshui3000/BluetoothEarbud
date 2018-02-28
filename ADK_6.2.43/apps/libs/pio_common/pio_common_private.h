/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    pio_common_private.h

DESCRIPTION
    Private pio_common functions shared throughout the library.

*/

#ifndef PIO_COMMON_PRIVATE_H_
#define PIO_COMMON_PRIVATE_H_

#define NUMBER_OF_PIOS_PER_BANK    (32U)
#define PIO_NONE                   (0UL)
#define PIO_SUCCESS                PIO_NONE

#define PIO_BANK(pio)   (uint16)((pio)/NUMBER_OF_PIOS_PER_BANK)
#define PIO_COMMON_ASSERT(x) PanicNull(x)

/*!
    @brief pioCommonGetMask, get the mask for a given pio

    @return uint32, the mask of the pio
*/
uint32 pioCommonGetMask(uint8 pio);

#endif /* PIO_COMMON_PRIVATE_H_ */

