/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_power.h
\brief	    Header file for the Power Management
*/

#ifndef __AV_HEADSET_POWER_H
#define __AV_HEADSET_POWER_H

/*! \brief Events that can block auto power off */
typedef enum power_event_masks
{
    APP_POWER_EVENT_NONE    = 0x00, /*< Always zero - Reset value */
    APP_POWER_EVENT_HFP     = 0x01, /*!< HFP active */
    APP_POWER_EVENT_A2DP    = 0x02, /*!< A2DP active */
    APP_POWER_EVENT_PAIRING = 0x04  /*!< Pairing or inquiry active */
} appPowerEventMask;

/*! \brief Type of power down that is active */
typedef enum power_powerdown_modes
{
    APP_POWER_LOW_POWER_MODE_NONE,      /*!< Reset & initialisation value */
    APP_POWER_LOW_POWER_MODE_DORMANT,   /*!< Attempting to enter dormant */
    APP_POWER_LOW_POWER_MODE_OFF        /*!< Attempting to power off */
} appPowerPowerdownType;

/*! \brief Function type used to get permission for reducing the power mode.

    Should be used in conjunction with \ref appPowerRegisterPowerDownCheck()
    if additional checks are necesary. Return TRUE to allow reduced
    power, or FALSE to reject.
 */
typedef bool (*allowPowerDownFunc)(void);

/*! @brief Power control module state. */
typedef struct
{
    TaskData                task;                   /*!< Task for Power control messages */
    appPowerEventMask       power_events_mask;      /*!< Events which can prevent auto power off */
    allowPowerDownFunc      allow_powerdown_fn;     /*!< Registered function used to check idle power down */
    appPowerPowerdownType   powerdown_type_wanted;  /*!< Type of powerdown we are processing */
    bool                    allow_dormant;          /*!< Flag that can be modified during testing to disable dormant */
    bool                    cancel_dormant;         /*!< Flag set during dormant transition to abandon it */
} powerTaskData;


#ifdef INCLUDE_POWER_CONTROL
/*! \brief Disable power off timer as a specific event is in progress.
    \param event_mask The event.

    This function is called to track events that dictate whether the
    headset should enable an automatic power off timer or not.

    The power off timer will be stopped following this call, and can
    only be started when this and all other events have finished and
    notified via a call to \ref appPowerOffTimerEnable().
*/
extern void appPowerOffTimerDisable(appPowerEventMask event_mask);

/*! \brief Enable power off timer if this (and all other events)
            have ended.
    \param event_mask The event.

    This function is called when tracking events that dictate whether the
    headset should enable an automatic power off timer or not.

    The power off timer may be started after this call, if there are no
    events currently being tracked.
*/
extern void appPowerOffTimerEnable(appPowerEventMask event_mask);

/*! \brief Update the auto power off timer

    This function is called to restart the power off timer (if any). If any
    events that we track are active, the timer will be cancelled. Otherwise
    the timer will be started.

    Once the timer fires the automatic power off sequence will be initiated.
*/
extern void appPowerOffTimerRestart(void);

/*! \brief Enter actions when we enter the asleep state.
 *
 * The application is expected to be shutting down imminently.
 */
extern void appPowerEnterAsleep(void);

/*! \brief Cancel an impending shutdown (if any)

    Cancels the sleep timer, and resets the powerdown type.
*/
extern void appPowerCancelInProgressDormant(void);
#else
/*! \brief Not implemented */
#define appPowerOffTimerDisable(msk)        UNUSED(msk)
/*! \brief Not implemented */
#define appPowerOffTimerEnable(msk)         UNUSED(msk)
/*! \brief Not implemented */
#define appPowerOffTimerRestart()           ((void)0)
/*! \brief Not implemented */
#define appPowerEnterAsleep()               ((void)0)
/*! \brief Not implemented */
#define appPowerCancelInProgressDormant()   ((void)0)
#endif

/*! \brief Power on device

    This function is called to request powering on the device.  Turning on the
    powersupply is handled  by the state machine.
*/
extern void appPowerOn(void);

/*! \brief Reboot headset

    This function is called when the power-off watchdog has expired, this
    means we have failed to shutdown after 10 seconds.

    We should now force a reboot.
*/
extern void appPowerReboot(void);

#ifdef INCLUDE_POWER_CONTROL

/*! \brief Initialise power control task

    Called at start up to initialise the data for power control,
    which is not a task at present.
*/
extern void appPowerControlInit(void);

/*! \brief Power off device

    This function is called from the UI to power off the device.

    Connections will be shut down cleanly if possible, but the device
    \b will be powered off.
*/
extern void appPowerOff(void);


/*! \brief Process a change in the connection state of our links

    Check if all links have been cleared and send a message to trigger an immediate
    shut down if so.
 */
extern void appPowerLinkStateUpdated(void);

/*! \brief Handle idle power-off timeout

    This function is called when an idle timer has expired indicating that it
    may be time to switch the device to a lower power operating mode.
*/
extern void appPowerHandleIdleTimer(void);

/*! \brief Register function to be called when we try an idle power
            down.
    \param pd_func The function to register.

    The registered function should return FALSE to block the
    power down activity.

    \note The idle power off timer will be restarted if power down
            is rejected.
 */
extern void appPowerRegisterPowerDownCheck(allowPowerDownFunc pd_func);

/*! \brief Enter actions when the soporific state is entered.

   The device will power off after a delay regardless of the
   links_still_to_be_disconnected parameter.

   If links need to be disconnected, then the timeout is longer.

   \param links_still_to_be_disconnected  Whether links are still being
                disconnected.
 */
extern void appPowerEnterSoporific(bool links_still_to_be_disconnected);
#endif /* INCLUDE_POWER_CONTROL */

#endif /* __AV_HEADSET_POWER_H */
