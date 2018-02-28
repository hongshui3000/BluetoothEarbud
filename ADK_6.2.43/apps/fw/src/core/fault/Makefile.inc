# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for the fault subsystem
#
# MODIFICATION HISTORY
#	#1	15:oct:07	jb03	Ported to DASH from //depot/dot11/main/mac/fault/Makefile.inc#1
#
# REVISION

# Name of this subsystem (same as directory name)
SUBSYSTEM = fault

# Source files
SRC_C = fault.c \
        fault_appcmd.c \
        fault_comms.c \
        fault_db.c

# Headers for autogeneration
SCHED_H = fault_sched.h
ITIME_H = fault_itime.h

# Include standard rules
include Makefile.rules
