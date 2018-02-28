############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2012 - 2017 Qualcomm Technologies International, Ltd.
#
############################################################################
"""
Madule which check for patches.
"""
from ACAT.Core import CoreTypes as ct
from . import Analysis

VARIABLE_DEPENDENCIES = {
    'not_strict': ('$_patched_fw_version',),
    'old_style': ('$_patch_level',)
}


class Patches(Analysis.Analysis):
    """
    @brief This class encapsulates an analysis for patches
    """

    def __init__(self, **kwarg):
        # Call the base class constructor.
        Analysis.Analysis.__init__(self, **kwarg)

    def run_all(self):
        """
        @brief Perform all useful analysis and spew the output to the formatter.
        It outputs the patch level of the firmware.
        @param[in] self Pointer to the current object
        """
        self.formatter.section_start('Patches')
        self.analyse_patch_level()
        self.formatter.section_end()

        # TODO - slurp out the patch data from PM RAM and analyse it
        # TODO - could look at the $ROM_PATCHXXX registers?

    def get_patch_level(self):
        """
        @brief Returns the patch level of the firmware.
        @param[in] self Pointer to the current object
        """
        # Old versions of code used a different variable to modern ones.
        patch_level = 0

        try:
            # Old-style
            patch_level = self.chipdata.get_var_strict('$_patch_level').value
        except ct.DebugInfoNoVariable:
            # Do nothing
            pass

        try:
            # New-style
            patch_level = self.chipdata.get_var_strict('$_patched_fw_version').value
        except ct.DebugInfoNoVariable:
            # Do nothing in this case as well
            pass

        return patch_level

    #######################################################################
    # Analysis methods - public since we may want to call them individually
    #######################################################################

    def analyse_patch_level(self):
        """
        @brief Prints the patch level.
        @param[in] self Pointer to the current object
        """
        self.formatter.output('Patch Level: ' + str(self.get_patch_level()))
