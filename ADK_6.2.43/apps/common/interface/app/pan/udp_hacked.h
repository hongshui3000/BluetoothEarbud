/******************************************************************************

    file udp.h  --  User Datagram Protocol

Copyright (c) 2003 - 2016 Qualcomm Technologies International, Ltd.
  Part of 6.2
*/


#ifndef UDP_H
#define UDP_H


#include "pantypes.h"
#include "ip.h"


/******************************************************************************

    enum UDP_MSG_TYPE  --  UDP task message types
    typedefs for UDP task messages
    enums for UDP task message parameters

******************************************************************************/

#define UDP_PRIM_BASE 0x0300

#define ENUM_SEP_UDP_FIRST_PRIM UDP_PRIM_BASE
#define ENUM_SEP_UDP_LAST_PRIM (UDP_PRIM_BASE + 12)

enum UDP_MSG_TYPE
{
    UDP_OPEN_REQ = UDP_PRIM_BASE + 1,
    UDP_OPEN_CFM,
    UDP_LISTEN_REQ,
    UDP_IGNORE_REQ,
    UDP_DATA_REQ,
    UDP_EXTENDED_DATA_REQ,
    UDP_DATA_CFM,
    UDP_DATA_IND,
    UDP_EXTENDED_DATA_IND,
    UDP_CLOSE_REQ = UDP_PRIM_BASE + 10,
    UDP_CLOSE_CFM
};

typedef struct UDP_OPEN_REQ_T
{
    uint16 type;
    phandle_t phandle;
    uint16 flags; /* reserved for future use */
    IP_ADDR rem_addr; /* May be unspecified */
    uint16 rem_port; /* May be IN_PORT_ANY or IN_PORT_ZERO */
    uint16 loc_port; /* May be IN_PORT_ANY */
} UDP_OPEN_REQ_T;

enum UDP_OPEN_CFM_STATUS
{
    UDP_OPEN_CFM_STATUS_SUCCESS,
    UDP_OPEN_CFM_STATUS_BADARG,
    UDP_OPEN_CFM_STATUS_NOMEM,
    UDP_OPEN_CFM_STATUS_NOLOCADDR
};
typedef struct UDP_OPEN_CFM_T
{
    uint16 type;
    phandle_t phandle;
    uint16 status;
    uint16 id;
    uint16 reserved;
} UDP_OPEN_CFM_T;

typedef struct UDP_LISTEN_REQ_T
{
    uint16 type;
    uint16 id;
    IP_ADDR loc_addr; /* Must be multicast */
} UDP_LISTEN_REQ_T;

typedef struct UDP_IGNORE_REQ_T
{
    uint16 type;
    uint16 id;
    IP_ADDR loc_addr;
} UDP_IGNORE_REQ_T;

typedef struct UDP_DATA_REQ_T
{
    uint16 type;
    uint16 id;
    uint16 len;
    uint8 *data;
} UDP_DATA_REQ_T;

typedef struct UDP_EXTENDED_DATA_REQ_T
{
    uint16 type;
    IP_ADDR rem_addr;
    uint16 rem_port;
    IP_ADDR loc_addr;
    uint16 id;
    uint16 len;
    uint8 *data;
} UDP_EXTENDED_DATA_REQ_T;

typedef struct UDP_DATA_CFM_T
{
    uint16 type;
    phandle_t phandle;
    uint16 id;
    uint16 len;
} UDP_DATA_CFM_T;

typedef struct UDP_DATA_IND_T
{
    uint16 type;
    phandle_t phandle;
    uint16 id;
    uint16 len;
    uint8 *data;
} UDP_DATA_IND_T;

typedef struct UDP_EXTENDED_DATA_IND_T
{
    uint16 type;
    phandle_t phandle;
    IP_ADDR rem_addr;
    uint16 rem_port;
    IP_ADDR loc_addr;
    uint16 id;
    uint16 len;
    uint8 *data;
} UDP_EXTENDED_DATA_IND_T;

typedef struct UDP_CLOSE_REQ_T
{
    uint16 type;
    uint16 id;
} UDP_CLOSE_REQ_T;

typedef struct UDP_CLOSE_CFM_T
{
    uint16 type;
    phandle_t phandle;
    uint16 id;
} UDP_CLOSE_CFM_T;


#endif /* TCP_H */
