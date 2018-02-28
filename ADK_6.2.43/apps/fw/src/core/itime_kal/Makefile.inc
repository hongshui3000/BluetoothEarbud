# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = itime_kal

# Source files
SRC_C = itime_kal.c

SRC_S =

LATE_MAKEFILE = Makefile_late.inc

# Include standard rules
include Makefile.rules
