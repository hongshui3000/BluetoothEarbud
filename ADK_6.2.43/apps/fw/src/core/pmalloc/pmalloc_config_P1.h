/* Copyright (c) 2016 - 2017 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Internal header file holding the fixed pool configurations
 * known for each subsystem on processor P1 builds
 * This configuration provides the *minimum* allocation, which 
 * is topped up with sane allocations for the remaining heap space
 * for each module or subsystem, feel free to edit *your* allocation
 */
#ifndef PMALLOC_CONFIG_H
#define PMALLOC_CONFIG_H

/*****************************************************************************
 * DEFAULT MEMORY POOL CONFIGURATION
 *****************************************************************************/
 #include "../../../common/interface/app/nfc/nfc_prim.h"
 
 static pmalloc_pool_config pools_reqd[] =
 { 
    {4, 60},
    {8, 35},
    {16, 45},
    {32, 95},
    {48, 64},           /* -5 for SDIO host with an Amber */
    {64, 28},           /* -2 for SDIO */
    {128, 24},          /* -2 for SDIO */
    {256, 12},
    {512, 2},
    {764, 2},           /* Needed for Broadcast Audio Receiver */
    {1648, 1},          /* Needed for Broadcast Audio Broadcaster */
 };
  
#endif /* PMALLOC_CONFIG_H */
