#!/usr/bin/env python2.6
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2

'''A script to analyse memory usage for an apps build. The name du is a hangover from the du (disc usage) utility
'''
import sys
import types
import os
import subprocess

have_nm = False
have_loc = False
have_in = False
have_out = False

nm_code_dict = {}
nm_data_dict = {}
code_addr_dict = {}
data_addr_dict = {}
lib_dict = {}
loc_dict = {}
h_list = []
i_list = []
hierarchy_dict = {}
top_dict = {}
# We might override these on the command line
objdump = "objdump"
nm = "nm"

# Slightly hacky that we have to put this here
# but there doesn't seem an easy way to pass it in
code_sects = ['.text', '.text_reset', '.text_pre', '.rodata', '.text_shadow']

verbose = False
debug = False

def usage():
    '''The function to print out usage info. Shouldnt need a docstring, duh'''
    sys.stderr.write('usage: mem_du.py -n <nm file> -l <loc file> -o <output>')

def skip_spaces(line):
    '''A space skipper'''
    j = 0
    while j < len(line):
        if ' ' == line[j]:
            j += 1
        else:
            break
    return line[j : ]

def int_read(aString):
    '''Read an int in hex or decimal, or even octal (thought not to be used)'''
    if aString.startswith("0x") or aString.startswith("0X"):
        return int(aString, 16)
    elif aString.startswith("0"):
        return int(aString, 8)
    else:
        return int(aString)

def parse_nm_line(line):
    '''Parse a line of nm info, looks like eg 00020000 B $_mmu_handles'''
    line = skip_spaces(line)
    if debug:
        print 'After space skip line is', line
    j = line.find(' ')
    if -1 == j:
        sys.stderr.write('Unexpected nm file address "%s", terminating\n' % line)
        sys.exit(1)
    if debug:
        print 'Next space is at', j
    addr = line[ : j ]
    if debug:
        print 'Parsing', addr, 'as hex int'
    line = skip_spaces(line[j : ])
    if debug:
        print 'After space skip line is', line
    j = line.find(' ')
    if -1 == j:
        sys.stderr.write('Unexpected nm file type "%s", terminating\n' % line)
        sys.exit(1)
    if debug:
        print 'Next space is at', j
    text = line[ : j ]
    line = skip_spaces(line[j : ])
    if debug:
        print 'After space skip line is', line
    return (int(addr, 16), text, line)

def read_nm_file(my_nm_file):
    '''Read the whole file of nm info and feed the lines to parse_nm_line'''
    for line in my_nm_file:
        (addr, text, name) = parse_nm_line(line.strip())
        if debug:
            print 'addr =', addr, 't =', text, 'name =', name
        # Put the read only stuff in the code area, not the data
        if 'T' == text.upper():
            if debug:
                print 'Adding', name, 'to code dictionary at address', addr
            nm_code_dict[name] = addr
        else:
            if debug:
                print 'Adding', name, 'to data dictionary at address', addr
            nm_data_dict[name] = addr

def parse_split_loc_line(names):
    '''Parse location info from a list, saying where each symbol is defined'''
    if 3 != len(names):
        sys.stderr.write('Unexpected loc file line "%s", terminating\n' % line)
        sys.exit(1)
    loc_dict[names[2]] = names[1]
    if not lib_dict.has_key(names[1]):
        lib_dict[names[1]] = names[0]
    if debug:
        print 'Found def of', names[2], 'in', names[1], 'lib', names[0]

def parse_loc_line(line):
    '''Parse location info from a string, saying where each symbol is defined'''
    names = line.split(':')
    parse_split_loc_line(names)

def read_loc_file(my_loc_file):
    '''Read and parse location info, saying where each symbol is defined'''
    for line in my_loc_file:
        parse_loc_line(line.strip())

def parse_h_line(line):
    '''Parse a line of header information, giving the section names, bases and sizes'''
    # Item 1 is the section name, 2 is the size (in bytes), 3 is the address
    # Looking up by address is the most useful
    if verbose:
        sys.stdout.write('Adding %s of size %s and address %s to sections\n' %
                         (line[0], line[1], line[2]))
    h_list.append((int(line[2], 16), int(line[1], 16), line[0]))

def read_h_file(my_h_file):
    '''Read and parse the elf header information'''
    for line in my_h_file:
        parse_h_line(line.strip())

def get_header_info(my_elf_file):
    '''Get the header info by interrogating objdump'''
    p = subprocess.Popen([objdump, "-h", x ],
                         stdout = subprocess.PIPE)
    my_h_list = []
    had_idx = False
    for line in p.stdout:
        line_split = line.split()
        if not had_idx:
            if "Idx" in line_split and "Name" in line_split:
                had_idx = True
            continue # Ignore everything up to the Idx Name line
        if line_split[0].isdigit():
            # Got a line starting number, so not a comment line
            line_split = line_split[1:]
            # Now get rid of .comment, .debug, .stab and debug_strings
            if ".debug" in line_split or ".comment" in line_split or ".stab" in line_split or "debug_strings" in line_split:
                continue
            parse_h_line(line_split)

def get_ignore_info(my_elf_file):
    '''Get the list of symbols to ignore by interrogating objdump'''
    p = subprocess.Popen([objdump, "-t", x ],
                         stdout = subprocess.PIPE)
    my_i_list = []
    for line in p.stdout:
        line_split = line.split()
        if "debug_strings" in line_split and "O" in line_split:
            my_i_list.append(line_split[-1])
    i_list = sorted(set(my_i_list))

def split_path(path):
    '''Split a path into a list of components
    Clearly not tail recursive, but we expect depth to be at most 2'''
    (head, tail) = os.path.split(path)
    if "" == head:
        return [tail]
    else:
        foo = split_path(head)
        foo.append(tail)
        return foo # NB Could not concatinate the above lines as in 'split_path(head).append(fail)'

def lookup_section_soft(addr, code):
    '''Find which section an address lives in. code say true (code), false (data)
    Does not fault on error'''
    best = ''
    for (a, l, k) in h_list:
        if debug:
            print("Trying base = 0x%x, length = 0x%x, section %s" % (a, l, k))
        if (code and k in code_sects) or (not (code or k in code_sects)):
            if a <= addr and addr <= a + l:
                if addr < a+l:
                    return k
                best = k
        else:
            if verbose:
                sys.stdout.write('Ignoring section %s for address 0x%x due to CODE/RAM mismatch\n' % (k, addr))
    if '' != best:
        return best
    return None

def lookup_section(addr, code):
    '''Like lookup_section_soft, but errors and exits on lookup failure'''
    ans = lookup_section_soft(addr, code)
    if None != ans:
        return ans
    sys.stderr.write('address 0x%x not found, code %s, terminating\n' % (addr, "true" if code else "false"))
    sys.exit(1)

def find_section(addr, sections_list):
    '''Find the section containing this address'''
    for (start, length, section_name) in sections_list:
        if start <= addr and addr < start + length:
            # Duplicate list element as python doesn't seem to allow "as"
            return (start, length, section_name)
    return None # Oops, didn't find it at all

def mkoutput(name, addr, region):
    '''Produce the output string'''
    code = region == 'CODE'
    if verbose:
        sys.stdout.write('Making output for %s at address 0x%x in %s\n' % (name, addr, region))
    if loc_dict.has_key(name):
        fname = loc_dict[name]
        lib = lib_dict[fname]
        section = lookup_section(addr, code)
        l = [region, section]
        # lib may be a multi component path
        lib_path = split_path(lib)
        l.extend(lib_path)
        l.extend([fname, name])
        return l
    else:
        if verbose:
            print 'Fail to find location for', name
        # Here we process a number of exceptions
        if 'pmalloc_begin' == name or 'pmalloc_end' == name: # Total hack
            section = lookup_section_soft(addr, code)
            if None != section:
                if verbose:
                    print 'Returning', region, '/', section, '/', name, addr
                return [region, section, name]
            else:
                if verbose:
                    sys.stdout.write('Fail to find 0x%x in sections\n' % addr)
        if verbose:
            print 'Returning fail for lookup of', name, 'in loc_dict'
        return [''] # Dummy values

def process_hierarchy(li, size):
    '''Processes a hierarchy for a symbol, setting up
    the dictionaries needed by print_hierarchy'''
    # Arg is a list from top to bottom of hierarchy, right down to symbol
    elt = '.'
    my_dict = top_dict
    for x in li:
        if not my_dict.has_key(x):
            # This bit not added yet
            my_dict[x] = {}
            # Now process rest
        my_dict = my_dict[x] # Move down hierarchy
        if debug:
            print 'Adding', x, 'to', elt
        elt = x
    # Now add the size. elt is now the symbol
    my_dict[elt] = size

def print_hierarchy(my_file, in_dict, prefix):
    '''Print the final results to my_file
    in_dict is a dictionary of all stuff below current level
    prefix is what to prepend for this level
    Recurses down until it finds values which are integers rather than dictionaries
    Each is then printed with its size, and prepended with the prefix'''
    size = 0 # Nothing in this hierarchy yet
    li = in_dict.items()
    for (key, value) in li:
        if debug:
            print 'Computing hierarchy for"', key, '"', value
        # Iterate over the values unless they are ints
        if types.IntType == type(value) or types.LongType == type(value):
            size += value
            if debug:
                print "Type is int"
        else:
            if debug:
                print "value has type", type(value)
            if '' != prefix:
                new_prefix = '/'.join([prefix, key])
                if verbose:
                    sys.stdout.write("Create %s from %s and %s\n" % (new_prefix, prefix, key))
            else:
                new_prefix = key
            size += print_hierarchy(my_file, value, new_prefix)
        if debug:
            print 'Incrementing size for"', prefix, '"to', size
    # Now print out the hierarchy for this level
    my_file.write('%u ./%s\n' % (size, prefix))
    return size

argc = len(sys.argv)

i = 1
while i < argc:
    x = sys.argv[i]
    if debug:
        print 'Handling x', x
    if x == '-n':
        if i + 1 >= argc:
            # Missing parameter
            sys.stderr.write('Missing nm file\n')
            usage()
            sys.exit(1)
        i += 1
        if have_nm:
            sys.stderr.write('nm file already specified\n')
            usage()
            sys.exit(1)
        try:
            nm_file = open(sys.argv[i], 'r')
        except IOError:
            sys.stderr.write('%s not found\n' % sys.argv[i])
            sys.exit(1)
        read_nm_file(nm_file)
        have_nm = True
        nm_file.close()
    elif x == '-l':
        if i + 1 >= argc:
            # Missing parameter
            sys.stderr.write('Missing loc file\n')
            usage()
            sys.exit(1)
        i += 1
        if have_loc:
            sys.stderr.write('loc file already specified\n')
            usage()
            sys.exit(1)
        try:
            loc_file = open(sys.argv[i], 'r')
        except IOError:
            sys.stderr.write('%s not found\n' % sys.argv[i])
            sys.exit(1)
        read_loc_file(loc_file)
        have_loc = True
        loc_file.close()
    elif x == '-o':
        if i + 1 >= argc:
            # Missing parameter
            sys.stderr.write('Missing output file\n')
            usage()
            sys.exit(1)
        i += 1
        if have_out:
            sys.stderr.write('Output file already specified\n')
            usage()
            sys.exit(1)
        try:
            out_file = open(sys.argv[i], 'w')
        except IOError:
            sys.stderr.write('%s: cannot open\n' % sys.argv[i])
            sys.exit(1)
        have_out = True
    elif x == '-v':
        verbose = True
    elif x == '-d':
        debug = True
    else:
        # Must be the name of the elf file
        # We'll use this to get the ignored symbol list
        if not have_in:
            have_in = True
            if os.path.isfile(x):
                # Try out objdump for ignore and header
                get_ignore_info(x)
                get_header_info(x)
            else:
                sys.stderr.write('Elf file "%s" not found\n' % x)
                usage()
                sys.exit(1)
        else:
            # Can't have more than one object file
            sys.stderr.write('Unrecognised parameter "%s"\n' % x)
            usage()
            sys.exit(1)

    i += 1

if not (have_nm and have_out and have_loc and have_in):
    if not have_nm:
        sys.stderr.write('Missing nm parameter\n')
    if not have_loc:
        sys.stderr.write('Missing lib parameter\n')
    if not have_in:
        sys.stderr.write('Missing elf file\n')
    if not have_out:
        sys.stderr.write('Missing out parameter\n')
    usage()
    sys.exit(1)

# Processing
# First strip out the ignore file entries from the nm dictionaries
for x in i_list:
    if x in nm_code_dict:
        del nm_code_dict[x]
    else:
        print 'Strange,', x, 'not found in nm_code_dict'
# Get the code and data ranges
# Now associate those ranges with files
# Current scheme, a bit painful
# First setup a reverse association addresses to symbols
# Sort this by address
# Then build a dict of address => next address
nm_code_items = nm_code_dict.items()
nm_code_items.sort(cmp=lambda a,b : cmp(a[1],b[1]))
if verbose:
    print 'nm_code_items contains:'
    for (k, a) in nm_code_items:
        sys.stdout.write('Name: %s address: 0x%x\n' % (k, a))
old = 0
for (k, a) in nm_code_items:
    code_addr_dict[old] = a - old
    old = a
# Finally add a terminating length 0 symbol
if debug:
    print 'adding', old, 'of length', 0
code_addr_dict[old] = 0
if verbose:
    print 'code_addr_dict contains:'
    for (a, s) in code_addr_dict.items():
        sys.stdout.write('Address: 0x%x, size: 0x%x\n' % (a, s))
nm_data_items = nm_data_dict.items()
nm_data_items.sort(cmp=lambda a,b : cmp(a[1],b[1]))
if verbose:
    print 'nm_data_items contains:'
    for (k, a) in nm_data_items:
        sys.stdout.write('Name: %s address: 0x%x\n' % (k, a))
old = 0
# We need to cope with a and old being in different, possibly discontiguous sections
for (k, a) in nm_data_items:
    # If old is in one section, and a is in another
    # terminate old with end of previous section so we don't get silly sizes
    (start, length, name) = find_section(old, h_list)
    data_addr_dict[old] = a - old if a <= start + length else start + length - old
    if verbose:
        sys.stdout.write('Adding size as 0x%x for 0x%x with successor 0x%x\n' % (data_addr_dict[old], old, a))
    old = a
# Finally add a terminating length 0 symbol
if debug:
    print 'adding', old, 'of length', 0
data_addr_dict[old] = 0
if verbose:
    print 'data_addr_dict contains:'
    for (a, s) in data_addr_dict.items():
        sys.stdout.write('Address: 0x%x, size: 0x%x\n' % (a, s))
# Now look up symbols to get filenames
# Need to order this stuff by file and then library
for (k, a) in nm_code_dict.items():
    ad = nm_code_dict[k]
    size = code_addr_dict[nm_code_dict[k]]
    if verbose:
        sys.stdout.write('process %s at address 0x%x of size 0x%x\n' % (k, ad, size))
    li = mkoutput(k, a, 'CODE')
    if '' != li[0]:
        process_hierarchy(li, size)
for (k, a) in nm_data_dict.items():
    ad = nm_data_dict[k]
    if debug:
        if not data_addr_dict.has_key(ad):
            print 'Failed to lookup', ad, 'in data_addr_dict'
            print 'data_addr_dict contains:'
            for (a, s) in data_addr_dict.items():
                print 'Address:', a, 'size:', s
            sys.exit(1)
    size = data_addr_dict[nm_data_dict[k]]
    if verbose:
        sys.stdout.write('process %s at address 0x%x of size 0x%x\n' % (k, ad, size))
    li = mkoutput(k, a, 'RAM')
    if '' != li[0]:
        process_hierarchy(li, size)
# Now output all the results
print_hierarchy(out_file, top_dict, '')
out_file.close()
