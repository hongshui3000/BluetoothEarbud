# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
include ../config/kcc_compiler_base.mk
KCC_BIN_DIR=	${KCC_DIR}/bin
KCC_INC_DIR=	${KCC_DIR}/include
KCC_INCLUDES=	${KCC_INC_DIR} ${KCC_INC_DIR}/target32
CC = ${KCC_BIN_DIR}/kcc
ASM = ${KCC_BIN_DIR}/kas
LD = ${KCC_BIN_DIR}/kld
AR = ${KCC_BIN_DIR}/kar
KALELF2MEM = ${KCC_BIN_DIR}/kalelf2mem
MAPCMD = ${KCC_BIN_DIR}/kmapper datadump symlist
OBJDUMP = ${KCC_BIN_DIR}/kobjdump

# Derived stuff for the compiler
LIB_DIR = ${KCC_DIR}/lib/${KCC_CHIP_NAME}
LIBS = ${LIB_DIR}/libc.a ${LIB_DIR}/libfle.a ${LIB_DIR}/libcrt.a

# Set this option to -minim or to nothing to enable/disable minim code
MINIM_OPT=-minim

ASMDEPGENFLAGS=	-MD -MF ${@:%.o=%.d}
# Lint doesn't look at asm files, which is just as well
# as kas doesn't understand -MT
DEPGENFLAGS=	${ASMDEPGENFLAGS} -MT '$@ ${@:%.o=%.lob} ${@:%.o=%.lint.out}'
# Create a compiler command line option to force include the definitions
DEFS = -preinclude ${BUILD_DEFS_HEADER}
ASM_DEFS=	-include ${BUILD_DEFS_HEADER}
ASM_OPTIONS = ${CHIP_NAME_OPT} -g ${ASM_DEFS} ${INC}

WARNING_OPTS = -Wall -WAccuracyLoss -WnoPointerUnaligned -WnoConstantTest

# Suppress the -WAccuracyLoss for now for the IP stack code. We 
# should re-enable it if kcc becomes as forgiving as gcc.
WLAN_WARNING_OPTS += -WnoAccuracyLoss
# Similarly disable it for Bluestack
BT_WARNING_OPTS += -WnoAccuracyLoss -WnoArgSizeLarger -WnoUnsignedCompare

# The anonymous union member is needed for assigning an LWIP const pointer
# to an elemnet in the p_buf structure to avoid a compiler warning. 
# Suppressing this is less hazardous than suppressing the 
# discarded qualifier warnings. 
WLAN_WARNING_OPTS += -WnoAnonymousMember

ifeq ("${DAILY_BUILD}", "yes")
# For daily build we want warnings to be errors so that Jenkins and
# the make_all.py script pick them up
COMMON_CFLAGS+=	-Werror
endif

CFLAGS = ${COMMON_CFLAGS} -g ${MINIM_OPT} -c -Xcompiler -o2 -Xassembler -apply-b179745-workaround ${WARNING_OPTS}
CFLAGS_WLAN = ${CFLAGS} ${WLAN_WARNING_OPTS} 
CFLAGS_BT = ${CFLAGS} ${BT_WARNING_OPTS}
PPFLAGS = ${CHIP_NAME_OPT} -E -I. ${INC} ${DEFS}
LD_OPTIONS = --relax -EL -L ${BUILD_DIR}/../../config/tool

# Add this to the CFLAGS to dump out a list of what files are being included
# to debug errors in headers
# e.g. CFLAGS += ${EXTRA_CFLAGS_FOR_DEBUGGING_INCLUDES}
EXTRA_CFLAGS_FOR_DEBUGGING_INCLUDES = -Xc --*.anc0:report_files

DEFINE_FLAG = -D
CCOUT = -o $@
LDOUTFLAG = -o
INCLUDE_FLAG = -I
