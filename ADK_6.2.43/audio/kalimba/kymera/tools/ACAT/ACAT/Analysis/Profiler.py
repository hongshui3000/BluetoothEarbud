############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2015 - 2017 Qualcomm Technologies International, Ltd.
#
############################################################################
"""
Module used to profiler Kymera.
"""
import threading
from collections import Counter
import operator
import time as import_time
import math
import Queue

from ACAT.Analysis import Analysis
from ACAT.Analysis import DebugLog
from ACAT.Core import CoreTypes as ct
from ACAT.Core import Arch
from ACAT.Core import ChipData

VARIABLE_DEPENDENCIES = {
    'not_strict': (
        '$_current_id', '$_profiler_list', '$interrupt.cpu_fraction',
        '$interrupt.cpu_fraction_with_task'
    )
}
TYPE_DEPENDENCIES = {'profiler': ('next', 'id', 'cpu_fraction')}


def output_percentage(
        output_function,
        occurrence,
        total_measurements=100,
        key_transform_function=None,
        sort=True,
        column=56
    ):
    """
    @brief Helper function to display a dictionary or a list of occurrences.
    @param[in] output_function
    @param[in] occurrence
    @param[in] total_measurements = 100
    @param[in] key_transform_function = None
    @param[in] sort = True
    @param[in] column Which terminal column to left-justify the formatted percentage to
    """
    if key_transform_function is None:
        def _dummy(input_param):
            """
            Dummy function.
            """
            return input_param
        key_transform_function = _dummy

    # outputs the percentage
    if sort:
        sorted_occurrence = sorted(
            occurrence.items(), key=operator.itemgetter(1)
        )
        sorted_occurrence.reverse()
    else:
        sorted_occurrence = occurrence
    for element in sorted_occurrence:
        output_string = "%-*s " % (
            column - 1, key_transform_function(element[0])
        )
        if isinstance(element[1], tuple):
            for values in element[1]:
                output_string += "%7.3f%% " % (
                    float(values * 100) / total_measurements
                )
        else:
            output_string += "%7.3f%%" % (
                float(element[1] * 100) / total_measurements
            )
        output_function(output_string)


class RunClksReader(threading.Thread, Analysis.Analysis):
    """
    @brief This class is responsible for reading the run clocks counter repeatedly (every
    1 s) until a stop event arrives. After each reading it checks if an overflow occurred
    and corrects the total clks counter accordingly.
    """

    def __init__(self, measure_event, **kwarg):
        # Call the base class constructors.
        threading.Thread.__init__(self)
        Analysis.Analysis.__init__(self, **kwarg)
        self._measure_event = measure_event
        try:
            # check if we are in a 32 bit machine
            self.chipdata.get_reg_strict("$NUM_RUN_CLKS")
            self.registes_size = 32
            self.registes_shift = 0
        except KeyError:
            # check if we are in a 24 bit machine
            # if this fails maybe we have an other platform to worry about
            self.chipdata.get_reg_strict("$NUM_RUN_CLKS_MS")
            self.registes_size = 8
            self.registes_shift = 24

        self._mips_usage = 0
        self._mips_percent = 0
        self._chip_cpu_speed_mhz = 0

    def run_all(self):
        """
        @brief This analysis does not implement a run_all function because it
        is a helper module .
        @param[in] self Pointer to the current object
        """
        # Nothing we can usefully do here.
        return

    def run(self):
        """
        @brief This task calculates the total mips consumption using $NUM_RUN_CLKS_MS. It reads
        the register value and re-reads it after the given time. Calculates the mips
        consumption using the following formula:
                 delta * 2^register_shift
         MIPS = --------------
                  10^6   (assuming 1 clk is 1 instruction)

        @param[in] self Pointer to the current object
        """
        overflow_value = int(math.pow(2, self.registes_size))
        total_overflow = 0

        # wait until we the measure event is set
        self._measure_event.wait()

        # read the initial value of the run clks and the start time.
        start_time = import_time.time()
        if Arch.addr_per_word != 4:
            start_clk = self.chipdata.get_reg_strict("$NUM_RUN_CLKS_MS").value
        else:
            start_clk = self.chipdata.get_reg_strict("$NUM_RUN_CLKS").value
        prev_clk = start_clk
        cur_clk = start_clk

        while self._measure_event.is_set():
            import_time.sleep(1)
            if Arch.addr_per_word != 4:
                cur_clk = self.chipdata.get_reg_strict(
                    "$NUM_RUN_CLKS_MS"
                ).value
            else:
                cur_clk = self.chipdata.get_reg_strict("$NUM_RUN_CLKS").value
            if cur_clk < prev_clk:
                self.formatter.output("Overflow detected")
                total_overflow += overflow_value
            prev_clk = cur_clk

        # read end time
        end_time = import_time.time()
        delta_time = (end_time - start_time)
        # print "delta_time (should be close to the given value)= ", delta_time

        # calculate the mips usage
        delta_clk = cur_clk + total_overflow - start_clk
        # print "delta_clk = ",  delta_clk

        mips_usage = (
            float(delta_clk) * math.pow(2, self.registes_shift)
        ) / (delta_time * math.pow(10, 6))
        try:
            chip_cpu_speed_mhz = self.chipdata.get_var_strict(
                "$profiler.cpu_speed_mhz"
            ).value
            if chip_cpu_speed_mhz == 0:
                # invalid value. Raise exception to handle the error and
                # print error message.
                raise ct.DebugInfoNoVariable()
        except ct.DebugInfoNoVariable:
            self.formatter.output(
                "CPU speed is not set or invalid. "
                "(On Hydra platforms this is set by the Curator. "
                "Please see SERVICE_ADVICE for CCP_FACTS_ABOUT_SUBSYSTEM IE.)\n"
                "CPU speed is an assumption!")
            chip_cpu_speed_mhz = Arch.chip_cpu_speed_mhz

        mips_percent = (mips_usage * 100) / chip_cpu_speed_mhz
        self.formatter.output(
            "Total MIPS used = %3.3f (CPU active for ~%6.3f%% @%dMHz)" %
            (mips_usage, mips_percent, chip_cpu_speed_mhz)
        )

        self._mips_usage = mips_usage
        self._mips_percent = mips_percent
        self._chip_cpu_speed_mhz = chip_cpu_speed_mhz


class Profiler(Analysis.Analysis):
    """
    @brief This class encapsulates analyses for measuring MIPS.
    """

    def __init__(self, **kwarg):
        # Call the base class constructor.
        Analysis.Analysis.__init__(self, **kwarg)
        try:
            self._profiler_list_address = self.debuginfo.get_var_strict(
                '$_profiler_list'
            ).address
        except ct.DebugInfoNoVariable:
            self._profiler_list_address = None

        self.old_formatter = None

    def run_all(self, run_pc_clks=False):
        """
        @brief Perform all useful analysis and spew the output to the
            formatter. If the default  values for the different analyses
            are not suitable for you consider calling them directly.

        run_kymera_builtin() is one of them. It reads the built
            in Kymera profiler (if it exists) and displays it in a more
            useful way.

        run_clks() calculates the total mips consumption using
            $NUM_RUN_CLKS_MS. Please see RunClksReader for more details.
            Only runs if run_pc_clks input is True.

        run_pc() runs the PC profiling analysis for a given time. Only runs
            if run_pc_clks input is True.

        @param[in] self Pointer to the current object
        @param[in] run_pc_clks If set to True PC and RUN_CLKS profiler will
            called.
        """
        self.formatter.section_start('Profiler')
        self.run_kymera_builtin()
        if run_pc_clks:  # for live chips
            self.run_clks(10)
            self.run_pc(10, False)
        self.formatter.section_end()

    @DebugLog.suspend_log_decorator(0)
    def run_pc(self, time=10, task_id_read=False):
        """
        @brief This reads the program counter for a period of time.
        After reading the Program Counter numerous times it checks which is the nearest
        code label to those PC values. Finally it puts in order the occurrences of the
        different code labels.

        It can also read the current task id to compare the active times of the different
        tasks.
        @param[in] self Pointer to the current object
        @param[in] time = 10
        @param[in] task_id_read = False
        """
        queue = Queue.Queue()
        _stop_event = threading.Event()

        if task_id_read:
            reader = ChipData.VariablePoller(
                _stop_event, ['$PC_STATUS', '$_current_id'], self.chipdata,
                queue
            )
        else:
            reader = ChipData.VariablePoller(
                _stop_event, ['$PC_STATUS'], self.chipdata, queue
            )

        reader.start()
        self.formatter.output("\nPC profiling started for %d sec." % time)
        import_time.sleep(time)
        _stop_event.set()
        # it will take a almost the same time to analyse the collected data
        reader.join()

        pc_data = queue.get()
        count = len(pc_data)
        self.formatter.output("Total measurements = %d" % count)

        if task_id_read:
            pc_occurrence = Counter([row[0] for row in pc_data])
            task_id_occurrence = Counter([row[1] for row in pc_data])
        else:
            pc_occurrence = Counter(pc_data)

        module_occurrence = {}
        for address in pc_occurrence:
            try:
                module_name = self.debuginfo.get_source_info(
                    address
                ).module_name
            except ct.BundleMissingError:
                module_name = (
                    "No source information for 0x%08x. " +
                    "Bundle is missing." % (address)
                )
            module_name = module_name.replace("L_", "")
            if module_occurrence.get(module_name) is None:
                module_occurrence[module_name] = pc_occurrence[address]
            else:
                module_occurrence[module_name] += pc_occurrence[address]

        output_percentage(self.formatter.output, module_occurrence, count)

        if task_id_read:
            self.formatter.output("\nTask ID")
            output_percentage(
                self.formatter.output,
                task_id_occurrence,
                count,
                key_transform_function=lambda x: "0x%x" %
                self.chipdata.get_data(x) if x != 0 else "None")

    @DebugLog.suspend_log_decorator(0)
    def run_clks(self, time):
        """
        @brief Calculates the total mips consumption using $NUM_RUN_CLKS_MS. Please see
        RunClksReader for more details.
        @param[in] self Pointer to the current object
        @param[in] time
        """

        self.formatter.output(
            "\nRun clocks profiling started for %d sec." % time
        )

        # While the measure event is set RunClksReader is profiling.
        measure_event = threading.Event()
        measure_event.clear()

        kwargs = {
            "chipdata": self.chipdata,
            "debuginfo": self.debuginfo,
            "formatter":  self.formatter,
            "interpreter": self.interpreter
        }

        reader = RunClksReader(
            measure_event, **kwargs
        )
        reader.start()
        measure_event.set()
        import_time.sleep(time)
        measure_event.clear()

        # wait for the task to finish
        reader.join()

    @DebugLog.suspend_log_decorator(0)
    def ret_run_clks(self, time):
        """
        @brief Calculates the total mips consumption using $NUM_RUN_CLKS_MS. Please see
        RunClksReader for more details.
        @param[in] self Pointer to the current object
        @param[in] time
        """

        self.formatter.output(
            "\nRun clocks profiling started for %d sec." % time
        )

        # While the measure event is set RunClksReader is profiling.
        measure_event = threading.Event()
        measure_event.clear()

        kwargs = {
            "chipdata": self.chipdata,
            "debuginfo": self.debuginfo,
            "formatter":  self.formatter,
            "interpreter": self.interpreter
        }

        reader = RunClksReader(
            measure_event, **kwargs
        )
        reader.start()
        measure_event.set()
        import_time.sleep(time)
        measure_event.clear()

        # wait for the task to finish
        reader.join()

        mips_usage = getattr(reader, '_mips_usage', None)
        mips_percent = getattr(reader, '_mips_percent', None)
        chip_cpu_speed_mhz = getattr(reader, '_chip_cpu_speed_mhz', None)
        return mips_usage, mips_percent, chip_cpu_speed_mhz

    # The built in profiler is not accurate while the debuglog is enabled.
    # The profiler updates its reading every 1.024 second.
    # Sleeping for 1.024 second is needed to refresh Kymera builtin profiler..
    @DebugLog.suspend_log_decorator(1.024)
    def run_kymera_builtin(self, detailed_debug=False):
        """
        @brief Reads the built in Kymera profiler (if it exists) and
        display it in a more useful way.
        @param[in] self Pointer to the current object
        @param[in] detailed_debug=False
        """

        if self._profiler_list_address is None:
            self.formatter.output("Profiling is not enabled for this build.")
            return

        profiler_head_ptr = self.chipdata.get_var_strict(
            self._profiler_list_address
        ).value

        active = None
        sleep = None

        cpu_usage_overview = []
        cpu_usage = {}
        total_user_defined = 0

        # For debugging the builtin profiler the logs are saved.
        if detailed_debug:

            def builtin_profiler_log(string):
                """
                Function which stores the detailed debugging of the profiler.
                """
                builtin_profiler_log.debug_string += string

            builtin_profiler_log.debug_string = "\nDetailed:\n"
        else:

            def builtin_profiler_log(_):
                """
                Dummy function to ignore detailed debugging of the profiler.
                """
                pass

        # Check if the profiler list is not empty
        if profiler_head_ptr != 0:
            # read the first element of the list and parse the profiler list
            profiler_list = self.chipdata.cast(profiler_head_ptr, "profiler")

            for profiler_entry in self.parse_linked_list(
                    profiler_list, 'next'
                ):
                # read the name of the profiler
                profiler_name = "%s 0x%x" % (
                    self.debuginfo.get_kymera_debuginfo().debug_strings.
                    get(profiler_entry.get_member("name").value),
                    profiler_entry.get_member("id").value
                )

                if profiler_name == "None 0x0":  # assembly style mips measurement
                    # get the original name form debuginfo
                    profiler_name = self.debuginfo.get_var_strict(
                        profiler_entry.address
                    ).name.replace("L_", "")
                    builtin_profiler_log(
                        "assembly style mips measure entry: \"%s 0x%x\"," %
                        (profiler_name, profiler_entry.address)
                    )
                else:
                    builtin_profiler_log(
                        "C style mips measure entry: \"%s\"," %
                        (profiler_name)
                    )

                # read the cpu usage of the profiler and convert it to percent.
                # Originally the cpu usage is in thousands.
                cpu_usage_val = float(
                    profiler_entry.get_member("cpu_fraction").value
                ) / 10

                try:
                    stat_kick_period = float(
                        profiler_entry.get_member("kick_total").value
                    )
                    if stat_kick_period > 0:
                        stat_kick_period = 1024.0 / stat_kick_period
                    else:
                        stat_kick_period = 0.0
                except BaseException:
                    stat_kick_period = 0.0

                try:
                    stat_kick_ratio = float(
                        profiler_entry.get_member("count").value
                    )
                    if stat_kick_ratio > 0 and stat_kick_period > 0:
                        stat_kick_ratio = 102400.0 / \
                            (stat_kick_ratio * stat_kick_period)
                    else:
                        stat_kick_ratio = 0.0
                except BaseException:
                    stat_kick_ratio = 0.0

                try:
                    peak_cpu_usage = float(
                        profiler_entry.get_member("peak_cpu_fraction").value
                    ) / 10
                except BaseException:
                    peak_cpu_usage = None
                # Check if the profiler is not a special one. Sleep time and
                # scheduler loop is considered special. They give the user information
                # about the system.
                if profiler_name == "$_sleep_time or $profiler.sleep_time":
                    cpu_usage_overview.insert(0, ("sleep", cpu_usage_val))
                    sleep = cpu_usage_val
                elif profiler_name == "sched_loop":
                    cpu_usage_overview.append(("sched_loop", cpu_usage_val))
                    active = cpu_usage_val
                else:
                    # This is a user defined or a Kymera profiler. Kymera profiler
                    # is used to measure the cpu usage of an operator. The User defined
                    # can be any profiling used for debug purposes.
                    total_user_defined += cpu_usage_val
                    if profiler_name.startswith("Operator 0x"):
                        opid = int(profiler_name[9:], 16)
                        try:
                            opmgr = self.interpreter.get_analysis(
                                "opmgr", self.chipdata.processor
                            )
                            cid_name = opmgr.get_operator(opid).cap_data.name
                        except BaseException:
                            cid_name = "unknown capability"
                        profiler_name += ' (' + cid_name + ')'
                    cpu_usage[profiler_name] = (
                        cpu_usage_val, peak_cpu_usage, stat_kick_period,
                        stat_kick_ratio
                    )
                # log the cpu usage to the detailed debug string.
                builtin_profiler_log(
                    "\nused %6.3f%% of the cpu.\n" % (cpu_usage_val) +
                    str(profiler_entry) + "\n"
                )
        else:
            builtin_profiler_log("the profiler list empty")

        if active is None or sleep is None:
            # shallow sleep was disabled
            cpu_usage_overview.insert(0, ("sleep not detected", 0))
            # using array because it will be modified after the interrupt time
            # is read.
            cpu_usage_overview.append(["sched_loop", 100])

        try:
            # Analyse the interrupt profiler
            interrupt_cpu_fraction = float(
                self.chipdata.get_var_strict('$interrupt.cpu_fraction').value
            )
            # cpu_usage["interrupt handler"] = interrupt_cpu_fraction/10
            cpu_usage_overview.append(
                (
                    "    interrupt handler (cannot be nested)",
                    interrupt_cpu_fraction / 10
                )
            )

            # read all the nested interrupts
            level = 0
            total_percent = 0
            # Interrupt with task per nested level.
            for intr_nested_level in self.chipdata.get_var_strict(
                    '$interrupt.cpu_fraction_with_task').value:
                percent = float(intr_nested_level) / 10
                total_percent += percent
                cpu_usage_overview.append(
                    (
                        "    interrupt with task at nested level %d" % level,
                        percent
                    )
                )
                level += 1

            cpu_usage_overview.insert(
                2, (
                    "total interrupt = int. handler + int. triggered task",
                    total_percent
                )
            )

            if active is None or sleep is None:
                # correct the scheduler loop value
                active = 100 - total_percent
                cpu_usage_overview[1][1] = active
                # Because the sleep was disabled the sleep percentage is zero.
                sleep = 0

            cpu_usage_overview.append(
                (
                    "sleep + active + int = %d Remains " %
                    (total_percent + sleep + active),
                    100 - (total_percent + sleep + active)
                )
            )

        except ct.DebugInfoNoVariable:
            # Interrupt profiler is off
            pass

        self.formatter.output("Overview:")
        output_percentage(
            self.formatter.output, cpu_usage_overview, sort=False
        )
        if cpu_usage == {}:
            self.formatter.output(
                "\nNo kymera or user defined measurements available. "
            )
        else:
            self.formatter.output("\nKymera and user defined measurements:")
            self.formatter.output(
                " " * 42 + "current   peak    KickPd    KickEffic"
            )

            # outputs the percentage
            sorted_occurrence = sorted(
                cpu_usage.items(), key=operator.itemgetter(1)
            )
            sorted_occurrence.reverse()

            total_measurements = 100
            for element in sorted_occurrence:
                output_string = "%-40s " % (element[0])
                values = element[1]
                output_string += "%7.3f%% " % (
                    float(values[0] * 100) / total_measurements
                )
                if values[1] is None:
                    output_string += "         "
                else:
                    output_string += "%7.3f%% " % (
                        float(values[1] * 100) / total_measurements
                    )
                output_string += "%6.3fms " % (float(values[2]))
                output_string += "%8.3f%% " % (float(values[3]))
                self.formatter.output(output_string)

            self.formatter.output("_" * 58)
            output_percentage(
                self.formatter.output,
                [("total user defined", total_user_defined)],
                sort=False,
                column=41
            )

        if detailed_debug:
            self.formatter.output(builtin_profiler_log.debug_string)

    def clear_peak(self):
        """
        @brief Clears the peak cpu usage for all the built in Kymera profilers.
        @param[in] self Pointer to the current object
        """
        if self._profiler_list_address is None:
            self.formatter.output("Profiling is not enabled for this build.")
            return

        profiler_head_ptr = self.chipdata.get_var_strict(
            self._profiler_list_address
        ).value

        # Check if the profiler list is not empty
        if profiler_head_ptr != 0:
            # read the first element of the list and parse the profiler list
            profiler_list = self.chipdata.cast(profiler_head_ptr, "profiler")

            for profiler_entry in self.parse_linked_list(
                    profiler_list, 'next'
                ):
                # clear the peak cpu usage.
                self.chipdata.set_data(
                    profiler_entry.get_member("peak_cpu_fraction").address,
                    [0]
                )
