# Copyright (c) 2017 Qualcomm Technologies International, Ltd.
#   %%version
#
# List the modules to build for the aura customer (P1) library build.
#
# This should not contain any application related files such as Apps P1 tests
# or libraries that are not required by the ADK's Aura P1 library.

# Aura uses the base set
include ../config/customer/modules.mk

# Remove modules that are not required in the .a.
CUSTOMER_LIBS_FIRMWARE_MODULES =
CUSTOMER_APPS_FIRMWARE_MODULES =

# Header include for final subcomponent list (incuding any chip specific 
# modules)
INC += $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, -Icustomer/${SUBCOMPONENT})
INC += $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, -I${BUILD_DIR}/customer/${SUBCOMPONENT})
