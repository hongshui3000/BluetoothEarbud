# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
"""Implement some simple grep functionality for as8051 assembly support"""
#
# No arguments, takes stdin to stdout
#

import sys

for line in sys.stdin:
    if not line.startswith(".linefile"):
        sys.stdout.write(line)
