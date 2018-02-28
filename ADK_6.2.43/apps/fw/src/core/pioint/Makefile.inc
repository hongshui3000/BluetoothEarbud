# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for the PIO interrupt generation code
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = pioint

# Source files
SRC_C = pioint_configure.c pioint_init.c

# Include standard rules
include Makefile.rules
