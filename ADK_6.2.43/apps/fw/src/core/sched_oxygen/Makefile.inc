# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = sched_oxygen

# Source files
SRC_C = sched_oxygen.c sched_oxygen_cancel.c

SRC_S =

LATE_MAKEFILE = Makefile_late.inc

# Include standard rules
include Makefile.rules
