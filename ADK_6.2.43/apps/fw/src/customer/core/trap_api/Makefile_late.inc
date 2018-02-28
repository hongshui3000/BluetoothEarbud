# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Makefile containing the rules to autogenerate IPC signals and primitives
# and the trap API headers they depend on

include customer/core/trap_api/Makefile_late_common.inc

API_IPC_GLUE = ${API_GEN_BUILD_DIR}/trap_api_ipc_glue.c

API_IPC_GLUE_TARGET = $(call FIXPATH,${API_IPC_GLUE})
ifneq ("${API_IPC_GLUE_TARGET}","${API_IPC_GLUE}")
${API_IPC_GLUE_TARGET} : ${API_IPC_GLUE}
endif

${API_IPC_GLUE} : ${API_XML} ${API_CODEGEN_TOOLS}
	@-${MAKE_DIR}
	@${ECHO} $(call FIX_ECHO_STRING,Running trap IPC glue autogeneration)
	${PYTHON} ${API_CODEGEN} --ipc-send-glue -i ${API_XML} -o $@

ifeq ($(wildcard ${API_HEADERS_GEN}),)
${OBJECTS} ${LINT_OBJECTS}  ${DEPFILES} ${MODULE_LIB_OBJECTS} :	${API_HEADERS_TARGET}
endif
ifeq ($(wildcard ${API_IPC_GLUE}),)
${OBJECTS} ${LINT_OBJECTS}  ${DEPFILES} ${MODULE_LIB_OBJECTS} :	${API_IPC_GLUE}
endif
${DOXYFILE}:	${API_HEADERS_GEN} ${API_IPC_GLUE}
install_os_hdrs: ${API_HEADERS_TARGET}
