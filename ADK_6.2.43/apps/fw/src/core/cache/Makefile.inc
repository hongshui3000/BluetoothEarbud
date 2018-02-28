# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (same as directory name)
SUBSYSTEM = cache

# Source files
SRC_C = cache.c

ifeq (${ENABLE_CACHE_TEST_LARGE_CODE},1)
SRC_C += \
	cache_test_1.c \
	cache_test_2.c \
	cache_test_3.c \
	cache_test_4.c \
	cache_test_5.c \
	cache_test_6.c \
	cache_test_7.c \
	cache_test_8.c
endif

SRC_S =

# Include standard rules
include Makefile.rules
