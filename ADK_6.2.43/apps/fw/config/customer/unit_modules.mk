# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version


CUSTOMER_MODULES = $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, $(foreach MODULE, ${CUSTOMER_$(strip $(call uc, ${SUBCOMPONENT}))_COMMON_MODULES}, ${MODULE}))

CUSTOMER_MODULE_PATHS = $(foreach SUBCOMPONENT, ${SUBCOMPONENTS}, $(foreach MODULE, ${CUSTOMER_$(strip $(call uc, ${SUBCOMPONENT}))_COMMON_MODULES}, ${SUBCOMPONENT}/${MODULE}))

