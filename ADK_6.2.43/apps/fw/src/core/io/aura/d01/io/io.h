
/* ***************************************************************************
   COMMERCIAL IN CONFIDENCE
   Copyright (C) 2017 Qualcomm Technologies International Ltd.

   Qualcomm Technologies International Ltd.
   Churchill House,
   Cambridge Business Park,
   Cowley Park,
   Cambridge, CB4 0WZ. UK
   http://www.csr.com

   DESCRIPTION
      Auto-generated memory-mapped register include file. The file included depends on
      the definition of USE_IO_MAP_PUBLIC. This specifies whether the
      addresses of the registers are used directly or pointers to them.

****************************************************************************/

#ifndef __IO_H__
#define __IO_H__

#include "common/types.h"

/* Include the appropriate memory-mapped register file */
#ifdef USE_IO_MAP_PUBLIC
#include "io/io_map_public.h"
#else
#include "io/io_map.h"
#endif

#include "io/io_defs.h"

#endif /* __IO_H__ */

