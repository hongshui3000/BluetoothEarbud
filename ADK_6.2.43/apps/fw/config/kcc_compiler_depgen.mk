# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Contains macros to be used for running KCC as a standalone dependency
# generator
include ../config/kcc_compiler_base.mk

KCC = ${KCC_DIR}/bin/kcc

# When we generate dependencies as a separate step, the .c file is the 
# prerequisite and the .d file is the target.  But we want the dependency rule 
# we generate to make the .o file the target

DEFS = -preinclude ${BUILD_DEFS_HEADER}
SYSTEM_FLAGS = ${SYSTEM_DEPS:%=-D%} ${SYSTEM_INCLUDE:${INCLUDE_FLAG}%=-I%}
KCC_MISC_DEPGEN_FLAGS =  -DKCC_DEPENDENCY_GEN -DBLUESTACK_HOST_IS_APPS -Wnone
KCC_STANDALONE_DEPGEN_FLAGS = -MD -MF ${<:%.c=${BUILD_DIR}/%.d} -MT ${<:%.c=${BUILD_DIR}/%.o}

KCC_DEPGEN_FLAGS = ${KCC_MISC_DEPGEN_FLAGS} ${KCC_STANDALONE_DEPGEN_FLAGS} ${SYSTEM_FLAGS}

# It seems that KCC needs to have something to do other than generate 
# dependencies.  We'll pick preprocessing as it's the quickest, but throw the 
# result away.
KCC_NULLACTION = -E -o ${NULL}

# Need to define this to provide a set of targets that can force header
# autogeneration to take place before dependency generation
DEPFILES = ${OBJECTS:%.o=%.d}
