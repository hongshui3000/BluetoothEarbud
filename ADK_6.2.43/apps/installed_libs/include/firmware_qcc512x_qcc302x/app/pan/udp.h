/******************************************************************************

    file udp.h  --  User Datagram Protocol

Copyright (c) 2003 - 2016 Qualcomm Technologies International, Ltd.
  Part of 6.2

    $Log: udp.h,v $

******************************************************************************/


#ifndef UDP_H
#define UDP_H


#include "pantypes.h"
#include "ip.h"
#include "ipv4.h" 


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
    UDP_CLOSE_REQ = UDP_PRIM_BASE + 10,
    UDP_CLOSE_CFM,
    IPV4_CONFIGURE_REQ = UDP_PRIM_BASE + 0x101 
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
    enum UDP_OPEN_CFM_STATUS status;
    Source source;
    Sink sink;
} UDP_OPEN_CFM_T;

typedef struct UDP_LISTEN_REQ_T
{
    uint16 type;
    Sink sink;
    IP_ADDR loc_addr; /* Must be multicast */
} UDP_LISTEN_REQ_T;

typedef struct UDP_IGNORE_REQ_T
{
    uint16 type;
    Sink sink;
    IP_ADDR loc_addr;
} UDP_IGNORE_REQ_T;

typedef struct UDP_METADATA
{
    IP_ADDR rem_addr; /* Must match */
    uint16 rem_port;  /* UDP_EXTENDED_DATA_REQ/IND_T */
    IP_ADDR loc_addr; /* exactly */
} UDP_METADATA;


typedef struct UDP_CLOSE_REQ_T
{
    uint16 type;
    Sink sink;
} UDP_CLOSE_REQ_T;

typedef struct UDP_CLOSE_CFM_T
{
    uint16 type;
    phandle_t phandle;
    Sink sink;
} UDP_CLOSE_CFM_T;

typedef struct IPV4_CONFIGURE_REQ_T 
{
    uint16 type;
    IPV4_ADDR autoconf_addr;
    IPV4_ADDR autoconf_subnet_mask;
    IPV4_ADDR autoconf_router_addr;
} IPV4_CONFIGURE_REQ_T;


#endif /* UDP_H */
