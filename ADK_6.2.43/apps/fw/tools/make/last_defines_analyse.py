# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
"""For usage see last_defines_analyse.py --help"""

import optparse
import os
import re
import sys
import shutil

# Some utility functions to implement grep -w `find ...`

def grep_words(words, file_name):
    """A function to see if any string in a list occurs as a word in a file"""
    try:
        handle = open(file_name, "r")
    except IOError:
        return False
    for line in handle:
        for word in words:
            if re.search("\\b" + word + "\\b", line.strip()):
                handle.close()
                return True
    handle.close()
    return False

def grep_words_in_files(words, file_names):
    """A function to see if any string in a list occurs as a word in a list of files
    Returns a new list of those containing at least one of the given words"""
    return [file_name for file_name in file_names if grep_words(words, file_name)]

def matching_file(suffixes, file_name):
    """See if a filename has one of a list of suffixes"""
    (root, ext) = os.path.splitext(file_name)
    if len(ext) > 1:
        return ext in suffixes
    else:
        return False

def matching_files(suffixes, file_names):
    """Filter a list of filenames down to those with one of the given suffixes"""
    return [file_name for file_name in file_names if matching_file(suffixes, file_name)]

def files_only(paths):
    """Reduce a list of paths to only the files"""
    return [file_name for file_name in paths if os.path.isfile(file_name)]

def matching_files_from_list(file_names, suffices, changes):
    """Find all the files from a set with one of a list of suffices
    containing one of the changed definitions"""
    suff_files = matching_files(suffices, file_names)
    return grep_words_in_files(changes, suff_files)

# Find all the files on a path with one of a list of suffices
# containing one of the changed definitions
def matching_files_on_path(dir, suffices, changes):
    """Find all the files on a path with one of a list of suffices
    containing one of the changed definitions"""
    all_files = files_only([os.path.join(dir, file_name) for file_name in os.listdir(dir)])
    return matching_files_from_list(all_files, suffices, changes)

def extend_dict(dictionary, line):
    """A function to process a single definition and put it in a dictionary"""
    parts = line.strip().split('=')
    length = len(parts)
    if 1 != length and 2 != length:
        sys.stderr.write("%s: invalid define %s\n" % (sys.argv[0], line))
        sys.exit(1)
    if 1 == length:
        # A define with default value
        val = None
    else:
        val = parts[1]
    dictionary[parts[0]] = val

def dict_from_file(file_name):
    """A function to take a last_defines.h file and return a dictionary of settings"""
    new_dict = {}
    try:
        handle = open(file_name, "r")
    except IOError:
        sys.stderr.write("%s: unable to open %s\n" % (sys.argv[0], file_name))
        sys.exit(1)
    for line in handle:
        # Throw away the #define bit
        parts = line.split()
        extend_dict(new_dict, "=".join(parts[1:]))
    handle.close()
    return new_dict

def new_defines(name, defines_dict, mtime = None):
    """A function to produce the new last_defines.h"""
    try:
        out_file = open(name, "w")
    except IOError:
        sys.stderr.write("%s: unable to open %s\n" % (sys.argv[0], name))
    for (key, val) in defines_dict.iteritems():
        if None == val:
            out_file.write("#define %s\n" % key)
        else:
            out_file.write("#define %s %s\n" % (key, val))
    out_file.close()
    os.utime(name, mtime)

def depends_on(dep_file, files_changed):
    """A function to see if a given object file depends on a source
    Works by analysing the .d file
    Also returns (as a list) the items depending on that source"""
    try:
        handle = open(dep_file, "r")
    except IOError:
        sys.stderr.write("%s: unable to open %s\n" % (sys.argv[0], dep_file))
        sys.exit(1)
    targets = []
    required = False
    for line in handle:
        # Get the part after the : if any
        stripped_line = line.strip()
        if "" == stripped_line:
            continue # Ignore blank lines
        if ":" in stripped_line:
            parts = stripped_line.split(":")
            # We keep the part before the :
            # as it may be a dependent
            # Of course, it may also be a DOS drive letter,
            # so we reject those early
            target = parts[0].strip()
            if 1 == len(target):
                # A DOS drive letter, eg C:, so ignore the line
                # Note that we have no genuine targets of length 1
                # So this can't produce a false positive
                continue
            # There could be multiple targets on the line, so split and extend
            targets.extend(target.split())
            # We don't need to worry about other stuff such as continuation
            # as it won't match a pathname
            # But we need to fix potential mixed separation on win32
            dependents = os.path.normpath(parts[-1])
        else:
            dependents = os.path.normpath(stripped_line)
        # Now need to look at the changed files and see if any is
        # a trailing substring of dependent
        # We use substring because dependents may have more path, eg a ./
        for change in files_changed:
            if change in dependents:
                required = True
    handle.close()
    return (targets, required)

# Process command line arguments

usage = """Analyse the header specifying last set of defines and the new definition set
to see if anything needs rebuilding
Touch relevant sources if so
Command line parameters
Options: see below"""
parser = optparse.OptionParser(usage=usage)
parser.add_option("-v", action="store_true", dest="verbose", default=False, help="be more verbose, mostly for debugging the script")
parser.add_option("-r", action="store", type="string", dest="root", default="", help="the stem of the name of build defines information (including directory) (mandatory)")
parser.add_option("-d", action="store", type="string", dest="dirs", default="", help="specify dirs to be seached (space separated)")
parser.add_option("-m", action="store", type="string", dest="modules", default="", help="specify enabled modules (space separated)")
parser.add_option("-a", action="store", type="string", dest="defines", default="", help="specify additional defines (space separated)")
(options, args) = parser.parse_args()

# Check that we have a root value
if "" == options.root:
    sys.stderr.write("%s: no -r <root> option specified\n" % sys.argv[0])

config_filename = options.root

# Construct defines from the arguments
defines = [m.upper() + "_MODULE_PRESENT" for m in options.modules.split()]
defines += options.defines.split()

# Now read the definitions into new_defs
new_defs = {}
for define in defines:
    extend_dict(new_defs, define)

log_file_name = config_filename + ".log"
h_file_name = config_filename + ".h"
h_file_name_stamp = h_file_name + ".stamp"
d_file_name = config_filename + ".d"
config_file_dir = os.path.dirname(config_filename)
if options.verbose:
    sys.stderr.write("Opening log file %s\n" % log_file_name)
try:
    log_file = open(log_file_name, "w")
except IOError:
    sys.stderr.write("%s: unable to open %s\n" % (sys.argv[0], log_file_name))
    sys.exit(1)

if options.verbose:
    sys.stderr.write("Opening dependence file %s\n" % d_file_name)
try:
    d_file = open(d_file_name, "w")
except IOError:
    sys.stderr.write("%s: unable to open %s\n" % (sys.argv[0], d_file_name))
    sys.exit(1)

if options.verbose:
    sys.stderr.write("Checking for %s\n" % h_file_name)

# Now see if we need to compare for differences
if os.path.isfile(h_file_name):
    # Read this and see if it differs
    h_file = open(h_file_name, "r")
    # Get the last mod time so we can preserve it
    mtime = os.stat(h_file_name).st_mtime
    changes_list = []
    if options.verbose:
        sys.stderr.write("Constructing dictionaries\n")
    prev_defs = dict_from_file(h_file_name)
    if options.verbose:
        sys.stderr.write("Comparing dictionaries\n")
    for key in prev_defs:
        if key not in new_defs or prev_defs[key] != new_defs[key]:
            changes_list.append(key)
    for key in new_defs:
        if key not in prev_defs:
            changes_list.append(key)

    if options.verbose:
        sys.stderr.write("%d changes\n" % len(changes_list))
        for change in changes_list:
            sys.stderr.write("'%s'\n" % change)
    if 0 != len(changes_list):
        if options.verbose:
            sys.stderr.write("processing differences\n")
        log_file.write("*** Definitions changed: ***\n")
        # Check for this definition in all c, h, asm, xml files
        # and (eventually) create a .d file recording what needs rebuilding
        # It would be nice to be able to abstract the paths we care
        # about, and whether we care recursively or not
        # For example, we need ../../common/interface/*.xml
        # ../../common/interface/gen/xap/*.h (yes, xap)
        # and some source directories
        # The source directories apart from common are specified using -d
        # Note that these numpty interfaces don't return full paths,
        # so in order to use the file name you have to prepend the path
        # Yet more arcane python list comprehension syntax required
        files_changed = []
        matches = matching_files_on_path("../../common/interface", [".xml"], changes_list)
        files_changed.extend(matches)
        matches = matching_files_on_path("../../common/interface/gen/xap", [".h"], changes_list)
        files_changed.extend(matches)
        # Now walk the recursive source directories and friends
        # Note, if this is erroneously passed in empty
        # the code will still work, so we don't test for it
        for walk_dir in options.dirs.strip().split():
            for (path, dirs, file_names) in os.walk(walk_dir):
                matches = matching_files_from_list([os.path.join(path, file_name) for file_name in file_names], [".c", ".h", ".asm"], changes_list)
                files_changed.extend(matches)

        # We now have all the files affected
        # We create a new .d file saying what need rebuilding
        # We find all the .d files containing any of the out of date files
        # And mark their corresponding objects as depending on build_defs.h.stamp
        if 0 != len(files_changed):
            log_file.write("Updating the following files:\n")
            for f in files_changed:
                log_file.write("%s\n" % f)
            dep_files = []
            for (path, dirs, file_names) in os.walk(config_file_dir):
                matches = matching_files([".d"], [os.path.join(path, file_name) for file_name in file_names])
                dep_files.extend(matches)
            # Now we have the list of .d files
            # search for the changed files
            for dep_file in dep_files:
                # Search for each of files_changed in d_file
                # Note the normalisation of dep_file, in order to
                # deal with windows mixed separators
                # We also get the targets back from depends_on
                # This allows us to get all things we need (not just .o)
                # eg .lob, .lint.out, .e, .asm etc
                (targets, required) = depends_on(os.path.normpath(dep_file), files_changed)
                if required:
                    # Set up the stamp file
                    # We need to create it
                    with open(h_file_name_stamp, "w"):
                        os.utime(h_file_name_stamp, None)
                    for target in targets:
                        d_file.write("%s: %s\n" % (target, h_file_name_stamp))
        # Output the new definitions if they've changed
        # But set the time stamp to as before to prevent wholesale rebuild
        # Note: we have already determined what should be rebuilt
        # and added the .d file to force this
        new_defines(h_file_name, new_defs, (mtime, mtime))
else:
    # If config_filename.h doesn't exist then there's little to do
    if options.verbose:
        sys.stderr.write("%s not found\n" % h_file_name)
    new_defines(h_file_name, new_defs)

d_file.close()
log_file.close()
# Now send the log file to stderr
log_file = open(log_file_name, "r")
for line in log_file:
    sys.stderr.write(line)
log_file.close()
