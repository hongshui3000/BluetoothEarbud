# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# Makefile for the trap_version module
#

SUBSYSTEM = trap_version

TRAP_VERSION_C_FILE = trap_version_supported.c

AUTO_C = ${TRAP_VERSION_C_FILE}
API_TRAP_SUPPORTED_C = ${BUILD_DIR}/core/trap_version/${TRAP_VERSION_C_FILE}

SLT_ENTRY_H = trap_version_slt_entry.h

LATE_MAKEFILE = Makefile_late.inc

# Include standard rules
include Makefile.rules
