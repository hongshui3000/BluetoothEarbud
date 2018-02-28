# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = hal

# Source files
SRC_C =  \
         \
        hal_delay_us.c \
        hal_led.c \
         \
         \
         \
         \
         \
        hal_data_conv.c \
        hal_data_conv_access.c \
        hal_data_conv_cal.c

SRC_S =

# Include standard rules
include Makefile.rules
