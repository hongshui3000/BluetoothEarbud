# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for APPCMD
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = appcmd

# Source files
SRC_C = appcmd.c
SRC_S = appcmd_call_function.asm
SCHED_H = appcmd_sched.h

# Include standard rules
include Makefile.rules

