/****************************************************************************
Copyright (c) 2005 - 2017 Qualcomm Technologies International, Ltd.

FILE NAME
    power_battery_target.c

DESCRIPTION
    This file contains the battery monitoring functionality specific to
    a given architecture target
*/
#include <charger.h>
#include <power.h>

charger_battery_status PowerBatteryGetStatusAtBoot(void)
{
    /* P0 Trap is unimplemented in Hydracore so return a value that will force
     * the app to re-check the battery */
    return CHARGER_BATTERY_DEAD;
}
