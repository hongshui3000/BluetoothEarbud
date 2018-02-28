# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for the private memory allocator
#
# MODIFICATION HISTORY
#   1.1     30:apr:03   at      Created.
#   1.2     01:may:03   at      Added files for debugging support.
#   1.3     02:may:03   at      Added files for tracing ownership.
#
# REVISION

# Name of this subsystem (same as directory name)
SUBSYSTEM = pmalloc

# Source files
SRC_C = init_pmalloc.c pcopy.c pfree.c  pmalloc.c pmalloc_available.c \
        pmalloc_set_monitor_limits.c prightsize.c psizeof.c \
        xpcopy.c xpmalloc.c xprealloc.c xzpmalloc.c zpmalloc.c \
        pfree_set_free_list_ptr.c \
        pmalloc_debug_check_block.c \
        pmalloc_debug_validate_free_list.c \
        pmalloc_config.c \
        pmalloc_debug_validate_pool_control.c xpmalloc_buffer.c

SRC_S=	pmalloc_trace_pc.asm

SRC_C+=	prealloc.c

# Include standard rules
include Makefile.rules
