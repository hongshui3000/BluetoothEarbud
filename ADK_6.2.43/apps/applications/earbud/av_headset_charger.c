/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_charger.c
\brief      Charger monitoring
*/


#include "av_headset.h"
#include "av_headset_ui.h"
#include "av_headset_charger.h"
#include "av_headset_log.h"

/*! \brief Make sure that the charger is disabled

    This function should be called from power off code to make sure that
    the charger is off. The function is available even if the application
    does not normally support the charger as the charger cannot be enabled
    if we wish to power down.  The charger should never have been enabled.
*/
void appChargerForceDisable(void)
{
    ChargerConfigure(CHARGER_ENABLE, 0);
}

/**************************************************************************/
/*! /brief Check if we KNOW that the system can power off.

    This function should only be called if the charger is enabled (should
    always be), and checks if there is any power detected.

    If the power status is not known, the function assumes that we cannot 
    power off. An example would be a charger status of #DISABLE_ERROR.
    
    \note If charger support has not been included, then this function
    will return FALSE as the system behaviour is not known/detectable.

    \returns TRUE if it is certain there is no external power
 */
bool appChargerCanPowerOff(void)
{
#ifdef INCLUDE_CHARGER
    switch (ChargerStatus())
    {
        case NO_POWER:
            return TRUE;

        default:
            return FALSE;
    }
#else
    /* Assume we can't turn off */
    return FALSE;
#endif
}

/**************************************************************************/
/*! \brief Check if a charger is connected

    \note   The value returned if the charger is disabled is indeterminate. 
            This function should only be relied upon if the charger is enabled.

    \note   If there is no charger support in the project, then this 
            function will return TRUE

    \return TRUE if the charger is in a state where we know there is power, 
            FALSE otherwise 
 */
bool appChargerIsConnected(void)
{
#ifdef INCLUDE_CHARGER
    switch (ChargerStatus())
    {
        case TRICKLE_CHARGE:
        case FAST_CHARGE:
        case DISABLED_ERROR:    /*! \todo Does this fix an edge case, otherwise delete */
        case STANDBY:
        case PRE_CHARGE:
        case HEADROOM_ERROR:
            return TRUE;

            /* Could list all the other cases here, so there is a compilation
                error if the enum is extended... but the type mixes error codes
                with actual state */
        default:
            return FALSE;
    }
#else
    return TRUE;
#endif
}



/* Only compile if CHARGER defined */
#ifdef INCLUDE_CHARGER

#define CHARGER_READ_PERIOD (D_SEC(1))
#define CHARGER_INTERNAL_TIMER  (0)

/**************************************************************************/
static  bool appChargerIsCharging(void)
{
    switch (ChargerStatus())
    {
        case TRICKLE_CHARGE:
        case FAST_CHARGE:
        case PRE_CHARGE:
            return TRUE;
        default:
            return FALSE;
    }
}

/**************************************************************************/
static void appChargerEvent(void)
{
    chargerTaskData *theCharger = appGetCharger();
    bool is_connected = appChargerIsConnected();
    bool is_charging = appChargerIsCharging();

    if (theCharger->status != ChargerStatus())
    {
        theCharger->status = ChargerStatus();
        switch (theCharger->status)
        {
            case TRICKLE_CHARGE:
                DEBUG_LOG("appChargerIsCharging, trickle charge");
                break;
            case FAST_CHARGE:
                DEBUG_LOG("appChargerIsCharging, fast charge");
                break;
            case DISABLED_ERROR:
                DEBUG_LOG("appChargerIsCharging, disabled");
                break;
            case STANDBY:
                DEBUG_LOG("appChargerIsCharging, standby");
                break;
            case PRE_CHARGE:
                DEBUG_LOG("appChargerIsCharging, pre charge");
                break;
            case NO_POWER:
                DEBUG_LOG("appChargerIsCharging, no power");
                break;
            case HEADROOM_ERROR:
                DEBUG_LOG("appChargerIsCharging, headroom error");
                break;
            default:
                DEBUG_LOGF("appChargerIsCharging, status %d", ChargerStatus());
                break;
        }
    }

    /* Check if connected status has changed */
    if (is_connected != theCharger->is_connected)
    {
        /* Check if connected */
        if (is_connected)
        {
            /* Indicate charger connection */
            appUiChargerConnected();

            /* Clear charging flag now to kick off charging indication later */
            theCharger->is_charging = FALSE;

            appTaskListMessageSendId(theCharger->client_tasks, CHARGER_MESSAGE_ATTACHED);
        }
        else
        {
            /* Indicate charger disconnection */
            appUiChargerDisconnected();
        
            /* Cancel any timers running */
            MessageCancelAll(&theCharger->task, CHARGER_INTERNAL_TIMER);

            /* Clear charging flag */
            theCharger->is_charging = FALSE;

            appTaskListMessageSendId(theCharger->client_tasks, CHARGER_MESSAGE_DETACHED);
        }

        /* Store updated connected state */
        theCharger->is_connected = is_connected;
    }
    
    /* Check if connected */
    if (is_connected)
    {
        /* Check if we have finished charging */
        if (!is_charging && theCharger->is_charging)
            appUiChargerComplete();
        else if (is_charging)
        {
            if (appBatteryGetVoltage() > appConfigBatteryVoltageOk())
                appUiChargerChargingOk();
            else
                appUiChargerChargingLow();
        }

        /* Store updated charging status */
        theCharger->is_charging = is_charging;
    }

    /* Start charger poll timer if charger is connected and still charging */
    if (is_connected && is_charging)
    {
        MessageCancelAll(&theCharger->task, CHARGER_INTERNAL_TIMER);
        MessageSendLater(&theCharger->task, CHARGER_INTERNAL_TIMER, 0, CHARGER_READ_PERIOD);
    }
}

/**************************************************************************/
static void appChargerHandleMessage(Task task, MessageId id, Message message)
{
    UNUSED(task);
    UNUSED(id);
    UNUSED(message);

    /* Check for charger events */
    appChargerEvent();
}

/* Set the configuration of the charger.
 */
static void appChargerConfigureCharger(void)
{
    ChargerConfigure(CHARGER_ENABLE, 0);

    /* Currents */
    ChargerConfigure(CHARGER_TRICKLE_CURRENT, appConfigChargerTrickleCurrent());
    ChargerConfigure(CHARGER_PRE_CURRENT, appConfigChargerPreCurrent());
    ChargerConfigure(CHARGER_FAST_CURRENT, appConfigChargerFastCurrent());
    ChargerConfigure(CHARGER_ITERM_CTRL, appConfigChargerTerminationCurrent());

    /* Voltages */
    ChargerConfigure(CHARGER_PRE_FAST_THRESHOLD, appConfigChargerPreFastThresholdVoltage());
    ChargerConfigure(CHARGER_TERM_VOLTAGE, appConfigChargerTerminationVoltage());

    /* Thresholds/timing */
    ChargerConfigure(CHARGER_STANDBY_FAST_HYSTERESIS, appConfigChargerStandbyFastVoltageHysteresis());
    ChargerConfigure(CHARGER_STATE_CHANGE_DEBOUNCE, appConfigChargerStateChangeDebounce());


    ChargerConfigure(CHARGER_ENABLE, 1);
}

#endif /* INCLUDE_CHARGER */

/*! \brief Restore the charger to the expected state.

    This function should only be called after a call to 
    \ref appChargerForceDisable(). It re-enables the charger hardware
    then checks the correct mode of operation.
*/
void appChargerRestoreState(void)
{
#ifdef INCLUDE_CHARGER
    appChargerConfigureCharger();
    appChargerEvent();
#endif
}


/*! \brief Initialise the application handling of charger

    This function should be called during application initialisation.
    If charger support is not required, the function ensures that
    the charger is disabled.
*/
#ifdef INCLUDE_CHARGER
void appChargerInit(void)
{
    chargerTaskData *theCharger = appGetCharger();

    /* Set up task handler & record current charger status */
    theCharger->task.handler = appChargerHandleMessage;
    theCharger->client_tasks = appTaskListInit();
    theCharger->is_connected = FALSE;
    theCharger->is_charging = FALSE;
    theCharger->status = ENABLE_FAIL_UNKNOWN;

    appChargerConfigureCharger();

    /* Register for charger messages */
    MessageChargerTask(&theCharger->task);

    /* Check for charger events */
    appChargerEvent();
}
#endif /* INCLUDE_CHARGER */

bool appChargerClientRegister(Task client_task)
{
#ifdef INCLUDE_CHARGER
    chargerTaskData *theCharger = appGetCharger();
    if (appTaskListAddTask(theCharger->client_tasks, client_task))
    {
        /* Send initial state if charger is attached */
        if (appChargerIsConnected())
        {
            MessageSend(client_task, CHARGER_MESSAGE_ATTACHED, NULL);
        }
        else
        {
            MessageSend(client_task, CHARGER_MESSAGE_DETACHED, NULL);
        }
        return TRUE;
    }
#else
    UNUSED(client_task);
#endif
    return FALSE;
}


void appChargerClientUnregister(Task client_task)
{
#ifdef INCLUDE_CHARGER
    chargerTaskData *theCharger = appGetCharger();
    appTaskListRemoveTask(theCharger->client_tasks, client_task);
#else
    UNUSED(client_task);
#endif
}

