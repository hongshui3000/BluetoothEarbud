/******************************************************************************

    file ipv4.h  --  Internet Protocol, Version 4

Copyright (c) 2002 - 2016 Qualcomm Technologies International, Ltd.
  Part of 6.2
*/


#ifndef IPV4_H
#define IPV4_H


#include "app/pan/pantypes.h"


/******************************************************************************

    typedef IPV4_ADDR  --  an IPv4 address

******************************************************************************/

typedef uint32 IPV4_ADDR;


/******************************************************************************

    #defines and macros for IPv4 addressing

Note that reserved addresses include the unspecified address,
and class E addresses include the broadcast address.

******************************************************************************/

#define IPV4_ADDR_LEN 4 /* in octets */
#define IPV4_ADDR_ANY 0 /* 0.0.0.0 */
#define IPV4_ADDR_ALL_SYSTEMS 0xe0000001 /* 224.0.0.1 */
#define IPV4_ADDR_ALL_ROUTERS 0xe0000002 /* 224.0.0.2 */
#define IPV4_ADDR_MDNS 0xe00000fb /* 224.0.0.251 */
#define IPV4_ADDR_BROADCAST 0xffffffff /* 255.255.255.255 */
#define IPV4_PREFIX_CLASSD 0xe /* 224.0.0.0/4 */
#define IPV4_PREFIX_CLASSDANDE 7 /* 224.0.0.0/3 */
#define IPV4_PREFIX_LOOPBACK 127 /* 127.0.0.0/8 */
#define IPV4_PREFIX_LINK_LOCAL 0xa9fe /* 169.254.0.0/16 */
#define IPV4_IS_CLASSD(x) ((unsigned) ((x) >> 28) == IPV4_PREFIX_CLASSD)
#define IPV4_IS_CLASSDORE(x) ((unsigned) ((x) >> 29) == IPV4_PREFIX_CLASSDANDE)
#define IPV4_IS_UNSPECIFIED(x) ((x) == IPV4_ADDR_ANY)
#define IPV4_IS_LOOPBACK(x) ((unsigned) ((x) >> 24) == IPV4_PREFIX_LOOPBACK)
#define IPV4_IS_SAME_ADDR(x,y) ((x) == (y))
#define IPV4_IS_BROADCAST(x) ((x) == IPV4_ADDR_BROADCAST)
#define IPV4_IS_LINK_LOCAL(x) ((unsigned) ((x) >> 16) == IPV4_PREFIX_LINK_LOCAL)


#endif /* IPV4_H */
