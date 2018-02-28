############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2014 - 2017 Qualcomm Technologies International, Ltd.
#
############################################################################
"""
Heap memory related classes. Implementation before the heap config change.
"""
from ACAT.Analysis import Analysis
from ACAT.Core import CoreTypes as ct
from ACAT.Core import CoreUtils as cu
from ACAT.Core import Arch
from ACAT.Analysis import DebugLog

# 'heap_config':() is empty because members are not necessarily accessed,
# 'mem_node' also has members 'line' and 'file' missing since they are in try
VARIABLE_DEPENDENCIES = {
    'strict': ('$_heap_debug_free', '$_heap_debug_min_free'),
    'not_strict': (
        '$_heap1_max_size', '$_heap4_max_size', '$_heap2_max_size',
        '$_heap3_max_size', 'L_pheap_info', '$_heap_info_list', '$_heap1',
        '$_heap2', 'L_freelist1', 'L_freelist2', 'L_freelist3', 'L_freelist4',
        'L_memory_pool_limits'
    )
}
TYPE_DEPENDENCIES = {
    'heap_config': (),
    'mem_node': ('length', 'u', 'u.next', 'u.magic')
}


class HeapMem(Analysis.Analysis):
    """
    @brief This class encapsulates an analysis for heap memory usage.
    """
    # heap names
    heap_names = ["DM1 heap", "DM2 heap", "DM2 shared heap", "DM1 ext heap"]
    # maximum number of heaps per processor.
    max_num_heaps = 4

    def __init__(self, **kwarg):
        # Call the base class constructor.
        Analysis.Analysis.__init__(self, **kwarg)
        # Look up the debuginfo once. Don't do it here though; we don't want
        # to throw an exception from the constructor if something goes
        # wrong.
        self._do_debuginfo_lookup = True

        self.freelist1 = None
        self.freelist2 = None
        self.freelist3 = None
        self.freelist4 = None
        self.heap1 = None
        self.heap2 = None
        self.heap3 = None
        self.heap4 = None
        self.pmalloc_debug_enabled = None

    def run_all(self):
        """
        @brief Perform all useful analysis and spew the output to the formatter.
        Displays the heap memory usage and the pmalloc debug info.
        @param[in] self Pointer to the current object
        """
        # Look up the debug information, unless we already have.
        if self._do_debuginfo_lookup:
            self._lookup_debuginfo()
            self._do_debuginfo_lookup = False

        self.formatter.section_start('Heap Memory Info')

        if self.freelist1 is not None:
            try:
                heap1_size = self.chipdata.get_var_strict(
                    '$_heap1_max_size'
                ).value
            except BaseException:
                heap1_size = self.heap1.size
            self.formatter.output("DM1 heap enabled")
            self.formatter.output_list(
                self._free_blocks(self.chipdata.get_data(self.freelist1))
            )
            (heap1info,
             debug1info) = self._alloc_blocks(self.heap1.address, heap1_size)

            self.formatter.output_list(heap1info)

        if self.freelist4 is not None:
            heap4_max_size = self.chipdata.get_var_strict(
                '$_heap4_max_size'
            ).value
            self.formatter.output("DM1 fast heap enabled")
            self.formatter.output_list(
                self._free_blocks(self.chipdata.get_data(self.freelist4))
            )
            (heap4info, debug4info) = self._alloc_blocks(
                self.heap4.address, heap4_max_size
            )

            self.formatter.output_list(heap4info)

        if self.freelist2 is not None:
            try:
                heap2_size = self.chipdata.get_var_strict(
                    '$_heap2_max_size'
                ).value
            except BaseException:
                heap2_size = self.heap2.size
            self.formatter.output("DM2 heap enabled")
            self.formatter.output_list(
                self._free_blocks(self.chipdata.get_data(self.freelist2))
            )
            (heap2info,
             debug2info) = self._alloc_blocks(self.heap2.address, heap2_size)

            self.formatter.output_list(heap2info)

        if self.freelist3 is not None:
            heap3_max_size = self.chipdata.get_var_strict(
                '$_heap3_max_size'
            ).value
            self.formatter.output("DM2 shared heap enabled")
            self.formatter.output_list(
                self._free_blocks(self.chipdata.get_data(self.freelist3))
            )
            (heap3info, debug3info) = self._alloc_blocks(
                self.heap3.address, heap3_max_size
            )

            self.formatter.output_list(heap3info)

        try:
            heap_info_list = self.chipdata.get_var_strict("$_heap_info_list")
            if heap_info_list is not None:
                num_heap_info_list = len(heap_info_list.members)
                self.formatter.section_start('Heap Configuration')
                for pnum in xrange(0, num_heap_info_list):
                    self.formatter.section_start('P%d' % pnum)

                    if self.freelist1 is not None:
                        self.formatter.output(
                            "%16s : %5x (%5.1f KiB) at address: %06x" % (
                                "DM1 heap",
                                heap_info_list.members[pnum].get_member(
                                    'heap1'
                                ).get_member('heap_size').value, (
                                    heap_info_list.members[pnum].get_member(
                                        'heap1'
                                    ).get_member('heap_size').value / 1024.0
                                ), heap_info_list.members[pnum].get_member(
                                    'heap1'
                                ).get_member('heap_start').value
                            )
                        )

                    if self.freelist2 is not None:
                        self.formatter.output(
                            "%16s : %5x (%5.1f KiB) at address: %06x" % (
                                "DM2 heap",
                                heap_info_list.members[pnum].get_member(
                                    'heap2'
                                ).get_member('heap_size').value, (
                                    heap_info_list.members[pnum].get_member(
                                        'heap2'
                                    ).get_member('heap_size').value / 1024.0
                                ), heap_info_list.members[pnum].get_member(
                                    'heap2'
                                ).get_member('heap_start').value
                            )
                        )

                    if self.freelist3 is not None:
                        self.formatter.output(
                            "%16s : %5x (%5.1f KiB) at address: %06x" % (
                                "DM2 shared heap",
                                heap_info_list.members[pnum].get_member(
                                    'heap3'
                                ).get_member('heap_size').value, (
                                    heap_info_list.members[pnum].get_member(
                                        'heap3'
                                    ).get_member('heap_size').value / 1024.0
                                ), heap_info_list.members[pnum].get_member(
                                    'heap3'
                                ).get_member('heap_start').value
                            )
                        )

                    if self.freelist4 is not None:
                        self.formatter.output(
                            "%16s : %5x (%5.1f KiB) at address: %06x" % (
                                "DM1 fast heap",
                                heap_info_list.members[pnum].get_member(
                                    'heap4'
                                ).get_member('heap_size').value, (
                                    heap_info_list.members[pnum].get_member(
                                        'heap4'
                                    ).get_member('heap_size').value / 1024.0
                                ), heap_info_list.members[pnum].get_member(
                                    'heap4'
                                ).get_member('heap_start').value
                            )
                        )

                    self.formatter.section_end()
                self.formatter.section_end()
        except BaseException:
            pass

        self.formatter.section_start('Debug info')
        self.formatter.output(
            "pmalloc_debug_enabled = " + str(self.pmalloc_debug_enabled)
        )

        if self.freelist1 is not None:
            self.formatter.output_list(debug1info)

        if self.freelist4 is not None:
            self.formatter.output_list(debug4info)

        if self.freelist2 is not None:
            self.formatter.output_list(debug2info)

        if self.freelist3 is not None:
            self.formatter.output_list(debug3info)

        self.formatter.section_end()

        self.formatter.section_start('Heap memory usage')
        self.get_watermarks()
        self.formatter.section_end()

        self.formatter.section_end()

    @DebugLog.suspend_log_decorator(0)
    def ret_get_watermarks(self):
        """
        @brief Returns the minimum available memory for the total, the free
        and the minimum free memories.
        @param[in] self Pointer to the current object
        """
        pheap_info = None
        heap_config = None
        # Look up the debug information, unless we already have.
        if self._do_debuginfo_lookup:
            self._lookup_debuginfo()
            self._do_debuginfo_lookup = False

        total_heap = 0
        try:
            pheap_info = self.chipdata.get_var_strict("L_pheap_info").value
            heap_config = self.chipdata.cast(pheap_info, 'heap_config')
        except ct.DebugInfoNoVariable:
            pass

        free_heap = self.chipdata.get_var_strict("$_heap_debug_free").value
        min_free_heap = self.chipdata.get_var_strict(
            "$_heap_debug_min_free"
        ).value

        if self.freelist1 is not None:
            if pheap_info is not None:
                total_dm1 = heap_config.get_member('heap1').get_member(
                    'heap_size'
                ).value
            else:
                total_dm1 = self.debuginfo.get_var_strict("$_heap1").size
            total_heap += total_dm1

        if self.freelist4 is not None:
            total_dm1_fast = heap_config.get_member('heap4').get_member(
                'heap_size'
            ).value
            total_heap += total_dm1_fast

        if self.freelist2 is not None:
            if pheap_info is not None:
                total_dm2 = heap_config.get_member('heap2').get_member(
                    'heap_size'
                ).value
            else:
                total_dm2 = self.debuginfo.get_var_strict("$_heap2").size
            total_heap += total_dm2

        if self.freelist3 is not None:
            total_dm2_shared = heap_config.get_member('heap3').get_member(
                'heap_size'
            ).value
            total_heap += total_dm2_shared

        return total_heap, free_heap, min_free_heap

    def get_watermarks(self):
        """
        @brief Same as get_watermarks, but it will return values rather than
        print outs.
        @param[in] self Pointer to the current object
        @param[out] List with the heap usage + pools statistic.
        """
        total_heap, free_heap, min_free_heap = self.ret_get_watermarks()

        output_str = ""
        output_str += (
            "entire heap memory, total size,   " +
            cu.mem_size_to_string(total_heap) +
            "\n"
        )
        output_str += (
            "entire heap memory, current free, " +
            cu.mem_size_to_string(free_heap) +
            "\n"
        )
        output_str += (
            "entire heap memory, minimum free, " +
            cu.mem_size_to_string(min_free_heap) +
            "\n"
        )
        # use output_raw to keep the spaces.
        self.formatter.output_raw(output_str)

    @DebugLog.suspend_log_decorator(0)
    def clear_watermarks(self):
        """
        @brief Clears the minimum available memory watermark by equating it with the current
        available memory.
        @param[in] self Pointer to the current object
        """
        # Look up the debug information, unless we already have.
        if self._do_debuginfo_lookup:
            self._lookup_debuginfo()
            self._do_debuginfo_lookup = False

        free_heap = self.chipdata.get_var_strict("$_heap_debug_free").value
        # Wash down the watermark (min available =  current available)
        self.chipdata.set_data(
            self.chipdata.get_var_strict("$_heap_debug_min_free").address,
            [free_heap]
        )

    ##################################################
    # Private methods
    ##################################################

    def _free_blocks(self, address):

        free_info = []
        while address != 0:
            freeblock = self.chipdata.cast(address, 'mem_node')
            free_info.append(
                "Free block size : {0:>d} at address: {1:0>6x}".
                format(freeblock.get_member('length').value, address)
            )
            address = freeblock.get_member('u').get_member('next').value
        return free_info

    def _alloc_blocks(self, heap_address, heap_size):

        magic_val = 0xabcd01
        alloc_info = []
        debug_info = []

        address = heap_address
        heap_data = self.chipdata.get_data(heap_address, heap_size)
        total = 0
        index = -Arch.addr_per_word
        # calculate the offset of the magic field
        testblock = self.chipdata.cast(address, 'mem_node')
        magic_var = testblock.get_member('u').get_member('magic')
        testblock_address = magic_var.address
        magic_offset = testblock_address - address

        # Search through the entire heap block, looking for allocated blocks
        # based on the presence of the magic word
        while True:
            try:
                index = index + Arch.addr_per_word * \
                    (heap_data[index / Arch.addr_per_word + 1:].index(magic_val) + 1)
                address = heap_address + index - magic_offset
                testblock = self.chipdata.cast(address, 'mem_node')
                magic = testblock.get_member('u').get_member('magic').value
                if magic != magic_val:
                    raise ct.AnalysisError(
                        "Magic word not found at expected offset."
                    )

                length = testblock.get_member('length').value
                if (length > 0) and \
                        (address + length < heap_address + heap_size):
                    alloc_info.append(
                        "Allocated block size : {0:>4d} at address: {1:0>6x}".
                        format(length, address)
                    )
                    index = index + length
                    total = total + length
                    if self.pmalloc_debug_enabled:
                        file_address = testblock.get_member('file').value
                        line = testblock.get_member('line').value
                        if line == 0:
                            # 'file' is actually the value of rlink when we called malloc.
                            # We can look that up to work out roughly where the allocation
                            # took place.
                            owner = self.debuginfo.get_source_info(
                                file_address)
                            owner_hint = owner.src_file + \
                                ', near line ' + str(owner.line_number)
                            debug_info.append(
                                "Ptr: {0:0>6x} size: {1:>3d} allocated by: {2:s}". format(
                                    address, length, owner_hint))
                        else:
                            filename = self.debuginfo.read_const_string(
                                file_address)
                            debug_info.append(
                                "Ptr: {0:0>6x} size: {1:>3d} allocated by: {2:s}, line {3:d}". format(
                                    address, length, filename, line))
            except ValueError:
                break

        alloc_info.append("Total heap allocation : {0:>d}".format(total))

        return alloc_info, debug_info

    def _lookup_debuginfo(self):
        """
        @brief Queries debuginfo for information we will need to get the heap memory usage
        @param[in] self Pointer to the current object
        """

        # Freelist

        try:
            self.freelist1 = self.debuginfo.get_var_strict(
                'L_freelist1'
            ).address
        except BaseException:
            self.freelist1 = None

        try:
            self.freelist2 = self.debuginfo.get_var_strict(
                'L_freelist2'
            ).address
        except BaseException:
            self.freelist2 = None

        try:
            self.freelist3 = self.debuginfo.get_var_strict(
                'L_freelist3'
            ).address
        except BaseException:
            self.freelist3 = None

        try:
            self.freelist4 = self.debuginfo.get_var_strict(
                'L_freelist4'
            ).address
        except BaseException:
            self.freelist4 = None

        if self.freelist1 is not None:
            self.heap1 = self.debuginfo.get_var_strict('$_heap1')

        if self.freelist2 is not None:
            self.heap2 = self.debuginfo.get_var_strict('$_heap2')

        if self.freelist3 is not None:
            self.heap3 = self.debuginfo.get_var_strict('$_heap3')

        if self.freelist4 is not None:
            self.heap4 = self.debuginfo.get_var_strict('$_heap4')

        # Check for PMALLOC_DEBUG
        # If L_memory_pool_limits exists then PMALLOC_DEBUG is enabled
        try:
            self.debuginfo.get_var_strict('L_memory_pool_limits')
            self.pmalloc_debug_enabled = True
        except ct.DebugInfoNoVariable:
            self.pmalloc_debug_enabled = False
