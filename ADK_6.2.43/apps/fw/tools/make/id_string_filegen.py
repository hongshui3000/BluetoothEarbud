# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
# Python script to generate a C file containing the build string
# as a C array
#

import sys

output_file_name = sys.argv[1]
build_string_text = sys.argv[2]
build_id_number = sys.argv[3]

try:
    output_file = open(output_file_name, "w")
except IOError:
    print "Can't open file",output_file_name
    sys.exit()

output_string="""#include "hydra/hydra_types.h"

const uint8 build_id_string[] = "%s";
const uint32 build_id_number = %sUL;
""" % (build_string_text, build_id_number)

output_file.write(output_string)
output_file.close()
