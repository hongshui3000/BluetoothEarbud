# Copyright (c) 2017 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# Convert binary file into xuv (uint16 little endian)
#

import sys

def bin2xuvle(bin_filename, xuv_filename = None):
    """
    Reads a binary file and generates an xuv file.
    """
    if xuv_filename is None:
        xuv_filename = bin_filename.replace(".bin", ".xuv")

    addr = 0
    with open(bin_filename, "rb") as f, open(xuv_filename, 'wb') as g:
        b1 = f.read(1)
        b2 = f.read(1)
        if len(b2) < 1:
            b2 = chr(0xFF)
        while b1 and b2:
            g.write("@%06X   %02X%02X\n" % (addr, ord(b2), ord(b1)))
            b1 = f.read(1)
            b2 = f.read(1)
            if len(b2) < 1:
                b2 = chr(0xFF)
            addr += 1

    print "Created %s" % xuv_filename

if __name__ == "__main__":
    """
    After parsing the arguments it kicks off the xuv generator.
    """
    try:
        bin_filename = sys.argv[1]
    except IndexError:
        print "Supply directory containing bin as a command line parameter"
        exit(1)

    if len(sys.argv) >=3:
        xuv_filename = sys.argv[2]
    else:
        xuv_filename = None
    
    bin2xuvle(bin_filename, xuv_filename)