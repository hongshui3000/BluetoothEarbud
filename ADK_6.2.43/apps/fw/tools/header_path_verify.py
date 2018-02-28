#!/usr/bin/env python2
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2

"""This script looks for various problems with header files and #includes.

Include directories are any directories begining with "core/include" plus any
hardware-specific subdirectories of hal and io.

Headers that live in an include directory may have common names otherwise they
must be unique.

#includes that reference a file that's not in an include directory must specify
a path.

Example:
    $ cd fw/src && python ../tools/header_path_verify.py
"""

import glob
import os
import re
import sys

"""Files that are allowed to have duplicate names and are allowed to be
included without a path.
"""
INCLUDE_DIR_EXCEPTIONS = (
    "./core/include*/*",
    "./core/hal/*/*/hal/*",
    "./core/io/*/*/io/*",
    "./core/io/*/*/pio_ctrl_sys_io/*",
)

"""Files exempted from the general "unique file names" rule.
There should be a very good reason to use this!!!
"""
DUPLICATE_FILE_EXCEPTIONS = [
    "./customer/core/main/main.h",
]

def _walk(*exts):
    """Walk over each file with an extension in exts."""

    for root, dirs, files in os.walk("."):
        posix_root = root.replace('\\', '/')

        # 'wlan' is from Synergy who have different conventions, so ignore it
        if posix_root in ("./wlan/ip", "./wlan/include"):
            dirs[:] = []
            continue

        for f in files:
            name, ext = os.path.splitext(f)
            if ext in exts:
                yield posix_root + '/' + f

def _include_dir_exceptions():
    return [s.replace('\\', '/')
            for p in INCLUDE_DIR_EXCEPTIONS
            for s in glob.glob(p)]

def _duplicate_name_exceptions():
    return set(_include_dir_exceptions() + DUPLICATE_FILE_EXCEPTIONS)

def duplicate_header_names():
    """Walk over all header files with duplicate names.

    Yields:
        Tuple(str, str): The file that has a duplicated name and the directory
            that the duplicate is in.
    Example:
        >>> from header_path_verify import duplicate_header_names
        >>> print([i for i in duplicate_header_names()])
        [('./path/to/file.c', './duplicate/dir'),
         ('./path/other_file.c', './another/duplicate_dir')]
    """
    visited = {}
    for f in _walk(".h"):
        name = os.path.basename(f)
        if name in visited and f not in _duplicate_name_exceptions():
            yield (f, visited[name])
        visited[name] = os.path.dirname(f)

def _header_filenames_are_unique():
    valid = True
    for f, dup in duplicate_header_names():
        sys.stderr.write("WARNING: found {0} in both {1} and {2}\n".format(
            f, os.path.dirname(f), dup))
        valid = False
    return valid

def includes_without_path():
    """Walk over all includes that do not have a path prefix.

    Walks over all includes that do not have a path prefix. Any headers present
    in the INCLUDE_DIR_EXCEPTIONS list are ignored.

    Yields:
        Tuple(str, int, str): A tuple containing the file name that includes
            a file without a path, the line number that this occurs and the
            header file name that it includes.

    Example:
        >>> from header_path_verify import includes_without_path
        >>> print([i for i in includes_without_path()])
        [('path/to/file.c', 5, 'include.h'),
         ('path/to/other_file.c', 10, 'header.h')]
    """
    files_in_include_dirs = set(os.path.basename(e)
                                for e in _include_dir_exceptions())
    include_re = re.compile("^\s*#\s*include\s+\"([^\"/]+)\"")
    for file_path in _walk(".h", ".c"):
        with open(file_path, 'r') as f:
            for lineno, line in enumerate(f):
                m = include_re.match(line)
                if m:
                    header = m.group(1)
                    if header not in files_in_include_dirs:
                        yield (file_path, lineno, header)

def _includes_contain_path():
    valid = True
    for f, lineno, header in includes_without_path():
        sys.stderr.write("{0}:{1}: WARNING: header {2}:\n"
            "    not in an include directory, included without path\n".format(
                f, str(lineno + 1), header))
        valid = False
    return valid

def validate_includes():
    """Validate include files.

    Traverses the current working dictory and confirms that:
    1. All header files have a unique name, ignoring exceptions in the
       file_exception_strings set.
    2. All includes have a directory prefixed to them, unless the included
       file's directory is in the include_dir_exception_strings set.

    Returns:
        bool: The return value. True if there are no include errors,
            False otherwise.
    Example:
        >>> from header_path_verify import validate_includes
        >>> validate_includes()
        WARNING: found ./private.h in both ./core/windows and ./core/win32
        WARNING: found ./test.h in both ./nfc/nfc_unit and ./nfc/nfc_test
        ./nfc/nfc_unit/llcp_srv_test.c:42: WARNING: header assert.h:
            not in an include directory, included without path
    """
    return all([_header_filenames_are_unique(), _includes_contain_path()])

if __name__ == "__main__":
    sys.exit(not validate_includes())
