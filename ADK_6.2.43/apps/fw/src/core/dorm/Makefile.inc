# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for the dorm module

# Name of this subsystem (same as directory name)
SUBSYSTEM = dorm

# Source files
SRC_C=	dorm_config.c \
	dorm_get_kip_flags.c

ifeq ("${KALIMBA_BUILD}","1")
SRC_C+=	dorm_kalimba.c
else
SRC_C+=	dorm_hydra_deep_sleep.c \
	dorm_itime.c \
	dorm_pick_clock.c \
	dorm_set_full_mmu_clock.c \
	dorm_set_reduced_mmu_clock.c \
	dorm_shallow_sleep.c \
	dorm_sleep.c \
	dorm_wake.c \
	dorm_wake_common.c \
	dorm_wake_interrupt.c \
	init_dorm.c
endif

# Include standard rules
include Makefile.rules
