# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# FILE
#   Makefile.inc  -  makefile for the low-level PIO abstraction
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = pio

# Source files
SRC_C=	init_pio.c \
	pio_set_directions_mask.c \
	pio_set_internal_owners_mask.c \
	pio_set_levels_mask.c \
	pio_get_levels_mask.c

# Include standard rules
include Makefile.rules
