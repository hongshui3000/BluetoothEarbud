# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# List the modules to build for the customer component of the various P1 
# builds.

SUBCOMPONENTS += \
           core \
           bt \
           apps \
           libs

CRESCENDO_SUBCOMPONENTS = \
           wlan \
           nfc

PRESTO_SUBCOMPONENTS = \
           wlan \
           nfc

# Module variables must be named following this syntax: 
# ${COMPONENT}_${SUBCOMPONENT}_MODULES
CUSTOMER_CORE_COMMON_MODULES += \
 trap_api

CUSTOMER_CORE_FIRMWARE_MODULES += \
 init

CUSTOMER_BT_FIRWARE_MODULES +=

CUSTOMER_WLAN_FIRMWARE_MODULES +=

CUSTOMER_NFC_FIRMWARE_MODULES +=

CUSTOMER_APPS_FIRMWARE_MODULES += \
							
CRESCENDO_CUSTOMER_APPS_FIRMWARE_MODULES += \

PRESTO_CUSTOMER_APPS_FIRMWARE_MODULES += \
 nfc_ex \
 nfc_negotiated_handover	

CUSTOMER_LIBS_FIRMWARE_MODULES = \

# nfc_api nfc_cl
# The nfc_api and nfc_cl modules should be integrated to 
# vm/libs/nfc_api and vm/libs/nfc_cl in the ADK6

CRESCENDO_CUSTOMER_LIBS_FIRMWARE_MODULES  = 
					
PRESTO_CUSTOMER_LIBS_FIRMWARE_MODULES  = nfc_api nfc_cl
					
    



