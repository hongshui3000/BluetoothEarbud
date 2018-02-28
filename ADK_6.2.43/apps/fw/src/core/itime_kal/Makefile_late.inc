# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Makefile containing the rule to autogenerate itime_subsystems.h

# This is the real platform-independent name of the file
ITIME_SUBSYSTEM_HDR = ${BUILD_DIR}/core/itime_kal/itime_subsystems.h
# This is how this file appears as a dependency in the .d file (may or may not
# be the same)
ITIME_SUBSYSTEM_HDR_TARGET = $(strip $(call MUNGE_DEP_PATH, $(ITIME_SUBSYSTEM_HDR)))

# Map the .d file entry name to the real filename if they differ
ifneq ("$(ITIME_SUBSYSTEM_HDR_TARGET)","$(ITIME_SUBSYSTEM_HDR)")
$(ITIME_SUBSYSTEM_HDR_TARGET) : $(ITIME_SUBSYSTEM_HDR)
endif

# Create the itime subsystem task header
${ITIME_SUBSYSTEM_HDR}: ${MODULE_ITIME_HDRS}
	@-${MAKE_DIR}
	${PYTHON} ${MAKE_TOOLS_DIR}/header_autogen.py -m ITIME_EVENT -o $@ \
	                                                   ${MODULE_ITIME_HDRS}
	@${ECHO} $(call FIX_ECHO_STRING,Regenerated $@)
		
# If the header doesn't already exist, we can't assume that the .d files exist
# to force it to be recreated.  (If it does exist but the .d files don't for
# some reason, then changes to it won't force a rebuild of its  
# dependents.  But that scenario would require a deliberate effort because the
# builds either remove this and the .d files (clean) or create/update them all.)
ifeq ($(wildcard $(ITIME_SUBSYSTEM_HDR)),) 
$(OBJECTS) ${LINT_OBJECTS}  ${DEPFILES} :	$(ITIME_SUBSYSTEM_HDR) $(BG_INT_SUBSYSTEM_HDR)
endif
${DOXYFILE}:	$(ITIME_SUBSYSTEM_HDR) $(BG_INT_SUBSYSTEM_HDR)