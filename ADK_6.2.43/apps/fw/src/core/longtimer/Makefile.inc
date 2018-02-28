# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for longtimer
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = longtimer

# Source files
SRC_C = get_deci_time.c \
        get_milli_time.c \
        get_second_time.c \
        longtimer.c

# Include standard rules
include Makefile.rules
