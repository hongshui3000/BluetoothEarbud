# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
'''A litle make helper script to glue together the path to an object
and the object file name. '''
#
# Parameters:
# 1: The output file for the list of objects
# 2: the path to the object files
# 3: the object files
#
# The script just creates $1/$n for all n > 1 and outputs it
#

import sys, os
argc = len(sys.argv)
if 2 >= argc:
    sys.stderr.write("%s needs at least two arguments, preferably three\n" % sys.argv[0])
    sys.exit(1)
outfile = sys.argv[1]
prefix = sys.argv[2]
fdout = open(outfile, "a") 
for obj in sys.argv[3:]:
    file = "%s/%s" % (prefix, obj)
    # Check that it exists, if not we need to find it in a subdirectory
    if os.path.isfile(file):
        fdout.write("%s/%s\n" % (prefix, obj))
    else:
        for (path, dirs, filenames) in os.walk(prefix):
            if obj in filenames:
                # Need to convert back all the \ into / in case we've run windoze python
                fdout.write("%s/%s\n" % (path.replace("\\", "/"), obj))
                break
