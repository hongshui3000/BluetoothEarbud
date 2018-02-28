############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2014 - 2017 Qualcomm Technologies International, Ltd.
#
ifeq ($(CONFIG),)
ifeq ($(RELEASE_ID),)
$(error No CONFIG or RELEASE_ID provided, nothing to do)
endif
endif

# Point to a known version of Python for Linux build environments
ifneq ($(findstring linux, $(OSTYPE)), )
ifeq ($(shell uname -m),x86_64)
PYTHON=/comm/python/2.7.8/bin/python
else
$(error Only 64-bit machines supported)
endif
else
ARGS += -makepath=$(DEVKIT_ROOT)\tools\bin\make.exe
PYTHON=$(DEVKIT_ROOT)\tools\python27\python.exe
endif

#CONFIG
ifneq ($(CONFIG),)
ARGS += -config=$(CONFIG)
endif

# If a release id has been provided, check if if we want to do a dummy release (link against a
# released image but don't release) or a proper release
ifneq ($(RELEASE_ID),)
ifeq ($(RELEASE),True)
RELEASE_FLAG = True
else
RELEASE_FLAG = False
endif
ARGS += -buildid=$(RELEASE_ID) RELEASE=$(RELEASE_FLAG)
endif

# Tests like cap_download_mgr_test will need to define the variables below as the KCSMaker
# won't be able to find them automatically, so pass them on to the script.

#BUNDLE_NAME
ifneq ($(BUNDLE_NAME),)
ARGS += -bundlename=$(BUNDLE_NAME)
endif

#EXTRA_INCLUDE
ifneq ($(EXTRA_INCLUDE),)
ARGS += -extrainclude=$(EXTRA_INCLUDE)
endif

# BUILD_PATH
ifneq ($(BUILD_PATH),)
ARGS += -buildpath=$(BUILD_PATH)
endif

# BUILD_NAME
ifneq ($(BUILD_NAME),)
ARGS += -buildname=$(BUILD_NAME)
endif

# BUILD_ROOT
ifneq ($(BUILD_ROOT),)
ARGS += -buildroot=$(BUILD_ROOT)
endif

# KYMERA_SRC_PATH
ifneq ($(KYMERA_SRC_PATH),)
ARGS += -kymerasourcepath=$(KYMERA_SRC_PATH)
endif

# CLEAN
ifneq ($(CLEAN),)
ifeq ($(CLEAN),True)
ARGS += -clean
else
ARGS += -noclean
endif
endif

ARGS += -ostype=$(OSTYPE)

.PHONY: all
.PHONY: clean
.PHONY: build

all:
	$(PYTHON) KCSMaker.py $(ARGS) && echo $(CONFIG) built successfully

build: all


# clean rule here only deletes one file
# QMDE sends clean/build targets to the audio build system. 
# Deleting the last_bundle.txt file here will force a clean build next time. 
# This approach is consistent with a "rebuild" in QMDE which performs a "clean" 
# and immediately after a "build"

clean:
ifeq ("$(OSTYPE)","Windows_NT")
	-del /q /f .\out\$(CONFIG)\last_bundle.txt 2> nul
else ifeq ("$(OSTYPE)","linux")
	rm -f ./out/$(CONFIG)/last_bundle.txt
endif
