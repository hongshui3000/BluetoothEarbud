# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
# List the modules to build for the core component of the various P1 builds. 
# NOTE: COMMON_MODULES should contain only modules that are really built for
# all builds (except for the digits build, which is a law unto itself).  
# For modules shared between a few builds, define a suitable macro and add
# it to each of the "gathering" makefiles (firmware_modules.mk, etc)



COMMON_MODULES = \
 sched_oxygen \
 hydra_log \
 pl_timers \
 appcmd \
 itime_kal \
 trap_version

FIRMWARE_MODULES = \
 buffer \
 cache \
 crt \
 dorm \
 excep \
 fault \
 id \
 int \
 io \
 ipc \
 hal \
 led \
 led_cfg \
 kal_utils \
 longtimer \
 memprot \
 panic \
 pio \
 piodebounce \
 pioint \
 pmalloc \
 slt
 
CRESCENDO_FIRMWARE_MODULES = \
 

PRESTO_FIRMWARE_MODULES = \
 

DESKTOP_COMMON_MODULES = \
  coal \
  verdict \
  stub \
  ${OS}

UNIT_MODULES = \
  unit \
  buffer \
  ccp \
  hydra_service \
  bc_buf_adapter \
  ps_adapter \
  hostio \
  hostio_usb_common \
  hostio_usb_hydra \
  hostio_usb_test_device

COAL_MODULES = \
  coal_test
