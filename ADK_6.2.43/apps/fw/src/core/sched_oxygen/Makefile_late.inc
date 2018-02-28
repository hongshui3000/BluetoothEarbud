# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Makefile containing the rule to autogenerate sched_subsystems.h

# This is the real platform-independent name of the file
SCHED_SUBSYSTEM_HDR = ${BUILD_DIR}/core/sched_oxygen/sched_subsystem.h
BG_INT_SUBSYSTEM_HDR = ${BUILD_DIR}/core/sched_oxygen/bg_int_subsystem.h
# This is how this file appears as a dependency in the .d file (may or may not
# be the same)
SCHED_SUBSYSTEM_HDR_TARGET = $(strip $(call MUNGE_DEP_PATH, $(SCHED_SUBSYSTEM_HDR)))
BG_INT_SUBSYSTEM_HDR_TARGET = $(strip $(call MUNGE_DEP_PATH, $(BG_INT_SUBSYSTEM_HDR)))

# Map the .d file entry name to the real filename if they differ
ifneq ("$(SCHED_SUBSYSTEM_HDR_TARGET)","$(SCHED_SUBSYSTEM_HDR)")
$(SCHED_SUBSYSTEM_HDR_TARGET) : $(SCHED_SUBSYSTEM_HDR)
endif
ifneq ("$(BG_INT_SUBSYSTEM_HDR_TARGET)","$(BG_INT_SUBSYSTEM_HDR)")
$(BG_INT_SUBSYSTEM_HDR_TARGET) : $(BG_INT_SUBSYSTEM_HDR)
endif

# Create the sched subsystem task header
${SCHED_SUBSYSTEM_HDR}: ${MODULE_SCHED_HDRS}
	@-${MAKE_DIR}
	${PYTHON} ${MAKE_TOOLS_DIR}/header_autogen.py -m SCHED_TASK -o $@ \
	                                                   ${MODULE_SCHED_HDRS}
	@${ECHO} $(call FIX_ECHO_STRING,Regenerated $@)
	
# Create the sched subsystem bg int header
${BG_INT_SUBSYSTEM_HDR}: ${MODULE_SCHED_HDRS}
	-${MAKE_DIR}
	${PYTHON} ${MAKE_TOOLS_DIR}/header_autogen.py -m BG_INT -o $@ \
	                                                   ${MODULE_SCHED_HDRS}
	@${ECHO} $(call FIX_ECHO_STRING,Regenerated $@)

	
# If the header doesn't already exist, we can't assume that the .d files exist
# to force it to be recreated.  (If it does exist but the .d files don't for
# some reason, then changes to it won't force a rebuild of its  
# dependents.  But that scenario would require a deliberate effort because the
# builds either remove this and the .d files (clean) or create/update them all.)
ifeq ($(wildcard $(SCHED_SUBSYSTEM_HDR)),) 
$(OBJECTS) ${LINT_OBJECTS} ${DEPFILES} :	$(SCHED_SUBSYSTEM_HDR) $(BG_INT_SUBSYSTEM_HDR)
endif
${DOXYFILE}:	$(SCHED_SUBSYSTEM_HDR) $(BG_INT_SUBSYSTEM_HDR)
