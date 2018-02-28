/*****************************************************************************

            Copyright (c) 2018 Qualcomm Technologies International, Ltd.

            WARNING: This is an auto-generated file!
                     DO NOT EDIT!

*****************************************************************************/
#ifndef SYSTEM_PRIM_H__
#define SYSTEM_PRIM_H__

#include "hydra/hydra_types.h"


/*******************************************************************************

  NAME
    SYSTEM_SUBSYSTEM_TYPE

  DESCRIPTION
    Hardware independent subsystem type

 VALUES
    CURATOR - Curator
    BT      - Bluetooth
    WLAN    - Wireless LAN
    AUDIO   - FM/Audio
    GNSS    - GNSS
    NFC     - NFC
    JANITOR - Janitor
    APP     - Application

*******************************************************************************/
typedef enum
{
    SYSTEM_SUBSYSTEM_TYPE_CURATOR = 0x0000,
    SYSTEM_SUBSYSTEM_TYPE_BT = 0x0001,
    SYSTEM_SUBSYSTEM_TYPE_WLAN = 0x0002,
    SYSTEM_SUBSYSTEM_TYPE_AUDIO = 0x0003,
    SYSTEM_SUBSYSTEM_TYPE_GNSS = 0x0004,
    SYSTEM_SUBSYSTEM_TYPE_NFC = 0x0005,
    SYSTEM_SUBSYSTEM_TYPE_JANITOR = 0x0006,
    SYSTEM_SUBSYSTEM_TYPE_APP = 0x0007
} SYSTEM_SUBSYSTEM_TYPE;


#define SYSTEM_PRIM_ANY_SIZE 1


#endif /* SYSTEM_PRIM_H__ */

