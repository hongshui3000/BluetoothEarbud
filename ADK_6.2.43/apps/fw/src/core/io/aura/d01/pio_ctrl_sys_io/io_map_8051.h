
/* *************************************************************************  *
   COMMERCIAL IN CONFIDENCE
   Copyright (C) 2017 Qualcomm Technologies International Ltd.

   Qualcomm Technologies International Ltd.,
   Churchill House,
   Cambridge Business Park,
   Cowley Park,
   Cambridge, CB4 0WZ.
   UK
   http://www.csr.com

   $Id: //depot/bg/Releases/ADK/ADK6.2/hydra_os_aura/fw/src/core/io/aura/d01/pio_ctrl_sys_io/io_map_8051.h#1 $
   $Name$
   $Source$

   DESCRIPTION
      Hardware declarations header file (lower level).
      Lists memory mapped register addresses.

   MODIFICATIONS
      1.0    12/04/99    RWY    First created.
      1.x    xx/xx/xx    RWY    Automatically generated.

*  *************************************************************************  */

#ifndef __IO_MAP_8051_H__
#define __IO_MAP_8051_H__


/* -- Mildred 8051 special function registers -- */

/* Declarations of read/write or mixed registers */
volatile __sfr  __at (0xdc) CLKGEN_ACTIVE_CLOCK_RATE;
volatile __sfr  __at (0xdb) CLKGEN_ACTIVITY_ENABLES;
volatile __sfr  __at (0xd4) CLKGEN_DEBUG_SELECT;
volatile __sfr  __at (0xd7) CLKGEN_ENABLES;
volatile __sfr  __at (0xd3) CLKGEN_MAX_CLOCK_RATE;
volatile __sfr  __at (0xd2) CLKGEN_MIN_CLOCK_RATE;
volatile __sfr  __at (0xd1) CLKGEN_TIMER_ENABLES;
volatile __sfr  __at (0xfe) JANITOR_MAILBOX_OUT;
volatile __sfr  __at (0xdf) MILDRED_DEEP_SLEEP_EN;
volatile __sfr  __at (0xb1) MILDRED_INT_EN;
volatile __sfr  __at (0xb3) MILDRED_INT_PRIORITIES;
volatile __sfr  __at (0xb6) MILDRED_INT_PRIORITIES2;
volatile __sfr  __at (0xb5) MILDRED_INT_SOURCES2_EN;
volatile __sfr  __at (0xb2) MILDRED_INT_SOURCES_EN;
volatile __sfr  __at (0xc0) MILDRED_INT_STATUS;
volatile __sfr  __at (0xd8) MILDRED_INT_STATUS2;
volatile __sfr  __at (0xb4) MILDRED_INT_VECTOR_HI;
volatile __sfr  __at (0x80) MILDRED_P0;
volatile __sbit __at (0x80) MILDRED_P0_0;
volatile __sbit __at (0x81) MILDRED_P0_1;
volatile __sbit __at (0x82) MILDRED_P0_2;
volatile __sbit __at (0x83) MILDRED_P0_3;
volatile __sbit __at (0x84) MILDRED_P0_4;
volatile __sbit __at (0x85) MILDRED_P0_5;
volatile __sbit __at (0x86) MILDRED_P0_6;
volatile __sbit __at (0x87) MILDRED_P0_7;
volatile __sfr  __at (0x91) MILDRED_P0_DRIVE_EN;
volatile __sfr  __at (0x90) MILDRED_P1;
volatile __sbit __at (0x90) MILDRED_P1_0;
volatile __sbit __at (0x91) MILDRED_P1_1;
volatile __sbit __at (0x92) MILDRED_P1_2;
volatile __sbit __at (0x93) MILDRED_P1_3;
volatile __sbit __at (0x94) MILDRED_P1_4;
volatile __sbit __at (0x95) MILDRED_P1_5;
volatile __sbit __at (0x96) MILDRED_P1_6;
volatile __sbit __at (0x97) MILDRED_P1_7;
volatile __sfr  __at (0x92) MILDRED_P1_DRIVE_EN;
volatile __sfr  __at (0xa0) MILDRED_P2;
volatile __sbit __at (0xa0) MILDRED_P2_0;
volatile __sbit __at (0xa1) MILDRED_P2_1;
volatile __sbit __at (0xa2) MILDRED_P2_2;
volatile __sbit __at (0xa3) MILDRED_P2_3;
volatile __sbit __at (0xa4) MILDRED_P2_4;
volatile __sbit __at (0xa5) MILDRED_P2_5;
volatile __sbit __at (0xa6) MILDRED_P2_6;
volatile __sbit __at (0xa7) MILDRED_P2_7;
volatile __sfr  __at (0x93) MILDRED_P2_DRIVE_EN;
volatile __sfr  __at (0xb0) MILDRED_P3;
volatile __sbit __at (0xb0) MILDRED_P3_0;
volatile __sbit __at (0xb1) MILDRED_P3_1;
volatile __sbit __at (0xb2) MILDRED_P3_2;
volatile __sbit __at (0xb3) MILDRED_P3_3;
volatile __sbit __at (0xb4) MILDRED_P3_4;
volatile __sbit __at (0xb5) MILDRED_P3_5;
volatile __sbit __at (0xb6) MILDRED_P3_6;
volatile __sbit __at (0xb7) MILDRED_P3_7;
volatile __sfr  __at (0x94) MILDRED_P3_DRIVE_EN;
volatile __sfr  __at (0xe1) MILDRED_PIO_BANK_SELECT;
volatile __sfr  __at (0xe2) MILDRED_PIO_SELECT_0300;
volatile __sfr  __at (0xe3) MILDRED_PIO_SELECT_0704;
volatile __sfr  __at (0xe4) MILDRED_PIO_SELECT_1108;
volatile __sfr  __at (0xe5) MILDRED_PIO_SELECT_1512;
volatile __sfr  __at (0xe6) MILDRED_PIO_SELECT_1916;
volatile __sfr  __at (0xe7) MILDRED_PIO_SELECT_2320;
volatile __sfr  __at (0xe9) MILDRED_PIO_SELECT_2724;
volatile __sfr  __at (0xea) MILDRED_PIO_SELECT_3128;
volatile __sfr  __at (0xf1) MILDRED_PIO_SELECT_TMRCTR0;
volatile __sfr  __at (0xf2) MILDRED_PIO_SELECT_TMRCTR1;
volatile __sfr  __at (0xf3) MILDRED_PIO_SELECT_TMRCTR2;
volatile __sfr  __at (0xeb) PIO_CTRL_IMEM_WABLM;
volatile __sfr  __at (0xfb) PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CLEAR;
volatile __sfr  __at (0xfc) PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_CODE;
volatile __sfr  __at (0xfd) PIO_CTRL_JANITOR_MAILBOX_WATCHDOG_ENABLE;

/* Declarations of read only registers */
volatile __sfr  __at (0xdd) CLKGEN_ACTIVITY_STATUS;
volatile __sfr  __at (0xd9) CLKGEN_CLOCK_RATE_STATUS_HI;
volatile __sfr  __at (0xda) CLKGEN_CLOCK_RATE_STATUS_LO;
volatile __sfr  __at (0xde) CLKGEN_STATUS;
volatile __sfr  __at (0xd5) CLKGEN_TIMER_FAST_STATUS_HI;
volatile __sfr  __at (0xd6) CLKGEN_TIMER_FAST_STATUS_LO;
volatile __sfr  __at (0xff) JANITOR_MAILBOX_IN;
volatile __sfr  __at (0xe8) MILDRED_INT_UNMASKED_STATUS;
volatile __sfr  __at (0xf8) MILDRED_INT_UNMASKED_STATUS2;
volatile __sfr  __at (0xa9) MILDRED_TIMER_TIME_0700;
volatile __sfr  __at (0xaa) MILDRED_TIMER_TIME_1508;
volatile __sfr  __at (0xab) MILDRED_TIMER_TIME_2316;
volatile __sfr  __at (0xac) MILDRED_TIMER_TIME_3124;


/* -- PIO controller subsystem PIO control and status registers -- */

/* Declarations of read/write or mixed registers */
volatile __xdata __at (0xf3ba) unsigned char PIOCTRL_RAM0_EMA;
volatile __xdata __at (0xf370) unsigned char PIO_CTRL_SYS_PIO_EVENT_CLEAR;
volatile __xdata __at (0xf34c) unsigned char PIO_CTRL_SYS_PIO_EVENT_FALLING_ENABLE;
volatile __xdata __at (0xf340) unsigned char PIO_CTRL_SYS_PIO_EVENT_RISING_ENABLE;
volatile __xdata __at (0xf3bc) unsigned char PIO_SYS_RAM_SEQ_TIMING_REG0;
volatile __xdata __at (0xf3be) unsigned char PIO_SYS_RAM_SEQ_TIMING_REG1;
volatile __xdata __at (0xf3c0) unsigned char PIO_SYS_RAM_SEQ_TIMING_REG2;

/* Declarations of read only registers */
volatile __xdata __at (0xf364) unsigned char PIO_CTRL_SYS_PIO_EVENT_FALLING_CAUSE;
volatile __xdata __at (0xf358) unsigned char PIO_CTRL_SYS_PIO_EVENT_RISING_CAUSE;


/* -- Common subsystem level registers -- */

/* Declarations of read/write or mixed registers */
volatile __xdata __at (0xf332) unsigned char SUB_SYS_DEBUG_SELECT;
volatile __xdata __at (0xf326) unsigned char SUB_SYS_MAILBOX0;
volatile __xdata __at (0xf328) unsigned char SUB_SYS_MAILBOX1;
volatile __xdata __at (0xf32a) unsigned char SUB_SYS_MAILBOX2;
volatile __xdata __at (0xf32c) unsigned char SUB_SYS_MAILBOX3;
volatile __xdata __at (0xf324) unsigned char SUB_SYS_MISC_ENABLES;
volatile __xdata __at (0xf320) unsigned char SUB_SYS_P0_INTERRUPT1;
volatile __xdata __at (0xf322) unsigned char SUB_SYS_P0_INTERRUPT2;
volatile __xdata __at (0xf330) unsigned char SUB_SYS_RST_STATUS;
volatile __xdata __at (0xf338) unsigned char SUB_SYS_TRANSACTION_MUX_DBG_SEL;

/* Declarations of read only registers */
volatile __xdata __at (0x1fd02) unsigned char SUB_SYS_DEBUGGER_REF_ID;
volatile __xdata __at (0xf33a) unsigned char SUB_SYS_CLK_MMU_COUNT;
volatile __xdata __at (0xf334) unsigned char SUB_SYS_DEBUG_STATUS;
volatile __xdata __at (0xf32e) unsigned char SUB_SYS_ID;
volatile __xdata __at (0x1ff34) unsigned char SUB_SYS_LEGACY_CHIP_VERSION;
volatile __xdata __at (0x1f4c2) unsigned char SUB_SYS_REG_SRC_HASH;


/* -- PIO controller subsystem debug registers -- */

/* Declarations of read/write or mixed registers */
volatile __xdata __at (0xf010) unsigned char PIO_CTRL_DEBUG;
volatile __xdata __at (0xf002) unsigned char PIO_CTRL_DEBUG_BREAK_ADDR0;
volatile __xdata __at (0xf004) unsigned char PIO_CTRL_DEBUG_BREAK_ADDR1;
volatile __xdata __at (0xf006) unsigned char PIO_CTRL_DEBUG_BREAK_ADDR2;
volatile __xdata __at (0xf000) unsigned char PIO_CTRL_DEBUG_BREAK_EN;

/* Declarations of read only registers */
volatile __xdata __at (0xf008) unsigned char PIO_CTRL_DEBUG_PC;
volatile __xdata __at (0xf00c) unsigned char PIO_CTRL_DEBUG_STATE;
volatile __xdata __at (0xf00a) unsigned char PIO_CTRL_DEBUG_STOPPED;
volatile __xdata __at (0xf00e) unsigned char PIO_CTRL_DEBUG_STOP_COUNT;


/* -- Generic Watchdog registers -- */

/* Declarations of read/write or mixed registers */
volatile __xdata __at (0xf380) unsigned char WATCHDOG_DELAY;
volatile __xdata __at (0xf384) unsigned char WATCHDOG_DISABLE;
volatile __xdata __at (0xf386) unsigned char WATCHDOG_ENABLE;
volatile __xdata __at (0xf388) unsigned char WATCHDOG_KICK;

/* Declarations of read only registers */
volatile __xdata __at (0xf38a) unsigned char WATCHDOG_KICK_PENDING;
volatile __xdata __at (0xf38e) unsigned char WATCHDOG_SLOW_KICK_PENDING;
volatile __xdata __at (0xf38c) unsigned char WATCHDOG_STATUS;


/* -- Auto-generated from subsystem memory XML. -- */

/* Declarations of read/write or mixed registers */
volatile __xdata __at (0xf3a6) unsigned char RAM_CTRL_PMEM_1024X16_ASLEEP_DS;
volatile __xdata __at (0xf3a8) unsigned char RAM_CTRL_PMEM_1024X16_ASLEEP_LS;
volatile __xdata __at (0xf3aa) unsigned char RAM_CTRL_PMEM_1024X16_ASLEEP_SD;
volatile __xdata __at (0xf3a0) unsigned char RAM_CTRL_PMEM_1024X16_AWAKE_DS;
volatile __xdata __at (0xf3a2) unsigned char RAM_CTRL_PMEM_1024X16_AWAKE_LS;
volatile __xdata __at (0xf3a4) unsigned char RAM_CTRL_PMEM_1024X16_AWAKE_SD;


/* -- Interrupt transaction processing config and status registers -- */

/* Declarations of read/write or mixed registers */
volatile __xdata __at (0xf306) unsigned char BUS_INT_CLEAR;
volatile __xdata __at (0xf302) unsigned char BUS_INT_CONFIG;
volatile __xdata __at (0xf304) unsigned char BUS_INT_MASK;
volatile __xdata __at (0xf300) unsigned char BUS_INT_SELECT;
volatile __xdata __at (0xf30e) unsigned char BUS_INT_SEND_INT_CONFIG;
volatile __xdata __at (0xf310) unsigned char BUS_INT_SEND_INT_STATUS_FIELD;

/* Declarations of read only registers */
volatile __xdata __at (0xf30a) unsigned char BUS_INT_CONFIG_STATUS;
volatile __xdata __at (0xf30c) unsigned char BUS_INT_MASK_STATUS;
volatile __xdata __at (0xf312) unsigned char BUS_INT_SEND_INT_SEND_STATUS;
volatile __xdata __at (0xf308) unsigned char BUS_INT_STATUS;


/* -- Mildred 8051 special function registers (reset by all post boot resets) -- */

/* Declarations of read/write or mixed registers */
volatile __sfr  __at (0xe0) MILDRED_ACC;
volatile __sbit __at (0xe0) MILDRED_ACC_0;
volatile __sbit __at (0xe1) MILDRED_ACC_1;
volatile __sbit __at (0xe2) MILDRED_ACC_2;
volatile __sbit __at (0xe3) MILDRED_ACC_3;
volatile __sbit __at (0xe4) MILDRED_ACC_4;
volatile __sbit __at (0xe5) MILDRED_ACC_5;
volatile __sbit __at (0xe6) MILDRED_ACC_6;
volatile __sbit __at (0xe7) MILDRED_ACC_7;
volatile __sfr  __at (0xf0) MILDRED_B;
volatile __sbit __at (0xf0) MILDRED_B_0;
volatile __sbit __at (0xf1) MILDRED_B_1;
volatile __sbit __at (0xf2) MILDRED_B_2;
volatile __sbit __at (0xf3) MILDRED_B_3;
volatile __sbit __at (0xf4) MILDRED_B_4;
volatile __sbit __at (0xf5) MILDRED_B_5;
volatile __sbit __at (0xf6) MILDRED_B_6;
volatile __sbit __at (0xf7) MILDRED_B_7;
volatile __sfr  __at (0x83) MILDRED_DPH;
volatile __sfr  __at (0x82) MILDRED_DPL;
volatile __sfr  __at (0xa8) MILDRED_IE;
volatile __sbit __at (0xa8) MILDRED_IE_EX0;
volatile __sbit __at (0xa9) MILDRED_IE_ET0;
volatile __sbit __at (0xaa) MILDRED_IE_EX1;
volatile __sbit __at (0xab) MILDRED_IE_ET1;
volatile __sbit __at (0xac) MILDRED_IE_ES;
volatile __sbit __at (0xad) MILDRED_IE_ET2;
volatile __sbit __at (0xae) MILDRED_IE_RSVD_6;
volatile __sbit __at (0xaf) MILDRED_IE_EA;
volatile __sfr  __at (0xb8) MILDRED_IP;
volatile __sbit __at (0xb8) MILDRED_IP_PX0;
volatile __sbit __at (0xb9) MILDRED_IP_PT0;
volatile __sbit __at (0xba) MILDRED_IP_PX1;
volatile __sbit __at (0xbb) MILDRED_IP_PT1;
volatile __sbit __at (0xbc) MILDRED_IP_PS;
volatile __sbit __at (0xbd) MILDRED_IP_PT2;
volatile __sbit __at (0xbe) MILDRED_IP_RSVD_6;
volatile __sbit __at (0xbf) MILDRED_IP_RSVD_7;
volatile __sfr  __at (0x87) MILDRED_PCON;
volatile __sfr  __at (0xd0) MILDRED_PSW;
volatile __sbit __at (0xd0) MILDRED_PSW_P;
volatile __sbit __at (0xd1) MILDRED_PSW_USER;
volatile __sbit __at (0xd2) MILDRED_PSW_OV;
volatile __sbit __at (0xd3) MILDRED_PSW_RS0;
volatile __sbit __at (0xd4) MILDRED_PSW_RS1;
volatile __sbit __at (0xd5) MILDRED_PSW_F0;
volatile __sbit __at (0xd6) MILDRED_PSW_AC;
volatile __sbit __at (0xd7) MILDRED_PSW_CY;
volatile __sfr  __at (0xcb) MILDRED_RCAP2H;
volatile __sfr  __at (0xca) MILDRED_RCAP2L;
volatile __sfr  __at (0xa7) MILDRED_READ_WRITE_INTERNAL_INT_STATE;
volatile __sfr  __at (0x99) MILDRED_SBUF;
volatile __sfr  __at (0x98) MILDRED_SCON;
volatile __sbit __at (0x98) MILDRED_SCON_RI;
volatile __sbit __at (0x99) MILDRED_SCON_TI;
volatile __sbit __at (0x9a) MILDRED_SCON_RB8;
volatile __sbit __at (0x9b) MILDRED_SCON_TB8;
volatile __sbit __at (0x9c) MILDRED_SCON_REN;
volatile __sbit __at (0x9d) MILDRED_SCON_SM2;
volatile __sbit __at (0x9e) MILDRED_SCON_SM1;
volatile __sbit __at (0x9f) MILDRED_SCON_SM0;
volatile __sfr  __at (0x86) MILDRED_SET_PC_HI;
volatile __sfr  __at (0x85) MILDRED_SET_PC_LO;
volatile __sfr  __at (0x9d) MILDRED_SIU_SDIV;
volatile __sfr  __at (0x81) MILDRED_SP;
volatile __sfr  __at (0xc8) MILDRED_T2CON;
volatile __sbit __at (0xc8) MILDRED_TCON_CPRL2;
volatile __sbit __at (0xc9) MILDRED_TCON_CT2;
volatile __sbit __at (0xca) MILDRED_TCON_TR2;
volatile __sbit __at (0xcb) MILDRED_TCON_EXEN2;
volatile __sbit __at (0xcc) MILDRED_TCON_TCLK;
volatile __sbit __at (0xcd) MILDRED_TCON_RCLK;
volatile __sbit __at (0xce) MILDRED_TCON_EXF2;
volatile __sbit __at (0xcf) MILDRED_TCON_TF2;
volatile __sfr  __at (0x88) MILDRED_TCON;
volatile __sbit __at (0x88) MILDRED_TCON_IT0;
volatile __sbit __at (0x89) MILDRED_TCON_IE0;
volatile __sbit __at (0x8a) MILDRED_TCON_IT1;
volatile __sbit __at (0x8b) MILDRED_TCON_IE1;
volatile __sbit __at (0x8c) MILDRED_TCON_TR0;
volatile __sbit __at (0x8d) MILDRED_TCON_TF0;
volatile __sbit __at (0x8e) MILDRED_TCON_TR1;
volatile __sbit __at (0x8f) MILDRED_TCON_TF1;
volatile __sfr  __at (0x8c) MILDRED_TH0;
volatile __sfr  __at (0x8d) MILDRED_TH1;
volatile __sfr  __at (0xcd) MILDRED_TH2;
volatile __sfr  __at (0x9c) MILDRED_TIMER_COUNTER_PRESCALE_FRAC;
volatile __sfr  __at (0x9b) MILDRED_TIMER_COUNTER_PRESCALE_INT;
volatile __sfr  __at (0x8a) MILDRED_TL0;
volatile __sfr  __at (0x8b) MILDRED_TL1;
volatile __sfr  __at (0xcc) MILDRED_TL2;
volatile __sfr  __at (0x89) MILDRED_TMOD;

/* Declarations of read only registers */
volatile __sfr  __at (0xa6) MILDRED_PROGRAM_COUNTER_HI;
volatile __sfr  __at (0xa5) MILDRED_PROGRAM_COUNTER_LO;


#endif /* __IO_MAP_8051_H__ */
