/*
 * THIS FILE IS AUTOGENERATED, DO NOT EDIT!
 *
 * generated by gattdbgen from gatt_running_speed_cadence_server/gatt_running_speed_cadence_server_db.dbi_
 */
#ifndef __GATT_RUNNING_SPEED_CADENCE_SERVER_DB_H
#define __GATT_RUNNING_SPEED_CADENCE_SERVER_DB_H

#include <csrtypes.h>

#define HANDLE_RUNNING_SPEED_AND_CADENCE_SERVICE (0x0001)
#define HANDLE_RUNNING_SPEED_AND_CADENCE_SERVICE_END (0xffff)
#define HANDLE_RUNNING_SPEED_AND_CADENCE_MEASUREMENT (0x0003)
#define HANDLE_RUNNING_SPEED_AND_CADENCE_CLIENT_CONFIG (0x0004)
#define HANDLE_RSC_FEATURE              (0x0006)

uint16 *GattGetDatabase(uint16 *len);
uint16 GattGetDatabaseSize(void);

#endif

/* End-of-File */