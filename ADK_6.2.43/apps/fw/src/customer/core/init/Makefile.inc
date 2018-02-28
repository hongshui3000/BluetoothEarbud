# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = core/init

# Source files
SRC_C = init.c

LIB_NAME = init_usb_early_init.a
LIB_C = init_usb_early_init.c

# Include standard rules
include Makefile.rules

