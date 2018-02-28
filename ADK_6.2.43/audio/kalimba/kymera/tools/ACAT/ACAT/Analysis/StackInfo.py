############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2012 - 2017 Qualcomm Technologies International, Ltd.
#
############################################################################
"""
Module to analyse the stack.
"""
import copy
from collections import OrderedDict
import traceback

from ACAT.Core import CoreTypes as ct
from ACAT.Core import CoreUtils as cu
from . import Analysis
from ..Core import Arch

VARIABLE_DEPENDENCIES = {'strict': ('$error_stack_exception.stack',)}

# Clearly needs to be modified if we ever change the contents of an interrupt
# stack frame. Lowest address first.
INT_FRAME_DESCRIPTORS_HYDRA = [
    'FP',
    'r0',
    'r1',
    'r2',
    'rflags',
    'ARITHMETIC_MODE',
    'MM_RINTLINK',
    'r3',
    'r4',
    'r5',
    'r6',
    'r7',
    'r8',
    'r9',
    'r10',
    'rLink',
    'I0',
    'I1',
    'I2',
    'I3',
    'I4',
    'I5',
    'I6',
    'I7',
    'M0',
    'M1',
    'M2',
    'M3',
    'L0',
    'L1',
    'L4',
    'L5',
    'rMAC2',
    'rMAC1',
    'rMAC0',
    'DoLoopStart',
    'DoLoopEnd',
    'DivResult',
    'DivRemainder',
    'rMACB2',
    'rMACB1',
    'rMACB0',
    'B0',
    'B1',
    'B4',
    'B5',
    'INT_SAVE_INFO'
]
INT_FRAME_DESCRIPTORS_BC = [
    'FP',
    'r0',
    'r1',
    'r2',
    'rflags',
    'ARITHMETIC_MODE',
    'CLOCK_DIVIDE_RATE',
    'MM_RINTLINK',
    'r3',
    'r4',
    'r5',
    'r6',
    'r7',
    'r8',
    'r9',
    'r10',
    'rLink',
    'I0',
    'I1',
    'I2',
    'I3',
    'I4',
    'I5',
    'I6',
    'I7',
    'M0',
    'M1',
    'M2',
    'M3',
    'L0',
    'L1',
    'L4',
    'L5',
    'rMAC2',
    'rMAC1',
    'rMAC0',
    'DoLoopStart',
    'DoLoopEnd',
    'DivResult',
    'DivRemainder',
    'rMACB2',
    'rMACB1',
    'rMACB0',
    'B0',
    'B1',
    'B4',
    'B5',
    'INT_SAVE_INFO'
]

class StackReadFailure(Exception):
    """
    Exception signals stack read failure.
    """
    pass


class StackInfo(Analysis.Analysis):
    """
    @brief This class encapsulates an analysis for a stack backtrace.
    """

    # If you're trying to understand some of the code here it might help to
    # look at http://wiki/AudioCPU/Stack.

    def __init__(self, **kwarg):
        # Call the base class constructor.
        Analysis.Analysis.__init__(self, **kwarg)
        self.interrupt_frames = []
        if Arch.chip_arch == "Hydra":
            self.int_frame_descriptors = INT_FRAME_DESCRIPTORS_HYDRA
        elif Arch.chip_arch == "Bluecore":
            self.int_frame_descriptors = INT_FRAME_DESCRIPTORS_BC
        elif Arch.chip_arch == "KAS":
            self.int_frame_descriptors = INT_FRAME_DESCRIPTORS_BC
        elif Arch.chip_arch == "Napier":
            self.int_frame_descriptors = INT_FRAME_DESCRIPTORS_HYDRA
        else:
            # Other architectures not yet supported
            raise Exception("Chip architecture not yet supported")
        # Use Stack properties read from the build.
        self.use_internal_values = False
        # Give some default values for internal stack properties.
        self.start_of_stack = 0
        self.end_of_stack = 0
        self.preset_frame_pointer = 0
        self.preset_stack_pointer = 0

    def run_all(self):
        """
        @brief Perform all useful analysis and spew the output to the formatter.
        It outputs how much of the stack is used, if SP and FP are valid and
        the stack trace.
        @param[in] self Pointer to the current object
        """
        self.formatter.section_start('Stack')
        try:
            self.analyse_stack_usage()
        except BaseException:
            self.formatter.output(traceback.format_exc())

        try:
            self.analyse_stack_trace(False)
        except BaseException:
            self.formatter.output(traceback.format_exc())

        try:
            self.display_stack_content()
        except BaseException:
            self.formatter.output(traceback.format_exc())

        self.formatter.section_end()

    def stack_overflow_occurred(self):
        """
        @brief Have we experienced a stack overflow? (If so, we will have switched
        to using the exception stack.)
        Returns True/False.
        @param[in] self Pointer to the current object
        """
        # Could work this out in a few ways; easiest is to check
        # STACK_OVERFLOW_PC, which should be set if the hardware is doing its
        # job.
        overflow_pc = self._get_stack_overflow_pc()
        return overflow_pc != 0

    @Analysis.cache_decorator
    def get_stack_frames(self):
        """
        @brief Returns a list of tuples, each containing a stack frame. The oldest
        frame is at the start, and frames are listed with lowest-
        address first - that is, the same way DM normally appears.
        Might raise an AnalysisError exception.
        @param[in] self Pointer to the current object
        """
        if self.stack_overflow_occurred():
            raise ct.AnalysisError(
                "Can't get_stack_frames(): a stack overflow has occurred.")

        start_of_stack = self._get_stack_start()
        end_of_stack = self._get_stack_end()
        stack_pointer = self._get_stack_pointer()
        frame_pointer = self._get_frame_pointer()

        # Repeat the same sanity check we do in analyse_spfp; can't think of a
        # good way to modularise this since we don't want this method to do
        # analysis.
        if stack_pointer >= start_of_stack and stack_pointer <= end_of_stack and \
            frame_pointer >= start_of_stack and frame_pointer <= end_of_stack and \
            start_of_stack < end_of_stack:
            pass
        else:
            raise StackReadFailure(
                "Can't get stack! Probably the subsystem wasn't booted."
            )

        # Ok, if we get here we have no excuse not to at least give it a try.
        # Use _get_stack to save some time on reading the stack once again.
        sp_idx = (stack_pointer - start_of_stack) / Arch.addr_per_word
        the_stack = self._get_stack()[:sp_idx]
        all_stackframes = []

        # The first stackframe is between SP and FP.
        # index into the stack array.
        fp_idx = (frame_pointer - start_of_stack) / Arch.addr_per_word
        # Works because the_stack stops at SP.
        first_frame = the_stack[fp_idx:]
        all_stackframes.insert(0, first_frame)  # Insert at start of list.

        # Search for potential frame pointer on the stack.
        location = 0
        # will contain all the indexes of the potential FPs
        potential_stack_addresses = []
        for val in the_stack:
            if self.is_stack_address(val):
                potential_stack_addresses.append(location)
            location = location + 1
        # Because we traverse the stack in the reverse order we need to reverse
        # the list of potential FP locations too.
        potential_stack_addresses.reverse()

        # Now work backwards through all the other stack frames. This should end when
        # fp_idx is exactly equal to 0.
        safe_guard = 0
        max_loop = 100
        while (fp_idx > 0) and (safe_guard < max_loop):
            start_of_next_frame = (
                the_stack[fp_idx] - start_of_stack
            ) / Arch.addr_per_word
            if start_of_next_frame < 0 or start_of_next_frame > fp_idx:
                self.formatter.alert(
                    "Stack traversal failed! Stack corrupted at address "
                    "0x%08x. The value of the location is 0x%08x which "
                    "cannot be used as a frame pointer(FP)." % (
                        fp_idx * Arch.addr_per_word + start_of_stack,
                        the_stack[fp_idx]
                    )
                )
                potential_fp_found = False
                for next_fp_idx in potential_stack_addresses:
                    if next_fp_idx >= 0 and next_fp_idx < fp_idx:
                        start_of_next_frame = next_fp_idx
                        potential_fp_found = True
                        break
                # there is no reason to continue we run out of options.
                if not potential_fp_found:
                    break
            this_frame = the_stack[start_of_next_frame:fp_idx]
            # Insert at start of list.
            all_stackframes.insert(0, this_frame)
            fp_idx = start_of_next_frame
            # increment the safe guard to avoid infinit loops.
            safe_guard = safe_guard + 1

        if safe_guard == max_loop:
            self.formatter.alert("Exited because of safe_guard")

        # In case of a memory corruption the "while" loop can exit without
        # consuming all the stack. Add the remaining to the stackframes.
        if fp_idx != 0:
            this_frame = the_stack[0:fp_idx]
            all_stackframes.insert(0, this_frame)

        return all_stackframes

    def display_stack_content(self):
        """
        @brief Displays the stack in a programmer friendly format.
        @param[in] self Pointer to the current object
        """
        try:
            # make a copy of the stack frames to avoid modifying it.
            stack_frames = copy.deepcopy(self.get_stack_frames())
            stack_frames.reverse()
        except StackReadFailure:
            return

        # this variable will hold the output text.
        ouput_text = ""

        program_counter = self._get_program_counter()
        try:
            frame_label_name = self.debuginfo.get_source_info(program_counter).module_name
        except ct.BundleMissingError:
            frame_label_name = (
                "No source information for 0x%08X. Bundle is missing." % (program_counter)
            )

        position = self._get_stack_pointer() - self._get_stack_start()
        position = position / Arch.addr_per_word

        position += -1
        for frame in stack_frames:
            ouput_text += "  Stack of: %s \n" % (frame_label_name)

            is_interrupt_frame = frame_label_name == "$M.interrupt.handler"

            # Mark the next frame as unknown until something interesting is found
            frame_label_name = "Unknown function"
            # No need to deep copy as the tuple to list conversion already does
            # that.
            reverse_frame = list(frame)
            reverse_frame.reverse()
            stack_frame = OrderedDict()
            guessed_rerturn_address = None
            for value in reverse_frame:
                address = position * Arch.addr_per_word + self._get_stack_start()
                position += -1

                description = ""
                if self.is_saved_rlink(value):
                    try:
                        label = self.debuginfo.get_source_info(value)
                        label_name = label.module_name
                        # get the file name by removing the path.
                        # \ for win, / for Unix
                        file_name = label.src_file.split("\\")[-1].split("/")[-1]
                        description += "%s %s line: %d" % (
                            label.module_name,
                            file_name,
                            label.line_number
                        )
                    except ct.BundleMissingError:
                        label_name = (
                            "No source information for 0x%08X. " +
                            "Bundle is missing." % (program_counter)
                        )
                        description = label_name

                    # the next frame name will be ..
                    frame_label_name = label_name
                    guessed_rerturn_address = address

                stack_frame[address] = {
                    "value": value,
                    "description": description
                }

            # Signal if the stack was corrupted
            last_address = stack_frame.keys()[-1]
            if not self.is_stack_address(stack_frame[last_address]["value"]):
                stack_frame[last_address]["description"] = (
                    "  - Stack corrupted here!" +
                    stack_frame[last_address]["description"]
                )

            # loop counter
            count = 0
            for address in stack_frame:
                # print "address",address
                ouput_text += (
                    "    0x%08x: 0x%08x" %
                    (address, stack_frame[address]["value"])
                )

                if is_interrupt_frame:
                    # We are in a interrupt frame. Get the name of the saved
                    # register.
                    len_int_frame = len(self.int_frame_descriptors)
                    int_frame_index = len_int_frame - 1 - count
                    if int_frame_index >= 0:
                        register_name = self.int_frame_descriptors[int_frame_index]
                    else:
                        register_name = "too long interrupt frame"
                    ouput_text += " - %16s" % (register_name)

                # Now display the description of the address
                if stack_frame[address]["description"] != "":
                    if address == guessed_rerturn_address:
                        ouput_text += (
                            " - " +
                            stack_frame[address]["description"]
                        )

                    else:
                        ouput_text += (
                            " (potential return address - " +
                            stack_frame[address]["description"] + ")"
                        )

                ouput_text += "\n"
                count += 1

            ouput_text += "\n"

        # Print out the stack content
        self.formatter.section_start('Stack Content')
        self.formatter.output(ouput_text)
        self.formatter.section_end()

    def get_stack_trace(self):
        """
        @brief Returns a list of tuples, each containing (identifier, SourceInfo,
        annotation). 'identifier' in this context is an address which looks
        like it is part of the call stack.
        This function also adds any interrupt frames to self.interrupt_frames.
        @param[in] self Pointer to the current object
        """

        if self.stack_overflow_occurred():
            raise ct.AnalysisError(
                "Can't get_stack_trace(): a stack overflow has occurred.")

        try:
            stack_frames = self.get_stack_frames()
        except StackReadFailure:
            return

        # Reverse the list of stack frames, because we want to analyse
        # the most-recent stuff first.
        stack_frames.reverse()

        # Clear any previously-stored interrupt frames.
        self.interrupt_frames = []

        # Now we need to go through each frame and investigate things that
        # look like code addresses. Most of the time these will be saved
        # values of rLink, but there are some scenarios where that might
        # not be true:
        # * We handled an interrupt (since the frame will also then include
        #   MM_RINTLINK, MM_DOLOOP_* and who knows what else)
        # * Something saved a function pointer onto the stack
        # * For some reason there is a number on the stack which happens to
        #   look like a code address
        #
        # AND FURTHERMORE, there are some scenarios where multiple frames
        # have been lumped together because someone didn't bother to update
        # FP (probably because we passed into assembler code).

        # stack_trace is a list of tuples, containing (identifier, SourceInfo, annotation).
        # This seems a little unnecessary since a SourceInfo has an address
        # in it, but that may not match the one stored on the stack (it's
        # just the closest one we have any information about).
        stack_trace = []

        for frame in stack_frames:
            # INTERRUPT_STORE_STATE_SIZE is 47 on HydraCore, 48 on Bluecore.
            # But we can't just look for frames of exactly that number of words,
            # because an interrupt frame could be followed by some asm frames (which
            # don't update FP).
            if len(frame) < len(self.int_frame_descriptors):
                # Just grab every number that looks like it could be a saved rLink.
                # Skip the first word of the frame because we know this is a saved FP.
                # Reverse the order of frames themselves, because we want to
                # display the most-recent addresses at the top.
                interesting_locations = [
                    a for a in reversed(frame[1:]) if self.is_saved_rlink(a)
                ]
                for i in interesting_locations:
                    stack_trace.append(
                        (i, self.debuginfo.get_source_info(i), None)
                    )
            else:
                actual_interrupt_frame = None

                # Interrupt frames should have code addresses in the
                # saved values of rLink, MM_RINTLINK, DoLoopStart and DoLoopEnd.
                # If FP is stored in the frame, these are at (FP+i), for i in
                # {6, 15, 35, 36}
                interesting_locations = {}
                for idx, val in enumerate(frame):
                    if self.probably_a_code_address(val):
                        interesting_locations[idx] = val

                # This is really quite tricky since we can't guarantee that a) every
                # interesting address will be there (something might have gone wrong),
                # or b) there aren't *extra* interesting addresses due to there
                # being (say) function pointers stored in registers.
                # As a heuristic, let's rely on the saved values of MM_DOLOOP_* and look
                # for two 'interesting' addresses which are next to each other in the
                # stack frame and at least <the offset of DoLoopStart> words from the start.
                # I suppose it's possible that if we've never used the loop hardware
                # (or have executed something horribly from cache) this won't work, but
                # it should be ok for 99% of coredumps.
                # List of tuples containing (index of interesting location,
                # difference to previous)
                diffs = []
                last_i = 0
                for i in sorted(interesting_locations.keys()):
                    diffs.append((i, i - last_i))
                    last_i = i

                doloopstart_offset = self.int_frame_descriptors.index(
                    'DoLoopStart'
                )
                doloopstartpos = None
                for difference in diffs:
                    # difference is a tuple containing (index of interesting
                    # location, difference to previous).
                    # We want these to be (<at least doloopstart_offset>, 1),
                    # as per the explanation above.
                    if difference[0] >= self.int_frame_descriptors.index(
                            'DoLoopStart') and difference[1] == 1:
                        maybe_doloopstartpos = difference[0] - 1

                        # If this really is the location of doloopstart, we can sanity-check by
                        # validating the values of saved-rLink and MM_RINTLINK
                        relative_rlink_pos = doloopstart_offset - \
                            self.int_frame_descriptors.index('rLink')
                        relative_rintlink_pos = doloopstart_offset - \
                            self.int_frame_descriptors.index('MM_RINTLINK')

                        # guard the frame reads with this boolean value.
                        check_rlink = (
                            len(frame) >
                            (maybe_doloopstartpos - relative_rlink_pos)
                        )
                        check_rintlink = (
                            len(frame) >
                            (maybe_doloopstartpos - relative_rintlink_pos)
                        )

                        if (not check_rlink) and (not check_rintlink):
                            # no possible doloop left because doloopstart_offset is based
                            # on the index of interesting location which only
                            # increases
                            break

                        # Check if it is a saved rlink and if it is probably a code address.
                        # check_rlink and check_rintlink will avoid triggering
                        # IndexError
                        is_saved_rlink = (
                            check_rlink and self.is_saved_rlink(
                                frame[maybe_doloopstartpos -
                                      relative_rlink_pos]
                            )
                        )
                        prob_rintlink = (
                            check_rintlink and self.probably_a_code_address(
                                frame[maybe_doloopstartpos -
                                      relative_rintlink_pos]
                            )
                        )

                        if frame[maybe_doloopstartpos
                                 + 1] <= frame[maybe_doloopstartpos]:
                            # Probably wrong, doLoopEnd should be > doLoopStart
                            continue

                        if is_saved_rlink and prob_rintlink:
                            # Surely we have the right thing
                            doloopstartpos = maybe_doloopstartpos
                            break
                        elif is_saved_rlink or prob_rintlink:
                            # We *might* have the right thing.
                            # Set doloopstartpos but go round the loop again.
                            # If we find something that looks like a better
                            # match, we'll set doloopstartpos again.
                            doloopstartpos = maybe_doloopstartpos
                        else:
                            # Nope, try again. Maybe we found something that
                            # just really looks a bit like an interrupt frame.
                            continue

                if doloopstartpos is not None:
                    # DoLoopStart is doloopstart_offset words from the start of
                    # the interrupt frame.
                    start_of_int_frame = (doloopstartpos - doloopstart_offset)
                    end_of_int_frame = (
                        doloopstartpos - doloopstart_offset +
                        len(self.int_frame_descriptors)
                    )
                    if self.is_stack_address(frame[start_of_int_frame]):
                        actual_interrupt_frame = frame[start_of_int_frame:
                                                       end_of_int_frame]

                # Finally, add information to our stack trace.
                # We don't do a very detailed analysis here: that can be achieved by
                # calling analyse_interrupt_stack_frames().
                if actual_interrupt_frame is not None:
                    self.interrupt_frames.append(actual_interrupt_frame)
                    saved_rintlink = actual_interrupt_frame[
                        self.int_frame_descriptors.index('MM_RINTLINK')
                    ]
                    # rintlink could point to anywhere (not just a call instruction)
                    # so calling is_saved_rlink() won't work.
                    annotation = "Interrupt frame: saved rintlink"
                    try:
                        stack_trace.append(
                            (
                                saved_rintlink,
                                self.debuginfo.get_source_info(saved_rintlink),
                                annotation
                            )
                        )
                    except ct.InvalidPmAddress:
                        annotation = "Interrupt frame: saved rintlink (not a valid code address!)"
                        stack_trace.append((saved_rintlink, None, annotation))

                    # Now add the saved rLink. Do this after
                    # rintlink (remember, we're going back in time and
                    # the interrupt clearly occurred more recently).
                    saved_rlink = actual_interrupt_frame[
                        self.int_frame_descriptors.index('rLink')
                    ]
                    annotation = "Interrupt frame: saved rlink"
                    try:
                        stack_trace.append(
                            (
                                saved_rlink,
                                self.debuginfo.get_source_info(saved_rlink),
                                annotation
                            )
                        )
                    except ct.InvalidPmAddress:
                        annotation = "Interrupt frame: saved rlink (not a valid code address!)"
                        stack_trace.append((saved_rlink, None, annotation))
                else:
                    # Couldn't identify the frame contents properly, so revert
                    # to dumb behaviour.
                    annotation = "Confusing asm code or interrupt frame"
                    for i in interesting_locations.itervalues():
                        if self.is_saved_rlink(i):
                            stack_trace.append(
                                (
                                    i, self.debuginfo.get_source_info(i),
                                    annotation
                                )
                            )
        # Don't forget PC and RLINK - add these at the start.
        program_counter = self._get_program_counter()
        rlink = self._get_rlink()
        pc_info = self.debuginfo.get_source_info(program_counter)
        rlink_info = self.debuginfo.get_source_info(rlink)
        stack_trace.insert(0, ('RLINK', rlink_info, None))
        stack_trace.insert(0, ('PC', pc_info, None))

        return stack_trace

    def is_stack_address(self, address):
        """
        @brief Takes an address and tries to work out whether or not it points to
        a location on the stack.
        Returns True or False.
        @param[in] self Pointer to the current object
        @param[in] address
        """
        start_of_stack = self._get_stack_start()
        end_of_stack = self._get_stack_end()

        return address >= start_of_stack and address <= end_of_stack

    def probably_a_code_address(self, address):
        """
        @brief Takes an address from the stack and tries to work out whether or not
        it is the address of an instruction. This is a more lax check than
        is_saved_rlink, since we can still be fooled by the wrong kind of
        DM address.
        @param[in] self Pointer to the current object
        @param[in] address
        """
        # Need to look up the encoding type so that we know whether or not
        # to search for address or address-1.
        try:
            pm_encoding = self.debuginfo.get_pm_encoding(address)
        except ct.BundleMissingError:
            # The address is in a missing bundle. So, it is likely we are in
            # code address
            self.formatter.alert(
                "Bundle missing probably_a_code_address returned True"
            )
            return True
        except ct.InvalidPmAddress:
            # the address is not in the PM, therefore it is not a code address.
            return False

        if pm_encoding == 0:
            # Address wasn't even in static PM.
            return False
        elif pm_encoding == 1:
            # Maxim
            pass
        else:
            # Minim
            address = address - 1

        try:
            _ = self.debuginfo.get_instruction(address)
        except ct.InvalidPmAddress:
            return False
        except ct.BundleMissingError:
            # The address is in a missing bundle. So, it is likely we are in
            # code address
            self.formatter.alert(
                "Bundle missing probably_a_code_address returned True"
            )
            return True

        return True

    def is_saved_rlink(self, address):
        """
        @brief Takes an address from the stack and tries to work out
        whether or not it is a saved value of rLink.
        @param[in] self Pointer to the current object
        @param[in] address
        """
        if Arch.get_pm_region(address, False) is None:
            # Clearly not in PM
            return False

        # We need to look up the instruction *before* the supplied address
        # and work out whether it is a call statement.
        # There are some existing functions that work out whether or not
        # an instruction is a call, squirreled away in
        # //depot/devtools/main/kalimba/kalaccess/src/insn_set_maxim.cpp and
        # insn_set_minim.cpp.
        def _maxim_is_call_instruction(instr):
            # First 7 bits must be 1110 000 - ie opcode=call, type=a/b
            return instr & 0xfe000000 == 0xe0000000

        def _minim_is_call_instruction(instr, is_prefixed):
            if is_prefixed:
                # Prefixed Type B Call, Prefixed Type A Call in Insert32
                return ((instr &
                         0xF0E0) == 0xE020) or ((instr & 0xEFF0) == 0xCE10)
            # Unprefixed Type B Call, Unprefixed Type A Call
            return ((instr &
                     0xFE00) == 0x4E00) or ((instr & 0xFFF8) == 0x4CD0)

        def _minim_is_prefix_instruction(instr, is_prefixed):
            if is_prefixed:
                # 32-bit prefix in an Insert32
                return ((instr &
                         0xf000) == 0xf000) or ((instr & 0xfff0) == 0xcfd0)
            return (instr & 0xf000) == 0xf000

        # No idea how this would work if we had any dynamic code in PM RAM,
        # since there's no way to tell whether that is Minim or Maxim.
        # For now though, we only have to deal with static code (i.e. code
        # compiled along with everything else that happens to be hoisted into
        # RAM) - and debuginfo can tell us how that is encoded.
        try:
            pm_encoding = self.debuginfo.get_pm_encoding(address)
        except ct.BundleMissingError:
            # The address is in a missing bundle. So, it is likely we are in
            # code address but lets assume is not a return one.
            self.formatter.alert(
                "Bundle missing is_saved_rlink returned False"
            )
            return False

        if pm_encoding == 0:
            # Address wasn't even in static PM.
            return False
        elif pm_encoding == 1:
            # Maxim.
            # First, check that the address supplied is actually a code
            # address.
            try:
                _ = self.debuginfo.get_instruction(address)
            except ct.InvalidPmAddress:
                return False
            except ct.BundleMissingError:
                # The address is in a missing bundle. So, it is likely we are in
                # code address but lets assume is not a return one.
                self.formatter.alert(
                    "Bundle missing is_saved_rlink returned False"
                )
                return False

            # Now look for the previous Maxim instruction.
            # KAL_ARCH_3 DSPs have word-addressed PM (i.e. previous
            # instruction is at address-1), but others are octet-addressed.
            if Arch.kal_arch == 3:
                prev_address = address - 1
            else:
                # Sanity-check that the address supplied is divisible
                # by 4 (otherwise it's clearly not the address of an
                # instruction).
                if address % 4 != 0:
                    return False
                prev_address = address - 4

            # Now look for the previous Maxim instruction.
            try:
                instr_n1 = self.debuginfo.get_instruction(prev_address)
            except ct.InvalidPmAddress:
                return False
            except ct.BundleMissingError:
                # The address is in a missing bundle. So, it is likely we are in
                # code address but lets assume is not a return one.
                self.formatter.alert(
                    "Bundle missing is_saved_rlink returned False"
                )
                return False

            return _maxim_is_call_instruction(instr_n1)
        else:
            # Minim.
            # This is really quite tricky.
            # First, sanity-check that the address supplied is odd. (A saved
            # minim address will have the LSbit set). Then clear said bit,
            # so we can look up the actual instruction referred-to.
            if address & 1:
                address = address - 1
            else:
                return False

            # Now, we need to work out whether the instruction at (n-1) has a
            # prefix; need to know that before we can determine whether it is
            # a call instruction. The only way to do that is to look at (n-2).
            # Except THAT might have a prefix too, so we also need to look at
            # (n-3) as well. Thankfully Kalimba only permits a maximum of two
            # prefixes so we can stop there.
            # Any chip that supports Minim will also have octet-addressed PM,
            # so at least we don't have to make this even more complicated.
            try:
                # Previous Minim instruction; suspected 'call'.
                instr_n1 = self.debuginfo.get_instruction(address - 2)
                # n-2
                instr_n2 = self.debuginfo.get_instruction(address - 4)
                # n-3
                instr_n3 = self.debuginfo.get_instruction(address - 6)
            except ct.BundleMissingError:
                # The address is in a missing bundle. So, it is likely we are in
                # code address but lets assume is not a return one.
                self.formatter.alert(
                    "Bundle missing is_saved_rlink returned False"
                )
                return False
            except BaseException:
                # If we hit any errors in the above, it's probably because
                # (address - n) isn't actually a valid PM address.
                return False

            # Hopefully clear from the comment above.
            double_prefix = _minim_is_prefix_instruction(instr_n3, False)
            if _minim_is_prefix_instruction(instr_n2, double_prefix):
                # Test-with-prefix
                return _minim_is_call_instruction(instr_n1, True)

            # No prefix
            return _minim_is_call_instruction(instr_n1, False)

    #######################################################################
    # Analysis methods - public since we may want to call them individually
    #######################################################################

    def analyse_stack_usage(self):
        """
        @brief How much stack have we used? Are SP and FP valid?
        @param[in] self Pointer to the current object
        """
        # Re-use our 'where is the stack' code.
        the_stack = self._get_stack()
        start_of_stack = self._get_stack_start()
        end_of_stack = self._get_stack_end()
        the_stack_size = end_of_stack - start_of_stack

        stack_pointer = self._get_stack_pointer()
        frame_pointer = self._get_frame_pointer()

        if stack_pointer >= start_of_stack and stack_pointer <= end_of_stack and \
            frame_pointer >= start_of_stack and frame_pointer <= end_of_stack and \
            start_of_stack < end_of_stack:
            pass
        else:
            self.formatter.alert(
                "Incorrect stack settings. Probably the subsystem wasn't booted."
            )
        self.formatter.output(
            'Stack runs from 0x%x - 0x%x' % (start_of_stack, end_of_stack)
        )
        self.formatter.output('SP = 0x%x, FP = 0x%x' % (stack_pointer, frame_pointer))

        if stack_pointer >= start_of_stack and stack_pointer <= end_of_stack:
            pass
        else:
            self.formatter.alert('SP does not point to a stack address!')

        if frame_pointer >= start_of_stack and frame_pointer <= end_of_stack:
            pass
        else:
            self.formatter.alert('FP does not point to a stack address!')

        # We normally expect SP to be greater than FP
        if stack_pointer < frame_pointer:
            self.formatter.alert('SP is not greater than FP!')

        # Work out how much stack we have left.
        self.formatter.output(
            '%d words of stack free' %
            ((end_of_stack - stack_pointer) / Arch.addr_per_word)
        )

        # Work out our peak stack usage. Do this simply by counting the number
        # of words at the end of the stack which have never been set (where
        # 'never been set' means 'are zero'; not 100% accurate if you're
        # going to go around putting a bunch of zeroes on the stack).
        i = 0
        for i in xrange(
                the_stack_size / Arch.addr_per_word - Arch.addr_per_word,
                - Arch.addr_per_word, -Arch.addr_per_word
            ):
            if the_stack[i] != 0:
                break
        min_stack_space = the_stack_size - i
        self.formatter.output(
            '%d words minimum stack space' %
            (min_stack_space / Arch.addr_per_word)
        )

    def analyse_stack_trace(self, detailed=False):
        """
        @brief Go through all stack frames and try to work out a coherent stack trace.
        'detailed' is a flag telling us whether to splurge all the gritty
        details about each location. Most of the time this is overkill.
        @param[in] self Pointer to the current object
        @param[in] detailed = False
        """
        # Check that we haven't overflowed (overflown?) - no point
        # in attempting a stack backtrace if that's the case.
        if self.stack_overflow_occurred():
            self._analyse_overflow_stack()
            return

        # Ok we can get the good stuff.
        try:
            stack_frames = self.get_stack_frames()
        except StackReadFailure:
            return

        # Reverse the list of stack frames, because we want to display/analyse
        # the most-recent stuff first.
        stack_frames.reverse()

        self.formatter.section_start('Stack frames')
        self.formatter.output_list(stack_frames)
        self.formatter.section_end()

        # Clear any previously-stored interrupt frames.
        stack_trace = self.get_stack_trace()

        # Simple trace - just output the address and module
        self.formatter.section_start('Simple stack backtrace')
        self.formatter.output(
            'Note that this is a GUESS. Assembler code can cause confusion.'
        )
        for label in stack_trace:
            info = cu.hex(label[0])
            if label[1] is not None:
                info += ' - ' + label[1].module_name
            if label[2] is not None:
                info += ' - ' + label[2]
            self.formatter.output(info)
        self.formatter.section_end()

        # Detailed trace - inspect the interrupt stack frames
        if detailed:
            self.analyse_interrupt_stack_frames()

        # Detailed trace - output the whole SourceInfo.
        if detailed:
            self.formatter.section_start('Detailed stack backtrace')
            self.formatter.output(
                'Note that this is a GUESS. Assembler code can cause confusion.'
            )
            for label in stack_trace:
                self.formatter.output(cu.hex(label[0]))
                if label[2] is not None:
                    self.formatter.output(label[2])
                if label[1] is not None:
                    self.formatter.output(label[1])
            self.formatter.section_end()

    def analyse_interrupt_stack_frames(self):
        """
        @brief Prints out information about any interrupt frames on the stack.
        @param[in] self Pointer to the current object
        """
        if self.stack_overflow_occurred():
            raise ct.AnalysisError(
                "Can't analyse_interrupt_stack_frames(): a stack overflow has occurred.")

        # Do a stack trace in order to refresh our memory
        # about what's on the stack. This will set self.interrupt_frames.
        self.get_stack_trace()

        for frame in self.interrupt_frames:
            self.formatter.section_start('Interrupt stack frame')
            isf = []  # list of strings
            for idx, entry in enumerate(frame):
                # Reverse the order again
                isf.insert(
                    0, (
                        self.int_frame_descriptors[idx].ljust(15) + " - " +
                        cu.hex(entry)
                    )
                )
            self.formatter.output_list(isf)

            # Sanity check the frame.
            # Check that rintlink is valid.
            frame_saved_rintlink = frame[6]
            # rintlink could point to anywhere (not just a call instruction)
            # so calling is_saved_rlink() won't work.
            try:
                self.debuginfo.get_source_info(frame_saved_rintlink)
            except ct.InvalidPmAddress:
                self.formatter.alert(
                    'Saved rintlink in interrupt frame does not point to a '
                    'code address - we may have been executing from cache!'
                )
            self.formatter.section_end()

    def change_stack_properties(
            self, start_of_stack, end_of_stack, frame_pointer, stack_pointer
        ):
        """
        @brief Displays an alert once per cache clear (per ACAT command).
        @param[in] self Pointer to the current object
        @param[in] start_of_stack Address where the stack starts.
        @param[in] end_of_stack Address where the stack starts.
        @param[in] frame_pointer Address where the frame pointer points to.
        @param[in] stack_pointer Address where the stack pointer points to.
        """
        if (start_of_stack >=
                end_of_stack) or (frame_pointer >= stack_pointer):
            self.formatter.error("Invalid configuration!")
            return

        self.start_of_stack = start_of_stack
        self.end_of_stack = end_of_stack
        self.preset_frame_pointer = frame_pointer
        self.preset_stack_pointer = stack_pointer
        # From now onwards use the internally stored stack properties.
        self.use_internal_values = True

    #######################################################################
    # Private methods - don't call these externally.
    #######################################################################

    def _display_alert(self, alert):
        """
        @brief Displays an alert once per cache clear (per ACAT command).
        @param[in] self Pointer to the current object
        """
        # The cache is used to make sure the alert is only displayed once per
        # call.
        if not alert + "alert_displayed" in self.cache:
            self.cache[alert + "alert_displayed"] = True
            # The cache will be cleared automatically
            self.formatter.alert(alert)

    @Analysis.cache_decorator
    def _get_stack_pointer(self):
        """
        @brief Reads the stack pointer (SP) register of the chip.
        @param[in] self Pointer to the current object
        """
        if self.use_internal_values:
            return self.preset_stack_pointer

        stack_pointer = self.chipdata.get_reg_strict('REGFILE_SP').value
        if not self.is_stack_address(stack_pointer):
            sp_temp = self._get_stack_end()
            self._display_alert(
                "SP 0x%08x is not a valid stack address! "
                "0x%08x will be used instead." % (stack_pointer, sp_temp)
            )
            stack_pointer = sp_temp

        return stack_pointer

    @Analysis.cache_decorator
    def _get_frame_pointer(self):
        """
        @brief Reads the frame pointer (FP) register of the chip.
        @param[in] self Pointer to the current object
        """
        if self.use_internal_values:
            return self.preset_frame_pointer

        frame_pointer = self.chipdata.get_reg_strict('REGFILE_FP').value
        if not self.is_stack_address(frame_pointer):
            # FP should be the at least one word less than the SP
            fp_temp = self._get_stack_pointer() - Arch.addr_per_word
            self._display_alert(
                "FP 0x%08x is not a valid stack address! "
                "0x%08x will be used instead." % (frame_pointer, fp_temp)
            )
            frame_pointer = fp_temp
        return frame_pointer

    @Analysis.cache_decorator
    def _get_program_counter(self):
        """
        @brief Reads the program counter (PC) register of the chip.
        @param[in] self Pointer to the current object
        """
        program_counter = self.chipdata.get_reg_strict('REGFILE_PC').value
        if Arch.get_pm_region(program_counter, False) is None:
            self._display_alert(
                "PC 0x%08x is not a valid program memory address! "
                "0x%08x will be used instead." % (program_counter, 0)
            )
            program_counter = 0
        return program_counter

    @Analysis.cache_decorator
    def _get_rlink(self):
        """
        @brief Reads the rlink register of the chip.
        @param[in] self Pointer to the current object
        """
        rlink = self.chipdata.get_reg_strict('REGFILE_RLINK').value
        if Arch.get_pm_region(rlink, False) is None:
            self._display_alert(
                "rlink 0x%08x is not a valid program memory address! "
                "0x%08x will be used instead." % (rlink, 0)
            )
            rlink = 0
        return rlink

    @Analysis.cache_decorator
    def _get_stack_start(self):
        """
        @brief Reads the start address of the stack.
        @param[in] self Pointer to the current object
        """
        if self.use_internal_values:
            return self.start_of_stack

        start_of_stack = self.chipdata.get_reg_strict(
            '$STACK_START_ADDR'
        ).value
        return start_of_stack

    @Analysis.cache_decorator
    def _get_stack_end(self):
        """
        @brief Reads the end address of the stack.
        @param[in] self Pointer to the current object
        """
        if self.use_internal_values:
            return self.end_of_stack

        end_of_stack = self.chipdata.get_reg_strict('$STACK_END_ADDR').value
        return end_of_stack

    @Analysis.cache_decorator
    def _get_stack(self):
        """
        @brief Get hold of the entire stack.
        Returns a Variable encapsulating the entire stack buffer (use
        get_stack_frames if you want it split into frames).
        Might raise an AnalysisError exception, if something is suspicious.
        @param[in] self Pointer to the current object
        """
        if self.chipdata.is_volatile():
            self._display_alert(
                "Warning: connected to live chip -- The stack can be corrupted."
            )
        start_of_stack = self._get_stack_start()
        end_of_stack = self._get_stack_end()
        # It is enough to supply the address of the stack
        # (which is the value of the STACK_START_ADDR
        # register) to get the stack.buffer variable.
        # This is useful because for P1 it has a different
        # name (stack.p1_buffer).
        the_stack = self.chipdata.get_data(
            start_of_stack, (end_of_stack - start_of_stack)
        )

        # Load the necessary register to ACAT's cache to make sure that
        # the their read is very close to the actual stack read.
        self._get_stack_pointer()
        self._get_frame_pointer()
        self._get_program_counter()
        self._get_rlink()
        # self._get_stack_start() is already called above
        self._get_stack_end()
        self._get_stack_overflow_pc()
        self._get_exception_stack()

        return the_stack

    @Analysis.cache_decorator
    def _get_stack_overflow_pc(self):
        """
        @brief Reads the stack overflow pc.
        @param[in] self Pointer to the current object
        """
        overflow_pc = self.chipdata.get_reg_strict('$STACK_OVERFLOW_PC').value
        if Arch.get_pm_region(overflow_pc, False) is None:
            self._display_alert(
                "Overflow PC 0x%08x is not a valid program memory address! "
                "0x%08x will be used instead." % (overflow_pc, 0)
            )
            overflow_pc = 0
        return overflow_pc

    @Analysis.cache_decorator
    def _get_exception_stack(self):
        """
        @brief Get hold of the exception stack.
        Returns a Variable encapsulating the exception stack.
        @param[in] self Pointer to the current object
        """
        # We return a Variable here since it might be interesting to know
        # where the exception stack is and how big it is.
        return self.chipdata.get_var_strict('$error_stack_exception.stack')

    def _analyse_overflow_stack(self):
        """
        @brief Method to display a stack overflow.
        @param[in] self Pointer to the current object
        """
        if self.chipdata.is_volatile():
            self._display_alert(
                "Warning: connected to live chip -- The stack can be corrupted."
            )
        self.formatter.alert('Stack overflow has occurred!')

        # Try to work out when we overflowed. STACK_OVERFLOW_PC
        # should be set.
        overflow_pc = self._get_stack_overflow_pc()

        try:
            # a SourceInfo object
            culprit = self.debuginfo.get_source_info(overflow_pc)
            self.formatter.output('Overflow occurred in:')
            self.formatter.output(culprit)
        except ct.InvalidPmAddress:
            # overflow_pc.value was not a code address. Give up.
            self.formatter.output(
                'STACK_OVERFLOW_PC is 0x%08x - no help there! ' % overflow_pc
            )

        # Spew out the exception stack
        exception_stack = self._get_exception_stack()
        self.formatter.output(
            'Exception stack starts at 0x%x and comprises:' %
            exception_stack.address
        )
        self.formatter.output(exception_stack)
