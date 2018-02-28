/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

This code allows the user to read the dkcs file header.
This code may be removed once a suitable trap is added to perform this functionality.
*/

#ifndef LIBS_CHAIN_DKCS_READER_H_
#define LIBS_CHAIN_DKCS_READER_H_

#include <csrtypes.h>

typedef struct _dkcs_header
{
    /* The header chip ID field */
    uint16 chip_id;
    /* The header build ID field */
    uint32 build_id;
    /* The number of kdcs in the file */
    uint16 num_kdcs;
    /* The number of capability IDs in all kdcs defined in the header */
    uint32 num_cap_ids;
    /* A list of capability IDs in all kdcs defined in the header */
    uint16 capability_ids[1];
} dkcs_header_t;

/* Read and return the kdcs header. Panics if there are any exceptions. */
dkcs_header_t *dkcsHeaderRead(const char* filename);

/* Free the kdcs header. */
void dkcsHeaderFree(dkcs_header_t *header);

#endif
