/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/****************************************************************************
FILE
    get_second_time.c  -  get a 32 bit second time.

CONTAINS
    get_second_time  -  get a 32 bit second time.
*/

#include "longtimer/longtimer_private.h"

/****************************************************************************
NAME
    get_second_time  -  get a 32 bit second time
*/
SECONDTIME get_second_time(void)
{
    longtimer t;

    get_long_milli_time(&t);

#ifndef BIGINT_64BIT_INTRINSIC
    return (SECONDTIME) uint64_lo32(uint64_div16(t,1000));
#else
    return (SECONDTIME) (t/1000U);
#endif
}

/* BCHS_EXPORT_POINT_END */
