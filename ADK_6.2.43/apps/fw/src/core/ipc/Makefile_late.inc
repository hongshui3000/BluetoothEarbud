# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Makefile containing the rule to autogenerate the trap API headers

include customer/core/trap_api/Makefile_late_common.inc

IPC_GEN_BUILD_DIR = ${BUILD_DIR}/core/ipc/gen
IPC_TRAP_SIGNALS = $(call FIXPATH, ${IPC_GEN_BUILD_DIR}/ipc_trap_api_signals.h)
IPC_TRAP_PRIMS = $(call FIXPATH, ${IPC_GEN_BUILD_DIR}/ipc_trap_api_prims.h)
IPC_TRAP_SLOW_PRIM_CALLS = $(call FIXPATH, ${IPC_GEN_BUILD_DIR}/ipc_trap_api_recv_slow.c)
IPC_TRAP_FAST_PRIM_CALLS = $(call FIXPATH, ${IPC_GEN_BUILD_DIR}/ipc_trap_api_recv_fast.c)
IPC_TRAP_FAST_PRIM_CASES = $(call FIXPATH, ${IPC_GEN_BUILD_DIR}/ipc_trap_api_recv_fast_cases.c)

# Because the trap header autogeneration causes every file to #include api.h,
# everything will depend on it, and therefore everything depending on the trap
# API will be out-of-date if the API XML changes
API_HEADERS_TARGET = $(call FIXPATH, ${API_BUILD_DIR}/api.h)

${API_HEADERS_TARGET} : ${API_XML} ${API_CODEGEN_TOOLS}
	@${MAKE_DIR}
	@${ECHO} $(call FIX_ECHO_STRING,Running trap header autogeneration)
	${PYTHON} ${API_CODEGEN} --header -i ${API_XML} -o ${API_BUILD_DIR}


${IPC_TRAP_SIGNALS} : ${API_XML} ${API_CODEGEN_TOOLS}
	@${MAKE_DIR}
	@${ECHO} $(call FIX_ECHO_STRING,Running IPC trap signals autogeneration)
	${PYTHON} ${API_CODEGEN} --ipc-signals -i ${API_XML} -o $@

${IPC_TRAP_PRIMS} : ${IPC_TRAP_SIGNALS} ${API_HEADERS_TARGET} ${API_CODEGEN_TOOLS}
	@${MAKE_DIR}
	@${ECHO} $(call FIX_ECHO_STRING,Running IPC trap prim autogeneration)
	${PYTHON} ${API_CODEGEN} --ipc-prims -i ${API_XML} -o $@

${IPC_TRAP_SLOW_PRIM_CALLS} : ${API_XML} ${API_CODEGEN_TOOLS} ${IPC_TRAP_PRIMS}
	@${MAKE_DIR}
	@${ECHO} $(call FIX_ECHO_STRING,Running slow IPC trap prim receive impl autogeneration)
	${PYTHON} ${API_CODEGEN} --ipc-recv-glue -i ${API_XML} -o $@

${IPC_TRAP_FAST_PRIM_CALLS} : ${API_XML} ${API_CODEGEN_TOOLS} ${IPC_TRAP_PRIMS}
	@${MAKE_DIR}
	@${ECHO} $(call FIX_ECHO_STRING,Running fast IPC trap prim receive impl autogeneration)
	${PYTHON} ${API_CODEGEN} --ipc-recv-fast-glue -i ${API_XML} -o $@

${IPC_TRAP_FAST_PRIM_CASES} : ${API_XML} ${API_CODEGEN_TOOLS} ${IPC_TRAP_PRIMS}
	@${MAKE_DIR}
	@${ECHO} $(call FIX_ECHO_STRING,Running fast IPC trap prim case receive autogeneration)
	${PYTHON} ${API_CODEGEN} --ipc-recv-fast-cases -i ${API_XML} -o $@

ifeq ($(wildcard ${IPC_TRAP_PRIMS_CALLS}),)
${OBJECTS} ${LINT_OBJECTS}  ${DEPFILES} :	${IPC_TRAP_SLOW_PRIM_CALLS} ${IPC_TRAP_FAST_PRIM_CALLS} ${IPC_TRAP_FAST_PRIM_CASES}
endif
${DOXYFILE}:	${IPC_TRAP_SLOW_PRIM_CALLS} ${IPC_TRAP_FAST_PRIM_CALLS} ${IPC_TRAP_FAST_PRIM_CASES}
.PHONY: build_dox_hdrs
build_dox_hdrs: ${IPC_TRAP_SLOW_PRIM_CALLS} ${IPC_TRAP_FAST_PRIM_CALLS} ${IPC_TRAP_FAST_PRIM_CASES}

