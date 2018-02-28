# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# The fault code uses this token to indicate which processor a fault has
# occurred on
FIRMWARE_DEFS += THIS_PROCESSOR=P1_PROCESSOR

# Symbol for setting processor number in the debugger project file
PROCESSOR_NUM=1

# Enable logging of IRQs
FIRMWARE_DEFS += LOG_IRQS

# Enable firmware logging of putchar and printf characters
FIRMWARE_DEFS += FIRMWARE_PUTCHAR_LOG

# Enable TRB logging
FIRMWARE_DEFS += TRB_FIRMWARE_LOG

# Enable checking that pmalloc'ed memory isn't free'd more than once.
# Temporarily enabled for debugging purposes.
FIRMWARE_DEFS += PFREE_CHECK_ALREADY_FREED

# Defining this means that xpmalloc will malloc
# double the memory requested and fill the surplus
# memory with a 'guard' value. When freeing this
# memory a check is made that the 'guard' values haven't
# been overwritten.
FIRMWARE_DEFS += PMALLOC_BLOCK_OVERRUN_CHECK
FIRMWARE_DEFS += PMALLOC_BLOCK_OVERRUN_CHECK_SHORT

