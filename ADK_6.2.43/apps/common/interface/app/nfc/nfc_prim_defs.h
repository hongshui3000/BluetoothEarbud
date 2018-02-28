/****************************************************************************
Copyright (c) 2014 - 2016 Qualcomm Technologies International, Ltd.
  Part of 6.2

*/

#ifndef NFC_PRIM_DEFS_H
#define NFC_PRIM_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif

/* PROJECT INCLUDES **********************************************************/
#ifdef SUBSYSTEM_APPS
#include "hydra/hydra_types.h"
#else
#include <hydra_types.h>
#endif

/* PUBLIC MACROS ************************************************************/

/* PUBLIC TYPES DEFINITIONS *************************************************/
#define MAX_NFCID_SIZE TRIPLE_NFCID
/*
 * A valid NFC UID length can be either 4bytes (SINGLE), 7bytes (DOUBLE) or
 * 10bytes (TRIPLE).
 */

/****************************************************************************
NAME
    NFCID Size definitions
*/
#define NO_NFCID     (0)
#define SINGLE_NFCID (4)
#define DOUBLE_NFCID (7)
#define TRIPLE_NFCID (10)

#define IS_VALID_NFCID_SZ(id_sz) \
(TRIPLE_NFCID == (id_sz) ||  \
 DOUBLE_NFCID == (id_sz) ||  \
 SINGLE_NFCID == (id_sz) ||  \
 NO_NFCID == (id_sz))

/* CT Cascade tag x88 */
#define CASCADE_TAG (0x88)

/* Dynamic TAG SINGLE SIZE */
#define SINGLE_NFCID_DYNAMIC (0x08)

/* Single UID RFU - Do not use */
#define SINGLE_NFCID_RFU (0xF8)

/* Double and Triple UID private range */
#define NFCID_PRIVATE_MIN (0x81)
#define NFCID_PRIVATE_MAX (0xFE)

/* TT2_MAX_NDEF_PRIM - The maximum size of TAG Type 2 NDEF message
 * Note: This is same as TT2_MAX_NDEF=235 in the NFC HAL */
#define TT2_MAX_NDEF (235)


/***************************************************************************
NAME
    NRC_SERVICE_TYPE  -  HAL NFC Service type definition

DESCRIPTION
    Defines the HAL service associated with a particular NFC TARGET

*/
typedef enum nrc_service_type_enum
{
    NRC_SERVICE_P2P = 0,    /*!< NFC DEP protocol */
    NRC_SERVICE_TAG_TYPE_2, /*!< Tag Type 2 Protocol */
    NRC_SERVICE_TAG_TYPE_4  /*!< Not available on P1 app - ISO DEP protocol */
} NRC_SERVICE_TYPE;

#ifdef __cplusplus
#endif

/** @}*/
#endif /* NFC_PRIM_DEFS_H */

