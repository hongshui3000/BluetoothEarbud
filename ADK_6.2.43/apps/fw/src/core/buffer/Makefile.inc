# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# -*-Makefile-*-
# FILE
#   Makefile.inc  -  makefile for the Ultrawideband Link Controller

# Name of this subsystem (same as directory name)
SUBSYSTEM = buffer

# Source files
SRC_C=	buf_raw_write_map_16bit.c \
	buf_raw_write_map_16bit_be.c \
	buf_raw_write_map_16bit_save_state.c \
	buf_raw_write_only_map_16bit.c \
	buf_raw_write_only_map_16bit_be.c \
	buf_raw_write_map_8bit.c \
	buf_raw_write_map_8bit_save_state.c \
	buf_raw_write_only_map_8bit.c \
	buf_raw_write_only_map_8bit_save_state.c \
	buf_raw_write_update_restore_state.c \
	buf_raw_write_only_unmap.c \
	buf_raw_write_unmap.c \
	buf_raw_read_map_16bit.c \
	buf_raw_read_map_16bit_be.c \
	buf_raw_read_map_16bit_save_state.c \
	buf_raw_read_map_8bit.c \
	buf_raw_read_map_8bit_save_state.c \
	buf_raw_read_unmap.c \
	buf_raw_read_write_map_16bit.c \
	buf_raw_read_write_map_16bit_be.c \
	buf_raw_read_write_map_16bit_save_state.c \
	buf_raw_read_write_map_8bit.c \
	buf_raw_read_write_map_8bit_save_state.c \
	buf_raw_read_write_unmap.c \
	buf_raw_read_update_restore_state.c \
	buf_raw_update_tail_free.c \
	buffer_msg.c \
	buf_init_handle.c \
    buffer_msg_ptr_access.c
	

ifeq ("${PROCESSOR_P1}","")
SRC_C+=	buf_create_buffer.c \
	buf_destroy_buffer.c \
	buf_reinit.c \
	buf_create_clone_location.c \
	buf_create_placeholder_location.c
endif

# Inclue standard rules
include Makefile.rules
