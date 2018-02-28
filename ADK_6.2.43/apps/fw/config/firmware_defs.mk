# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Gather the full set of macro definitions for the firmware build.
# NOTE: do not add ad hoc directives in this file - add them to 
# the appropriate macro in ../config/defs.mk instead.  This keeps all
# the names in one place and makes it much easier to see what's what.

# Chip Specific defines go to their own files and this file should be 
# included at the end

include ../config/customer/defs.mk

TRAPSET_DEFS = $(patsubst %,TRAPSET_%, ${TRAPSETS})

# The definitions are split into Modules and Additional.
# The definition value for Modules can be derived from the Module name.
# Some shell commands may wish to process these defines seperately to reduce
# their command line length, which is limited.
ADDITIONAL_DEFS = $(COMMON_DEFS) $(FIRMWARE_DEFS) $(BIST_DEFS)
ADDITIONAL_DEFS += $(HTOL_DEFS) ${CUSTOMER_DEFS} ${BT_DEFS} ${TRAPSET_DEFS}

DEFLIST = $(MODULE_DEFS) $(ADDITIONAL_DEFS)


KALIMBA_BUILD=1

INC += -Icore/include_fw
