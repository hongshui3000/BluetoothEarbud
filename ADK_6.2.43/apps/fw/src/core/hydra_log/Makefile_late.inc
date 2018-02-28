# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Makefile containing the rule to autogenerate hydra_log_subsystems.h

# This is the real platform-independent name of the file
HYDRA_LOG_SUBSYSTEMS_HDR = ${BUILD_DIR}/core/hydra_log/hydra_log_subsystems.h

${HYDRA_LOG_SUBSYSTEMS_HDR}:
	@-${MAKE_DIR}
	${PYTHON} ${MAKE_TOOLS_DIR}/header_module_log_autogen.py -m HYDRA_LOG_FIRM_MODULES_X -o $@ \
	                                                   ${COMPONENT_SUBSYSTEMS}
	@${ECHO} $(call FIX_ECHO_STRING,Regenerated $@)
	
# ${HYDRA_LOG_SUBSYSTEMS_HDR}:
# 	@-${MAKE_DIR}
#	@echo "Auto-generating " $@
#	@echo #define HYDRA_LOG_FIRM_MODULES_X(x) \> $@
#For all elements in component subsystems print out the name of the file only
#	FOR %%A IN ($(COMPONENT_SUBSYSTEMS)) DO ECHO x(%%~nA) \>> $@
#Append an empty line to close the define backslash	
#	@echo /* List of auto generated modules end */ >> $@

# If the header doesn't already exist, we can't assume that the .d files exist
# to force it to be recreated.  (If it does exist but the .d files don't for
# some reason, then changes to it won't force a rebuild of its  
# dependents.  But that scenario would require a deliberate effort because the
# builds either remove this and the .d files (clean) or create/update them all.)
ifeq ($(wildcard $(HYDRA_LOG_SUBSYSTEMS_HDR)),)
$(OBJECTS) ${LINT_OBJECTS} ${DEPFILES} :	${HYDRA_LOG_SUBSYSTEMS_HDR}
endif
