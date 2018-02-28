############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2014 - 2017 Qualcomm Technologies International, Ltd.
#
############################################################################
"""
Module responsible to analyse the heap memory in Kymera.
"""
from ACAT.Core import CoreTypes as ct
from ACAT.Analysis import Analysis
from ACAT.Core import Arch
# import ACAT.Analysis.DebugLog as DebugLog


from ACAT.Analysis import DebugLog
from ACAT.Core import CoreUtils as cu



# 'heap_config':() is empty because members are not necessarily accessed,
# 'mem_node' also has members 'line' and 'file' missing since they are in try
VARIABLE_DEPENDENCIES = {
    'strict': (
        '$_processor_heap_info_list', 'L_pheap_info', '$_heap_debug_free',
        '$_heap_debug_min_free'
    )
}
TYPE_DEPENDENCIES = {
    'heap_config': (),
    'heap_info': ()
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
        self.pmalloc_debug_enabled = None
        self.heap_info = None
        self.freelist = None

        try:
            self.debuginfo.get_var_strict("$_processor_heap_info_list")
        except ct.DebugInfoNoVariable:
            # fallback to the old implementation
            raise ct.OutdatedFwAnalysisError()

    def run_all(self):
        """
        @brief Perform all useful analysis and spew the output to the formatter.
        Displays the heap memory usage and the pmalloc debug info.
        @param[in] self Pointer to the current object
        """
        # Look up the debug information.
        self._lookup_debuginfo()

        self.formatter.section_start('Heap Memory Info')

        for heap_num in range(self.max_num_heaps):
            if self.freelist[heap_num].value != 0:
                current_heap = self.heap_info[heap_num]
                heap_size = current_heap.get_member("heap_size").value
                heap_start = current_heap.get_member("heap_start").value

                self.formatter.output(self.heap_names[heap_num] + " enabled")
                self.formatter.output_list(
                    self._free_blocks(self.freelist[heap_num].value)
                )
                (heapinfo, debuginfo) = self._alloc_blocks(
                    heap_start, heap_size
                )

                self.formatter.output_list(heapinfo)

                if self.pmalloc_debug_enabled:
                    self.formatter.section_start('Debug info')
                    self.formatter.output_list(debuginfo)
                    self.formatter.section_end()

        self.display_configuration()

        self.formatter.section_start('Heap memory usage')
        self.get_watermarks()
        self.formatter.section_end()

        self.formatter.section_end()

    def display_configuration(self):
        """
        @brief Prints out the heap configuration for both processors.
        @param[in] self Pointer to the current object
        """
        self.formatter.section_start('Heap Configuration')

        # processor_heap_info_list should be always different than NULL!
        heap_info_list = self.chipdata.get_var_strict(
            "$_processor_heap_info_list"
        )
        num_heap_processors = len(heap_info_list.members)

        for pnum in range(num_heap_processors):
            self.formatter.section_start('Processor %d' % pnum)
            proc_heap_info = heap_info_list[pnum].get_member("heap")

            self.formatter.output(
                self._get_heap_config_str(proc_heap_info)
            )
            self.formatter.section_end()
        self.formatter.section_end()

    @DebugLog.suspend_log_decorator(0)
    def ret_get_watermarks(self):
        """
        @brief Same as get_watermarks, but it will return values rather than
        print outs.
        @param[in] self Pointer to the current object
        @param[out] List with the heap usage + pools statistic.
        """
        # Look up the debug information.
        self._lookup_debuginfo()

        total_heap = 0
        free_heap = self.chipdata.get_var_strict("$_heap_debug_free").value
        min_free_heap = self.chipdata.get_var_strict(
            "$_heap_debug_min_free"
        ).value

        for heap_num in range(self.max_num_heaps):
            if self.freelist[heap_num].value != 0:
                current_heap = self.heap_info[heap_num]
                heap_size = current_heap.get_member("heap_size").value
                total_heap += heap_size

        return total_heap, free_heap, min_free_heap

    def get_watermarks(self):
        """
        @brief Returns the minimum available memory for the total, the free
        and the minimum free memories.
        @param[in] self Pointer to the current object
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
        @brief Clears the minimum available memory watermark by equating it
        with the current available memory.
        @param[in] self Pointer to the current object
        """
        # Look up the debug information.
        self._lookup_debuginfo()

        free_heap = self.chipdata.get_var_strict("$_heap_debug_free").value
        # Wash down the watermark (min available =  current available)
        self.chipdata.set_data(
            self.chipdata.get_var_strict("$_heap_debug_min_free").address,
            [free_heap]
        )

    ##################################################
    # Private methods
    ##################################################

    def _get_heap_config_str(self, proc_config):
        """
        @brief Returns the string representation of the heap configuration .
            In other words is the to string fonction for processor
            configuration.
        @param[in] self Pointer to the current object
        @param[in] proc_config Pointer to the processor configuration.
        @param[out] processor configuration string.
        """
        return_str = ""
        for heap_num in range(self.max_num_heaps):
            heap_end_address = proc_config[heap_num].get_member(
                'heap_end'
            ).value - 1
            if heap_end_address == -1:
                # This heap is disabled in this build
                heap_end_address = 0

            return_str += "%16s : %5.1f KiB (0x%05x bytes) at 0x%08x - 0x%08x\n" % (
                self.heap_names[heap_num],
                proc_config[heap_num].get_member('heap_size').value / 1024.0,
                proc_config[heap_num].get_member('heap_size').value,
                proc_config[heap_num].get_member('heap_start').value,
                heap_end_address
            )
        return return_str

    def _free_blocks(self, address):
        """
        @brief Checks the free blocks.
        @param[in] self Pointer to the current object
        @param[in] address Address to start with.
        @param[out] Returns a list describing the free memory allocations.
        """
        free_info = []
        address_history = []
        while address != 0:
            # Avoid infinite loop by checking if the node was already checked.
            if address not in address_history:
                address_history.append(address)
            else:
                self.formatter.error(
                    "Repeating nodes with address 0x%x. Probably memory corruption" %
                    address)
                return free_info
            freeblock = self.chipdata.cast(address, 'mem_node')
            free_info.append(
                "Free block size : {0:>d} at address: {1:0>6x}".
                format(freeblock.get_member('length').value, address)
            )
            address = freeblock.get_member('u').get_member('next').value
        return free_info

    def _alloc_blocks(self, heap_address, heap_size):
        """
        @brief Reads and checks the allocated blocks.
        @param[in] self Pointer to the current object
        @param[in] heap_address The heap start address
        @param[in] heap_size The heap size.
        @param[out] Returns two lists. One for the ... The other for ...
        """
        magic_val = 0xabcd01
        alloc_info = []
        debug_info = []

        address = heap_address
        heap_data = self.chipdata.get_data(heap_address, heap_size)
        total = 0
        index = -Arch.addr_per_word
        # calculate the offset of the magic field
        testblock = self.chipdata.cast(address, 'mem_node')
        testblock_magic = testblock.get_member('u').get_member('magic')
        testblock_address = testblock_magic.address
        magic_offset = testblock_address - address

        # Search through the entire heap block, looking for allocated blocks
        # based on the presence of the magic word
        while True:
            try:
                index = index + Arch.addr_per_word * \
                    (heap_data[index / Arch.addr_per_word +
                               1:].index(magic_val) + 1)
                address = heap_address + index - magic_offset
                testblock = self.chipdata.cast(address, 'mem_node')
                magic = testblock.get_member('u').get_member('magic').value
                if magic != magic_val:
                    raise ct.AnalysisError(
                        "Magic word not found at expected offset.")

                length = testblock.get_member('length').value
                if (length > 0) and (address + length < heap_address + heap_size):
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
                            try:
                                owner = self.debuginfo.get_source_info(
                                    file_address)
                                owner_hint = (
                                    owner.src_file + ', near line ' +
                                    str(owner.line_number)
                                )
                            except ct.BundleMissingError:
                                owner_hint = (
                                    "No source information." +
                                    "Bundle is missing."
                                )
                            debug_info.append(
                                "Ptr: {0:0>6x} size: {1:>3d} allocated by: "
                                "{2:s}".format(
                                    address, length,
                                    owner_hint
                                )
                            )
                        else:
                            try:
                                filename = self.debuginfo.read_const_string(
                                    file_address
                                )
                            except KeyError:
                                filename = "Filename cannot be read!"
                            except ct.BundleMissingError:
                                filename = (
                                    "Filename cannot be read! " +
                                    "Bundle is missing."
                                )
                            debug_info.append(
                                "Ptr: {0:0>6x} size: {1:>3d} allocated by: "
                                "{2:s}, line {3:d}".format(
                                    address, length, filename, line
                                )
                            )
            except ValueError:
                break

        alloc_info.append("Total heap allocation : {0:>d}".format(total))

        return alloc_info, debug_info

    def _lookup_debuginfo(self):
        """
        @brief Queries debuginfo for information we will need to get the heap
        memory usage.
        @param[in] self Pointer to the current object
        """

        if not self._do_debuginfo_lookup:
            return

        self._do_debuginfo_lookup = False

        # Freelist
        self.freelist = self.chipdata.get_var_strict('L_freelist')

        # Check for PMALLOC_DEBUG
        # If L_memory_pool_limits exists then PMALLOC_DEBUG is enabled
        try:
            self.debuginfo.get_var_strict(
                'L_memory_pool_limits'
            )
            self.pmalloc_debug_enabled = True
        except ct.DebugInfoNoVariable:
            self.pmalloc_debug_enabled = False

        pheap_info = self.chipdata.get_var_strict("L_pheap_info").value
        heap_info = self.chipdata.cast(pheap_info, "heap_config")
        self.heap_info = heap_info.get_member("heap")
