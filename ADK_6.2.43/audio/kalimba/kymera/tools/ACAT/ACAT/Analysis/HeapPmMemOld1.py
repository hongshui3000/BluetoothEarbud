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

CONSTANT_DEPENDENCIES = {'not_strict': ('$PM_HEAP_SIZE', '$PM_P1_HEAP_SIZE')}
VARIABLE_DEPENDENCIES = {
    'not_strict': (
        'L_freelist_pm', 'L_freelist_pm_p1', 'L_heap_pm_adjusted',
        'L_heap_pm_adjusted_p1'
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
        self.heap_pm = None
        self.heap_pm_p1 = None

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

        self.formatter.section_start('Heap PM Memory Info')

        if self.chipdata.processor != 0:
            self.formatter.output(
                "Heap PM Memory Analysis is not available for processor %d. \n"
                "The control variables for Heap PM are only available in p0 "
                "domain." % self.chipdata.processor
            )
            return

        if self.freelist is not None:
            try:
                pm_heap_region_size = self.debuginfo.get_constant_strict(
                    '$PM_HEAP_SIZE'
                ).value
            except ct.DebugInfoNoVariable:
                raise KeyError(
                    'Constant $PM_HEAP_SIZE memory is not specified in build')

            if pm_heap_region_size == 0:
                self.formatter.output("No PM heap present on P0")
            else:
                self.formatter.output("Analysing heap pm P0 content")
                pointer_to_first_heap_pm = self.chipdata.get_data(
                    self.heap_pm.address
                )
                self.formatter.output_list(
                    self.
                    _free_blocks_pm(self.chipdata.get_data(self.freelist))
                )
                heap_pm_info = self._alloc_blocks_pm(
                    pointer_to_first_heap_pm, pm_heap_region_size
                )

                self.formatter.output_list(heap_pm_info)

        if self.freelist_p1 is not None:
            try:
                pm_p1_heap_region_size = self.debuginfo.get_constant_strict(
                    '$PM_P1_HEAP_SIZE'
                ).value
            except ct.DebugInfoNoVariable:
                raise KeyError(
                    'Constant $PM_P1_HEAP_SIZE memory is not specified in build')

            if pm_p1_heap_region_size == 0:
                self.formatter.output("No PM heap present on P1")
            else:
                self.formatter.output("Analysing heap pm P1 content")
                pointer_to_first_heap_pm_p1 = self.chipdata.get_data(
                    self.heap_pm_p1.address
                )
                self.formatter.output_list(
                    self.
                    _free_blocks_pm(self.chipdata.get_data(self.freelist_p1))
                )
                heap_pm_p1_info = self._alloc_blocks_pm(
                    pointer_to_first_heap_pm_p1, pm_p1_heap_region_size
                )

                self.formatter.output_list(heap_pm_p1_info)

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

    def _alloc_blocks_pm(self, pointer_to_heap_pm, pm_heap_region_size):

        magic_val = 0xabcd01
        alloc_info = []

        # Get the address we will be working with from the start of heap_pm
        address = pointer_to_heap_pm

        heap_pm_data = self.chipdata.get_data_pm(
            pointer_to_heap_pm, pm_heap_region_size
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
                address = pointer_to_heap_pm + \
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
                        address + length < pointer_to_heap_pm + pm_heap_region_size
                    ):
                    alloc_info.append(
                        "Allocated block size : {0:>4d} at address: {1:0>6x}".
                        format(length, address)
                    )
                    index = index + length / Arch.addr_per_word
                    total = total + length
            except ValueError:
                break

        alloc_info.append("Total heap_pm allocation : {0:>d}".format(total))

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
        except BaseException:
            self.freelist = None

        try:
            self.freelist_p1 = self.debuginfo.get_var_strict(
                'L_freelist_pm_p1'
            ).address
        except BaseException:
            self.freelist_p1 = None

        if self.freelist is not None:
            self.heap_pm = self.debuginfo.get_var_strict('L_heap_pm_adjusted')

        if self.freelist_p1 is not None:
            self.heap_pm_p1 = self.debuginfo.get_var_strict(
                'L_heap_pm_adjusted_p1'
            )
