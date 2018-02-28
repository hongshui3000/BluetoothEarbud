# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# FILE
#   Makefile.inc  -  makefile for the debug module
#
# MODIFICATION HISTORY
#   #1   03:aug:07   ph      B-28863: Created
#
# REVISION

# Name of this subsystem (same as directory name)
SUBSYSTEM = hydra_log

# Source files
SRC_C = hydra_log_firm.c hydra_log_soft.c

LATE_MAKEFILE = Makefile_late.inc

# Include standard rules
include Makefile.rules
