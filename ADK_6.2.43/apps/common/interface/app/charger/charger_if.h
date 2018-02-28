/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/****************************************************************************
FILE
    charger_if.h

CONTAINS
    Definitions for the charger subsystem.

DESCRIPTION
    This file is seen by the stack, and VM applications, and
    contains things that are common between them.
*/

#ifndef __APP_CHARGER_IF_H__
#define __APP_CHARGER_IF_H__

/*! @brief Status of the charger hardware as returned by the ChargerStatus()
    trap.
*/
typedef enum
{
    /*! The charger has no input power. */       
    NO_POWER,

    /*! The battery is flat and being charged slowly. */             
    TRICKLE_CHARGE,

    /*! The battery is in pre charge state. */
    PRE_CHARGE,

    /*! The battery is being charged quickly. */
    FAST_CHARGE,

    HEADROOM_ERROR,

    VBAT_OVERVOLT_ERROR,

    /*! The battery is full and not being charged. */
    STANDBY,

    /*! The charger is disabled or in an unreliable state. */
    DISABLED_ERROR,

    /*! Charger could not be configured: Unknown reason. */
    CONFIG_FAIL_UNKNOWN,

    /*! Charger could not be configured: Charger is currently enabled. */
    CONFIG_FAIL_CHARGER_ENABLED,

    /*! Charger could not be configured: Invalid primary eFuse CRC. */ 
    CONFIG_FAIL_EFUSE_CRC_INVALID, 
    
    /*! Charger could not be configured: One or more charger eFuse trims are zero. */
    CONFIG_FAIL_EFUSE_TRIMS_ZERO,
    
    /*! Charger could not be configured: One or more requested charge currents are zero. */
    CONFIG_FAIL_CURRENTS_ZERO, 
    
    /*! Charger could not be configured: One or more requested values or eFuse trims are out of range. */
    CONFIG_FAIL_VALUES_OUT_OF_RANGE, 
    
    /*! Charger could not be enabled/disabled: Unknown reason. */
    ENABLE_FAIL_UNKNOWN,

    /*! Charger could not be enabled: Invalid primary eFuse CRC. */
    ENABLE_FAIL_EFUSE_CRC_INVALID,

    /*! Internal current source LED configure failed */
    INTERNAL_CURRENT_SOURCE_CONFIG_FAIL
} charger_status;

/*! @brief Charger event enables that can be passed to ChargerDebounce() */
typedef enum
{
    CHARGER_VREG_EVENT    = (1 << 0),
    CHARGER_CONNECT_EVENT = (1 << 1)
}charger_events;

/*! @brief Charger config keys - used to configure the charger.

For BlueCore chips which include battery charger hardware, the data
sheet will include a "Battery Charger" section which gives electrical
characteristics of the charger.
The "I-CTRL" values in this section should be used to set the
#CHARGER_CURRENT.
The Trickle charger voltage threshold and Float voltage for pre BlueCore7
chips are both set using #CHARGER_TRIM. On BlueCore7 and later devices
the Trickle charger voltage threshold and Float voltage are both set using
#CHARGER_TERM_VOLTAGE
 */
typedef enum
{

    /*!
When using this key, a zero value will disable the charger and
a non zero value will enable the charger.
     */
    CHARGER_ENABLE,

    /*!
Used to set the trim on pre BlueCore7 chips which have an onchip
battery charger. Using this key overrides any setting in PSKEY_CHARGER_TRIM.
Valid values are in the range 0..15 and the exact mapping from value to voltage
is chip dependent. Consult the datasheet for your chip.
     */
    CHARGER_TRIM,

    /*!
Used to set the charger current on BlueCore chips which have an onchip
battery charger. Using this key overrides any setting in
PSKEY_CHARGER_CURRENT. On BlueCore7 and later devices, the value
passed specifies the required charger current in mA and the supported
range is device dependent. Any invalid value results in rounding to
lower or upper limits. On earlier devices, Valid values are in the
range 0..15 and the exact mapping from value to current is chip
dependent. Consult the datasheet for your chip.
     */
    CHARGER_CURRENT,

    /*!
Used to remove control of LED0 from the battery charger
hardware and give control to the VM application. A zero value gives control
to the battery charger hardware (which may automatically flash the LED when
the battery charger is in operation) while a non zero value gives control to
the VM application.
     */
    CHARGER_SUPPRESS_LED0,

    /*!
Some BlueCore variants with battery charger hardware
have an option to boost the charging current. This function allows the 
application to switch the boost on or off. For this function to have any effect,
PSKEY_CHARGER_ENABLE_BOOST must be set to TRUE. In this case, as the
charger boost hardware requires use of the LED1 pin, the normal use of
LED1 is not available.
     */
    CHARGER_ENABLE_BOOST,

    /*!
Some BlueCore variants with battery charger hardware have
an option to use an external power transistor to boost the available charger current.
CHARGER_USE_EXT_TRAN selects between internal and external modes.  In
internal mode the current is limited but adjustable using CHARGER_CURRENT.
In external mode the current is fixed (set by a resistor on the PCB).
     */
    CHARGER_USE_EXT_TRAN,

    /*!
On BlueCore7 chips it is possible to reset the CPU on a charger-attach
event, this being enabled/disabled by PSKEY_RESET_ON_CHARGER_ATTACH.
In addition to the PSKEY, the VM may also independently enable or
disable the Charger-Attach-Reset using this VM Trap.
     */
    CHARGER_ATTACH_RESET_ENABLE,

    /*!
Used to set the termination voltage on Bluecore7 and later chips which
have on chip battery charger. The value passed to this function is in
millivolts and the supported range is device dependent. Any invalid
value results in rounding to lower or upper limits. The register
settings are calculated by using PSKEY_CHARGER_CALC_VTERM and
PSKEY_CHARGER_CALC_RTRIM for the given millivolts.
    */
    CHARGER_TERM_VOLTAGE,

    /*!
On some BlueCore variants like CSRB5730, this is being used to enable
the external FET to select higher external charge current when charger
hardware is configured for external mode.
    */
    CHARGER_ENABLE_HIGH_CURRENT_EXTERNAL_MODE,

    /*!
When in external charge mode, if the battery conditions are appropriate
for trickle charge the current used could be different to the current
used in internal trickle charge mode. Use this command to set the
trickle charge current value when in external mode. The value is in mA
and should be between 0 and 50. Values outside this range are ignored.
    */
    CHARGER_SET_EXTERNAL_TRICKLE_CURRENT,

    /*!
On some BlueCore variants like CSRB5730, When the battery has become
fully charged, the charger will entry its STANDBY phase. During this
phase it will monitor the battery's voltage. When it drops below a
threshold, it will restart fast charging. This threshold is configured
with the STANDBY_FAST_HYSTERESIS control.
The parameter is in millivolts and describes the threshold below VFLOAT
when the charger will begin charging again.
The limits are chip dependent.
On CSRB5730, the possible values are 120mV, 258mV, 344mV and 426mV below
VFLOAT. Attempting to set a value greater than 426mV will return FALSE 
and no changes will be made to the charger. Attempting to set a value
less than 120mV will cause the hysteresis to be set to 120mV below 
VFLOAT. Other values will cause the next lowest value from the list to
be selected.
    */
    CHARGER_STANDBY_FAST_HYSTERESIS,

    /*!
Used to set the current for trickle charger mode. 
     */
    CHARGER_TRICKLE_CURRENT,

    /*!
Used to set the current for pre charger mode. 
     */
    CHARGER_PRE_CURRENT,

    /*!
Used to set the current for fast charger mode. 
     */
    CHARGER_FAST_CURRENT,
    
    /*!
Used to set the external series resistor value. 
     */
    CHARGER_EXTERNAL_RESISTOR,

    /*!
When using this key, a zero value will select "internal" mode of the charger
and a non zero value will be "external" mode of the charger to allow 
charging large batteries at higher currents than the internal charger is capable of.
     */
    CHARGER_USE_EXTERNAL_RESISTOR_FOR_PRE_CHARGE,

    /*!
When using this key, a zero value will select "internal" mode of the charger
and a non zero value will be "external" mode of the charger to allow 
charging large batteries at higher currents than the internal charger is capable of.
     */
    CHARGER_USE_EXTERNAL_RESISTOR_FOR_FAST_CHARGE,

    /*!
Used to set the threshold to switch from Pre to Fast charge. 
     */
    CHARGER_PRE_FAST_THRESHOLD,

    /*!
Used to set the iterm current termination value. 
     */
    CHARGER_ITERM_CTRL,

    /*!
Used to set the debounce timer as the charger changes states eg. Pre to Fast, 
Fast to Standby based on current and voltage changes. 
     */
    CHARGER_STATE_CHANGE_DEBOUNCE
}charger_config_key;


/*! @brief Battery status keys - used to configure the charger.
*/
typedef enum
{
    CHARGER_BATTERY_UNKNOWN = 0,  /*!< The battery voltage was not checked  */
    CHARGER_BATTERY_OK = 1,       /*!< Battery voltage sufficient to use */
    CHARGER_BATTERY_DEAD = 2      /*!< Battery voltage too low at start (dead) */
} charger_battery_status;

/*! @brief PMU power source
*/
typedef enum
{
   PMU_EXTERNAL_SUPPLY = 0,
   PMU_BATTERY = 1
}pmu_power_source;

#endif /* __APP_CHARGER_IF_H__  */
