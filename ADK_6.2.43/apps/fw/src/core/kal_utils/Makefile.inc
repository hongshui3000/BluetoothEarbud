# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = kal_utils

# Source files
SRC_C = kal_utils.c

SRC_S = kal_utils_asm.asm

# Include standard rules
include Makefile.rules
