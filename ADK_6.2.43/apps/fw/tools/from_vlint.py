# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# PROGRAM
#   from_vlint.py  -  report vlint value
#
# SYNOPSIS
#   from_vlint.py [-x] octet1 [octet2 ...]
#
# DESCRIPTION
#   The MIB internally makes heavy use of VLINTs - variable length
#   integers.  VLINT coding is defined in vlint.h.
#
#   This script prints to stdout the integer value of VLINT formed by
#   "octet1", "octet2", etc.  The "octet*" arguments must be in hexadecimal.
#   If the -x flag is present then the value is reported in hexadecimal.
#   
# ORIGIN
#   //depot/dot11/main/common/mib/from_vlint.py#2

import getopt, sys, string


def usage():
    print "usage: from_vlint.py [-x] octet1 [octet2 ...]"

def usage_exit():
    usage()
    sys.exit(1)

def moan(s):
    print "from_vlint: %s" % s

def croak(s):
    moan(s)
    sys.exit(1)

def vlint_from_octet_list(ol):
    if len(ol) == 1 and (ol[0] & 0x80) == 0x00:
        val = ol[0] & 0x7f
        if (val & 0x40) == 0x40:  # Negative
            val = val - 128
        return long(val)
    ollen = ol[0] & 0x1f
    if ollen == 0:
        croak("VLINT length error")
    if len(ol) != ollen + 1:
        croak("VLINT length error")
    if (ol[0] & 0x40) == 0x40:
        val = long(-1)
    else:
        val = long(0)
    ol = ol[1:]
    for o in ol:
        val = val << 8
        val = val | (o & 0xff)
    return val

def print_val(out, octets):
    print_octet_line(out, octets)

def vlint_to_int(octets, radix):
    val = vlint_from_octet_list(octets)
    if radix == 10:
        print "%ld" % val
    else:
        print "%lx" % val

# Start here: python calls this function when this python file is invoked
# directly (typically from a shell).
#
if __name__ == "__main__":
    radix = 10
    try:
        opts, args = getopt.getopt(sys.argv[1:], "x?", ["help"])
    except getopt.GetoptError:
        usage_exit()
    for o, a in opts:
        if o == "-x":
            radix = 16
        elif o in ("-?", "--help"):
            usage_exit()
    if len(args) == 0:
            usage_exit()
    ol = []
    for o in args:
        ol = ol + [int(o, 16)]
    vlint_to_int(ol, radix)
