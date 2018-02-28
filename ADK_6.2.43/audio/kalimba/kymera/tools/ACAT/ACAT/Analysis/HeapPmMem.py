############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2016 - 2017 Qualcomm Technologies International, Ltd.
#
############################################################################
"""
Module which analyses the heap PM ram.
"""
from ACAT.Core import CoreTypes as ct
from . import Analysis
from ..Core import Arch

CONSTANT_DEPENDENCIES = {'not_strict': ('$__pm_heap_start_addr', '$PM_RAM_P0_CODE_START')}
VARIABLE_DEPENDENCIES = {
    'not_strict': (
        'L_freelist_pm', 'L_freelist_pm_p1', 'L_heap_pm_start',
        'L_heap_pm_end', 'L_heap_pm_p1_start', 'L_heap_pm_p1_end',
        'L_addnl_heap_pm_p1_start', 'L_addnl_heap_pm_p1_end',
        'L_pm_reserved_size'
    )
}
TYPE_DEPENDENCIES = {
    'mem_node_pm': (
        'struct_mem_node.u.magic', 'struct_mem_node.length_32',
        'struct_mem_node.u.next'
    )
}


class HeapPmMem(Analysis.Analysis):
    """
    @brief This class encapsulates an analysis for heap memory usage.
    """

    def __init__(self, **kwarg):
        # Call the base class constructor.
        Analysis.Analysis.__init__(self, **kwarg)

        # Look up the debuginfo once. Don't do it here though; we don't want
        # to throw an exception from the constructor if something goes
        # wrong.
        self._do_debuginfo_lookup = True
        self.freelist = None
        self.freelist_p1 = None
        self.heap_pm_start = None
        self.heap_pm_end = None
        self.heap_pm_p1_start = None
        self.heap_pm_p1_end = None
        self.addnl_p1_start = None
        self.addnl_p1_end = None
        self.patch_size = None

        # There is an older version of PM heap that had fixed sizes for P0 and P1 heap.
        # If newer variables are not present, raise the outdated firmware exception.
        try:
            self.debuginfo.get_var_strict("L_heap_pm_start")
        except ct.DebugInfoNoVariable:
            # fallback to the old implementation
            raise ct.OutdatedFwAnalysisError()

    def run_all(self):
        """
        @brief Perform all useful analysis and spew the output to the formatter.
        Displays the heap_pm memory usage.
        @param[in] self Pointer to the current object
        """
        # Look up the debug information, unless we already have.
        if self._do_debuginfo_lookup:
            self._lookup_debuginfo()
            self._do_debuginfo_lookup = False

        self.analyse_pm_memory_map()

        self.formatter.section_start('Heap PM Memory Info')

        if self.chipdata.processor != 0:
            self.formatter.output(
                "Heap PM Memory Analysis is not available for processor %d. \n"
                "The control variables for Heap PM are only available in p0 "
                "domain." % self.chipdata.processor
            )
            return

        if self.freelist is not None:
            p0_size = self.chipdata.get_data(self.heap_pm_end.address) - \
                                    self.chipdata.get_data(self.heap_pm_start.address)

            if p0_size == 0:
                self.formatter.output(
                    "No PM heap present on P0\n"
                    "Is P1PMHeapAllocation MIB key set to allocate all memory to P1?")
            else:
                self.formatter.output("Analysing heap pm P0 content")
                p0_start = self.chipdata.get_data(
                    self.heap_pm_start.address
                )
                self.formatter.output_list(
                    self.
                    _free_blocks_pm(self.chipdata.get_data(self.freelist))
                )
                heap_pm_info = self._alloc_blocks_pm(p0_start, p0_size)

                self.formatter.output_list(heap_pm_info)

        if self.freelist_p1 is not None:
            p1_size = self.chipdata.get_data(self.heap_pm_p1_end.address) - \
                                        self.chipdata.get_data(self.heap_pm_p1_start.address)

            addnl_p1_size = 0
            if self.addnl_p1_start is not None:
                addnl_p1_size = self.chipdata.get_data(self.addnl_p1_end.address) - \
                                          self.chipdata.get_data(self.addnl_p1_start.address)

            if (p1_size + addnl_p1_size) == 0:
                self.formatter.output(
                    "No PM heap present on P1\n"
                    "Check values for P1PMHeapAllocation and CoreEnable MIB keys\n"
                    "P1 could also have been disabled at run time")
            else:
                self.formatter.output("Analysing heap pm P1 content")

                # free blocks is common to actual and additional P1 heap
                self.formatter.output_list(
                    self.
                    _free_blocks_pm(self.chipdata.get_data(self.freelist_p1))
                )

                if p1_size != 0:
                    p1_start = self.chipdata.get_data(
                        self.heap_pm_p1_start.address
                    )
                    heap_pm_p1_info = self._alloc_blocks_pm(
                        p1_start, p1_size, "heap_pm_p1"
                    )
                    self.formatter.output_list(heap_pm_p1_info)

                if addnl_p1_size != 0:
                    addnl_p1_start = self.chipdata.get_data(
                        self.addnl_p1_start.address
                    )
                    addnl_heap_pm_p1_info = self._alloc_blocks_pm(
                        addnl_p1_start, addnl_p1_size, "addnl_heap_pm_p1"
                    )
                    self.formatter.output_list(addnl_heap_pm_p1_info)

        self.formatter.section_end()

    #######################################################################
    # Analysis methods - public since we may want to call them individually
    #######################################################################

    def analyse_pm_memory_map(self):
        """
        @brief Display PM memory map based on current settings
        @param[in] self Pointer to the current object
        """
        # Look up the debug information, unless we already have.
        if self._do_debuginfo_lookup:
            self._lookup_debuginfo()
            self._do_debuginfo_lookup = False

        self.formatter.section_start('PM Memory Map')

        p0_size = 0
        p1_size = 0
        addnl_p1_size = 0
        code_start = 0
        code_end = 0
        patch_size = 0

        if self.freelist is not None:
            p0_start = self.chipdata.get_data(self.heap_pm_start.address)
            p0_end = self.chipdata.get_data(self.heap_pm_end.address)
            p0_size = p0_end - p0_start

        if self.freelist_p1 is not None:
            p1_start = self.chipdata.get_data(self.heap_pm_p1_start.address)
            p1_end = self.chipdata.get_data(self.heap_pm_p1_end.address)
            p1_size = p1_end - p1_start
            # Only need to check additional P1 PM heap if the main one is present
            if self.addnl_p1_start is not None:
                addnl_p1_start = self.chipdata.get_data(self.addnl_p1_start.address)
                addnl_p1_end = self.chipdata.get_data(self.addnl_p1_end.address)
                addnl_p1_size = addnl_p1_end - addnl_p1_start

        if self.patch_size is not None:
            patch_size = self.chipdata.get_data(self.patch_size)

        try:
            code_start = self.debuginfo.get_constant_strict(
                '$PM_RAM_P0_CODE_START'
            ).value
            code_end = self.debuginfo.get_constant_strict(
                '$__pm_heap_start_addr'
            ).value
        except ct.DebugInfoNoVariable:
            raise KeyError(
                'Constants for static code start and end address is not specified in build')

        # Now that all values are read, create the memory map
        if addnl_p1_size != 0:
            self.formatter.output("==============================<-{:8x}".format(addnl_p1_end))
            self.formatter.output("|   Additional P1 PM Heap    |")
            self.formatter.output("|        {:6d} Bytes        |".format(addnl_p1_size))
            self.formatter.output("==============================<-{:8x}".format(addnl_p1_start))
            self.formatter.output("|         P1 Cache           |")
            self.formatter.output("==============================<-{:8x}".format(p1_end))

        if p1_size != 0:
            if addnl_p1_size == 0:
                self.formatter.output("==============================")
                self.formatter.output("|         P1 Cache           |")
                self.formatter.output("==============================<-{:8x}".format(p1_end))
            self.formatter.output("|        P1 PM Heap          |")
            self.formatter.output("|       {:7d} Bytes        |".format(p1_size))
            self.formatter.output("==============================<-{:8x}".format(p1_start))

        if p0_size != 0:
            if p1_size == 0 and addnl_p1_size == 0:
                # No P1
                self.formatter.output("==============================<-{:8x}".format(p0_end))
            self.formatter.output("|        P0 PM Heap          |")
            self.formatter.output("|       {:7d} Bytes        |".format(p0_size))
            self.formatter.output("==============================<-{:8x}".format(p0_start))

        if patch_size != 0:
            self.formatter.output("|        Patch Code          |")
            self.formatter.output("|         {:5d} Bytes        |".format(patch_size))

        if code_start != 0 and code_end != 0:
            if patch_size != 0:
                self.formatter.output("==============================<-{:8x}".format(code_end))
            self.formatter.output("|                            |")
            self.formatter.output("|       PM RAM Code          |")
            self.formatter.output("|                            |")
            self.formatter.output("==============================<-{:8x}".format(code_start))

        self.formatter.output("==============================")
        self.formatter.output("|         P0 Cache           |")
        self.formatter.output("==============================")
        self.formatter.section_end()

    def _free_blocks_pm(self, address):

        free_info = []
        while address != 0:
            freeblock = self.chipdata.cast(address, 'mem_node_pm', False, 'PM')
            free_info.append(
                "Free block size : {0:>d} at address: {1:0>6x}".format(
                    freeblock.get_member('struct_mem_node').
                    get_member('length_32').value * Arch.addr_per_word, address
                )
            )
            address = freeblock.get_member('struct_mem_node').get_member(
                'u'
            ).get_member('next').value
        return free_info

    def _alloc_blocks_pm(self, start_addr, pm_size, heap_name="heap_pm"):

        magic_val = 0xabcd01
        alloc_info = []

        # Get the address we will be working with from the start of heap_pm
        address = start_addr

        heap_pm_data = self.chipdata.get_data_pm(
            start_addr, pm_size
        )
        total = 0

        # here index is the index of the magic word in heap_pm_data. Since in
        # heap_pm we are working with 32-bit words and not strictly addresses,
        # for start, index should actually be
        # -Arch.addr_per_word/Arch.arrd_per_word, which is -1
        index = -1

        testblock = self.chipdata.cast(address, 'mem_node_pm', False, 'PM')
        testblock_address = testblock.get_member('struct_mem_node').get_member(
            'u'
        ).get_member('magic').address
        # magic_offset here shows the distance between two 32-bit words, first
        # being start of the test block and second being magic value
        magic_offset = (testblock_address - address) / Arch.addr_per_word

        # Search through the entire heap_pm block, looking for allocated blocks
        # based on the presence of the magic word
        while True:
            try:
                index = index + heap_pm_data[index + 1:].index(magic_val) + 1
                address = start_addr + \
                    (index - magic_offset) * Arch.addr_per_word
                testblock = self.chipdata.cast(
                    address, 'mem_node_pm', False, 'PM'
                )
                magic = testblock.get_member('struct_mem_node').get_member(
                    'u'
                ).get_member('magic').value
                if magic != magic_val:
                    raise ct.AnalysisError(
                        "Magic word not found at expected offset.")

                # Get length of memory allocation.
                # Since lenght_32 gives us the number of 32-bit words and not
                # The memory size, we convert it by multiplying by
                # Arch.arrd_per_word
                length = testblock.get_member('struct_mem_node').get_member(
                    'length_32'
                ).value * Arch.addr_per_word
                # Check if we are still in valid region
                if (length > 0) and (
                        address + length <= start_addr + pm_size
                    ):
                    alloc_info.append(
                        "Allocated block size : {0:>4d} at address: {1:0>6x}".
                        format(length, address)
                    )
                    index = index + length / Arch.addr_per_word
                    total = total + length
            except ValueError:
                break

        alloc_info.append("Total {0:s} allocation : {1:>d}".format(heap_name, total))

        return alloc_info

    def _lookup_debuginfo(self):
        """
        @brief Queries debuginfo for information we will need to get the heap
            pm memory usage
        @param[in] self Pointer to the current object
        """

        try:
            self.freelist = self.debuginfo.get_var_strict(
                'L_freelist_pm'
            ).address
        except ct.DebugInfoNoVariable:
            self.freelist = None

        try:
            self.freelist_p1 = self.debuginfo.get_var_strict(
                'L_freelist_pm_p1'
            ).address
        except ct.DebugInfoNoVariable:
            self.freelist_p1 = None

        try:
            self.patch_size = self.debuginfo.get_var_strict(
                'L_pm_reserved_size'
            ).address
        except ct.DebugInfoNoVariable:
            self.patch_size = None

        if self.freelist is not None:
            self.heap_pm_start = self.debuginfo.get_var_strict('L_heap_pm_start')
            self.heap_pm_end = self.debuginfo.get_var_strict('L_heap_pm_end')

        if self.freelist_p1 is not None:
            self.heap_pm_p1_start = self.debuginfo.get_var_strict('L_heap_pm_p1_start')
            self.heap_pm_p1_end = self.debuginfo.get_var_strict('L_heap_pm_p1_end')
            try:
                self.addnl_p1_start = self.debuginfo.get_var_strict('L_addnl_heap_pm_p1_start')
                self.addnl_p1_end = self.debuginfo.get_var_strict('L_addnl_heap_pm_p1_end')
            except ct.DebugInfoNoVariable:
                self.addnl_p1_start = None
                self.addnl_p1_end = None
