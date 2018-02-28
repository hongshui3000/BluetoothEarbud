/******************************************************************************

    file ip.h  --  generic Internet Protocol support

Copyright (c) 2002 - 2016 Qualcomm Technologies International, Ltd.
  Part of 6.2
*/


#ifndef IP_H
#define IP_H


#include "ipv6.h"


/******************************************************************************

    typedef IP_ADDR  --  an IP address

******************************************************************************/

typedef IPV6_ADDR IP_ADDR;


/******************************************************************************

    #defines and macros for IP addressing

HERE: We're not dealing with directed (IPv4) broadcasts here.

******************************************************************************/

#define IP_IS_MULTICAST(x) (IPV6_IS_MULTICAST (x) || \
                            IPV6_IS_IPV4MAPPED_CLASSDORE (x))
#define IP_IS_UNSPECIFIED(x) (( \
        /* :: or 0.0.0.0 */    (x)->w[0] | \
                               (x)->w[1] | \
                               (x)->w[2] | \
                               (x)->w[3] | \
                               (x)->w[4] | \
                               (x)->w[6] | \
                               (x)->w[7] \
                             ) == 0 && \
                             ((x)->w[5] == 0 || \
                              (x)->w[5] == 0xffff))
#define IP_IS_SAME_ADDR IPV6_IS_SAME_ADDR
#define IP_IS_IPV4MAPPED IPV6_IS_IPV4MAPPED


/******************************************************************************

    macro IP_TO_IPV4  --  convert from IP_ADDR to IPV4_ADDR

The IP_ADDR is assumed to be IPv4-mapped.

******************************************************************************/

#define IP_TO_IPV4(x) ((IPV4_ADDR) (x)->w[6] << 16 | (x)->w[7])


/******************************************************************************

    variable IP_ADDR_ANY  --  the wildcard address

******************************************************************************/

extern const IP_ADDR IP_ADDR_ANY;


#endif /* IP_H */
