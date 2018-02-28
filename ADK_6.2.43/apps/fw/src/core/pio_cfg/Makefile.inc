# Copyright (c) 2017 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# FILE
#   Makefile.inc  -  makefile for low-level PIO configuration
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = pio_cfg

# Source files
SRC_C := pio_cfg.c \
         pio_cfg_ccp.c \
         pio_cfg_utils.c

# Include standard rules
include Makefile.rules
