# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
"""A script to produce region usages out of an elf"""
# This script outputs a summary of memory usage from a combination
# of running "knm" on the ELF file and information passed from the Makefile.
# The first argument is the path to the knm binary, as windoze
# doesn't give an easy to set this externally
# The second argument is the ELF file, remaining arguments are in the
# form NAME=NUMERIC_VALUE; as this appears in the Makefile these arguments
# will typically look like PROC_PROG_ROM_SIZE=${PROC_PROG_ROM_SIZE}
#
# Historical note:  this script was derived from a similar one
# used with curator, which itself replaced inline code in the Makefile.
# The output from the script is very similar; the script just makes
# it easier to perform sanity checks.

import os
import sys

class Symbols:
    """A means of holding the bits of the elf symbol table we want"""
    # Front end to a dictionary with symbols from the ELF file
    # and the command line, used for spotting missing symbols.

    def __init__(self):
        """Initialise the module: docstring to shut up pylint"""
        self.symbols = { }

    def add(self, sy_name, sy_value):
        """Add a symbol to our table"""
        self.symbols[sy_name] = sy_value

    def get(self, sy_name):
        """Look up a symbol in our table"""
        if self.symbols.has_key(sy_name):
            return self.symbols[sy_name]
        sys.stderr.write("WARNING: symbol %s not in ELF file, substituting 0\n"
                         % sy_name)
        return 0

from subprocess import Popen, PIPE
from re import split

prog = sys.argv[0]
sys.argv = sys.argv[1:]

if len(sys.argv) < 2:
    sys.stderr.write("Usage: %s <path to nm> <elf-file> [ NAME=NUMERIC_VALUE ... ]\n", prog)
    sys.exit(1)
nm_path = sys.argv[0]
elf_file = sys.argv[1]
sys.argv = sys.argv[2:]

# Get a dictionary of symbols and values from the ELF file using knm.
sub = Popen([nm_path, elf_file], stdout=PIPE)
symbols = Symbols()

for line in sub.stdout:
    array  = split(' +', line.rstrip())
    if len(array) != 3:
        # Complain if the format is unexpected.
        sys.stderr.write("WARNING: failed to understand nm line:\n%s" %
                         (line))
        continue
    value = array[0]
    name = array[2]
    # These values are implicitly in hex
    # The elf may contain undefined symbols, of type U
    if "U" != array[1]:
        symbols.add(name, int(value, 16))

status = sub.wait()
if status != 0:
    sys.stderr.write("Bad status (%d) from nm subprocess\n", status)
    sys.exit(1)

# Get the values from the command line.
# These override anything found by nm.
for elt in sys.argv:
    (name, value) = split('=', elt)
    # These values have a base indicator
    symbols.add(name, int(value, 0))

# Exit status, non-zero if a test below fails.
status = 0

# Now do our calculations.
# RAM sizes
p0_ram_base = symbols.get("$P0D_P0_DM_RAM_LOWER_ENUM")
p0_ram_top = symbols.get("$P0D_P0_DM_RAM_UPPER_ENUM")
p0_ram_size = p0_ram_top - p0_ram_base
p1_ram_base = symbols.get("$P0D_P1_DM_RAM_LOWER_ENUM")
p1_ram_top = symbols.get("$P0D_P1_DM_RAM_UPPER_ENUM")
p1_ram_size = p1_ram_top - p1_ram_base

tcm_code_start = symbols.get("$MEM_MAP_TCM_CODE_START")
tcm_code_end = symbols.get("$MEM_MAP_TCM_CODE_END")
tcm_code_size = tcm_code_end - tcm_code_start

code_start = symbols.get("$MEM_MAP_CODE_START")
code_end = symbols.get("$MEM_MAP_CODE_END")
code_size = code_end - code_start

regular_code_size = code_end - tcm_code_end

rom_const_start = symbols.get("$MEM_MAP_CONST_START")
rom_const_end = symbols.get("$MEM_MAP_CONST_END")
rom_const_size = rom_const_end - rom_const_start

tcm_const_start = symbols.get("$MEM_MAP_CONST_TCM_START")
tcm_const_end = symbols.get("$MEM_MAP_CONST_TCM_END")
tcm_const_size = tcm_const_end - tcm_const_start

initc_start = symbols.get("$MEM_MAP_INITC_START")
initc_end = symbols.get("$MEM_MAP_INITC_END")
initc_size = initc_end - initc_start
initc_region_size, ram_size = (p0_ram_size, p0_ram_size) if initc_start < p0_ram_top else (p1_ram_size, p1_ram_size)
# Convert from bytes to K.
ram_size_k_octets = ram_size / 1024
code_size_k_octets = code_size /1024

bss_start = symbols.get("$MEM_MAP_BSS_START")
bss_end = symbols.get("$MEM_MAP_BSS_END")
bss_size = bss_end - bss_start
bss_region_size = p0_ram_size if bss_start < p0_ram_top else p1_ram_size

total_var_size = initc_size + bss_size

pmalloc_start = symbols.get("$MEM_MAP_PMALLOC_START")
pmalloc_end = symbols.get("$MEM_MAP_PMALLOC_END")
pmalloc_size = pmalloc_end - pmalloc_start
pmalloc_region_size = p0_ram_size if pmalloc_start < p0_ram_top else p1_ram_size

stack_start = symbols.get("$MEM_MAP_STACK_START")
stack_end = symbols.get("$MEM_MAP_STACK_END")
stack_size = stack_end - stack_start
stack_region_size = p0_ram_size if stack_start < p0_ram_top else p1_ram_size

total_used_dm_ram = (stack_size +  total_var_size + pmalloc_size)
unused_local_dm_ram = ram_size  - total_used_dm_ram 

shared_ram_size_bytes = symbols.get("$SHARED_RAM_SIZE_BYTES")
shared_ram_size_k_octets = shared_ram_size_bytes/1024

shared_section_start = symbols.get("$MEM_MAP_SHARED_MEMORY_START")
shared_section_end = symbols.get("$MEM_MAP_SHARED_MEMORY_END")
shared_section_size = shared_section_end - shared_section_start

mmu_handles_start = symbols.get("$MEM_MAP_MMU_HANDLES_START")
mmu_handles_end = symbols.get("$MEM_MAP_MMU_HANDLES_END")
mmu_handles_size = mmu_handles_end - mmu_handles_start
mmu_pages_start = symbols.get("$MEM_MAP_MMU_PAGES_START")
mmu_pages_end = symbols.get("$MEM_MAP_MMU_PAGES_END")
mmu_pages_size = mmu_pages_end - mmu_pages_start

total_used_shared_ram = shared_section_size + mmu_handles_size + mmu_pages_size
unused_shared_ram = shared_ram_size_bytes - total_used_shared_ram

# Output the result.
width = str(33)
str_fmt = "  %-" + width + "s"
size_fmt = str_fmt + " %d (0x%x) = %d (0x%x) %s"

all = list()
all.append("\nAll sizes in bytes unless specified")

def write_details(cur_details):
    global all
    all.append(cur_details) 

write_details((str_fmt + " %d (0x%08x) ") % (
    "Code size =", code_size, code_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  TCM code =", tcm_code_size, tcm_code_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  PM code =", regular_code_size, regular_code_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  ROM const =", rom_const_size, rom_const_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  TCM const =", tcm_const_size, tcm_const_size))

write_details((size_fmt % (
    "Private DM size =", ram_size, ram_size,
    ram_size_k_octets, ram_size_k_octets, "kB")))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  RAM given to variables =", total_var_size, total_var_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "    BSS data =", bss_size, bss_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "    InitC data =", initc_size, initc_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  RAM given to pmalloc pools =", pmalloc_size, pmalloc_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  RAM given to stack =", stack_size, stack_size))

#Only show the used and unused numbers if they are interesting: if we have some unused
if unused_local_dm_ram:
    write_details((str_fmt + " %d (0x%08x) ") % (
        "  RAM used =", total_used_dm_ram , total_used_dm_ram))
    write_details((str_fmt + " %d (0x%08x) ") % (
        "  RAM unused =", unused_local_dm_ram , unused_local_dm_ram))

write_details((size_fmt % (
    "Shared DM size =", shared_ram_size_bytes, shared_ram_size_bytes,
    shared_ram_size_k_octets, shared_ram_size_k_octets, "kB")))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  RAM given to MMU handles =", mmu_handles_size, mmu_handles_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  RAM given to MMU pages =", mmu_pages_size, mmu_pages_size))

write_details((str_fmt + " %d (0x%08x) ") % (
    "  RAM given to smalloc =", shared_section_size, shared_section_size))


#Only show the used and unused numbers if they are interesting: if we have some unused
if unused_shared_ram:
    write_details((str_fmt + " %d (0x%08x) ") % (
        "  RAM used =", total_used_shared_ram , total_used_shared_ram))
    write_details((str_fmt + " %d (0x%08x) ") % (
        "  RAM unused =", unused_shared_ram , unused_shared_ram))    

#Now write the details to stdout and file 
build_dir = os.path.dirname(elf_file) 
out_file = open(os.path.join(build_dir, "mem_usage.txt"), "w")
out_file.write("Build: %s\n" % elf_file) 

for i in all:
    print i
    out_file.write(i+"\n")    

out_file.close()

# And write a CSV file that Jenkins can use to plot nice graphs.
out_file = open(os.path.join(build_dir, "mem_usage.csv"), "w")
out_file.write("code_total, code_tcm, code_pm, code_tcm_const, code_pm_const, data_total, data_bss, data_initc, data_pools, shared_total, shared_used,\n")
out_file.write("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n" % (code_size, tcm_code_size, regular_code_size, tcm_const_size, rom_const_size,
                                                          ram_size, bss_size, initc_size, pmalloc_size, shared_ram_size_bytes, total_used_shared_ram))
out_file.close()

sys.exit(status)

