# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = io

# Source files
SRC_C = 

SRC_S = $(CHIP_NAME)/$(CHIP_REV)/io/io_defs.asm \
        $(CHIP_NAME)/$(CHIP_REV)/io/io_map.asm

SLT_ENTRY_H=	io_slt_entry.h

# Include standard rules
include Makefile.rules
