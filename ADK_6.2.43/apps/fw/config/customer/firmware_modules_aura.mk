# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# Gather the full set of modules for the customer firmware build.
# NOTE: do not add ad hoc modules to CUSTOMER_MODULES in this file - add them
# to the appropriate macro in ../config/customer/modules.mk instead.  This
# keeps all the names in one place and makes it much easier to see what's what.


include ../config/customer/modules_aura.mk
include ../config/customer/firmware_modules.mk

