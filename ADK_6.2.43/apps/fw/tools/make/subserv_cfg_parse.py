#!/usr/bin/env python
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2

# Script for parsing the subserv.cfg file as part of the build process.
# This generates a header file containing definitions including values
# that can be used in advertisements as well as a C file that contains
# the array of subserv handlers.
#
# Takes any number of -D, -U and -I options to be passed to the preprocessor.
# We also add -DSUBSERV_PREPROCESSING that allows the header to avoid
# including anything we haven't generated yet.
#
# See below for what the remaining arguments mean.

from subprocess import *
import sys
import re
import os
import errno
from optparse import OptionParser
import shutil

usage_string = ("Usage: %s cfg-file private.h "
                "output_private.h output_public.h output.c"
                 % sys.argv[0])
parser = OptionParser(usage=usage_string);
parser.add_option("-c", "--compiler", help="The command for running "
                    "the C preprocessor. ",
                    type="string", dest="cpp_command", default='gcc')
parser.add_option("-f", "--flags", help="Flags to pass to the compiler"
                    "to produce pre-processed output."
                    " They should be enclosed in quotes.",
                    type="string", dest="cpp_args")

options, args = parser.parse_args()

# Verbose output for debugging.
debug = False

class Service:
    # For recording each service record we come across.
    # We'll populate the record as we scan the file.
    def __init__(self, name):
        self.name = name
        self.header_start = -1
        self.handler_start = -1
        self.ies_start = -1
        self.ies_words = 0
        self.ies_end = -1

prog_name = sys.argv[0]

cpp_args = options.cpp_args.strip("'").split()

if len(args) != 5:
    raise RuntimeError, usage_string

# Input configuration file
cfg_file = args[0]
# Private include file for subsystem
private_h = args[1]
# Output private header file with advertisement information.
output_private_h = args[2]
# Output public header file containing handler declarations.
output_public_h = args[3]
# Output source file with subserv handler records.
output_c = args[4]

def make_sure_path_exists(path):
    try:
        os.makedirs(path)
    except OSError as exception:
        if exception.errno != errno.EEXIST:
            raise
        
# Create a temporary preprocessor output file in the dest directory.
# The dest directory should only have one build in it at once, so
# we don't need to take care about the temporary file name.
# We'll also not bother deleting it as it's lost in the noise and
# potentially useful for debugging.
output_directory = os.path.dirname(output_c)
tmpfile = os.path.join(output_directory, "subserv_cfg_parse_tmp.c")
cfg_file_copy_name = os.path.join(output_directory, "subserv_cfg_temp.c")
make_sure_path_exists(output_directory) 
shutil.copyfile(cfg_file, cfg_file_copy_name)
# Preprocess the configuration file into temporary file
ftmp = open(tmpfile, "w")
ret = call([ options.cpp_command ] + cpp_args + 
           ["-DSUBSERV_PREPROCESSING", cfg_file_copy_name, "-o", tmpfile], 
           stdout=ftmp)
if ret != 0:
    raise RuntimeError, "Bad status %d from C preprocessor" % (ret)
ftmp.close()

service_records = [ ]

# Keep track of lines in the configuration file itself.
# We'll ignore lines in any other file.
line_no = 1
if "gcc" == options.cpp_command:
    line_pat = re.compile(r'\#\s*(\d+)\s+"(\S+)"')
    begin_pat = re.compile(r'%BEGIN\s*(\S*)')
else:
    # Assume this is kcc
    line_pat = re.compile(r'\#line\s*(\d+)\s+"(\S+)"')
    begin_pat = re.compile(r'%[ ]*BEGIN\s*(\S*)')
# Match commas at the ends of lines
comma_end = re.compile(r',\s*$')
# Match comma alone on line, if not at end of previous line
comma_only = re.compile(r'\s*,\s*$')
# Record when we're passed the comma for a header or handler entry.
comma_pending = False
# Flag we're looking at an include file we don't need to parse
# for configuration data
not_this_file = False
# Array of handler names
handlers = {}
cur_record = None
infile = open(tmpfile)
# Scan the temporary file looking for service records.
# As this point we'll just record line numbers and count IE sizes,
# but not generate any output.  For that we'll rescan the config file
# using the information we've found here.
for line in infile:
    # Handle preprocessor line directives.
    # Hope these are reasonably standard.
    m = line_pat.match(line)
    if m:
        if m.group(2) == cfg_file_copy_name:
            not_this_file = False
            line_no = int(m.group(1))
            if debug:
                print "%s:%d: line number updated" % (cfg_file, line_no)
            # this refers to the next line, so don't increment
            # before we see that
            continue
        else:
            not_this_file = True
    # To avoid nasty additional complexities, we require all
    # data before replacement to come from the config file itself.
    if not_this_file:
        continue
    # See if we've found a new service record.
    m = begin_pat.match(line)
    if m:
        if cur_record is not None:
            # Check the existing record was sufficiently complete.
            if cur_record.handler_start == -1:
                raise RuntimeError, "%s:%d: missing header for %s" % (
                    cfg_file, line_no, cur_record.name)
            if cur_record.ies_start == -1:
                raise RuntimeError, "%s:%d: missing handler for %s" % (
                    cfg_file, line_no, cur_record.name)
            cur_record.next_start = line_no
        name = m.group(1)
        if len(name) == 0:
            name = "Unknown"
        cur_record = Service(name)
        service_records.append(cur_record)
        cur_record.header_start = line_no + 1
        if debug:
            print "%s:%d: found service %s" % (cfg_file, line_no, name)
        line_no += 1
        continue
    elif cur_record is None:
        # Still looking for the first record.
        line_no += 1
        continue
    if cur_record.handler_start == -1:
        # Still looking for the service record header.
        # If there's a comma at the end of the line it's
        # finished here, else we'll have to keep looking for
        # the comma before the subserv handler.
        comma_pending = not comma_end.search(line)
        if re.match(r'\s*$', line):
            line_no += 1
            continue
        if not re.match(r'[^,]*,[^,]*(,\s*)?$', line):
            raise RuntimeError , "%s:%d: no record header found for %s" % (
                cfg_file, line_no, cur_record.name)
        if debug:
            print "%s:%d found record header for service %s" % (
                cfg_file, line_no, cur_record.name)
        # Provisional guess for the line where the handler is.
        # We'll skip blank lines in the CPP output to ensure
        # we don't get the end of the service record header macro.
        # We're going to make the naive assumption that the
        # handler name is a single word on a line.
        cur_record.handler_start = line_no + 1
    elif cur_record.ies_start == -1:
        # Looking for the handler.
        if re.match(r'\s*$', line):
            line_no += 1
            # Ensure we don't get bits of the SRH with the handler
            cur_record.handler_start = line_no
            continue
        if comma_pending:
            # If we found the service record but there was no
            # comma on the line (which can happen because of
            # macro replacement) we need that comma, and only that
            # comma, before the line with the subserv handler.
            if comma_only.match(line):
                line_no += 1
                cur_record.handler_start = line_no
                comma_pending = False
                continue
            else:
                raise RuntimeError, \
                    "%s:%d: missing comma after header for %s: got\n%s" % (
                    cfg_file, line_no, cur_record.name, line)
        # Now look for the subserv handler.  We don't expect this to
        # have macros, so it should be followed immediately by a comma.
        # Actually, we never need this comma for syntactic reasons,
        # but we'll check for consistency with all the other lines.
        m = re.match(r'\s*([a-zA-Z_][a-zA-Z0-9_]+)\s*,\s*$', line)
        if not m:
            raise RuntimeError, \
                "%s:%d: expecting subserv handler with trailing comma, got:\n%s" % (
                cfg_file, line_no, line)
        if debug:
            print "%s:%d: found handler %s for service %s" % (
                cfg_file, line_no, m.group(1), cur_record.name)
        if handlers.has_key(m.group(1)):
            handlers[m.group(1)].append(cur_record.name)
        else:
            handlers[m.group(1)] = [ cur_record.name ]
        cur_record.ies_start = line_no + 1
    else:
        # Looking for IEs.
        if re.match(r'\s*$', line):
            cur_record.ies_end = line_no
            line_no += 1
            continue
        array = re.split(',', line)
        larray = len(array)
        if larray > 1:
            # comma is terminator rather than separator
            larray -= 1
            if debug:
                print "%s:%d: found %d word(s) in IEs" % (
                    cfg_file, line_no, larray)
            cur_record.ies_words += larray
        # Provisional end of IEs.  We'll update this as we find more text
        # before the next %BEGIN.  However, we won't update it on empty
        # lines --- that's so that when copy the original configuration file
        # we don't pull in unmatched "#endif"s etc. that occur between the
        # IEs and the next %BEGIN or end of file.
        cur_record.ies_end = line_no + 1
    line_no += 1
infile.close()

if cur_record is not None:
    # Confirm that the final record was sufficiently complete.
    if cur_record.handler_start == -1:
        raise RuntimeError, "%s:%d: missing header for %s" % (
            cfg_file, line_no, cur_record.name)
    if cur_record.ies_start == -1:
        raise RuntimeError, "%s:%d: missing handler for %s" % (
            cfg_file, line_no, cur_record.name)

# Count sizes.
ies_words = 0
n_services = len(service_records)
for rec in service_records:
    ies_words += rec.ies_words


# Output the header file.
infile = open(cfg_file)
output_h_guard = re.sub(r'\.', '_',
                        re.sub(r'^.*/', '', output_private_h)).upper()

fh = open(output_private_h, "w")
fh.write("""/**
 * \\file
 * Private header automatically generated by %s.
 * This contains the array of advertisements exported by the subsystem
 * together with its length, in an appropriate form for use in the
 * subsystem SLT, and also the number of entries in the subserv
 * array.
 */

#ifndef %s
#define %s
""" % (prog_name, output_h_guard, output_h_guard))
        # Output number of service, length of advertisement array in words and
        # header for array itself
fh.write("""/** Number of services handled by subserv */
#define SUBSERV_SERVICES_COUNT (%d)

/**
 * The array supplied by the subsystem containing function information.
 */
extern const SUBSERV_PROVIDER_INFO
subserv_provider_info[SUBSERV_SERVICES_COUNT];

/**
 * Size of a service advert excluding IEs.
 * The record header plus the word with the count of IEs.
 */
#define SUBSERV_BASIC_ADVERT_SIZE \\
    (CCP_SERVICE_RECORD_HEADER_WORD_SIZE + 1U)

/** Number of words in the advertisement array */
#define SUBSERV_ADVERTISEMENT_WORDS \\
    (SUBSERV_SERVICES_COUNT * SUBSERV_BASIC_ADVERT_SIZE + %d /* IEs */)

/** The advertisement array itself */
#define SUBSERV_ADVERTISEMENT_ARRAY \\
""" % (n_services, ies_words))

# Construct advertisement array.
# No preprocessor garbage this time, so the line number
# management is thankfully trivial
line = infile.readline()
line_no = 1
ies_str = ""
for cur_record in service_records:
    ies_str += "    /* BEGIN %s */ \\\n" % (cur_record.name)
    # Skip to the record header for this service.
    while line_no < cur_record.header_start:
        line = infile.readline()
        line_no += 1
    # Copy all lines of the record header.
    # Note this is all part of a macro, hence the trailing backslashes.
    while True:
        line = re.sub(r'\n$', '', line)
        ies_str += "%s \\\n" % (line)
        line = infile.readline()
        line_no += 1
        if line_no == cur_record.handler_start:
            break
    ies_str += "    %d /* following IE words */, \\\n" % (
        cur_record.ies_words)
    if cur_record.ies_words > 0:
        while True:
            # Skip the handler, not used in advertisements.
            line = infile.readline()
            line_no += 1
            if line_no == cur_record.ies_start:
                break
        while True:
            # Keep going at least until cur_record.ies_end,
            # and then until we get to a new BEGIN, a CPP
            # directive, or end of file.
            if not re.match(r'\s*$', line):
                line = re.sub(r'\n$', '', line)
                ies_str += "%s \\\n" % (line)
            line = infile.readline()
            line_no += 1
            if len(line) == 0:
                break
            if line_no >= cur_record.ies_end and (
                begin_pat.match(line) or
                re.match(r'^\#', line)):
                break

if len(ies_str):
    ies_str = re.sub(r'\s*,\s*\\\n$', '', ies_str)
else:
    ies_str = "/* Empty */"

# Trailer
fh.write("""%s
#endif /* %s */
""" % (ies_str, output_h_guard))

fh.close()


# Output the public header file with the handler definitions.
fh = open(output_public_h, "w")
output_h_guard = re.sub(r'\.', '_',
                        re.sub(r'^.*/', '', output_public_h)).upper()
fh.write("""/**
 * \\file
 * Public header automatically generated by %s.
 * This contains the declarations of handlers.  It is included automatically
 * by subserv.h.
 */

#ifndef %s
#define %s
""" % (prog_name, output_h_guard, output_h_guard))

# Handler prototypes.
handler_keys = handlers.keys()
handler_keys.sort()
for hand_name in handler_keys:
    services = handlers[hand_name]
    fh.write("""
/** Handler for service(s) %s */
extern SUBSERV_RETURN %s(SUBSERV_INSTANCE_DATA *idata);
/** Indicate that prototype is present */
#define HAS_%s_PROTOTYPE
""" % (", ".join(services), hand_name, hand_name.upper()))

# Trailer
fh.write("""#endif /* %s */
""" % (output_h_guard))

fh.close()


# Output the source file with the subserv handler.

fc = open(output_c, "w")
infile.seek(0)
fc.write("""/**
 * \\file
 * File automatically generated by %s.
 * This contains the provider handler information required by subserv.
 */
#include "%s"

/**
 * The array of information for all services provided by this subsystem.
 */
const SUBSERV_PROVIDER_INFO subserv_provider_info[SUBSERV_SERVICES_COUNT] =
{
""" % (prog_name, private_h));
line = infile.readline()
line_no = 1
record_str = ""
for cur_record in service_records:
    if len(record_str):
        record_str += ",\n"
    record_str += "    /* BEGIN %s */\n" % (cur_record.name)
    # Skip to the record header for this service.
    while line_no < cur_record.header_start:
        line = infile.readline()
        line_no += 1
    record_str += """    {
        { {
"""
    # Copy all lines of the record header.
    while True:
        record_str += line
        line = infile.readline()
        line_no += 1
        if line_no == cur_record.handler_start:
            break
    record_str = re.sub(r'\s*,\s*\n$', "\n", record_str)
    record_str += """        } },
"""
    # Add handler line, already read
    record_str += re.sub(r'\s*,\s*\n$', "\n", line)
    record_str += """    }"""
fc.write("""%s
};
""" % record_str)
fc.close()

infile.close()
