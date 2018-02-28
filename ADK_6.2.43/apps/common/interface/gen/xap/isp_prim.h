/*****************************************************************************

            Copyright (c) 2018 Qualcomm Technologies International, Ltd.

            WARNING: This is an auto-generated file!
                     DO NOT EDIT!

*****************************************************************************/
#ifndef ISP_PRIM_H__
#define ISP_PRIM_H__

#include "hydra/hydra_types.h"


/*******************************************************************************

  NAME
    IspAddr

  DESCRIPTION

 VALUES
    curator  - Curator always at address 0
    host     - Host entities at address 16-30. Those from 16 to 23 route through
               the host transport. Addresses from 24 to 30 route through the
               TRBr Router.
    trb_host - Host entities at address 24-30 are routed through the TRBr
    max      - The highest possible ISP address

*******************************************************************************/
typedef enum
{
    ISP_ADDR_CURATOR = 0,
    ISP_ADDR_HOST = 16,
    ISP_ADDR_TRB_HOST = 24,
    ISP_ADDR_MAX = 31
} ISP_ADDR;
/*******************************************************************************

  NAME
    IspPortMappingEnum

  DESCRIPTION
    Default mapping of protocols to ports - needed for Omnicli testing

 VALUES
    CCP              - Chip Control Protocol is always on port 0
    FTP              - File transfer protocol command port
    TestTunnel       - Port for Hydra TestTunnel as described in CS-308599-SP
    TrbHostTransport - Port for ISP out-band signalling for TrB host transport
    Max_Static       - The largest port value allocated by one of the entries
                       above. This is not a port type in its own right.  Any new
                       port should be added before this entry and the value of
                       this entry incremented.
    FTP_Data         - File transfer protocol data port
    AuxCmd           - Aux command channel
    any              - Port 31 is reserved as a wildcard and is used within the
                       firmware submsg API
    MibCmd           - Port to allow read/write of MIB
    DebugCmd         - Curator test port
    Hostio           - Port for internal chip (firmware) messages for
                       hostio_sdio for raising host interrupts
    Coexistence      - Port for Coexistence Messaging
    ToolCmd          - Port for tools that request curator to perform memory
                       read/write
    Unitest          - Port for Hydra Unitest as described in CS-228957-SP

*******************************************************************************/
typedef enum
{
    ISP_PORT_MAPPING_ENUM_CCP = 0,
    ISP_PORT_MAPPING_ENUM_FTP = 1,
    ISP_PORT_MAPPING_ENUM_FTP_DATA = 2,
    ISP_PORT_MAPPING_ENUM_AUXCMD = 3,
    ISP_PORT_MAPPING_ENUM_MIBCMD = 4,
    ISP_PORT_MAPPING_ENUM_DEBUGCMD = 5,
    ISP_PORT_MAPPING_ENUM_HOSTIO = 6,
    ISP_PORT_MAPPING_ENUM_COEXISTENCE = 7,
    ISP_PORT_MAPPING_ENUM_TOOLCMD = 8,
    ISP_PORT_MAPPING_ENUM_UNITEST = 9,
    ISP_PORT_MAPPING_ENUM_TESTTUNNEL = 10,
    ISP_PORT_MAPPING_ENUM_TRBHOSTTRANSPORT = 11,
    ISP_PORT_MAPPING_ENUM_MAX_STATIC = 11,
    ISP_PORT_MAPPING_ENUM_ANY = 31
} ISP_PORT_MAPPING_ENUM;


#define ISP_PRIM_ANY_SIZE 1

/*******************************************************************************

  NAME
    isp_header

  DESCRIPTION
    ISP Header first word

  MEMBERS
    dest_address - destination address
    src_address  - source address
    dest_port    - destination port
    E            - Header extension bit. Extended when E=1

*******************************************************************************/
typedef struct
{
    uint16 _data[1];
} ISP_HEADER;

/* The following macros take ISP_HEADER *isp_header_ptr */
#define ISP_HEADER_DEST_ADDRESS_WORD_OFFSET (0)
#define ISP_HEADER_DEST_ADDRESS_GET(isp_header_ptr) ((ISP_ADDR)(((isp_header_ptr)->_data[0] & 0x1f)))
#define ISP_HEADER_DEST_ADDRESS_SET(isp_header_ptr, dest_address) ((isp_header_ptr)->_data[0] =  \
                                                                       (uint16)(((isp_header_ptr)->_data[0] & ~0x1f) | (((dest_address)) & 0x1f)))
#define ISP_HEADER_SRC_ADDRESS_GET(isp_header_ptr) ((ISP_ADDR)(((isp_header_ptr)->_data[0] & 0x3e0) >> 5))
#define ISP_HEADER_SRC_ADDRESS_SET(isp_header_ptr, src_address) ((isp_header_ptr)->_data[0] =  \
                                                                     (uint16)(((isp_header_ptr)->_data[0] & ~0x3e0) | (((src_address) << 5) & 0x3e0)))
#define ISP_HEADER_DEST_PORT_GET(isp_header_ptr) ((ISP_PORT_MAPPING_ENUM)(((isp_header_ptr)->_data[0] & 0x7c00) >> 10))
#define ISP_HEADER_DEST_PORT_SET(isp_header_ptr, dest_port) ((isp_header_ptr)->_data[0] =  \
                                                                 (uint16)(((isp_header_ptr)->_data[0] & ~0x7c00) | (((dest_port) << 10) & 0x7c00)))
#define ISP_HEADER_E_GET(isp_header_ptr) ((((isp_header_ptr)->_data[0] & 0x8000) >> 15))
#define ISP_HEADER_E_SET(isp_header_ptr, e) ((isp_header_ptr)->_data[0] =  \
                                                 (uint16)(((isp_header_ptr)->_data[0] & ~0x8000) | (((e) << 15) & 0x8000)))
#define ISP_HEADER_WORD_SIZE (1)
/*lint -e(773) allow unparenthesized*/
#define ISP_HEADER_CREATE(dest_address, src_address, dest_port, E) \
    (uint16)(((dest_address)) & 0x1f) | \
    (uint16)(((src_address) << 5) & 0x3e0) | \
    (uint16)(((dest_port) << 10) & 0x7c00) | \
    (uint16)(((E) << 15) & 0x8000)
#define ISP_HEADER_PACK(isp_header_ptr, dest_address, src_address, dest_port, E) \
    do { \
        (isp_header_ptr)->_data[0] = (uint16)((uint16)(((dest_address)) & 0x1f) | \
                                              (uint16)(((src_address) << 5) & 0x3e0) | \
                                              (uint16)(((dest_port) << 10) & 0x7c00) | \
                                              (uint16)(((E) << 15) & 0x8000)); \
    } while (0)

#define ISP_HEADER_MARSHALL(addr, isp_header_ptr) memcpy((void *)(addr), (void *)(isp_header_ptr), 1)
#define ISP_HEADER_UNMARSHALL(addr, isp_header_ptr) memcpy((void *)(isp_header_ptr), (void *)(addr), 1)


/*******************************************************************************

  NAME
    isp_extended_header

  DESCRIPTION
    ISP Header optional second word

  MEMBERS
    normal_header - First header word
    src_port      - source port
    Spare         -

*******************************************************************************/
typedef struct
{
    uint16 _data[2];
} ISP_EXTENDED_HEADER;

/* The following macros take ISP_EXTENDED_HEADER *isp_extended_header_ptr */
#define ISP_EXTENDED_HEADER_NORMAL_HEADER_WORD_OFFSET (0)
#define ISP_EXTENDED_HEADER_NORMAL_HEADER_GET(isp_extended_header_ptr, normal_header_ptr) do {  \
        (normal_header_ptr)->_data[0] = (isp_extended_header_ptr)->_data[0]; } while (0)
#define ISP_EXTENDED_HEADER_NORMAL_HEADER_SET(isp_extended_header_ptr, normal_header_ptr) do {  \
        (isp_extended_header_ptr)->_data[0] = (normal_header_ptr)->_data[0]; } while (0)
#define ISP_EXTENDED_HEADER_SRC_PORT_WORD_OFFSET (1)
#define ISP_EXTENDED_HEADER_SRC_PORT_GET(isp_extended_header_ptr) ((((isp_extended_header_ptr)->_data[1] & 0x1f)))
#define ISP_EXTENDED_HEADER_SRC_PORT_SET(isp_extended_header_ptr, src_port) ((isp_extended_header_ptr)->_data[1] =  \
                                                                                 (uint16)(((isp_extended_header_ptr)->_data[1] & ~0x1f) | (((src_port)) & 0x1f)))
#define ISP_EXTENDED_HEADER_WORD_SIZE (2)
/*lint -e(773) allow unparenthesized*/
#define ISP_EXTENDED_HEADER_CREATE(normal_header, src_port) \
    (uint16), \
    (uint16)(((src_port)) & 0x1f)
#define ISP_EXTENDED_HEADER_PACK(isp_extended_header_ptr, normal_header_ptr, src_port) \
    do { \
        (isp_extended_header_ptr)->_data[0] = (uint16)((normal_header_ptr)->_data[0]); \
        (isp_extended_header_ptr)->_data[1] = (uint16)((uint16)(((src_port)) & 0x1f)); \
    } while (0)

#define ISP_EXTENDED_HEADER_MARSHALL(addr, isp_extended_header_ptr) memcpy((void *)(addr), (void *)(isp_extended_header_ptr), 2)
#define ISP_EXTENDED_HEADER_UNMARSHALL(addr, isp_extended_header_ptr) memcpy((void *)(isp_extended_header_ptr), (void *)(addr), 2)


#endif /* ISP_PRIM_H__ */

