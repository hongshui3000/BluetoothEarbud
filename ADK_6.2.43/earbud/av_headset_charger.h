/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    Part of 6.2
\file       av_headset_charger.h
\brief      Header file for Charger monitoring
*/

#ifndef _AV_HEADSET_CHARGER_H_
#define _AV_HEADSET_CHARGER_H_

#include <charger.h>
#include "av_headset.h"

/*! \brief Messages which may be sent by the charger module. */
typedef enum 
{
    /*! Message to inform client the charger was attached */
    CHARGER_MESSAGE_ATTACHED = CHARGER_MESSAGE_BASE,
    /*! Message to inform client the charger was detached */
    CHARGER_MESSAGE_DETACHED
} chargerMessages;

/*! The charger module internal state */
typedef struct
{
    /*! Charger task */
    TaskData task;
    /*! List of client tasks */
    TaskList *client_tasks;
    /*! Set when charger is charging */
    unsigned is_charging:1;
    /*! Set when charger is connected */
    unsigned is_connected:1;
    /*! The current charger status */
    charger_status status;
} chargerTaskData;

extern void appChargerInit(void);

extern void appChargerForceDisable(void);
extern void appChargerRestoreState(void);

extern bool appChargerCanPowerOff(void);
extern bool appChargerIsConnected(void);

/*! @brief Register a client to receive status messages from the charger module.
    @param client_task The task to register.
    @return TRUE if successfully registered.
            FALSE if registration not sucessfull of if the charger support is not
            compiled into the application.
*/
extern bool appChargerClientRegister(Task client_task);

/*! @brief Unregister a client.
    @param client_task The task to unregister. */
extern void appChargerClientUnregister(Task client_task);


#endif
