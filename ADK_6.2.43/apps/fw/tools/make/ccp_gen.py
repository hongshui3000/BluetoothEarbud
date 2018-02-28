#!/usr/bin/env python
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2

# Script to turn CCP XML file into functions for handling CCP PDUs.
# Relies on the standard interface header, ccp_prim.h.
# Generates its own prototype header and individual functions.
#
# Options (must be in separate words before all arguments):
# -s:  Strip the path from headers found in ccp.xml for #include's.
#
# Arguments:
# Input XML file (lives in common/interface/ccp.xml).
# Private header in the form to be #include'd in generated C files.
# Output prototype header file (used unmodified).
# Output directory for generated functions.
#

import sys
import re
import xml.dom.minidom

strip = False
prog = sys.argv[0]
sys.argv = sys.argv[1:]

if len(sys.argv) > 0 and sys.argv[0] == '-s':
   strip = True
   sys.argv = sys.argv[1:]

if len(sys.argv) != 4:
   raise """Usage: %s [-s] ccp.xml private_header output_header output_dir
-s strips directories from headers found in ccp.xml.""" % (prog)

input_file = sys.argv[0]
private_header = sys.argv[1]
output_header = sys.argv[2]
output_dir = sys.argv[3]

class Myglobals:
   signals_xml = None
   signal_types_xml = None
   types_xml = { }
   scanned_files = { }

dirfind = input_file.rfind('/')
if dirfind != -1:
   Myglobals.top_dir = input_file[0:dirfind]
else:
   Myglobals.top_dir = ''

# Scan XML files recursively.
def scan_dom(fname):
   dom = xml.dom.minidom.parse(fname)
   Myglobals.scanned_files[fname] = 1
   for element in dom.documentElement.childNodes:
      if element.nodeName == "type":
         if element.hasAttribute("resource"):
            if element.getAttribute("resource") == "FILE":
               fname2 = element.getAttribute("file")
               if fname2[0] != '/' and Myglobals.top_dir != '' :
                  fname2 = Myglobals.top_dir + '/' + fname2
               if not Myglobals.scanned_files.has_key(fname2):
                  scan_dom(fname2)
               continue
         name = element.getAttribute("name")
         if name.upper() == "CCP_SIGNAL_ID":
            Myglobals.signals_xml = element
         Myglobals.types_xml[name] = element
         # print attr.name, attr.value
      elif element.nodeName == "resource":
         name = element.getAttribute("name")
         if name.upper() == "CCP_MESSAGE_PAYLOAD":
            Myglobals.signal_types_xml = element
scan_dom(input_file)

if Myglobals.signals_xml is None:
   raise RuntimeError, "Couldn't find type with name Ccp_Signal_Id in %s" % (input_file)
if Myglobals.signal_types_xml is None:
   raise RuntimeError, "Couldn't find type with name CCP_Message_Payload in %s" % (input_file)

num_signals = 0

for sig in Myglobals.signals_xml.getElementsByTagName("value"):
   name = sig.getAttribute("name")
   value = int(sig.getAttribute("value")) + 1
   if value > num_signals:
      num_signals = value

if num_signals == 0:
   raise "No signals defined by Ccp_Signal_id in %s" % (input_file)

def word_offset(signame, argname):
   """Return the word offset macro definition for signal argument.
   The offset defined includes the size of the CCP header."""
   return "CCP_%s_%s_WORD_OFFSET" % (
      signame.upper(), argname.upper())

def word_size(tpname):
   """Return the word size macro definition for type name."""
   if tpname == "uint32":
      return "2"
   elif tpname == "uint16":
      return "1"
   elif Signal.types.has_key(tpname):
      return "%s" % (Signal.types[tpname].size)
   else:
      return "%s_WORD_SIZE" % (tpname.upper())

def mangle_type(typename):
   # Do some fairly simple mangling on types so we can use them in
   # prototypes.
   #
   # We will not bother doing the full quoting used in C++
   # name mangling; we'll just assume we can use a few well-chosen
   # names of our own.
   return re.sub("\\*", "_PTR_", re.sub(' ', '_', typename))

def gen_proto_send(signame, argname, typename, size, is_enum, is_struct, is_varlen):
   """Generate prototype and copy code for sending an argument."""
   add_code = None
   wordoff = word_offset(signame, argname)
   if is_varlen:
      # Array.
      # We'll let the user copy in the value, so there's
      # no entry for this in the prototype.  Instead,
      # we'll generate a macro giving the position in the
      # array.
      proto_code = None
      copy_code = None
      add_code = """/**
 * Offset from start of command PDU to variable length argument.
 * Used to access argument in place.
 */
#define CCP_OFFSET_%s_%s \\
    %s
""" % (signame.upper(), argname.upper(), wordoff)
      signature = None
   else:
      # There's not actually any point putting in the "CONSTPTR"
      # bit if is_struct, since each type maps uniquely to a form of
      # argument handling, so lets keep verbosity down.
      # signature = "CONSTPTR_" + mangle_type(typename)
      signature = mangle_type(typename)
      if is_struct:
         proto_code = "const %s *%s" % (typename, argname)
         copy_code = "    CCP_%s_%s_SET(pdu, %s);" % (
            signame.upper(), argname.upper(), argname)
      else:
         # Can still pass in as integer
         proto_code = "%s %s" % (typename, argname)
         if is_enum:
            copy_code = "    /*lint -e{571} passing enum through generic interface */\n"
         else:
            copy_code = ""
         copy_code += "    CCP_%s_%s_SET(pdu, %s);" % (
            signame.upper(), argname.upper(), argname)
   return (proto_code, copy_code, add_code, signature)

def gen_proto_recv(signame, argname, typename, size, is_enum, is_struct, is_varlen):
   """Generate prototype and copy code for receiving an argument."""
   add_code = None
   wordoff = word_offset(signame, argname)
   if is_varlen:
      # Array.  Logic is identical to send direction; copying
      # is done elsewhere, we just return an offset.
      # The offset's the same for send, so we don't need
      # to generate any code.
      proto_code = None
      copy_code = None
      signature = None
   else:
      signature = mangle_type(typename)
      if is_struct:
         proto_code = "%s *%s" % (typename, argname)
         copy_code = "    CCP_%s_%s_GET(pdu, %s);" % (
            signame.upper(), argname.upper(), argname)
      else:
         proto_code = "%s *%s" % (typename, argname)
         cast = "(%s)" % (typename)
         copy_code = "    *%s = %sCCP_%s_%s_GET(pdu);" % (
            argname, cast, signame.upper(), argname.upper())
   return (proto_code, copy_code, add_code, signature)

class Signal:
   # List of all command signals
   signals = [ ]
   # Map from signal name to type name, as in XML file
   types_map = { }
   # List of signals ordered by ID.  Not currently used
   signals_by_id = None
   # Dictionary of SignalType entries for types.
   types = { }
   # Dictionary of XML DOM structure for the type.
   # None because assigned directly from what the XML parser returns.
   types_xml = None

   # List of all headers we need to include for types.
   headers = [ ]
   # Single-inclusion protection for headers array
   headers_seen = { }

   SEND = 0
   RECV = 1

   PROTO = 0
   COPY = 1
   ADD = 2
   SIGNATURE = 3

   class SignalType:
      def __init__(self, name, subtypes, size, is_enum, is_struct, is_varlen, comment):
         self.name = name
         self.subtypes = subtypes
         self.size = size
         self.enum = is_enum
         self.struct = is_struct
         self.varlen = is_varlen
         self.comment = comment
         Signal.types[name] = self

   class Arg:
      def __init__(self, signame, name, typename, varlen, comment):
         self.name = name
         self.typename = typename
         self.comment = comment
         self.varlen = varlen
         if not Signal.types.has_key(typename):
            raise IndexError, "No definition for type %s" % (typename)
         if comment is None:
            raise RuntimeError, "Argument %s of signal %s has no comment." % (
               name, signame)
         m = re.search('type depends on (\S+)', comment)
         if m and m.group(1) != typename:
            # There is a type for this defined elsewhere.
            # We'll attempt to use that.
            deftypename = m.group(1)
            if not Signal.types.has_key(deftypename):
               tp = Signal.types[typename]
               # Treat all type dependencies as enum.
               # All that means is we get a lint suppression when
               # casting, which is fine.
               Signal.SignalType(deftypename, [], tp.size, 1, tp.struct,
                                 tp.varlen,
                                 "Generated from %s" % (deftypename))
            typename = deftypename
            self.comment = re.sub('.*type depends on.*\n?', '', self.comment)
         self.tp = Signal.types[typename]
         # In principle self.varlen and tp.varlen are different.
         # tp.varlen means the component of the type is repeated,
         #   e.g. a uint16 within an IE array.
         # self.varlen means the whole type is repeated, e.g.
         #   a service record header in an srh_array.
         # However, we don't actually care about the nesting;
         # both just mean we have a pointer to the type and a length
         # of the number of entries of the type.  So treat the same.
         if self.tp.varlen:
            self.varlen = 1
            # The following gets rid of placeholders only used to
            # indicate arrays.  Works fine at the moment, we'll need
            # to be careful we update the XML consistently.
            if len(self.tp.subtypes) == 1:
               self.tp = Signal.types[self.tp.subtypes[0]]
         if self.tp.size is None:
            raise RuntimeError, "No size found for type %s" % (typename)
         self.code = [ None, None ]
         self.code[Signal.SEND] = gen_proto_send(
            signame, name, self.tp.name, self.tp.size, self.tp.enum,
            self.tp.struct, self.varlen)
         self.code[Signal.RECV] = gen_proto_recv(
            signame, name, self.tp.name, self.tp.size, self.tp.enum,
            self.tp.struct, self.varlen)

   def __init__(self, name, sig_id):
      self.name = name
      self.sig_id = sig_id
      if not Signal.types_map.has_key(name):
         raise IndexError, "Couldn't find type for signal %s" % (name)
      self.sig_typename = Signal.types_map[name]
      if not Signal.types.has_key(self.sig_typename):
         print "No type %s for signal %s" % (self.sig_typename, name)
      self.sig_type = Signal.types[self.sig_typename]
      self.comment = None
      self.varlen = 0

      Signal.signals.append(self)
      Signal.signals_by_id[self.sig_id] = name

      self.comment = Signal.types[self.sig_typename].comment
      if self.comment is not None:
         m = re.search('HEADER (\S+\.h)', self.comment)
         if m is not None:
            header = m.group(1)
            if not Signal.headers_seen.has_key(header):
               Signal.headers.append(header)
               Signal.headers_seen[header] = 1

      self.args = []
      if not Signal.types_xml.has_key(self.sig_typename):
         print "No XML for type %s for signal %s" % (tp, name)
      # Handle the various arguments of the command.
      for arg in Signal.types_xml[self.sig_typename].getElementsByTagName("field"):
         arg_name = arg.getAttribute("name")
         if re.match('reserved', arg_name):
            continue
         if re.match('spare', arg_name):
            continue
         arg_comment = None
         arg_type = None
         if self.varlen:
            raise RuntimeError, """Arg %s of command %s follows array.
The current code only copes with arrays last.""" % (arg_name, self.name)
         # Comments appear before the type they describe,
         # so we need to look at all child nodes for the field.
         for field in arg.childNodes:
            if field.nodeType == xml.dom.Node.COMMENT_NODE and \
                   arg_comment == None:
               arg_comment = field.nodeValue
            elif field.nodeType == xml.dom.Node.ELEMENT_NODE and \
                   field.tagName == "type":
               for cn in field.childNodes:
                  if arg_type is not None:
                     raise RuntimeError, \
                         "Multiple types for sig %s arg %s: %s" % (
                        self.name, arg_name, cn.data)
                  arg_type = cn.data
         if arg.hasAttribute("blocklength") or re.match('NullTermString', arg_type):
            arg_varlen = 1
         else:
            arg_varlen = 0
         sigarg = Signal.Arg(self.name, arg_name, arg_type, arg_varlen, arg_comment)
         if sigarg.varlen:
            self.varlen = 1
         self.args.append(sigarg)
      # sys.stdout.write("Fields for signal %s" % (self.name))
      # if (self.varlen):
      #    sys.stdout.write("(varlen)")
      # print ":"
      # for arg in self.args:
      #    print "  %s -> %s (%d): %s" % (
      #       arg.name, arg.typename, arg.varlen, arg.comment)

   def pdu_name(self):
      return "CCP_SIGNAL_ID_%s" % (self.name.upper())

   def word_size(self):
      return "CCP_%s_WORD_SIZE" % (self.name.upper())

# Set up static elements of Signal
# Add types.  We'd be interested to know if the type is an enum,
# a structure, or is (variable length).
#
# We'll do this both for resource "TYPE" and resource "CCP_SIGNALS".
Signal.types_xml = Myglobals.types_xml
for tp in Myglobals.types_xml.items():
   name = tp[0]
   fields = tp[1]
   is_enum = 0
   is_struct = 0
   is_varlen = None
   nfields = 0
   comment = None
   subtypes = [ ]
   if fields.hasAttribute("size"):
      size = int(fields.getAttribute("size"))
   else:
      size = None
   if fields.getAttribute("resource") == "FILE":
      continue
   for field in fields.childNodes:
      if field.nodeType == xml.dom.Node.COMMENT_NODE and \
             comment is None:
         comment = field.nodeValue
      if field.nodeType == xml.dom.Node.ELEMENT_NODE:
         if field.tagName == "value":
            is_enum = 1
         elif field.tagName == "field":
            nfields += 1
            if field.hasAttribute("blocklength"):
               is_varlen = 1
            for tp in field.getElementsByTagName("type"):
               for cn in tp.childNodes:
                  if cn.nodeType == xml.dom.Node.TEXT_NODE:
                     subtypes.append(cn.data)
                     if not Signal.types.has_key(cn.data):
                        sizematch = re.compile("u?int([0-9]+)$")
                        m = sizematch.match(cn.data)
                        if m:
                           subsize = int(m.group(1))
                        elif cn.data == "Bit":
                           subsize = 1
                        else:
                           continue
                        Signal.SignalType(cn.data, [], subsize,
                                          0, 0, 0, "Autogenerated.")
   if nfields > 1:
      is_struct = 1
   Signal.SignalType(name, subtypes, size, is_enum, is_struct, is_varlen,
                     comment)

# Look at type and subtype for variable length
def signal_type_is_varlen(tp):
   if tp.varlen is not None:
      return
   for subtp_name in tp.subtypes:
      subtp = Signal.types[subtp_name]
      if subtp.varlen is None:
         signal_type_is_varlen(subtp)
      if subtp.varlen:
         tp.varlen = 1
         return
   tp.varlen = 0

# Look at type and subtype for size
def signal_size(tp):
   if tp.size is not None:
      return
   if len(tp.subtypes) == 0:
      raise RuntimeError, "type %s: no size but no subtypes" % (tp.name)
   size = 0
   for subtp_name in tp.subtypes:
      subtp = Signal.types[subtp_name]
      if subtp.size is None:
         signal_size(subtp)
      size += subtp.size
   tp.size = size

# Check recursively for sizes etc.
for tp in Signal.types.items():
   signal_type_is_varlen(tp[1])
   signal_size(tp[1])

# Add type mapping for signals
for sig in Myglobals.signal_types_xml.getElementsByTagName("rmap"):
   dvalue = sig.getAttribute("dvalue")
   tp = sig.getAttribute("type")
   Signal.types_map[dvalue] = tp

# Add signals
Signal.signals_by_id = [ None ] * num_signals
for sig in Myglobals.signals_xml.getElementsByTagName("value"):
   Signal(sig.getAttribute("name"), int(sig.getAttribute("value")))

def comment_format(comment):
   lines = comment.splitlines()
   comment = ""
   for line in lines:
      comment += re.sub('^ *', ' * ', re.sub(' *$', '', line)) + "\n"
   return comment

# Now do stuff with what we've generated.
#
# Generate prototypes.
#
fout = open(output_header, "w")
sys.stdout = fout
prot_macro = output_header
while 1:
   if re.search('\.', prot_macro) is None:
      break
   prot_macro = re.sub('\.', "_", prot_macro)
prot_macro = re.sub("^.*/", "", prot_macro).upper()
print """/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */"""
print """/**
 * \\file %s
 *
 * Autogenerated prototypes of functions for sending and receiving
 * CCP commands.
 */

#ifndef %s
#define %s
""" % (output_header, prot_macro, prot_macro)

if len(Signal.headers) > 0:
   print """/*
 * The following headers are specified as needed for certain
 * types.  If the type resides elswhere in the subsystem firmware,
 * inclusion of the header can be suppressed by macro and an alternative
 * header included from ccp.h.
 */"""
   for header in Signal.headers:
      header_sub = re.sub('[\./]', '_', header).upper()
      if strip:
         header = re.sub('^.*/', '', header)
      print "#ifndef CCP_DONT_INCLUDE_%s" % header_sub
      print "#include \"%s\"" % (header)
      print "#endif"
   print

# Record argument-based prototypes we've already seen
send_proto_done = { }
recv_proto_done = { }

for sig in Signal.signals:
   proto = ""
   for arg in sig.args:
      for dir in (Signal.SEND, Signal.RECV):
         if arg.code[dir][Signal.ADD] is not None:
            proto += arg.code[dir][Signal.ADD] + "\n"
   if sig.comment is None:
      comment = "(No comment provided.)\n"
   else:
      comment = comment_format(sig.comment)
   #
   # Send
   #
   signature = ""
   for arg in sig.args:
      if arg.code[Signal.SEND][Signal.SIGNATURE] is not None:
         if len(signature) > 0:
            signature += "__"
         signature += arg.code[Signal.SEND][Signal.SIGNATURE]
      if arg.varlen:
         if len(signature) > 0:
            signature += "__"
         signature += "VARLEN"
   if signature == "":
      signature = "NOARGS"
   # We'll use the same signature string for send and recv:
   # although the argument types are different they map
   # consistently.
   sig.signature = signature

   send_proto_doc = """/**
 * Send CCP PDU %s.
 *
%s""" % (sig.name, comment)
   send_proto_doc += """ *
 * Arguments:
 *
 *   * \\c dest
 * Destination ISP address on port \\c ISP_PROTOCOL_CCP
"""
   if sig.varlen:
      send_proto_doc += """ *
 *   * \\c pdu
 * PDU with variable-length data already filled in.
 *
 *   * \\c pdu_len_words
 * Total length of PDU in 16-bit words including the variable-length
 * data.
"""
   normal_args = 0
   for arg in sig.args:
      if arg.comment is not None:
         if arg.varlen:
            send_proto_doc += """\
 *
 *   * Non-parameter argument \\c %s:\n""" % (arg.name)
            send_proto_doc += comment_format(arg.comment)
            send_proto_doc += """ * This argument is variable length and is not passed in band.
 * Create the PDU separately and insert the data using the offset
 * \\c CCP_%s_%s_WORD_OFFSET.
 * Then call the function passing the PDU, the
 * total length in words, and any other arguments.
 * The PDU may be freed immediately after the call.
""" % (sig.name.upper(), arg.name.upper())
         else:
            normal_args += 1
            send_proto_doc += " *\n *   * \\c %s\n" % (arg.name)
            send_proto_doc += comment_format(arg.comment)
   send_proto_doc += """ *
 * \\return \\c TRUE if the PDU was successfully sent, else \\c FALSE.
 */
"""
   if not send_proto_done.has_key(sig.signature):
      prologue = "bool ccp_send_pdu_" + sig.signature + "("
      len_indent = len(prologue)
      send_proto = send_proto_doc + prologue
      # Cosmetics
      if len_indent > 40:
         len_indent = 8
         send_proto += "\n" + (" " * len_indent)
      send_proto += "CCP_SIGNAL_ID ccp_signal_id, uint16 dest"
      sep = ",\n" + (" " * len_indent)
      if sig.varlen:
         send_proto += sep + "uint16 *pdu, uint16 pdu_len_words"
      for arg in sig.args:
         if arg.code[Signal.SEND][Signal.PROTO] is not None:
            send_proto += sep + arg.code[Signal.SEND][Signal.PROTO]
      send_proto += ")"
      sig.send_proto = send_proto
      proto += send_proto + ";\n\n" + send_proto_doc
      send_proto_done[sig.signature] = 1
   else:
      send_proto_done[sig.signature] += 1
      proto += send_proto_doc

   # Use definition to hide common signature-based function
   if sig.varlen:
      send_args = normal_args + 3
   else:
      send_args = normal_args + 1
   proto += "#define ccp_send_pdu_%s(" % (sig.name)
   for i in range(send_args):
      proto += "arg%d" % (i + 1)
      if i < send_args - 1:
         proto += ", "
   proto += ") \\\n    ccp_send_pdu_%s(%s, " % (sig.signature, sig.pdu_name())
   for i in range(send_args):
      proto += "arg%d" % (i + 1)
      if i < send_args - 1:
         proto += ", "
   proto += ")\n\n"

   #
   # Receive
   #
   recv_proto_doc = """/**
 * Analyse received CCP PDU %s.
 *
%s""" % (sig.name, comment)
   recv_proto_doc += """ *
 * Arguments:
 *
 *   * \\c pdu
 * The received PDU.
 *
 *   * \\c pdu_len_words
 * The length of the received PDU in words.
"""
   for arg in sig.args:
      if arg.comment is not None:
         if arg.varlen:
            recv_proto_doc += """\
 *
 *   Non-parameter argument \\c %s:\n""" % (arg.name)
            recv_proto_doc += comment_format(arg.comment)
            recv_proto_doc += """\
 * This argument is variable length and is not retrieved by this function.
 * Use the offset
 * \\c CCP_%s_%s_WORD_OFFSET
 * to find the argument in the PDU and its length.
""" % (sig.name.upper(), arg.name.upper())
         else:
            recv_proto_doc += " *\n *   * \\c %s\n" % (arg.name)
            recv_proto_doc += comment_format(arg.comment)
   recv_proto_doc += """ *
 * \\return \\c TRUE if the PDU was successfully analysed, else \\c FALSE.
 */
"""
   if not recv_proto_done.has_key(sig.signature):
      prologue = "bool ccp_recv_pdu_" + sig.signature + "("
      len_indent = len(prologue)
      recv_proto = recv_proto_doc + prologue
      if len_indent > 40:
         len_indent = 8
         recv_proto += "\n" + (" " * len_indent)
      recv_proto += "const uint16 *pdu, uint16 pdu_len_words"
      sep = ",\n" + (" " * len_indent)
      for arg in sig.args:
         if arg.code[Signal.RECV][Signal.PROTO] is not None:
            recv_proto += sep + arg.code[Signal.RECV][Signal.PROTO]
      recv_proto += ")"
      sig.recv_proto = recv_proto
      proto += recv_proto + ";\n\n" + recv_proto_doc
      recv_proto_done[sig.signature] = 1
   else:
      recv_proto_done[sig.signature] += 1
      proto += recv_proto_doc

   # Use definition to hide common signature-based function
   proto += "#define ccp_recv_pdu_%s(" % (sig.name)
   recv_args = normal_args + 2
   if recv_args > 0:
      for i in range(recv_args):
         proto += "arg%d" % (i + 1)
         if i < recv_args - 1:
            proto += ", "
   proto += ") \\\n    ccp_recv_pdu_%s(" % (sig.signature)
   if recv_args > 0:
      for i in range(recv_args):
         proto += "arg%d" % (i + 1)
         if i < recv_args - 1:
            proto += ", "
   proto += ")\n"

   print proto
   print

print "#endif /* %s */" % prot_macro

sys.stdout = sys.__stdout__
fout.close()

# Record code signatures we've already seen.
# Just do this for send, which covers the test for receive, too.
code_done = { }

#
# Generate code.
#
for sig in Signal.signals:
   #
   # Send
   #
   if code_done.has_key(sig.signature):
      continue
   code_done[sig.signature] = 1

   fname = "ccp_send_pdu_" + sig.signature + ".c"
   fout = open(output_dir + "/" + fname, "w")
   sys.stdout = fout

   print """/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */"""
   print """/**
 * \\file
 */
/*
 * Autogenerated by %s
 * from %s""" % (prog, input_file)
   if send_proto_done[sig.signature] > 1:
      print """ *
 * This function is based on %s
 * but is used by other PDUs with the same signature.""" % (sig.pdu_name())
   print """ */

#include "%s"

%s
{""" % (private_header, sig.send_proto)

   if not sig.varlen:
      print "    uint16 pdu[%s];\n" % ( sig.word_size() )
   print "    pdu[CCP_HEADER_SIGNAL_ID_WORD_OFFSET] = ccp_signal_id;\n"
   args_done = 0
   for arg in sig.args:
      if arg.code[Signal.SEND][Signal.COPY] is not None:
         print arg.code[Signal.SEND][Signal.COPY]
         args_done = 1
   if args_done:
      print
   if sig.varlen:
      print "    return CCP_SEND(pdu, pdu_len_words, dest);"
   else:
      print "    return CCP_SEND(pdu, %s, dest);" % (
         sig.word_size())
   print "}"

   sys.stdout = sys.__stdout__
   fout.close()
   #
   # Receive
   #

   fname = "ccp_recv_pdu_" + sig.signature + ".c"
   fout = open(output_dir + "/" + fname, "w")
   sys.stdout = fout

   ncopies = 0
   for arg in sig.args:
      if arg.code[Signal.RECV][Signal.COPY] is not None:
         ncopies += 1
   if ncopies == 0:
      unused = "\n#else /* CCP_RECV_PANIC */\n    UNUSED(pdu);"
   else:
      unused = ""

   print """/* Copyright (c) 2016 Qualcomm Technologies International, Ltd. */
/*   Part of 6.2 */"""
   print """/**
 * \\file %s
 * Autogenerated by %s
 * from %s""" % (fname, prog, input_file)
   if recv_proto_done[sig.signature] > 1:
      print """ *
 * This function is based on %s
 * but is used by other PDUs with the same signature.""" % (sig.pdu_name())
   print """ */

#include "%s"

%s
{""" % (private_header, sig.recv_proto)
   if recv_proto_done[sig.signature] == 1:
      print """#ifdef CCP_RECV_PANIC
    if (pdu[CCP_HEADER_SIGNAL_ID_WORD_OFFSET] != %s)
    {
        panic_diatribe(PANIC_CCP_BAD_RECV_TYPE,
                       pdu[CCP_HEADER_SIGNAL_ID_WORD_OFFSET]);
    }%s
#endif /* CCP_RECV_PANIC */
""" % (sig.pdu_name(), unused)
   else:
      print """    /*
     * As this function is used to receive multiple PDUs, there
     * is no sanity check for the signal ID here.
     */"""
      if ncopies == 0:
         print "    UNUSED(pdu);"

   if sig.varlen:
      print "    if (pdu_len_words < %s)" % (
         word_offset(sig.name,sig.args[-1].name))
   else:
      print "    if (pdu_len_words != %s)" % (
         sig.word_size())
   print """    {
#ifdef CCP_SIZE_CHECKS
        fault_diatribe(FAULT_CCP_INVALID_RX_LENGTH, %s);
#endif /* CCP_SIZE_CHECKS */
        return FALSE;
    }
""" % (sig.pdu_name())
   for arg in sig.args:
      if arg.code[Signal.RECV][Signal.COPY] is not None:
         print arg.code[Signal.RECV][Signal.COPY]
   print """    return TRUE;
}"""

   sys.stdout = sys.__stdout__
   fout.close()
