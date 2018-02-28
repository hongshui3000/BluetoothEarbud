# Copyright (c) 2017 Qualcomm Technologies International, Ltd.
#   %%version
# Top level makefile fragment for the aura P1 firmware build (d01
# version). The build is put together with respect to the chip revision,
# modules, output type and target device (d01, full, firmware, p1).
# Note we need to include d01_defs first
# core/io/Makefile.inc relies on CHIP_REV
BUILDID_SUBSET := full
include ../config/aura_d01_defs.mk


CORE_MODULES_LIST =../config/core/firmware_modules_p1.mk
CUSTOMER_MODULES_LIST =../config/customer/firmware_modules_aura.mk

# Information for last_defines processing
COMPONENTS+=	customer

include ../config/customer/defs.mk

include ../config/firmware.mk
include ../config/release/common/aura_p1.mk
