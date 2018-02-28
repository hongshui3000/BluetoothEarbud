#!/usr/bin/env python2.6
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2

'''A script to put the correct values for build id and build date into mib_app.xml
'''
import sys

import optparse

# Process command line arguments

usage = "%prog [-d <date file>] [-i <id_file>] < <original xml file> > <derived xml file>"

parser = optparse.OptionParser(usage=usage)
parser.add_option("-d", action="store", type="string", dest="date_file", default="", help="the file containing the build date, typically <build_dir>/date.txt")
parser.add_option("-i", action="store", type="string", dest="id_file", default="", help="the file containing the build id, typically <build_dir>/buildid.int")
(options, args) = parser.parse_args()

date_file = options.date_file
id_file = options.id_file

# Method
# We read the input line by line, looking for a line like
# <metadata subsystem_name="app" subsystem_alias="system" subsystem_id="7" version="0">
# We then cat the id and date files into it giving something like
# <metadata subsystem_name="app" subsystem_alias="system" subsystem_id="7" version="1000000538" build_date_time="2014-11-05 14:58">
# Finally we copy the rest of the file

# First read the date and time file
try:
    date_handle = open(date_file)
except IOError:
    sys.stderr.write("%s: unable to open %s\n" % (argv[0], date_file))
    sys.exit(1)

date_string = date_handle.readline().strip()
date_handle.close()

# Now the id file
try:
    id_handle = open(id_file)
except IOError:
    sys.stderr.write("%s: unable to open %s\n" % (argv[0], id_file))
    sys.exit(1)

id_string = id_handle.readline().strip()
id_handle.close()

for line in sys.stdin:
    stripped_line = line.strip() # Throw away surrounding whitespace
    if 0 != len(stripped_line) and '<' == stripped_line[0]:
        # Is this the line we want
        line_words = stripped_line[1:].split()
        if 1 < len(line_words) and "metadata" == line_words[0] and "subsystem_name" in line_words[1]:
            # Yes, this is the line. Copy up to version then substitute
            i = 0
            while ' ' == line[i]:
                sys.stdout.write(' ')
                i += 1
            sys.stdout.write('<')
            l = len("version")
            for word in line_words:
                if len(word) < l or "version" != word[0:l]:
                    sys.stdout.write("%s " % word)
                else:
                    sys.stdout.write("version=\"%s\" build_date_time=\"%s\">\n" % (id_string, date_string))
                    break # We assume version is the last element
        else:
            # Just copy it
            sys.stdout.write(line)
    else:
        sys.stdout.write(line)
