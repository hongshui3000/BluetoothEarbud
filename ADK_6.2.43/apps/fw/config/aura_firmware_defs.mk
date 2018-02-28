# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Gather the full set of macro definitions for the Aura firmware build.
# NOTE: do not add ad hoc directives in this file - add them to 
# the appropriate macro in ../config/core/defs.mk instead.  This keeps all
# the names in one place and makes it much easier to see what's what.

# Master Definition List
include ../config/defs.mk

# Append Aura Specific Defines
COMMON_DEFS += ${AURA_COMMON_DEFS}
FIRMWARE_DEFS += ${AURA_FIRMWARE_DEFS}

# finally include firmware defs
include ../config/firmware_defs.mk
include ../config/bt/dle_defs.mk
