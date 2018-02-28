/*!
\copyright  Copyright (c) 2018 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_config.c
\brief	    Config data
*/

#if defined(INCLUDE_PROXIMITY)
#if   defined(HAVE_VNCL3020)

#include "peripherals/vncl3020.h"
const struct __proximity_config proximity_config = {
    .threshold_low = 3000,
    .threshold_high = 3500,
    .threshold_counts = vncl3020_threshold_count_4,
    .rate = vncl3020_proximity_rate_7p8125_per_second,
    .i2c_clock_khz = 100,
    .pios = {
#if defined(CF111_CF429) || defined(CF111_CF440)
        .on = 5,
        .i2c_scl = 20,
        .i2c_sda = 21,
        .interrupt = 71,
#elif defined(CF111_CF212)
        .on = 5,
        .i2c_scl = 48,
        .i2c_sda = 49,
        .interrupt = 71,
#else
#error INCLUDE_PROXIMITY and HAVE_VNCL3020 were defined, but no valid platform type was defined.
#endif
    },
};

#else
#error INCLUDE_PROXIMITY was defined, but no proximity sensor type was defined.
#endif   /* HAVE_VNCL3020 */
#endif /* INCLUDE_PROXIMITY */

#if defined(INCLUDE_ACCELEROMETER)
#if   defined(HAVE_ADXL362)

#include "peripherals/adxl362.h"
const struct __accelerometer_config accelerometer_config = {
    /* 250mg activity threshold, magic value from datasheet */
    .activity_threshold = 0x00FA,
    /* 150mg activity threshold, magic value from datasheet */
    .inactivity_threshold = 0x0096,
    /* Inactivity timer is about 5 seconds */
    .inactivity_timer = 30,
    .spi_clock_khz = 400,
    .pios = {
#if defined(CF111_CF429) || defined(CF111_CF440)
        .on = 15,
        .spi_clk = 16,
        .spi_cs = 17,
        .spi_mosi = 18,
        .spi_miso = 19,
        .interrupt = 70,
#elif defined(CF111_CF212)
        .on = 20,
        .spi_clk = 16,
        .spi_cs = 17,
        .spi_mosi = 18,
        .spi_miso = 19,
        .interrupt = 67,
#else
#error INCLUDE_ACCELEROMETER and HAVE_ADXL362 were defined, but no valid platform type was defined.
#endif
    },
};
#else
#error INCLUDE_ACCELEROMETER was defined, but no accelerometer type was defined.
#endif   /* HAVE_ADXL362*/
#endif /* INCLUDE_ACCELEROMETER */
