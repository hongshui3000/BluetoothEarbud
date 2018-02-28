# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Definitions included in the makefile to express details of the chip

# Definitions used by the Makefile
CHIP_NAME = aura
SUBSYS_ID =4
FIRMWARE_DEFS+= SUBSYS_ID=4

KCC_CHIP_NAME = QCC3400_app

# Definition of the chip and revision
COMMON_DEFS+=	CHIP_AURA

#USB host HW is not present on Aura
COMMON_DEFS += CHIP_HAS_USB_HOST_HARDWARE=0

# Number of PIOs
COMMON_DEFS += NUMBER_OF_PIOS=72

# PIOs per bank
COMMON_DEFS += PIOS_PER_BANK=32

# Nr of LED controller instances
COMMON_DEFS += NUM_OF_LEDS=6
COMMON_DEFS += DEFAULT_LED0_PIO=66



# SYS_CTRL pad index CS-331417-DD - PIO 0 for all variants 
COMMON_DEFS += SYS_CTRL_IN_PAD_IDX=0



include ../config/chip_apps.mk
