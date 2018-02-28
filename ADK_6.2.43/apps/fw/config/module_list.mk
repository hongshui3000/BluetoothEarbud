# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
include ../tools/make_utils.mk

# Definitions enabling firmware behaviour

INC += -I../../common/interface -I../../common/interface/app

# The config files below will populate the MODULES variable with a
# list of Makefile.inc files - one for each module directory. Each of the
# Makefile fragments are included so that they set the SRC and SRC_ASM
# variables for the build
-include ${CORE_MODULES_LIST}
COMPONENT = core
include $(CORE_MODULES:%=core/%/Makefile.inc)
-include ${BIST_MODULES_LIST}
COMPONENT = core
include $(BIST_MODULES:%=core/%/Makefile.inc)
-include ${HTOL_MODULES_LIST}
COMPONENT = core
include $(HTOL_MODULES:%=core/%/Makefile.inc)
-include ${CUSTOMER_MODULES_LIST}
COMPONENT = customer
include $(CUSTOMER_MODULE_PATHS:%=customer/%/Makefile.inc)
-include ${NFC_MODULES_LIST}
COMPONENT = nfc
include $(NFC_MODULES:%=nfc/%/Makefile.inc)
-include ${NFC_HTOL_MODULES_LIST}
COMPONENT = nfc
include $(NFC_HTOL_MODULES:%=nfc/%/Makefile.inc)
-include ${BT_MODULES_LIST}
COMPONENT = bt
include $(BT_MODULES:%=bt/%/Makefile.inc)
-include ${WLAN_MODULES_LIST}
COMPONENT = wlan
include $(WLAN_MODULES:%=wlan/%/Makefile.inc)

MODULES = $(CORE_MODULES) $(BIST_MODULES) $(CUSTOMER_MODULES) $(NFC_MODULES) \
          $(BT_MODULES) $(WLAN_MODULES)
UPPER_CASE_MODULES = $(call uc,$(MODULES))
MODULE_DEFS = $(UPPER_CASE_MODULES:%=%_MODULE_PRESENT)
