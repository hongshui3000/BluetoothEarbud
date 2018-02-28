/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file 
 * Hidden data (possibly used by the slt)
 */

#include "slt/slt_private.h"


/*lint -esym(750, SLT_DATA) devilishly clever goings on here */
#define SLT_DATA(m,n) \
    SLT_DATA_EXPAND_##m n


/*lint -esym(750, SLT_DATA_EXPAND_DATA) wow, this is clever */
#define SLT_DATA_EXPAND_DATA(n, v) const n = v;

SLT_DATA_LIST(DATA)
