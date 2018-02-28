# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
"""
@file
Transaction bus MSC generation file.

@section Description
This script generates message sequence charts(MSC) from a transaction bus dump.
A pcap file should be opened in Wireshark and then exported as a PDML XML file. 
This script processes the xml file and generates msc-gen syntax which is then 
passed to msc-gen to produce the chart as an image. 

@section Usage
The normal use case is running the script with a set of parameters to generate
the MSC image. There is also an interactive mode for debugging accessed by 
using the "-i" parameter. In this mode there is an instance of the TrbMsc 
object called "trbmsc" for easy access to the internals.
See http://wiki/Hydra_wireshark#Generating_message_sequence_charts
"""

import os
import sys
import re
import pprint
import platform
from subprocess import Popen, PIPE
from decimal import Decimal as dec
from xml.dom import minidom
from cgi import log

# Prepend script-relative "./pylib" to the sys.path so a local py library 
# will override any default library.
#
if __name__ == '__main__':
    my_dir = os.path.abspath(os.path.dirname(__file__))    
    pylib_dir = os.path.join(my_dir, "pylib")
    if pylib_dir not in sys.path:
        sys.path.insert(0, pylib_dir)
        
    del my_dir
    del pylib_dir

from csr.wheels.front_end import FrontEnd

# Some constants and default values for some parameters
DEFAULTS = {"OS" : {"Windows" : { "msc_tool" : "msc-gen",
                                 "check_cmd" : "where"},
                      "Linux" : { "msc_tool" : "msc-gen",
                                 "check_cmd" : "which"}}[platform.system()],
         "rules" : "",
          "type" : "svg",
       "outfile" : "",
        "hscale" : "2"}

class TrbMsc(object):
    """
    This object takes a path to a PDML XML file and parses it on creation.
    It saves the packets in a local variable waiting for further actions like
    generating the MSC image or printing the packets.
    """
    class _Utils(object):
        """
        Helper class which implements helper functions for TrbMsc.
        """
        def __init__(self, log=None):
            self._log = log
        
        def _error(self, str, instr=None):
            """
            @brief Helper function to output error messages.
            
            @par str
            Error message.
            
            @par instr
            Flag which indicates which instructions to print if any.
            """
            strlog = str
            if instr == "msc_tool":
                strlog = (strlog + "\n" + self.__class__.__name__ + " needs " + 
                         DEFAULTS["OS"]["msc_tool"] + "to be able to generate "
                         "MSC charts. Get it from: http://sourceforge.net/"
                         "projects/msc-generator/. Version 4.4.0 was used "
                         "during development.")
            elif instr == "xml":
                strlog = (strlog + "\n" + r"Add wireshark plugin directory to "
                         "environment variable: export WIRESHARK_PLUGIN_DIR=/"
                         "home/hydra/checkedout/hydra/main/common/wireshark"
                         "/linux_64bit" + "\n" + r"Open the trace pcap file "
                         "with wireshark: /home/digital/wireshark/bin/"
                         "wireshark trace.pcap" + "\n" + 
                         r"Export the trace as a PDML XML.")
            elif instr == "rules":
                strlog = (strlog + "\nProcessing rules are designed to control"
                        " the output of message sequences. The following "
                         "keywords are supported:\n"
                         " - collapse: By default each subsystem and block "
                         "pair have dedicated columns. By using the collapse "
                         "keyword the user can instruct the parser to merge a "
                         "subsystem's blocks into a single column. Example: "
                         "\"collapse 0 2\" will collapse subsystem 0 and 2.\n"
                         " - no: Remove a subsystem or a subsystem block from "
                         "the output. This removes all messages coming from or"
                         " going to the specified subsystem or subsystem block."
                         " Example: \"no 0 4\" will block subsystem 0 and 4. "
                         "There is a special case of no keyword use: \"no "
                         "color\". This is used to produce black and white "
                         "images.\n"
                         " - proto: Limits the information to the specified "
                         "protocol. By default every protocol is displayed. "
                         "This can limit the amount of information displayed "
                         "to one or more. Packets which don't encapsulate the "
                         "protocol are not displayed at all. The order in which"
                         " protocols are parsed from left to right is the same"
                         " order they appear in the chart from bottom to top. "
                         "Example: \"proto csr_ccp_ver_v0 csr_ftp_ver_v0\" "
                         "will display all packets that encapsulate protocols "
                         "CCP and FTP limiting the protocol information to "
                         "only these two. Supported protocols are: csr_tbus, "
                         "csr_submsg_ver_v0, csr_isp_ver_v0, csr_ccp_ver_v0 "
                         "and csr_ftp_var_v0\n"
                         " - order: Enforces a fixed order of the columns. By "
                         "default they are alphabetically sorted. Example: "
                         "\"order 2 0 4\" starting from the left side, will "
                         "display subsystem 2 then 0 and finally 4. If "
                         "subsystems are not collapsed, there will be a "
                         "vertical line for each block of all expanded "
                         "subsystems in alphabetical order.\n"
                         " - timestamp: Enables the output of the time stamp " 
                         "assuming the first packet arrives at time 0. Example:"
                         " \"timestamp\". By using \"timestamp absolute\" the "
                         "time stamp will be grabbed as is in the XML file and"
                         " converted to milliseconds.\n"
                         " - ssid: Maps the subsystem ID to the name. ID and "
                         "name pairs must be enclosed in brackets. Example: "
                         "\"ssid (0 Curator)(2 Bluetooth)\" will create two "
                         "entries in the map associating 0 with Curator and 2 "
                         "with Bluetooth.\n"
                         "Rules can be chained by separating them by a comma: "
                         "\"no 4, collapse 0 2, order 2 0,timestamp, "
                         "proto csr_tbus, proto csr_submsg_ver_v0 "
                         "csr_isp_ver_v0, ssid (0 Curator)(4 Apps)\"")
            if self._log:
                self._log.fault(strlog)
            else:
                self._print(strlog)
        
        def _print(self, strlog):
            """
            @brief Shim function for printing useful information.
            
            @par strlog
            String to print.
            """
            if self._log:
                self._log.major(strlog)
            else:
                print(strlog)
        
        def _get_substr(self, str_in, marker1=None, marker2=None):
            """
            @brief Helper function which returns a substring delimited by two 
            markers.
            
            @par str_in
            Input string.
            
            @par marker1
            First marker string. If it's None it will be the beginning of the 
            string. Regex special characters should be properly escaped.
            
            @par marker2
            Second marker string. If it's None it will be the end of the string.
            Regex special characters should be properly escaped.
            
            It also strips leading and trailing white spaces from the resulting 
            substring.
            """
            if marker1 == None:
                prefix = r"^"
            else:
                prefix = r"%s[\s]*" %marker1
            
            if marker2 == None:
                suffix = r"$"
            else:
                suffix = r"[\s]*%s" %marker2
            
            ret =  re.findall(r'%s(.*?)%s' %(prefix, suffix), str_in)
            if ret:
                return ret[0]
            else:
                return ""
        
        def _dec_to_str(self, d):
            """
            @brief Returns a string containing a representation of a Decimal 
            number.
            
            @par d
            Decimal number.
            
            The representation contains negative sign, decimal mark and
            thousands separators. Also the number of decimal digits are fixed 
            to 6.
            """
            dec_places = 6
            d = d.normalize()
            if d < 0:
                sign = "-"
            else:
                sign = ""
            frac_part = str(abs(d-int(d))).split(".")[-1][:dec_places]
            frac_part = frac_part + "0"*(dec_places-len(frac_part))
            int_part = str(abs(int(d)))
            int_rev = int_part[::-1]
            int_rev = ",".join([int_rev[k:k+3] for k in range(0, len(int_rev), 
                                                                           3)])
            int_part = int_rev[::-1]
            return "%s%s.%s" %(sign, int_part, frac_part)
    
    
    
    def __init__(self, file, log=None):
        """
        @brief Creates the util object for future use and processes the xml 
        file saving the packets as a list of dictionaries.
        
        @par file
        Path to a PDML XML file.
        
        @par log
        Reference to the log object needed to output status and error messages.
        This can be omitted in which case simple prints will be used and the 
        verbosity level will be maximum.
        """
        self._utils = TrbMsc._Utils(log)
        self._packets = self._parse_file(file)
        if not self._packets:
            raise IOError("Failed to parse %s" %file)
    
    def _parse_file(self, file):
        """
        @brief Returns a list of dictionaries which store all packet 
        information.
        
        @par file
        Path to a PDML XML file.
        
        This function also checks for various conditions with which the XML 
        file must comply.
        """
        def _process_node(node, node_names=[]):
            """
            @brief Returns a dictionary containing processed data from one XML 
            node. This also contains all children nodes.
            
            @par node
            A dom child node object of a pdml node. This is a node named 
            "packet".
            
            @par node_names
            List of node names to look for and ignore the rest.
            
            This function parses nodes grabbing attribute data only. Then it 
            grabs all children nodes one by one in a recursive manner. Children
            nodes can be accessed by name in the members dictionary. There is 
            also a list of names in member_names if the order order the 
            children nodes is important. 
            """
            ret = dict()
            for attr in node.attributes.items():
                ret[attr[0]] = attr[1]
            ret["members"] = dict()
            ret["member_names"] = []
            for childnode in node.childNodes:
                if node_names and childnode.nodeName not in node_names:
                    continue
                member = _process_node(childnode, node_names)
                member_name = childnode.getAttribute("name")
                ret["members"][member_name] = member
                ret["member_names"].append(member_name)
            return ret
        
        if not isinstance(file, str):
            self._utils._error("TrbMsc needs a Wireshark PDML XML export.", 
                                                                         "xml")
            return None
        try:
            xml = minidom.parse(file)
        except IOError:
            self._utils._error("Cannot open XML file.", "xml")
            return None
        try:
            self._pdml = xml.firstChild
            if self._pdml.nodeName != "pdml":
                self._utils._error("Not a PDML XML file.", "xml")
                return None
        except:
            self._utils._error("Bad XML file.", "xml")
            return None
        if self._pdml.getAttribute("version") != "0":
            self._utils._error("PDML version %d is not supported" 
                                           %self._pdml.getAttribute("version"))
        
        ret = []
        for node in self._pdml.childNodes:
            if node.nodeName == "packet":
                ret.append(_process_node(node, ["proto", "field", "field"]))
        return ret
    
    def _process_packets(self, rules):
        """
        @brief Sumarises the XML packet information and trims it down according
        to the rules. Returns a dictionary containing the order of the 
        subsystems and list of dictionaries containing information for each 
        packet.
        
        @par rules
        String which encodes the rules: which subsystem(s) to supress or 
        collapse, enabling timestamp, order, SSID mapping, color supression and
        protocol supression. 
        """
        def _get_color(i):
            """
            @brief Associates an index to a hard-coded msc-gen color.
            
            @par i
            Color index.
            """
            return {0 : "black",
                    1 : "blue-50",
                    2 : "red-50",
                    3 : "green-60",
                    4 : "yellow-50",
                    5 : "blue+20",
                    6 : "red+40"}[i]
        
        def _get_ss(id, map):
            """
            @brief Helper function for decoding a subsystem's name having the 
            ID. Returns the subsystem name as a string.
            
            @par id
            ID of the subsystem as a string. This can also take block IDs. For 
            example "0.0" returns "Curator.0"(if the map associates 0 with 
            Curator.
            
            @par map
            Dictionary which associates subsystem IDs to names. Both are 
            strings.
            """
            i = self._utils._get_substr(id, r"^", r"(?!\d)")
            if i in map:
                decoded = map[i]
            else:
                decoded = i
            return "%s%s" %(decoded, self._utils._get_substr(id, r"^\d*", r"$"))
        
        def _process_timestamp(packet, ssmap):
            """
            @brief Helper function which returns useful information for the XML 
            timestamp attribute.
            
            @par packet
            Processed packet to look into.
            
            @par ssmap
            Optional subsystem ID map for decoding the subsystem name. Not used 
            here. 
            """
            ret = packet["members"]["geninfo"]["members"]["timestamp"]["value"]
            return ret, None
    
        def _process_tbus(packet, ssmap):
            """
            @brief Helper function which returns useful information for the XML 
            tbus protocol node.
            
            @par packet
            Processed packet to look into.
            
            @par ssmap
            Optional subsystem ID map for decoding the subsystem name. Not used.
            """
            tbus = packet["members"]["csr_tbus"]["members"]
            tag = "%d" %int(tbus["csr_tbus.Tag"]["value"], 16)
            op = self._utils._get_substr(tbus["csr_tbus.Opcode"]["showname"], 
                                                                     ":", "\(")
            if int(tbus["csr_tbus.Opcode"]["value"], 16) == 9:
                op = (op + " - " + 
                      self._utils._get_substr(tbus["csr_tbus.Payload"]
                              ["members"]["csr_tbus.t_extended_payload.Opcode"]
                                                      ["showname"], ":", "\("))
            ret = "TBUS:  %s,  tag %s" %(op,tag)
            return ret, None
        
        def _process_submsg_ver_v0(packet, ssmap):
            """
            @brief Helper function which returns useful information for the XML 
            submsg protocol node.
            
            @par packet
            Processed packet to look into.
            
            @par ssmap
            Optional subsystem ID map for decoding the subsystem name.
            """
            submsg = packet["members"]["csr_submsg_ver_v0"]["members"]
            src = _get_ss("%d" %int(submsg["csr_submsg.src_addr"]["value"]), 
                                                                         ssmap)
            dest = _get_ss("%d" %int(submsg["csr_submsg.dest_addr"]["value"]), 
                                                                         ssmap)
            chan = "%d" %int(submsg["csr_submsg.channel"]["value"])
            ret = "SUBMSG:  src %s, dest %s, chan %s" %(src,dest,chan)
            if "csr_submsg.protocol" in submsg:
                proto = self._utils._get_substr(submsg["csr_submsg.protocol"]
                                                       ["showname"], ":", "\(")
                ret = r"%s\nprotocol %s" %(ret,proto)
            if "csr_submsg.command" in submsg:
                com = self._utils._get_substr(submsg["csr_submsg.command"]
                                                       ["showname"], ":", "\(")
                ret = r"%s\ncommand %s" %(ret,com)
            return ret, None
    
        def _process_isp_ver_v0(packet, ssmap):
            """
            @brief Helper function which returns useful information for the XML 
            isp protocol node.
            
            @par packet
            Processed packet to look into.
            
            @par ssmap
            Optional subsystem ID map for decoding the subsystem name.
            """
            isp = packet["members"]["csr_isp_ver_v0"]["members"]
            src = _get_ss("%d" %int(isp["csr_isp.src_addr"]["value"]), ssmap)
            dest = _get_ss("%d:%d" %(int(isp["csr_isp.dest_addr"]["value"]),
                                int(isp["csr_isp.dest_port"]["value"])), ssmap)
            ret = "ISP:  src %s, dest %s" %(src,dest)
            return ret, None
        
        def _process_ccp_ver_v0(packet, ssmap):
            """
            @brief Helper function which returns useful information for the XML 
            ccp protocol node.
            
            @par packet
            Processed packet to look into.
            
            @par ssmap
            Optional subsystem ID map for decoding the subsystem name. Not used.
            """
            note = ""
            ccp = packet["members"]["csr_ccp_ver_v0"]["members"]
            ccp_names = packet["members"]["csr_ccp_ver_v0"]["member_names"]
            type = self._utils._get_substr(ccp["ccp_prim.type"]["showname"], 
                                                                     ":", "\(")
            text = "CCP:  %s" %type
            for elem in [ccp[k] for k in ccp_names]:
                if elem["name"] == "ccp_prim.type":
                    continue
                if "showname" in elem:
                    aux = elem["showname"]
                elif "show" in elem:
                    aux = elem["show"]
                else:
                    continue
                aux_trim = self._utils._get_substr(aux, "=")
                if aux_trim:
                    aux = aux_trim
                note = note + "\n%s" %aux
            return text, note.strip()
        
        def _process_ftp_ver_v0(packet, ssmap):
            """
            @brief Helper function which returns useful information for the XML 
            ftp protocol node.
            
            @par packet
            Processed packet to look into.
            
            @par ssmap
            Optional subsystem ID map for decoding the subsystem name. Not used.
            """
            note = ""
            ftp = packet["members"]["csr_ftp_ver_v0"]["members"]
            ftp_names = packet["members"]["csr_ftp_ver_v0"]["member_names"]
            type = self._utils._get_substr(ftp["ftp_prim.type"]["showname"], 
                                                                     ":", "\(")
            text = "FTP:  %s" %type
            for elem in [ftp[k] for k in ftp_names]:
                if elem["name"] == "ftp_prim.type":
                    continue
                if "showname" in elem:
                    aux = elem["showname"]
                elif "show" in elem:
                    aux = elem["show"]
                else:
                    continue
                aux_trim = self._utils._get_substr(aux, "=")
                if aux_trim:
                    aux = aux_trim
                note = note + "\n%s" %aux
            return text, note.strip()
        
        funcs = {"_timestamp" : _process_timestamp,
                 "_tbus" : _process_tbus,
                 "_submsg_ver_v0" : _process_submsg_ver_v0,
                 "_isp_ver_v0" : _process_isp_ver_v0,
                 "_ccp_ver_v0" : _process_ccp_ver_v0,
                 "_ftp_ver_v0" : _process_ftp_ver_v0}
        ret = []
        denied = []
        collapsed = []
        protocols = []
        order = []
        ssmap = dict()
        timestamp = False
        base_time = None
        if rules:
            rules = rules.split(",")
            for rule in rules:
                rule = rule.strip().split(" ", 1)
                op = rule[0]
                if len(rule) > 1:
                    data = rule[1]
                else:
                    data = None
                if op == "no":
                    denied = denied + data.split(" ")
                elif op == "collapse":
                    collapsed = collapsed + data.split(" ")
                elif op == "proto":
                    protocols = protocols + data.split(" ")
                elif op == "order":
                    order = order + data.split(" ")
                elif op == "timestamp":
                    timestamp = True
                    if data and data.split(" ")[0] == "absolute":
                        base_time = dec(0)
                elif op == "ssid":
                    while data:
                        pair = self._utils._get_substr(data, "\(", "\)").split()
                        data = self._utils._get_substr(data, "\)", "$")
                        if len(pair) != 2:
                            return None
                        ssmap[pair[0]] = pair[1]
                else:
                    return None
        if not protocols:
            protocols = ["csr_tbus", "csr_submsg_ver_v0", "csr_isp_ver_v0",
                                            "csr_ccp_ver_v0", "csr_ftp_ver_v0"]
        for packet in self._packets:
            srcM = packet["members"]["csr_tbus"]["members"]\
                                              ["csr_tbus.SysSourceId"]["value"]
            srcm = packet["members"]["csr_tbus"]["members"]\
                                            ["csr_tbus.BlockSourceId"]["value"]
            dstM = packet["members"]["csr_tbus"]["members"]\
                                                ["csr_tbus.SysDestId"]["value"]
            dstm = packet["members"]["csr_tbus"]["members"]\
                                              ["csr_tbus.BlockDestId"]["value"]
            src = srcM + "." + srcm
            dst = dstM + "." + dstm
            if (srcM in denied) or (dstM in denied) or (src in denied) or \
                                                               (dst in denied):
                continue
            # If there is no protocol found, skip this packet                 
            found = False
            for protocol in protocols:
                if protocol in packet["members"]:
                    found = True
            if not found:
                continue
            ppack = dict()
            if srcM in collapsed:
                ppack["source"] = srcM
            else:
                ppack["source"] = src
            if dstM in collapsed:
                ppack["destination"] = dstM
            else:
                ppack["destination"] = dst
            
            ppack["source_name"] = _get_ss(ppack["source"], ssmap)
            ppack["destination_name"] = _get_ss(ppack["destination"], ssmap)
            ppack["protocols"] = ""
            ppack["notes"] = []
            color = 1
            
            if timestamp:
                text, note = _process_timestamp(packet, ssmap)
                if base_time is None:
                    base_time = dec(text)
                time_str = (self._utils._dec_to_str((dec(text) - base_time) * 
                                                                 1000) + " ms")
                ppack["protocols"] = (r"\c(%s)time: %s\n" 
                                                     %(_get_color(0),time_str))
                if note:
                    ppack["notes"].append(r"\c(%s)%s" %(_get_color(0),note))
            for proto in protocols:
                if proto in packet["members"]:
                    if "color" in denied:
                        color = 0
                    ppack["color"] = color
                    text, note = funcs[proto[3:]](packet, ssmap)
                    ppack["protocols"] = (r"\c(%s)\b%s\b\n%s" 
                                             %(_get_color(ppack["color"]),text,
                                        ppack["protocols"].replace(r"\b", "")))
                    if note:
                        ppack["notes"].append(r"\c(%s)%s"
                                            %(_get_color(ppack["color"]),note))
                    color = color + 1
            ppack["protocols"] = r"%s\n" %ppack["protocols"]
            ppack["color"] = _get_color(ppack["color"])
            ret.append(ppack) 
        return {"order" : order, "packets" : ret}
    
    def _generate_msc_syntax(self, processed_packets, order=[], hscale="auto"):
        """
        @brief Returns a string containing the msc-gen syntax containing the 
        information in processed_packets.
        
        @par processed_packets
        Packet information as processed by _process_packets().
        
        @par order
        Optional list of subsystems which dictates their order.
        
        @par hascale
        String containing the horizontal scale of the image. This is usually 
        "auto" but it can be a real number referenced to 600 pixels.
        """
        msc = dict()
        msc["header"] = ("hscale=%s;\n" %hscale)
        msc["subsys"] = []
        msc["subsys_name"] = dict()
        msc["msg"] = []
        msc["footer"] = ""
        for packet in processed_packets:
            src = packet["source"]
            dest = packet["destination"]
            src_n = "\"" + packet["source_name"] + "\""
            dest_n = "\"" + packet["destination_name"] + "\""
            if src not in msc["subsys"]:
                msc["subsys_name"][src] = src_n
                msc["subsys"].append(src)
            if dest not in msc["subsys"]:
                msc["subsys_name"][dest] = dest_n
                msc["subsys"].append(dest)
            msc["msg"].append("{defstyle arrow[color=%s]; %s->%s:%s;};" 
                          %(packet["color"], src_n,dest_n,packet["protocols"]))
            if packet["notes"]:
                msc["msg"].append("note:%s[text.ident=left];" 
                                                     %"".join(packet["notes"]))
        if order:
            long_l = []
            for o in order:
                l=[]
                for s in msc["subsys"]:
                    if s.startswith(o):
                        l.append(s)
                l.sort()
                long_l = long_l + l
            msc["subsys"] = long_l
        else:
            msc["subsys"].sort()
        return (msc["header"] + 
                ",".join([msc["subsys_name"][i] for i in msc["subsys"]]) + 
                ";\n" + "\n".join(msc["msg"]) +
                msc["footer"])
    
    def generate(self, rules=None, type=None, out_file=None, hscale=None):
        """
        @brief Verifies input parameters and kicks off the MSC generation 
        process.
        
        @par rules
        String which encodes the rules: which subsystem(s) to supress or 
        collapse, enabling timestamp, order, SSID mapping, color supression and
        protocol supression. 
        
        @par type
        String which contains the file type to generate. This is usually "svg".
        
        @par out_file
        String which contains the path to the output file.
        
        @par hscale
        String containing the horizontal scale of the image. This is usually 
        "auto" but it can be a real number referenced to 600 pixels.
        """
        if rules == None:
            rules = DEFAULTS["rules"]
        if type == None:
            type = DEFAULTS["type"]
        if out_file == None:
            out_file = DEFAULTS["outfile"]
        if hscale == None:
            hscale = DEFAULTS["hscale"]
        
        if not out_file:
            self._utils._error("No output file name given")
            return
        check_sp = Popen([DEFAULTS["OS"]["check_cmd"], 
             DEFAULTS["OS"]["msc_tool"]], stdin=PIPE, stdout=PIPE, stderr=PIPE)
        tool_path, err = check_sp.communicate()
        if err:
            self._utils._error("Cannot find msc generator: %s\n Error: %s" 
                         %(DEFAULTS["OS"]["msc_tool"],err), instr = "msc_tool")
            return
        tool_path = tool_path.rstrip()
        msc_tool = Popen([tool_path, "-T", type, "-o", out_file], 
                                          stdin=PIPE, stdout=PIPE, stderr=PIPE)
        pp = self._process_packets(rules)
        if pp is None:
            self._utils._error("Processing rules error", instr = "rules")
            return
        if pp["packets"]:
            out, err = msc_tool.communicate(self._generate_msc_syntax(
                                           pp["packets"], pp["order"], hscale))
            err = err.strip()
            if err != "Success.":
                self._utils._error("MSC tool error:\n%s" %err)
                return
            self._utils._print("File of type %s with scale %s and rules \"%s\""
                           " was generated at %s"%(type,hscale,rules,out_file))
        else:
            self._utils._print("No data after processing") 
    
    def print_packets(self):
        """
        @brief Debug function for printing the parsed XML packets on screen.
        """
        pp = pprint.PrettyPrinter(indent=2)
        pp.pprint(self._packets)

class TrbMscFE(FrontEnd):
    """
    Front end class for TrbMsc. This class parses input parametes, sets default
    values and either kicks off the generation process or enters interactive 
    mode for debugging.
    """
    @staticmethod
    def _add_cmd_line_options(parser, custom_args = None):
        """
        @brief Registers options used by TrbMsc.
        """
        parser.add_option("-i", "--interactive",
                          help="Go to interactive mode",
                          action="store_true",
                          dest="interactive",
                          default=False)
        
        parser.add_option("-x", "--xmlfile",
                          help="PDML XML file path",
                          action="store", type="string",
                          dest="xmlfile",
                          default=None)
        
        parser.add_option("-o", "--outfile",
                          help="Output file path",
                          action="store", type="string",
                          dest="outfile",
                          default=None)
        
        parser.add_option("-t", "--outputtype",
                          help="Output file type: svg, png, pdf, eps or emf",
                          action="store", type="string",
                          dest="type",
                          default="svg")
        
        parser.add_option("-r", "--rules",
                          help="Packet processing rules"
                                        "(e.g. \"no 4, collapse 2 0\")",
                          action="store", type="string",
                          dest="rules",
                          default="")
        
        parser.add_option("-w", "--hscale",
                          help="Horizontal scale referenced to a 600 pixel "
                                                                    "wide PNG",
                          action="store", type="string",
                          dest="hscale",
                          default="auto")
    
    @staticmethod
    def main(options, args, log, shell=globals()):
        """
        @brief Creates a TrbMsc object, stores default values and either kicks 
        off the generation process or enters interactive mode.
        """
        global trbmsc
        trbmsc = TrbMsc(options.xmlfile, log)
        log.trivial("trbmsc object created successfully")
        if options.interactive:
            DEFAULTS["rules"] = options.rules
            DEFAULTS["type"] = options.type
            DEFAULTS["outfile"] = options.outfile
            DEFAULTS["hscale"] = options.hscale
            
        else:
            trbmsc.generate(options.rules, options.type, options.outfile, 
                                                                options.hscale)
            exit(0)

if __name__ == "__main__":
    TrbMscFE.main_wrapper()
