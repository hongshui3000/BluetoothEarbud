# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Definitions included in the makefile to express chip details common to all apps targets

SUBSYS_ID =4

FIRMWARE_DEFS+= SUBSYS_ID=4

# Definition of the CPU architecture
FIRMWARE_DEFS+=	__KALIMBA__ __KALIMBA32__ KAL_ARCH4


# Memory Configuration


FIRMWARE_DEFS += MMU_WINDOW_ADDR_BITS_PER_BUFFER=20
FIRMWARE_DEFS += MMU_LOG_PAGE_BYTES=7

COMMON_DEFS += CHIP_HAS_MMU_BIG_ENDIAN_MAPPING=0









