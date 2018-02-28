/******************************************************************************

    file bnep.h  --  Bluetooth Network Encapsulation Protocol

Copyright (c) 2002 - 2016 Qualcomm Technologies International, Ltd.
  Part of 6.2
*/


#ifndef BNEP_H
#define BNEP_H


#include "pantypes.h"


/******************************************************************************

    #defines for BNEP magic numbers

******************************************************************************/

#define BNEP_UUID_PANU 0x1115 /* http://www.bluetooth.org/assigned-numbers/sdp.htm */
#define BNEP_UUID_NAP 0x1116 /* http://www.bluetooth.org/assigned-numbers/sdp.htm */
#define BNEP_UUID_GN 0x1117 /* http://www.bluetooth.org/assigned-numbers/sdp.htm */


/******************************************************************************

    struct ETHER_ADDR  --  an Ethernet address

******************************************************************************/

typedef struct ETHER_ADDR
{
    uint16 w[3]; /* Big-endian */
} ETHER_ADDR;


/******************************************************************************

    enum BNEP_MSG_TYPE  --  BNEP task message types
    typedefs for BNEP task messages
    enums for BNEP task message parameters

******************************************************************************/

#define BNEP_PRIM_BASE 0x00fe

#define ENUM_SEP_BNEP_FIRST_PRIM BNEP_PRIM_BASE
#define ENUM_SEP_BNEP_LAST_PRIM (BNEP_PRIM_BASE + 23)

enum BNEP_MSG_TYPE
{
    BNEP_FLOW_REQ = BNEP_PRIM_BASE + 1,
    BNEP_FLOW_IND, /* HERE: rename to PAN_FLOW_* */
    BNEP_REGISTER_REQ,
    BNEP_CONNECT_REQ,
    BNEP_CONNECT_CFM,
    BNEP_CONNECT_IND,
    BNEP_LISTEN_REQ,
    BNEP_IGNORE_REQ,
    BNEP_UP_IND,
    BNEP_DOWN_IND,
    BNEP_DATA_REQ,
    BNEP_EXTENDED_DATA_REQ,
    BNEP_EXTENDED_MULTICAST_DATA_REQ,
    BNEP_DATA_IND,
    BNEP_EXTENDED_DATA_IND,
    BNEP_MULTICAST_DATA_IND,
    BNEP_LOOPBACK_DATA_IND,
    BNEP_DISCONNECT_REQ,
    BNEP_DISCONNECT_IND,
    BNEP_DEBUG_REQ,
    BNEP_DEBUG_IND,
    BNEP_HCI_SWITCH_ROLE_RSP
};

#define PAN_FLOW_REQ BNEP_FLOW_REQ
#define PAN_FLOW_IND BNEP_FLOW_IND

typedef struct BNEP_REGISTER_REQ_T
{
    uint16 type;
    uint16 flags;
    phandle_t phandle;
} BNEP_REGISTER_REQ_T;

typedef struct BNEP_CONNECT_REQ_T
{
    uint16 type;
    uint16 flags;
    ETHER_ADDR rem_addr; /* set msw to ETHER_UNKNOWN for passive */
} BNEP_CONNECT_REQ_T;

enum BNEP_CONNECT_CFM_STATUS
{
    BNEP_CONNECT_CFM_STATUS_SUCCESS,
    BNEP_CONNECT_CFM_STATUS_BADARG,
    BNEP_CONNECT_CFM_STATUS_NOMEM,
    BNEP_CONNECT_CFM_STATUS_CONFLICT,
    BNEP_CONNECT_CFM_STATUS_TRYLATER
};
typedef struct BNEP_CONNECT_CFM_T
{
    uint16 type;
    uint16 status;
} BNEP_CONNECT_CFM_T;

enum BNEP_CONNECT_IND_STATUS
{
    /*
     * Some of these must match the L2CA_CONNECTION codes in l2cap_prim.h.
     */
    BNEP_CONNECT_IND_STATUS_SUCCESSFUL_OUT = 0x0000, /* must match */
    BNEP_CONNECT_IND_STATUS_REJ_PSM = 0x0002, /* must match */
    BNEP_CONNECT_IND_STATUS_REJ_SECURITY = 0x0003, /* must match */
    BNEP_CONNECT_IND_STATUS_REJ_RESOURCES = 0x0004, /* must match */
    BNEP_CONNECT_IND_STATUS_NOT_MASTER = (int) 0xdda5,
    BNEP_CONNECT_IND_STATUS_ABEND = (int) 0xddab,
    BNEP_CONNECT_IND_STATUS_REJ_ROLES = (int) 0xddb2, /* HERE: pack in reason? */
    BNEP_CONNECT_IND_STATUS_REJ_NOT_READY = (int) 0xdddd, /* must match */
    BNEP_CONNECT_IND_STATUS_FAILED = (int) 0xddde, /* must match */
    BNEP_CONNECT_IND_STATUS_TIMEOUT = (int) 0xeeee, /* must match */
    BNEP_CONNECT_IND_STATUS_SUCCESSFUL_IN = (int) 0xffff
};
typedef struct BNEP_CONNECT_IND_T
{
    uint16 type;
    uint16 status;
    uint16 id;
    ETHER_ADDR rem_addr;
    uint16 rem_uuid;
    uint16 loc_uuid;
} BNEP_CONNECT_IND_T;

typedef struct BNEP_LISTEN_REQ_T
{
    uint16 type;
    phandle_t phandle;
    uint16 ether_type;
    ETHER_ADDR addr;
} BNEP_LISTEN_REQ_T;

typedef struct BNEP_IGNORE_REQ_T
{
    uint16 type;
    uint16 ether_type;
    ETHER_ADDR addr;
} BNEP_IGNORE_REQ_T;

typedef struct BNEP_DATA_REQ_T
{
    uint16 type;
    uint16 ether_type;
    ETHER_ADDR rem_addr; /* HERE: this could be multicast -- probably get BNEP to transform into a BNEP_EXTENDED_DATA_REQ if not PANU */
    MBLK_T *mblk;
    uint16 len; /* added */
} BNEP_DATA_REQ_T;

typedef struct BNEP_EXTENDED_DATA_REQ_T
{
    uint16 type;
    uint16 id; /* HERE: if 0 then to local stack, 1 to go to dst_addr */
    uint16 ether_type;
    ETHER_ADDR dst_addr; /* typically a PANU's address, but allow future exp */
    ETHER_ADDR src_addr;
    MBLK_T *mblk;
    uint16 len; /* added */
} BNEP_EXTENDED_DATA_REQ_T;

typedef struct BNEP_EXTENDED_MULTICAST_DATA_REQ_T
{
    uint16 type;
    uint16 id_not; /* HERE: if 0 then not to local stack, 1 to go to all */
    uint16 ether_type;
    ETHER_ADDR dst_addr;
    ETHER_ADDR src_addr;
    MBLK_T *mblk;
    uint16 len; /* added */
} BNEP_EXTENDED_MULTICAST_DATA_REQ_T;

typedef struct BNEP_DATA_IND_T
{
    uint16 type;
    phandle_t phandle;
    MBLK_T *mblk;
    uint16 len; /* added */
} BNEP_DATA_IND_T;

typedef struct BNEP_EXTENDED_DATA_IND_T
{
    uint16 type;
    uint16 id; /* HERE: if 0 then from local stack */
    uint16 ether_type;
    ETHER_ADDR dst_addr; /* note may be multicast */
    ETHER_ADDR src_addr; /* should be a PANU's address, but who knows? */
    MBLK_T *mblk;
    uint16 len; /* added */
} BNEP_EXTENDED_DATA_IND_T;

typedef struct BNEP_MULTICAST_DATA_IND_T
{
    uint16 type;
    phandle_t phandle;
    MBLK_T *mblk;
    uint16 len; /* added */
} BNEP_MULTICAST_DATA_IND_T;

typedef struct BNEP_LOOPBACK_DATA_IND_T
{
    uint16 type;
    phandle_t phandle;
    MBLK_T *mblk;
    uint16 len; /* added */
} BNEP_LOOPBACK_DATA_IND_T;

typedef struct BNEP_UP_IND_T
{
    uint16 type;
    phandle_t phandle;
    uint16 rem_uuid;
    uint16 loc_uuid;
} BNEP_UP_IND_T;

typedef struct BNEP_DOWN_IND_T /* This must be compatible with BNEP_UP_IND_T */
{
    uint16 type;
    phandle_t phandle;
} BNEP_DOWN_IND_T;

typedef struct BNEP_DISCONNECT_REQ_T
{
    uint16 type;
    uint16 flags;
    uint16 id;
} BNEP_DISCONNECT_REQ_T;

enum BNEP_DISCONNECT_IND_REASON
{
    /* 0-255 reserved in case one day we get HCI reason codes from L2CAP */
    BNEP_DISCONNECT_IND_REASON_ABEND = 0x0100,
    BNEP_DISCONNECT_IND_REASON_DONE = 0x0101,
    BNEP_DISCONNECT_IND_REASON_NOT_MASTER = 0x0102
};
typedef struct BNEP_DISCONNECT_IND_T
{
    uint16 type;
    uint16 id;
    uint16 reason;
} BNEP_DISCONNECT_IND_T;

typedef struct BNEP_DEBUG_REQ_T
{
    uint16 type;
    uint16 id;
    uint16 test;
    uint16 args[18];
} BNEP_DEBUG_REQ_T;

typedef struct BNEP_DEBUG_IND_T
{
    uint16 type;
    uint16 id;
    uint16 test;
    uint16 args[18];
} BNEP_DEBUG_IND_T;

typedef struct BNEP_FLOW_REQ_T
{
    uint16 type;
} BNEP_FLOW_REQ_T;
#define PAN_FLOW_REQ_T BNEP_FLOW_REQ_T

typedef struct BNEP_FLOW_IND_T
{
    uint16 type;
    uint16 free;
} BNEP_FLOW_IND_T;
#define PAN_FLOW_IND_T BNEP_FLOW_IND_T

typedef struct BNEP_HCI_SWITCH_ROLE_RSP_T /* DM_HCI_SWITCH_ROLE_COMPLETE_T */
{
    uint16 type;
    phandle_t phandle; /* unused */
    BD_ADDR_T bd_addr;
    uint8 status;
    uint8 role;
} BNEP_HCI_SWITCH_ROLE_RSP_T;


/******************************************************************************

    #defines and macros for Ethernet addressing

******************************************************************************/

#define ETHER_ADDR_LEN 6 /* in octets */
#define ETHER_MTU 1500 /* excluding header, CRC and any 802.1Q header */
#define ETHER_LOOPBACK 0x1bac /* First 16 bits -- note this is locally-assigned and multicast to ensure it won't be in use on the network */
#define ETHER_UNKNOWN 0x0b0f /* Ditto */
#define ETHER_MULTICAST_IPV4 0x01005e /* First 24 bits, and then a zero bit */
#define ETHER_MULTICAST_IPV6 0x3333 /* First 16 bits */
#define ETHER_BROADCAST_IPV4 0xffff /* Replicated across all 48 bits */
#define ETHER_TYPE_IPV4 0x0800
#define ETHER_TYPE_ARP 0x0806
#define ETHER_TYPE_8021Q 0x8100
#define ETHER_TYPE_IPV6 0x86dd
#define ETHER_IS_LOOPBACK(x) ((x)->w[0] == ETHER_LOOPBACK)
#define ETHER_IS_UNKNOWN(x) ((x)->w[0] == ETHER_UNKNOWN)
#define ETHER_IS_BROADCAST(x) (( \
                                ~(x)->w[0] | \
                                ~(x)->w[1] | \
                                ~(x)->w[2] \
                               ) == 0)
#define ETHER_IS_MULTICAST(x) (((x)->w[0] & 0x0100) != 0)
#define ETHER_IS_SAME_ADDR(x,y) (( \
                                 ((x)->w[0] ^ (y)->w[0]) | \
                                 ((x)->w[1] ^ (y)->w[1]) | \
                                 ((x)->w[2] ^ (y)->w[2]) \
                               ) == 0)


#endif /* BNEP_H */
