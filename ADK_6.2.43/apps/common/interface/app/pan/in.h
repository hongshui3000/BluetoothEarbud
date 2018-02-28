/******************************************************************************

    file in.h  --  generic Internet support

Copyright (c) 2002 - 2016 Qualcomm Technologies International, Ltd.
  Part of 6.2
*/


#ifndef IN_H
#define IN_H


/******************************************************************************

    #defines for Internet well-known port codes

See http://www.iana.org/assignments/port-numbers .

******************************************************************************/

#define IN_PORT_ANY 0 /* Reserved */
#define IN_PORT_ECHO 7 /* RFC862 */
#define IN_PORT_DISCARD 9 /* RFC863 */
#define IN_PORT_DAYTIME 13 /* RFC867 */
#define IN_PORT_QUOTE 17 /* RFC865 */
#define IN_PORT_CHARGEN 19 /* RFC864 */
#define IN_PORT_TELNET 23 /* RFC854 */
#define IN_PORT_SMTP 25 /* RFC821 */
#define IN_PORT_DOMAIN 53 /* RFC1034, RFC1035 */
#define IN_PORT_BOOTPS 67 /* RFC951 */
#define IN_PORT_BOOTPC 68 /* RFC951 */
#define IN_PORT_TFTP 69 /* RFC1350 */
#define IN_PORT_HTTP 80 /* RFC2616 */
#define IN_PORT_POP3 110 /* RFC1939 */
#define IN_PORT_NNTP 119 /* RFC977 */
#define IN_PORT_ZERO 1023 /* Reserved */
#define IN_PORT_MDNS 5353 /* draft-ietf-dnsext-mdns-10 */


#endif /* IN_H */
