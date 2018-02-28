/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * UART raw public header file.
 * This file contains the public interface definitions for the uart_raw module.
 */
/**
 * \defgroup uart_raw uart_raw
 * \ingroup core
 * \section uart_raw_introduction Introduction
 * The uart_raw provides a command, event and bulk data interface to the UART 
 * peripheral for the raw operation mode. The UART hardware operation is 
 * described in <a href="http://cognidox.europe.root.pri/vdocs/CS-203982-DD-2.pdf">CS-203982-DD</a>.
 * The hardware interface used is the one defined by \c hal_uart.h. 
 * 
 * \section uart_raw_init Initialisation
 * The module is started by a call to \c uart_raw_init() function by the module
 * that uses it. The init function allocates the necessary buffers, sets up the 
 * UART hardware and configures bus interrupts. It is not mandatory but 
 * recommended to register a callback function using 
 * \c uart_raw_register_callback() to get a notification when specific events 
 * happen at the driver level. Further settings are required to be made: 
 * parity, stop bits, flow control, baud rate, tx enable, rx enable, *
 * transfer length, rx idle, no activity detector. After these have been set 
 * the UART can immediately receive data.
 * 
 * \section uart_raw_settings Settings
 * - \c uart_raw_set_parity() - enable/disable parity bit, odd/even parity
 * - \c uart_raw_set_stop_bits() - one/two stop bits
 * - \c uart_raw_flow_ctrl_enable() - enable/disable RTS-CTS flow control
 * - \c uart_raw_set_rate() - baud rate
 * - \c uart_raw_tx_enable() - enable/disable TX
 * - \c uart_raw_rx_enable() - enable/disable RX
 * - \c uart_raw_set_transfer_length() - transfer length (bytes)
 * - \c uart_raw_set_rx_idle_timeout() - rx idle timeout (bits)
 * - \c uart_raw_set_no_activity_detector() - no activity detector 
 * (CLK_SLOW periods)
 * 
 * \section uart_raw_events Events
 * UART raw implements the following event IDs in \c uart_raw_events_t:
 * - RX_SUCCESS - data has been received
 * - TX_SUCCESS - data transmission succeeded
 * - RX_IDLE - no activity on RX line after given number of bit periods, 
 * following previous activity
 * - NO_ACTIVITY - no activity on TX or RX lines for given number of 
 * CLK_SLOW periods
 * 
 * \section uart_raw_transfer_length Transfer Length
 * The transfer length is used when receiving data and it controls how often
 * internal interrupts trigger and implicitly how often the callback is called.
 * This sets up the hardware to trigger an RX_SUCCESS event after the given 
 * number of bytes have been received. For example, a transfer length of 3 
 * would time events as follows:
 * \verbatim
       _____ _____ _____ _____ _____ _____ _____
bytes |__1__|__2__|__3__|__4__|__5__|__6__|__7__|
                        |                 |
                        |                 |
                   RX_SUCCESS        RX_SUCCESS
                        |                 |
                    callback          callback
   \endverbatim
 * Please note that the last byte received does not produce an event and could 
 * be potentially lost. 
 * A too low transfer length may cause buffers to overrun. When setting the 
 * transfer length it's good to keep in mind that every RX_SUCCESS event uses 
 * 8 bytes and every RX_IDLE uses 4 bytes from the status buffer. A transfer 
 * length of 1 can easily overrun the status buffer.
 * 
 * \section uart_raw_rx_idle RX Idle
 * RX idle is a mechanism in the UART hardware that detects if no data has 
 * been received after a programmable number of bit periods. If no activity has
 * been seen on the RX line for the given number of bit periods, an RX_IDLE 
 * event is generated. After an RX_SUCCESS event has been generated the 
 * detector will not start looking for an idle period until another byte has 
 * been successfully received. If RX idle timeout is 0, this mechanism is 
 * disabled. For example, a transfer length of 3 and a non zero RX idle would 
 * time events as follows:
 * \verbatim
       _____ _____ _____ _____ _____ _____ _____
bytes |__1__|__2__|__3__|__4__|__5__|__6__|__7__|.........
                        |                 |               |
                        |                 |               |
                   RX_SUCCESS        RX_SUCCESS        RX_IDLE
                        |                 |               |
                    callback          callback        callback
   \endverbatim
 * In the following example, in the same conditions as above, no RX_IDLE is 
 * generated:
  * \verbatim
       _____ _____ _____ _____ _____ _____ 
bytes |__1__|__2__|__3__|__4__|__5__|__6__|
                        |                 |
                        |                 |
                   RX_SUCCESS        RX_SUCCESS
                        |                 |
                    callback          callback
   \endverbatim
 * Please note that the actual time period is dependant on the baud rate used.
 * When an RX_IDLE event is received the upper module should call 
 * \c uart_raw_rx_force_end() to get the remainder of the data.
 * 
 * \section uart_raw_rx_force_end RX Force End
 * This mechanism is designed to flush all UART hardware buffers and put the 
 * data in the RX buffer. When the operation is completed an RX_SUCCESS is 
 * generated regardless if new data is put in the RX buffer or not. This is 
 * the recommended function to call by the upper module when an RX_IDLE event is
 * received. After this, the upper module should wait for the extra RX_SUCCESS 
 * event to guarantee it gets all the received data.
 * 
 * \section uart_raw_no_activity_detector No Activity Detector
 * This feature allows the firmware to detect when there has been no activity 
 * on the UART for a programmable length of time. This allows the firmware to 
 * consider going into sleep states. Activity is defined as any transition on 
 * UART_DATA_IN or when the UART attempts to send data. The time is set by 
 * calling \c uart_raw_set_no_activity_detector(), with a parameter 
 * representing the number of CLK_SLOW periods over which no activity must be 
 * seen. After this happens, a NO_ACTIVITY event is generated. To generate a 
 * further NO_ACTIVITY event, it is necessary to call 
 * \c uart_raw_set_no_activity_detector() again. Calling 
 * \c uart_raw_set_no_activity_detector() will also reset the countdown.
 * 
 * \section uart_raw_callback Callback
 * The callback is a function implemented in the upper module and registered 
 * by a call to \c uart_raw_register_callback(). This is used to notify the 
 * upper module of events it should know about. Apart from the event ID, there 
 * is a second parameter for passing the size of data to the upper module. This 
 * is used just for TX_SUCCESS and RX_SUCCESS events and represents the size of
 * the data either transmitted or received. Please note that the callback is 
 * called in interrupt context.
 * 
 * \section uart_raw_tx_rx_data Sending and Receiving Data
 * It's the responsibility of the upper module to discard the data from RX
 * buffer no longer needed. It's not mandatory to free the data since the buffer
 * wraps. The decision of when to read the RX buffer is made by the upper module
 * RX_SUCCESS and RX_IDLE events can be used to figure out the right moments to 
 * access the RX buffer.
 * It's the responsibility of the upper module to discard the data from TX
 * buffer after it has been used. The upper module must not write or attempt to 
 * free the data already marked for sending. To trigger transmission, the 
 * upper module should call \c uart_raw_send_data() passing the size of data to 
 * be transmitted after it has populated the TX buffer. Please note that a call
 * to \c uart_raw_send_data() uses 8 bytes from the TX header buffer. Calling 
 * \c uart_raw_send_data() also advances pointers to mask the data that is 
 * currently passed to hardware so that it appears as used(but not freed).
 * Freeing can be done by upper module when a TX_SUCCESS event is generated
 * which is passed to the upper module to signal a successful transmission.
 * At this point the upper module should consider the data sent and can be freed
 * Every call to \c uart_raw_send_data() will use 4 bytes from the status buffer
 * when the transmission finishes.
 *
 */

#ifndef UART_RAW_H
#define UART_RAW_H

#include "hydra/hydra_types.h"
#include "buffer/buffer.h"
#include "hal/hal_uart.h"
#include "hal/hal_macros.h"

/** Enum encoding the signals used by the UART. */
typedef enum uart_signal
{
    UART_DATA_IN,
    UART_DATA_OUT,
    UART_CTSB_IN,
    UART_RTSB_OUT
} uart_signal;

/**
 * Data type used for stop bits setting.
 */
typedef enum
{
    ONE_STOP_BIT = 0,
    TWO_STOP_BITS
}uart_raw_stop_bits;

/**
 * Data type used for parity type setting.
 */
typedef enum
{
    ODD_PARITY = 0,
    EVEN_PARITY
}uart_raw_parity_type;

/**
 * Data type for event IDs.
 */
typedef enum
{
    RX_SUCCESS  = 1 << 0,
    TX_SUCCESS  = 1 << 1,
    RX_IDLE     = 1 << 2,
    NO_ACTIVITY = 1 << 3,
    INIT_DONE   = 1 << 4,
    RELEASE_DONE = 1 << 5
}uart_raw_events;

/**
 * Type for the callback function.
 */
typedef void (*uart_raw_callback)(uart_raw_events event, uint16 size);

/**
 * Called by the upper stack to perform a basic initialisation of the UART 
 * driver.
 * \param tx_data_buf TX data buffer.
 * \param tx_header_buf_size Used to control the size of the TX header buffer.
 * \param rx_data_buf RX data buffer.
 * \param status_buf_size Used to control the size of the status buffer.
 */
void uart_raw_init(BUFFER *tx_data_buf,
                   mmu_buffer_size tx_header_buf_size,
                   BUFFER *rx_data_buf,
                   mmu_buffer_size status_buf_size);

/**
 * Called by the upper module to turn off the UART hardware and destroy all 
 * buffers.
 */
void uart_raw_deinit(void);

/**
 * Configures the UART PIOs. This can be done at any time.
 * \param signal UART signal to mux on the given PIO.
 * \param pio_index PIO index. Please use NUMBER_OF_PIOS to indicate an unused
 * signal.
 */
void uart_configure_pio(uart_signal signal, uint8 pio_index);

/**                
 * Used by the upper module to register a callback. This is usually part of the 
 * init sequence. The callback always executes in interrupt context.
 * \param callback Function pointer to the callback.
 */
void uart_raw_register_callback(uart_raw_callback callback);

/**
 * Used to enable or disable the parity bit and set odd or even parity. This 
 * is usually part of the init sequence.
 * \param enable Used to enable(True) or disable(False) the use of parity bit. 
 * \param type Used to select even or odd parity when enabling the parity bit. 
 */
void uart_raw_set_parity(bool enable, uart_raw_parity_type type);

/**
 * Called by the upper module to select one or two stop bits. This is usually 
 * part of the init sequence.
 * \param stop_bits Used to select one or two stop bits.
 */
void uart_raw_set_stop_bits(uart_raw_stop_bits stop_bits);

/**
 * Called by the upper module to enable or disable RTS CTS flow control. This is
 * usually part of the init sequence. The module is designed to have the RTS 
 * line under hardware control which is enabled by calling 
 * hal_uart_set_rts_auto(). Disabling rts auto allows for software control of 
 * the RTS line but this is not used by the module.
 * \param enable Used to enable(True) or disable(False) the use of flow 
 * control.
 */
void uart_raw_flow_ctrl_enable(bool enable);

#define uart_raw_flow_ctrl_enable(enable) \
    (hal_uart_set_rts_auto(enable), hal_uart_set_flow_ctrl(enable))

/**
 * Used by the upper module to set the baud rate. This is usually part of the 
 * init sequence.
 * \param rate Used to pass the baud rate.
 */
void uart_raw_set_rate(uart_raw_rate rate);

#define uart_raw_set_rate(rate) hal_uart_set_rate(rate)

/**
 * Called to enable or disable transmission. This is usually part of the init 
 * sequence. 
 * \param enable Used to enable(True) or disable(False) transmission.
 */
void uart_raw_tx_enable(bool enable);

#define uart_raw_tx_enable(enable) hal_uart_set_tx_enable(enable)

/**
 * Called to enable or disable reception. This is usually part of the init 
 * sequence. 
 * \param enable Used to enable(True) or disable(False) reception.
 */
void uart_raw_rx_enable(bool enable);

#define uart_raw_rx_enable(enable) hal_uart_set_rx_enable(enable)

/**
 * Called by the upper module to set the transfer length. An interrupt will be 
 * generated after this many bytes have been received and an RX_SUCCESS event 
 * will be passed to the callback.
 * \param length Number of bytes to set as the transfer length.
 */
void uart_raw_set_transfer_length(uint16 length);

#define uart_raw_set_transfer_length(length) \
                             hal_uart_set_protocol_none_transfer_length(length)

/**
 * Sets UART RTS pin.
 * \param value Value to set the RTS pin to.
 */
void uart_raw_set_rts(bool value);

#define uart_raw_set_rts(value) hal_uart_set_rts(value)

/**
 * Gets UART config register value.
 */
uint16 uart_raw_get_config(void);

#define uart_raw_get_config(void) hal_get_reg_uart_config(void)

/**
 * Called by the upper module to set the RX idle timeout. The timeout is 
 * measured in bit periods, which are dependant on the baud rate. 
 * Sets the RX idle timeout in number of bit periods.
 * \param bit_periods Bit periods wait time for the RX idle mechanism.
 */
void uart_raw_set_rx_idle_timeout(uint16 bit_periods);

#define uart_raw_set_rx_idle_timeout(bit_periods) \
                                      hal_uart_set_rx_idle_timeout(bit_periods)

/**
 * Used to force a pipeline flush and put the data in the RX buffer. This 
 * should be used by the upper module when an RX_IDLE event is received. An 
 * RX_SUCCESS event is generated when this process is completed.
 */
void uart_raw_rx_force_end(void);

#define uart_raw_rx_force_end(void) hal_uart_set_rx_force_end(TRUE)

/**
 * Used by the upper module to trigger a transmission. After populating the TX 
 * data buffer a call to this function will send and consume the data. After 
 * transmission completes a TX_SUCCESS event will be generated. When the upper 
 * layer receives this event it should consider the data freed.
 * \param size Number of bytes to send.
 */
void uart_raw_send_data(uint16 size);

/**
 * Used by the upper module to set the no activity detector timeout in CLK_SLOW 
 * periods.
 * \param clk_periods Number of CLK_SLOW periods to wait until a NO_ACTIVITY 
 * event is generated.
 */
void uart_raw_set_no_activity_detector(uint16 clk_periods);

#define uart_raw_set_no_activity_detector(clk_periods) \
                                 hal_uart_set_no_activity_detector(clk_periods)

#endif /* UART_RAW_H */
