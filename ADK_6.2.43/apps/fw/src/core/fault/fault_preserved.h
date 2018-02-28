/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */
/**
 * \file
 * Preserved memory used for groaning quietly
 *
 */

#ifndef FAULT_PRESERVED_H
#define FAULT_PRESERVED_H

#include "fault/fault.h"


/** Static preserved data for recording fault status */
typedef struct fault_preserved
{
    /* Variable in which the last fault reason code and arg is stored */
    faultid last_id;
    uint16 last_arg;

} fault_preserved;
#define FAULT_PRESERVED_DATA(m) PRESERVED_DATA(m, (fault_preserved, fault))


#endif /* FAULT_PRESERVED_H */
