
/* *************************************************************************  *
   COMMERCIAL IN CONFIDENCE
   Copyright (C) 2017 Qualcomm Technologies International Ltd.

   Qualcomm Technologies International Ltd.
   Churchill House,
   Cambridge Business Park,
   Cowley Park,
   Cambridge, CB4 0WZ. UK
   http://www.csr.com

   $Id: //depot/bg/Releases/ADK/ADK6.2/hydra_os_aura/fw/src/core/io/aura/d01/pio_ctrl_sys_io/io_defs.h#1 $
   $Name$
   $Source$

   DESCRIPTION
      Hardware declarations header file (higher level).
      Lists masks and values for use when setting or interpreting
      the contents of memory mapped hardware registers.

   INTERFACE
      Entry   :-
      Exit    :-

   MODIFICATIONS
      1.0    24/06/99    RWY    First created.
      1.x    xx/xx/xx    RWY    Automatically generated.

*  *************************************************************************  */

#ifndef __IO_DEFS_H__
#define __IO_DEFS_H__

#define CHIP_REGISTER_HASH 0x748A


/* -- mildred_sfr_extra -- Mildred 8051 special function registers -- */

enum clkgen_active_rate_enum
{
   CLKGEN_ACTIVE_RATE_80M                   = (int)0xF,
   CLKGEN_ACTIVE_RATE_40M                   = (int)0xE,
   CLKGEN_ACTIVE_RATE_35M                   = (int)0xD,
   CLKGEN_ACTIVE_RATE_32M                   = (int)0xC,
   CLKGEN_ACTIVE_RATE_30M                   = (int)0xB,
   CLKGEN_ACTIVE_RATE_25M                   = (int)0xA,
   CLKGEN_ACTIVE_RATE_20M                   = (int)0x9,
   CLKGEN_ACTIVE_RATE_16M                   = (int)0x8,
   CLKGEN_ACTIVE_RATE_15M                   = (int)0x7,
   CLKGEN_ACTIVE_RATE_10M                   = (int)0x6,
   CLKGEN_ACTIVE_RATE_8M                    = (int)0x5,
   CLKGEN_ACTIVE_RATE_5M                    = (int)0x4,
   CLKGEN_ACTIVE_RATE_4M                    = (int)0x3,
   CLKGEN_ACTIVE_RATE_2M                    = (int)0x2,
   CLKGEN_ACTIVE_RATE_1M                    = (int)0x1,
   CLKGEN_ACTIVE_RATE_500K                  = (int)0x0,
   MAX_CLKGEN_ACTIVE_RATE                   = (int)0xF
};
typedef enum clkgen_active_rate_enum clkgen_active_rate;


enum clkgen_clk_source_sel_enum_enum
{
   CLKGEN_CLK_SOURCE_SEL_80M                = (int)0x0,
   CLKGEN_CLK_SOURCE_SEL_SLOW               = (int)0x1,
   CLKGEN_CLK_SOURCE_SEL_FOSC               = (int)0x2,
   MAX_CLKGEN_CLK_SOURCE_SEL_ENUM           = (int)0x2
};
#define NUM_CLKGEN_CLK_SOURCE_SEL_ENUM (0x3)
typedef enum clkgen_clk_source_sel_enum_enum clkgen_clk_source_sel_enum;


enum mildred_int_sources_posn_enum
{
   MILDRED_INT_SOURCE_BUS_INT_POSN                    = (int)0,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_BUS_INT_LSB_POSN = (int)0,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_BUS_INT_MSB_POSN = (int)0,
   MILDRED_INT_SOURCE_TIME_UPDATE_POSN                = (int)1,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_TIME_UPDATE_LSB_POSN = (int)1,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_TIME_UPDATE_MSB_POSN = (int)1,
   MILDRED_INT_SOURCE_PIO_RISING_POSN                 = (int)2,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_PIO_RISING_LSB_POSN = (int)2,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_PIO_RISING_MSB_POSN = (int)2,
   MILDRED_INT_SOURCE_PIO_FALLING_POSN                = (int)3,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_PIO_FALLING_LSB_POSN = (int)3,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_PIO_FALLING_MSB_POSN = (int)3,
   MILDRED_INT_SOURCE_JANITOR_POSN                    = (int)4,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_JANITOR_LSB_POSN = (int)4,
   MILDRED_INT_SOURCES_MILDRED_INT_SOURCE_JANITOR_MSB_POSN = (int)4
};
typedef enum mildred_int_sources_posn_enum mildred_int_sources_posn;

#define MILDRED_INT_SOURCE_BUS_INT_MASK          (0x00000001u)
#define MILDRED_INT_SOURCE_TIME_UPDATE_MASK      (0x00000002u)
#define MILDRED_INT_SOURCE_PIO_RISING_MASK       (0x00000004u)
#define MILDRED_INT_SOURCE_PIO_FALLING_MASK      (0x00000008u)
#define MILDRED_INT_SOURCE_JANITOR_MASK          (0x00000010u)

enum mildred_int_sources2_posn_enum
{
   MILDRED_INT_SOURCE_DUMMY_POSN                      = (int)0,
   MILDRED_INT_SOURCES2_MILDRED_INT_SOURCE_DUMMY_LSB_POSN = (int)0,
   MILDRED_INT_SOURCES2_MILDRED_INT_SOURCE_DUMMY_MSB_POSN = (int)0
};
typedef enum mildred_int_sources2_posn_enum mildred_int_sources2_posn;

#define MILDRED_INT_SOURCE_DUMMY_MASK            (0x00000001u)

enum pio_select_func_enum
{
   PIO_SELECT_FUNC_CPU                      = (int)0x0,
   PIO_SELECT_FUNC_SIU                      = (int)0x1,
   PIO_SELECT_FUNC_SPECIAL1                 = (int)0x2,
   PIO_SELECT_FUNC_SPECIAL2                 = (int)0x3,
   MAX_PIO_SELECT_FUNC                      = (int)0x3
};
#define NUM_PIO_SELECT_FUNC (0x4)
typedef enum pio_select_func_enum pio_select_func;


enum uart_rate_enum
{
   UART_RATE_2K4                            = (int)0x2,
   UART_RATE_9K6                            = (int)0x8,
   UART_RATE_19K2                           = (int)0x10,
   UART_RATE_38K4                           = (int)0x1F,
   UART_RATE_57K6                           = (int)0x2F,
   UART_RATE_115K2                          = (int)0x5E,
   UART_RATE_230K4                          = (int)0xBD,
   UART_RATE_460K8                          = (int)0x179,
   UART_RATE_921K6                          = (int)0x2F3,
   UART_RATE_1382K4                         = (int)0x46C,
   UART_RATE_1843K2                         = (int)0x5E6,
   UART_RATE_2764K8                         = (int)0x8D9,
   UART_RATE_3686K4                         = (int)0xBCC,
   UART_RATE_5529K6                         = (int)0x11B2,
   MAX_UART_RATE                            = (int)0x11B2
};
typedef enum uart_rate_enum uart_rate;


enum clkgen_active_clock_rate_posn_enum
{
   CLKGEN_ACTIVE_INT_LSB_POSN                         = (int)0,
   CLKGEN_ACTIVE_CLOCK_RATE_CLKGEN_ACTIVE_INT_LSB_POSN = (int)0,
   CLKGEN_ACTIVE_INT_MSB_POSN                         = (int)3,
   CLKGEN_ACTIVE_CLOCK_RATE_CLKGEN_ACTIVE_INT_MSB_POSN = (int)3,
   CLKGEN_ACTIVE_BUS_LSB_POSN                         = (int)4,
   CLKGEN_ACTIVE_CLOCK_RATE_CLKGEN_ACTIVE_BUS_LSB_POSN = (int)4,
   CLKGEN_ACTIVE_BUS_MSB_POSN                         = (int)7,
   CLKGEN_ACTIVE_CLOCK_RATE_CLKGEN_ACTIVE_BUS_MSB_POSN = (int)7
};
typedef enum clkgen_active_clock_rate_posn_enum clkgen_active_clock_rate_posn;

#define CLKGEN_ACTIVE_INT_LSB_MASK               (0x00000001u)
#define CLKGEN_ACTIVE_INT_MSB_MASK               (0x00000008u)
#define CLKGEN_ACTIVE_BUS_LSB_MASK               (0x00000010u)
#define CLKGEN_ACTIVE_BUS_MSB_MASK               (0x00000080u)

enum clkgen_activity_enables_posn_enum
{
   CLKGEN_ACTIVITY_EN_INT_POSN                        = (int)0,
   CLKGEN_ACTIVITY_ENABLES_CLKGEN_ACTIVITY_EN_INT_LSB_POSN = (int)0,
   CLKGEN_ACTIVITY_ENABLES_CLKGEN_ACTIVITY_EN_INT_MSB_POSN = (int)0,
   CLKGEN_ACTIVITY_EN_SPI_POSN                        = (int)1,
   CLKGEN_ACTIVITY_ENABLES_CLKGEN_ACTIVITY_EN_SPI_LSB_POSN = (int)1,
   CLKGEN_ACTIVITY_ENABLES_CLKGEN_ACTIVITY_EN_SPI_MSB_POSN = (int)1,
   CLKGEN_ACTIVITY_EN_BUS_POSN                        = (int)2,
   CLKGEN_ACTIVITY_ENABLES_CLKGEN_ACTIVITY_EN_BUS_LSB_POSN = (int)2,
   CLKGEN_ACTIVITY_ENABLES_CLKGEN_ACTIVITY_EN_BUS_MSB_POSN = (int)2
};
typedef enum clkgen_activity_enables_posn_enum clkgen_activity_enables_posn;

#define CLKGEN_ACTIVITY_EN_INT_MASK              (0x00000001u)
#define CLKGEN_ACTIVITY_EN_SPI_MASK              (0x00000002u)
#define CLKGEN_ACTIVITY_EN_BUS_MASK              (0x00000004u)

enum clkgen_activity_status_posn_enum
{
   CLKGEN_STATUS_BUS_ACTIVE_POSN                      = (int)0,
   CLKGEN_ACTIVITY_STATUS_CLKGEN_STATUS_BUS_ACTIVE_LSB_POSN = (int)0,
   CLKGEN_ACTIVITY_STATUS_CLKGEN_STATUS_BUS_ACTIVE_MSB_POSN = (int)0,
   CLKGEN_STATUS_INT_ACTIVE_POSN                      = (int)1,
   CLKGEN_ACTIVITY_STATUS_CLKGEN_STATUS_INT_ACTIVE_LSB_POSN = (int)1,
   CLKGEN_ACTIVITY_STATUS_CLKGEN_STATUS_INT_ACTIVE_MSB_POSN = (int)1,
   CLKGEN_STATUS_SPI_ACTIVE_POSN                      = (int)2,
   CLKGEN_ACTIVITY_STATUS_CLKGEN_STATUS_SPI_ACTIVE_LSB_POSN = (int)2,
   CLKGEN_ACTIVITY_STATUS_CLKGEN_STATUS_SPI_ACTIVE_MSB_POSN = (int)2
};
typedef enum clkgen_activity_status_posn_enum clkgen_activity_status_posn;

#define CLKGEN_STATUS_BUS_ACTIVE_MASK            (0x00000001u)
#define CLKGEN_STATUS_INT_ACTIVE_MASK            (0x00000002u)
#define CLKGEN_STATUS_SPI_ACTIVE_MASK            (0x00000004u)

enum clkgen_enables_posn_enum
{
   CLKGEN_REGS_EN_POSN                                = (int)0,
   CLKGEN_ENABLES_CLKGEN_REGS_EN_LSB_POSN             = (int)0,
   CLKGEN_ENABLES_CLKGEN_REGS_EN_MSB_POSN             = (int)0,
   CLKGEN_CLK_SOURCE_SEL_A_LSB_POSN                   = (int)1,
   CLKGEN_ENABLES_CLKGEN_CLK_SOURCE_SEL_A_LSB_POSN    = (int)1,
   CLKGEN_CLK_SOURCE_SEL_A_MSB_POSN                   = (int)2,
   CLKGEN_ENABLES_CLKGEN_CLK_SOURCE_SEL_A_MSB_POSN    = (int)2,
   CLKGEN_CLK_SOURCE_SEL_B_LSB_POSN                   = (int)3,
   CLKGEN_ENABLES_CLKGEN_CLK_SOURCE_SEL_B_LSB_POSN    = (int)3,
   CLKGEN_CLK_SOURCE_SEL_B_MSB_POSN                   = (int)4,
   CLKGEN_ENABLES_CLKGEN_CLK_SOURCE_SEL_B_MSB_POSN    = (int)4,
   CLKGEN_CLK_SOURCE_SEL_MUX_POSN                     = (int)5,
   CLKGEN_ENABLES_CLKGEN_CLK_SOURCE_SEL_MUX_LSB_POSN  = (int)5,
   CLKGEN_ENABLES_CLKGEN_CLK_SOURCE_SEL_MUX_MSB_POSN  = (int)5,
   CLKGEN_PIO_EN_POSN                                 = (int)6,
   CLKGEN_ENABLES_CLKGEN_PIO_EN_LSB_POSN              = (int)6,
   CLKGEN_ENABLES_CLKGEN_PIO_EN_MSB_POSN              = (int)6
};
typedef enum clkgen_enables_posn_enum clkgen_enables_posn;

#define CLKGEN_REGS_EN_MASK                      (0x00000001u)
#define CLKGEN_CLK_SOURCE_SEL_A_LSB_MASK         (0x00000002u)
#define CLKGEN_CLK_SOURCE_SEL_A_MSB_MASK         (0x00000004u)
#define CLKGEN_CLK_SOURCE_SEL_B_LSB_MASK         (0x00000008u)
#define CLKGEN_CLK_SOURCE_SEL_B_MSB_MASK         (0x00000010u)
#define CLKGEN_CLK_SOURCE_SEL_MUX_MASK           (0x00000020u)
#define CLKGEN_PIO_EN_MASK                       (0x00000040u)

enum clkgen_clk_source_sel_mux_enum
{
   CLKGEN_CLK_SOURCE_SEL_MUX_A              = (int)0x0,
   CLKGEN_CLK_SOURCE_SEL_MUX_B              = (int)0x1,
   MAX_CLKGEN_CLK_SOURCE_SEL_MUX            = (int)0x1
};
#define NUM_CLKGEN_CLK_SOURCE_SEL_MUX (0x2)
typedef enum clkgen_clk_source_sel_mux_enum clkgen_clk_source_sel_mux;


enum clkgen_status_posn_enum
{
   CLKGEN_STATUS_SLOW_CLK_MON_INT_POSN                = (int)0,
   CLKGEN_STATUS_CLKGEN_STATUS_SLOW_CLK_MON_INT_LSB_POSN = (int)0,
   CLKGEN_STATUS_CLKGEN_STATUS_SLOW_CLK_MON_INT_MSB_POSN = (int)0
};
typedef enum clkgen_status_posn_enum clkgen_status_posn;

#define CLKGEN_STATUS_SLOW_CLK_MON_INT_MASK      (0x00000001u)

enum clkgen_timer_enables_posn_enum
{
   CLKGEN_TIMER_EN_POSN                               = (int)0,
   CLKGEN_TIMER_ENABLES_CLKGEN_TIMER_EN_LSB_POSN      = (int)0,
   CLKGEN_TIMER_ENABLES_CLKGEN_TIMER_EN_MSB_POSN      = (int)0,
   CLKGEN_TIMER_FAST_EN_POSN                          = (int)1,
   CLKGEN_TIMER_ENABLES_CLKGEN_TIMER_FAST_EN_LSB_POSN = (int)1,
   CLKGEN_TIMER_ENABLES_CLKGEN_TIMER_FAST_EN_MSB_POSN = (int)1
};
typedef enum clkgen_timer_enables_posn_enum clkgen_timer_enables_posn;

#define CLKGEN_TIMER_EN_MASK                     (0x00000001u)
#define CLKGEN_TIMER_FAST_EN_MASK                (0x00000002u)

enum mildred_p0_posn_enum
{
   MILDRED_P0_0_POSN                                  = (int)0,
   MILDRED_P0_MILDRED_P0_0_LSB_POSN                   = (int)0,
   MILDRED_P0_MILDRED_P0_0_MSB_POSN                   = (int)0,
   MILDRED_P0_1_POSN                                  = (int)1,
   MILDRED_P0_MILDRED_P0_1_LSB_POSN                   = (int)1,
   MILDRED_P0_MILDRED_P0_1_MSB_POSN                   = (int)1,
   MILDRED_P0_2_POSN                                  = (int)2,
   MILDRED_P0_MILDRED_P0_2_LSB_POSN                   = (int)2,
   MILDRED_P0_MILDRED_P0_2_MSB_POSN                   = (int)2,
   MILDRED_P0_3_POSN                                  = (int)3,
   MILDRED_P0_MILDRED_P0_3_LSB_POSN                   = (int)3,
   MILDRED_P0_MILDRED_P0_3_MSB_POSN                   = (int)3,
   MILDRED_P0_4_POSN                                  = (int)4,
   MILDRED_P0_MILDRED_P0_4_LSB_POSN                   = (int)4,
   MILDRED_P0_MILDRED_P0_4_MSB_POSN                   = (int)4,
   MILDRED_P0_5_POSN                                  = (int)5,
   MILDRED_P0_MILDRED_P0_5_LSB_POSN                   = (int)5,
   MILDRED_P0_MILDRED_P0_5_MSB_POSN                   = (int)5,
   MILDRED_P0_6_POSN                                  = (int)6,
   MILDRED_P0_MILDRED_P0_6_LSB_POSN                   = (int)6,
   MILDRED_P0_MILDRED_P0_6_MSB_POSN                   = (int)6,
   MILDRED_P0_7_POSN                                  = (int)7,
   MILDRED_P0_MILDRED_P0_7_LSB_POSN                   = (int)7,
   MILDRED_P0_MILDRED_P0_7_MSB_POSN                   = (int)7
};
typedef enum mildred_p0_posn_enum mildred_p0_posn;

#define MILDRED_P0_0_MASK                        (0x00000001u)
#define MILDRED_P0_1_MASK                        (0x00000002u)
#define MILDRED_P0_2_MASK                        (0x00000004u)
#define MILDRED_P0_3_MASK                        (0x00000008u)
#define MILDRED_P0_4_MASK                        (0x00000010u)
#define MILDRED_P0_5_MASK                        (0x00000020u)
#define MILDRED_P0_6_MASK                        (0x00000040u)
#define MILDRED_P0_7_MASK                        (0x00000080u)

enum mildred_p0_drive_en_posn_enum
{
   MILDRED_P0_DRIVE_EN_0_POSN                         = (int)0,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_0_LSB_POSN = (int)0,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_0_MSB_POSN = (int)0,
   MILDRED_P0_DRIVE_EN_1_POSN                         = (int)1,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_1_LSB_POSN = (int)1,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_1_MSB_POSN = (int)1,
   MILDRED_P0_DRIVE_EN_2_POSN                         = (int)2,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_2_LSB_POSN = (int)2,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_2_MSB_POSN = (int)2,
   MILDRED_P0_DRIVE_EN_3_POSN                         = (int)3,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_3_LSB_POSN = (int)3,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_3_MSB_POSN = (int)3,
   MILDRED_P0_DRIVE_EN_4_POSN                         = (int)4,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_4_LSB_POSN = (int)4,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_4_MSB_POSN = (int)4,
   MILDRED_P0_DRIVE_EN_5_POSN                         = (int)5,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_5_LSB_POSN = (int)5,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_5_MSB_POSN = (int)5,
   MILDRED_P0_DRIVE_EN_6_POSN                         = (int)6,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_6_LSB_POSN = (int)6,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_6_MSB_POSN = (int)6,
   MILDRED_P0_DRIVE_EN_7_POSN                         = (int)7,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_7_LSB_POSN = (int)7,
   MILDRED_P0_DRIVE_EN_MILDRED_P0_DRIVE_EN_7_MSB_POSN = (int)7
};
typedef enum mildred_p0_drive_en_posn_enum mildred_p0_drive_en_posn;

#define MILDRED_P0_DRIVE_EN_0_MASK               (0x00000001u)
#define MILDRED_P0_DRIVE_EN_1_MASK               (0x00000002u)
#define MILDRED_P0_DRIVE_EN_2_MASK               (0x00000004u)
#define MILDRED_P0_DRIVE_EN_3_MASK               (0x00000008u)
#define MILDRED_P0_DRIVE_EN_4_MASK               (0x00000010u)
#define MILDRED_P0_DRIVE_EN_5_MASK               (0x00000020u)
#define MILDRED_P0_DRIVE_EN_6_MASK               (0x00000040u)
#define MILDRED_P0_DRIVE_EN_7_MASK               (0x00000080u)

enum mildred_p1_posn_enum
{
   MILDRED_P1_0_POSN                                  = (int)0,
   MILDRED_P1_MILDRED_P1_0_LSB_POSN                   = (int)0,
   MILDRED_P1_MILDRED_P1_0_MSB_POSN                   = (int)0,
   MILDRED_P1_1_POSN                                  = (int)1,
   MILDRED_P1_MILDRED_P1_1_LSB_POSN                   = (int)1,
   MILDRED_P1_MILDRED_P1_1_MSB_POSN                   = (int)1,
   MILDRED_P1_2_POSN                                  = (int)2,
   MILDRED_P1_MILDRED_P1_2_LSB_POSN                   = (int)2,
   MILDRED_P1_MILDRED_P1_2_MSB_POSN                   = (int)2,
   MILDRED_P1_3_POSN                                  = (int)3,
   MILDRED_P1_MILDRED_P1_3_LSB_POSN                   = (int)3,
   MILDRED_P1_MILDRED_P1_3_MSB_POSN                   = (int)3,
   MILDRED_P1_4_POSN                                  = (int)4,
   MILDRED_P1_MILDRED_P1_4_LSB_POSN                   = (int)4,
   MILDRED_P1_MILDRED_P1_4_MSB_POSN                   = (int)4,
   MILDRED_P1_5_POSN                                  = (int)5,
   MILDRED_P1_MILDRED_P1_5_LSB_POSN                   = (int)5,
   MILDRED_P1_MILDRED_P1_5_MSB_POSN                   = (int)5,
   MILDRED_P1_6_POSN                                  = (int)6,
   MILDRED_P1_MILDRED_P1_6_LSB_POSN                   = (int)6,
   MILDRED_P1_MILDRED_P1_6_MSB_POSN                   = (int)6,
   MILDRED_P1_7_POSN                                  = (int)7,
   MILDRED_P1_MILDRED_P1_7_LSB_POSN                   = (int)7,
   MILDRED_P1_MILDRED_P1_7_MSB_POSN                   = (int)7
};
typedef enum mildred_p1_posn_enum mildred_p1_posn;

#define MILDRED_P1_0_MASK                        (0x00000001u)
#define MILDRED_P1_1_MASK                        (0x00000002u)
#define MILDRED_P1_2_MASK                        (0x00000004u)
#define MILDRED_P1_3_MASK                        (0x00000008u)
#define MILDRED_P1_4_MASK                        (0x00000010u)
#define MILDRED_P1_5_MASK                        (0x00000020u)
#define MILDRED_P1_6_MASK                        (0x00000040u)
#define MILDRED_P1_7_MASK                        (0x00000080u)

enum mildred_p1_drive_en_posn_enum
{
   MILDRED_P1_DRIVE_EN_0_POSN                         = (int)0,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_0_LSB_POSN = (int)0,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_0_MSB_POSN = (int)0,
   MILDRED_P1_DRIVE_EN_1_POSN                         = (int)1,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_1_LSB_POSN = (int)1,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_1_MSB_POSN = (int)1,
   MILDRED_P1_DRIVE_EN_2_POSN                         = (int)2,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_2_LSB_POSN = (int)2,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_2_MSB_POSN = (int)2,
   MILDRED_P1_DRIVE_EN_3_POSN                         = (int)3,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_3_LSB_POSN = (int)3,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_3_MSB_POSN = (int)3,
   MILDRED_P1_DRIVE_EN_4_POSN                         = (int)4,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_4_LSB_POSN = (int)4,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_4_MSB_POSN = (int)4,
   MILDRED_P1_DRIVE_EN_5_POSN                         = (int)5,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_5_LSB_POSN = (int)5,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_5_MSB_POSN = (int)5,
   MILDRED_P1_DRIVE_EN_6_POSN                         = (int)6,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_6_LSB_POSN = (int)6,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_6_MSB_POSN = (int)6,
   MILDRED_P1_DRIVE_EN_7_POSN                         = (int)7,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_7_LSB_POSN = (int)7,
   MILDRED_P1_DRIVE_EN_MILDRED_P1_DRIVE_EN_7_MSB_POSN = (int)7
};
typedef enum mildred_p1_drive_en_posn_enum mildred_p1_drive_en_posn;

#define MILDRED_P1_DRIVE_EN_0_MASK               (0x00000001u)
#define MILDRED_P1_DRIVE_EN_1_MASK               (0x00000002u)
#define MILDRED_P1_DRIVE_EN_2_MASK               (0x00000004u)
#define MILDRED_P1_DRIVE_EN_3_MASK               (0x00000008u)
#define MILDRED_P1_DRIVE_EN_4_MASK               (0x00000010u)
#define MILDRED_P1_DRIVE_EN_5_MASK               (0x00000020u)
#define MILDRED_P1_DRIVE_EN_6_MASK               (0x00000040u)
#define MILDRED_P1_DRIVE_EN_7_MASK               (0x00000080u)

enum mildred_p2_posn_enum
{
   MILDRED_P2_0_POSN                                  = (int)0,
   MILDRED_P2_MILDRED_P2_0_LSB_POSN                   = (int)0,
   MILDRED_P2_MILDRED_P2_0_MSB_POSN                   = (int)0,
   MILDRED_P2_1_POSN                                  = (int)1,
   MILDRED_P2_MILDRED_P2_1_LSB_POSN                   = (int)1,
   MILDRED_P2_MILDRED_P2_1_MSB_POSN                   = (int)1,
   MILDRED_P2_2_POSN                                  = (int)2,
   MILDRED_P2_MILDRED_P2_2_LSB_POSN                   = (int)2,
   MILDRED_P2_MILDRED_P2_2_MSB_POSN                   = (int)2,
   MILDRED_P2_3_POSN                                  = (int)3,
   MILDRED_P2_MILDRED_P2_3_LSB_POSN                   = (int)3,
   MILDRED_P2_MILDRED_P2_3_MSB_POSN                   = (int)3,
   MILDRED_P2_4_POSN                                  = (int)4,
   MILDRED_P2_MILDRED_P2_4_LSB_POSN                   = (int)4,
   MILDRED_P2_MILDRED_P2_4_MSB_POSN                   = (int)4,
   MILDRED_P2_5_POSN                                  = (int)5,
   MILDRED_P2_MILDRED_P2_5_LSB_POSN                   = (int)5,
   MILDRED_P2_MILDRED_P2_5_MSB_POSN                   = (int)5,
   MILDRED_P2_6_POSN                                  = (int)6,
   MILDRED_P2_MILDRED_P2_6_LSB_POSN                   = (int)6,
   MILDRED_P2_MILDRED_P2_6_MSB_POSN                   = (int)6,
   MILDRED_P2_7_POSN                                  = (int)7,
   MILDRED_P2_MILDRED_P2_7_LSB_POSN                   = (int)7,
   MILDRED_P2_MILDRED_P2_7_MSB_POSN                   = (int)7
};
typedef enum mildred_p2_posn_enum mildred_p2_posn;

#define MILDRED_P2_0_MASK                        (0x00000001u)
#define MILDRED_P2_1_MASK                        (0x00000002u)
#define MILDRED_P2_2_MASK                        (0x00000004u)
#define MILDRED_P2_3_MASK                        (0x00000008u)
#define MILDRED_P2_4_MASK                        (0x00000010u)
#define MILDRED_P2_5_MASK                        (0x00000020u)
#define MILDRED_P2_6_MASK                        (0x00000040u)
#define MILDRED_P2_7_MASK                        (0x00000080u)

enum mildred_p2_drive_en_posn_enum
{
   MILDRED_P2_DRIVE_EN_0_POSN                         = (int)0,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_0_LSB_POSN = (int)0,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_0_MSB_POSN = (int)0,
   MILDRED_P2_DRIVE_EN_1_POSN                         = (int)1,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_1_LSB_POSN = (int)1,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_1_MSB_POSN = (int)1,
   MILDRED_P2_DRIVE_EN_2_POSN                         = (int)2,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_2_LSB_POSN = (int)2,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_2_MSB_POSN = (int)2,
   MILDRED_P2_DRIVE_EN_3_POSN                         = (int)3,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_3_LSB_POSN = (int)3,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_3_MSB_POSN = (int)3,
   MILDRED_P2_DRIVE_EN_4_POSN                         = (int)4,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_4_LSB_POSN = (int)4,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_4_MSB_POSN = (int)4,
   MILDRED_P2_DRIVE_EN_5_POSN                         = (int)5,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_5_LSB_POSN = (int)5,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_5_MSB_POSN = (int)5,
   MILDRED_P2_DRIVE_EN_6_POSN                         = (int)6,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_6_LSB_POSN = (int)6,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_6_MSB_POSN = (int)6,
   MILDRED_P2_DRIVE_EN_7_POSN                         = (int)7,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_7_LSB_POSN = (int)7,
   MILDRED_P2_DRIVE_EN_MILDRED_P2_DRIVE_EN_7_MSB_POSN = (int)7
};
typedef enum mildred_p2_drive_en_posn_enum mildred_p2_drive_en_posn;

#define MILDRED_P2_DRIVE_EN_0_MASK               (0x00000001u)
#define MILDRED_P2_DRIVE_EN_1_MASK               (0x00000002u)
#define MILDRED_P2_DRIVE_EN_2_MASK               (0x00000004u)
#define MILDRED_P2_DRIVE_EN_3_MASK               (0x00000008u)
#define MILDRED_P2_DRIVE_EN_4_MASK               (0x00000010u)
#define MILDRED_P2_DRIVE_EN_5_MASK               (0x00000020u)
#define MILDRED_P2_DRIVE_EN_6_MASK               (0x00000040u)
#define MILDRED_P2_DRIVE_EN_7_MASK               (0x00000080u)

enum mildred_p3_posn_enum
{
   MILDRED_P3_0_POSN                                  = (int)0,
   MILDRED_P3_MILDRED_P3_0_LSB_POSN                   = (int)0,
   MILDRED_P3_MILDRED_P3_0_MSB_POSN                   = (int)0,
   MILDRED_P3_1_POSN                                  = (int)1,
   MILDRED_P3_MILDRED_P3_1_LSB_POSN                   = (int)1,
   MILDRED_P3_MILDRED_P3_1_MSB_POSN                   = (int)1,
   MILDRED_P3_2_POSN                                  = (int)2,
   MILDRED_P3_MILDRED_P3_2_LSB_POSN                   = (int)2,
   MILDRED_P3_MILDRED_P3_2_MSB_POSN                   = (int)2,
   MILDRED_P3_3_POSN                                  = (int)3,
   MILDRED_P3_MILDRED_P3_3_LSB_POSN                   = (int)3,
   MILDRED_P3_MILDRED_P3_3_MSB_POSN                   = (int)3,
   MILDRED_P3_4_POSN                                  = (int)4,
   MILDRED_P3_MILDRED_P3_4_LSB_POSN                   = (int)4,
   MILDRED_P3_MILDRED_P3_4_MSB_POSN                   = (int)4,
   MILDRED_P3_5_POSN                                  = (int)5,
   MILDRED_P3_MILDRED_P3_5_LSB_POSN                   = (int)5,
   MILDRED_P3_MILDRED_P3_5_MSB_POSN                   = (int)5,
   MILDRED_P3_6_POSN                                  = (int)6,
   MILDRED_P3_MILDRED_P3_6_LSB_POSN                   = (int)6,
   MILDRED_P3_MILDRED_P3_6_MSB_POSN                   = (int)6,
   MILDRED_P3_7_POSN                                  = (int)7,
   MILDRED_P3_MILDRED_P3_7_LSB_POSN                   = (int)7,
   MILDRED_P3_MILDRED_P3_7_MSB_POSN                   = (int)7
};
typedef enum mildred_p3_posn_enum mildred_p3_posn;

#define MILDRED_P3_0_MASK                        (0x00000001u)
#define MILDRED_P3_1_MASK                        (0x00000002u)
#define MILDRED_P3_2_MASK                        (0x00000004u)
#define MILDRED_P3_3_MASK                        (0x00000008u)
#define MILDRED_P3_4_MASK                        (0x00000010u)
#define MILDRED_P3_5_MASK                        (0x00000020u)
#define MILDRED_P3_6_MASK                        (0x00000040u)
#define MILDRED_P3_7_MASK                        (0x00000080u)

enum mildred_p3_drive_en_posn_enum
{
   MILDRED_P3_DRIVE_EN_0_POSN                         = (int)0,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_0_LSB_POSN = (int)0,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_0_MSB_POSN = (int)0,
   MILDRED_P3_DRIVE_EN_1_POSN                         = (int)1,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_1_LSB_POSN = (int)1,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_1_MSB_POSN = (int)1,
   MILDRED_P3_DRIVE_EN_2_POSN                         = (int)2,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_2_LSB_POSN = (int)2,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_2_MSB_POSN = (int)2,
   MILDRED_P3_DRIVE_EN_3_POSN                         = (int)3,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_3_LSB_POSN = (int)3,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_3_MSB_POSN = (int)3,
   MILDRED_P3_DRIVE_EN_4_POSN                         = (int)4,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_4_LSB_POSN = (int)4,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_4_MSB_POSN = (int)4,
   MILDRED_P3_DRIVE_EN_5_POSN                         = (int)5,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_5_LSB_POSN = (int)5,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_5_MSB_POSN = (int)5,
   MILDRED_P3_DRIVE_EN_6_POSN                         = (int)6,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_6_LSB_POSN = (int)6,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_6_MSB_POSN = (int)6,
   MILDRED_P3_DRIVE_EN_7_POSN                         = (int)7,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_7_LSB_POSN = (int)7,
   MILDRED_P3_DRIVE_EN_MILDRED_P3_DRIVE_EN_7_MSB_POSN = (int)7
};
typedef enum mildred_p3_drive_en_posn_enum mildred_p3_drive_en_posn;

#define MILDRED_P3_DRIVE_EN_0_MASK               (0x00000001u)
#define MILDRED_P3_DRIVE_EN_1_MASK               (0x00000002u)
#define MILDRED_P3_DRIVE_EN_2_MASK               (0x00000004u)
#define MILDRED_P3_DRIVE_EN_3_MASK               (0x00000008u)
#define MILDRED_P3_DRIVE_EN_4_MASK               (0x00000010u)
#define MILDRED_P3_DRIVE_EN_5_MASK               (0x00000020u)
#define MILDRED_P3_DRIVE_EN_6_MASK               (0x00000040u)
#define MILDRED_P3_DRIVE_EN_7_MASK               (0x00000080u)

enum clkgen_clock_rate_status_hi_posn_enum
{
   CLKGEN_CLOCK_RATE_STATUS_HI_LSB_POSN               = (int)0,
   CLKGEN_CLOCK_RATE_STATUS_HI_CLKGEN_CLOCK_RATE_STATUS_HI_LSB_POSN = (int)0,
   CLKGEN_CLOCK_RATE_STATUS_HI_MSB_POSN               = (int)3,
   CLKGEN_CLOCK_RATE_STATUS_HI_CLKGEN_CLOCK_RATE_STATUS_HI_MSB_POSN = (int)3
};
typedef enum clkgen_clock_rate_status_hi_posn_enum clkgen_clock_rate_status_hi_posn;

#define CLKGEN_CLOCK_RATE_STATUS_HI_LSB_MASK     (0x00000001u)
#define CLKGEN_CLOCK_RATE_STATUS_HI_MSB_MASK     (0x00000008u)

enum clkgen_clock_rate_status_lo_posn_enum
{
   CLKGEN_CLOCK_RATE_STATUS_LO_LSB_POSN               = (int)0,
   CLKGEN_CLOCK_RATE_STATUS_LO_CLKGEN_CLOCK_RATE_STATUS_LO_LSB_POSN = (int)0,
   CLKGEN_CLOCK_RATE_STATUS_LO_MSB_POSN               = (int)7,
   CLKGEN_CLOCK_RATE_STATUS_LO_CLKGEN_CLOCK_RATE_STATUS_LO_MSB_POSN = (int)7
};
typedef enum clkgen_clock_rate_status_lo_posn_enum clkgen_clock_rate_status_lo_posn;

#define CLKGEN_CLOCK_RATE_STATUS_LO_LSB_MASK     (0x00000001u)
#define CLKGEN_CLOCK_RATE_STATUS_LO_MSB_MASK     (0x00000080u)

enum clkgen_debug_select_posn_enum
{
   CLKGEN_DEBUG_SELECT_LSB_POSN                       = (int)0,
   CLKGEN_DEBUG_SELECT_CLKGEN_DEBUG_SELECT_LSB_POSN   = (int)0,
   CLKGEN_DEBUG_SELECT_MSB_POSN                       = (int)3,
   CLKGEN_DEBUG_SELECT_CLKGEN_DEBUG_SELECT_MSB_POSN   = (int)3
};
typedef enum clkgen_debug_select_posn_enum clkgen_debug_select_posn;

#define CLKGEN_DEBUG_SELECT_LSB_MASK             (0x00000001u)
#define CLKGEN_DEBUG_SELECT_MSB_MASK             (0x00000008u)

enum clkgen_max_clock_rate_posn_enum
{
   CLKGEN_MAX_CLOCK_RATE_LSB_POSN                     = (int)0,
   CLKGEN_MAX_CLOCK_RATE_CLKGEN_MAX_CLOCK_RATE_LSB_POSN = (int)0,
   CLKGEN_MAX_CLOCK_RATE_MSB_POSN                     = (int)7,
   CLKGEN_MAX_CLOCK_RATE_CLKGEN_MAX_CLOCK_RATE_MSB_POSN = (int)7
};
typedef enum clkgen_max_clock_rate_posn_enum clkgen_max_clock_rate_posn;

#define CLKGEN_MAX_CLOCK_RATE_LSB_MASK           (0x00000001u)
#define CLKGEN_MAX_CLOCK_RATE_MSB_MASK           (0x00000080u)

enum clkgen_min_clock_rate_posn_enum
{
   CLKGEN_MIN_CLOCK_RATE_LSB_POSN                     = (int)0,
   CLKGEN_MIN_CLOCK_RATE_CLKGEN_MIN_CLOCK_RATE_LSB_POSN = (int)0,
   CLKGEN_MIN_CLOCK_RATE_MSB_POSN                     = (int)7,
   CLKGEN_MIN_CLOCK_RATE_CLKGEN_MIN_CLOCK_RATE_MSB_POSN = (int)7
};
typedef enum clkgen_min_clock_rate_posn_enum clkgen_min_clock_rate_posn;

#define CLKGEN_MIN_CLOCK_RATE_LSB_MASK           (0x00000001u)
#define CLKGEN_MIN_CLOCK_RATE_MSB_MASK           (0x00000080u)

enum clkgen_timer_fast_status_hi_posn_enum
{
   CLKGEN_TIMER_FAST_STATUS_HI_LSB_POSN               = (int)0,
   CLKGEN_TIMER_FAST_STATUS_HI_CLKGEN_TIMER_FAST_STATUS_HI_LSB_POSN = (int)0,
   CLKGEN_TIMER_FAST_STATUS_HI_MSB_POSN               = (int)7,
   CLKGEN_TIMER_FAST_STATUS_HI_CLKGEN_TIMER_FAST_STATUS_HI_MSB_POSN = (int)7
};
typedef enum clkgen_timer_fast_status_hi_posn_enum clkgen_timer_fast_status_hi_posn;

#define CLKGEN_TIMER_FAST_STATUS_HI_LSB_MASK     (0x00000001u)
#define CLKGEN_TIMER_FAST_STATUS_HI_MSB_MASK     (0x00000080u)

enum clkgen_timer_fast_status_lo_posn_enum
{
   CLKGEN_TIMER_FAST_STATUS_LO_LSB_POSN               = (int)0,
   CLKGEN_TIMER_FAST_STATUS_LO_CLKGEN_TIMER_FAST_STATUS_LO_LSB_POSN = (int)0,
   CLKGEN_TIMER_FAST_STATUS_LO_MSB_POSN               = (int)7,
   CLKGEN_TIMER_FAST_STATUS_LO_CLKGEN_TIMER_FAST_STATUS_LO_MSB_POSN = (int)7
};
typedef enum clkgen_timer_fast_status_lo_posn_enum clkgen_timer_fast_status_lo_posn;

#define CLKGEN_TIMER_FAST_STATUS_LO_LSB_MASK     (0x00000001u)
#define CLKGEN_TIMER_FAST_STATUS_LO_MSB_MASK     (0x00000080u)

enum janitor_mailbox_in_posn_enum
{
   JANITOR_MAILBOX_IN_LSB_POSN                        = (int)0,
   JANITOR_MAILBOX_IN_JANITOR_MAILBOX_IN_LSB_POSN     = (int)0,
   JANITOR_MAILBOX_IN_MSB_POSN                        = (int)7,
   JANITOR_MAILBOX_IN_JANITOR_MAILBOX_IN_MSB_POSN     = (int)7
};
typedef enum janitor_mailbox_in_posn_enum janitor_mailbox_in_posn;

#define JANITOR_MAILBOX_IN_LSB_MASK              (0x00000001u)
#define JANITOR_MAILBOX_IN_MSB_MASK              (0x00000080u)

enum janitor_mailbox_out_posn_enum
{
   JANITOR_MAILBOX_OUT_LSB_POSN                       = (int)0,
   JANITOR_MAILBOX_OUT_JANITOR_MAILBOX_OUT_LSB_POSN   = (int)0,
   JANITOR_MAILBOX_OUT_MSB_POSN                       = (int)7,
   JANITOR_MAILBOX_OUT_JANITOR_MAILBOX_OUT_MSB_POSN   = (int)7
};
typedef enum janitor_mailbox_out_posn_enum janitor_mailbox_out_posn;

#define JANITOR_MAILBOX_OUT_LSB_MASK             (0x00000001u)
#define JANITOR_MAILBOX_OUT_MSB_MASK             (0x00000080u)

enum mildred_deep_sleep_en_posn_enum
{
   MILDRED_DEEP_SLEEP_EN_POSN                         = (int)0,
   MILDRED_DEEP_SLEEP_EN_MILDRED_DEEP_SLEEP_EN_LSB_POSN = (int)0,
   MILDRED_DEEP_SLEEP_EN_MILDRED_DEEP_SLEEP_EN_MSB_POSN = (int)0
};
typedef enum mildred_deep_sleep_en_posn_enum mildred_deep_sleep_en_posn;

#define MILDRED_DEEP_SLEEP_EN_MASK               (0x00000001u)

enum mildred_int_en_posn_enum
{
   MILDRED_INT_EN_POSN                                = (int)0,
   MILDRED_INT_EN_MILDRED_INT_EN_LSB_POSN             = (int)0,
   MILDRED_INT_EN_MILDRED_INT_EN_MSB_POSN             = (int)0
};
typedef enum mildred_int_en_posn_enum mildred_int_en_posn;

#define MILDRED_INT_EN_MASK                      (0x00000001u)

enum mildred_int_vector_hi_posn_enum
{
   MILDRED_INT_VECTOR_HI_LSB_POSN                     = (int)0,
   MILDRED_INT_VECTOR_HI_MILDRED_INT_VECTOR_HI_LSB_POSN = (int)0,
   MILDRED_INT_VECTOR_HI_MSB_POSN                     = (int)7,
   MILDRED_INT_VECTOR_HI_MILDRED_INT_VECTOR_HI_MSB_POSN = (int)7
};
typedef enum mildred_int_vector_hi_posn_enum mildred_int_vector_hi_posn;

#define MILDRED_INT_VECTOR_HI_LSB_MASK           (0x00000001u)
#define MILDRED_INT_VECTOR_HI_MSB_MASK           (0x00000080u)

enum mildred_pio_bank_select_posn_enum
{
   MILDRED_PIO_BANK_SELECT_LSB_POSN                   = (int)0,
   MILDRED_PIO_BANK_SELECT_MILDRED_PIO_BANK_SELECT_LSB_POSN = (int)0,
   MILDRED_PIO_BANK_SELECT_MSB_POSN                   = (int)1,
   MILDRED_PIO_BANK_SELECT_MILDRED_PIO_BANK_SELECT_MSB_POSN = (int)1
};
typedef enum mildred_pio_bank_select_posn_enum mildred_pio_bank_select_posn;

#define MILDRED_PIO_BANK_SELECT_LSB_MASK         (0x00000001u)
#define MILDRED_PIO_BANK_SELECT_MSB_MASK         (0x00000002u)

enum mildred_pio_select_0300_posn_enum
{
   MILDRED_PIO_SELECT_0300_LSB_POSN                   = (int)0,
   MILDRED_PIO_SELECT_0300_MILDRED_PIO_SELECT_0300_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_0300_MSB_POSN                   = (int)7,
   MILDRED_PIO_SELECT_0300_MILDRED_PIO_SELECT_0300_MSB_POSN = (int)7
};
typedef enum mildred_pio_select_0300_posn_enum mildred_pio_select_0300_posn;

#define MILDRED_PIO_SELECT_0300_LSB_MASK         (0x00000001u)
#define MILDRED_PIO_SELECT_0300_MSB_MASK         (0x00000080u)

enum mildred_pio_select_0704_posn_enum
{
   MILDRED_PIO_SELECT_0704_LSB_POSN                   = (int)0,
   MILDRED_PIO_SELECT_0704_MILDRED_PIO_SELECT_0704_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_0704_MSB_POSN                   = (int)7,
   MILDRED_PIO_SELECT_0704_MILDRED_PIO_SELECT_0704_MSB_POSN = (int)7
};
typedef enum mildred_pio_select_0704_posn_enum mildred_pio_select_0704_posn;

#define MILDRED_PIO_SELECT_0704_LSB_MASK         (0x00000001u)
#define MILDRED_PIO_SELECT_0704_MSB_MASK         (0x00000080u)

enum mildred_pio_select_1108_posn_enum
{
   MILDRED_PIO_SELECT_1108_LSB_POSN                   = (int)0,
   MILDRED_PIO_SELECT_1108_MILDRED_PIO_SELECT_1108_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_1108_MSB_POSN                   = (int)7,
   MILDRED_PIO_SELECT_1108_MILDRED_PIO_SELECT_1108_MSB_POSN = (int)7
};
typedef enum mildred_pio_select_1108_posn_enum mildred_pio_select_1108_posn;

#define MILDRED_PIO_SELECT_1108_LSB_MASK         (0x00000001u)
#define MILDRED_PIO_SELECT_1108_MSB_MASK         (0x00000080u)

enum mildred_pio_select_1512_posn_enum
{
   MILDRED_PIO_SELECT_1512_LSB_POSN                   = (int)0,
   MILDRED_PIO_SELECT_1512_MILDRED_PIO_SELECT_1512_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_1512_MSB_POSN                   = (int)7,
   MILDRED_PIO_SELECT_1512_MILDRED_PIO_SELECT_1512_MSB_POSN = (int)7
};
typedef enum mildred_pio_select_1512_posn_enum mildred_pio_select_1512_posn;

#define MILDRED_PIO_SELECT_1512_LSB_MASK         (0x00000001u)
#define MILDRED_PIO_SELECT_1512_MSB_MASK         (0x00000080u)

enum mildred_pio_select_1916_posn_enum
{
   MILDRED_PIO_SELECT_1916_LSB_POSN                   = (int)0,
   MILDRED_PIO_SELECT_1916_MILDRED_PIO_SELECT_1916_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_1916_MSB_POSN                   = (int)7,
   MILDRED_PIO_SELECT_1916_MILDRED_PIO_SELECT_1916_MSB_POSN = (int)7
};
typedef enum mildred_pio_select_1916_posn_enum mildred_pio_select_1916_posn;

#define MILDRED_PIO_SELECT_1916_LSB_MASK         (0x00000001u)
#define MILDRED_PIO_SELECT_1916_MSB_MASK         (0x00000080u)

enum mildred_pio_select_2320_posn_enum
{
   MILDRED_PIO_SELECT_2320_LSB_POSN                   = (int)0,
   MILDRED_PIO_SELECT_2320_MILDRED_PIO_SELECT_2320_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_2320_MSB_POSN                   = (int)7,
   MILDRED_PIO_SELECT_2320_MILDRED_PIO_SELECT_2320_MSB_POSN = (int)7
};
typedef enum mildred_pio_select_2320_posn_enum mildred_pio_select_2320_posn;

#define MILDRED_PIO_SELECT_2320_LSB_MASK         (0x00000001u)
#define MILDRED_PIO_SELECT_2320_MSB_MASK         (0x00000080u)

enum mildred_pio_select_2724_posn_enum
{
   MILDRED_PIO_SELECT_2724_LSB_POSN                   = (int)0,
   MILDRED_PIO_SELECT_2724_MILDRED_PIO_SELECT_2724_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_2724_MSB_POSN                   = (int)7,
   MILDRED_PIO_SELECT_2724_MILDRED_PIO_SELECT_2724_MSB_POSN = (int)7
};
typedef enum mildred_pio_select_2724_posn_enum mildred_pio_select_2724_posn;

#define MILDRED_PIO_SELECT_2724_LSB_MASK         (0x00000001u)
#define MILDRED_PIO_SELECT_2724_MSB_MASK         (0x00000080u)

enum mildred_pio_select_3128_posn_enum
{
   MILDRED_PIO_SELECT_3128_LSB_POSN                   = (int)0,
   MILDRED_PIO_SELECT_3128_MILDRED_PIO_SELECT_3128_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_3128_MSB_POSN                   = (int)7,
   MILDRED_PIO_SELECT_3128_MILDRED_PIO_SELECT_3128_MSB_POSN = (int)7
};
typedef enum mildred_pio_select_3128_posn_enum mildred_pio_select_3128_posn;

#define MILDRED_PIO_SELECT_3128_LSB_MASK         (0x00000001u)
#define MILDRED_PIO_SELECT_3128_MSB_MASK         (0x00000080u)

enum mildred_pio_select_tmrctr0_posn_enum
{
   MILDRED_PIO_SELECT_TMRCTR0_LSB_POSN                = (int)0,
   MILDRED_PIO_SELECT_TMRCTR0_MILDRED_PIO_SELECT_TMRCTR0_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_TMRCTR0_MSB_POSN                = (int)6,
   MILDRED_PIO_SELECT_TMRCTR0_MILDRED_PIO_SELECT_TMRCTR0_MSB_POSN = (int)6
};
typedef enum mildred_pio_select_tmrctr0_posn_enum mildred_pio_select_tmrctr0_posn;

#define MILDRED_PIO_SELECT_TMRCTR0_LSB_MASK      (0x00000001u)
#define MILDRED_PIO_SELECT_TMRCTR0_MSB_MASK      (0x00000040u)

enum mildred_pio_select_tmrctr1_posn_enum
{
   MILDRED_PIO_SELECT_TMRCTR1_LSB_POSN                = (int)0,
   MILDRED_PIO_SELECT_TMRCTR1_MILDRED_PIO_SELECT_TMRCTR1_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_TMRCTR1_MSB_POSN                = (int)6,
   MILDRED_PIO_SELECT_TMRCTR1_MILDRED_PIO_SELECT_TMRCTR1_MSB_POSN = (int)6
};
typedef enum mildred_pio_select_tmrctr1_posn_enum mildred_pio_select_tmrctr1_posn;

#define MILDRED_PIO_SELECT_TMRCTR1_LSB_MASK      (0x00000001u)
#define MILDRED_PIO_SELECT_TMRCTR1_MSB_MASK      (0x00000040u)

enum mildred_pio_select_tmrctr2_posn_enum
{
   MILDRED_PIO_SELECT_TMRCTR2_LSB_POSN                = (int)0,
   MILDRED_PIO_SELECT_TMRCTR2_MILDRED_PIO_SELECT_TMRCTR2_LSB_POSN = (int)0,
   MILDRED_PIO_SELECT_TMRCTR2_MSB_POSN                = (int)6,
   MILDRED_PIO_SELECT_TMRCTR2_MILDRED_PIO_SELECT_TMRCTR2_MSB_POSN = (int)6
};
typedef enum mildred_pio_select_tmrctr2_posn_enum mildred_pio_select_tmrctr2_posn;

#define MILDRED_PIO_SELECT_TMRCTR2_LSB_MASK      (0x00000001u)
#define MILDRED_PIO_SELECT_TMRCTR2_MSB_MASK      (0x00000040u)

enum mildred_timer_time_0700_posn_enum
{
   MILDRED_TIMER_TIME_0700_LSB_POSN                   = (int)0,
   MILDRED_TIMER_TIME_0700_MILDRED_TIMER_TIME_0700_LSB_POSN = (int)0,
   MILDRED_TIMER_TIME_0700_MSB_POSN                   = (int)7,
   MILDRED_TIMER_TIME_0700_MILDRED_TIMER_TIME_0700_MSB_POSN = (int)7
};
typedef enum mildred_timer_time_0700_posn_enum mildred_timer_time_0700_posn;

#define MILDRED_TIMER_TIME_0700_LSB_MASK         (0x00000001u)
#define MILDRED_TIMER_TIME_0700_MSB_MASK         (0x00000080u)

enum mildred_timer_time_1508_posn_enum
{
   MILDRED_TIMER_TIME_1508_LSB_POSN                   = (int)0,
   MILDRED_TIMER_TIME_1508_MILDRED_TIMER_TIME_1508_LSB_POSN = (int)0,
   MILDRED_TIMER_TIME_1508_MSB_POSN                   = (int)7,
   MILDRED_TIMER_TIME_1508_MILDRED_TIMER_TIME_1508_MSB_POSN = (int)7
};
typedef enum mildred_timer_time_1508_posn_enum mildred_timer_time_1508_posn;

#define MILDRED_TIMER_TIME_1508_LSB_MASK         (0x00000001u)
#define MILDRED_TIMER_TIME_1508_MSB_MASK         (0x00000080u)

enum mildred_timer_time_2316_posn_enum
{
   MILDRED_TIMER_TIME_2316_LSB_POSN                   = (int)0,
   MILDRED_TIMER_TIME_2316_MILDRED_TIMER_TIME_2316_LSB_POSN = (int)0,
   MILDRED_TIMER_TIME_2316_MSB_POSN                   = (int)7,
   MILDRED_TIMER_TIME_2316_MILDRED_TIMER_TIME_2316_MSB_POSN = (int)7
};
typedef enum mildred_timer_time_2316_posn_enum mildred_timer_time_2316_posn;

#define MILDRED_TIMER_TIME_2316_LSB_MASK         (0x00000001u)
#define MILDRED_TIMER_TIME_2316_MSB_MASK         (0x00000080u)

enum mildred_timer_time_3124_posn_enum
{
   MILDRED_TIMER_TIME_3124_LSB_POSN                   = (int)0,
   MILDRED_TIMER_TIME_3124_MILDRED_TIMER_TIME_3124_LSB_POSN = (int)0,
   MILDRED_TIMER_TIME_3124_MSB_POSN                   = (int)7,
   MILDRED_TIMER_TIME_3124_MILDRED_TIMER_TIME_3124_MSB_POSN = (int)7
};
typedef enum mildred_timer_time_3124_posn_enum mildred_timer_time_3124_posn;

#define MILDRED_TIMER_TIME_3124_LSB_MASK         (0x00000001u)
#define MILDRED_TIMER_TIME_3124_MSB_MASK         (0x00000080u)

enum pio_ctrl_imem_wablm_posn_enum
{
   PIO_CTRL_IMEM_WABLM_LSB_POSN                       = (int)0,
   PIO_CTRL_IMEM_WABLM_PIO_CTRL_IMEM_WABLM_LSB_POSN   = (int)0,
   PIO_CTRL_IMEM_WABLM_MSB_POSN                       = (int)1,
   PIO_CTRL_IMEM_WABLM_PIO_CTRL_IMEM_WABLM_MSB_POSN   = (int)1
};
typedef enum pio_ctrl_imem_wablm_posn_enum pio_ctrl_imem_wablm_posn;

#define PIO_CTRL_IMEM_WABLM_LSB_MASK             (0x00000001u)
#define PIO_CTRL_IMEM_WABLM_MSB_MASK             (0x00000002u)

enum pio_ctrl_janitor_mailbox_watchdog_clear_posn_enum
{
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CLEAR_POSN       = (int)0,
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CLEAR_PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CLEAR_LSB_POSN = (int)0,
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CLEAR_PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CLEAR_MSB_POSN = (int)0
};
typedef enum pio_ctrl_janitor_mailbox_watchdog_clear_posn_enum pio_ctrl_janitor_mailbox_watchdog_clear_posn;

#define PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CLEAR_MASK (0x00000001u)

enum pio_ctrl_janitor_mailbox_watchdog_code_posn_enum
{
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CODE_LSB_POSN    = (int)0,
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CODE_PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CODE_LSB_POSN = (int)0,
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CODE_MSB_POSN    = (int)7,
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CODE_PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CODE_MSB_POSN = (int)7
};
typedef enum pio_ctrl_janitor_mailbox_watchdog_code_posn_enum pio_ctrl_janitor_mailbox_watchdog_code_posn;

#define PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CODE_LSB_MASK (0x00000001u)
#define PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CODE_MSB_MASK (0x00000080u)

enum pio_ctrl_janitor_mailbox_watchdog_enable_posn_enum
{
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_ENABLE_POSN      = (int)0,
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_ENABLE_PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_ENABLE_LSB_POSN = (int)0,
   PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_ENABLE_PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_ENABLE_MSB_POSN = (int)0
};
typedef enum pio_ctrl_janitor_mailbox_watchdog_enable_posn_enum pio_ctrl_janitor_mailbox_watchdog_enable_posn;

#define PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_ENABLE_MASK (0x00000001u)

enum mildred_int_priorities_posn_enum
{
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_BUS_INT_LSB_POSN = (int)0,
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_BUS_INT_MSB_POSN = (int)0,
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_TIME_UPDATE_LSB_POSN = (int)1,
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_TIME_UPDATE_MSB_POSN = (int)1,
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_PIO_RISING_LSB_POSN = (int)2,
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_PIO_RISING_MSB_POSN = (int)2,
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_PIO_FALLING_LSB_POSN = (int)3,
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_PIO_FALLING_MSB_POSN = (int)3,
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_JANITOR_LSB_POSN = (int)4,
   MILDRED_INT_PRIORITIES_MILDRED_INT_SOURCE_JANITOR_MSB_POSN = (int)4
};
typedef enum mildred_int_priorities_posn_enum mildred_int_priorities_posn;


enum mildred_int_priorities2_posn_enum
{
   MILDRED_INT_PRIORITIES2_MILDRED_INT_SOURCE_DUMMY_LSB_POSN = (int)0,
   MILDRED_INT_PRIORITIES2_MILDRED_INT_SOURCE_DUMMY_MSB_POSN = (int)0
};
typedef enum mildred_int_priorities2_posn_enum mildred_int_priorities2_posn;


enum mildred_int_sources2_en_posn_enum
{
   MILDRED_INT_SOURCES2_EN_MILDRED_INT_SOURCE_DUMMY_LSB_POSN = (int)0,
   MILDRED_INT_SOURCES2_EN_MILDRED_INT_SOURCE_DUMMY_MSB_POSN = (int)0
};
typedef enum mildred_int_sources2_en_posn_enum mildred_int_sources2_en_posn;


enum mildred_int_sources_en_posn_enum
{
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_BUS_INT_LSB_POSN = (int)0,
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_BUS_INT_MSB_POSN = (int)0,
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_TIME_UPDATE_LSB_POSN = (int)1,
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_TIME_UPDATE_MSB_POSN = (int)1,
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_PIO_RISING_LSB_POSN = (int)2,
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_PIO_RISING_MSB_POSN = (int)2,
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_PIO_FALLING_LSB_POSN = (int)3,
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_PIO_FALLING_MSB_POSN = (int)3,
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_JANITOR_LSB_POSN = (int)4,
   MILDRED_INT_SOURCES_EN_MILDRED_INT_SOURCE_JANITOR_MSB_POSN = (int)4
};
typedef enum mildred_int_sources_en_posn_enum mildred_int_sources_en_posn;


enum mildred_int_status_posn_enum
{
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_BUS_INT_LSB_POSN = (int)0,
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_BUS_INT_MSB_POSN = (int)0,
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_TIME_UPDATE_LSB_POSN = (int)1,
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_TIME_UPDATE_MSB_POSN = (int)1,
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_PIO_RISING_LSB_POSN = (int)2,
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_PIO_RISING_MSB_POSN = (int)2,
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_PIO_FALLING_LSB_POSN = (int)3,
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_PIO_FALLING_MSB_POSN = (int)3,
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_JANITOR_LSB_POSN = (int)4,
   MILDRED_INT_STATUS_MILDRED_INT_SOURCE_JANITOR_MSB_POSN = (int)4
};
typedef enum mildred_int_status_posn_enum mildred_int_status_posn;


enum mildred_int_status2_posn_enum
{
   MILDRED_INT_STATUS2_MILDRED_INT_SOURCE_DUMMY_LSB_POSN = (int)0,
   MILDRED_INT_STATUS2_MILDRED_INT_SOURCE_DUMMY_MSB_POSN = (int)0
};
typedef enum mildred_int_status2_posn_enum mildred_int_status2_posn;


enum mildred_int_unmasked_status_posn_enum
{
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_BUS_INT_LSB_POSN = (int)0,
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_BUS_INT_MSB_POSN = (int)0,
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_TIME_UPDATE_LSB_POSN = (int)1,
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_TIME_UPDATE_MSB_POSN = (int)1,
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_PIO_RISING_LSB_POSN = (int)2,
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_PIO_RISING_MSB_POSN = (int)2,
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_PIO_FALLING_LSB_POSN = (int)3,
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_PIO_FALLING_MSB_POSN = (int)3,
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_JANITOR_LSB_POSN = (int)4,
   MILDRED_INT_UNMASKED_STATUS_MILDRED_INT_SOURCE_JANITOR_MSB_POSN = (int)4
};
typedef enum mildred_int_unmasked_status_posn_enum mildred_int_unmasked_status_posn;


enum mildred_int_unmasked_status2_posn_enum
{
   MILDRED_INT_UNMASKED_STATUS2_MILDRED_INT_SOURCE_DUMMY_LSB_POSN = (int)0,
   MILDRED_INT_UNMASKED_STATUS2_MILDRED_INT_SOURCE_DUMMY_MSB_POSN = (int)0
};
typedef enum mildred_int_unmasked_status2_posn_enum mildred_int_unmasked_status2_posn;


/* -- pio_ctrl_sys_pio -- PIO controller subsystem PIO control and status registers -- */

enum pioctrl_ram_ema_fields_posn_enum
{
   BT_EMAW_LSB_POSN                                   = (int)0,
   PIOCTRL_RAM_EMA_FIELDS_BT_EMAW_LSB_POSN            = (int)0,
   BT_EMAW_MSB_POSN                                   = (int)1,
   PIOCTRL_RAM_EMA_FIELDS_BT_EMAW_MSB_POSN            = (int)1,
   BT_EMA_LSB_POSN                                    = (int)2,
   PIOCTRL_RAM_EMA_FIELDS_BT_EMA_LSB_POSN             = (int)2,
   BT_EMA_MSB_POSN                                    = (int)4,
   PIOCTRL_RAM_EMA_FIELDS_BT_EMA_MSB_POSN             = (int)4
};
typedef enum pioctrl_ram_ema_fields_posn_enum pioctrl_ram_ema_fields_posn;

#define BT_EMAW_LSB_MASK                         (0x00000001u)
#define BT_EMAW_MSB_MASK                         (0x00000002u)
#define BT_EMA_LSB_MASK                          (0x00000004u)
#define BT_EMA_MSB_MASK                          (0x00000010u)

enum pio_ctrl_sys_pio_event_clear_posn_enum
{
   PIO_CTRL_SYS_PIO_EVENT_CLEAR_RISING_POSN           = (int)0,
   PIO_CTRL_SYS_PIO_EVENT_CLEAR_PIO_CTRL_SYS_PIO_EVENT_CLEAR_RISING_LSB_POSN = (int)0,
   PIO_CTRL_SYS_PIO_EVENT_CLEAR_PIO_CTRL_SYS_PIO_EVENT_CLEAR_RISING_MSB_POSN = (int)0,
   PIO_CTRL_SYS_PIO_EVENT_CLEAR_FALLING_POSN          = (int)1,
   PIO_CTRL_SYS_PIO_EVENT_CLEAR_PIO_CTRL_SYS_PIO_EVENT_CLEAR_FALLING_LSB_POSN = (int)1,
   PIO_CTRL_SYS_PIO_EVENT_CLEAR_PIO_CTRL_SYS_PIO_EVENT_CLEAR_FALLING_MSB_POSN = (int)1
};
typedef enum pio_ctrl_sys_pio_event_clear_posn_enum pio_ctrl_sys_pio_event_clear_posn;

#define PIO_CTRL_SYS_PIO_EVENT_CLEAR_RISING_MASK (0x00000001u)
#define PIO_CTRL_SYS_PIO_EVENT_CLEAR_FALLING_MASK (0x00000002u)

enum pio_sys_ram_seq_timing_reg0_posn_enum
{
   PIO_SYS_TValF3_LSB_POSN                            = (int)0,
   PIO_SYS_RAM_SEQ_TIMING_REG0_PIO_SYS_TValF3_LSB_POSN = (int)0,
   PIO_SYS_TValF3_MSB_POSN                            = (int)3,
   PIO_SYS_RAM_SEQ_TIMING_REG0_PIO_SYS_TValF3_MSB_POSN = (int)3,
   PIO_SYS_TValF1_LSB_POSN                            = (int)4,
   PIO_SYS_RAM_SEQ_TIMING_REG0_PIO_SYS_TValF1_LSB_POSN = (int)4,
   PIO_SYS_TValF1_MSB_POSN                            = (int)7,
   PIO_SYS_RAM_SEQ_TIMING_REG0_PIO_SYS_TValF1_MSB_POSN = (int)7,
   PIO_SYS_TValE1_LSB_POSN                            = (int)8,
   PIO_SYS_RAM_SEQ_TIMING_REG0_PIO_SYS_TValE1_LSB_POSN = (int)8,
   PIO_SYS_TValE1_MSB_POSN                            = (int)11,
   PIO_SYS_RAM_SEQ_TIMING_REG0_PIO_SYS_TValE1_MSB_POSN = (int)11,
   PIO_SYS_TValD1_LSB_POSN                            = (int)12,
   PIO_SYS_RAM_SEQ_TIMING_REG0_PIO_SYS_TValD1_LSB_POSN = (int)12,
   PIO_SYS_TValD1_MSB_POSN                            = (int)15,
   PIO_SYS_RAM_SEQ_TIMING_REG0_PIO_SYS_TValD1_MSB_POSN = (int)15
};
typedef enum pio_sys_ram_seq_timing_reg0_posn_enum pio_sys_ram_seq_timing_reg0_posn;

#define PIO_SYS_TValF3_LSB_MASK                  (0x00000001u)
#define PIO_SYS_TValF3_MSB_MASK                  (0x00000008u)
#define PIO_SYS_TValF1_LSB_MASK                  (0x00000010u)
#define PIO_SYS_TValF1_MSB_MASK                  (0x00000080u)
#define PIO_SYS_TValE1_LSB_MASK                  (0x00000100u)
#define PIO_SYS_TValE1_MSB_MASK                  (0x00000800u)
#define PIO_SYS_TValD1_LSB_MASK                  (0x00001000u)
#define PIO_SYS_TValD1_MSB_MASK                  (0x00008000u)

enum pio_sys_ram_seq_timing_reg1_posn_enum
{
   PIO_SYS_TValC1_LSB_POSN                            = (int)0,
   PIO_SYS_RAM_SEQ_TIMING_REG1_PIO_SYS_TValC1_LSB_POSN = (int)0,
   PIO_SYS_TValC1_MSB_POSN                            = (int)3,
   PIO_SYS_RAM_SEQ_TIMING_REG1_PIO_SYS_TValC1_MSB_POSN = (int)3,
   PIO_SYS_TValB1_LSB_POSN                            = (int)4,
   PIO_SYS_RAM_SEQ_TIMING_REG1_PIO_SYS_TValB1_LSB_POSN = (int)4,
   PIO_SYS_TValB1_MSB_POSN                            = (int)7,
   PIO_SYS_RAM_SEQ_TIMING_REG1_PIO_SYS_TValB1_MSB_POSN = (int)7,
   PIO_SYS_TValA1_LSB_POSN                            = (int)8,
   PIO_SYS_RAM_SEQ_TIMING_REG1_PIO_SYS_TValA1_LSB_POSN = (int)8,
   PIO_SYS_TValA1_MSB_POSN                            = (int)11,
   PIO_SYS_RAM_SEQ_TIMING_REG1_PIO_SYS_TValA1_MSB_POSN = (int)11,
   PIO_SYS_TValD_LSB_POSN                             = (int)12,
   PIO_SYS_RAM_SEQ_TIMING_REG1_PIO_SYS_TValD_LSB_POSN = (int)12,
   PIO_SYS_TValD_MSB_POSN                             = (int)15,
   PIO_SYS_RAM_SEQ_TIMING_REG1_PIO_SYS_TValD_MSB_POSN = (int)15
};
typedef enum pio_sys_ram_seq_timing_reg1_posn_enum pio_sys_ram_seq_timing_reg1_posn;

#define PIO_SYS_TValC1_LSB_MASK                  (0x00000001u)
#define PIO_SYS_TValC1_MSB_MASK                  (0x00000008u)
#define PIO_SYS_TValB1_LSB_MASK                  (0x00000010u)
#define PIO_SYS_TValB1_MSB_MASK                  (0x00000080u)
#define PIO_SYS_TValA1_LSB_MASK                  (0x00000100u)
#define PIO_SYS_TValA1_MSB_MASK                  (0x00000800u)
#define PIO_SYS_TValD_LSB_MASK                   (0x00001000u)
#define PIO_SYS_TValD_MSB_MASK                   (0x00008000u)

enum pio_sys_ram_seq_timing_reg2_posn_enum
{
   PIO_SYS_TValC_LSB_POSN                             = (int)0,
   PIO_SYS_RAM_SEQ_TIMING_REG2_PIO_SYS_TValC_LSB_POSN = (int)0,
   PIO_SYS_TValC_MSB_POSN                             = (int)3,
   PIO_SYS_RAM_SEQ_TIMING_REG2_PIO_SYS_TValC_MSB_POSN = (int)3,
   PIO_SYS_TValB_LSB_POSN                             = (int)4,
   PIO_SYS_RAM_SEQ_TIMING_REG2_PIO_SYS_TValB_LSB_POSN = (int)4,
   PIO_SYS_TValB_MSB_POSN                             = (int)7,
   PIO_SYS_RAM_SEQ_TIMING_REG2_PIO_SYS_TValB_MSB_POSN = (int)7,
   PIO_SYS_TValA_LSB_POSN                             = (int)8,
   PIO_SYS_RAM_SEQ_TIMING_REG2_PIO_SYS_TValA_LSB_POSN = (int)8,
   PIO_SYS_TValA_MSB_POSN                             = (int)11,
   PIO_SYS_RAM_SEQ_TIMING_REG2_PIO_SYS_TValA_MSB_POSN = (int)11
};
typedef enum pio_sys_ram_seq_timing_reg2_posn_enum pio_sys_ram_seq_timing_reg2_posn;

#define PIO_SYS_TValC_LSB_MASK                   (0x00000001u)
#define PIO_SYS_TValC_MSB_MASK                   (0x00000008u)
#define PIO_SYS_TValB_LSB_MASK                   (0x00000010u)
#define PIO_SYS_TValB_MSB_MASK                   (0x00000080u)
#define PIO_SYS_TValA_LSB_MASK                   (0x00000100u)
#define PIO_SYS_TValA_MSB_MASK                   (0x00000800u)

enum pioctrl_ram0_ema_posn_enum
{
   PIOCTRL_RAM0_EMA_BT_EMAW_LSB_POSN                  = (int)0,
   PIOCTRL_RAM0_EMA_BT_EMAW_MSB_POSN                  = (int)1,
   PIOCTRL_RAM0_EMA_BT_EMA_LSB_POSN                   = (int)2,
   PIOCTRL_RAM0_EMA_BT_EMA_MSB_POSN                   = (int)4
};
typedef enum pioctrl_ram0_ema_posn_enum pioctrl_ram0_ema_posn;


/* -- sub_sys_common -- Common subsystem level registers -- */

enum chip_version_posn_enum
{
   CHIP_VERSION_MAJOR_LSB_POSN                        = (int)0,
   CHIP_VERSION_CHIP_VERSION_MAJOR_LSB_POSN           = (int)0,
   CHIP_VERSION_MAJOR_MSB_POSN                        = (int)7,
   CHIP_VERSION_CHIP_VERSION_MAJOR_MSB_POSN           = (int)7,
   CHIP_VERSION_VARIANT_LSB_POSN                      = (int)8,
   CHIP_VERSION_CHIP_VERSION_VARIANT_LSB_POSN         = (int)8,
   CHIP_VERSION_VARIANT_MSB_POSN                      = (int)11,
   CHIP_VERSION_CHIP_VERSION_VARIANT_MSB_POSN         = (int)11,
   CHIP_VERSION_MINOR_LSB_POSN                        = (int)12,
   CHIP_VERSION_CHIP_VERSION_MINOR_LSB_POSN           = (int)12,
   CHIP_VERSION_MINOR_MSB_POSN                        = (int)15,
   CHIP_VERSION_CHIP_VERSION_MINOR_MSB_POSN           = (int)15
};
typedef enum chip_version_posn_enum chip_version_posn;

#define CHIP_VERSION_MAJOR_LSB_MASK              (0x00000001u)
#define CHIP_VERSION_MAJOR_MSB_MASK              (0x00000080u)
#define CHIP_VERSION_VARIANT_LSB_MASK            (0x00000100u)
#define CHIP_VERSION_VARIANT_MSB_MASK            (0x00000800u)
#define CHIP_VERSION_MINOR_LSB_MASK              (0x00001000u)
#define CHIP_VERSION_MINOR_MSB_MASK              (0x00008000u)

enum sub_sys_debug_select_posn_enum
{
   SUB_SYS_DEBUG_SELECT_LO_LSB_POSN                   = (int)0,
   SUB_SYS_DEBUG_SELECT_SUB_SYS_DEBUG_SELECT_LO_LSB_POSN = (int)0,
   SUB_SYS_DEBUG_SELECT_LO_MSB_POSN                   = (int)7,
   SUB_SYS_DEBUG_SELECT_SUB_SYS_DEBUG_SELECT_LO_MSB_POSN = (int)7,
   SUB_SYS_DEBUG_SELECT_HI_LSB_POSN                   = (int)8,
   SUB_SYS_DEBUG_SELECT_SUB_SYS_DEBUG_SELECT_HI_LSB_POSN = (int)8,
   SUB_SYS_DEBUG_SELECT_HI_MSB_POSN                   = (int)15,
   SUB_SYS_DEBUG_SELECT_SUB_SYS_DEBUG_SELECT_HI_MSB_POSN = (int)15
};
typedef enum sub_sys_debug_select_posn_enum sub_sys_debug_select_posn;

#define SUB_SYS_DEBUG_SELECT_LO_LSB_MASK         (0x00000001u)
#define SUB_SYS_DEBUG_SELECT_LO_MSB_MASK         (0x00000080u)
#define SUB_SYS_DEBUG_SELECT_HI_LSB_MASK         (0x00000100u)
#define SUB_SYS_DEBUG_SELECT_HI_MSB_MASK         (0x00008000u)

enum sub_sys_misc_enables_posn_enum
{
   SUB_SYS_MISC_SPI_STOP_OUT_EN_POSN                  = (int)0,
   SUB_SYS_MISC_ENABLES_SUB_SYS_MISC_SPI_STOP_OUT_EN_LSB_POSN = (int)0,
   SUB_SYS_MISC_ENABLES_SUB_SYS_MISC_SPI_STOP_OUT_EN_MSB_POSN = (int)0,
   SUB_SYS_MISC_MMU_INIT_DONE_POSN                    = (int)1,
   SUB_SYS_MISC_ENABLES_SUB_SYS_MISC_MMU_INIT_DONE_LSB_POSN = (int)1,
   SUB_SYS_MISC_ENABLES_SUB_SYS_MISC_MMU_INIT_DONE_MSB_POSN = (int)1
};
typedef enum sub_sys_misc_enables_posn_enum sub_sys_misc_enables_posn;

#define SUB_SYS_MISC_SPI_STOP_OUT_EN_MASK        (0x00000001u)
#define SUB_SYS_MISC_MMU_INIT_DONE_MASK          (0x00000002u)

enum sub_sys_rst_status_enum
{
   SUB_SYS_RST_STATUS_RESET_TYPE_POWER_ON   = (int)0x0,
   SUB_SYS_RST_STATUS_RESET_TYPE_P0_WATCHDOG = (int)0x1,
   SUB_SYS_RST_STATUS_RESET_TYPE_P1_WATCHDOG = (int)0x2,
   SUB_SYS_RST_STATUS_RESET_TYPE_DBG        = (int)0x4,
   MAX_SUB_SYS_RST_STATUS                   = (int)0x4
};
typedef enum sub_sys_rst_status_enum sub_sys_rst_status;


enum sub_sys_id_posn_enum
{
   SUB_SYS_ID_LSB_POSN                                = (int)0,
   SUB_SYS_ID_SUB_SYS_ID_LSB_POSN                     = (int)0,
   SUB_SYS_ID_MSB_POSN                                = (int)3,
   SUB_SYS_ID_SUB_SYS_ID_MSB_POSN                     = (int)3
};
typedef enum sub_sys_id_posn_enum sub_sys_id_posn;

#define SUB_SYS_ID_LSB_MASK                      (0x00000001u)
#define SUB_SYS_ID_MSB_MASK                      (0x00000008u)

enum sub_sys_legacy_chip_version_posn_enum
{
   SUB_SYS_LEGACY_CHIP_VERSION_LSB_POSN               = (int)0,
   SUB_SYS_LEGACY_CHIP_VERSION_SUB_SYS_LEGACY_CHIP_VERSION_LSB_POSN = (int)0,
   SUB_SYS_LEGACY_CHIP_VERSION_MSB_POSN               = (int)15,
   SUB_SYS_LEGACY_CHIP_VERSION_SUB_SYS_LEGACY_CHIP_VERSION_MSB_POSN = (int)15
};
typedef enum sub_sys_legacy_chip_version_posn_enum sub_sys_legacy_chip_version_posn;

#define SUB_SYS_LEGACY_CHIP_VERSION_LSB_MASK     (0x00000001u)
#define SUB_SYS_LEGACY_CHIP_VERSION_MSB_MASK     (0x00008000u)

enum sub_sys_mailbox0_posn_enum
{
   SUB_SYS_MAILBOX0_LSB_POSN                          = (int)0,
   SUB_SYS_MAILBOX0_SUB_SYS_MAILBOX0_LSB_POSN         = (int)0,
   SUB_SYS_MAILBOX0_MSB_POSN                          = (int)15,
   SUB_SYS_MAILBOX0_SUB_SYS_MAILBOX0_MSB_POSN         = (int)15
};
typedef enum sub_sys_mailbox0_posn_enum sub_sys_mailbox0_posn;

#define SUB_SYS_MAILBOX0_LSB_MASK                (0x00000001u)
#define SUB_SYS_MAILBOX0_MSB_MASK                (0x00008000u)

enum sub_sys_mailbox1_posn_enum
{
   SUB_SYS_MAILBOX1_LSB_POSN                          = (int)0,
   SUB_SYS_MAILBOX1_SUB_SYS_MAILBOX1_LSB_POSN         = (int)0,
   SUB_SYS_MAILBOX1_MSB_POSN                          = (int)15,
   SUB_SYS_MAILBOX1_SUB_SYS_MAILBOX1_MSB_POSN         = (int)15
};
typedef enum sub_sys_mailbox1_posn_enum sub_sys_mailbox1_posn;

#define SUB_SYS_MAILBOX1_LSB_MASK                (0x00000001u)
#define SUB_SYS_MAILBOX1_MSB_MASK                (0x00008000u)

enum sub_sys_mailbox2_posn_enum
{
   SUB_SYS_MAILBOX2_LSB_POSN                          = (int)0,
   SUB_SYS_MAILBOX2_SUB_SYS_MAILBOX2_LSB_POSN         = (int)0,
   SUB_SYS_MAILBOX2_MSB_POSN                          = (int)15,
   SUB_SYS_MAILBOX2_SUB_SYS_MAILBOX2_MSB_POSN         = (int)15
};
typedef enum sub_sys_mailbox2_posn_enum sub_sys_mailbox2_posn;

#define SUB_SYS_MAILBOX2_LSB_MASK                (0x00000001u)
#define SUB_SYS_MAILBOX2_MSB_MASK                (0x00008000u)

enum sub_sys_mailbox3_posn_enum
{
   SUB_SYS_MAILBOX3_LSB_POSN                          = (int)0,
   SUB_SYS_MAILBOX3_SUB_SYS_MAILBOX3_LSB_POSN         = (int)0,
   SUB_SYS_MAILBOX3_MSB_POSN                          = (int)15,
   SUB_SYS_MAILBOX3_SUB_SYS_MAILBOX3_MSB_POSN         = (int)15
};
typedef enum sub_sys_mailbox3_posn_enum sub_sys_mailbox3_posn;

#define SUB_SYS_MAILBOX3_LSB_MASK                (0x00000001u)
#define SUB_SYS_MAILBOX3_MSB_MASK                (0x00008000u)

enum sub_sys_p0_interrupt1_posn_enum
{
   SUB_SYS_P0_INTERRUPT1_LSB_POSN                     = (int)0,
   SUB_SYS_P0_INTERRUPT1_SUB_SYS_P0_INTERRUPT1_LSB_POSN = (int)0,
   SUB_SYS_P0_INTERRUPT1_MSB_POSN                     = (int)15,
   SUB_SYS_P0_INTERRUPT1_SUB_SYS_P0_INTERRUPT1_MSB_POSN = (int)15
};
typedef enum sub_sys_p0_interrupt1_posn_enum sub_sys_p0_interrupt1_posn;

#define SUB_SYS_P0_INTERRUPT1_LSB_MASK           (0x00000001u)
#define SUB_SYS_P0_INTERRUPT1_MSB_MASK           (0x00008000u)

enum sub_sys_p0_interrupt2_posn_enum
{
   SUB_SYS_P0_INTERRUPT2_LSB_POSN                     = (int)0,
   SUB_SYS_P0_INTERRUPT2_SUB_SYS_P0_INTERRUPT2_LSB_POSN = (int)0,
   SUB_SYS_P0_INTERRUPT2_MSB_POSN                     = (int)15,
   SUB_SYS_P0_INTERRUPT2_SUB_SYS_P0_INTERRUPT2_MSB_POSN = (int)15
};
typedef enum sub_sys_p0_interrupt2_posn_enum sub_sys_p0_interrupt2_posn;

#define SUB_SYS_P0_INTERRUPT2_LSB_MASK           (0x00000001u)
#define SUB_SYS_P0_INTERRUPT2_MSB_MASK           (0x00008000u)

enum sub_sys_reg_src_hash_posn_enum
{
   SUB_SYS_REG_SRC_HASH_LSB_POSN                      = (int)0,
   SUB_SYS_REG_SRC_HASH_SUB_SYS_REG_SRC_HASH_LSB_POSN = (int)0,
   SUB_SYS_REG_SRC_HASH_MSB_POSN                      = (int)15,
   SUB_SYS_REG_SRC_HASH_SUB_SYS_REG_SRC_HASH_MSB_POSN = (int)15
};
typedef enum sub_sys_reg_src_hash_posn_enum sub_sys_reg_src_hash_posn;

#define SUB_SYS_REG_SRC_HASH_LSB_MASK            (0x00000001u)
#define SUB_SYS_REG_SRC_HASH_MSB_MASK            (0x00008000u)

enum sub_sys_transaction_mux_dbg_sel_posn_enum
{
   SUB_SYS_TRANSACTION_MUX_DBG_SEL_LSB_POSN           = (int)0,
   SUB_SYS_TRANSACTION_MUX_DBG_SEL_SUB_SYS_TRANSACTION_MUX_DBG_SEL_LSB_POSN = (int)0,
   SUB_SYS_TRANSACTION_MUX_DBG_SEL_MSB_POSN           = (int)5,
   SUB_SYS_TRANSACTION_MUX_DBG_SEL_SUB_SYS_TRANSACTION_MUX_DBG_SEL_MSB_POSN = (int)5
};
typedef enum sub_sys_transaction_mux_dbg_sel_posn_enum sub_sys_transaction_mux_dbg_sel_posn;

#define SUB_SYS_TRANSACTION_MUX_DBG_SEL_LSB_MASK (0x00000001u)
#define SUB_SYS_TRANSACTION_MUX_DBG_SEL_MSB_MASK (0x00000020u)

enum sub_sys_common__pio_ctrl_sys_debug_source_enum
{
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_BUS_TRANSACTION_MUX = (int)0x0,
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_BUS_INTERRUPT = (int)0x1,
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_BUS_MISC = (int)0x2,
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_BUS_REMOTE_DATA = (int)0x3,
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_BUS_REMOTE_DEBUG = (int)0x4,
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_CLKGEN = (int)0x5,
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_IMEM_ARBITER = (int)0x6,
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_PMEM_ARBITER = (int)0x7,
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_REGS_ARBITER = (int)0x8,
   SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE_DEBUG_REGS_ARBITER = (int)0x9,
   MAX_SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE = (int)0x9
};
#define NUM_SUB_SYS_COMMON__PIO_CTRL_SYS_DEBUG_SOURCE (0xA)
typedef enum sub_sys_common__pio_ctrl_sys_debug_source_enum sub_sys_common__pio_ctrl_sys_debug_source;


enum sub_sys_chip_version_posn_enum
{
   SUB_SYS_DEBUGGER_REF_ID_CHIP_VERSION_MAJOR_LSB_POSN   = (int)0,
   SUB_SYS_DEBUGGER_REF_ID_CHIP_VERSION_MAJOR_MSB_POSN   = (int)7,
   SUB_SYS_DEBUGGER_REF_ID_CHIP_VERSION_VARIANT_LSB_POSN = (int)8,
   SUB_SYS_DEBUGGER_REF_ID_CHIP_VERSION_VARIANT_MSB_POSN = (int)11,
   SUB_SYS_DEBUGGER_REF_ID_CHIP_VERSION_MINOR_LSB_POSN   = (int)12,
   SUB_SYS_DEBUGGER_REF_ID_CHIP_VERSION_MINOR_MSB_POSN   = (int)15
};
typedef enum sub_sys_chip_version_posn_enum sub_sys_chip_version_posn;


/* -- pio_ctrl_sys_debug -- PIO controller subsystem debug registers -- */

enum pio_ctrl_debug_posn_enum
{
   PIO_CTRL_DEBUG_STOP_POSN                           = (int)0,
   PIO_CTRL_DEBUG_PIO_CTRL_DEBUG_STOP_LSB_POSN        = (int)0,
   PIO_CTRL_DEBUG_PIO_CTRL_DEBUG_STOP_MSB_POSN        = (int)0,
   PIO_CTRL_DEBUG_STEP_POSN                           = (int)1,
   PIO_CTRL_DEBUG_PIO_CTRL_DEBUG_STEP_LSB_POSN        = (int)1,
   PIO_CTRL_DEBUG_PIO_CTRL_DEBUG_STEP_MSB_POSN        = (int)1,
   PIO_CTRL_DEBUG_BREAK_IN_EN_POSN                    = (int)2,
   PIO_CTRL_DEBUG_PIO_CTRL_DEBUG_BREAK_IN_EN_LSB_POSN = (int)2,
   PIO_CTRL_DEBUG_PIO_CTRL_DEBUG_BREAK_IN_EN_MSB_POSN = (int)2,
   PIO_CTRL_DEBUG_BREAK_OUT_EN_POSN                   = (int)3,
   PIO_CTRL_DEBUG_PIO_CTRL_DEBUG_BREAK_OUT_EN_LSB_POSN = (int)3,
   PIO_CTRL_DEBUG_PIO_CTRL_DEBUG_BREAK_OUT_EN_MSB_POSN = (int)3
};
typedef enum pio_ctrl_debug_posn_enum pio_ctrl_debug_posn;

#define PIO_CTRL_DEBUG_STOP_MASK                 (0x00000001u)
#define PIO_CTRL_DEBUG_STEP_MASK                 (0x00000002u)
#define PIO_CTRL_DEBUG_BREAK_IN_EN_MASK          (0x00000004u)
#define PIO_CTRL_DEBUG_BREAK_OUT_EN_MASK         (0x00000008u)

enum pio_ctrl_debug_state_enum
{
   PIO_CTRL_DEBUG_STATE_RUN                 = (int)0x0,
   PIO_CTRL_DEBUG_STATE_STEP                = (int)0x1,
   PIO_CTRL_DEBUG_STATE_BREAK               = (int)0x2,
   PIO_CTRL_DEBUG_STATE_EXTERNAL_BREAK      = (int)0x3,
   PIO_CTRL_DEBUG_STATE_STOP                = (int)0x4,
   MAX_PIO_CTRL_DEBUG_STATE                 = (int)0x4
};
#define NUM_PIO_CTRL_DEBUG_STATE (0x5)
typedef enum pio_ctrl_debug_state_enum pio_ctrl_debug_state;


enum pio_ctrl_debug_break_addr0_posn_enum
{
   PIO_CTRL_DEBUG_BREAK_ADDR0_LSB_POSN                = (int)0,
   PIO_CTRL_DEBUG_BREAK_ADDR0_PIO_CTRL_DEBUG_BREAK_ADDR0_LSB_POSN = (int)0,
   PIO_CTRL_DEBUG_BREAK_ADDR0_MSB_POSN                = (int)15,
   PIO_CTRL_DEBUG_BREAK_ADDR0_PIO_CTRL_DEBUG_BREAK_ADDR0_MSB_POSN = (int)15
};
typedef enum pio_ctrl_debug_break_addr0_posn_enum pio_ctrl_debug_break_addr0_posn;

#define PIO_CTRL_DEBUG_BREAK_ADDR0_LSB_MASK      (0x00000001u)
#define PIO_CTRL_DEBUG_BREAK_ADDR0_MSB_MASK      (0x00008000u)

enum pio_ctrl_debug_break_addr1_posn_enum
{
   PIO_CTRL_DEBUG_BREAK_ADDR1_LSB_POSN                = (int)0,
   PIO_CTRL_DEBUG_BREAK_ADDR1_PIO_CTRL_DEBUG_BREAK_ADDR1_LSB_POSN = (int)0,
   PIO_CTRL_DEBUG_BREAK_ADDR1_MSB_POSN                = (int)15,
   PIO_CTRL_DEBUG_BREAK_ADDR1_PIO_CTRL_DEBUG_BREAK_ADDR1_MSB_POSN = (int)15
};
typedef enum pio_ctrl_debug_break_addr1_posn_enum pio_ctrl_debug_break_addr1_posn;

#define PIO_CTRL_DEBUG_BREAK_ADDR1_LSB_MASK      (0x00000001u)
#define PIO_CTRL_DEBUG_BREAK_ADDR1_MSB_MASK      (0x00008000u)

enum pio_ctrl_debug_break_addr2_posn_enum
{
   PIO_CTRL_DEBUG_BREAK_ADDR2_LSB_POSN                = (int)0,
   PIO_CTRL_DEBUG_BREAK_ADDR2_PIO_CTRL_DEBUG_BREAK_ADDR2_LSB_POSN = (int)0,
   PIO_CTRL_DEBUG_BREAK_ADDR2_MSB_POSN                = (int)15,
   PIO_CTRL_DEBUG_BREAK_ADDR2_PIO_CTRL_DEBUG_BREAK_ADDR2_MSB_POSN = (int)15
};
typedef enum pio_ctrl_debug_break_addr2_posn_enum pio_ctrl_debug_break_addr2_posn;

#define PIO_CTRL_DEBUG_BREAK_ADDR2_LSB_MASK      (0x00000001u)
#define PIO_CTRL_DEBUG_BREAK_ADDR2_MSB_MASK      (0x00008000u)

enum pio_ctrl_debug_break_en_posn_enum
{
   PIO_CTRL_DEBUG_BREAK_EN_LSB_POSN                   = (int)0,
   PIO_CTRL_DEBUG_BREAK_EN_PIO_CTRL_DEBUG_BREAK_EN_LSB_POSN = (int)0,
   PIO_CTRL_DEBUG_BREAK_EN_MSB_POSN                   = (int)2,
   PIO_CTRL_DEBUG_BREAK_EN_PIO_CTRL_DEBUG_BREAK_EN_MSB_POSN = (int)2
};
typedef enum pio_ctrl_debug_break_en_posn_enum pio_ctrl_debug_break_en_posn;

#define PIO_CTRL_DEBUG_BREAK_EN_LSB_MASK         (0x00000001u)
#define PIO_CTRL_DEBUG_BREAK_EN_MSB_MASK         (0x00000004u)

enum pio_ctrl_debug_pc_posn_enum
{
   PIO_CTRL_DEBUG_PC_LSB_POSN                         = (int)0,
   PIO_CTRL_DEBUG_PC_PIO_CTRL_DEBUG_PC_LSB_POSN       = (int)0,
   PIO_CTRL_DEBUG_PC_MSB_POSN                         = (int)15,
   PIO_CTRL_DEBUG_PC_PIO_CTRL_DEBUG_PC_MSB_POSN       = (int)15
};
typedef enum pio_ctrl_debug_pc_posn_enum pio_ctrl_debug_pc_posn;

#define PIO_CTRL_DEBUG_PC_LSB_MASK               (0x00000001u)
#define PIO_CTRL_DEBUG_PC_MSB_MASK               (0x00008000u)

enum pio_ctrl_debug_stopped_posn_enum
{
   PIO_CTRL_DEBUG_STOPPED_POSN                        = (int)0,
   PIO_CTRL_DEBUG_STOPPED_PIO_CTRL_DEBUG_STOPPED_LSB_POSN = (int)0,
   PIO_CTRL_DEBUG_STOPPED_PIO_CTRL_DEBUG_STOPPED_MSB_POSN = (int)0
};
typedef enum pio_ctrl_debug_stopped_posn_enum pio_ctrl_debug_stopped_posn;

#define PIO_CTRL_DEBUG_STOPPED_MASK              (0x00000001u)

enum pio_ctrl_debug_stop_count_posn_enum
{
   PIO_CTRL_DEBUG_STOP_COUNT_LSB_POSN                 = (int)0,
   PIO_CTRL_DEBUG_STOP_COUNT_PIO_CTRL_DEBUG_STOP_COUNT_LSB_POSN = (int)0,
   PIO_CTRL_DEBUG_STOP_COUNT_MSB_POSN                 = (int)7,
   PIO_CTRL_DEBUG_STOP_COUNT_PIO_CTRL_DEBUG_STOP_COUNT_MSB_POSN = (int)7
};
typedef enum pio_ctrl_debug_stop_count_posn_enum pio_ctrl_debug_stop_count_posn;

#define PIO_CTRL_DEBUG_STOP_COUNT_LSB_MASK       (0x00000001u)
#define PIO_CTRL_DEBUG_STOP_COUNT_MSB_MASK       (0x00000080u)

/* -- chip -- Auto-generated from chip manifest. -- */

enum deep_sleep_wakeup_posn_enum
{
   DEEP_SLEEP_WAKEUP_TIMER_POSN                       = (int)0,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_TIMER_LSB_POSN = (int)0,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_TIMER_MSB_POSN = (int)0,
   DEEP_SLEEP_WAKEUP_XTAL_POSN                        = (int)1,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_XTAL_LSB_POSN  = (int)1,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_XTAL_MSB_POSN  = (int)1,
   DEEP_SLEEP_WAKEUP_SPIB_POSN                        = (int)2,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_SPIB_LSB_POSN  = (int)2,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_SPIB_MSB_POSN  = (int)2,
   DEEP_SLEEP_WAKEUP_PIO_POSN                         = (int)3,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_PIO_LSB_POSN   = (int)3,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_PIO_MSB_POSN   = (int)3,
   DEEP_SLEEP_WAKEUP_TBRIDGE2_POSN                    = (int)11,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_TBRIDGE2_LSB_POSN = (int)11,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_TBRIDGE2_MSB_POSN = (int)11,
   DEEP_SLEEP_WAKEUP_PMU_TS_POSN                      = (int)12,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_PMU_TS_LSB_POSN = (int)12,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_PMU_TS_MSB_POSN = (int)12,
   DEEP_SLEEP_WAKEUP_USB_POSN                         = (int)8,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_USB_LSB_POSN   = (int)8,
   DEEP_SLEEP_WAKEUP_DEEP_SLEEP_WAKEUP_USB_MSB_POSN   = (int)8
};
typedef enum deep_sleep_wakeup_posn_enum deep_sleep_wakeup_posn;

#define DEEP_SLEEP_WAKEUP_TIMER_MASK             (0x00000001u)
#define DEEP_SLEEP_WAKEUP_XTAL_MASK              (0x00000002u)
#define DEEP_SLEEP_WAKEUP_SPIB_MASK              (0x00000004u)
#define DEEP_SLEEP_WAKEUP_PIO_MASK               (0x00000008u)
#define DEEP_SLEEP_WAKEUP_TBRIDGE2_MASK          (0x00000800u)
#define DEEP_SLEEP_WAKEUP_PMU_TS_MASK            (0x00001000u)
#define DEEP_SLEEP_WAKEUP_USB_MASK               (0x00000100u)

enum deep_sleep_wakeup_enum
{
   DEEP_SLEEP_WAKEUP_TOTAL_NUM_SOURCES      = (int)0xD,
   MAX_DEEP_SLEEP_WAKEUP                    = (int)0xD
};
typedef enum deep_sleep_wakeup_enum deep_sleep_wakeup;


enum scan_config_enum
{
   ATPG_NUM_SHIFT_CLK                       = (int)0x4,
   ATPG_NUM_COMPRESSOR_CHANNEL              = (int)0x4,
   ATPG_NUM_DECOMPRESSOR_CHANNEL            = (int)0x258,
   ATPG_NUM_STUCKATONLY_CHANNEL             = (int)0x0,
   MAX_SCAN_CONFIG                          = (int)0x258
};
typedef enum scan_config_enum scan_config;


enum system_bus_posn_enum
{
   SYSTEM_BUS_HOST_SYS_POSN                           = (int)1,
   SYSTEM_BUS_SYSTEM_BUS_HOST_SYS_LSB_POSN            = (int)1,
   SYSTEM_BUS_SYSTEM_BUS_HOST_SYS_MSB_POSN            = (int)1,
   SYSTEM_BUS_BT_SYS_POSN                             = (int)2,
   SYSTEM_BUS_SYSTEM_BUS_BT_SYS_LSB_POSN              = (int)2,
   SYSTEM_BUS_SYSTEM_BUS_BT_SYS_MSB_POSN              = (int)2,
   SYSTEM_BUS_APPS_SYS_POSN                           = (int)4,
   SYSTEM_BUS_SYSTEM_BUS_APPS_SYS_LSB_POSN            = (int)4,
   SYSTEM_BUS_SYSTEM_BUS_APPS_SYS_MSB_POSN            = (int)4,
   SYSTEM_BUS_CURATOR_POSN                            = (int)0,
   SYSTEM_BUS_SYSTEM_BUS_CURATOR_LSB_POSN             = (int)0,
   SYSTEM_BUS_SYSTEM_BUS_CURATOR_MSB_POSN             = (int)0,
   SYSTEM_BUS_PIO_CTRL_SYS_POSN                       = (int)5,
   SYSTEM_BUS_SYSTEM_BUS_PIO_CTRL_SYS_LSB_POSN        = (int)5,
   SYSTEM_BUS_SYSTEM_BUS_PIO_CTRL_SYS_MSB_POSN        = (int)5,
   SYSTEM_BUS_AUDIO_SYS_POSN                          = (int)3,
   SYSTEM_BUS_SYSTEM_BUS_AUDIO_SYS_LSB_POSN           = (int)3,
   SYSTEM_BUS_SYSTEM_BUS_AUDIO_SYS_MSB_POSN           = (int)3
};
typedef enum system_bus_posn_enum system_bus_posn;

#define SYSTEM_BUS_HOST_SYS_MASK                 (0x00000002u)
#define SYSTEM_BUS_BT_SYS_MASK                   (0x00000004u)
#define SYSTEM_BUS_APPS_SYS_MASK                 (0x00000010u)
#define SYSTEM_BUS_CURATOR_MASK                  (0x00000001u)
#define SYSTEM_BUS_PIO_CTRL_SYS_MASK             (0x00000020u)
#define SYSTEM_BUS_AUDIO_SYS_MASK                (0x00000008u)

enum system_bus_enum
{
   SYSTEM_BUS_HOST_SYS                      = (int)0x1,
   SYSTEM_BUS_BT_SYS                        = (int)0x2,
   SYSTEM_BUS_APPS_SYS                      = (int)0x4,
   SYSTEM_BUS_CURATOR                       = (int)0x0,
   SYSTEM_BUS_PIO_CTRL_SYS                  = (int)0x5,
   SYSTEM_BUS_AUDIO_SYS                     = (int)0x3,
   SYSTEM_BUS_NUM_SUBSYSTEMS                = (int)0x6,
   SYSTEM_BUS_RANGE_LSB                     = (int)0x0,
   SYSTEM_BUS_RANGE_MSB                     = (int)0x5,
   MAX_SYSTEM_BUS                           = (int)0x6
};
typedef enum system_bus_enum system_bus;


/* -- watchdog -- Generic Watchdog registers -- */

enum watchdog_disable_enum
{
   WATCHDOG_DISABLE_CODE1                   = (int)0x6734,
   WATCHDOG_DISABLE_CODE2                   = (int)0xD6BF,
   WATCHDOG_DISABLE_CODE3                   = (int)0xC31E,
   WATCHDOG_DEBUG_CODE3                     = (int)0x3DB2,
   MAX_WATCHDOG_DISABLE                     = (int)0xD6BF
};
typedef enum watchdog_disable_enum watchdog_disable;


enum watchdog_enable_posn_enum
{
   WATCHDOG_ENABLE_POSN                               = (int)0,
   WATCHDOG_ENABLE_WATCHDOG_ENABLE_LSB_POSN           = (int)0,
   WATCHDOG_ENABLE_WATCHDOG_ENABLE_MSB_POSN           = (int)0
};
typedef enum watchdog_enable_posn_enum watchdog_enable_posn;

#define WATCHDOG_ENABLE_MASK                     (0x00000001u)

enum watchdog_kick_posn_enum
{
   WATCHDOG_KICK_POSN                                 = (int)0,
   WATCHDOG_KICK_WATCHDOG_KICK_LSB_POSN               = (int)0,
   WATCHDOG_KICK_WATCHDOG_KICK_MSB_POSN               = (int)0
};
typedef enum watchdog_kick_posn_enum watchdog_kick_posn;

#define WATCHDOG_KICK_MASK                       (0x00000001u)

enum watchdog_kick_pending_posn_enum
{
   WATCHDOG_KICK_PENDING_POSN                         = (int)0,
   WATCHDOG_KICK_PENDING_WATCHDOG_KICK_PENDING_LSB_POSN = (int)0,
   WATCHDOG_KICK_PENDING_WATCHDOG_KICK_PENDING_MSB_POSN = (int)0
};
typedef enum watchdog_kick_pending_posn_enum watchdog_kick_pending_posn;

#define WATCHDOG_KICK_PENDING_MASK               (0x00000001u)

enum watchdog_slow_kick_pending_posn_enum
{
   WATCHDOG_SLOW_KICK_PENDING_POSN                    = (int)0,
   WATCHDOG_SLOW_KICK_PENDING_WATCHDOG_SLOW_KICK_PENDING_LSB_POSN = (int)0,
   WATCHDOG_SLOW_KICK_PENDING_WATCHDOG_SLOW_KICK_PENDING_MSB_POSN = (int)0
};
typedef enum watchdog_slow_kick_pending_posn_enum watchdog_slow_kick_pending_posn;

#define WATCHDOG_SLOW_KICK_PENDING_MASK          (0x00000001u)

enum watchdog_status_posn_enum
{
   WATCHDOG_STATUS_LSB_POSN                           = (int)0,
   WATCHDOG_STATUS_WATCHDOG_STATUS_LSB_POSN           = (int)0,
   WATCHDOG_STATUS_MSB_POSN                           = (int)3,
   WATCHDOG_STATUS_WATCHDOG_STATUS_MSB_POSN           = (int)3
};
typedef enum watchdog_status_posn_enum watchdog_status_posn;

#define WATCHDOG_STATUS_LSB_MASK                 (0x00000001u)
#define WATCHDOG_STATUS_MSB_MASK                 (0x00000008u)

/* -- ram_control_pmem_1024x16 -- Auto-generated from subsystem memory XML. -- */

enum ram_ctrl_pmem_1024x16_asleep_ds_posn_enum
{
   RAM_CTRL_PMEM_1024X16_ASLEEP_DS_RAM0_POSN          = (int)0,
   RAM_CTRL_PMEM_1024X16_ASLEEP_DS_RAM_CTRL_PMEM_1024X16_ASLEEP_DS_RAM0_LSB_POSN = (int)0,
   RAM_CTRL_PMEM_1024X16_ASLEEP_DS_RAM_CTRL_PMEM_1024X16_ASLEEP_DS_RAM0_MSB_POSN = (int)0
};
typedef enum ram_ctrl_pmem_1024x16_asleep_ds_posn_enum ram_ctrl_pmem_1024x16_asleep_ds_posn;

#define RAM_CTRL_PMEM_1024X16_ASLEEP_DS_RAM0_MASK (0x00000001u)

enum ram_ctrl_pmem_1024x16_asleep_ls_posn_enum
{
   RAM_CTRL_PMEM_1024X16_ASLEEP_LS_RAM0_POSN          = (int)0,
   RAM_CTRL_PMEM_1024X16_ASLEEP_LS_RAM_CTRL_PMEM_1024X16_ASLEEP_LS_RAM0_LSB_POSN = (int)0,
   RAM_CTRL_PMEM_1024X16_ASLEEP_LS_RAM_CTRL_PMEM_1024X16_ASLEEP_LS_RAM0_MSB_POSN = (int)0
};
typedef enum ram_ctrl_pmem_1024x16_asleep_ls_posn_enum ram_ctrl_pmem_1024x16_asleep_ls_posn;

#define RAM_CTRL_PMEM_1024X16_ASLEEP_LS_RAM0_MASK (0x00000001u)

enum ram_ctrl_pmem_1024x16_asleep_sd_posn_enum
{
   RAM_CTRL_PMEM_1024X16_ASLEEP_SD_RAM0_POSN          = (int)0,
   RAM_CTRL_PMEM_1024X16_ASLEEP_SD_RAM_CTRL_PMEM_1024X16_ASLEEP_SD_RAM0_LSB_POSN = (int)0,
   RAM_CTRL_PMEM_1024X16_ASLEEP_SD_RAM_CTRL_PMEM_1024X16_ASLEEP_SD_RAM0_MSB_POSN = (int)0
};
typedef enum ram_ctrl_pmem_1024x16_asleep_sd_posn_enum ram_ctrl_pmem_1024x16_asleep_sd_posn;

#define RAM_CTRL_PMEM_1024X16_ASLEEP_SD_RAM0_MASK (0x00000001u)

enum ram_ctrl_pmem_1024x16_awake_ds_posn_enum
{
   RAM_CTRL_PMEM_1024X16_AWAKE_DS_RAM0_POSN           = (int)0,
   RAM_CTRL_PMEM_1024X16_AWAKE_DS_RAM_CTRL_PMEM_1024X16_AWAKE_DS_RAM0_LSB_POSN = (int)0,
   RAM_CTRL_PMEM_1024X16_AWAKE_DS_RAM_CTRL_PMEM_1024X16_AWAKE_DS_RAM0_MSB_POSN = (int)0
};
typedef enum ram_ctrl_pmem_1024x16_awake_ds_posn_enum ram_ctrl_pmem_1024x16_awake_ds_posn;

#define RAM_CTRL_PMEM_1024X16_AWAKE_DS_RAM0_MASK (0x00000001u)

enum ram_ctrl_pmem_1024x16_awake_ls_posn_enum
{
   RAM_CTRL_PMEM_1024X16_AWAKE_LS_RAM0_POSN           = (int)0,
   RAM_CTRL_PMEM_1024X16_AWAKE_LS_RAM_CTRL_PMEM_1024X16_AWAKE_LS_RAM0_LSB_POSN = (int)0,
   RAM_CTRL_PMEM_1024X16_AWAKE_LS_RAM_CTRL_PMEM_1024X16_AWAKE_LS_RAM0_MSB_POSN = (int)0
};
typedef enum ram_ctrl_pmem_1024x16_awake_ls_posn_enum ram_ctrl_pmem_1024x16_awake_ls_posn;

#define RAM_CTRL_PMEM_1024X16_AWAKE_LS_RAM0_MASK (0x00000001u)

enum ram_ctrl_pmem_1024x16_awake_sd_posn_enum
{
   RAM_CTRL_PMEM_1024X16_AWAKE_SD_RAM0_POSN           = (int)0,
   RAM_CTRL_PMEM_1024X16_AWAKE_SD_RAM_CTRL_PMEM_1024X16_AWAKE_SD_RAM0_LSB_POSN = (int)0,
   RAM_CTRL_PMEM_1024X16_AWAKE_SD_RAM_CTRL_PMEM_1024X16_AWAKE_SD_RAM0_MSB_POSN = (int)0
};
typedef enum ram_ctrl_pmem_1024x16_awake_sd_posn_enum ram_ctrl_pmem_1024x16_awake_sd_posn;

#define RAM_CTRL_PMEM_1024X16_AWAKE_SD_RAM0_MASK (0x00000001u)

/* -- bus_interrupt -- Interrupt transaction processing config and status registers -- */

enum bus_int_config_enum_posn_enum
{
   BUS_INT_CONFIG_SUBSYSTEM_LSB_POSN                  = (int)0,
   BUS_INT_CONFIG_ENUM_BUS_INT_CONFIG_SUBSYSTEM_LSB_POSN = (int)0,
   BUS_INT_CONFIG_SUBSYSTEM_MSB_POSN                  = (int)3,
   BUS_INT_CONFIG_ENUM_BUS_INT_CONFIG_SUBSYSTEM_MSB_POSN = (int)3,
   BUS_INT_CONFIG_BLOCK_LSB_POSN                      = (int)4,
   BUS_INT_CONFIG_ENUM_BUS_INT_CONFIG_BLOCK_LSB_POSN  = (int)4,
   BUS_INT_CONFIG_BLOCK_MSB_POSN                      = (int)7,
   BUS_INT_CONFIG_ENUM_BUS_INT_CONFIG_BLOCK_MSB_POSN  = (int)7,
   BUS_INT_CONFIG_ENABLE_POSN                         = (int)8,
   BUS_INT_CONFIG_ENUM_BUS_INT_CONFIG_ENABLE_LSB_POSN = (int)8,
   BUS_INT_CONFIG_ENUM_BUS_INT_CONFIG_ENABLE_MSB_POSN = (int)8,
   BUS_INT_CONFIG_STATUS_CLEAR_ON_READ_POSN           = (int)9,
   BUS_INT_CONFIG_ENUM_BUS_INT_CONFIG_STATUS_CLEAR_ON_READ_LSB_POSN = (int)9,
   BUS_INT_CONFIG_ENUM_BUS_INT_CONFIG_STATUS_CLEAR_ON_READ_MSB_POSN = (int)9
};
typedef enum bus_int_config_enum_posn_enum bus_int_config_enum_posn;

#define BUS_INT_CONFIG_SUBSYSTEM_LSB_MASK        (0x00000001u)
#define BUS_INT_CONFIG_SUBSYSTEM_MSB_MASK        (0x00000008u)
#define BUS_INT_CONFIG_BLOCK_LSB_MASK            (0x00000010u)
#define BUS_INT_CONFIG_BLOCK_MSB_MASK            (0x00000080u)
#define BUS_INT_CONFIG_ENABLE_MASK               (0x00000100u)
#define BUS_INT_CONFIG_STATUS_CLEAR_ON_READ_MASK (0x00000200u)

enum bus_int_send_int_config_posn_enum
{
   BUS_INT_SEND_INT_CONFIG_SUBSYSTEM_LSB_POSN         = (int)0,
   BUS_INT_SEND_INT_CONFIG_BUS_INT_SEND_INT_CONFIG_SUBSYSTEM_LSB_POSN = (int)0,
   BUS_INT_SEND_INT_CONFIG_SUBSYSTEM_MSB_POSN         = (int)3,
   BUS_INT_SEND_INT_CONFIG_BUS_INT_SEND_INT_CONFIG_SUBSYSTEM_MSB_POSN = (int)3,
   BUS_INT_SEND_INT_CONFIG_SRC_BLOCK_ID_LSB_POSN      = (int)4,
   BUS_INT_SEND_INT_CONFIG_BUS_INT_SEND_INT_CONFIG_SRC_BLOCK_ID_LSB_POSN = (int)4,
   BUS_INT_SEND_INT_CONFIG_SRC_BLOCK_ID_MSB_POSN      = (int)7,
   BUS_INT_SEND_INT_CONFIG_BUS_INT_SEND_INT_CONFIG_SRC_BLOCK_ID_MSB_POSN = (int)7,
   BUS_INT_SEND_INT_CONFIG_DEST_BLOCK_ID_LSB_POSN     = (int)8,
   BUS_INT_SEND_INT_CONFIG_BUS_INT_SEND_INT_CONFIG_DEST_BLOCK_ID_LSB_POSN = (int)8,
   BUS_INT_SEND_INT_CONFIG_DEST_BLOCK_ID_MSB_POSN     = (int)11,
   BUS_INT_SEND_INT_CONFIG_BUS_INT_SEND_INT_CONFIG_DEST_BLOCK_ID_MSB_POSN = (int)11
};
typedef enum bus_int_send_int_config_posn_enum bus_int_send_int_config_posn;

#define BUS_INT_SEND_INT_CONFIG_SUBSYSTEM_LSB_MASK (0x00000001u)
#define BUS_INT_SEND_INT_CONFIG_SUBSYSTEM_MSB_MASK (0x00000008u)
#define BUS_INT_SEND_INT_CONFIG_SRC_BLOCK_ID_LSB_MASK (0x00000010u)
#define BUS_INT_SEND_INT_CONFIG_SRC_BLOCK_ID_MSB_MASK (0x00000080u)
#define BUS_INT_SEND_INT_CONFIG_DEST_BLOCK_ID_LSB_MASK (0x00000100u)
#define BUS_INT_SEND_INT_CONFIG_DEST_BLOCK_ID_MSB_MASK (0x00000800u)

enum bus_int_clear_posn_enum
{
   BUS_INT_CLEAR_LSB_POSN                             = (int)0,
   BUS_INT_CLEAR_BUS_INT_CLEAR_LSB_POSN               = (int)0,
   BUS_INT_CLEAR_MSB_POSN                             = (int)15,
   BUS_INT_CLEAR_BUS_INT_CLEAR_MSB_POSN               = (int)15
};
typedef enum bus_int_clear_posn_enum bus_int_clear_posn;

#define BUS_INT_CLEAR_LSB_MASK                   (0x00000001u)
#define BUS_INT_CLEAR_MSB_MASK                   (0x00008000u)

enum bus_int_mask_status_posn_enum
{
   BUS_INT_MASK_STATUS_LSB_POSN                       = (int)0,
   BUS_INT_MASK_STATUS_BUS_INT_MASK_STATUS_LSB_POSN   = (int)0,
   BUS_INT_MASK_STATUS_MSB_POSN                       = (int)15,
   BUS_INT_MASK_STATUS_BUS_INT_MASK_STATUS_MSB_POSN   = (int)15
};
typedef enum bus_int_mask_status_posn_enum bus_int_mask_status_posn;

#define BUS_INT_MASK_STATUS_LSB_MASK             (0x00000001u)
#define BUS_INT_MASK_STATUS_MSB_MASK             (0x00008000u)

enum bus_int_select_posn_enum
{
   BUS_INT_SELECT_LSB_POSN                            = (int)0,
   BUS_INT_SELECT_BUS_INT_SELECT_LSB_POSN             = (int)0,
   BUS_INT_SELECT_MSB_POSN                            = (int)3,
   BUS_INT_SELECT_BUS_INT_SELECT_MSB_POSN             = (int)3
};
typedef enum bus_int_select_posn_enum bus_int_select_posn;

#define BUS_INT_SELECT_LSB_MASK                  (0x00000001u)
#define BUS_INT_SELECT_MSB_MASK                  (0x00000008u)

enum bus_int_send_int_send_status_posn_enum
{
   BUS_INT_SEND_INT_SEND_STATUS_LSB_POSN              = (int)0,
   BUS_INT_SEND_INT_SEND_STATUS_BUS_INT_SEND_INT_SEND_STATUS_LSB_POSN = (int)0,
   BUS_INT_SEND_INT_SEND_STATUS_MSB_POSN              = (int)1,
   BUS_INT_SEND_INT_SEND_STATUS_BUS_INT_SEND_INT_SEND_STATUS_MSB_POSN = (int)1
};
typedef enum bus_int_send_int_send_status_posn_enum bus_int_send_int_send_status_posn;

#define BUS_INT_SEND_INT_SEND_STATUS_LSB_MASK    (0x00000001u)
#define BUS_INT_SEND_INT_SEND_STATUS_MSB_MASK    (0x00000002u)

enum bus_int_send_int_status_field_posn_enum
{
   BUS_INT_SEND_INT_STATUS_FIELD_LSB_POSN             = (int)0,
   BUS_INT_SEND_INT_STATUS_FIELD_BUS_INT_SEND_INT_STATUS_FIELD_LSB_POSN = (int)0,
   BUS_INT_SEND_INT_STATUS_FIELD_MSB_POSN             = (int)15,
   BUS_INT_SEND_INT_STATUS_FIELD_BUS_INT_SEND_INT_STATUS_FIELD_MSB_POSN = (int)15
};
typedef enum bus_int_send_int_status_field_posn_enum bus_int_send_int_status_field_posn;

#define BUS_INT_SEND_INT_STATUS_FIELD_LSB_MASK   (0x00000001u)
#define BUS_INT_SEND_INT_STATUS_FIELD_MSB_MASK   (0x00008000u)

enum bus_int_status_posn_enum
{
   BUS_INT_STATUS_LSB_POSN                            = (int)0,
   BUS_INT_STATUS_BUS_INT_STATUS_LSB_POSN             = (int)0,
   BUS_INT_STATUS_MSB_POSN                            = (int)15,
   BUS_INT_STATUS_BUS_INT_STATUS_MSB_POSN             = (int)15
};
typedef enum bus_int_status_posn_enum bus_int_status_posn;

#define BUS_INT_STATUS_LSB_MASK                  (0x00000001u)
#define BUS_INT_STATUS_MSB_MASK                  (0x00008000u)

enum bus_int_config_posn_enum
{
   BUS_INT_CONFIG_BUS_INT_CONFIG_SUBSYSTEM_LSB_POSN   = (int)0,
   BUS_INT_CONFIG_BUS_INT_CONFIG_SUBSYSTEM_MSB_POSN   = (int)3,
   BUS_INT_CONFIG_BUS_INT_CONFIG_BLOCK_LSB_POSN       = (int)4,
   BUS_INT_CONFIG_BUS_INT_CONFIG_BLOCK_MSB_POSN       = (int)7,
   BUS_INT_CONFIG_BUS_INT_CONFIG_ENABLE_LSB_POSN      = (int)8,
   BUS_INT_CONFIG_BUS_INT_CONFIG_ENABLE_MSB_POSN      = (int)8,
   BUS_INT_CONFIG_BUS_INT_CONFIG_STATUS_CLEAR_ON_READ_LSB_POSN = (int)9,
   BUS_INT_CONFIG_BUS_INT_CONFIG_STATUS_CLEAR_ON_READ_MSB_POSN = (int)9
};
typedef enum bus_int_config_posn_enum bus_int_config_posn;


enum bus_int_config_status_posn_enum
{
   BUS_INT_CONFIG_STATUS_BUS_INT_CONFIG_SUBSYSTEM_LSB_POSN = (int)0,
   BUS_INT_CONFIG_STATUS_BUS_INT_CONFIG_SUBSYSTEM_MSB_POSN = (int)3,
   BUS_INT_CONFIG_STATUS_BUS_INT_CONFIG_BLOCK_LSB_POSN = (int)4,
   BUS_INT_CONFIG_STATUS_BUS_INT_CONFIG_BLOCK_MSB_POSN = (int)7,
   BUS_INT_CONFIG_STATUS_BUS_INT_CONFIG_ENABLE_LSB_POSN = (int)8,
   BUS_INT_CONFIG_STATUS_BUS_INT_CONFIG_ENABLE_MSB_POSN = (int)8,
   BUS_INT_CONFIG_STATUS_BUS_INT_CONFIG_STATUS_CLEAR_ON_READ_LSB_POSN = (int)9,
   BUS_INT_CONFIG_STATUS_BUS_INT_CONFIG_STATUS_CLEAR_ON_READ_MSB_POSN = (int)9
};
typedef enum bus_int_config_status_posn_enum bus_int_config_status_posn;


/* -- mildred_sfr_core -- Mildred 8051 special function registers (reset by all post boot resets) -- */

enum mildred_internal_int_regs_posn_enum
{
   MILDRED_INTERNAL_INT_REGS_INT_PRE_2_POSN           = (int)0,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_INT_PRE_2_LSB_POSN = (int)0,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_INT_PRE_2_MSB_POSN = (int)0,
   MILDRED_INTERNAL_INT_REGS_INT_LOW_POSN             = (int)1,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_INT_LOW_LSB_POSN = (int)1,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_INT_LOW_MSB_POSN = (int)1,
   MILDRED_INTERNAL_INT_REGS_INT_HIGH_POSN            = (int)2,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_INT_HIGH_LSB_POSN = (int)2,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_INT_HIGH_MSB_POSN = (int)2,
   MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_DATA_POSN      = (int)3,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_DATA_LSB_POSN = (int)3,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_DATA_MSB_POSN = (int)3,
   MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_DATA_POSN      = (int)4,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_DATA_LSB_POSN = (int)4,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_DATA_MSB_POSN = (int)4,
   MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_HIGH_PRIORITY_DATA_POSN = (int)5,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_HIGH_PRIORITY_DATA_LSB_POSN = (int)5,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_HIGH_PRIORITY_DATA_MSB_POSN = (int)5,
   MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_HIGH_PRIORITY_DATA_POSN = (int)6,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_HIGH_PRIORITY_DATA_LSB_POSN = (int)6,
   MILDRED_INTERNAL_INT_REGS_MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_HIGH_PRIORITY_DATA_MSB_POSN = (int)6
};
typedef enum mildred_internal_int_regs_posn_enum mildred_internal_int_regs_posn;

#define MILDRED_INTERNAL_INT_REGS_INT_PRE_2_MASK (0x00000001u)
#define MILDRED_INTERNAL_INT_REGS_INT_LOW_MASK   (0x00000002u)
#define MILDRED_INTERNAL_INT_REGS_INT_HIGH_MASK  (0x00000004u)
#define MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_DATA_MASK (0x00000008u)
#define MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_DATA_MASK (0x00000010u)
#define MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_HIGH_PRIORITY_DATA_MASK (0x00000020u)
#define MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_HIGH_PRIORITY_DATA_MASK (0x00000040u)

enum mildred_tmod_timer_mode_enum_enum
{
   MILDRED_TMOD_TIMER_MODE_13_BIT           = (int)0x0,
   MILDRED_TMOD_TIMER_MODE_16_BIT           = (int)0x1,
   MILDRED_TMOD_TIMER_MODE_8_BIT_AUTO_RELOAD = (int)0x2,
   MILDRED_TMOD_TIMER_MODE_SPLIT            = (int)0x3,
   MAX_MILDRED_TMOD_TIMER_MODE_ENUM         = (int)0x3
};
#define NUM_MILDRED_TMOD_TIMER_MODE_ENUM (0x4)
typedef enum mildred_tmod_timer_mode_enum_enum mildred_tmod_timer_mode_enum;


enum mildred_acc_posn_enum
{
   MILDRED_ACC_0_POSN                                 = (int)0,
   MILDRED_ACC_MILDRED_ACC_0_LSB_POSN                 = (int)0,
   MILDRED_ACC_MILDRED_ACC_0_MSB_POSN                 = (int)0,
   MILDRED_ACC_1_POSN                                 = (int)1,
   MILDRED_ACC_MILDRED_ACC_1_LSB_POSN                 = (int)1,
   MILDRED_ACC_MILDRED_ACC_1_MSB_POSN                 = (int)1,
   MILDRED_ACC_2_POSN                                 = (int)2,
   MILDRED_ACC_MILDRED_ACC_2_LSB_POSN                 = (int)2,
   MILDRED_ACC_MILDRED_ACC_2_MSB_POSN                 = (int)2,
   MILDRED_ACC_3_POSN                                 = (int)3,
   MILDRED_ACC_MILDRED_ACC_3_LSB_POSN                 = (int)3,
   MILDRED_ACC_MILDRED_ACC_3_MSB_POSN                 = (int)3,
   MILDRED_ACC_4_POSN                                 = (int)4,
   MILDRED_ACC_MILDRED_ACC_4_LSB_POSN                 = (int)4,
   MILDRED_ACC_MILDRED_ACC_4_MSB_POSN                 = (int)4,
   MILDRED_ACC_5_POSN                                 = (int)5,
   MILDRED_ACC_MILDRED_ACC_5_LSB_POSN                 = (int)5,
   MILDRED_ACC_MILDRED_ACC_5_MSB_POSN                 = (int)5,
   MILDRED_ACC_6_POSN                                 = (int)6,
   MILDRED_ACC_MILDRED_ACC_6_LSB_POSN                 = (int)6,
   MILDRED_ACC_MILDRED_ACC_6_MSB_POSN                 = (int)6,
   MILDRED_ACC_7_POSN                                 = (int)7,
   MILDRED_ACC_MILDRED_ACC_7_LSB_POSN                 = (int)7,
   MILDRED_ACC_MILDRED_ACC_7_MSB_POSN                 = (int)7
};
typedef enum mildred_acc_posn_enum mildred_acc_posn;

#define MILDRED_ACC_0_MASK                       (0x00000001u)
#define MILDRED_ACC_1_MASK                       (0x00000002u)
#define MILDRED_ACC_2_MASK                       (0x00000004u)
#define MILDRED_ACC_3_MASK                       (0x00000008u)
#define MILDRED_ACC_4_MASK                       (0x00000010u)
#define MILDRED_ACC_5_MASK                       (0x00000020u)
#define MILDRED_ACC_6_MASK                       (0x00000040u)
#define MILDRED_ACC_7_MASK                       (0x00000080u)

enum mildred_b_posn_enum
{
   MILDRED_B_0_POSN                                   = (int)0,
   MILDRED_B_MILDRED_B_0_LSB_POSN                     = (int)0,
   MILDRED_B_MILDRED_B_0_MSB_POSN                     = (int)0,
   MILDRED_B_1_POSN                                   = (int)1,
   MILDRED_B_MILDRED_B_1_LSB_POSN                     = (int)1,
   MILDRED_B_MILDRED_B_1_MSB_POSN                     = (int)1,
   MILDRED_B_2_POSN                                   = (int)2,
   MILDRED_B_MILDRED_B_2_LSB_POSN                     = (int)2,
   MILDRED_B_MILDRED_B_2_MSB_POSN                     = (int)2,
   MILDRED_B_3_POSN                                   = (int)3,
   MILDRED_B_MILDRED_B_3_LSB_POSN                     = (int)3,
   MILDRED_B_MILDRED_B_3_MSB_POSN                     = (int)3,
   MILDRED_B_4_POSN                                   = (int)4,
   MILDRED_B_MILDRED_B_4_LSB_POSN                     = (int)4,
   MILDRED_B_MILDRED_B_4_MSB_POSN                     = (int)4,
   MILDRED_B_5_POSN                                   = (int)5,
   MILDRED_B_MILDRED_B_5_LSB_POSN                     = (int)5,
   MILDRED_B_MILDRED_B_5_MSB_POSN                     = (int)5,
   MILDRED_B_6_POSN                                   = (int)6,
   MILDRED_B_MILDRED_B_6_LSB_POSN                     = (int)6,
   MILDRED_B_MILDRED_B_6_MSB_POSN                     = (int)6,
   MILDRED_B_7_POSN                                   = (int)7,
   MILDRED_B_MILDRED_B_7_LSB_POSN                     = (int)7,
   MILDRED_B_MILDRED_B_7_MSB_POSN                     = (int)7
};
typedef enum mildred_b_posn_enum mildred_b_posn;

#define MILDRED_B_0_MASK                         (0x00000001u)
#define MILDRED_B_1_MASK                         (0x00000002u)
#define MILDRED_B_2_MASK                         (0x00000004u)
#define MILDRED_B_3_MASK                         (0x00000008u)
#define MILDRED_B_4_MASK                         (0x00000010u)
#define MILDRED_B_5_MASK                         (0x00000020u)
#define MILDRED_B_6_MASK                         (0x00000040u)
#define MILDRED_B_7_MASK                         (0x00000080u)

enum mildred_ie_posn_enum
{
   MILDRED_IE_EX0_POSN                                = (int)0,
   MILDRED_IE_MILDRED_IE_EX0_LSB_POSN                 = (int)0,
   MILDRED_IE_MILDRED_IE_EX0_MSB_POSN                 = (int)0,
   MILDRED_IE_ET0_POSN                                = (int)1,
   MILDRED_IE_MILDRED_IE_ET0_LSB_POSN                 = (int)1,
   MILDRED_IE_MILDRED_IE_ET0_MSB_POSN                 = (int)1,
   MILDRED_IE_EX1_POSN                                = (int)2,
   MILDRED_IE_MILDRED_IE_EX1_LSB_POSN                 = (int)2,
   MILDRED_IE_MILDRED_IE_EX1_MSB_POSN                 = (int)2,
   MILDRED_IE_ET1_POSN                                = (int)3,
   MILDRED_IE_MILDRED_IE_ET1_LSB_POSN                 = (int)3,
   MILDRED_IE_MILDRED_IE_ET1_MSB_POSN                 = (int)3,
   MILDRED_IE_ES_POSN                                 = (int)4,
   MILDRED_IE_MILDRED_IE_ES_LSB_POSN                  = (int)4,
   MILDRED_IE_MILDRED_IE_ES_MSB_POSN                  = (int)4,
   MILDRED_IE_ET2_POSN                                = (int)5,
   MILDRED_IE_MILDRED_IE_ET2_LSB_POSN                 = (int)5,
   MILDRED_IE_MILDRED_IE_ET2_MSB_POSN                 = (int)5,
   MILDRED_IE_RSVD_6_POSN                             = (int)6,
   MILDRED_IE_MILDRED_IE_RSVD_6_LSB_POSN              = (int)6,
   MILDRED_IE_MILDRED_IE_RSVD_6_MSB_POSN              = (int)6,
   MILDRED_IE_EA_POSN                                 = (int)7,
   MILDRED_IE_MILDRED_IE_EA_LSB_POSN                  = (int)7,
   MILDRED_IE_MILDRED_IE_EA_MSB_POSN                  = (int)7
};
typedef enum mildred_ie_posn_enum mildred_ie_posn;

#define MILDRED_IE_EX0_MASK                      (0x00000001u)
#define MILDRED_IE_ET0_MASK                      (0x00000002u)
#define MILDRED_IE_EX1_MASK                      (0x00000004u)
#define MILDRED_IE_ET1_MASK                      (0x00000008u)
#define MILDRED_IE_ES_MASK                       (0x00000010u)
#define MILDRED_IE_ET2_MASK                      (0x00000020u)
#define MILDRED_IE_RSVD_6_MASK                   (0x00000040u)
#define MILDRED_IE_EA_MASK                       (0x00000080u)

enum mildred_ip_posn_enum
{
   MILDRED_IP_PX0_POSN                                = (int)0,
   MILDRED_IP_MILDRED_IP_PX0_LSB_POSN                 = (int)0,
   MILDRED_IP_MILDRED_IP_PX0_MSB_POSN                 = (int)0,
   MILDRED_IP_PT0_POSN                                = (int)1,
   MILDRED_IP_MILDRED_IP_PT0_LSB_POSN                 = (int)1,
   MILDRED_IP_MILDRED_IP_PT0_MSB_POSN                 = (int)1,
   MILDRED_IP_PX1_POSN                                = (int)2,
   MILDRED_IP_MILDRED_IP_PX1_LSB_POSN                 = (int)2,
   MILDRED_IP_MILDRED_IP_PX1_MSB_POSN                 = (int)2,
   MILDRED_IP_PT1_POSN                                = (int)3,
   MILDRED_IP_MILDRED_IP_PT1_LSB_POSN                 = (int)3,
   MILDRED_IP_MILDRED_IP_PT1_MSB_POSN                 = (int)3,
   MILDRED_IP_PS_POSN                                 = (int)4,
   MILDRED_IP_MILDRED_IP_PS_LSB_POSN                  = (int)4,
   MILDRED_IP_MILDRED_IP_PS_MSB_POSN                  = (int)4,
   MILDRED_IP_PT2_POSN                                = (int)5,
   MILDRED_IP_MILDRED_IP_PT2_LSB_POSN                 = (int)5,
   MILDRED_IP_MILDRED_IP_PT2_MSB_POSN                 = (int)5,
   MILDRED_IP_RSVD_6_POSN                             = (int)6,
   MILDRED_IP_MILDRED_IP_RSVD_6_LSB_POSN              = (int)6,
   MILDRED_IP_MILDRED_IP_RSVD_6_MSB_POSN              = (int)6,
   MILDRED_IP_RSVD_7_POSN                             = (int)7,
   MILDRED_IP_MILDRED_IP_RSVD_7_LSB_POSN              = (int)7,
   MILDRED_IP_MILDRED_IP_RSVD_7_MSB_POSN              = (int)7
};
typedef enum mildred_ip_posn_enum mildred_ip_posn;

#define MILDRED_IP_PX0_MASK                      (0x00000001u)
#define MILDRED_IP_PT0_MASK                      (0x00000002u)
#define MILDRED_IP_PX1_MASK                      (0x00000004u)
#define MILDRED_IP_PT1_MASK                      (0x00000008u)
#define MILDRED_IP_PS_MASK                       (0x00000010u)
#define MILDRED_IP_PT2_MASK                      (0x00000020u)
#define MILDRED_IP_RSVD_6_MASK                   (0x00000040u)
#define MILDRED_IP_RSVD_7_MASK                   (0x00000080u)

enum mildred_pcon_posn_enum
{
   MILDRED_PCON_0_POSN                                = (int)0,
   MILDRED_PCON_MILDRED_PCON_0_LSB_POSN               = (int)0,
   MILDRED_PCON_MILDRED_PCON_0_MSB_POSN               = (int)0,
   MILDRED_PCON_1_POSN                                = (int)1,
   MILDRED_PCON_MILDRED_PCON_1_LSB_POSN               = (int)1,
   MILDRED_PCON_MILDRED_PCON_1_MSB_POSN               = (int)1,
   MILDRED_PCON_2_POSN                                = (int)2,
   MILDRED_PCON_MILDRED_PCON_2_LSB_POSN               = (int)2,
   MILDRED_PCON_MILDRED_PCON_2_MSB_POSN               = (int)2,
   MILDRED_PCON_3_POSN                                = (int)3,
   MILDRED_PCON_MILDRED_PCON_3_LSB_POSN               = (int)3,
   MILDRED_PCON_MILDRED_PCON_3_MSB_POSN               = (int)3,
   MILDRED_PCON_4_POSN                                = (int)4,
   MILDRED_PCON_MILDRED_PCON_4_LSB_POSN               = (int)4,
   MILDRED_PCON_MILDRED_PCON_4_MSB_POSN               = (int)4,
   MILDRED_PCON_5_POSN                                = (int)5,
   MILDRED_PCON_MILDRED_PCON_5_LSB_POSN               = (int)5,
   MILDRED_PCON_MILDRED_PCON_5_MSB_POSN               = (int)5,
   MILDRED_PCON_6_POSN                                = (int)6,
   MILDRED_PCON_MILDRED_PCON_6_LSB_POSN               = (int)6,
   MILDRED_PCON_MILDRED_PCON_6_MSB_POSN               = (int)6,
   MILDRED_PCON_SMOD_POSN                             = (int)7,
   MILDRED_PCON_MILDRED_PCON_SMOD_LSB_POSN            = (int)7,
   MILDRED_PCON_MILDRED_PCON_SMOD_MSB_POSN            = (int)7
};
typedef enum mildred_pcon_posn_enum mildred_pcon_posn;

#define MILDRED_PCON_0_MASK                      (0x00000001u)
#define MILDRED_PCON_1_MASK                      (0x00000002u)
#define MILDRED_PCON_2_MASK                      (0x00000004u)
#define MILDRED_PCON_3_MASK                      (0x00000008u)
#define MILDRED_PCON_4_MASK                      (0x00000010u)
#define MILDRED_PCON_5_MASK                      (0x00000020u)
#define MILDRED_PCON_6_MASK                      (0x00000040u)
#define MILDRED_PCON_SMOD_MASK                   (0x00000080u)

enum mildred_psw_posn_enum
{
   MILDRED_PSW_P_POSN                                 = (int)0,
   MILDRED_PSW_MILDRED_PSW_P_LSB_POSN                 = (int)0,
   MILDRED_PSW_MILDRED_PSW_P_MSB_POSN                 = (int)0,
   MILDRED_PSW_USER_POSN                              = (int)1,
   MILDRED_PSW_MILDRED_PSW_USER_LSB_POSN              = (int)1,
   MILDRED_PSW_MILDRED_PSW_USER_MSB_POSN              = (int)1,
   MILDRED_PSW_OV_POSN                                = (int)2,
   MILDRED_PSW_MILDRED_PSW_OV_LSB_POSN                = (int)2,
   MILDRED_PSW_MILDRED_PSW_OV_MSB_POSN                = (int)2,
   MILDRED_PSW_RS0_POSN                               = (int)3,
   MILDRED_PSW_MILDRED_PSW_RS0_LSB_POSN               = (int)3,
   MILDRED_PSW_MILDRED_PSW_RS0_MSB_POSN               = (int)3,
   MILDRED_PSW_RS1_POSN                               = (int)4,
   MILDRED_PSW_MILDRED_PSW_RS1_LSB_POSN               = (int)4,
   MILDRED_PSW_MILDRED_PSW_RS1_MSB_POSN               = (int)4,
   MILDRED_PSW_F0_POSN                                = (int)5,
   MILDRED_PSW_MILDRED_PSW_F0_LSB_POSN                = (int)5,
   MILDRED_PSW_MILDRED_PSW_F0_MSB_POSN                = (int)5,
   MILDRED_PSW_AC_POSN                                = (int)6,
   MILDRED_PSW_MILDRED_PSW_AC_LSB_POSN                = (int)6,
   MILDRED_PSW_MILDRED_PSW_AC_MSB_POSN                = (int)6,
   MILDRED_PSW_CY_POSN                                = (int)7,
   MILDRED_PSW_MILDRED_PSW_CY_LSB_POSN                = (int)7,
   MILDRED_PSW_MILDRED_PSW_CY_MSB_POSN                = (int)7
};
typedef enum mildred_psw_posn_enum mildred_psw_posn;

#define MILDRED_PSW_P_MASK                       (0x00000001u)
#define MILDRED_PSW_USER_MASK                    (0x00000002u)
#define MILDRED_PSW_OV_MASK                      (0x00000004u)
#define MILDRED_PSW_RS0_MASK                     (0x00000008u)
#define MILDRED_PSW_RS1_MASK                     (0x00000010u)
#define MILDRED_PSW_F0_MASK                      (0x00000020u)
#define MILDRED_PSW_AC_MASK                      (0x00000040u)
#define MILDRED_PSW_CY_MASK                      (0x00000080u)

enum mildred_scon_posn_enum
{
   MILDRED_SCON_RI_POSN                               = (int)0,
   MILDRED_SCON_MILDRED_SCON_RI_LSB_POSN              = (int)0,
   MILDRED_SCON_MILDRED_SCON_RI_MSB_POSN              = (int)0,
   MILDRED_SCON_TI_POSN                               = (int)1,
   MILDRED_SCON_MILDRED_SCON_TI_LSB_POSN              = (int)1,
   MILDRED_SCON_MILDRED_SCON_TI_MSB_POSN              = (int)1,
   MILDRED_SCON_RB8_POSN                              = (int)2,
   MILDRED_SCON_MILDRED_SCON_RB8_LSB_POSN             = (int)2,
   MILDRED_SCON_MILDRED_SCON_RB8_MSB_POSN             = (int)2,
   MILDRED_SCON_TB8_POSN                              = (int)3,
   MILDRED_SCON_MILDRED_SCON_TB8_LSB_POSN             = (int)3,
   MILDRED_SCON_MILDRED_SCON_TB8_MSB_POSN             = (int)3,
   MILDRED_SCON_REN_POSN                              = (int)4,
   MILDRED_SCON_MILDRED_SCON_REN_LSB_POSN             = (int)4,
   MILDRED_SCON_MILDRED_SCON_REN_MSB_POSN             = (int)4,
   MILDRED_SCON_SM2_POSN                              = (int)5,
   MILDRED_SCON_MILDRED_SCON_SM2_LSB_POSN             = (int)5,
   MILDRED_SCON_MILDRED_SCON_SM2_MSB_POSN             = (int)5,
   MILDRED_SCON_SM1_POSN                              = (int)6,
   MILDRED_SCON_MILDRED_SCON_SM1_LSB_POSN             = (int)6,
   MILDRED_SCON_MILDRED_SCON_SM1_MSB_POSN             = (int)6,
   MILDRED_SCON_SM0_POSN                              = (int)7,
   MILDRED_SCON_MILDRED_SCON_SM0_LSB_POSN             = (int)7,
   MILDRED_SCON_MILDRED_SCON_SM0_MSB_POSN             = (int)7
};
typedef enum mildred_scon_posn_enum mildred_scon_posn;

#define MILDRED_SCON_RI_MASK                     (0x00000001u)
#define MILDRED_SCON_TI_MASK                     (0x00000002u)
#define MILDRED_SCON_RB8_MASK                    (0x00000004u)
#define MILDRED_SCON_TB8_MASK                    (0x00000008u)
#define MILDRED_SCON_REN_MASK                    (0x00000010u)
#define MILDRED_SCON_SM2_MASK                    (0x00000020u)
#define MILDRED_SCON_SM1_MASK                    (0x00000040u)
#define MILDRED_SCON_SM0_MASK                    (0x00000080u)

enum mildred_t2con_posn_enum
{
   MILDRED_TCON_CPRL2_POSN                            = (int)0,
   MILDRED_T2CON_MILDRED_TCON_CPRL2_LSB_POSN          = (int)0,
   MILDRED_T2CON_MILDRED_TCON_CPRL2_MSB_POSN          = (int)0,
   MILDRED_TCON_CT2_POSN                              = (int)1,
   MILDRED_T2CON_MILDRED_TCON_CT2_LSB_POSN            = (int)1,
   MILDRED_T2CON_MILDRED_TCON_CT2_MSB_POSN            = (int)1,
   MILDRED_TCON_TR2_POSN                              = (int)2,
   MILDRED_T2CON_MILDRED_TCON_TR2_LSB_POSN            = (int)2,
   MILDRED_T2CON_MILDRED_TCON_TR2_MSB_POSN            = (int)2,
   MILDRED_TCON_EXEN2_POSN                            = (int)3,
   MILDRED_T2CON_MILDRED_TCON_EXEN2_LSB_POSN          = (int)3,
   MILDRED_T2CON_MILDRED_TCON_EXEN2_MSB_POSN          = (int)3,
   MILDRED_TCON_TCLK_POSN                             = (int)4,
   MILDRED_T2CON_MILDRED_TCON_TCLK_LSB_POSN           = (int)4,
   MILDRED_T2CON_MILDRED_TCON_TCLK_MSB_POSN           = (int)4,
   MILDRED_TCON_RCLK_POSN                             = (int)5,
   MILDRED_T2CON_MILDRED_TCON_RCLK_LSB_POSN           = (int)5,
   MILDRED_T2CON_MILDRED_TCON_RCLK_MSB_POSN           = (int)5,
   MILDRED_TCON_EXF2_POSN                             = (int)6,
   MILDRED_T2CON_MILDRED_TCON_EXF2_LSB_POSN           = (int)6,
   MILDRED_T2CON_MILDRED_TCON_EXF2_MSB_POSN           = (int)6,
   MILDRED_TCON_TF2_POSN                              = (int)7,
   MILDRED_T2CON_MILDRED_TCON_TF2_LSB_POSN            = (int)7,
   MILDRED_T2CON_MILDRED_TCON_TF2_MSB_POSN            = (int)7
};
typedef enum mildred_t2con_posn_enum mildred_t2con_posn;

#define MILDRED_TCON_CPRL2_MASK                  (0x00000001u)
#define MILDRED_TCON_CT2_MASK                    (0x00000002u)
#define MILDRED_TCON_TR2_MASK                    (0x00000004u)
#define MILDRED_TCON_EXEN2_MASK                  (0x00000008u)
#define MILDRED_TCON_TCLK_MASK                   (0x00000010u)
#define MILDRED_TCON_RCLK_MASK                   (0x00000020u)
#define MILDRED_TCON_EXF2_MASK                   (0x00000040u)
#define MILDRED_TCON_TF2_MASK                    (0x00000080u)

enum mildred_tcon_posn_enum
{
   MILDRED_TCON_IT0_POSN                              = (int)0,
   MILDRED_TCON_MILDRED_TCON_IT0_LSB_POSN             = (int)0,
   MILDRED_TCON_MILDRED_TCON_IT0_MSB_POSN             = (int)0,
   MILDRED_TCON_IE0_POSN                              = (int)1,
   MILDRED_TCON_MILDRED_TCON_IE0_LSB_POSN             = (int)1,
   MILDRED_TCON_MILDRED_TCON_IE0_MSB_POSN             = (int)1,
   MILDRED_TCON_IT1_POSN                              = (int)2,
   MILDRED_TCON_MILDRED_TCON_IT1_LSB_POSN             = (int)2,
   MILDRED_TCON_MILDRED_TCON_IT1_MSB_POSN             = (int)2,
   MILDRED_TCON_IE1_POSN                              = (int)3,
   MILDRED_TCON_MILDRED_TCON_IE1_LSB_POSN             = (int)3,
   MILDRED_TCON_MILDRED_TCON_IE1_MSB_POSN             = (int)3,
   MILDRED_TCON_TR0_POSN                              = (int)4,
   MILDRED_TCON_MILDRED_TCON_TR0_LSB_POSN             = (int)4,
   MILDRED_TCON_MILDRED_TCON_TR0_MSB_POSN             = (int)4,
   MILDRED_TCON_TF0_POSN                              = (int)5,
   MILDRED_TCON_MILDRED_TCON_TF0_LSB_POSN             = (int)5,
   MILDRED_TCON_MILDRED_TCON_TF0_MSB_POSN             = (int)5,
   MILDRED_TCON_TR1_POSN                              = (int)6,
   MILDRED_TCON_MILDRED_TCON_TR1_LSB_POSN             = (int)6,
   MILDRED_TCON_MILDRED_TCON_TR1_MSB_POSN             = (int)6,
   MILDRED_TCON_TF1_POSN                              = (int)7,
   MILDRED_TCON_MILDRED_TCON_TF1_LSB_POSN             = (int)7,
   MILDRED_TCON_MILDRED_TCON_TF1_MSB_POSN             = (int)7
};
typedef enum mildred_tcon_posn_enum mildred_tcon_posn;

#define MILDRED_TCON_IT0_MASK                    (0x00000001u)
#define MILDRED_TCON_IE0_MASK                    (0x00000002u)
#define MILDRED_TCON_IT1_MASK                    (0x00000004u)
#define MILDRED_TCON_IE1_MASK                    (0x00000008u)
#define MILDRED_TCON_TR0_MASK                    (0x00000010u)
#define MILDRED_TCON_TF0_MASK                    (0x00000020u)
#define MILDRED_TCON_TR1_MASK                    (0x00000040u)
#define MILDRED_TCON_TF1_MASK                    (0x00000080u)

enum mildred_tmod_posn_enum
{
   MILDRED_TMOD_0_POSN                                = (int)0,
   MILDRED_TMOD_MILDRED_TMOD_0_LSB_POSN               = (int)0,
   MILDRED_TMOD_MILDRED_TMOD_0_MSB_POSN               = (int)0,
   MILDRED_TMOD_T0M0_POSN                             = (int)0,
   MILDRED_TMOD_MILDRED_TMOD_T0M0_LSB_POSN            = (int)0,
   MILDRED_TMOD_MILDRED_TMOD_T0M0_MSB_POSN            = (int)0,
   MILDRED_TMOD_1_POSN                                = (int)1,
   MILDRED_TMOD_MILDRED_TMOD_1_LSB_POSN               = (int)1,
   MILDRED_TMOD_MILDRED_TMOD_1_MSB_POSN               = (int)1,
   MILDRED_TMOD_T0M1_POSN                             = (int)1,
   MILDRED_TMOD_MILDRED_TMOD_T0M1_LSB_POSN            = (int)1,
   MILDRED_TMOD_MILDRED_TMOD_T0M1_MSB_POSN            = (int)1,
   MILDRED_TMOD_T0_MODE_LSB_POSN                      = (int)0,
   MILDRED_TMOD_MILDRED_TMOD_T0_MODE_LSB_POSN         = (int)0,
   MILDRED_TMOD_T0_MODE_MSB_POSN                      = (int)1,
   MILDRED_TMOD_MILDRED_TMOD_T0_MODE_MSB_POSN         = (int)1,
   MILDRED_TMOD_2_POSN                                = (int)2,
   MILDRED_TMOD_MILDRED_TMOD_2_LSB_POSN               = (int)2,
   MILDRED_TMOD_MILDRED_TMOD_2_MSB_POSN               = (int)2,
   MILDRED_TMOD_TC0_POSN                              = (int)2,
   MILDRED_TMOD_MILDRED_TMOD_TC0_LSB_POSN             = (int)2,
   MILDRED_TMOD_MILDRED_TMOD_TC0_MSB_POSN             = (int)2,
   MILDRED_TMOD_3_POSN                                = (int)3,
   MILDRED_TMOD_MILDRED_TMOD_3_LSB_POSN               = (int)3,
   MILDRED_TMOD_MILDRED_TMOD_3_MSB_POSN               = (int)3,
   MILDRED_TMOD_GATE0_POSN                            = (int)3,
   MILDRED_TMOD_MILDRED_TMOD_GATE0_LSB_POSN           = (int)3,
   MILDRED_TMOD_MILDRED_TMOD_GATE0_MSB_POSN           = (int)3,
   MILDRED_TMOD_M0_POSN                               = (int)4,
   MILDRED_TMOD_MILDRED_TMOD_M0_LSB_POSN              = (int)4,
   MILDRED_TMOD_MILDRED_TMOD_M0_MSB_POSN              = (int)4,
   MILDRED_TMOD_T1M0_POSN                             = (int)4,
   MILDRED_TMOD_MILDRED_TMOD_T1M0_LSB_POSN            = (int)4,
   MILDRED_TMOD_MILDRED_TMOD_T1M0_MSB_POSN            = (int)4,
   MILDRED_TMOD_M1_POSN                               = (int)5,
   MILDRED_TMOD_MILDRED_TMOD_M1_LSB_POSN              = (int)5,
   MILDRED_TMOD_MILDRED_TMOD_M1_MSB_POSN              = (int)5,
   MILDRED_TMOD_T1M1_POSN                             = (int)5,
   MILDRED_TMOD_MILDRED_TMOD_T1M1_LSB_POSN            = (int)5,
   MILDRED_TMOD_MILDRED_TMOD_T1M1_MSB_POSN            = (int)5,
   MILDRED_TMOD_T1_MODE_LSB_POSN                      = (int)4,
   MILDRED_TMOD_MILDRED_TMOD_T1_MODE_LSB_POSN         = (int)4,
   MILDRED_TMOD_T1_MODE_MSB_POSN                      = (int)5,
   MILDRED_TMOD_MILDRED_TMOD_T1_MODE_MSB_POSN         = (int)5,
   MILDRED_TMOD_TC_POSN                               = (int)6,
   MILDRED_TMOD_MILDRED_TMOD_TC_LSB_POSN              = (int)6,
   MILDRED_TMOD_MILDRED_TMOD_TC_MSB_POSN              = (int)6,
   MILDRED_TMOD_TC1_POSN                              = (int)6,
   MILDRED_TMOD_MILDRED_TMOD_TC1_LSB_POSN             = (int)6,
   MILDRED_TMOD_MILDRED_TMOD_TC1_MSB_POSN             = (int)6,
   MILDRED_TMOD_GATE_POSN                             = (int)7,
   MILDRED_TMOD_MILDRED_TMOD_GATE_LSB_POSN            = (int)7,
   MILDRED_TMOD_MILDRED_TMOD_GATE_MSB_POSN            = (int)7,
   MILDRED_TMOD_GATE1_POSN                            = (int)7,
   MILDRED_TMOD_MILDRED_TMOD_GATE1_LSB_POSN           = (int)7,
   MILDRED_TMOD_MILDRED_TMOD_GATE1_MSB_POSN           = (int)7
};
typedef enum mildred_tmod_posn_enum mildred_tmod_posn;

#define MILDRED_TMOD_0_MASK                      (0x00000001u)
#define MILDRED_TMOD_T0M0_MASK                   (0x00000001u)
#define MILDRED_TMOD_1_MASK                      (0x00000002u)
#define MILDRED_TMOD_T0M1_MASK                   (0x00000002u)
#define MILDRED_TMOD_T0_MODE_LSB_MASK            (0x00000001u)
#define MILDRED_TMOD_T0_MODE_MSB_MASK            (0x00000002u)
#define MILDRED_TMOD_2_MASK                      (0x00000004u)
#define MILDRED_TMOD_TC0_MASK                    (0x00000004u)
#define MILDRED_TMOD_3_MASK                      (0x00000008u)
#define MILDRED_TMOD_GATE0_MASK                  (0x00000008u)
#define MILDRED_TMOD_M0_MASK                     (0x00000010u)
#define MILDRED_TMOD_T1M0_MASK                   (0x00000010u)
#define MILDRED_TMOD_M1_MASK                     (0x00000020u)
#define MILDRED_TMOD_T1M1_MASK                   (0x00000020u)
#define MILDRED_TMOD_T1_MODE_LSB_MASK            (0x00000010u)
#define MILDRED_TMOD_T1_MODE_MSB_MASK            (0x00000020u)
#define MILDRED_TMOD_TC_MASK                     (0x00000040u)
#define MILDRED_TMOD_TC1_MASK                    (0x00000040u)
#define MILDRED_TMOD_GATE_MASK                   (0x00000080u)
#define MILDRED_TMOD_GATE1_MASK                  (0x00000080u)

enum mildred_dph_posn_enum
{
   MILDRED_DPH_LSB_POSN                               = (int)0,
   MILDRED_DPH_MILDRED_DPH_LSB_POSN                   = (int)0,
   MILDRED_DPH_MSB_POSN                               = (int)7,
   MILDRED_DPH_MILDRED_DPH_MSB_POSN                   = (int)7
};
typedef enum mildred_dph_posn_enum mildred_dph_posn;

#define MILDRED_DPH_LSB_MASK                     (0x00000001u)
#define MILDRED_DPH_MSB_MASK                     (0x00000080u)

enum mildred_dpl_posn_enum
{
   MILDRED_DPL_LSB_POSN                               = (int)0,
   MILDRED_DPL_MILDRED_DPL_LSB_POSN                   = (int)0,
   MILDRED_DPL_MSB_POSN                               = (int)7,
   MILDRED_DPL_MILDRED_DPL_MSB_POSN                   = (int)7
};
typedef enum mildred_dpl_posn_enum mildred_dpl_posn;

#define MILDRED_DPL_LSB_MASK                     (0x00000001u)
#define MILDRED_DPL_MSB_MASK                     (0x00000080u)

enum mildred_program_counter_hi_posn_enum
{
   MILDRED_PROGRAM_COUNTER_HI_LSB_POSN                = (int)0,
   MILDRED_PROGRAM_COUNTER_HI_MILDRED_PROGRAM_COUNTER_HI_LSB_POSN = (int)0,
   MILDRED_PROGRAM_COUNTER_HI_MSB_POSN                = (int)7,
   MILDRED_PROGRAM_COUNTER_HI_MILDRED_PROGRAM_COUNTER_HI_MSB_POSN = (int)7
};
typedef enum mildred_program_counter_hi_posn_enum mildred_program_counter_hi_posn;

#define MILDRED_PROGRAM_COUNTER_HI_LSB_MASK      (0x00000001u)
#define MILDRED_PROGRAM_COUNTER_HI_MSB_MASK      (0x00000080u)

enum mildred_program_counter_lo_posn_enum
{
   MILDRED_PROGRAM_COUNTER_LO_LSB_POSN                = (int)0,
   MILDRED_PROGRAM_COUNTER_LO_MILDRED_PROGRAM_COUNTER_LO_LSB_POSN = (int)0,
   MILDRED_PROGRAM_COUNTER_LO_MSB_POSN                = (int)7,
   MILDRED_PROGRAM_COUNTER_LO_MILDRED_PROGRAM_COUNTER_LO_MSB_POSN = (int)7
};
typedef enum mildred_program_counter_lo_posn_enum mildred_program_counter_lo_posn;

#define MILDRED_PROGRAM_COUNTER_LO_LSB_MASK      (0x00000001u)
#define MILDRED_PROGRAM_COUNTER_LO_MSB_MASK      (0x00000080u)

enum mildred_rcap2h_posn_enum
{
   MILDRED_RCAP2H_LSB_POSN                            = (int)0,
   MILDRED_RCAP2H_MILDRED_RCAP2H_LSB_POSN             = (int)0,
   MILDRED_RCAP2H_MSB_POSN                            = (int)7,
   MILDRED_RCAP2H_MILDRED_RCAP2H_MSB_POSN             = (int)7
};
typedef enum mildred_rcap2h_posn_enum mildred_rcap2h_posn;

#define MILDRED_RCAP2H_LSB_MASK                  (0x00000001u)
#define MILDRED_RCAP2H_MSB_MASK                  (0x00000080u)

enum mildred_rcap2l_posn_enum
{
   MILDRED_RCAP2L_LSB_POSN                            = (int)0,
   MILDRED_RCAP2L_MILDRED_RCAP2L_LSB_POSN             = (int)0,
   MILDRED_RCAP2L_MSB_POSN                            = (int)7,
   MILDRED_RCAP2L_MILDRED_RCAP2L_MSB_POSN             = (int)7
};
typedef enum mildred_rcap2l_posn_enum mildred_rcap2l_posn;

#define MILDRED_RCAP2L_LSB_MASK                  (0x00000001u)
#define MILDRED_RCAP2L_MSB_MASK                  (0x00000080u)

enum mildred_sbuf_posn_enum
{
   MILDRED_SBUF_LSB_POSN                              = (int)0,
   MILDRED_SBUF_MILDRED_SBUF_LSB_POSN                 = (int)0,
   MILDRED_SBUF_MSB_POSN                              = (int)7,
   MILDRED_SBUF_MILDRED_SBUF_MSB_POSN                 = (int)7
};
typedef enum mildred_sbuf_posn_enum mildred_sbuf_posn;

#define MILDRED_SBUF_LSB_MASK                    (0x00000001u)
#define MILDRED_SBUF_MSB_MASK                    (0x00000080u)

enum mildred_set_pc_hi_posn_enum
{
   MILDRED_SET_PC_HI_LSB_POSN                         = (int)0,
   MILDRED_SET_PC_HI_MILDRED_SET_PC_HI_LSB_POSN       = (int)0,
   MILDRED_SET_PC_HI_MSB_POSN                         = (int)7,
   MILDRED_SET_PC_HI_MILDRED_SET_PC_HI_MSB_POSN       = (int)7
};
typedef enum mildred_set_pc_hi_posn_enum mildred_set_pc_hi_posn;

#define MILDRED_SET_PC_HI_LSB_MASK               (0x00000001u)
#define MILDRED_SET_PC_HI_MSB_MASK               (0x00000080u)

enum mildred_set_pc_lo_posn_enum
{
   MILDRED_SET_PC_LO_LSB_POSN                         = (int)0,
   MILDRED_SET_PC_LO_MILDRED_SET_PC_LO_LSB_POSN       = (int)0,
   MILDRED_SET_PC_LO_MSB_POSN                         = (int)7,
   MILDRED_SET_PC_LO_MILDRED_SET_PC_LO_MSB_POSN       = (int)7
};
typedef enum mildred_set_pc_lo_posn_enum mildred_set_pc_lo_posn;

#define MILDRED_SET_PC_LO_LSB_MASK               (0x00000001u)
#define MILDRED_SET_PC_LO_MSB_MASK               (0x00000080u)

enum mildred_siu_sdiv_posn_enum
{
   MILDRED_SIU_SDIV_LSB_POSN                          = (int)0,
   MILDRED_SIU_SDIV_MILDRED_SIU_SDIV_LSB_POSN         = (int)0,
   MILDRED_SIU_SDIV_MSB_POSN                          = (int)4,
   MILDRED_SIU_SDIV_MILDRED_SIU_SDIV_MSB_POSN         = (int)4
};
typedef enum mildred_siu_sdiv_posn_enum mildred_siu_sdiv_posn;

#define MILDRED_SIU_SDIV_LSB_MASK                (0x00000001u)
#define MILDRED_SIU_SDIV_MSB_MASK                (0x00000010u)

enum mildred_sp_posn_enum
{
   MILDRED_SP_LSB_POSN                                = (int)0,
   MILDRED_SP_MILDRED_SP_LSB_POSN                     = (int)0,
   MILDRED_SP_MSB_POSN                                = (int)7,
   MILDRED_SP_MILDRED_SP_MSB_POSN                     = (int)7
};
typedef enum mildred_sp_posn_enum mildred_sp_posn;

#define MILDRED_SP_LSB_MASK                      (0x00000001u)
#define MILDRED_SP_MSB_MASK                      (0x00000080u)

enum mildred_th0_posn_enum
{
   MILDRED_TH0_LSB_POSN                               = (int)0,
   MILDRED_TH0_MILDRED_TH0_LSB_POSN                   = (int)0,
   MILDRED_TH0_MSB_POSN                               = (int)7,
   MILDRED_TH0_MILDRED_TH0_MSB_POSN                   = (int)7
};
typedef enum mildred_th0_posn_enum mildred_th0_posn;

#define MILDRED_TH0_LSB_MASK                     (0x00000001u)
#define MILDRED_TH0_MSB_MASK                     (0x00000080u)

enum mildred_th1_posn_enum
{
   MILDRED_TH1_LSB_POSN                               = (int)0,
   MILDRED_TH1_MILDRED_TH1_LSB_POSN                   = (int)0,
   MILDRED_TH1_MSB_POSN                               = (int)7,
   MILDRED_TH1_MILDRED_TH1_MSB_POSN                   = (int)7
};
typedef enum mildred_th1_posn_enum mildred_th1_posn;

#define MILDRED_TH1_LSB_MASK                     (0x00000001u)
#define MILDRED_TH1_MSB_MASK                     (0x00000080u)

enum mildred_th2_posn_enum
{
   MILDRED_TH2_LSB_POSN                               = (int)0,
   MILDRED_TH2_MILDRED_TH2_LSB_POSN                   = (int)0,
   MILDRED_TH2_MSB_POSN                               = (int)7,
   MILDRED_TH2_MILDRED_TH2_MSB_POSN                   = (int)7
};
typedef enum mildred_th2_posn_enum mildred_th2_posn;

#define MILDRED_TH2_LSB_MASK                     (0x00000001u)
#define MILDRED_TH2_MSB_MASK                     (0x00000080u)

enum mildred_timer_counter_prescale_frac_posn_enum
{
   MILDRED_TIMER_COUNTER_PRESCALE_FRAC_LSB_POSN       = (int)0,
   MILDRED_TIMER_COUNTER_PRESCALE_FRAC_MILDRED_TIMER_COUNTER_PRESCALE_FRAC_LSB_POSN = (int)0,
   MILDRED_TIMER_COUNTER_PRESCALE_FRAC_MSB_POSN       = (int)7,
   MILDRED_TIMER_COUNTER_PRESCALE_FRAC_MILDRED_TIMER_COUNTER_PRESCALE_FRAC_MSB_POSN = (int)7
};
typedef enum mildred_timer_counter_prescale_frac_posn_enum mildred_timer_counter_prescale_frac_posn;

#define MILDRED_TIMER_COUNTER_PRESCALE_FRAC_LSB_MASK (0x00000001u)
#define MILDRED_TIMER_COUNTER_PRESCALE_FRAC_MSB_MASK (0x00000080u)

enum mildred_timer_counter_prescale_int_posn_enum
{
   MILDRED_TIMER_COUNTER_PRESCALE_INT_LSB_POSN        = (int)0,
   MILDRED_TIMER_COUNTER_PRESCALE_INT_MILDRED_TIMER_COUNTER_PRESCALE_INT_LSB_POSN = (int)0,
   MILDRED_TIMER_COUNTER_PRESCALE_INT_MSB_POSN        = (int)7,
   MILDRED_TIMER_COUNTER_PRESCALE_INT_MILDRED_TIMER_COUNTER_PRESCALE_INT_MSB_POSN = (int)7
};
typedef enum mildred_timer_counter_prescale_int_posn_enum mildred_timer_counter_prescale_int_posn;

#define MILDRED_TIMER_COUNTER_PRESCALE_INT_LSB_MASK (0x00000001u)
#define MILDRED_TIMER_COUNTER_PRESCALE_INT_MSB_MASK (0x00000080u)

enum mildred_tl0_posn_enum
{
   MILDRED_TL0_LSB_POSN                               = (int)0,
   MILDRED_TL0_MILDRED_TL0_LSB_POSN                   = (int)0,
   MILDRED_TL0_MSB_POSN                               = (int)7,
   MILDRED_TL0_MILDRED_TL0_MSB_POSN                   = (int)7
};
typedef enum mildred_tl0_posn_enum mildred_tl0_posn;

#define MILDRED_TL0_LSB_MASK                     (0x00000001u)
#define MILDRED_TL0_MSB_MASK                     (0x00000080u)

enum mildred_tl1_posn_enum
{
   MILDRED_TL1_LSB_POSN                               = (int)0,
   MILDRED_TL1_MILDRED_TL1_LSB_POSN                   = (int)0,
   MILDRED_TL1_MSB_POSN                               = (int)7,
   MILDRED_TL1_MILDRED_TL1_MSB_POSN                   = (int)7
};
typedef enum mildred_tl1_posn_enum mildred_tl1_posn;

#define MILDRED_TL1_LSB_MASK                     (0x00000001u)
#define MILDRED_TL1_MSB_MASK                     (0x00000080u)

enum mildred_tl2_posn_enum
{
   MILDRED_TL2_LSB_POSN                               = (int)0,
   MILDRED_TL2_MILDRED_TL2_LSB_POSN                   = (int)0,
   MILDRED_TL2_MSB_POSN                               = (int)7,
   MILDRED_TL2_MILDRED_TL2_MSB_POSN                   = (int)7
};
typedef enum mildred_tl2_posn_enum mildred_tl2_posn;

#define MILDRED_TL2_LSB_MASK                     (0x00000001u)
#define MILDRED_TL2_MSB_MASK                     (0x00000080u)

enum mildred_read_write_internal_int_state_posn_enum
{
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_INT_PRE_2_LSB_POSN = (int)0,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_INT_PRE_2_MSB_POSN = (int)0,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_INT_LOW_LSB_POSN = (int)1,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_INT_LOW_MSB_POSN = (int)1,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_INT_HIGH_LSB_POSN = (int)2,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_INT_HIGH_MSB_POSN = (int)2,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_DATA_LSB_POSN = (int)3,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_DATA_MSB_POSN = (int)3,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_DATA_LSB_POSN = (int)4,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_DATA_MSB_POSN = (int)4,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_HIGH_PRIORITY_DATA_LSB_POSN = (int)5,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_EXT_0_ISR_HIGH_PRIORITY_DATA_MSB_POSN = (int)5,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_HIGH_PRIORITY_DATA_LSB_POSN = (int)6,
   MILDRED_READ_WRITE_INTERNAL_INT_STATE_MILDRED_INTERNAL_INT_REGS_EXT_1_ISR_HIGH_PRIORITY_DATA_MSB_POSN = (int)6
};
typedef enum mildred_read_write_internal_int_state_posn_enum mildred_read_write_internal_int_state_posn;


/* -- pio_ctrl_sys_sys -- PIO controller subsystem enumerations -- */

enum pio_ctrl_sys_mux_enum
{
   PIO_CTRL_SYS_MUX_REMOTE_DATA             = (int)0x0,
   PIO_CTRL_SYS_MUX_INTERRUPTS              = (int)0x1,
   PIO_CTRL_SYS_MUX_MISC                    = (int)0x2,
   PIO_CTRL_SYS_MUX_DEBUG                   = (int)0xF,
   PIO_CTRL_SYS_MUX_NUM_QUEUES              = (int)0x3,
   MAX_PIO_CTRL_SYS_MUX                     = (int)0xF
};
typedef enum pio_ctrl_sys_mux_enum pio_ctrl_sys_mux;


enum pio_ctrl_sys_mux_connected_enum
{
   PIO_CTRL_SYS_MUX_CONNECTED_BLOCKS        = (int)0x7,
   MAX_PIO_CTRL_SYS_MUX_CONNECTED           = (int)0x7
};
typedef enum pio_ctrl_sys_mux_connected_enum pio_ctrl_sys_mux_connected;


enum pio_ctrl_sys_debug_source_enum
{
   PIO_CTRL_SYS_DEBUG_SOURCE_BUS_TRANSACTION_MUX = (int)0x0,
   PIO_CTRL_SYS_DEBUG_SOURCE_BUS_INTERRUPT  = (int)0x1,
   PIO_CTRL_SYS_DEBUG_SOURCE_BUS_MISC       = (int)0x2,
   PIO_CTRL_SYS_DEBUG_SOURCE_BUS_REMOTE_DATA = (int)0x3,
   PIO_CTRL_SYS_DEBUG_SOURCE_BUS_REMOTE_DEBUG = (int)0x4,
   PIO_CTRL_SYS_DEBUG_SOURCE_CLKGEN         = (int)0x5,
   PIO_CTRL_SYS_DEBUG_SOURCE_IMEM_ARBITER   = (int)0x6,
   PIO_CTRL_SYS_DEBUG_SOURCE_PMEM_ARBITER   = (int)0x7,
   PIO_CTRL_SYS_DEBUG_SOURCE_REGS_ARBITER   = (int)0x8,
   PIO_CTRL_SYS_DEBUG_SOURCE_DEBUG_REGS_ARBITER = (int)0x9,
   MAX_PIO_CTRL_SYS_DEBUG_SOURCE            = (int)0x9
};
#define NUM_PIO_CTRL_SYS_DEBUG_SOURCE (0xA)
typedef enum pio_ctrl_sys_debug_source_enum pio_ctrl_sys_debug_source;


enum pio_ctrl_sys_spi_memory_map_enum
{
   PIO_CTRL_SYS_SPI_IMEM_OFFSET             = (int)0x0,
   PIO_CTRL_SYS_SPI_IMEM_SIZE               = (int)0x100,
   PIO_CTRL_SYS_SPI_PMEM_OFFSET             = (int)0x1000,
   PIO_CTRL_SYS_SPI_PMEM_SIZE               = (int)0x800,
   PIO_CTRL_SYS_SPI_DEBUG_REGS_OFFSET       = (int)0xF000,
   PIO_CTRL_SYS_SPI_DEBUG_REGS_SIZE         = (int)0x100,
   PIO_CTRL_SYS_SPI_REGS_OFFSET             = (int)0xF100,
   PIO_CTRL_SYS_SPI_REGS_SIZE               = (int)0xF00,
   MAX_PIO_CTRL_SYS_SPI_MEMORY_MAP          = (int)0xF100
};
typedef enum pio_ctrl_sys_spi_memory_map_enum pio_ctrl_sys_spi_memory_map;


#endif /* __IO_DEFS_H__ */
