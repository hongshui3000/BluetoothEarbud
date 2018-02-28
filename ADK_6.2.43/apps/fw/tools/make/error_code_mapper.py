#!/usr/bin/env python
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2

# When pointed at the panic and fault ID headers, order unimportant,
# outputs tables to be included in the host code to map the numbers
# to human readable strings.
#
# This script is much shorter in Perl.

import sys
import re

faults = {}
panics = {}

# Beware of regular expressions the 're' library doesn't support, e.g. [[:xdigit:]]...
line_match = r'^\s*\b(%s_\S+)\s*=\s*(0x[0-9a-fA-F]+|[0-9]+)'
panic_pat = re.compile(line_match % ('PANIC'))
fault_pat = re.compile(line_match % ('FAULT'))

for filename in sys.argv:
    f = open(filename, "r")
    for line in f:
        m = panic_pat.match(line)
        if m:
            name = m.group(1)
            if name == "PANIC_NONE":
                continue
            num = eval(m.group(2))
            if panics.has_key(num) and name != panics[num]:
                raise RuntimeError, "ERROR: panic 0x%.4x is defined twice:\n%s and %s\n" %(
                    num, panics[num], name)
            panics[num] = name
        m = fault_pat.match(line)
        if m:
            name = m.group(1)
            if name == "FAULT_NONE":
                continue
            num = eval(m.group(2))
            if faults.has_key(num) and name != faults[num]:
                raise RuntimeError, "ERROR: fault 0x%.4x is defined twice:\n%s and %s\n" %(
                    num, faults[num], name)
            faults[num] = name
    f.close()

print """/**
 * \\file
 *
 * Panic and fault codes mapped to human readable strings.
 * Should be included in a source file with appropriate definitions:
 * this is more maintainable than trying to include all the headers here.
 * Note, however, this defines constants so should only be included at
 * a single place.
 */

/** Panic codes and strings */
const PANICMAP_T panic_id_strings[] =
{"""

keys = panics.keys()
keys.sort()
for key in keys:
    name = panics[key]
    print "    { ERROR_ID_NAME_MAP(%s) }," % (name)

print """    { PANIC_NONE, (char *)NULL }
};

/** Fault codes and strings */
const FAULTMAP_T fault_id_strings[] =
{"""

keys = faults.keys()
keys.sort()
for key in keys:
    name = faults[key]
    print "    { ERROR_ID_NAME_MAP(%s) }," % (name)
print """    { FAULT_NONE, (char *)NULL }
};"""
