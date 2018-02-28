/****************************************************************************
Copyright (c) 2015 Qualcomm Technologies International, Ltd.
Part of 6.2

FILE NAME
    upgrade_partitions_config.c

DESCRIPTION

    Implementation of partition management, config dependant parts
    of for the upgrade_partitions.c module for CONFIG_HYDRACORE.


IMPLEMENTATION NOTES

See the the upgrade_partitions.c module.

*/
    
#include <ps.h>
#include <print.h>
#include <partition.h>
#include <sink.h>
#include <stream.h>
#include <imageupgrade.h>

#include "upgrade_private.h"
#include "upgrade_ctx.h"
#include "upgrade_partitions.h"

/****************************************************************************
NAME
    UpgradePartitionsEraseAllManaged  -  Erase partitions

DESCRIPTION
    For the CONFIG_HYDRACORE version this just calls ImageUpgradeErase
*/
UpgradePartitionsState UpgradePartitionsEraseAllManaged(void)
{
    PRINT(("ImageUpgradeErase()\n"));
    ImageUpgradeErase();
    return UPGRADE_PARTITIONS_ERASED;
}
