# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for the panic subsystem
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = panic

# Source files
SRC_C=	panic.c \
	panic_on_assert.c \
	panic_comms.c

ifneq ($(findstring -DXAP,${MAKEDEFINES}),)
SRC_S=	panic_on_assert_very_brief.s
endif

# Include standard rules
include Makefile.rules
