/******************************************************************************
Copyright (c) 2002 - 2016 Qualcomm Technologies International, Ltd.
  Part of 6.2

    file bnep.h  --  Bluetooth Network Encapsulation Protocol

******************************************************************************/


#ifndef BNEP_H
#define BNEP_H


#include "pantypes.h"
#include "app/bluestack/hci.h" /* for SIZE_LINK_KEY */


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
#define ENUM_SEP_BNEP_LAST_PRIM (BNEP_PRIM_BASE + 25)

enum BNEP_MSG_TYPE
{
    BNEP_REGISTER_REQ = BNEP_PRIM_BASE + 3,
    BNEP_CONNECT_REQ,
    BNEP_CONNECT_CFM,
    BNEP_CONNECT_IND,
    BNEP_DISCONNECT_REQ = BNEP_PRIM_BASE + 18,
    BNEP_DISCONNECT_IND,
    BNEP_HCI_SWITCH_ROLE_RSP = BNEP_PRIM_BASE + 22,
    BNEP_ROLE_CHANGE_IND,
    BNEP_BM_IND
};
#define PAN_FLOW_REQ BNEP_FLOW_REQ
#define PAN_FLOW_IND BNEP_FLOW_IND

#ifndef BTCHIP
#define BNEP_REGISTER_REQ_FLAG_DISABLE_STACK 0x0001
#endif

typedef struct BNEP_REGISTER_REQ_T
{
    uint16 type;
#ifdef BTCHIP
    unsigned int : 13;
    unsigned int disable_extended : 1;
    unsigned int manual_bridge : 1; /* reserved for future use */
    unsigned int disable_stack : 1;
#else
    uint16 flags;
#endif
    phandle_t phandle;
} BNEP_REGISTER_REQ_T;

#ifndef BTCHIP
#define BNEP_CONNECT_REQ_FLAG_RNAP 0x4000
#define BNEP_CONNECT_REQ_FLAG_RGN 0x2000
#define BNEP_CONNECT_REQ_FLAG_RPANU 0x1000
#define BNEP_CONNECT_REQ_FLAG_LNAP 0x0400
#define BNEP_CONNECT_REQ_FLAG_LGN 0x0200
#define BNEP_CONNECT_REQ_FLAG_LPANU 0x0100
#define BNEP_CONNECT_REQ_FLAG_NO_SWITCH 0x0004
#define BNEP_CONNECT_REQ_FLAG_SINGLE_USER 0x0002
#define BNEP_CONNECT_REQ_FLAG_ACCEPT_IN 0x0001
#else
typedef struct BNEP_CONNECT_REQ_FLAGS
{
    unsigned int reserved15 : 1;
    unsigned int rnap : 1;
    unsigned int rgn : 1;
    unsigned int rpanu : 1;
    unsigned int reserved11 : 1;
    unsigned int lnap : 1;
    unsigned int lgn : 1;
    unsigned int lpanu : 1;
    /* Not all combinations are valid! */
    unsigned int reserved7 : 1;
    unsigned int reserved6 : 1;
    unsigned int reserved5 : 1;
    unsigned int persist : 1; /* reserved for future use */
    unsigned int on_demand : 1; /* reserved for future use */
    unsigned int no_switch : 1;
    unsigned int single_user : 1;
    unsigned int accept_in : 1; /* only for passive */
} BNEP_CONNECT_REQ_FLAGS;
#endif
typedef struct BNEP_CONNECT_REQ_T
{
    uint16 type;
#ifdef BTCHIP
    BNEP_CONNECT_REQ_FLAGS flags;
#else
    uint16 flags;
#endif
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
    enum BNEP_CONNECT_CFM_STATUS status;
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
    enum BNEP_CONNECT_IND_STATUS status;
    uint16 id;
    ETHER_ADDR rem_addr;
    uint16 rem_uuid;
    uint16 loc_uuid;
} BNEP_CONNECT_IND_T;










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
    enum BNEP_DISCONNECT_IND_REASON reason;
} BNEP_DISCONNECT_IND_T;





typedef struct BNEP_HCI_SWITCH_ROLE_RSP_T /* DM_HCI_SWITCH_ROLE_COMPLETE_T */
{
    uint16 type;
    phandle_t phandle; /* unused */
    BD_ADDR_T bd_addr;
    uint8 status;
    uint8 role;
} BNEP_HCI_SWITCH_ROLE_RSP_T;

typedef struct BNEP_ROLE_CHANGE_IND_T
{
    uint16 type;
    uint16 rem_uuid;
    uint16 loc_uuid;
} BNEP_ROLE_CHANGE_IND_T;

#define BNEP_BM_IND_MAX_NONI 6
typedef struct BNEP_BM_IND_NODE
{
    unsigned int is_bm : 1;
    unsigned int has_link_key : 1;
    ETHER_ADDR rem_addr;
    uint8 link_key[SIZE_LINK_KEY]; /* only present if has_link_key */
} BNEP_BM_IND_NODE;
typedef struct BNEP_BM_IND_T
{
    uint16 type;
    BNEP_BM_IND_NODE *node[BNEP_BM_IND_MAX_NONI]; /* 1st, if present, is primary BM; if not present it's a session end */
} BNEP_BM_IND_T;

typedef union BNEP_UPRIM_T
{
    uint16 type;
    /* HERE: put all the primitives in here */
} BNEP_UPRIM_T;


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
