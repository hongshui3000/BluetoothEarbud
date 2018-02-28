############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2012 - 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
#
############################################################################
import os,sys
here = os.path.realpath(os.path.join(os.path.dirname(__file__),"pylib"))
old_sys_path = sys.path
sys.path = [here] + sys.path
from csr.interface.mib_type import *
# We can reasonably assume that the import above hasn't resulted in a change to
# sys.path, so we can reset it safely
sys.path = old_sys_path

