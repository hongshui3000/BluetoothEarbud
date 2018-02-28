# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Makefile containing the rules to autogenerate trap version information
# in a C file.

API_TRAP_SUPPORTED_C_TARGET = $(call FIXPATH,${API_TRAP_SUPPORTED_C})
ifneq ("${API_TRAP_SUPPORTED_C_TARGET}","${API_TRAP_SUPPORTED_C}")
${API_TRAP_SUPPORTED_C_TARGET} : ${API_TRAP_SUPPORTED_C}
endif

${API_TRAP_SUPPORTED_C} : ${API_XML} ${API_CODEGEN_TOOLS}
	@-${MAKE_DIR}
	@${ECHO} $(call FIX_ECHO_STRING,Running trap version autogeneration)
	${PYTHON} ${API_CODEGEN} --slt -i ${API_XML} -o $@ -d ${BUILD_DEFS_HEADER}
