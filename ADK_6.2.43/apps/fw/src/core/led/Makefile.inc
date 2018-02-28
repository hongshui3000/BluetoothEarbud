# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# Name of this subsystem (same as directory name)
SUBSYSTEM = led

# Source files
SRC_C := 	led.c		\
		led_appcmd.c	

SCHED_H :=	led_sched.h	\

# Include standard rules
include Makefile.rules
