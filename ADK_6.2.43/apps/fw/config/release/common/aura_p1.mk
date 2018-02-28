# Copyright (c) 2017 Qualcomm Technologies International, Ltd.
#   %%version
# Top level makefile fragment for the aura P1 kitchen sink firmware build.
# Here we have the P1 specific defines, the default project name and the build
# directory. We also include some makefile fragments needed by all P1 builds.
BUILDID_TARGET_DEVICE := aura_p1

COMMON_DEFS += PROCESSOR_P1
PROCESSOR_P1 = true

ifeq ("${PROJECT}", "")
PROJECT = aura_app_p1
endif
BUILD_DIR = ../build/${RELEASE}

include ../config/chip_aura.mk
include ../config/core/processor_1.mk
include ../config/aura_firmware_defs.mk
# In principle P1 needn't use the same trapset definitions as P0, but that's
# the obvious thing to do for Apps development
include ../config/core/trapsets_aura.mk
include ../config/bt/trapsets.mk
include ../config/nfc/trapsets_disabled.mk

# Include all the module makefile fragments
# Note that this include must come after those that contain any definitions that 
# are used within a module Makefile.inc 
include ../config/module_list.mk

FW_TARGETS += ${LIBRARY_FILE}
