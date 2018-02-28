# List the modules to build for the customer component of the various P1 
# builds.

SUBCOMPONENTS = \
           core \
           bt \
           wlan \
           nfc
#           apps

# Module variables must be named following this syntax: 
# ${COMPONENT}_${SUBCOMPONENT}_MODULES
CUSTOMER_CORE_COMMON_MODULES = \
 trap_api \
 init

CUSTOMER_CORE_FIRMWARE_MODULES = \
 main
# usb_device_class

CUSTOMER_BT_FIRWARE_MODULES =

CUSTOMER_WLAN_FIRMWARE_MODULES =

CUSTOMER_NFC_FIRMWARE_MODULES =

CUSTOMER_APPS_FIRMWARE_MODULES = 
# usb_audio_loopback


INC += $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, -Icustomer/${SUBCOMPONENT})
INC += $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, -I${BUILD_DIR}/customer/${SUBCOMPONENT})
