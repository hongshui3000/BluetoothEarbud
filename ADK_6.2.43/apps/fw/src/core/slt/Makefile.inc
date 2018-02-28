# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for the Symbol Lookup Table
#

# Name of this subsystem (same as directory name)
SUBSYSTEM=	slt

SRC_C=	slt_entry.c

SRC_S=	slt_header.asm

LATE_MAKEFILE = Makefile_late.inc

# Include standard rules
include Makefile.rules
