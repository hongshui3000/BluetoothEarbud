# Copyright (c) 2017 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# Module to read and write xuv file format
#

from re import match, compile

# Prevent being run as a script which is pointless.
if __name__ == '__main__':
    print('ERROR: This is a python module, use import.')
    exit(1)

##
# @brief Read a XUV file.
# 
# @param[in] filename   The path and filename of the xuv file.
#
# @return               A dict containing address as key and values as value.
#
def read(filename):
    p = compile('\s*@(?P<addr>[0-9a-fA-F]+) +(?P<value>[0-9a-fA-F]+)\s*')
    values = {}
    with open(filename, 'r') as f:
        for line in f:
            m = p.match(line)
            if m:
                addr = int(m.group('addr'), 16)
                value = int(m.group('value'), 16)
                values[addr] = value
                #print(m.group('addr'), int(m.group('addr'), 16), m.group('value'))
            #print(line)
    return values

##
# @brief Write a XUV file.
#
# @param[in] filename   The path and filename of the xuv file.
# @param[in] values     Dict containing address as key and values as value.
#
def write(filename, values):
    with open(filename, 'w') as f:
        for addr in values:
            f.write('@%06X %04X\n' % (addr, values[addr]))

