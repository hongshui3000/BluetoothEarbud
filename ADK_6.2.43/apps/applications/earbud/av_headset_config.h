/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_config.h
\brief      Application configuration file
*/

#ifndef _AV_HEADSET_CONFIG_H_
#define _AV_HEADSET_CONFIG_H_

/*! Time to wait before powering off when idle */
#define APP_AUTO_POWER_OFF_TIMEOUT      (300)

/*! Delay power off slightly for UI tones to complete */
#define APP_POWER_OFF_WAIT_MS           (D_SEC(1))

/*! Interval between checking if we are able to sleep */
#define APP_POWER_OFF_FORCE_MS          (D_SEC(5))

/*! Interval to retry dormant transition if failed. This
    should be relatively long as if we are in this state
    we should consume least power possible.
 */
#define APP_POWER_OFF_DORMANT_RETRY_MS  (D_SEC(30))

/*! Timeout for entering the case after selecting DFU in
    the UI (with no charger). Normally, can't choose UI
    options with a charger attached.
 */
#define APP_DFU_TIMEOUT_TO_PLACE_CASE   (D_SEC(60))

/*! Timeout for DFU mode, entered after a reboot when upgrade completed */
#define APP_DFU_TIMEOUT_REBOOT_TO_CONFIRM_MS (D_SEC(20))

/*! Timeout for DFU mode, entered after a reboot in DFU mode */
#define APP_DFU_TIMEOUT_REBOOT_IN_DFU_MS (D_SEC(15))

/*! Timeout for DFU mode, entered from UI */
#define APP_DFU_TIMEOUT_ENTERED_FROM_UI_MS (30000)

/*! Timeout for leaving DFU mode when no activity reported for a very long time*/
#define APP_DFU_TIMEOUT_NO_ACTIVITY_MS (120000)

/*! Timeout for DFU mode, entered from GAIA */
#define APP_DFU_TIMEOUT_ENTERED_FROM_GAIA_MS (20000)

/*! Default speaker gain */
#define HFP_SPEAKER_GAIN    (10)

/*! Default microphone gain */
#define HFP_MICROPHONE_GAIN (15)

/*! Auto connect HFP on power up */
#define AUTO_CONNECT_HFP

/*! Auto connect AV on power up */
#define AUTO_CONNECT_AV

/*! Auto answer call on connect */
#define HFP_CONNECT_AUTO_ANSWER

/*! Disable - Auto transfer call on connect */
#undef HFP_CONNECT_AUTO_TRANSFER

/*! Use 2 microphones for HFP phone calls */
//#define HFP_USE_2MIC

/*! Default BR/EDR Authenticated Payload Timeout */
#define DEFAULT_BR_EDR_AUTHENTICATED_PAYLOAD_TIMEOUT (10000)

/*! Maximum Authenticated Payload Timeout as per HFP spec */
#define HFP_AUTHENTICATED_PAYLOAD_TIMEOUT_SC_MAX_MS (10000)

#ifdef USE_BDADDR_FOR_LEFT_RIGHT
/*! Left and right earbud roles are selected from Bluetooth address. */
/*! TRUE if this is the left earbud (Bluetooth address LAP is odd). */
#define appConfigIsLeft()           (appGetInit()->appInitIsLeft)
/*! TRUE if this is the right earbud (Bluetooth address LAP is even). */
#define appConfigIsRight()          (appConfigIsLeft() ^ 1)
#else
/*! Left and right earbud roles are selected from the state of this PIO */
#define appConfigHandednessPio()    (2)
/*! TRUE if this is the left earbud (the #appConfigHandednessPio state is 1) */
#define appConfigIsLeft()           ((PioGet32Bank(appConfigHandednessPio() / 32) & (1UL << appConfigHandednessPio())) ? 1 : 0)
/*! TRUE if this is the right earbud (the #appConfigHandednessPio state is 0) */
#define appConfigIsRight()          (appConfigIsLeft() ^ 1)
#endif

/*! Number of paired devices that are remembered */
#define appConfigMaxPairedDevices() (4)

/*! Timeout in seconds for user initiated peer pairing */
#define appConfigPeerPairingTimeout()       (120)
/*! Timeout in seconds for user initiated handset pairing */
#define appConfigHandsetPairingTimeout()    (120)

/*! Timeout in seconds for automatic peer pairing */
#define appConfigAutoPeerPairingTimeout()       (0)
/*! Timeout in seconds for automatic handset pairing */
#define appConfigAutoHandsetPairingTimeout()    (0)

/*! Qualcomm Bluetooth SIG company ID */
#define appConfigBtSigCompanyId() (0x00AU)
/*! Qualcomm IEEE company ID */
#define appConfigIeeeCompanyId()  (0x00025BUL)

/*! Key ID peer Earbud link-key derivation */
#define appConfigTwsKeyId()       (0x74777332UL)

/*!@{ @name LED pin PIO assignments (chip specific)
      @brief The LED pads can either be controlled by the led_controller hardware
             or driven as PIOs. The following define the PIO numbers used to
             control the LED pads as PIOs.
*/
#define CHIP_LED_0_PIO 66
#define CHIP_LED_1_PIO 67
#define CHIP_LED_2_PIO 68
#define CHIP_LED_3_PIO 69
#define CHIP_LED_4_PIO 70
#define CHIP_LED_5_PIO 71
//!@}

#if defined(CF111_CF212) || defined(CE821_CF212) || defined(CF376_CF212)
/*! The number of LEDs av_headset_led will control. If one LED is configured,
    it will use appConfigLed0Pio(), if two LEDs are configured it will use
    appConfigLed0Pio() and appConfigLed1Pio() etc. */
#define appConfigNumberOfLeds()  (1)

/*! PIO to control LED0 */
#define appConfigLed0Pio()       CHIP_LED_0_PIO
/*! PIO to control LED1 (not used) */
#define appConfigLed1Pio()       (0)
/*! PIO to control LED2 (not used) */
#define appConfigLed2Pio()       (0)

#elif defined(CE821_CF429) || defined(CE821_CF440) || defined(CF111_CF429) || defined(CF111_CF440) || \
      defined(CF376_CF429) || defined(CF376_CF440) || defined(CF133)
/* The number of LEDs av_headset_led will control. */
#define appConfigNumberOfLeds()  (3)
/*! PIO to control LED0 */
#define appConfigLed0Pio()       CHIP_LED_0_PIO
/*! PIO to control LED1 */
#define appConfigLed1Pio()       CHIP_LED_1_PIO
/*! PIO to control LED2 */
#define appConfigLed2Pio()       CHIP_LED_2_PIO

#else
#error LED config not defined for this platform.
#endif

/*! Returns boolean TRUE if PIO is an LED pin, otherwise FALSE */
#define appConfigPioIsLed(pio) (((pio) >= CHIP_LED_0_PIO) && ((pio <= CHIP_LED_5_PIO)))
/*! Returns the LED number for a LED PIO. Assumes led is an LED PIO. */
#define appConfigPioLedNumber(pio) ((pio) - CHIP_LED_0_PIO)
/*! Product specific range of PIO that can wake the chip from dormant */
#define appConfigPioCanWakeFromDormant(pio) ((pio) >= 1 && ((pio) <= 8))

/*! Allow LED indications when Earbud is in ear */
#define appConfigInEarLedsEnabled() (TRUE)

/*! Page timeout to use as left earbud attempting connection to the right earbud. */
#define appConfigLeftEarbudPageTimeout()    (0x1000)

/*! Page timeout to use as right earbud attempting connection to the left earbud. */
#define appConfigRightEarbudPageTimeout()   (0x2000)

/*! Page timeout to use for connecting to any non-peer earbud devices. */
#define appConfigDefaultPageTimeout()       (0x4000)

/*! Inactivity timeout after which peer signalling channel will be disconnected, 0 to leave connected (in sniff) */
#define appConfigPeerSignallingChannelTimeoutSecs()   (0)

/*! Default link supervision timeout for all ACLs (in milliseconds) */
#define appConfigDefaultLinkSupervisionTimeout()  (5000)

/*! Minimum volume gain in dB */
#define appConfigMinVolumedB() (-45)

/*! Maximum volume gain in dB */
#define appConfigMaxVolumedB() (0)

/*! Default volume gain in dB */
#define appConfigDefaultVolumedB() (-10)

/*! The minimum time to play to be added on incoming SCO connections 
    to allow synchronisation. This should represent the total propagation 
    delay in the chain */
#define appConfigScoChainBaseTTP()      (15000)

/*! Time to play to be applied on this earbud, based on the Wesco
    value specified when creating the connection.
    A value of 0 will disable TTP.  */
#define appConfigScoChainTTP(wesco)     0

/*! Should the earbud automatically try to pair with a peer earbud
 * if no pairing exists? */
#define appConfigAutoPeerPairingEnabled()   (TRUE)

/*! Should the earbud automatically try to pair with a handset
 * if no pairing exists? */
#define appConfigAutoHandsetPairingEnabled()   (TRUE)

/*! Should the earbud automatically reconnect to last connected
 * handset on startup? */
#define appConfigAutoReconnectHandset()     (TRUE)

/*! The time before the TTP at which a packet should be transmitted */
#define appConfigTwsTimeBeforeTx()  (100000UL)

/*! The last time before the TTP at which a packet may be transmitted */
#define appConfigTwsDeadline()      (50000UL)

/*! Charger configuration */

/*! The time to debounce charger state changes */
#define appConfigChargerStateChangeDebounce()          (3)

/*! Trickle-charge current (mA) */
#define appConfigChargerTrickleCurrent()               (50)

/*! Pre-charge current (mA)*/
#define appConfigChargerPreCurrent()                   (50)
/*! Pre-charge to fast-charge threshold, see "Vfast threshold (B)" in datasheet
    for mapping between value and voltage */
#define appConfigChargerPreFastThresholdVoltage()      (1)

/*! Fast-charge current (mA) */
#define appConfigChargerFastCurrent()                  (100)
/*! Fast-charge (constant voltage) to standby transition point,
    see "Termination point (E)" in datasheet for mapping between value and voltage */
#define appConfigChargerTerminationCurrent()           (3)
/*! Fast-charge Vfloat voltage (mV) */
#define appConfigChargerTerminationVoltage()           (4200)

/*! Standby to fast charge hysteresis, see "Vhys threshold (D)" in datasheet
    for mapping between value and voltage */
#define appConfigChargerStandbyFastVoltageHysteresis() (3)


//!@{ @name Battery voltage levels in milli-volts
#define appConfigBatteryFullyCharged()      (4200)
#define appConfigBatteryVoltageOk()         (3600)
#define appConfigBatteryVoltageLow()        (3300)
#define appConfigBatteryVoltageCritical()   (3000)
//!@}

/*! Define which channel the 'left' audio channel comes out of. */
#define appConfigLeftAudioChannel()              (AUDIO_CHANNEL_A)

/*! Define which audio instance is used for microphone */
#define appConfigMicAudioInstance()              (AUDIO_INSTANCE_0)

/*! Time to wait for successful disconnection of links to peer and handset
 *  before forcing factory reset. */
#define appConfigFactoryResetTimeoutMs()        (5000)

/*!@{ @name External AMP control
      @brief If required, allows the PIO/bank/masks used to control an external
             amp to be defined.
*/
#if defined(CE821_CF212) || defined(CF376_CF212)

#define appConfigExternalAmpControlRequired()    (TRUE)
#define appConfigExternalAmpControlPio()         (32)
#define appConfigExternalAmpControlPioBank()     (1)
#define appConfigExternalAmpControlEnableMask()  (0 << 0)
#define appConfigExternalAmpControlDisableMask() (1 << (appConfigExternalAmpControlPio() % 32))

#else

#define appConfigExternalAmpControlRequired()    (FALSE)
#define appConfigExternalAmpControlPio()         (0)
#define appConfigExternalAmpControlEnableMask()  (0)
#define appConfigExternalAmpControlDisableMask() (0)

#endif /* defined(CE821_CF212) or defined(CF376_CF212) */
//!@}


#if !defined(CF133)

/*! Only enable LED indications when Earbud is out of ear */
#undef appConfigInEarLedsEnabled()
#define appConfigInEarLedsEnabled() (FALSE)

#endif /* !defined(CF133) */


/*! Timeout for A2DP audio when earbud removed from ear. */
#define appConfigOutOfEarA2dpTimeoutSecs()      (2)

/*! Timeout within which A2DP audio will be automatically restarted
 *  if placed back in the ear. */
#define appConfigInEarA2dpStartTimeoutSecs()    (10)

/*! Timeout for SCO audio when earbud removed from ear. */
#define appConfigOutOfEarScoTimeoutSecs()      (2)

/*! Time to wait to connect AVRCP after a remotely initiated A2DP connection
    indication if the remote device does not initiate a AVRCP connection */
#define appConfigAvrcpConnectDelayAfterRemoteA2dpConnectMs() D_SEC(3)

/*! Microphone path delay variation. */
#define appConfigMicPathDelayVariationUs()      (10000)

#if defined(INCLUDE_PROXIMITY)
#include "av_headset_proximity.h"
/*! The proximity sensor configuration */
extern const proximityConfig proximity_config;
/*! Returns the proximity sensor configuration */
#define appConfigProximity() (&proximity_config)
#endif /* INCLUDE_PROXIMITY */


#if defined(INCLUDE_ACCELEROMETER)
#include "av_headset_accelerometer.h"
/*! The accelerometer configuration */
extern const accelerometerConfig accelerometer_config;
/*! Returns the accelerometer configuration */
#define appConfigAccelerometer() (&accelerometer_config)
#endif /* INCLUDE_ACCELEROMETER */

#endif /* _AV_HEADSET_CONFIG_H_ */

