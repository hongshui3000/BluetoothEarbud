# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Gather the full set of modules for the customer firmware build.
# NOTE: do not add ad hoc modules to CUSTOMER_MODULES in this file - add them
# to the appropriate macro in ../config/customer/modules.mk instead.  This
# keeps all the names in one place and makes it much easier to see what's what.

MODULE_SETS = COMMON FIRMWARE

INC += $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, -Icustomer/${SUBCOMPONENT})
INC += $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, -I${BUILD_DIR}/customer/${SUBCOMPONENT})

CUSTOMER_MODULES = $(foreach(MODULE_SET, ${MODULE_SETS}, $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, $(foreach MODULE, ${CUSTOMER_$(strip $(call uc, ${SUBCOMPONENT}))_${MODULE_SET}_MODULES}, ${MODULE})))
CUSTOMER_MODULE_PATHS = $(foreach MODULE_SET, ${MODULE_SETS}, $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, $(foreach MODULE, ${CUSTOMER_$(strip $(call uc, ${SUBCOMPONENT}))_${MODULE_SET}_MODULES}, ${SUBCOMPONENT}/${MODULE})))
