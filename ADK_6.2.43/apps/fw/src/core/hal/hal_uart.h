/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * UART hardware abstraction layer header file.
 * This file contains the interface abstraction to the UART hardware. 
 * \internal The UART 
 * hardware operation is described in <a href="http://cognidox.europe.root.pri/vdocs/CS-203982-DD-2.pdf">CS-203982-DD</a> 
 * \endinternal
 * The lower hardware interface documentation can be found in 
 * /app_ss/main/fw/src/core/io/crescendo/dev/doc/main.html.
 * This is designed to be included by the \c uart_raw module and not used 
 * directly by higher layer code.
 */

#ifndef HAL_UART_H
#define HAL_UART_H

#include "hydra/hydra_types.h"
#define IO_DEFS_MODULE_UART
#define IO_DEFS_MODULE_UART_PER_SS
#define IO_DEFS_MODULE_HOST_SYS_CONFIG
#define IO_DEFS_MODULE_BUS_INTERRUPT
#include "io/io_defs.h"
#include "hal/halmgt.h"

/**
 * Constant for baud rate adjustment. This is used when running on an emulator.
 * On the chip there is no adjustment since the baud rate parameters should be
 * properly calculated and the HIF clock should be fixed. The emulator is 
 * slower than the actual chip and the clock can be changed in any build. The
 * HIF clock is currently twice as slow and used to be four times slower than
 * the target chip clock. Baud rate parameters are calculated assuming the 
 * final chip HIF clock.
 */
#define BAUD_ADJ 2

/**
 * Abstractions for setting hardware registers.
 */
/* This is a workaround for HOST_SYS_CONFIG_ENABLES being defined as an enum 
in the hal layer. this should be removed when bug is fixed. */
#define hal_set_reg_host_sys_config_enables_wa(x) \
           hal_set_register(*(volatile uint32 *)&HOST_SYS_CONFIG_ENABLES, x, 1)

#define hal_uart_enable(enable) hal_set_reg_host_sys_config_enables_wa( \
                           (enable)? (hal_get_reg_host_sys_config_enables() | \
                                      (1 << HOST_SYS_UART_BLOCK_ID)) \
                                   : (hal_get_reg_host_sys_config_enables() & \
                                      ~(1 << HOST_SYS_UART_BLOCK_ID)))

#define hal_uart_set_parity_bit(enable) \
                               hal_set_uart_config_one_parity_bit((enable)?1:0)
#define hal_uart_set_even_parity(enable) \
                                  hal_set_uart_config_even_parity((enable)?1:0)
#define hal_uart_set_two_stop_bits(enable) \
                                hal_set_uart_config_two_stop_bits((enable)?1:0)
#define hal_uart_set_flow_ctrl(enable) \
                                 hal_set_uart_config_flow_ctrl_en((enable)?1:0)
#define hal_uart_set_rts_auto(enable) \
                                  hal_set_uart_config_rts_auto_en((enable)?1:0)
#define hal_uart_set_rts(val) hal_set_uart_config_rts((val)?1:0)
#define hal_uart_set_rate(rate) hal_set_reg_uart_rate( \
       (hal_running_on_emulator())?(uart_raw_rate)((rate) * (BAUD_ADJ)):(rate))
#define hal_uart_set_tx_enable(enable) hal_set_reg_uart_tx_enable((enable)?1:0)
#define hal_uart_set_rx_enable(enable) hal_set_reg_uart_rx_enable((enable)?1:0)
#define hal_uart_get_status() hal_get_reg_uart_status()
#define hal_uart_set_rx_default_buffer(handle) \
                                     hal_set_reg_uart_rx_default_buffer(handle)
#define hal_uart_set_rx_status_buffer(handle) \
                                      hal_set_reg_uart_rx_status_buffer(handle)
#define hal_uart_set_tx_status_buffer(handle) \
                                      hal_set_reg_uart_tx_status_buffer(handle)
#define hal_uart_set_tx_td_buffer0(handle) \
                                         hal_set_reg_uart_tx_td_buffer0(handle)
#define hal_uart_set_rx_default_ssid(ssid) \
                                         hal_set_reg_uart_rx_default_ssid(ssid)
#define hal_uart_set_rx_protocol(protocol) \
                                         hal_set_reg_uart_rx_protocol(protocol)
#define hal_uart_set_protocol_none_transfer_length(length) \
                      hal_set_reg_uart_rx_protocol_none_transfer_length(length)
#define hal_uart_set_td_ctrl_add_td(val) hal_set_td_ctrl_add_td(val)
#define hal_uart_assign_sru(ssid, sruid) \
                              hal_set_reg_uart_sru_control((ssid << 4) | sruid)
#define hal_uart_set_curator_ssid(ssid) hal_set_reg_uart_curator_ssid(ssid)
#define hal_uart_set_uart_interrupt_enable(mask) \
                                        hal_set_reg_uart_interrupt_enable(mask)
#define hal_uart_set_uart_event_desc_enable(mask) \
                                       hal_set_reg_uart_event_desc_enable(mask)
#define hal_uart_set_bus_int_select(id) hal_set_reg_bus_int_select(id)
#define hal_uart_get_bus_int_status() hal_get_reg_bus_int_status()
#define hal_uart_set_bus_int_clear(mask) hal_set_reg_bus_int_clear(mask)
#define hal_uart_set_rx_idle_timeout(value) \
                                        hal_set_reg_uart_rx_idle_timeout(value)
#define hal_uart_set_rx_force_end(value) \
                                 hal_set_reg_uart_rx_force_end_of_packet(value)
#define hal_uart_set_no_activity_detector(time) \
                                    hal_set_reg_uart_no_activity_detector(time)
#define hal_uart_set_data_in_pio(pio) \
                             hal_set_reg_host_sys_uart_data_input_pio_ctrl(pio)
#define hal_uart_set_data_out_pio(pio) \
                            hal_set_reg_host_sys_uart_data_output_pio_ctrl(pio)
#define hal_uart_set_ctsb_in_pio(pio) \
                             hal_set_reg_host_sys_uart_ctsb_input_pio_ctrl(pio)
#define hal_uart_set_rtsb_out_pio(pio) \
                            hal_set_reg_host_sys_uart_rtsb_output_pio_ctrl(pio)

/**
 * Helper macros for accessing bit fields in buffers.
 */
#define hal_uart_write_field(reg, x, mask, shift, type) \
                 (reg = (type)(  (reg & (~mask))  |  ((x << shift) & mask)  ))
#define hal_uart_write_td_field(reg, x, mask, shift) \
                             hal_uart_write_field(reg, x, mask, shift, uint8) 
#define hal_uart_write_ed_field(reg, x, mask, shift) \
                            hal_uart_write_field(reg, x, mask, shift, uint16) 

#define hal_uart_read_field(reg, mask, shift, type) \
                                                  (type)((reg & mask) >> shift)
#define hal_uart_read_td_field(reg, mask, shift) \
                                   hal_uart_read_field(reg, mask, shift, uint8)
#define hal_uart_read_ed_field(reg, mask, shift) \
                                  hal_uart_read_field(reg, mask, shift, uint16)

/**
 * Macros for handling transfer descriptors.
 */
#define UART_RAW_TD_TX_PROTOCOL_MASK 0x0f
#define UART_RAW_TD_TX_PROTOCOL_LSB_POSN 0x00
#define UART_RAW_TD_TX_PROTOCOL_BYTE_POSN 0x00
#define UART_RAW_TD_SOURCE_SSID_MASK 0xf0
#define UART_RAW_TD_SOURCE_SSID_LSB_POSN 0x04
#define UART_RAW_TD_SOURCE_SSID_BYTE_POSN 0x00
#define UART_RAW_TD_BUFFER_MASK 0xff
#define UART_RAW_TD_BUFFER_LSB_POSN 0x00
#define UART_RAW_TD_BUFFER_BYTE_POSN 0x01
#define UART_RAW_TD_PAYLOAD_LENGTH_LOW_MASK 0xff
#define UART_RAW_TD_PAYLOAD_LENGTH_LOW_LSB_POSN 0x00
#define UART_RAW_TD_PAYLOAD_LENGTH_LOW_BYTE_POSN 0x02
#define UART_RAW_TD_PAYLOAD_LENGTH_HIGH_MASK 0xff
#define UART_RAW_TD_PAYLOAD_LENGTH_HIGH_LSB_POSN 0x00
#define UART_RAW_TD_PAYLOAD_LENGTH_HIGH_BYTE_POSN 0x03
#define UART_RAW_TD_TAG_MASK 0x70
#define UART_RAW_TD_TAG_LSB_POSN 0x04
#define UART_RAW_TD_TAG_BYTE_POSN 0x05
#define UART_RAW_TD_EVT_EN_MASK 0x80
#define UART_RAW_TD_EVT_EN_LSB_POSN 0x07
#define UART_RAW_TD_EVT_EN_BYTE_POSN 0x05

#define hal_uart_set_td_tx_protocol(td_ptr, x) \
    hal_uart_write_td_field(((uint8 *)td_ptr) \
                                          [UART_RAW_TD_TX_PROTOCOL_BYTE_POSN],\
                            x, \
                            UART_RAW_TD_TX_PROTOCOL_MASK, \
                            UART_RAW_TD_TX_PROTOCOL_LSB_POSN)
#define hal_uart_get_td_tx_protocol(td_ptr) \
    hal_uart_read_td_field(((const uint8 *)td_ptr) \
                                         [UART_RAW_TD_TX_PROTOCOL_BYTE_POSN], \
                           UART_RAW_TD_TX_PROTOCOL_MASK, \
                           UART_RAW_TD_TX_PROTOCOL_LSB_POSN)
#define hal_uart_set_td_source_ssid(td_ptr, x) \
    hal_uart_write_td_field(((uint8 *)td_ptr) \
                                          [UART_RAW_TD_SOURCE_SSID_BYTE_POSN],\
                            x, \
                            UART_RAW_TD_SOURCE_SSID_MASK, \
                            UART_RAW_TD_SOURCE_SSID_LSB_POSN)
#define hal_uart_get_td_source_ssid(td_ptr) \
    hal_uart_read_td_field(((const uint8 *)td_ptr) \
                                         [UART_RAW_TD_SOURCE_SSID_BYTE_POSN], \
                           UART_RAW_TD_SOURCE_SSID_MASK, \
                           UART_RAW_TD_SOURCE_SSID_LSB_POSN)
#define hal_uart_set_td_buffer(td_ptr, x) \
    hal_uart_write_td_field(((uint8 *)td_ptr) \
                                              [UART_RAW_TD_BUFFER_BYTE_POSN], \
                            x, \
                            UART_RAW_TD_BUFFER_MASK, \
                            UART_RAW_TD_BUFFER_LSB_POSN)
#define hal_uart_get_td_buffer(td_ptr) \
    hal_uart_read_td_field(((const uint8 *)td_ptr) \
                                              [UART_RAW_TD_BUFFER_BYTE_POSN], \
                           UART_RAW_TD_BUFFER_MASK, \
                           UART_RAW_TD_BUFFER_LSB_POSN)
#define hal_uart_set_td_payload_length(td_ptr, x) \
    hal_uart_write_td_field(((uint8 *)td_ptr) \
                                  [UART_RAW_TD_PAYLOAD_LENGTH_LOW_BYTE_POSN], \
                            (x & 0xff), \
                            UART_RAW_TD_PAYLOAD_LENGTH_LOW_MASK, \
                            UART_RAW_TD_PAYLOAD_LENGTH_LOW_LSB_POSN); \
    hal_uart_write_td_field(((uint8 *)td_ptr) \
                                 [UART_RAW_TD_PAYLOAD_LENGTH_HIGH_BYTE_POSN], \
                            (x >> 8), \
                            UART_RAW_TD_PAYLOAD_LENGTH_HIGH_MASK, \
                            UART_RAW_TD_PAYLOAD_LENGTH_HIGH_LSB_POSN)
#define hal_uart_get_td_payload_length(td_ptr) \
    (hal_uart_read_td_field(((const uint8 *)td_ptr) \
                                  [UART_RAW_TD_PAYLOAD_LENGTH_LOW_BYTE_POSN], \
                            UART_RAW_TD_PAYLOAD_LENGTH_LOW_MASK, \
                            UART_RAW_TD_PAYLOAD_LENGTH_LOW_LSB_POSN) + \
    (hal_uart_read_td_field(((const uint8 *)td_ptr) \
                                 [UART_RAW_TD_PAYLOAD_LENGTH_HIGH_BYTE_POSN], \
                            UART_RAW_TD_PAYLOAD_LENGTH_HIGH_MASK, \
                            UART_RAW_TD_PAYLOAD_LENGTH_HIGH_LSB_POSN) << \
     8))
#define hal_uart_set_td_tag(td_ptr, x) \
    hal_uart_write_td_field(((uint8 *)td_ptr)[UART_RAW_TD_TAG_BYTE_POSN], \
                            x, \
                            UART_RAW_TD_TAG_MASK, \
                            UART_RAW_TD_TAG_LSB_POSN)
#define hal_uart_get_td_tag(td_ptr) \
    hal_uart_read_td_field(((const uint8 *)td_ptr)[UART_RAW_TD_TAG_BYTE_POSN],\
                           UART_RAW_TD_TAG_MASK, \
                           UART_RAW_TD_TAG_LSB_POSN)
#define hal_uart_set_td_evt_en(td_ptr, x) \
    hal_uart_write_td_field(((uint8 *)td_ptr)[UART_RAW_TD_EVT_EN_BYTE_POSN], \
                            x, \
                            UART_RAW_TD_EVT_EN_MASK, \
                            UART_RAW_TD_EVT_EN_LSB_POSN)
#define hal_uart_get_td_evt_en(td_ptr) \
    hal_uart_read_td_field(((const uint8 *)td_ptr) \
                                              [UART_RAW_TD_EVT_EN_BYTE_POSN], \
                           UART_RAW_TD_EVT_EN_MASK, \
                           UART_RAW_TD_EVT_EN_LSB_POSN)
#define hal_uart_get_sizeof_td() 8

/**
 * Macros for handling all event descriptors.
 */

#define UART_RAW_ED_EVT_DESC_LENGTH_MASK 0x000f
#define UART_RAW_ED_EVT_DESC_LENGTH_LSB_POSN 0x00
#define UART_RAW_ED_EVT_DESC_LENGTH_WORD_POSN 0x00
#define UART_RAW_ED_EVENT_INDEX_MASK 0x00f0
#define UART_RAW_ED_EVENT_INDEX_LSB_POSN 0x04
#define UART_RAW_ED_EVENT_INDEX_WORD_POSN 0x00
#define UART_RAW_ED_EVENT_GROUP_MASK 0xe000
#define UART_RAW_ED_EVENT_GROUP_LSB_POSN 0x0d
#define UART_RAW_ED_EVENT_GROUP_WORD_POSN 0x00
#define UART_RAW_ED_TX_ERROR_CODE_MASK 0x00f0
#define UART_RAW_ED_TX_ERROR_CODE_LSB_POSN 0x04
#define UART_RAW_ED_TX_ERROR_CODE_WORD_POSN 0x01
#define UART_RAW_ED_RX_ERROR_CODE_MASK 0x00f0
#define UART_RAW_ED_RX_ERROR_CODE_LSB_POSN 0x04
#define UART_RAW_ED_RX_ERROR_CODE_WORD_POSN 0x01
#define UART_RAW_ED_RX_PAYLOAD_LENGTH_MASK 0xffff
#define UART_RAW_ED_RX_PAYLOAD_LENGTH_LSB_POSN 0x00
#define UART_RAW_ED_RX_PAYLOAD_LENGTH_WORD_POSN 0x02

#define hal_uart_get_ed_evt_desc_length(ed_ptr) \
    hal_uart_read_ed_field(((const uint16 *)ed_ptr) \
                                     [UART_RAW_ED_EVT_DESC_LENGTH_WORD_POSN], \
                           UART_RAW_ED_EVT_DESC_LENGTH_MASK, \
                           UART_RAW_ED_EVT_DESC_LENGTH_LSB_POSN)
#define hal_uart_get_ed_event_index(ed_ptr) \
    hal_uart_read_ed_field(((const uint16 *)ed_ptr) \
                                  [UART_RAW_ED_EVENT_INDEX_WORD_POSN], \
                           UART_RAW_ED_EVENT_INDEX_MASK, \
                           UART_RAW_ED_EVENT_INDEX_LSB_POSN)
#define hal_uart_get_ed_event_group(ed_ptr) \
    hal_uart_read_ed_field(((const uint16 *)ed_ptr) \
                                         [UART_RAW_ED_EVENT_GROUP_WORD_POSN], \
                           UART_RAW_ED_EVENT_GROUP_MASK, \
                           UART_RAW_ED_EVENT_GROUP_LSB_POSN)
#define hal_uart_get_ed_tx_error_code(ed_ptr) \
    hal_uart_read_ed_field(((const uint16 *)ed_ptr) \
                                       [UART_RAW_ED_TX_ERROR_CODE_WORD_POSN], \
                           UART_RAW_ED_TX_ERROR_CODE_MASK, \
                           UART_RAW_ED_TX_ERROR_CODE_LSB_POSN)
#define hal_uart_get_ed_rx_error_code(ed_ptr) \
    hal_uart_read_ed_field(((const uint16 *)ed_ptr) \
                                       [UART_RAW_ED_RX_ERROR_CODE_WORD_POSN], \
                           UART_RAW_ED_RX_ERROR_CODE_MASK, \
                           UART_RAW_ED_RX_ERROR_CODE_LSB_POSN)
#define hal_uart_get_ed_rx_payload_length(ed_ptr) \
    hal_uart_read_ed_field(((const uint16 *)ed_ptr) \
                                   [UART_RAW_ED_RX_PAYLOAD_LENGTH_WORD_POSN], \
                           UART_RAW_ED_RX_PAYLOAD_LENGTH_MASK, \
                           UART_RAW_ED_RX_PAYLOAD_LENGTH_LSB_POSN)

/**
 * Defines bit masks for all UART event groups.
 */
typedef enum {
    UART_EVENT_GROUP_RX_SUCCESS_MASK     = 1<<UART_EVENT_GROUP_RX_SUCCESS,
    UART_EVENT_GROUP_TX_SUCCESS_MASK     = 1<<UART_EVENT_GROUP_TX_SUCCESS,
    UART_EVENT_GROUP_RX_ERROR_MASK       = 1<<UART_EVENT_GROUP_RX_ERROR,
    UART_EVENT_GROUP_TX_ERROR_MASK       = 1<<UART_EVENT_GROUP_TX_ERROR,
    UART_EVENT_GROUP_NO_ACTIVITY_MASK    = 1<<UART_EVENT_GROUP_NO_ACTIVITY,
    UART_EVENT_GROUP_RX_IDLE_MASK        = 1<<UART_EVENT_GROUP_RX_IDLE,
    UART_EVENT_GROUP_SEQUENCE_FOUND_MASK = 1<<UART_EVENT_GROUP_SEQUENCE_FOUND,
    UART_EVENT_GROUP_PACKET_TIMEOUT_MASK = 1<<UART_EVENT_GROUP_PACKET_TIMEOUT,
    UART_EVENT_GROUP_ED_WRITE_ERROR_MASK = 1<<UART_EVENT_GROUP_ED_WRITE_ERROR
}hal_uart_event_groups;

/**
 * Defines the data type for the baud rate.
 */
typedef uart_rate uart_raw_rate;

#endif /* HAL_UART_H */
