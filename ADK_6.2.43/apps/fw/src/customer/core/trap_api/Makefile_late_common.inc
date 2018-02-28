# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Makefile fragment providing the trap API-related variables and rules that are
# common to both the normal trap_api module build and the ipc build, which just
# requires the trap_api headers

CCORE_BUILD_DIR = ${BUILD_DIR}/${CCORE_DIR}
API_BUILD_DIR = ${BUILD_DIR}/${API_DIR}

API_GEN_BUILD_DIR = ${API_BUILD_DIR}/gen

# The trap API headers are supposed to be available without a module path
INC += -I${API_DIR} -I${API_BUILD_DIR}

# Also allow trap API header inclusion with the module path.
# Visual Studio adds the current compilation unit's directory to the include
# path by default which can cause name collisions when using pathless includes.
INC += -I${CCORE_DIR} -I${CCORE_BUILD_DIR}
