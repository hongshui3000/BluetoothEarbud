# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# List the aura specific modules to build for the customer component of an
# Aura P1 build.

# Aura uses the base set
include ../config/customer/modules.mk

# Header include for final subcomponent list (incuding any chip specific 
# modules)
INC += $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, -Icustomer/${SUBCOMPONENT})
INC += $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, -I${BUILD_DIR}/customer/${SUBCOMPONENT})

