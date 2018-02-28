# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# PROGRAM
#   to_vlint.py  -  report vlint coded octet sequence for an integer
#
# SYNOPSIS
#   to_vlint.py [-x] int
#
# DESCRIPTION
#   The MIB internally makes heavy use of VLINTs - variable length
#   integers.  VLINT coding is defined in vlint.h.
#
#   This script prints to stdout the VLINT coded octet sequence that
#   holds value "int".  If the -x flag is present then "int" is taken
#   to be in hexadecimal.
#   
# ORIGIN
#   //depot/dot11/main/common/mib/to_vlint.py#2

import sys, string


def usage():
    print "usage: to_vlint.py [-x] int"

def usage_exit():
    usage()
    sys.exit(1)

def moan(s):
    print "to_vlint: %s" % s

def croak(s):
    moan(s)
    sys.exit(1)

def print_octet_line(out, l):
    n = 0
    ll = len(l)
    first = 1
    for o in l:
        if not first:
            out.write(", ")
        if o < 0:
            tmp = "%02x" % o
            out.write("0x%s" % tmp[-2:])
        else:
            out.write("0x%02x" % o)
        first = 0
    out.write("\n")

# Return an array holding the bytes of a vlint holding val.
def vlint_octet_list(val):
    if val == 0:
        return [0x00]
    if val >= -64 and val <= 63:
        return [val & 0x7f]
    # Use vwi re-entrantly to snip off 8 bits of "val" at each call.
    o = []
    o = vwi(o, val);
    if val < 0:
        return [0xC0 | len(o)] + o
    else:
        return [0x80 | len(o)] + o

def vwi(o, val):
    if not (val >= -128 and val <= 255):
        o = vwi(o, val >> 8)
    return o + [val & 0xff]

def print_val(out, octets):
    print_octet_line(out, octets)

def int_to_vlint(val):
    print_val(sys.stdout, vlint_octet_list(val))

# Start here: python calls this function when this python file is invoked
# directly (typically from a shell).
#
if __name__ == '__main__':
    if not sys.argv[1:]:
        usage_exit()
    if sys.argv[1] == "-x":
        radix = 16
        if not sys.argv[2:]:
            usage_exit()
        num = sys.argv[2]
    else:
        radix = 10
        num = sys.argv[1]
    try:
        int = long(num, radix)
    except ValueError:
        usage_exit()
    int_to_vlint(int)
