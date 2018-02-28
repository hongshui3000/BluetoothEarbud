/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_power.c
\brief      Power Management
*/

#include <panic.h>
#include <connection.h>
#include <ps.h>
#include <boot.h>
#include <dormant.h>

#include "av_headset.h"
#include "av_headset_log.h"
#include "av_headset_sm.h"
#include "av_headset_power.h"
#include "av_headset_ui.h"
#include "av_headset_charger.h"

/*! Internal messages */
enum power_internal_messages
{
                                            /*!  Power off timeout has expired */
    APP_POWER_INTERNAL_POWER_OFF_TIMEOUT = INTERNAL_MESSAGE_BASE, 
    APP_POWER_INTERNAL_POWER_OFF,           /*!< Power off now */ 
    APP_POWER_INTERNAL_AUTO_POWER_OFF,      /*!< Idle auto power off timer */
};

/*! Infinite loop, which will trigger a watchdog reset in time. */
#define FOREVER()   for(;;)

void appPowerOn(void)
{
    DEBUG_LOG("appPowerOn");
    
    /* Check if we are in limbo state (i.e logically powered off) */
    if (appSmIsSleepy())
    {
        /* Work our way back to operation */
        appSetState(APP_STATE_STARTUP);
    }

    /* Display power on LED pattern */
    appUiPowerOn();
}

void appPowerReboot(void)
{
    /* Reboot now */
    BootSetMode(BootGetMode());
    FOREVER();
}


#ifdef INCLUDE_POWER_CONTROL

/*! \brief Query if power is going to dormant mode */
#define appPowerGoingToDormant() (appSmIsSleepy() && \
                                  APP_POWER_LOW_POWER_MODE_DORMANT \
                                      == appGetPowerControl()->powerdown_type_wanted)

void appPowerEnterSoporific(bool links_to_be_disconnected)
{
    if (links_to_be_disconnected)
    {
        /* waiting for link disconnect completion,
         * start a timer to force reset if we fail to complete for some reason */
        MessageSendLater(appGetPowerTask(), APP_POWER_INTERNAL_POWER_OFF_TIMEOUT, 
                            NULL, APP_POWER_OFF_FORCE_MS);
    }
    else
    {
        /* No links being disconnected, so shorter delay  */
        MessageSendLater(appGetPowerTask(), APP_POWER_INTERNAL_POWER_OFF_TIMEOUT, 
                            NULL, APP_POWER_OFF_WAIT_MS);
    }
}

void appPowerLinkStateUpdated(void)
{
    if (!appConManagerAnyLinkConnected() && appHfpIsDisconnected() && appAvIsDisconnected())
    {
        MessageSend(appGetPowerTask(), APP_POWER_INTERNAL_POWER_OFF, NULL);
    }
}

void appPowerEnterAsleep(void)
{
     MessageSendLater(appGetPowerTask(), APP_POWER_INTERNAL_POWER_OFF_TIMEOUT, NULL, APP_POWER_OFF_DORMANT_RETRY_MS);
}

void appPowerOffTimerRestart(void)
{
    powerTaskData *thePower = appGetPowerControl();

    DEBUG_LOGF("appPowerOffTimerRestart. Flags 0x%X",thePower->power_events_mask);

    MessageCancelFirst(appGetPowerTask(), APP_POWER_INTERNAL_AUTO_POWER_OFF);

    /* Check if any events are active */
    if (APP_POWER_EVENT_NONE == thePower->power_events_mask)
    {
        /* (Re)start timer if all logged activity ceases */
        MessageSendLater(appGetPowerTask(), APP_POWER_INTERNAL_AUTO_POWER_OFF, 0, D_SEC(APP_AUTO_POWER_OFF_TIMEOUT));
    }
}

void appPowerOffTimerDisable(appPowerEventMask event_mask)
{
    powerTaskData *thePower = appGetPowerControl();

    thePower->power_events_mask |= event_mask;

    appPowerOffTimerRestart();
}

void appPowerOffTimerEnable(appPowerEventMask event_mask)
{
    powerTaskData *thePower = appGetPowerControl();

    thePower->power_events_mask &= ~event_mask;

    appPowerOffTimerRestart();
}


/*! \brief Start power down operations

    This function is called to enter the shutdown states and notify the
    UI.
*/
static void appPowerControlStartPowerDownMode(appPowerPowerdownType mode)
{
    powerTaskData *thePower = appGetPowerControl();

    thePower->powerdown_type_wanted = mode;
    thePower->cancel_dormant = FALSE;

    /* Display power off LED pattern */
    appUiPowerOff();

    /* Initiate shutdown through the state machine */
    appSmPowerDownDisconnect();


}

void appPowerOff(void)
{
    DEBUG_LOG("appPowerOff");

    /* Only attempt to power off if we can */
    if (appChargerCanPowerOff())
    {
        appPowerControlStartPowerDownMode(APP_POWER_LOW_POWER_MODE_OFF);
    }
    else
    {
        DEBUG_LOG("Charger connected, blocking power off");
        appUiError();
    }
}

void appPowerHandleIdleTimer(void)
{
    powerTaskData *thePower = appGetPowerControl();

    DEBUG_LOG("appPowerHandleIdleTimer");

    /* Only attempt to power off if we are actually powered on (ie active) */
    if (!appSmIsActive())
    {
        DEBUG_LOGF("Idle PowerDown aborted as the application is not in an active state (state %d)",appGetState());
        return;
    }

    if (APP_POWER_LOW_POWER_MODE_NONE != thePower->powerdown_type_wanted)
    {
        /* We shouldn't be trying to enter dormant if something else
         * has requested a powerdown. */
        Panic();
    }

    if (   thePower->allow_powerdown_fn
        && !(*thePower->allow_powerdown_fn)())
    {
        DEBUG_LOGF("Idle PowerDown aborted as not allowed (state:%d, phystate:%d)",appGetState(),appPhyStateGetState());
        appPowerOffTimerRestart();
        return;
    }

    /* Although a simpler check, make sure it is last so tests have the 
       chance to check other behaviours */
    if (!thePower->allow_dormant)
    {
        DEBUG_LOG("DORMANT blocked by test command");
        return;
    }

    appPowerControlStartPowerDownMode(APP_POWER_LOW_POWER_MODE_DORMANT);
}

/*! \brief Enter dormant mode

    This function is called internally to enter dormant.
*/
static void appPowerControlEnterDormantMode(void)
{
#ifdef INCLUDE_ACCELEROMETER
    powerTaskData *thePower = appGetPowerControl();
    dormant_config_key key;
    uint32 value;
    /* Register to ensure accelerometer is active */
    appAccelerometerClientRegister(&thePower->task);
    if (appAccelerometerGetDormantConfigureKeyValue(&key, &value))
    {
        /* Since there is only one dormant wake source (not including sys_ctrl),
           just apply the accelerometer's key and value */
        PanicFalse(DormantConfigure(key, value));
    }
    else
    {
        appAccelerometerClientUnregister(&thePower->task);
    }
#endif

    /* An active charge module blocks dormant, regardless of whether
       it has power */
    appChargerForceDisable();

    /* Make sure dormant will ignore any wake up time */
    PanicFalse(DormantConfigure(DEADLINE_VALID,FALSE));

    /* Enter dormant */
    PanicFalse(DormantConfigure(DORMANT_ENABLE,TRUE));

    /* If we happen to get here then Dormant didn't work,
     * so make sure the charger is running again (if needed)
     * so we could continue. */
    appChargerRestoreState();
    Panic();
}

/*! \brief Enter power off

    This function is called internally to enter the power off mode.
*/
static void appPowerControlDoPowerOff(void)
{
    /* No need to disable charger for power down, but if a charger is connected
       we will fail. This status should have been checked when the power off 
       command was received */
    PsuConfigure(PSU_ALL, PSU_ENABLE, FALSE);

    DEBUG_LOG("Turning off power supplies was ineffective?");

    FOREVER();
}


/*! \brief Power-off timeout expired

    This function is called when the power-off timeout has expired.
    It is responsible for calling appropriate functions to power off
    by either rebooting (DFU), entering dormant, or powering down.

    There is a small window during which something could have happened that
    has the effect of cancelling a transition to dormant. An example would 
    be movement of the earbud. This is handled here, in the timeout, to avoid
    any race conditions elsewhere.
*/
static void appPowerHandleInternalPowerOffTimeout(void)
{
    powerTaskData *thePower = appGetPowerControl();

    DEBUG_LOG("appPowerHandleInternalPowerOffTimeout");

    if (!appSmIsSleepy())
    {
        DEBUG_LOGF("appPowerHandleInternalPowerOffTimeout called when not in a sleep state. State:%d",appGetState());
        Panic();
    }

    if (APP_POWER_LOW_POWER_MODE_DORMANT == thePower->powerdown_type_wanted)
    {
        if (!thePower->cancel_dormant)
        {
            appPowerControlEnterDormantMode();
        }
        else
        {
            appPowerCancelInProgressDormant();
            appSetState(APP_STATE_STARTUP);
        }
    }
    else if (APP_POWER_LOW_POWER_MODE_OFF == thePower->powerdown_type_wanted)
    {
        appPowerControlDoPowerOff();
    }
}

void appPowerRegisterPowerDownCheck(allowPowerDownFunc pd_func)
{
    powerTaskData *thePower = appGetPowerControl();

    thePower->allow_powerdown_fn = pd_func;
}

void appPowerCancelInProgressDormant(void)
{
    powerTaskData *thePower = appGetPowerControl();

    MessageCancelFirst(appGetPowerTask(), APP_POWER_INTERNAL_POWER_OFF_TIMEOUT);
    thePower->powerdown_type_wanted = APP_POWER_LOW_POWER_MODE_NONE;
    thePower->cancel_dormant = FALSE;

    appPowerOffTimerRestart();
}


/*! \brief Handle a change in the earbuds physcal state 

    We only need to action this if we were in the process of going to 
    sleep (dormant) and the state indicates that we should awake.

    In other cases we treat this as evidence that the earbud was active
    and just restart the power off timer.
*/
static void appPowerHandlePhyStateChanged(const PHY_STATE_CHANGED_IND_T *message)
{
    if (appPowerGoingToDormant())
    {
        if (   PHY_STATE_OUT_OF_EAR == message->new_state
            || PHY_STATE_IN_EAR == message->new_state)
        {
            appGetPowerControl()->cancel_dormant = TRUE;
        }
    }
    /* The OUT_OF_EAR_AT_REST state indicates that movement has stopped. This is
       normally just the end of transition from ear to table, so nothing new 
       requiring idle timer to be restarted. */
    else if (PHY_STATE_OUT_OF_EAR_AT_REST != message->new_state)
    {
        appPowerOffTimerRestart();
    }
}

/*! \brief Handle the charger becoming attached

    We only need to action this if we were in the process of going to 
    sleep (dormant) as we should now postpone.

    In other cases restart the power off timer as, with a charger, we
    won't be sleeping for a while.
*/
static void appPowerHandleChargerAttached(void)
{
    if (appPowerGoingToDormant())
    {
        appGetPowerControl()->cancel_dormant = TRUE;
    }
    else
    {
        appPowerOffTimerRestart();
    }
}


/*! @brief Power control message handler
 */
static void appPowerHandleMessage(Task task, MessageId id, Message message)
{
    UNUSED(task);
    UNUSED(message);

    switch (id)
    {
        case APP_POWER_INTERNAL_POWER_OFF_TIMEOUT:
        case APP_POWER_INTERNAL_POWER_OFF:
            appPowerHandleInternalPowerOffTimeout();
            return;
            
        case APP_POWER_INTERNAL_AUTO_POWER_OFF:
            appPowerHandleIdleTimer();
            return;

        case PHY_STATE_CHANGED_IND:
            appPowerHandlePhyStateChanged((const PHY_STATE_CHANGED_IND_T *)message);
            break;

        case CHARGER_MESSAGE_ATTACHED:
            appPowerHandleChargerAttached();
            break;
    }
}

void appPowerControlInit(void)
{
    powerTaskData *thePower = appGetPowerControl();

    memset(thePower, 0, sizeof(*thePower));

    thePower->task.handler = appPowerHandleMessage;
    thePower->allow_dormant = TRUE;
    thePower->cancel_dormant = FALSE;

    appPhyStateRegisterClient(appGetPowerTask());
    appChargerClientRegister(appGetPowerTask());
}
#endif /* INCLUDE_POWER_CONTROL */

