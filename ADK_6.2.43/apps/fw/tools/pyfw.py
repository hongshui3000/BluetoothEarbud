############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2014 - 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
############################################################################
import os
import sys

if __name__ == '__main__':
    my_dir = os.path.abspath(os.path.dirname(__file__))    
    pylib_dir = os.path.join(my_dir, "pylib")
    if pylib_dir not in sys.path:
        sys.path.insert(0, pylib_dir)
        
    del my_dir
    del pylib_dir
    
import csr
from csr.dev.env.standalone_env import StandaloneFirmwareInfoEnv
from csr.front_end.base_front_end import BaseFrontEnd as FrontEnd

class PyfwFrontEnd(FrontEnd):
    
    @staticmethod
    def _add_cmd_line_options(parser):
        pass
    
    @staticmethod
    def main(options, interactive, log, shell=globals()):
        
        builds = options.firmware_builds
        build_types = builds.split(",")
        for b in build_types:
            build = b.split(":",1)
            if len(build) != 2:
                raise ValueError("Bad syntax in fw option: missing ':' "
                                 "(%s)" % build)
            
            build_type, build_dir = build
        
            log.major("Creating firmware environment for '%s'" % build_type)
            shell["%s_env" % build_type] = \
                        StandaloneFirmwareInfoEnv.create(build_dir, build_type)
        return True
# ----------------------------------------------------------------------------
# Entry
# ----------------------------------------------------------------------------

if __name__ == '__main__':
    
    PyfwFrontEnd.main_wrapper(shell=globals())
