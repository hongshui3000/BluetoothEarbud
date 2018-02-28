# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
TOP:=		${abspath ..}
TOOLS_DIR?=	${TOP}/tools
MAKE_TOOLS_DIR= ${TOOLS_DIR}/make

include ../tools/make_utils.mk
include ../config/shell_abstractions.mk

# Lint stuff
PCLINTTOOL=	flint-9.00i
PCLINTTOOL_INCS=${KCC_INCLUDES:%=-I%}
PCLINT_DEFS=	-header ${BUILD_DEFS_HEADER}
PCLINTPPFLAGS=	${PCLINT_DEFS} -Dasm="" ${INC} ${INC_NFC} ${PCLINTTOOL_INCS}
PCLINTBTPPFLAGS=${PCLINT_DEFS} -Dasm="" ${INC} ${INC_BT} ${PCLINTTOOL_INCS}
PCLINTFLAGS=	-v -b ${TOOLS_DIR}/kal.lnt
PCLINT=		${PCLINTTOOL} ${PCLINTPPFLAGS} ${PCLINTFLAGS}
PCLINTBT=	${PCLINTTOOL} ${PCLINTBTPPFLAGS} ${PCLINTFLAGS}
TARGET_PCLINT=	pclint.out

# Override RELEASE on the command line for anything other than this
# default release. e.g. make -s -j8 RELEASE=digits_tb_lib
RELEASE?=	crescendo_p1_d00_lib
 
# Build id stuff
BUILDIDTOOL=	${PYTHON} ${MAKE_TOOLS_DIR}/id.py
BUILDIDFLAGS=	-c ${RELEASE} -d ${TOP}
ifeq ("${DAILY_BUILD}", "yes")
BUILDIDFLAGS+=	-n
endif
BUILDID=	${BUILDIDTOOL} ${BUILDIDFLAGS}
BUILDID_OBJECTS=${BUILD_DIR}/id_string_file.o

ASM_SRC =
SRC =
AUTO_SRC =
INC =

DOXYGEN_SRC=

# The default components we'll make
# Larger builds will extend this
COMPONENTS=core

# If PRODUCTION_BUILD is set, don't include the fw unit tests
ifneq ("${PRODUCTION_BUILD}", "")
FW_ONLY?=1
endif

# OBJECTS must be set before the include immediately following, because this
# includes a makefile that uses $(OBJECTS) as the target of a rule, and targets
# of rules are expanded immediately.  (See GNU make manual, sec. 3.7)
NON_MAIN_OBJECTS= $(patsubst %.c,${BUILD_DIR}/%.o,${SRC}) \
                  $(patsubst %.c,%.o,${AUTO_SRC}) \
                  $(patsubst %.asm,${BUILD_DIR}/%.o,${ASM_SRC})
MAIN_OBJECT= $(patsubst %.c,${BUILD_DIR}/%.o,${MAIN_SRC})
OBJECTS = ${MAIN_OBJECT} ${NON_MAIN_OBJECTS}

# We don't want to lint USB audio class, vm and stream right now... 
LINT_SRC = $(filter-out bt/% wlan/% core/rwfs/./fs% core/rwfs/./target_files/% core/rwfs/./sim/%, ${SRC})

# List of FSMs which need to be generated for LINT builds
LINT_FSM = $(foreach fsm, ${BLUESTACK_FSMS}, ${BUILD_DIR}/${fsm}_fsm_constants.c)
LINT_NFC_FSM = $(filter ${BUILD_DIR}/nfc/%, ${LINT_FSM})

# Lint produces two types of output file per source
# .lob is a binary fed into the final stage
# .lint.out is human readable and forms part of the final output
LINTOUT_OBJECTS=$(patsubst %.c,${BUILD_DIR}/%.lint.out,${LINT_SRC})
LINTLOB_OBJECTS=$(patsubst %.c,${BUILD_DIR}/%.lob,${LINT_SRC})

LINT_OBJECTS=	${LINTOUT_OBJECTS} ${LINTLOB_OBJECTS}

include ../config/release/${RELEASE}.mk

ifneq ("${CHIP_CONFIG}", "")
LINK_SCRIPT = ../config/tool/link_script_${BUILDID_TARGET_DEVICE}_${CHIP_REV}_${CHIP_CONFIG}.ld
else
LINK_SCRIPT = ../config/tool/link_script_${BUILDID_TARGET_DEVICE}_${CHIP_REV}.ld
endif

INC += -Icore -Icore/include -I${BUILD_DIR}/core -Ibt

SOURCES= ${SRC} ${ASM_SRC} ${AUTO_SRC}

LIBRARY_FILE = ${BUILD_DIR}/lib${PROJECT}.a
ELF_FILE = ${BUILD_DIR}/${PROJECT}.elf
MAP_FILE = ${BUILD_DIR}/${PROJECT}.kmap
LST_FILE = ${BUILD_DIR}/${PROJECT}.lst
XUV_FILE = ${BUILD_DIR}/${PROJECT}.xuv
XIDE_WORKSPACE = ${BUILD_DIR}/${PROJECT}.xiw
XIDE_PROJECT = ${BUILD_DIR}/${PROJECT}.xip
QMDE_PROJECT = ${BUILD_DIR}/${PROJECT}.x2p
PYTHON_REG_FILES = ${BUILD_DIR}/io_defs.py ${BUILD_DIR}/io_map.py ${BUILD_DIR}/fpga_io_map.py
DEBUGGER_FILES = ${XIDE_WORKSPACE} ${XIDE_PROJECT} ${QMDE_PROJECT}
BUILDID_STAMP=		${BUILD_DIR}/id.txt
BUILDID_DATE=		${BUILD_DIR}/date.txt

# signed apps p0 image stuff
XUV_BIN_FILE = ${BUILD_DIR}/${PROJECT}.bin
SIGNED_BIN_FILE = ${BUILD_DIR}/${PROJECT}_signed.bin
SIGNED_XUV_FILE = ${BUILD_DIR}/${PROJECT}_signed.xuv

CCORE_DIR = customer/core
API_DIR = ${CCORE_DIR}/trap_api
API_XML = ${API_DIR}/api.xml
API_CODEGEN = ${MAKE_TOOLS_DIR}/api_codegen.py
API_CODEGEN_TOOLS = ${API_CODEGEN} ${MAKE_TOOLS_DIR}/apids.py 
API_SOURCE = ${BUILD_DIR}/${PROJECT}.html
API_DOCUMENT = ${BUILD_DIR}/${PROJECT}.html

DESKTOP_TEST_APP = ${BUILD_DIR}/${PROJECT}${EXE_SUFFIX}

BUILD_DEFS_HEADER=	${BUILD_DIR}/build_defs.h
TARGETS+=	${BUILD_DEFS_HEADER}

ifneq ("${FINAL_TARGETS}","")
all:		$(FINAL_TARGETS)
$(FINAL_TARGETS):	$(TARGETS)
else
all:		$(TARGETS)
endif
# No reason not to build the modules list always
all:		modules

ifneq ("${FW_TARGETS}","")
# Minimal targets to do a fast build of just the firmware elf
fw:		$(FW_TARGETS)
endif


pclint:	${BUILD_DIR}/${TARGET_PCLINT}

pclint_stdout:	pclint
	${CAT} ${BUILD_DIR}/${TARGET_PCLINT}

# Note how the script below gathers up the .lob files that exist
# Not every source lint produces one
# At present we only have flint
# lint-nt exists but is licenced, so this code doesn't currently support it
# This allows us to get away with linux specific command such as sed and cat
# Not to mention a fair amount of Bourne shell syntax
${BUILD_DIR}/${TARGET_PCLINT}:	${LINT_OBJECTS}
	@-[ x$$MAKE_QUIET = x ] && ${ECHO} "Lint link " $@
	cd ${BUILD_DIR} && (for lo in ${LINTOUT_OBJECTS:${BUILD_DIR}/%=%} ; do \
	  if [ -s $$lo ] ; then \
	    ${ECHO} ; \
	    ${ECHO} "================================================================================" ; \
	    ${ECHO} "$$lo:" | ${SED} -e 's/lint.out/c/' ; \
	    ${CAT} $$lo ; \
	  fi ; \
	done ; \
	${ECHO} "================================================================================" ; \
	${ECHO} "FINAL PASS:" ; \
	) > ${@F}
	cd ${BUILD_DIR} && for ss in ${LINTLOB_OBJECTS:${BUILD_DIR}/%=%} ; do \
	  if [ -f $$ss ] ; then \
	    ln_files="$$ln_files $$ss" ; \
	  fi ; \
	done ; \
	${PCLINT} '+os(${@F})' $$ln_files || \
	(S=$$?; ${CAT} ${@F}; ${ECHO} "($$S fatal messages)"; exit $$S)

${LINT_OBJECTS}: ${TOOLS_DIR}/kal.lnt

#
# Doxygen build
# -------------


# Create a list of all the directories that sources have come from in this build
# to point Doxygen at
SRC_MODULE_DIRS = $(sort $(dir ${SRC}))
# Also create a list of the corresponding build areas in case they contain 
# autogenerated sources.  We filter out the ones that don't exist in the recipe,
# which is executed *after* any necessary autogeneration has been triggered
BUILD_MODULE_DIRS = ${SRC_MODULE_DIRS:%=${BUILD_DIR}/%} 
# In principle we also want to tell Doxygen about other header files (e.g. in 
# common/interface) but at present none of these contain Doxygen mark-up so 
# there's no point
EXTRA_DOXYGEN_HEADER_DIRS = \
	../../common/interface/app/bluestack \
	../../common/interface/app/led \
	customer/core/portability
# Give Doxygen the same set of include directories as the build, but only those
# that are really present.  For some reason realpath is more reliable than
# wildcard here.
INCLUDE_DIRS = $(foreach idir, ${INC:${INCLUDE_FLAG}%=%}, $(realpath ${idir}))

# Doxygen requires special handling of quotes
# DEF="EXAMPLE" should be DEF="\"EXAMPLE\"" or the easier to construct "DEF=\"EXAMPLE\""
# Give Doxygen the same set of defines as the build
DOX_DEFINES_PLAIN=${DEFLIST:${DEFINE_FLAG}%=%}
# Escape any quotes in the define
DOX_DEFINES_ESCAPED_QUOTES=$(call ESCAPE_QUOTES,$(DOX_DEFINES_PLAIN))
# Wrap defines that contain quote character in quotes
DOX_DEFINES=$(foreach var,$(DOX_DEFINES_ESCAPED_QUOTES),$(if $(findstring ",$(var)),"$(var)",$(var)))

DOX_OUTPUT_DIR = ${BUILD_DIR}/doxygen_output

DOXYFILE = ${DOX_OUTPUT_DIR}/Doxyfile
dox: ${DOXYFILE}
	${DOXYGEN} 1>${NULL} ${DOXYFILE}
	${ECHO} Copying Register docs
	${COPY_DIR} ${call FIXPATH, core/io/${CHIP_NAME}/${CHIP_REV}/doc} ${call FIXPATH, ${BUILD_DIR}/register_docs} ${COPY_DIR_FLAGS}

DOX_INPUT_SRC = ${SRC_MODULE_DIRS}
DOX_INPUT_HDRS = ${EXTRA_DOXYGEN_HEADER_DIRS}
DOX_INPUT_BDIRS = $(foreach bdir, $(filter-out ${BUILD_DIR}/core/subserv/, ${BUILD_MODULE_DIRS}), $(wildcard ${bdir})) 

${DOXYFILE}: ../docs/doxygen/Doxyfile ../docs/doxygen/MasterDoxyfile
	-${MAKE_DIR}
	${INVOKE} ${call FIXPATH, ../docs/doxygen/Doxyfile} > $@
	${ECHO} $(call FIX_ECHO_STRING, INPUT = ${DOX_INPUT_SRC} ${DOX_INPUT_HDRS}) >> $@
	${ECHO} $(call FIX_ECHO_STRING, INPUT += ${DOX_INPUT_BDIRS}) >> $@
	${ECHO} $(call FIX_ECHO_STRING, INCLUDE_PATH += ${INCLUDE_DIRS}) >> $@
	${ECHO} $(call FIX_ECHO_STRING, PREDEFINED = DOXYGEN_IGNORE DOXYGEN_ONLY) >> $@
	${ECHO} $(call FIX_ECHO_STRING, PREDEFINED += ${DOX_DEFINES}) >> $@
	${ECHO} $(call FIX_ECHO_STRING, PROJECT_NAME = ${RELEASE}) >> $@
	${ECHO} $(call FIX_ECHO_STRING, OUTPUT_DIRECTORY = ${DOX_OUTPUT_DIR}) >> $@


#
# mem_du and HTML/Javascript viewer of the output
# 

# The du file itself. Note: This step does not work on windows yet B-166696
DU_FILE = ${BUILD_DIR}/${PROJECT}.du
MEMDUTOOL=        $(call FIXPATH,${MAKE_TOOLS_DIR}/mem_du.sh)
${DU_FILE}:	${ELF_FILE}
		${MEMDUTOOL} ${ELF_FILE} ${DU_FILE}

# A javascrupt include file of the du data
DU_VISUALISATION_DATA = $(BUILD_DIR)/du_include.js
DUPARSETOOL=        ${PYTHON} $(call FIXPATH,${MEM_DU_VISUALISATION_DIR}/du_parse.py)
${DU_VISUALISATION_DATA}:	${DU_FILE}
	${DUPARSETOOL} ${DU_FILE} ${DU_VISUALISATION_DATA}

# DU parsing HTML and Javascript files copied to builds
MEM_DU_VISUALISATION_DIR= ${TOOLS_DIR}/mem_du_visualisation
${BUILD_DIR}/apps_du.html: ${MEM_DU_VISUALISATION_DIR}/apps_du.html
	${COPY} $(call FIXPATH,$<) $(call FIXPATH,$@)

${BUILD_DIR}/g.pie.js: ${MEM_DU_VISUALISATION_DIR}/g.pie.js
	${COPY} $(call FIXPATH,$<) $(call FIXPATH,$@)

${BUILD_DIR}/g.raphael.js: ${MEM_DU_VISUALISATION_DIR}/g.raphael.js
	${COPY} $(call FIXPATH,$<) $(call FIXPATH,$@)

${BUILD_DIR}/raphael.js: ${MEM_DU_VISUALISATION_DIR}/raphael.js
	${COPY} $(call FIXPATH,$<) $(call FIXPATH,$@)

# The target for the all the DU and visualisation
memdu_vis: ${DU_VISUALISATION_DATA} ${BUILD_DIR}/apps_du.html ${BUILD_DIR}/g.pie.js ${BUILD_DIR}/g.raphael.js ${BUILD_DIR}/raphael.js


# Include the dependency files if they exist, unless making clean
ifeq ($(findstring clean, ${MAKECMDGOALS}), )
-include ${OBJECTS:.o=.d}
endif

${BUILDID_STAMP}:	makefile ${BUILD_DIR}/buildid.int ${BUILD_DIR}/buildid.str
			${BUILDID} -z -o string > $@
			${PYTHON} -c "print" >> $@ # Avoid windoze echo with no args

# Extract the date and time from the string id
# Script cunningly written by our resident python expert
# Note that date/time portion assumed fixed length (16 chars)
${BUILDID_DATE}:	${BUILDID_STAMP}
			${PYTHON} -c "import sys; print sys.stdin.read(16)" < $< > $@

firmware_final_print:	${FW_TARGETS}
			@${ECHO} Firmware successfully built with identifiers:
			@${ECHO} ${call FIX_ECHO_STRING,Numeric ID = ${shell ${CAT} ${call FIXPATH, ${BUILD_DIR}/buildid.int}}}
			@${ECHO} ${call FIX_ECHO_STRING,String ID = ${shell ${CAT} ${call FIXPATH, ${BUILD_DIR}/buildid.str}}}


# segments is defined here in case we have more than one firmware build
# Note that we can't put it in kcc_compiler.mk, where it logically belongs,
# as that will cause it to the the default make target rather than all
# It's added into FINAL_TARGETS by those builds that want it
SEGMENTINFOTOOL=	${PYTHON} $(call FIXPATH,${MAKE_TOOLS_DIR}/segment_info.py)
segments:	${ELF_FILE}
		@${ECHO} ${call FIX_ECHO_STRING,Computing link segments}
		${SEGMENTINFOTOOL} ${KCC_BIN_DIR}/knm ${ELF_FILE}

# Something to put the correct version info in the mib file
MAKE_APP_MIB_TOOL=	${call FIXPATH,${MAKE_TOOLS_DIR}/make_app_mib.py}
${BUILD_DIR}/${MIB_XML_FILE}:	${ELF_FILE} ${BUILDID_DATE} ${BUILD_DIR}/buildid.int
		${PYTHON} ${MAKE_APP_MIB_TOOL} -i ${call FIXPATH,${BUILD_DIR}/buildid.int} -d ${call FIXPATH,${BUILDID_DATE}} < ../../common/mib/${MIB_XML_FILE} > $@ || (${RM} $@; exit 1)

bitz.dmp:
	${ECHO} Copying Bitz File
	${COPY} ${call FIXPATH, core/io/${CHIP_NAME}/${CHIP_REV}/regs/bitz.dmp} ${call FIXPATH, ${BUILD_DIR}/bitz.dmp}

${MAP_FILE}: ${ELF_FILE}
	${ECHO} Creating Map File
	${MAPCMD} $< > $@

${LST_FILE}: ${ELF_FILE}
	${ECHO} Creating Listing File
	${OBJDUMP} $< -S > $@


LINK_OBJECTS_FILE = $(call FIXPATH,${BUILD_DIR}/link_objects.out)

# For some reason, putting the command to delete link_objects.out in
# the same recipe as the command to recreate it causes it not to be recreated.
# Hence this helper target.
rm_link_objs:
	-${RM} ${LINK_OBJECTS_FILE} 2>${NULL}

# Multiple target rule to build module libraries.
# All libraries share the same prerequisites, so changing any object file
# from ${MODULE_LIB_OBJECTS} starts rebuilding of all libraries
${MODULE_LIBS}: ${MODULE_LIB_OBJECTS}
	${AR} ruvc $@ ${${patsubst %/,%,${dir ${subst ${BUILD_DIR}/,,$@}}}_LIB_OBJECTS}

${ELF_FILE}: ${OBJECTS} ${LINK_SCRIPT} ${BUILDID_OBJECTS} ${MODULE_LIBS} rm_link_objs
	${ECHO} Linking
	${foreach comp_sub,${COMPONENT_SUBSYSTEMS},${shell ${PYTHON} \
		$(call FIXPATH,${MAKE_TOOLS_DIR}/mk_objects.py) ${LINK_OBJECTS_FILE} \
		${BUILD_DIR}/${comp_sub} ${${comp_sub}_BARE_OBJECTS}}}
	${LD} ${LD_OPTIONS} @${LINK_OBJECTS_FILE} ${BUILDID_OBJECTS} ${MODULE_LIBS} ${LIBS} -T ${LINK_SCRIPT} -o $@

# Target to build the non-main objects into an archive
${LIBRARY_FILE}: ${LIB_OBJECTS} ${BUILDID_OBJECTS} ${MODULE_LIBS} rm_link_objs
	${ECHO} Creating archive
	${AR} ruc $@ ${BUILDID_OBJECTS} ${NON_MAIN_OBJECTS}

lib: ${LIBRARY_FILE}

${BUILD_DIR}/%.o: %.asm
	-${MAKE_DIR}
	${ECHO} Assembling $<
	${ASM} -o $@ $< ${ASM_OPTIONS} ${ASMDEPGENFLAGS}

# Add module name define, which is calculated from the prerequisite directory (last folder identifies the module)
MODULE_NAME = -D_MODULE_NAME_=$(word $(words $(subst /, ,$(<D))),$(subst /, ,$(<D)))

# Special target for files in wlan - don't warn about AccuracyLoss
# This must come before the more general rule: make is supposed to find the
# most specific rule regardless of ordering, but our Windows make appears not
# to.
${BUILD_DIR}/wlan/%.o: wlan/%.c
	-${MAKE_DIR}
	${ECHO} Compiling $<
	${CC} ${CCOUT} $< ${CFLAGS_WLAN} ${DEPGENFLAGS} ${MODULE_NAME}

# Special target for files in bt - don't warn about AccuracyLoss
${BUILD_DIR}/bt/%.o: bt/%.c
	-${MAKE_DIR}
	${ECHO} Compiling $<
	${CC} ${CCOUT} $< ${CFLAGS_BT} ${DEPGENFLAGS} ${MODULE_NAME}

${BUILD_DIR}/%.o: %.c
	-${MAKE_DIR}
	${ECHO} Compiling $<
	${CC} ${CCOUT} $< ${CFLAGS} ${DEPGENFLAGS} ${MODULE_NAME}

# Special target for files in bt - don't warn about AccuracyLoss and add
# bt includes
# Position in the makefile is important - see above.
${BUILD_DIR}/bt/%.o: ${BUILD_DIR}/bt/%.c
	-${MAKE_DIR}
	${ECHO} Compiling $<
	${CC} ${CCOUT} $< ${CFLAGS_BT} ${DEPGENFLAGS} ${MODULE_NAME}

${BUILD_DIR}/%.o: ${BUILD_DIR}/%.c
	-${MAKE_DIR}
	${ECHO} Compiling $<
	${CC} ${CCOUT} $< ${CFLAGS} ${DEPGENFLAGS} ${MODULE_NAME}

ifneq (${SEPARATEDEPGEN},)
# Use KCC to generate dependency files
include ../config/kcc_compiler_depgen.mk
${BUILD_DIR}/%.o: ${BUILD_DIR}/%.d
${BUILD_DIR}/%.d: %.c
	-${MAKE_DIR}
	${ECHO} Generating dependencies for $<
	${KCC} ${COMMON_CFLAGS} $< ${KCC_DEPGEN_FLAGS} -E -o ${<:%.c=${BUILD_DIR}/%.i}
	${RM} $(call FIXPATH, ${<:%.c=${BUILD_DIR}/%.i}) 
endif


${XUV_FILE}: ${ELF_FILE}
	${PYTHON} ${MAKE_TOOLS_DIR}/elf2xuv.py $<
	
ifeq ("${ENABLE_SIGNED}", "true")
KEY_NO = 0
${XUV_BIN_FILE}: ${XUV_FILE}
	${PYTHON} ${MAKE_TOOLS_DIR}/xuv2binle.py $< $@

${SIGNED_BIN_FILE}: ${XUV_BIN_FILE}
	${PYTHON} ${MAKE_TOOLS_DIR}/create_apps_p0_signed.py $< $@ ${KEY_NO}

${SIGNED_XUV_FILE}: ${SIGNED_BIN_FILE}
	${PYTHON} ${MAKE_TOOLS_DIR}/bin2xuvle.py $< $@
endif

${BUILD_DIR}/bt/%.lob ${BUILD_DIR}/bt/%.lint.out: bt/%.c
	-${MAKE_DIR}
	${ECHO} Lint $<
	${PCLINTBT} -u '-os(${@D}/${*F}.lint.out)' $< '-oo(${@D}/${*F}.lob)' || \
        (S=$$?; ${CAT} ${@D}/${*F}.lint.out; \
         ${ECHO} "($$S fatal messages)"; exit $$S)

${BUILD_DIR}/bt/%.lob ${BUILD_DIR}/bt/%.lint.out: ${BUILD_DIR}/bt/%.c
	-${MAKE_DIR}
	${ECHO} Lint $<
	${PCLINTBT} -u '-os(${@D}/${*F}.lint.out)' $< '-oo(${@D}/${*F}.lob)' || \
        (S=$$?; ${CAT} ${@D}/${*F}.lint.out; \
         ${ECHO} "($$S fatal messages)"; exit $$S)

${BUILD_DIR}/%.lob ${BUILD_DIR}/%.lint.out: %.c ${LINT_NFC_FSM}
	-${MAKE_DIR}
	${ECHO} Lint $<
	${PCLINT} ${MODULE_NAME} -u '-os(${@D}/${*F}.lint.out)' $< '-oo(${@D}/${*F}.lob)' || \
        (S=$$?; ${CAT} ${@D}/${*F}.lint.out; \
         ${ECHO} "($$S fatal messages)"; exit $$S)

# Stuff for buildids
# These get remade if a library is built
# Hence they cannot be done by the usual auto generation stage
# as that occurs much too early.
# Force remove first as if we copied values for a patch build they may
# be read-only.
${BUILD_DIR}/buildid.int:	${OBJECTS}
	-${MAKE_DIR}
	${ECHO} Rebuilding $@
	${RM} ${call FIXPATH, $@} 2>${NULL}
	${BUILDID} -o number > $@

# Sequence this to avoid attempts to do it in parallel
${BUILD_DIR}/buildid.str:	${OBJECTS} ${BUILD_DIR}/buildid.int
	-${MAKE_DIR}
	${ECHO} Rebuilding $@
	${RM} ${call FIXPATH, $@} 2>${NULL}
	${BUILDID} -o string > $@

${BUILD_DIR}/id_string_file.c:	${BUILD_DIR}/buildid.int ${BUILD_DIR}/buildid.str
	@${ECHO} Rebuilding $@
	-${MAKE_DIR}
	${PYTHON} ${MAKE_TOOLS_DIR}/id_string_filegen.py $@ "${shell ${CAT} ${call FIXPATH, ${BUILD_DIR}/buildid.str}}" "${shell ${CAT} ${call FIXPATH, ${BUILD_DIR}/buildid.int}}"

# Build individual .i files locally.  This rule is not intended to be part of
# anything global
# Note: don't add whitespace between ${CCOUTFLAG} and $@ in the following
# recipe: the Windows compiler can't handle it
%.i: %.c FORCE
	-${MAKE_DIR}
	${ECHO} Preprocessing $<
	${CC} ${CCOUT} $< ${PPFLAGS} ${MODULE_NAME}

${XIDE_PROJECT}: $(ELF_FILE)
	${PYTHON} ${MAKE_TOOLS_DIR}/mkxip.py -o$@ -e $(notdir $<) -p "../../src" \
											-c ${PROCESSOR_NUM} -s ${SUBSYS_ID}
${QMDE_PROJECT}: ${OBJECTS} modules
	${ECHO} Creating x2p
	${PYTHON} ${MAKE_TOOLS_DIR}/mkx2p.py -i"${INCLUDE_DIRS} ." -o$@ -r${RELEASE} -d${BUILD_DEFS_HEADER} -l${LINK_SCRIPT}

WORKSPACE_CONTENT= <workspace> <project path="${PROJECT}.xip" /></workspace>
${XIDE_WORKSPACE}:
	${ECHO} $(call FIX_ECHO_STRING,${WORKSPACE_CONTENT}) > $@

ifeq (,$(wildcard ../../../hydra_os*/fw/tools/.*))
  #For when building from inside the ADK
  HYDRA_OS_INSTALL_DIR = installed_libs
else
  #For when building from a perforce checkout
  HYDRA_OS_INSTALL_DIR = ..\vm\installed_libs
endif

ifneq ($(findstring crescendo, ${RELEASE}),)
EXTERNAL_CHIP_NAME=$(subst _p1,,$(subst _lib,,$(subst crescendo,csra68100,${RELEASE})))
else ifneq ($(findstring aura, ${RELEASE}),)
EXTERNAL_CHIP_NAME=$(subst _p1,,$(subst _lib,,$(subst aura,qcc3400,${RELEASE})))
else ifneq ($(findstring qcc512x_qcc302x, ${RELEASE}),)
EXTERNAL_CHIP_NAME=$(subst _lib,,${RELEASE})
else
$(error Unknown external chip name)
endif
INSTALLED_LIBRARY_FILE := ../../${HYDRA_OS_INSTALL_DIR}/lib/os/${EXTERNAL_CHIP_NAME}/$(notdir ${LIBRARY_FILE})

install_lib_adk: ${INSTALLED_LIBRARY_FILE}

${INSTALLED_LIBRARY_FILE}: ${LIBRARY_FILE}
	cd ../.. && ${PYTHON} setup.py --install_dir ${HYDRA_OS_INSTALL_DIR} --build_to_copy ${RELEASE}

# Autogenerate and install library headers for the ADK libs build
install_os_hdrs: $(API_HEADERS_TARGET)
	cd ../.. && ${PYTHON} setup.py --install_dir ${HYDRA_OS_INSTALL_DIR} --build_to_copy ${RELEASE} --headers_only


${BUILD_DIR}/%.a: ${OBJECTS}
	${ECHO} Linking library $@
	${AR} ruvc $@ ${OBJECTS}

${API_DOCUMENT}: customer/core/trap_api/api.xml
	$(PYTHON) $(TOOLS_DIR)/apidocgen.py $< $@

${DESKTOP_TEST_APP}: ${OBJECTS} rm_link_objs
	${ECHO} Linking
	${foreach comp_sub,${COMPONENT_SUBSYSTEMS},${shell ${PYTHON} \
		$(call FIXPATH,${MAKE_TOOLS_DIR}/mk_objects.py) ${LINK_OBJECTS_FILE} \
		${BUILD_DIR}/${comp_sub} ${${comp_sub}_BARE_OBJECTS}}}
	${LD} ${LD_OPTIONS} @${LINK_OBJECTS_FILE} ${LIBS} ${LDOUTFLAG}$@
	${ECHO} Built desktop test application $@

${BUILD_DIR}/io_map.py: core/io/${CHIP_NAME}/${CHIP_REV}/io/io_map.py
	${COPY} $(call FIXPATH,$<) $(call FIXPATH,$@)

${BUILD_DIR}/io_defs.py: core/io/${CHIP_NAME}/${CHIP_REV}/io/io_defs.py
	${COPY} $(call FIXPATH,$<) $(call FIXPATH,$@)

${BUILD_DIR}/fpga_io_map.py: core/io/${CHIP_NAME}/${CHIP_REV}/ext_regs/io_map.py
	${COPY} $(call FIXPATH,$<) $(call FIXPATH,$@)

.PHONY: windowsforce firmware_final_print segments force
# Special target used to force Windows desktop builds to re-build everything as
# the compiler doesn't produce dependencies.
windowsforce:

# Genereal phony target to force things to be made each time
force:

# Ensure that the BlueStack fsm sources get generated first if they currently
# don't exist (which we assume implies it's a clean build, hence no .d files
# to do the job for us).
ifeq ($(wildcard ${BLUESTACK_FSMS:%=${BUILD_DIR}/%_fsm_definitions.h}),)
$(filter ${BUILD_DIR}/bt/%, ${OBJECTS}) : ${BLUESTACK_FSMS:%=${BUILD_DIR}/%_fsm_definitions.h}
$(filter ${BUILD_DIR}/nfc/%, ${OBJECTS}) : ${BLUESTACK_FSMS:%=${BUILD_DIR}/%_fsm_definitions.h}
endif

ifneq ($(call FIXPATH,${BUILD_DIR}), ${BUILD_DIR})
# Ensure that dependencies on Bluestack FSM files which are written with
# backslashes resolve to the rule the follows, which is written with forward
# slashes
FSM_CONSTANTS = $(foreach fsm, ${BLUESTACK_FSMS}, ${BUILD_DIR}/${fsm}_fsm_constants.c)
FSM_DEFINITIONS = $(foreach fsm, ${BLUESTACK_FSMS}, ${BUILD_DIR}/${fsm}_fsm_definitions.h)
$(foreach fsm, ${FSM_CONSTANTS}, $(eval $(call FIXPATH, ${fsm}) : ${fsm}))
$(foreach fsm, ${FSM_DEFINITIONS}, $(eval $(call FIXPATH, ${fsm}) : ${fsm}))
endif

${BUILD_DIR}/%_fsm_constants.c $(BUILD_DIR)/%_fsm_definitions.h:\
        %_fsm.fsm ${MAKE_TOOLS_DIR}/fsmgen.py \
        bt/utils/fsm/Templates/Sparse16_Constants_C.tpl \
        bt/utils/fsm/Templates/Sparse16_Definitions_H.tpl
	-${MAKE_DIR}
	${ECHO} Building FSM $<
	$(PYTHON) ${MAKE_TOOLS_DIR}/fsmgen.py $< ${BUILD_DIR} $(call ESCAPE_QUOTES,${DEFLIST})

# Now that all the variables scattered between different modules' Makefile.incs
# have been set, include any rules etc that depend on them
include $(LATE_INC_MAKEFILES)

.PHONY : clean cleanbt
clean:
	-${RMDIR} $(call FIXPATH,${BUILD_DIR})
cleanbt:
	-${RMDIR} $(call FIXPATH,${BUILD_DIR}/bt})

.PHONY: FORCE
FORCE:

MODULES_FILE=	${call FIXPATH,${BUILD_DIR}/module_list}

# Same reason as LINK_OBJECTS_FILE
# Although the foreach runs, it doesn't seem to write to the file
# This isn't an artefact of -j 8, it happens even without that
rm_modules_file:
		-${RM} ${MODULES_FILE} 2>${NULL}

modules:	rm_modules_file
		${foreach comp_sub,${COMPONENT_SUBSYSTEMS},${shell ${ECHO} ${comp_sub} >>${MODULES_FILE}}}

run:	${DESKTOP_TEST_APP}
	${DESKTOP_TEST_APP}

# Now some clever stuff to handle last_defines type functionality
# Note the use of info, forcing things to happen at parse time
ifeq ($(findstring clean, ${MAKECMDGOALS}), )
$(info $(shell $(call MKDIR,${BUILD_DIR}))Creating ${BUILD_DIR})
# Point the analysis script at the components' build-dir counterparts, in case
# there is any autogenerated code that needs looking at
BUILD_COMPONENTS = $(foreach buildcomp,${COMPONENTS:%=${BUILD_DIR}/%},$(wildcard ${buildcomp}))
$(info Analysed last_defines $(shell ${PYTHON} ${MAKE_TOOLS_DIR}/last_defines_analyse.py \
						-r ${BUILD_DEFS_HEADER:%.h=%} \
						-d "${COMPONENTS} ${BUILD_COMPONENTS}" \
						-m "${MODULES}" \
						-a "$(call ESCAPE_QUOTES,${ADDITIONAL_DEFS})"))
include ${BUILD_DEFS_HEADER:%.h=%.d}
endif

# Rules for people who rename files and expect the dependencies
# to continue to work
%.h:
	${ECHO} $@ appears to be missing, ignoring for the moment
