# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# gathered into overall lists suitable for the different builds in
# firmware_defs.mk and friends.
# NOTE: don't add the -D here - this is inserted later on your behalf, as it is
# in fact /D for native Windows builds

# Production builds
ifneq ("${PRODUCTION_BUILD}", "")
FIRMWARE_DEFS+=	PRODUCTION_BUILD=${PRODUCTION_BUILD}
endif



# This token is used to identify the P1 processor
FIRMWARE_DEFS += P1_PROCESSOR=1

# All code is for the application subsystem
COMMON_DEFS += SUBSYSTEM_APPS HAVE_32BIT_DATA_WIDTH
SUBSYSTEM_APPS=1
HAVE_32BIT_DATA_WIDTH=1

# Crescendo uses the Oxygen scheduler
COMMON_DEFS += OS_OXYGOS

# USB device support
COMMON_DEFS += ENABLE_USB_DEVICE \
			   HAVE_USB \
			   HOSTIO_USB_HIGH_SPEED_SUPPORT \
			   HOSTIO_USB_MULTIPLE_CONFIGURATION_SUPPORT \
			   HOSTIO_USB_VENDOR_REQ_SUPPORT \
			   HOSTIO_USB_ENDPOINT_OVERFLOW_PROTECTION





# Support for USB device as a host transport
#COMMON_DEFS += TRANSPORT_USB

# Support for UART device as a host transport
FIRMWARE_DEFS += TRANSPORT_USER

# We don't want the P1 build to see this, so we add it for all desktop builds
# and include it in COMMON_DEFS separately in crescendo_p0.mk
DESKTOP_COMMON_DEFS += HOSTIO_USB_HYDRA

# Enable USB VM 
COMMON_DEFS += HOSTIO_USB_VM

# Enable various interfaces in the streams implementation
COMMON_DEFS += INSTALL_USB_STREAM \
			   INSTALL_FILE_MODULE \
			   INSTALL_VM_MODULE



# These macros enable temporary interfaces and stubs that allow running
# modified Bluecore VM applications inside P0
COMMON_DEFS += HOSTIO_USB_VM_STUB DISABLE_EVT

# Streams support
COMMON_DEFS += INSTALL_STREAM_MODULE \
               INSTALL_VM_STREAM_SUPPORT \
               INSTALL_REGION_STREAM \
               INSTALL_IIC_STREAM \
               INSTALL_FILE_STREAM

# Bluestack stream support is not ported yet
COMMON_DEFS += INSTALL_ATT_STREAM \
               INSTALL_L2CAP_STREAM \
               INSTALL_RFCOMM_STREAM








# Enable this to save memory by using Bitfields in structures
FIRMWARE_DEFS += USE_BITFIELDS

# Defining this macro causes the io_defs.h and io_map.h files to only include
# digits modules that are needed. This is controlled by one or more 
# #define IO_DEFS_MODULE_xxx before the include of io/io.h. The module names
# to use can be found in the source documentation for the registers in
# ../src/core/io/crescendo/dev/doc/main.html
FIRMWARE_DEFS += IO_DEFS_LIMIT_MODULES KALIMBA_BUILD

# When only using C code (or if all assembler using other* registers
# is run with interrupts blocked) then we can use much shorter interrupt
# entry/exit code because there are fewer registers to save and restore.
# This definition enables that smaller, faster code.
# * Other registers means registers outside bank 1, arithmetic mode, bit
# reverse on address generator. Assembly code in interrupt must save and
# restore any registers it uses other than the C scratch ones itself. That
# includes the DoLoop registers.
# Non-interrupt code must block interrupts around the use of Bank2 (indexing)
# registers.
COMMON_DEFS += MINIMAL_C_INTERRUPT_FRAME

# We don't need patching on the app subsytem
COMMON_DEFS += HYDRA_SW_PATCH_DISABLE

#Enable the firmware logging
COMMON_DEFS += INSTALL_HYDRA_LOG
COMMON_DEFS += INSTALL_HYDRA_LOG_PER_MODULE_LEVELS

# Enable the timing of shallow sleep so we can get a CPU utlisation
FIRMWARE_DEFS += ENABLE_SHALLOW_SLEEP_TIMING



ifeq ("${FW_ONLY}","")
# Enable various pieces of test code to be built into the 
# firmware. Some is used for unit tests at the point the
# code boots and others add code that is needed by python tests.
FIRMWARE_DEFS += FW_UNIT_TEST
endif

## CRESCENDO SPECIFIC FIRMWARE DEFINES
















## CRESCENDO SPECIFIC FIRMWARE DEFINES END 


















##
## PRESTO SPECIFIC FIRMWARE DEFINES END 
##




# App SS has no Generic Window
COMMON_DEFS += CHIP_HAS_GENERIC_WINDOW_HW=0



# Desktop builds don't use runlevels
DESKTOP_COMMON_DEFS += SCHEDULER_WITHOUT_RUNLEVELS 

DESKTOP_COMMON_DEFS += DESKTOP_TEST_BUILD
DESKTOP_COMMON_DEFS += DEBUG_BUFFER
# Submsg support for local messages delivery
DESKTOP_COMMON_DEFS += SUBMSG_LOCAL_DELIVERY

# Allow the submsg functionality to simulate two subsystems
DESKTOP_COMMON_DEFS += SUBMSG_LOCAL_DELIVERY

DESKTOP_COMMON_DEFS +=	CHIP_HAS_SHARED_RAM=0 \
        CHIP_HAS_BUS_MESSAGE_STATUS_REGISTER=0 \
		MMU_WINDOW_DM_ADDRESS=0 \
		MMU_WINDOW_ADDR_BITS_PER_BUFFER=0 \
		PROC_RAM_BEGIN=0 \
		NO_PMALLOC_MEMMAP_SYMBOLS \
		ENABLE_SCHED_MSG_CONTEXTS


SCHED_NO_EXTERNAL_MSG_SUPPORT=1



# Add the 8051 test code into the build
#FIRMWARE_DEFS+=	INSTALL_8051_PIO_CTRL_TEST
#INSTALL_8051_PIO_CTRL_TEST=	1

# Don't allow the scheduler's context switching to make timed event handlers
# pre-empt or be pre-empted by task handlers.  Also, ensure timed event handlers
# _do_ pre-empt lower-priority tasks if necessary.
COMMON_DEFS += SCHED_NO_TIMER_PREEMPTION

# Extra defs for cov to get hw bits to compile
COV_DEFS=	CHIP_HAS_SHARED_RAM=0 \
		MMU_WINDOW_DM_ADDRESS=0 \
		MMU_WINDOW_ADDR_BITS_PER_BUFFER=0 \
		PROC_RAM_BEGIN=0 \
		NO_PMALLOC_MEMMAP_SYMBOLS \
		THIS_PROCESSOR=0

# Enable the code to be built for cov but not for unit test
COV_DEFS += COV_TEST_BUILD


FIRMWARE_DEFS += BIGINT_64BIT_INTRINSIC










# \todo : Remove this under B-165701
BT_DEFS += BLUESTACK_HOST_IS_APPS

# Build a cache test function that takes approx 800kbytes of code space
# to test cache misses. Left disabled by default because of the large code
# size. 
# This needs a makefile flag and a C preprocessor flag (they should both
# be enabled or both be commented out)
#FIRMWARE_DEFS += ENABLE_CACHE_TEST_LARGE_CODE
#ENABLE_CACHE_TEST_LARGE_CODE=1




#FIRMWARE_DEFS += PMALLOC_DONT_COMBINE_POOLS
#FIRMWARE_DEFS += PMALLOC_FIXED_CONFIG_ONLY

# Enable pmalloc PC trace
FIRMWARE_DEFS += PMALLOC_TRACE_OWNER_PC_ONLY

# Record the requested size for all allocated blocks
FIRMWARE_DEFS += PMALLOC_RECORD_LENGTHS

# Record the total number of requests for blocks of different size. Only on
# Crescendo due to memory limitations on Aura.
CRESCENDO_FIRMWARE_DEFS += PMALLOC_RECORD_REQUESTS

# Log the number of over-allocations and max num of each block size allocated
FIRMWARE_DEFS += PMALLOC_STATS

# Log current and max memory 'out'
#FIRMWARE_DEFS += PMALLOC_RECORD_USAGE_LEVEL









# Debug the sleep state on a PIO (high is asleep) for each processor
ifneq (,$(findstring PROCESSOR_P0,$(COMMON_DEFS)))
#FIRMWARE_DEFS += DEBUG_SLEEP_ON_PIO=4
else
#FIRMWARE_DEFS += DEBUG_SLEEP_ON_PIO=28
endif

# Local integer types file for inclusion in 
# common/interface/app/bluestack/types.h
COMMON_DEFS += BLUESTACK_TYPES_INT_TYPE_FILE="bt/bt/bluestack_types.h"

# Things we don't want from the usb_device_class App
COMMON_DEFS += USB_DEVICE_CLASS_REMOVE_MASS_STORAGE \
               USB_DEVICE_CLASS_REMOVE_CHARGING





COMMON_DEFS += FSM_NAME_LOGGING FSM_DEBUG_NAMES HYDRA_FSM_DEBUG











# Support SHA256 
COMMON_DEFS += INSTALL_SHA256

# Enables boot time copy of the first 32KB of firmware from SQIF into TCM
# and then remapping TCM over the start of SQIF. This can be enabled separately
# for P0 and P1. This needs to be enabled for the Apps SQIF access (other than
# code execution) to work - e.g. writable filesystem, PSKEYs, etc. 
FIRMWARE_DEFS += COPY_P0_CODE_INTO_TCM
# Disabled for P1 in d00 as P1 crashes when it is enabled. 
# Enabled on d01 in file d01_defs.mk. 
#FIRMWARE_DEFS += COPY_P1_CODE_INTO_TCM




# Number of bytes to leave at the end of the stack to protect us against 
# overrunning and give a bit of stack left to use when reporting
# the overrun. A margin of 256 leaves approximately 20 bytes of stack
# unused in the stack overrun test.
FIRMWARE_DEFS += STACK_OVERRUN_PROTECTION_BYTES=256

## AURA SPECIFIC FIRMWARE DEFINES














# Aura debug log buffer size (must be a power of 2)
AURA_FIRMWARE_DEFS += HYDRA_LOG_BUF_SIZE=256

# Aura char log buffer size (must be a power of 2)
AURA_FIRMWARE_DEFS += HYDRA_LOG_CHAR_BUF_SIZE=512








## AURA SPECIFIC FIRMWARE DEFINES END





# Defining this along with PMALLOC_BLOCK_OVERRUN_CHECK means
# that instead of allocating double the memory, xpmalloc will allocate the
# memory requested plus extra PMALLOC_OVERRUN_SIZE octets and fill the 
# surplus memory with a 'guard' value. When freeing this memory a check is
# made that the 'guard' values haven't been overwritten.
# This define will have no effect if PMALLOC_BLOCK_OVERRUN_CHECK is disabled
#COMMON_DEFS += PMALLOC_BLOCK_OVERRUN_CHECK_SHORT

