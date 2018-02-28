# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Gather the full set of core modules for the P1 firmware build.
# NOTE: do not add ad hoc modules to CORE_MODULES in this file - add them to 
# the appropriate macro in ../config/core/modules.mk instead.  This keeps all
# the names in one place and makes it much easier to see what's what.

include ../config/core/modules_p1.mk

CORE_MODULES = ${COMMON_MODULES} ${FIRMWARE_MODULES}
