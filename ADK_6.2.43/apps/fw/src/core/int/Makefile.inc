# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = int

# Source files
SRC_C = init_int.c \
        configure_interrupt.c

# Software interrupt sources
SRC_C += configure_sw_interrupt.c \
         configure_sw_interrupt_raw.c \
         generate_sw_interrupt.c \
         swint_demux.c \
         unconfigure_sw_interrupt.c

SRC_S = interrupt.asm

# Include standard rules
include Makefile.rules

