/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/****************************************************************************
FILE
    hid_if.h  - HID interface

CONTAINS
    The global definitions for the hidio subsystem.

DESCRIPTION
    This file is seen by the stack, and VM applications, and
    contains things that are common between them.
*/

#ifndef __APP_HID_IF_H__
#define __APP_HID_IF_H__

/*!
    @brief Configuration structure for SENSOR_AGILENT_AN2051.
*/
typedef struct
{
    uint16 size;    /*!< Size of structure. */    
    bool res_800;   /*!< Use 800dpi resolution. */
    bool swap_xy;   /*!< Swap X & Y coordinates. */
    bool neg_x;     /*!< Reverse X coordinates. */
    bool neg_y;     /*!< Reverse Y coordinates. */
} HID_AGILENT_AN2051_CONFIG;

/*!
    @brief Configuration structure for SENSOR_AGILENT_ADNS3030.
*/
typedef struct
{
    uint16 size;    /*!< Size of structure. */    
    bool res_800;   /*!< Use 800dpi resolution. */
    bool swap_xy;   /*!< Swap X & Y coordinates. */
    bool neg_x;     /*!< Reverse X coordinates. */
    bool neg_y;     /*!< Reverse Y coordinates. */
} HID_AGILENT_ADNS3030_CONFIG;

/*!
    @brief Configuration structure for Avago (was Agilent) ADNS5030 sensor.
*/
typedef struct
{
    uint16 size;    /*!< Size of structure. */    
    bool res_1000;  /*!< Use 1000dpi resolution. */
    bool swap_xy;   /*!< Swap X & Y coordinates. */
    bool neg_x;     /*!< Reverse X coordinates. */
    bool neg_y;     /*!< Reverse Y coordinates. */
} HID_AVAGO_ADNS5030_CONFIG;

/*!
    @brief Configuration structure for Avago (was Agilent) ADNS6030 sensor.
*/
typedef struct
{
    uint16 size;    /*!< Size of structure. */    
    bool res_800;   /*!< Use 800dpi resolution. */
    bool swap_xy;   /*!< Swap X & Y coordinates. */
    bool neg_x;     /*!< Reverse X coordinates. */
    bool neg_y;     /*!< Reverse Y coordinates. */
} HID_AVAGO_ADNS6030_CONFIG;

/*!
    @brief Configuration structure for Avago (was Agilent) ADNS7050 sensor.
*/
typedef struct
{
    uint16 size;    /*!< Size of structure. */    
    bool res_800;   /*!< Use 800dpi resolution. */
    bool swap_xy;   /*!< Swap X & Y coordinates. */
    bool neg_x;     /*!< Reverse X coordinates. */
    bool neg_y;     /*!< Reverse Y coordinates. */
} HID_AVAGO_ADNS7050_CONFIG;

/*!
    @brief Resolutions for PixArt PAN3201 sensor
*/
typedef enum
{
    pixart_res_400  = 0,
    pixart_res_500  = 1,
    pixart_res_600  = 2,
    pixart_res_800  = 3,
    pixart_res_1000 = 4,
    pixart_res_1200 = 5,
    pixart_res_1600 = 6
} pixart_res;

/*!
    @brief Configuration structure for a PixArt sensor.
*/
typedef struct
{
    uint16 size;    /*!< Size of structure. */    
    pixart_res resolution; /*!< Resolution to use. */
    bool swap_xy;   /*!< Swap X & Y coordinates. */
    bool neg_x;     /*!< Reverse X coordinates. */
    bool neg_y;     /*!< Reverse Y coordinates. */
} HID_PIXART_CONFIG;

/*!
    @brief Configuration structure for SENSOR_MATRIX.
*/
typedef struct
{
    uint16 size;    /*!< Size of structure. */
    uint16 rows;    /*!< Number of rows in matrix. */
    uint16 columns; /*!< Number of columns in matrix. */
    const uint16 *key_codes;    /*!< Pointer to key code lookup table. */
} HID_MATRIX_CONFIG;

/*! @brief Key types defintions used in key code table. */
#define HID_MATRIX_KEY_NORMAL    (0 << 8)  
#define HID_MATRIX_KEY_MODIFIER    (1 << 8)
#define HID_MATRIX_KEY_AUX        (2 << 8)

/*!
    @brief HID report types.  See HID profile 7.4.3, table 7 and
    7.4.4, table 8.  

    NOTE: These values must match those defined in the HID profile
    specification.
*/
typedef enum
{
    hid_report_other = 0,
    hid_report_input = 1,
    hid_report_output = 2,
    hid_report_feature = 3
} hid_report_type;

/*! @brief Bit mask used by hid_wheel_config enum */
#define HID_WHEEL_ACTIVE_HIGH_MASK (0x80)

/*!
    @brief HID wheel configuration enum. Used with SourceConfigure() to
    configure the HID source's mouse wheel.

    For example, SourceConfigure(StreamHidSource(), VM_SOURCE_HID_WHEEL,
    HID_WHEEL_FULL_PULSE_ACTIVE_LOW) will configure the HID source for a full
    quadrature wheel. The wheel enable PIO will be driven active low by
    BlueCore.

    Half Pulse A and Half Pulse B are two different types of half quadrature
    wheels.
*/
typedef enum
{
    HID_WHEEL_NONE = 0,
    HID_WHEEL_FULL_PULSE_ACTIVE_LOW = 1,
    HID_WHEEL_HALF_PULSE_A_ACTIVE_LOW = 2,
    HID_WHEEL_HALF_PULSE_B_ACTIVE_LOW = 3,
    HID_WHEEL_FULL_PULSE_ACTIVE_HIGH = 1 | HID_WHEEL_ACTIVE_HIGH_MASK,
    HID_WHEEL_HALF_PULSE_A_ACTIVE_HIGH = 2 | HID_WHEEL_ACTIVE_HIGH_MASK,
    HID_WHEEL_HALF_PULSE_B_ACTIVE_HIGH = 3 | HID_WHEEL_ACTIVE_HIGH_MASK
} hid_wheel_config;

#endif
