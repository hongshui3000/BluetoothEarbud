# Copyright (c) 2017 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# FILE
#   Makefile.inc  -  makefile for the led configuration module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = led_cfg

# Source files
SRC_C = led_cfg.c \
        led_cfg_utils.c

# Include standard rules
include Makefile.rules
