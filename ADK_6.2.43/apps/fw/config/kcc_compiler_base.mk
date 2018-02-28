# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version

# The location of a devkit may be supplied as DEVKIT or DEVKIT_ROOT, depending
# on context.  They both mean the same thing.
ifneq ($(DEVKIT_ROOT),)
DEVKIT=$(DEVKIT_ROOT)
endif

ifeq ($(DEVKIT),)
# To compile using internal KCC releases the KCC_RELEASE_DIR should be
# set to an empty value (KCC_RELEASE_DIR = ).
# For external releases it should contain the release path including the
# date, this must match the KCC_VER. e.g. for KCC 47 external:
# KCC_VER = 47
# KCC_RELEASE_DIR = releases/external/2016.Q2.1/
KCC_VER = 57
KCC_RELEASE_DIR = 

KCC_LOCAL_DIR = $(LOCAL_KCC_PATH)/$(KCC_RELEASE_DIR)$(KCC_VER_PATH)
KCC_NETWORK_DIR = $(NETWORK_KCC_PATH)/$(KCC_RELEASE_DIR)$(KCC_VER_PATH)
ifneq ($(wildcard $(KCC_LOCAL_DIR)/bin/*.*),)
    KCC_DIR = $(KCC_LOCAL_DIR)
else
    KCC_DIR = $(KCC_NETWORK_DIR)
endif
else
KCC_DIR = $(DEVKIT)/tools/kcc
endif
$(info "Using $(KCC_DIR)")

# Note: KCC versions prior to 47 used crescendo_app as the chip name
KCC_CHIP_NAME = csra68100_app
CHIP_NAME_OPT= -k${KCC_CHIP_NAME}
COMMON_CFLAGS = ${CHIP_NAME_OPT} ${INC} -I. ${DEFS} 

