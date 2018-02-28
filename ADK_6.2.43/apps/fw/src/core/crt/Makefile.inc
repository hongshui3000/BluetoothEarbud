# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = crt

# Source files
SRC_C = 

SRC_S = crt0_rst_maxim.asm \
        crt0.asm

# Include standard rules
include Makefile.rules
