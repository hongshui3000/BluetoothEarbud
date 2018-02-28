# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Makefile containing the rules to autogenerate SLT code

# These are the real platform-independent name of the files
SLT_ENTRY_SUBSYSTEM_HDR = ${BUILD_DIR}/core/slt/slt_entry_subsystems.h
SLT_DATA_SUBSYSTEM_HDR = ${BUILD_DIR}/core/slt/slt_data_subsystems.h
# This is how these files appear as a dependency in the .d file (may or may not
# be the same)
SLT_ENTRY_SUBSYSTEM_HDR_TARGET=	$(strip $(call MUNGE_DEP_PATH, ${SLT_ENTRY_SUBSYSTEM_HDR}))
SLT_DATA_SUBSYSTEM_HDR_TARGET=	$(strip $(call MUNGE_DEP_PATH, ${SLT_DATA_SUBSYSTEM_HDR}))

# Map the .d file entry name to the real filename if they differ
ifneq ("${SLT_ENTRY_SUBSYSTEM_HDR_TARGET}","${SLT_ENTRY_SUBSYSTEM_HDR}")
${SLT_ENTRY_SUBSYSTEM_HDR_TARGET} : ${SLT_ENTRY_SUBSYSTEM_HDR}
endif
ifneq ("${SLT_DATA_SUBSYSTEM_HDR_TARGET}","${SLT_DATA_SUBSYSTEM_HDR}")
${SLT_DATA_SUBSYSTEM_HDR_TARGET} : ${SLT_DATA_SUBSYSTEM_HDR}
endif

# Create the slt subsystem task headers
${SLT_ENTRY_SUBSYSTEM_HDR}: ${MODULE_SLT_ENTRY_HDRS}
	@-${MAKE_DIR}
	${PYTHON} ${MAKE_TOOLS_DIR}/header_autogen.py -m SLT_ENTRY -o $@ \
	                                                   ${MODULE_SLT_ENTRY_HDRS}
	@${ECHO} $(call FIX_ECHO_STRING,Regenerated $@)

${SLT_DATA_SUBSYSTEM_HDR}: ${MODULE_SLT_DATA_HDRS}
	@-${MAKE_DIR}
	${PYTHON} ${MAKE_TOOLS_DIR}/header_autogen.py -m SLT_DATA -o $@ \
	                                                   ${MODULE_SLT_DATA_HDRS}
	@${ECHO} $(call FIX_ECHO_STRING,Regenerated $@)

# If the headers don't already exist, we can't assume that the .d files exist
# to force them to be recreated.  (If they do exist but the .d files don't for
# some reason, then changes to them won't force a rebuild of its  
# dependents.  But that scenario would require a deliberate effort because the
# builds either remove these and the .d files (clean) or create/update them all.)
ifeq ($(wildcard ${SLT_ENTRY_SUBSYSTEM_HDR}),) 
${OBJECTS} ${LINT_OBJECTS} ${DEPFILES} :	${SLT_ENTRY_SUBSYSTEM_HDR} ${SLT_DATA_SUBSYSTEM_HDR}
endif
${DOXYFILE}:	${SLT_ENTRY_SUBSYSTEM_HDR} ${SLT_DATA_SUBSYSTEM_HDR}