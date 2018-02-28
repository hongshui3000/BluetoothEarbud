# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for this module
#

# Name of this subsystem (in the format: ${SUBCOMPONENT}/%{directory_name})
SUBSYSTEM = core/trap_api

# Source files
SRC_C = trap_api_message.c \
        trap_api_bitserial.c \
        trap_api_bluestack.c \
        trap_api_capacitive_sensor.c \
        trap_api_core.c \
        trap_api_core_pio.c \
        trap_api_core_util.c \
        trap_api_extra.c \
        trap_api_stream.c \
        trap_api_operator.c \
        trap_api_test_support.c \
        trap_api_led.c \
        trap_api_message_log.c \
        trap_api_charger.c \
        trap_api_csb.c \
        trap_api_psu.c \
        trap_api_uart.c \
        trap_api_audio.c \
        trap_api_sd_mmc.c         

AUTO_C = gen/trap_api_ipc_glue.c 

SRC_S =

SCHED_H = trap_api_sched.h

LATE_MAKEFILE = Makefile_late.inc

# Include standard rules
include Makefile.rules
