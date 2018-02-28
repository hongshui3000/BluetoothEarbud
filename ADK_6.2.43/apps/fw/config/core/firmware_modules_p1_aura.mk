# Gather the full set of core modules for the P1 Crescendo firmware build.
# NOTE: do not add ad hoc modules to CORE_MODULES in this file - add them to 
# the appropriate macro in ../config/core/modules.mk instead.  This keeps all
# the names in one place and makes it much easier to see what's what.

include ../config/core/firmware_modules_p1.mk

# Append any product specific FW modules
CORE_MODULES += ${AURA_FIRMWARE_MODULES}

