# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = ipc

# Source files
SRC_C = ipc_init.c \
        ipc_send.c \
        ipc_recv.c \
        ipc_mmu.c \
        ipc_malloc.c \
        ipc_pio.c \
        ipc_fault_panic.c \
        ipc_sched.c \
        ipc_test_tunnel.c \
        ipc_bluestack.c \
        ipc_trap_api.c \
        ipc_test.c \
        ipc_deep_sleep.c \
        ipc_stream.c \
        ipc_uart.c \
        ipc_test_traps.c \
        ipc_sd_mmc.c

AUTO_H = gen/ipc_trap_api_recv.c

SCHED_H = ipc_sched.h

LATE_MAKEFILE = Makefile_late.inc

# Include standard rules
include Makefile.rules

