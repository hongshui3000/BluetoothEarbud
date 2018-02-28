# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
'''
Script to autogenerate hydra module log header during the Apps firmware build from a 
supplied list of module headers. 
Module name is the last directory in module path.
'''

import sys
import os
from optparse import OptionParser

if __name__ == "__main__":
    
    usage = "sched_autogen.py: -m MACRO -o OUTPUT <module path list>"
    parser = OptionParser(usage=usage);
    parser.add_option("-o", "--output", help="Name of output file", 
                      type="string", dest="outfile")
    parser.add_option("-m", "--macro", help="Base name of macro to be "
                                            "generated, e.g. SCHED_TASK",
                      type="string", dest="macro")
    
    options, args = parser.parse_args()
    
    try:
        options.outfile
        options.macro
    except AttributeError:
        print usage
        sys.exit(1)
    
    #Open a file with the given name for writing
    try:
        with open(options.outfile, "wb") as out:            
            out.write("#define %s(x)\\\n" % options.macro)
            
            #Add all modules
            for module in args:            
                out.write("    x(%s) \\\n" % os.path.basename(module))
                
            out.write("\n")
    except IOError:
        print "Failed to open '%s' for writing!" % options.outfile
        sys.exit(1)
        
    