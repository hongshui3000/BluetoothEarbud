#!/usr/bin/env python
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2

# This takes Intel hex code output by the linker and turns it into
# an array of words to be downloaded to the keep-alive RAM on the
# Curator.  The two arguments are the start and the end (actually
# the start of the next section) of keep-alive RAM as seen within
# Mildred (the same locations contained in the Intel hex).
#
# The format of Intel hex is described at
#  http://en.wikipedia.org/wiki/Intel_hex
# We only handle data and end-of-file record types.

import sys, re

if len(sys.argv) < 3 or (len(sys.argv) & 1) == 0:
    raise RuntimeError, """Usage: %s [code_start code_end] [...] <intel_hex_file
code_start and code_end are the Mildred locations.""" % (
        sys.argv[0])
progname = sys.argv[0]
sys.argv = sys.argv[1:]

# Because the allowed Janitor memory locations are now fragmented,
# we may have multiple code ranges on the command line.
code_ranges = [ ]

while len(sys.argv) != 0:
    code_ranges.append([ eval(sys.argv[0]), eval(sys.argv[1]) ])
    sys.argv = sys.argv[2:]

# Accumulate data in start+length+data chunks
class Data:
    data_chunk = []
    data_chunk_start = None

    all_output = ""

    def output_data_chunk():
        data_len = len(Data.data_chunk)
        if len(Data.all_output) > 0:
            Data.all_output += ",\n"
        # TBD: should probably count words, not octets?  But
        # Janitor start address is in octets, too.
        Data.all_output += "0x%04x, 0x%04x" % (Data.data_chunk_start, data_len)
        addr = 0
        output = ""
        while addr < data_len:
            if len(output) > 0:
                output += ", "
            if addr == data_len - 1:
                output += "0x00%02x" % (Data.data_chunk[addr])
            else:
                output += "0x%02x%02x" % (
                    Data.data_chunk[addr+1], Data.data_chunk[addr])
            if (addr % 16) == 14:
                if len(Data.all_output) > 0:
                    Data.all_output += ",\n"
                Data.all_output += output
                output = ""
            addr += 2
        if len(output) > 0:
            if len(Data.all_output) > 0:
                Data.all_output += ",\n"
            Data.all_output += output
    output_data_chunk = staticmethod(output_data_chunk)

for line in sys.stdin:
    m = re.match(':([0-9a-fA-F]*)([0-9a-fA-F][0-9a-fA-F])', line)
    if m is None:
        raise RuntimeError, "Can't read Intel hex line: %s\n" % (line)
    octet_string = m.group(1)
    checksum = eval("0x" + m.group(2))
    cumulative = 0
    test_string = octet_string
    while len(test_string) > 0:
        if len(test_string) == 1:
            raise RuntimeError, "Bad length for code in line: %s\n" % (line)
        cumulative += eval("0x" + test_string[0:2])
        test_string = test_string[2:]
    if (cumulative & 0xFF) != (0x100 - checksum) & 0xff:
        raise RuntimeError, """Bad checksum for line: %s
Got 0x%.2x, expected 0x%.2x
""" % (line, cumulative & 0xff, (0x100 - checksum) & 0xff)
    m = re.match('(..)(....)(..)(.*)', octet_string)
    data_len = eval('0x' + m.group(1))
    start_offset = eval('0x' + m.group(2))
    end_offset = start_offset + data_len
    code = eval('0x' + m.group(3))
    if code == 1:
        break
    elif code != 0:
        raise RuntimeError, "Unhandled type code for line: %s\n" %(line)

    range_ok = False
    for r in code_ranges:
        if (start_offset >= r[0] and start_offset < r[1] and
            end_offset >= r[0] and end_offset <= r[1]):
            range_ok = True
            break
    if not range_ok:
        raise IndexError, "Bad code segment: [0x%x, 0x%x]\n" % (
            start_offset, end_offset)
    data_str = m.group(4)
    new_data = []
    add_len = data_len
    while add_len > 0:
        new_data += [ eval('0x' + data_str[0:2]) ]
        data_str = data_str[2:]
        add_len -= 1
    if (len(Data.data_chunk) == 0 or
        start_offset != Data.data_chunk_start + len(Data.data_chunk)):
        if len(Data.data_chunk) != 0:
            Data.output_data_chunk()
        if (start_offset & 1) != 0:
            # As we're writing pairs of octets we'll need
            # to write zero to the octet below.
            start_offset -= 1
            Data.data_chunk = [ 0 ]
        else:
            Data.data_chunk = [ ]
        Data.data_chunk_start = start_offset
        Data.data_chunk += new_data
    else:
        Data.data_chunk += new_data

if len(Data.data_chunk) != 0:
    Data.output_data_chunk()

print Data.all_output
