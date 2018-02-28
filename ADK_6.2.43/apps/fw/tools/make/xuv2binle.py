# Copyright (c) 2017 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# Convert XUV file into binary (uint16 little endian)
#

import sys 
import xuv

def xuv2binle(xuv_filename, bin_filename = None):
    """
    Reads an xuv file and generates a binary file.
    """
    if bin_filename is None:
        bin_filename = xuv_filename.replace(".xuv", ".bin")
    
    values = xuv.read(xuv_filename)
    with open(bin_filename, 'wb') as f:
        for i in range(min(values), max(values)+1):
            t = values.get(i, 0xFFFF)
            f.write(chr(t & 0xFF))
            f.write(chr(t >> 8))  

if __name__ == "__main__":
    """
    After parsing the arguments it kicks off the binary generator.
    """
    try:
        xuv_filename = sys.argv[1]
    except IndexError:
        print "Supply directory containing xuv as a command line parameter"
        exit(1)

    if len(sys.argv) >=3:
        bin_filename = sys.argv[2]
    else:
        bin_filename = None
        
    xuv2binle(xuv_filename, bin_filename)

