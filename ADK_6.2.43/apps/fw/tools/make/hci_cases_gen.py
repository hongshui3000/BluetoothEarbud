#!/usr/bin/env python
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2

# PROGRAM
#       hci_cases_gen.py -  gen C source to handle most command complete events
#
# USAGE
#       hci_cases_gen.py  dm_types_strfile hci_types_cfile output_cfile
#
# DESCRIPTION
#       This program is normally used to generate the C source file
#       which handles the majority of HCI command complete events in
#       the DM.  This program reads file "str_file", which contains a
#       list of HCI and DM primitives, typedefs, structs, unions, etc.
#       in a format suitable for this program.
#       The output C source is written to "output_file".
#
#       Note: this script was translated into Python from BlueCore's
#       hci_cases_gen.pl.  There are most likely plenty of traces of this
#       heritage still visible.
#
# EXAMPLE USE
#       hci_cases_gen.py  dm_prim_types.str dm_hci_interface_han_cc_cs.c \
#       dm_hci_int_cc_gen.c
#
# MODIFICATION HISTORY
#       #1    27:sep:04  sv01   B-4070: Created (codespace saving changes).
#       ------------------------------------------------------------------
#       --- This modification history is now closed. Do not update it. ---
#       ------------------------------------------------------------------

import sys
import time
import re

if len(sys.argv) != 4:
    print "Syntax:\n\t%s str_file han_cc_cs_file output_file." % sys.argv[0]
    sys.exit(1)

dm_types_file = sys.argv[1]
cs_types_cfile = sys.argv[2]
hci_cases_outfile = sys.argv[3]

prim_type = {}
#prim_type is a dictionary whose main purpose is to store the elements of each
# DM or HCI type.  The hash however is also used to map typedefs, unions etc.
# For the important entries, the key is a type (HCI or DM) and
# the value is an array of arrays (of elements).
#===========================================
# The following are 2 examples of elements of the prim_type hash
# HCI_READ_CLOCK_RET_T  =>
#  | handle    | typedef hci_connection_handle_t |
#  | clock     | typedef uint32_t                |
#  | accuracy  | typedef uint16_t                |
#
# DM_HCI_READ_CLOCK_CFM_T =>
#  | type      | typedef dm_prim_t  |
#  | phandle   | typedef phandle_t  |
#  | status    | typedef uint8_t    |
#  | bd_addr   | typedef BD_ADDR_T  |
#  | clock     | typedef uint32_t   |
#  | accuracy  | typedef uint16_t   |
#===========================================
# NB: For all DM types, there always is type, phandle and status.
# For DM_HCI_READ_AUTO_FLUSH_CFM_T and DM_HCI_ROLE_DISCOVERY_CFM_T,
# the elements bd_addr and status are in the opposite order.

# a dictionary where each prim type maps to the instance of that type (taken 
# from the union)
prim_type_inst = {}


def lookup_type(t):
    '''
    Returns a list containing the contents of the specified type if it is
    a composite type.  Otherwise returns an empty list.
    '''

    global prim_type

    while True:
        r = prim_type.get(t)
    
        if isinstance(r, list):
            return r 
    
        if not r:
            return []
    
        t = r

def get_corres_hci_elem(hci_elem, dm_elem):
    """
    because some elements in the dm type have a different name from their corresponding
    elements in the hci type, we have to translate those names specially
    """

    if hci_elem == dm_elem:
        return dm_elem
    elif dm_elem == "FlushTimeout" and hci_elem == "timeout":
        return "timeout"
    elif dm_elem == "FlowControlEnabled" and hci_elem == "sco_flow_control_enable":
        return "sco_flow_control_enable"
    elif dm_elem == "role" and hci_elem == "current_role":
        return "current_role"
    elif dm_elem == "link_policy" and hci_elem == "link_policy_settings":
        return "link_policy_settings"

force_op_code = {
    # These are types that are used both by the application and by Bluestack,
    # so just because we see Bluestack using them, doesn't mean that we should
    # leave them out of the switch statement that we're trying to generate.
    # We force their inclusion.
    "HCI_WRITE_LINK_POLICY_SETTINGS" : 1,
    "HCI_READ_LOCAL_SUPP_FEATURES" : 1,
    "HCI_READ_LOCAL_EXT_FEATURES" : 1,
    "HCI_DELETE_STORED_LINK_KEY" : 1,
    "HCI_CREATE_CONNECTION_CANCEL" : 1,
    "HCI_READ_BD_ADDR" : 1,
    "HCI_WRITE_SCAN_ENABLE" : 1,
    "HCI_ULP_SET_ADVERTISING_PARAMETERS" : 1,
    "HCI_ULP_SET_ADVERTISE_ENABLE" : 1,
    "HCI_ULP_CREATE_CONNECTION_CANCEL" : 1,
    "HCI_ULP_RAND" : 1,
    "HCI_ULP_SET_SCAN_ENABLE" : 1,
    "HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES" : 1,
    "HCI_ULP_SET_EVENT_MASK" : 1,
    "HCI_ULP_ENCRYPT" : 1,
    "HCI_READ_LOCAL_VER_INFO" : 1,
    "HCI_SET_EVENT_MASK" : 1,
    "HCI_READ_ENCRYPTION_KEY_SIZE" : 1,
    "HCI_ULP_SET_RANDOM_ADDRESS" : 1
}

freak_dm_type = {
    # We ignore these
    # so we don't need a duplicate case statement
    "DM_HCI_STANDARD_COMMAND_CFM_T" : 1,
    "DM_HCI_BD_ADDR_COMMAND_CFM_T" : 1,
    "DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM_T" : 1,
    "DM_HCI_TYPED_BD_ADDR_COMMAND_CFM_T" : 1,
    "DM_HCI_TYPED_BD_ADDR_PT_COMMAND_CFM_T" : 1

}

nonstd_dmtype2opcode = {
    "DM_HCI_REMOTE_NAME_CFM_T"
    : "HCI_REMOTE_NAME_REQ"
}

def dump_type(arr):
    for el in arr:
        print "    %s %s" % (el[0], el[1])

re_typedef_decl = re.compile(r"^typedef\s+")
def check_match(hci_element, dm_element):

    typematch = True
  
    # To be rigorous, we make sure the element name AND type matches
    if hci_element[1] != dm_element[1]:
        if re_typedef_decl.search(hci_element[1]) and \
                                    re_typedef_decl.search(dm_element[1]):
            hci_type = re_typedef_decl.sub("", hci_element[1])
            dm_type = re_typedef_decl.sub("", dm_element[1])
            if lookup_type(hci_type) != lookup_type(dm_type):
                if not (hci_type == "int8_t" and dm_type == "uint8_t"):
                    typematch = False
        else:
            typematch = False
  
    if typematch: # type matches, now try name
        if hci_element[0] == dm_element[0]:
            return True; # name matches.  bingo!
        elif hci_element[0] == "timeout" and dm_element[0] == "FlushTimeout":
            return True
        elif hci_element[0] == "sco_flow_control_enable" and dm_element[0] == "FlowControlEnabled":
            return True
        elif hci_element[0] == "current_role" and dm_element[0] == "role":
            return True
        elif hci_element[0] == "link_policy_settings" and dm_element[0] == "link_policy":
            return True
    #   elif hci_element[0] == "handle" and dm_element[0] == "bd_addr":
    #       print "*********** handle and bd_addr paired together ************\n"
    #       return True
    else:
        return 0

try:
    with open(dm_types_file) as DM_TYPES_FILE:

        # Each line is:
        # new_type_name base_type
        #
        # Where base_type is one of:
        #    builtin_type ([unsigned] [long|short] int, [unsigned] char, float, double, void ...)
        #    "typedef" user_type_name
        #    "pointer" base_type
        #    "array" "[" [number] "]" base_type
        #    "struct" [struct_name] "{" [element_name base_type]+ "}"
        #    "union" [union_name] "{" [element_name base_type]+ "}"
        #    "enum" [enum_name] "{" [value_name value]+ "}"
        # For example:
        # DM_HCI_READ_CLOCK_CFM_T struct  { type typedef dm_prim_t phandle typedef phandle_t status typedef uint8_t bd_addr typedef BD_ADDR_T clock typedef uint32_t accuracy typedef uint16_t }
        for line in DM_TYPES_FILE:
            
            match = re.search(r"^((?:HCI|DM_HCI)_[^  \t\n\r{]+)\s+struct\s+\{\s*([^}\n]+)", line)
            if match:
                type = match.group(1)
                types_string = match.group(2).rstrip()
           #    print "Trying to match >$types_string<\n"
                if re.search(r"\b(?:struct|union|enum|float|double)\b", types_string):
                    print "can't parse %s" % types_string
                    sys.exit(1)
            
                # Iterate over types_string and build the array of 2D arrays mentioned at top of this file
                last_pos = 0
                regex = r"\s*(\S+)\s+((?:pointer\s+|array\s+\[\d+\]\s+)*(?:typedef\s+\S+|(?:signed\s+|unsigned\s+|long\s+|short\s+)*(?:int|char|void)))"
                for match in re.finditer(regex, types_string):
                    if not type in prim_type:
                        prim_type[type] = []
                    prim_type[type].append([match.group(1), match.group(2)])
                    #print "Possible match >$2< >$3<\n"
                    
                continue
          
            match = re.search(r"^(\S+)\s+typedef\s+(\S+)", line)
            if match:
                prim_type[match.group(1)] = match.group(2)
                continue
            
            match = re.search(r"^(\S+)\s+((?:signed\s+|unsigned\s+|long\s+|short\s+)*(?:int|char))",
                              line)
            if match:
                prim_type[match.group(1)] = match.group(2)
                continue
            
            match = re.search(r"^(?:DM_UPRIM_T|HCI_COMMAND_COMPLETE_ARGS_T)\s+union\s+\{\s*([^{}\n]+)",
                              line)
            if match:
                types_string = match.group(1).rstrip()
                if re.search(r"\b(?:struct|union|enum|float|double)\b", types_string):
                    print "can't parse '%s'" % types_string
                    sys.exit(1)
                
                # re.finditer returns an iterator over matches
                for match in re.finditer(r"\s*(\S+)\s+typedef\s+(\S+)", types_string):
                    prim_type_inst[match.group(2)] = match.group(1)
                
except IOError:
    print "Can't open %s!!" % dm_types_file
    sys.exit(1)

#===============================================================================

try:
    with open(cs_types_cfile) as CS_DM_TYPES_FILE:
        
        cs_opcode_list = {}
        cc_special_opcode_list = {}
        state = 0
        
        for line in CS_DM_TYPES_FILE:
        
            # Reading the CS opcodes
            if state == 0:
                 if re.search(r"void\shandle_hci_cs_fail", line):
                     state = 1
            elif state == 1:
                match = re.search(r"\s*case\s+(HCI_[A-Z_0-9]+)\:", line)
                if match:
                    cs_opcode_list[match.group(1)] = 1
                elif re.search("\s*default\:", line):
                    state = 2
            # Reading the special case CC opcodes (application)
            elif state == 2:
                if re.search(r"void\s+handle_hci_cc_application", line):
                   state = 3
            elif state == 3:
                match = re.search(r"\s*case\s+(HCI_\w+)\:", line) 
                if match:
                    cc_special_opcode_list[match.group(1)] = 1
                elif re.search(r"\sdefault\:", line):
                    state = 4
            # Reading the special case CC opcodes (bluestack)
            elif state == 4:
                if re.search(r"void\s+handle_hci_cc_bluestack", line):
                    state = 5
            elif state == 5:
                match = re.search(r"\s*case\s+(HCI_\w+)\:", line)
                if match:
                    cc_special_opcode_list[match.group(1)] = 1
                elif re.search(r"\sdefault\:", line):
                    break
            
except IOError:
     print "Can't open %s !!" % cs_types_cfile 
     sys.exit(1)

def get_opcode(dm_type):

    try:
        return nonstd_dmtype2opcode[dm_type]
    except KeyError:
        return re.sub(r"^DM_(\S+)_CFM_T$", r"\1", dm_type)

def get_dm_type_code(dm_type_code_t):

    dm_type_code = dm_type_code_t.rstrip("_T")
  
    if re.search(r"_AUTO_FLUSH_CFM", dm_type_code):
        dm_type_code = re.sub("_AUTO_FLUSH_CFM","_AUTO_FLUSH_TIMEOUT_CFM", 
                              dm_type_code)
    elif re.search(r"_SCO_FLOW_CONTROL_CFM", dm_type_code):
        dm_type_code = re.sub(r"_SCO_FLOW_CONTROL_CFM", "_SCO_FLOW_CONTROL_ENABLE",
                              dm_type_code)
    return dm_type_code

try:
    with open(hci_cases_outfile, "w") as CASES_OUT:

        CASES_OUT.write("""
/****************************************************************************
 ****************************************************************************
 ****************************************************************************
 This file is automatically generated, do not edit by hand, changes will be
 lost.

""")
        CASES_OUT.write("%s\n\n" % hci_cases_outfile)
        (year,mon,mday,hour,min,sec,wday,yday,isdst) = time.localtime()
        CASES_OUT.write("Generated on:     %4d-%02d-%02d %02d:%02d:%02d\n\n" %
                                            (year+1900,mon+1,mday,hour,min,sec))
        CASES_OUT.write("Generated by:     %s\n\n" % sys.argv[0])
        CASES_OUT.write("Input files:      %s\n" % dm_types_file)
        CASES_OUT.write("                  %s\n\n" % cs_types_cfile)
        CASES_OUT.write("""
****************************************************************************
 ****************************************************************************
 ****************************************************************************/

#include <string.h>
#include "bluestack/types.h"
#include "bluestack/bluetooth.h"
#include "common/common.h"
#include "common/error.h"
#include "fsm/fsm.h"
#include "bluestack/hci.h"
#include "bluestack/l2cap_prim.h"
#include "bluestack/dm_prim.h"
#include "pmalloc/pmalloc.h"
#include "dm/dm_layer_manager.h"
#include "dm/dm_hci_interface.h"
#include "dm/dm_mgt_interface.h"
#include "mblk/mblk.h"
#include "tbdaddr/tbdaddr.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *  handle_cc_autogen
 *
 *  DESCRIPTION
 *  Autogerated function that handles all command completes that can be handled
 *  using autogenerated code.  WARNING: Do NOT change the order of the element
 *  assignments as this could make a big difference to the code size. The tails
 *  of most case statements are similar so the compiler re-uses tails a lot.
 *
 *  RETURNS/MODIFIES
 *  Creates a local DM_UPRIM_T and sets the relevants elements before sending
 *  it upstream.
 *---------------------------------------------------------------------------*/
void handle_cc_autogen(HCI_UPRIM_T *hci_prim, HCI_UPRIM_T *ret_prim)
{
    DM_UPRIM_T dm_prim;
    HCI_EV_COMMAND_COMPLETE_T *prim = &hci_prim->hci_command_complete_event;
    HCI_COMMAND_COMPLETE_ARGS_T *arg_ptr =
                           (HCI_COMMAND_COMPLETE_ARGS_T *) prim->argument_ptr;
    uint8_t status = prim->status;
    hci_op_code_t opcode = prim->op_code;
    uint16_t size;

    switch(opcode)
    {
""")

        cases_ctr=0
        
        for typename, elems_array in prim_type.iteritems():
        
            if re.match(r"DM_HCI_\w+_CFM_T$", typename):
          
                dm_type = typename
                opcode = get_opcode(dm_type)
                hci_type = opcode + "_RET_T"
              
                # If the DM_HCI_..._COMPLETE_T is actually a cs or is a cc that is handled
                # specially or a freak pdu that should not be handled then ignore it.
                if not opcode in force_op_code:
                    if opcode in cs_opcode_list or \
                       opcode in cc_special_opcode_list or \
                       dm_type in freak_dm_type :
                        continue
              
                dm_type_code = get_dm_type_code(dm_type)
              
                cases_ctr += 1
                CASES_OUT.write("\tcase %s:\n" % opcode)
              
                hci_type_list = lookup_type(hci_type)
                dm_type_list = lookup_type(dm_type)
                
                if not isinstance(hci_type_list, list):
                    print "'%s' is not a list; continuing" % hci_type_list
                    continue
                            
                dm_index = len(dm_type_list)-1
                hci_index = len(hci_type_list)-1
                need2free = 0
                conseq_flag = 1
                dm_marker = dm_index
                hci_marker = hci_index
                tmp_str = ""
                dm_var = prim_type_inst.get(dm_type)
                hci_var = prim_type_inst.get(hci_type)
                bd_addr_exists_flag = 0
                tbdaddr_exists_flag = 0
                status_after = 0
              
                if dm_var:   # if an instance of the dm type exists in the union DM_UPRIM_T use it
                    tmp_cast_str = "dm_prim.%s." % dm_var
                else:
                    tmp_cast_str = "((%s *) &dm_prim)->" % dm_type
              
                CASES_OUT.write("\t\t%stype = opcode + \n"
                                "\t\t    (%s -\n\t\t    %s);\n" % 
                                (tmp_cast_str, dm_type_code, opcode))
                CASES_OUT.write("\t\tsize = sizeof(%s);\n" % dm_type)
                # deal with dm elements which have matching hci elements
                while dm_index >= 0 and hci_index >= 0:
                    if not check_match(hci_type_list[hci_index],
                                       dm_type_list[dm_index]):
                        if hci_index > 0 or (
                                hci_type_list[hci_index][0] != "handle" and
                                hci_type_list[hci_index][0] != "connection_handle"):
                            print ("%s: Error in opcode %s, unable to pair "
                              "%s with %s. Fields in the first type are {%s}"
                                    ", fields in the second type are {%s}.\n" % \
                                    (sys.argv[0], opcode, 
                                     hci_type_list[hci_index][0],
                                     dm_type_list[dm_index][0],
                                     ", ".join("%s %s" % (type[1], type[0]) for type in hci_type_list),
                                     ", ".join("%s %s" % (type[1], type[0]) for type in dm_type_list)))
                            sys.exit(1)
                      
                        if dm_type_list[dm_index][0] == "status":
                            status_after = 1
                    else:
                        # We have matching elements, copy the value across.
                        if dm_type_list[dm_index][1] == "typedef BD_ADDR_T":
                            CASES_OUT.write("\t\tbd_addr_copy(&%s%s,\n" % 
                                        (tmp_cast_str, dm_type_list[dm_index][0]))
                            CASES_OUT.write("\t\t    &arg_ptr->%s.%s);\n" % 
                                            (hci_var, hci_type_list[hci_index][0]))
                      
                            bd_addr_exists_flag = 1
                      
                        elif dm_type_list[dm_index][1] == "typedef tbdaddr":
                            CASES_OUT.write("\t\ttbdaddr_copy(&%s%s,\n" % 
                                        (tmp_cast_str, dm_type_list[dm_index][0]))
                            CASES_OUT.write("\t\t    &arg_ptr->%1.%2);\n" % 
                                          (hci_var, hci_type_list[hci_index[0]]))
                      
                            tbdaddr_exists_flag = 1
                        else:
                            if re.match("array", dm_type_list[dm_index][1]):
                                CASES_OUT.write("\t\tmemcpy(&(%s%s),\n" % (tmp_cast_str,dm_type_list[dm_index][0]))
                                CASES_OUT.write("\t\t    &arg_ptr->%s.%s,\n" % (hci_var,hci_type_list[hci_index][0]))
                                CASES_OUT.write("\t\t    sizeof(%s%s));\n" % (tmp_cast_str,dm_type_list[dm_index][0]))
                            else:
                                CASES_OUT.write("\t\t%s%s =\n"
                                                "\t\t    arg_ptr->%s.%s;\n" % 
                                        (tmp_cast_str, dm_type_list[dm_index][0],
                                         hci_var, hci_type_list[hci_index][0]))
                      
                        dm_index -= 1
                    hci_index -= 1
              
                # deal with dm elements which do not have matching hci elements
                while dm_index >= 0:
                    if dm_type_list[dm_index][0] == "type":
                        #       CASES_OUT.write("\t\ttype = opcode + (%s - %s);\n" % (tmp_cast_str,dm_type_code,opcode))
                        pass
                    elif dm_type_list[dm_index][0] == "phandle":
                        #       CASES_OUT.write("\t\tphandle = am_phandle;\n" % ($tmp_cast_str))
                        pass
                    elif dm_type_list[dm_index][0] == "status":
                        CASES_OUT.write("\t\t%sstatus = status;\n" % (tmp_cast_str))
                    elif dm_type_list[dm_index][0] == "bd_addr": #deal with bd_addr here
                        hci_ret_prim = opcode.lower()
                  
                        bd_addr_exists_flag = 1
                    
                        CASES_OUT.write("\t\tbd_addr_copy(&%sbd_addr,\n"
                                        "\t\t    &ret_prim->%s.bd_addr);\n" % 
                                        (tmp_cast_str, hci_ret_prim))
                    elif dm_type_list[dm_index][0] == "addrt": #deal with TYPED_BD_ADDR_T here
                        hci_ret_prim = opcode.lower()
                    
                        tbdaddr_exists_flag = 1
                    
                        CASES_OUT.write("\t\ttbdaddr_copy(&%saddrt,\n"
                                        "\t\t    &ret_prim->%s.addrt);\n" % 
                                        (tmp_cast_str, hci_ret_prim))
                    elif dm_type_list[dm_index][0] == "tp_addrt": #deal with TP_BD_ADDR_T here
                        hci_ret_prim = opcode.lower()
                    
                        bd_trans_addrt_exists_flag = 1
                    
                        CASES_OUT.write("\t\t%stp_addrt = ret_prim->%s.tp_addrt;\n" % 
                                        (tmp_cast_str, hci_ret_prim))
                    else:
                        print "Unexpected DM element %s in %s no match in %s\n" \
                               % (dm_type_list[dm_index][0],dm_type,hci_type)
                        print "%s:" % dm_type
                        dump_type(dm_type_list)
                        print "%s:" % hci_type 
                        dump_type(hci_type_list)
                        sys.exit(1)
                        
                    dm_index -= 1
              
                CASES_OUT.write("\t\tDM_SendMessageAMphandle(pcopy(&dm_prim, size));\n")
                CASES_OUT.write("\t\tbreak;\n")
              
                tbdaddr_exists_flag = None
                bd_addr_exists_flag = None
                status_after = None
                hci_type = None
                dm_type = None
                dm_var = None
                hci_var = None
                tmp_cast_str = None
        
        if cases_ctr == 0:
            print "Error! No case statements autogenerated in %s.  Check the input files." % (hci_cases_outfile)
        
        print "Total number of case statements autogenerated in %s : %d" % (hci_cases_outfile, cases_ctr)

        CASES_OUT.write("""

        default:
            /* dm_ret_not_used(hci_prim, ret_prim); */
            return;
    }
}
""")

except IOError:
    print "Couldn't open %s for writing" % hci_cases_outfile
    sys.exit(1)
