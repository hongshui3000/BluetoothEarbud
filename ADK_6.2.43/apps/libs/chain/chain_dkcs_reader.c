/****************************************************************************
Copyright (c) 2017 Qualcomm Technologies International, Ltd.
Part of 6.2

DESCRIPTION
    Chain library dkcs file header parser.

    Kymera Capability Storage (KCS) files are used to store one or more capabilities
    that can be dynamically linked into Kymera at runtime.
    A DKCS file is a downloadable KCS file.
    A EDCKS file is a signed and/or encrypted DKCS file. It is prepended with an
    extra header defining: if signing/encryption are enabled, a signed hash and
    encryption nonce.

    A KDC (Kymera Downloadable Capabilities) file contains the necessary information
    for the Kymera dynamic linker to populate the Kalimba program and data memory
    banks with the contents of the program. It also contains the relocation information.
*/

#include "chain_dkcs_reader.h"

#include <string.h>
#include <stdlib.h>
#include <file.h>
#include <source.h>
#include <stream.h>
#include <panic.h>
#include <hydra_macros.h>
#include <print.h>

/* Start index when parsing the DKCS file header */
#define DKCS_CHIP_ID_INDEX 0
/* Start index when parsing the EDKCS file header */
#define EDKCS_CHIP_ID_INDEX 40
/* The EDKCS file header starts with this special chip ID field */
#define EDKCS_CHIP_ID 0xAAAA


/* Read uint16 from the file, incrementing index as data is read */
static uint16 readUint16(const uint8 *data, uint16 len, uint16 *index)
{
    uint8 lso, mso;
    uint16 i = *index;
    if (!data)
    {
        Panic();
    }
    PanicFalse(len > (*index + 1));
    mso = data[i++];
    lso = data[i++];
    *index = i;
    return (uint16)UINT16_BUILD(mso, lso);
}

/* Read uint32 from the file, incrementing index as data is read */
static uint32 readUint32(const uint8 *data, uint16 len, uint16 *index)
{
    uint16 msw = readUint16(data, len, index);
    uint16 lsw = readUint16(data, len, index);
    return UINT32_BUILD(msw, lsw);
}

/* Each kdc has a number of capabilities. Extend the header allocation to account
   for the capabilities in each kdc */
static dkcs_header_t *updateHeaderAllocation(dkcs_header_t *header, uint16 num_capabilities_in_kdc)
{
    size_t realloc_size = sizeof(*header) + ((header->num_cap_ids + num_capabilities_in_kdc - 1) * 
                                             sizeof(header->capability_ids[0]));
    return PanicNull(realloc(header, realloc_size));
}

static Source getFileSrc(const char* filename)
{
    FILE_INDEX index;
    Source src = NULL;

    index = FileFind(FILE_ROOT, filename, (uint16)strlen(filename));
    if (index != FILE_NONE)
    {
        src = StreamFileSource(index);
    }
    return src;
}

/* Read and return the kdc header. */
dkcs_header_t *dkcsHeaderRead(const char* filename)
{
    uint16 len, kdc, index, cap, num_capabilities_in_kdc;
    const uint8 *data;
    Source src;
    dkcs_header_t *header = NULL;

    src = getFileSrc(filename);
    if (src)
    {
        header = calloc(1, sizeof(*header));
        PanicNull(header);

        len = SourceSize(src);
        data = SourceMap(src);

        index = DKCS_CHIP_ID_INDEX;
        header->chip_id = readUint16(data, len, &index);
        if (EDKCS_CHIP_ID == header->chip_id)
        {
            /* This file has the additional EDKCS header. Skip over this and
               continue reading after the EDKCS header */
            index = EDKCS_CHIP_ID_INDEX;
            header->chip_id = readUint16(data, len, &index);
        }
        header->build_id = readUint32(data, len, &index);
        header->num_kdcs = readUint16(data, len, &index);
        header->num_cap_ids = 0;
        for (kdc = 0; kdc < header->num_kdcs; kdc++)
        {
            /* dummy read to move over kdc offset */
            readUint32(data, len, &index);

            num_capabilities_in_kdc = readUint16(data, len, &index);

            header = updateHeaderAllocation(header, num_capabilities_in_kdc);
            
            for (cap = 0; cap < num_capabilities_in_kdc; cap++)
            {
                header->capability_ids[header->num_cap_ids + cap] = readUint16(data, len, &index);
            }
            header->num_cap_ids += num_capabilities_in_kdc;
        }

        SourceClose(src);
    }
    return header;
}

/* Free the kdcs header. */
void dkcsHeaderFree(dkcs_header_t *header)
{
    free(header);
}
