# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# Makefile for the usb_device_class module
#


SUBSYSTEM = core/usb_device_class

SRC_C_NON_LIB = usb_device_audio.c \
    usb_device_hid.c \
	usb_device_class.c
	

# Include standard rules
include Makefile.rules
