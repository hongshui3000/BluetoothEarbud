# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# Name of this subsystem (same as directory name)
SUBSYSTEM = piodebounce

# Source files
SRC_C = piodebounce.c

# Headers for autogeneration
ITIME_H = piodebounce_itime.h
SCHED_H = piodebounce_sched.h

# Include standard rules
include Makefile.rules
