# Copyright (c) 2017 Qualcomm Technologies International, Ltd.
#   %%version
# Definitions used by the Makefile common to all aura variants/revisions

# Digits state machine to stop the clocks cleanly when entering shallow sleep.
# Aura digits has it on by default so firmware won't need to enable it.  
COMMON_DEFS+= CHIP_HAS_CLEAN_SHALLOW_SLEEP_CLOCK_STOP_THAT_DEFAULTS_OFF=1



# Set the number of bitserials available
COMMON_DEFS += HAVE_NUMBER_OF_BITSERIALS=2

# Compiler switch for the bitserial
COMMON_DEFS += CHIP_INSTALL_BITSERIAL



# Identify hydra bitserial
FIRMWARE_DEFS += HYDRA_BITSERIAL



# The memory protection hardware was completely rewritten for Crescendo D01
COMMON_DEFS += HAVE_P1_ACCESS_CONTROL

# Some chips have registers for avoiding race conditions when going into
# the shallow sleep state (CPU clocks stopped).
COMMON_DEFS += CHIP_HAS_SHALLOW_SLEEP_REGISTERS=1

# Some chips have a bus_proc_stream digital hardware block that allows
# the second processor to generate transactions that can be used for
# sending data to the host through the debugger (e.g. firmware log). 
COMMON_DEFS += CHIP_HAS_BUS_PROC_STREAM_MODULE=1



# Aura does not have aux adc
COMMON_DEFS += CHIP_HAS_AUX_DATA_CONV=0

# Aura does not have NFC
COMMON_DEFS += CHIP_HAS_NFC=0

# Enables boot time copy of the first 32KB of firmware from SQIF into TCM
# and then remapping TCM over the start of SQIF. Enabled for P1 here.
FIRMWARE_DEFS += COPY_P1_CODE_INTO_TCM











#Aura does not have SDIO device support and 1 slot
FIRMWARE_DEFS += SDIOEMB_SLOTS_MAX=NUMBER_OF_SD_SLOTS 
FIRMWARE_DEFS += SDIOEMB_FDRIVERS_MAX=1 SDIOEMB_FDEVS_MAX=1

#Add SD Host test hooks to allow for additional tests
#FIRMWARE_DEFS += INSTALL_SD_HOST_TEST_HOOKS


# (2) cap ds to 16 MHz - DS 16 MHz(XTAL) DS 10 MHz(SPLL) (cadence divider is 2n)
FIRMWARE_DEFS += SDIO_CLOCK_FREQ_NORMAL_SPD=16000000 SDIO_CLOCK_FREQ_MMC_26MHZ=16000000
# (3) cap HS to 32 MHz - HS 32 MHz(XTAL) HS 16 MHz(SPLL) (cadence divider is 2n)
FIRMWARE_DEFS += SDIO_CLOCK_FREQ_HIGH_SPD=32000000 SDIO_CLOCK_FREQ_MMC_52MHZ=32000000





