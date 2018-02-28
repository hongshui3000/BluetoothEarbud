# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# This file contains the definitions common to all firmware builds

# Minimum targets needed to produce some firmware (fastest build)
# This is picked up in the main makefile as the 'fw' target - 'make -s -j8 fw'
FW_TARGETS = ${ELF_FILE} ${DEBUGGER_FILES} ${BUILDID_STAMP} 

# Extra taragets for the firmware that are needed for full builds
FW_EXTRA_TARGETS = ${MAP_FILE} ${LST_FILE} ${API_DOCUMENT} ${MIB_EXTRA_TARGETS} ${XUV_FILE}

ifeq ("${ENABLE_SIGNED}", "true")
FW_EXTRA_TARGETS += ${XUV_BIN_FILE} ${SIGNED_BIN_FILE} ${SIGNED_XUV_FILE}
endif

TARGETS = ${FW_TARGETS} ${FW_EXTRA_TARGETS}

# We can't put this in TARGETS as we need all targets built
# before doing this
MIB_XML_FILE=app_mib.xml
FINAL_TARGETS=${BUILD_DIR}/${MIB_XML_FILE} firmware_final_print segments

# Compiler definitions
include ../config/kcc_compiler.mk

