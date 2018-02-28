# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# This file is used to abstract differences between command line shells
# that are used to launch the build process.
# Currently supports DOS cmd, cygwin sh,
# Linux bash and QT Creator (MINGW) in Windows

ifdef SYSTEMROOT
    # QT Creator running in Windows defines this symbol in upper case
    # Unfortunately so does cygwin, so we need to let the later detection run
    SystemRoot=SYSTEMROOT
endif

ifdef SystemRoot
    # This indicates a Windows build environment
    OS=win32
    # However, if cygwin is present, even the standalone make will choose that
    # in preference to cmd for its shell. AFAICT, this is indicated
    # by the presence of SHELL in the make environment. To be doubly sure
    # we check for it containing /bin/ as well
    ifeq ($(findstring /bin/, ${SHELL}), /bin/)
        OSTYPE:=$(shell uname -s)
        ifeq (CYGWIN,$(findstring CYGWIN,$(OSTYPE)))
            CommandType=linux
            FileType=cygwin
            CYGWIN_IGNORE_ERRORS=-
        else
            ifeq (MINGW,$(findstring MINGW,$(OSTYPE)))
                SHELL=cmd.exe
                CommandType=DOS
                FileType=DOS
            else
                $(error Unknown Unix-like Windows environment "${OSTYPE}")
            endif
        endif
    else
        CommandType=DOS
        FileType=DOS
    endif
else # SystemRoot
    OS=linux
    CommandType=linux
    FileType=linux
endif # SystemRoot

# Common shell commands
ECHO=echo
ESCAPE_QUOTES=$(subst ",\",$1)

ifeq ("${CommandType}", "DOS")
    NULL=nul:
    RM=del /Q
    RMDIR=rmdir /S /Q
    CAT=type
    FIXPATH=$(subst /,\,$1)
    MKDIR=if not exist $(call FIXPATH,$1) mkdir $(call FIXPATH,$1)
    COPY=copy
    # This is particularly nasty: on Windows KCC creates paths in the .d files
    # by joining the -I string from the command line to the #include path in
    # the C file, both of which use forward slashes, with a *backslash*.  As a
    # result, if you want to use these dependencies as targets for further
    # dependencies, you have to reproduce this exact munged syntax in your rule
    # because make interprets targets and dependencies as literal text strings.
    # In short, the purpose of this function is to replace
    #   core/module/file.h with core\module/file.h
    # and
    #   core/include/file.h with core/include\file.h
    # I bet you wish you hadn't asked now.
    MUNGE_DEP_PATH=$(subst core\include/,core/include\, $(subst core/,core\,$1))
    FIX_ECHO_STRING=$(subst \\,\,$(subst <,^<,$(subst >,^>,$1)))
    EXE_SUFFIX = .exe
    DOXYGEN=doxygen
    INVOKE=type
    AS_WELL_AS=&
    COPY_DIR=xcopy
    COPY_DIR_FLAGS=/e /i /h /Y
    TOUCH=copy /b $(call FIXPATH,$1) +,,
else
    # Must be a Linux build environment
    NULL=/dev/null
    RM=rm -f
    RMDIR=rm -rf
    CAT=cat
    SED=sed
    FIXPATH=$1
    MKDIR=mkdir -p $1
    COPY=cp -f
    MUNGE_DEP_PATH=$1
    FIX_ECHO_STRING="$(subst ",\",$(subst \,\\,$1))"
    DOXYGEN=PATH=/comm/doxygen/1.8.7/bin:${PATH} doxygen
    INVOKE=cat
    AS_WELL_AS=;
    COPY_DIR=cp
    COPY_DIR_FLAGS=-rf
    TOUCH=touch $1
endif
ifeq ("${FileType}", "linux")
    LOCAL_KCC_PATH=/tools/kcc
    NETWORK_KCC_PATH=/home/devtools/kcc
    KCC_VER_PATH=kcc-$(KCC_VER)-linux
    PYTHON?=python
    AS8051_TOOLS_PATH=
else
    LOCAL_KCC_PATH=C:/CSR/kcc
    NETWORK_KCC_PATH=//root.pri/FILEROOT/UnixHomes/home/devtools/kcc
    KCC_VER_PATH=kcc-$(KCC_VER)-win32
    ifeq ("${FileType}", "cygwin")
        PYTHON?=python
    else
        PATHSEARCH=$(firstword $(wildcard $(addsuffix /$(1),$(subst ;, ,$(PATH)))))
        PYTHON_ON_PATH=$(subst \,/,$(call PATHSEARCH,python.exe))
        ifeq ($(PYTHON_ON_PATH),)
            PYTHON?=C:/Python27/python.exe
        else
            PYTHON?=$(PYTHON_ON_PATH)
        endif
    endif
    AS8051_TOOLS_PATH=C:/CSR
endif
# MAKE_DIR should be preceded with a "-" to ignore errors, 
# Windows does not have a "ignore if exists" option and with parallel make 
# testing if the directory exists (as above) can still cause a failure
# due to two MKDIRs running in parallel
MAKE_DIR=$(call MKDIR,${@D})
