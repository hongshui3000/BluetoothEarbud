# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
from xml.dom.minidom import parse
import datetime
import getopt
import os
import re
import sys
import types
try:
    from hashlib import sha256 as hash
except:
    from md5 import new as hash


# global variables
DEFAULT_WIDTH             = 80
# regular expresion search strings
CAMEL_CASE_SEARCH         = re.compile(r"([a-z])([A-Z])")
CAMEL_CASE_REPLACE_STRING = r"\1_\2"
P4_CHANGE_SEARCH          = re.compile(r".*Change: ([0-9]*) .*")
P4_CHANGE_REPLACE_STRING  = r"\1"
P4_FILE_SEARCH            = re.compile(r".*File: .*/([^/]*) .*")
P4_FILE_REPLACE_STRING    = r"\1"



def xmlTableStart(name, size, headerFields):
    dec  = '<Table font-size="normal" orient="portrait"\n'
    dec += '       anchor="%s.t">\n' % name
    dec += '  <Tgroup cols="%d" align="left" charoff="50">\n' % size
    for i in xrange(1, 1+size):
        dec += '    <Colspec colnum="%d" colname="col%d" colwidth="*"/>\n' % (i, i)
    dec += '    <Thead>\n'
    dec += '      <Row>\n'
    if len(headerFields) == size:
        for h in headerFields:
            dec += '        <Entry>\n'
            dec += '          <Paragraph>%s</Paragraph>\n' % h
            dec += '        </Entry>\n'
    else:
        if len(headerFields) > size:
            raise Exception("Table declaration wrong, more headers than columns.")
        offset = 1
        for h in headerFields[:-1]:
            dec += '        <Entry colname="col%d">\n' % offset
            dec += '          <Paragraph>%s</Paragraph>\n' % h
            dec += '        </Entry>\n'
            offset += 1
        if len(headerFields) != 0:
            dec += '        <Entry namest="col%d" nameend="col%d">\n' % (offset, size)
            dec += '          <Paragraph>%s</Paragraph>\n' % headerFields[-1]
            dec += '        </Entry>\n'

    dec += '      </Row>\n'
    dec += '    </Thead>\n'
    dec += '    <Tbody>\n'
    return dec

def xmlTableEnd(title):
    dec  = '    </Tbody>\n'
    dec += '    <Title>%s</Title>\n' % title
    dec += '  </Tgroup>\n'
    dec += '</Table>\n'
    return dec





#*******************************************************************************
# Declare class
#
# This class is used to declare symbols and store the results in a lookup table,
# which is the used to augment future declarations.
#
# For example when a typedef symbol is used, the type is checked against the
# previously declared symbols to see if it a more useful output can be generated
# for Kalimba code.
#
#*******************************************************************************
class Declare:
    """Class that manages symbol declaration. An object of this type is
    instantiated and passed to the main document class, which calls it to build
    symbols. It is also called by the slightly more complex symbols that include
    nested symbols, such as structures, unions and typedefs.

    If certain applications require special symbols, sub-class this class and
    extend the lookup table with references to the special symbols.

    Then declare an object of this new derived class and pass it when the
    document object is instantiated."""
    def __init__(self):
        """Instantiate a declare object. Builds the initial lookup table
        'self.lut'.

        If extra symbols are required sub-class this class and extend the table:
        'self.lut' in the derived constructor with the new symbols."""
        # just the list of class/construct types
        self.lut               = {}
        self.lut["struct"]     = structure
        self.lut["typedef"]    = typedef
        self.lut["define"]     = define
        self.lut["enum"]       = enum
        self.lut["enumEntry"]  = enumEntry
        self.lut["ifdef"]      = ifdef
        self.lut["ifndef"]     = ifndef
        self.lut["hashIf"]     = hashIf
        self.lut["hashElse"]   = hashElse
        self.lut["hashElif"]   = hashElif
        self.lut["endif"]      = endif
        self.lut["banner"]     = banner
        self.lut["general"]    = general
        self.lut["listDefine"] = listDefine
        self.lut["listEntry"]  = listEntry
        self.lut["listNumEls"] = listNumEls
        self.lut["union"]      = union

        # and the dictionary of all symbols we declare
        self.symbols = {}

    def __call__(self, node):
        """Declare a new symbol, pass in the parsed XML node representing the
        symbol. It is expected that complex symbol constructors will call this
        function recursively."""
        # we assume we know what this type is and raise and catch the key error
        # exception if we don't
        try:
            s = self.lut[node.tagName](node, self)
        except KeyError, e:
            raise DeclareError(e[0], node)

        # save this, for use with typedef's later
        self.symbols[s.getType()+s.getName()] = s

        return s

    def checkLookup(self, name):
        """Helper function to see if this symbol is actually one we have already
        defined."""
        if not self.symbols.has_key(name):
            # we don't care
            return None
        # is it one we really care about
        t = self.symbols[name].getType()
        if t == "typedef":
            t = self.symbols[name].getAliasType()
            if t == "general" or t == "struct" or t == "union":
                return self.symbols[name]

    def has_symbol(self, sym):
        """Check if the symbol is known in the list of previously defined
        symbols.

        Keys are of the form: typeName, e.g. for the following 'C' pseudo
        code:
          struct foo
          {
            ...
          }
        The key would be 'structfoo'. Subscript this object to return the
        symbol."""
        return self.symbols.has_key(sym)

    def __getitem__(self, sym):
        "Return a symbol"
        return self.symbols[sym]


#*******************************************************************************
# Error classes
#*******************************************************************************

# tag error
#  raised when a tag is not found in a node
class BaseException(Exception):
    "Base exception class from which the genApi specific exceptions are derived."
    def __init__(self, text):
        self.tagText = text
    def __call__(self):
        return self.tagText
    def __str__(self):
        return self.tagText
    def __repr__(self):
        return self.tagText

class TagError(BaseException):
    "Exception class when a symbol/node is missing a required tag, e.g. name."
    def __init__(self, tag, node):
        if type(tag) == types.StringType:
            text = "Missing key: %s\nin node: \n%s\nMissing key: %s" % (tag, node.toxml(),tag)
        else:
            text  = "Missing key, require one of: %s" % (", ".join("'%s'" % t for t in tag))
            text += "\n  in node:\n  %s" % (node.toxml())
        BaseException.__init__(self, text)

class NodeError(BaseException):
    "Exception class when a symbol/node is missing a required child node, e.g. members."
    def __init__(self, name, node):
        text = "Unable to find unique node for: %s\nin node: \n%s\nName: %s" %(name, node.toxml(), name)
        BaseException.__init__(self, text)

class InstanceError(BaseException):
    """Exception class when an instance name is provided when the symbol is not
    a child of a structure of union. This is illegal as the header files that
    are generated are for defining types not declaring objects."""
    def __init__(self, name, inst):
        text = "Instance entry but no parent in name: %s, inst: %s" %(name, inst)
        BaseException.__init__(self, text)

class FormatError(BaseException):
    """Exception class raised when a bad output format is requested from the script"""
    def __init__(self, format):
        text  = "Unrecognised format type: '%s'.\n" % (format)
        text += "Must be one of: 'k', 'x', 'm', 'i', 'p'"
        BaseException.__init__(self, text)

class ParentError(BaseException):
    """Exception class when an instance requires a parent, but isn't supplied
    one. For example, the general class must exist within a structure of union
    definition, not at the top level."""
    def __init__(self, sym):
        text  = "Node requires a parent, but doesn't have one.\n"
        text += "Node name: %s\n     type: %s\n"% (sym.getName(), sym.getType())
        BaseException.__init__(self, text)

class DeclareError(BaseException):
    def __init__(self, type, node):
        self.nodeType = type
        text = "Unknown type %s found in symbol:\n  '%s'" % (type, node.toxml())
        BaseException.__init__(self, text)

#*******************************************************************************
# tag helper functions
#*******************************************************************************

# get an optional tag
# calls getTag and catches the exception if the tag doesn't exist
def getOptionalTag(node, tag, option=""):
    """Utility function to search the current node for a specified tag returning
    the data contained within that tag. If the tag is not found a supplied value
    will be returned. This value defaults to the empty string."""
    try:
        return getTag(node, tag)
    except TagError:
        return option

# get a tag
# find an element with the supplied tag, raises an exception if not present
# only returns elements that are direct children of the supplied node
def getTag(node, tag):
    """Utility function to search the current node for a specified tag returning
    the data contained within that tag. If the tag is not found a 'TagError'
    exception is raised."""
    elements = node.getElementsByTagName(tag)
    if elements.length == 0:
        raise TagError(tag, node)
    # now iterate over the results looking for something a direct child of the
    # node
    for el in elements:
        if el.parentNode == node:
            # got something, does it have children
            if elements[0].hasChildNodes():
                if elements[0].getAttribute("type") == "xml":
                    return "".join(map(lambda n: n.toxml(), elements[0].childNodes))
                return elements[0].childNodes[0].data
            else:
                return ""
    # there are no direct children, complain
    raise TagError(tag, node)


# get an optional tag
# calls getTag and catches the exception if the tag doesn't exist
def getOptionalNode(node, name, option=None):
    """Utility function to search the current node for a specified child node
    returning the node. If the tag is not found a supplied value will be
    returned. This value defaults to None."""
    try:
        return getNode(node, name)
    except NodeError:
        return option

# get a node
# find an child element with the supplied name, raises an exception if not
# present only returns elements that are direct children of the supplied node
def getNode(node, name):
    """Utility function to search the current node for a specified child node
    returning the node. If the node is not found a 'NodeError' exception is
    raised."""
    out = filter( lambda n: n.nodeType == n.ELEMENT_NODE and n.tagName == name,
                  node.childNodes )
    if len(out) != 1:
        raise NodeError(name, node)
    return out[0]



#*******************************************************************************
# symbol classes
#*******************************************************************************

# comment class
# used to contain a comment, does not support the standard set of APIs, only
# supports the buildComment method
class comment:
    """Comment class, contains comments. It doesn't support the complete set of
    APIs, simply the build comment methods."""

    tagsUsed = []
    optsUsed = ["comment"]

    def __init__(self, node, declare):
        """Instantiate a comment object, arguments are simply the comment node
        from the XML document."""
        self.comment = getOptionalTag(node, "comment", [])
        if self.comment != []:
            self.comment = self.comment.replace("\n"," ")
            while self.comment.find("  ") != -1:
                self.comment = self.comment.replace("  ", " ")
            self.comment = self.comment.split(" ")

    def buildKalComment(self, indent="", width=DEFAULT_WIDTH):
        "Build a Kalimba comment."
        return self.__buildComment(indent, width, "/*", "*/")

    def buildCComment(self, indent="", width=DEFAULT_WIDTH):
        "Build a 'C' comment."
        return self.__buildComment(indent, width, "/*", "*/")

    def buildMatlabComment(self, indent="", width=DEFAULT_WIDTH):
        "Build a Matlab comment."
        return self.__buildComment(indent, width, "%", "")

    def buildPythonComment(self, indent="", width=DEFAULT_WIDTH):
        "Build a Python comment."
        return self.__buildComment(indent, width, "#", "")

    def __buildComment(self, indent, width, comStart, comEnd):
        """Internal utility function used by all external methods. This is
        actually the function that does all the work."""
        if len(self.comment) == 0:
            return ""
        # if no width is supplied just print the line
        if width == None:
            return indent+comStart+" "+self.comment+" "+comEnd
        # we need to do something a little more clever
        dec = ""
        line = indent+comStart
        for com in self.comment:
            # work out if this will push us over the limit, remember 1 space
            if (len(line)+len(com)+len(comEnd)+1) > width:
                line = ("%%-%ds%%s" % (width-len(comEnd))) % (line, comEnd)
                dec += line.rstrip()+"\n"
                line = indent+comStart
            line += " "+com
        line = ("%%-%ds%%s" % (width-len(comEnd))) % (line, comEnd)
        dec += line.rstrip()+"\n"
        return dec

# base class everyone inherits from
class symbol:
    """Base class for all symbols from the XML document, except comment, banner,
    general and all preprocessor symbols."""

    tagsUsed = ["name", "info"]
    optsUsed = []

    def __init__(self, node, declare, type, prettyType=""):
        """Instantiate a base symbol object. Stores the basic information and
        generates the hash used as the html anchor link.

        When initialised the derived class provides its type and its 'pretty
        type', the latter defaults to the type if not provided. The pretty type
        is only used for the HTML information output."""
        self.name       = getTag(node, "name")
        self.info       = getTag(node, "info")
        self.comment    = comment(node, declare)
        self.type       = type
        self.prettyType = prettyType
        if prettyType == "":
            self.prettyType = type
        m = hash()
        m.update(self.name)
        m.update(self.info)
        m.update(self.type)
        self.link    = "a"+m.hexdigest()

    def getType(self):
        "Return the type."
        return self.type

    def getPrettyType(self):
        "Return the 'pretty type'."
        return self.prettyType

    def getName(self):
        "Return the name."
        return self.name

    def setName(self, name):
        "Change the name."
        self.name = name

    def getLink(self):
        "Get the HTML link."
        return self.link

    def getSize(self):
        """Calculate/return the size of the symbol in words, defaults to 1. This
        is typically used by Kaimba routines to generate the STRUC_SIZE field.

        This method is expected to be overloaded by more complex symbols."""
        return 1

    def getValue(self):
        "Get the value"
        return self.value

    def getNumGenerations(self):
        "Get the number of generations below us, assumed to be 0."
        return 0

    def getNumElements(self):
        """Get the number of elements from here down. This is only used by the
        html generation methods."""
        return 1

    def setValue(self, value):
        "Set the value"
        self.value = value

    def buildKalDeclaration(self):
        """Build the Kaimba output for the base symbol, the base class does not
        implement this. If the derived class does not, the base class will raise
        an exception."""
        raise Exception("Unimplemented function in symbol: "+self.name)

    def buildCDeclaration(self):
        """Build the 'C' output for the base symbol, the base class does not
        implement this. If the derived class does not, the base class will raise
        an exception."""
        raise Exception("Unimplemented function in symbol: "+self.name)

    def buildMatlabDeclaration(self):
        """Build the Matlab output for the base symbol, the base class does not
        implement this. If the derived class does not, the base class will raise
        an exception."""
        raise Exception("Unimplemented function in symbol: "+self.name)

    def buildPythonDeclaration(self):
        """Build the Python output for the base symbol, the base class does not
        implement this. If the derived class does not, the base class will raise
        an exception."""
        raise Exception("Unimplemented function in symbol: "+self.name)

    def buildSimpleXmlDeclaration(self):
        """Build the simple documentation output for the basesymbol. Unlike the
        other output formats, the base class does implement this method.

        Also, unlike the other output formats, the documentation output is a two
        stage process. First it uses the simple documentation declaration and
        then run the full documentation declaration method."""
        if self.info == "":
            raise Exception("Info block empty in symbol: "+self.name)
        # buid some html to show
        dec  = '<Row>\n'
        dec += '  <Entry>\n'
        dec += '    <Paragraph>'+self.name+'</Paragraph>\n'
        dec += '  </Entry>\n'
        dec += '  <Entry>\n'
        dec += '    <Paragraph>'+self.getPrettyType()+'</Paragraph>\n'
        dec += '  </Entry>\n'
        dec += '  <Entry>\n'
        dec += '    <Paragraph>'+self.info+'</Paragraph>\n'
        dec += '  </Entry>\n'
        dec += '</Row>\n'
        return dec

    def buildFullXmlDeclaration(self, title=0, offset=1, size=None):
        """Build the full documentation output for the base symbol. Unlike the
        other output formats, the base class does implement this method.

        This method is different from all other output methods, because it has
        an option flag: 'title'. This flag specifies whether the symbol is the
        main "parent" class or is a sub-class within a parent symbol. This flag
        defaults to off, and is only set by the top level calling function, the
        'writeXmlOutput' method of the document class.

        Also, unlike the other output formats, the documentation output is a two
        stage process. First it uses the simple documentation declaration and
        then run the full documentation declaration method."""

        if title:
            dec  = '<Chapter anchor="'+self.link+'.0">\n'
            dec += '<!--title: %s (%d)-->\n' % (self.name, self.getNumGenerations())
            dec += '  <Title>' + self.name + '</Title>\n'
            dec += '  <Description anchor="'+self.link+'.1">\n'
            dec += '    <Paragraph>' + self.info + '</Paragraph>\n'
            dec += '  </Description>\n'
            dec += '</Chapter>\n\n\n'
        else:
            dec  = '<Row>\n'
            dec += '<!--child: %s (%d)-->\n' % (self.name, self.getNumGenerations())
            for i in range(1, offset):
                dec += '  <Entry colname="col%d" rowsep="0">\n' % i
                dec += '    <Paragraph></Paragraph>\n  </Entry>\n'
            dec += '  <Entry colname="col%d">\n' % offset
            dec += '    <Paragraph>' + self.name + '</Paragraph>\n'
            dec += '  </Entry>\n'
            # move offset on
            offset += 1
            if offset == size:
                dec += '  <Entry colname="col%d">\n' % offset
            else:
                dec += '  <Entry namest="col%d" nameend="col%d">\n' % (offset, size)
            dec += '    <Paragraph>' + self.info + '</Paragraph>\n'
            dec += '  </Entry>\n'
            dec += '</Row>\n'

        return dec


    def buildSimpleInfoDeclaration(self):
        """Build the simple information output for the basesymbol. Unlike the
        other output formats, the base class does implement this method.

        Also, unlike the other output formats, the HTML information output is a
        two stage process. First it uses the simple information declaration and
        then run the full information declaration method."""
        if self.info == "":
            raise Exception("Info block empty in symbol: "+self.name)
        # buid some html to show
        dec  = "  <tr>\n"
        dec += "    <td><a href=\"#"+self.link+"\">"+self.name+"</a></td>\n"
        dec += "    <td>"+self.getPrettyType()
        dec += "</td>\n"
        dec += "    <td>"+self.info+"</td>\n"
        dec += "  </tr>\n"
        return dec

    def buildFullInfoDeclaration(self, title=0, depth=0):
        """Build the full information output for the base symbol. Unlike the
        other output formats, the base class does implement this method.

        This method is different from all other output methods, because it has
        an option flag: 'title'. This flag specifies whether the symbol is the
        main "parent" class or is a sub-class within a parent symbol. This flag
        defaults to off, and is only set by the top level calling function, the
        'writeInfoOutput' method of the document class.

        Also, unlike the other output formats, the HTML information output is a
        two stage process. First it uses the simple information declaration and
        then run the full information declaration method."""
        if title:
            dec = "<a class=\"anchor\" name=\""+self.link+"\"></a><h2><tt>"+self.name+"</tt></h2>"
            dec += "<p>"+self.info+"</p>\n"
        else:
            dec  = "<tr>\n  <td><b>"+self.name+"</b></td>\n"
            if depth == 0:
                dec += "  <td>"+self.info+"</td>\n</tr>\n"
            else:
                dec += "  <td colspan=%d>%s</td>\n</tr>\n" % (depth+1, self.info)
        if title:
            dec += "<hr>\n"
        return dec


class preproc:
    """Base class for all pre-processor symbols from the XML document.

    The main difference, between this and the 'symbol' class is that most
    attributes are optional. The pre-processor flags are normally fairly simple
    and as such don't need a full symbol definition."""

    tagsUsed = []
    optsUsed = ["name", "value", "test"]

    def __init__(self, node, declare, type, command):
        # defines are all quite different so make everything optional
        self.name    = getOptionalTag(node, "name")
        self.value   = getOptionalTag(node, "value")
        self.test    = getOptionalTag(node, "test")
        self.comment = comment(node, declare)
        # save the type and the command
        self.type    = type
        self.command = command
        # assume the default string is nothing
        self.string  = ""

    def getType(self):
        "Return the type."
        return self.type

    def getName(self):
        "Return the name."
        return self.name

    def getSize(self):
        "Return the size, which is always zero for preprocessor symbols."
        return 0

    def getNumGenerations(self):
        "Get the number of generations below us, always 0."
        return 0

    def getNumElements(self):
        """Get the number of elements from here down. This is only used by the
        html generation methods and as preproc symbols don't appear in the html
        output we return 0."""
        return 0

    def buildKalDeclaration(self, indent="", parent="", **kwargs):
        """Build the Kalimba output, one of the only two outputs for this
        symbol along with the 'C' output."""
        dec  = self.comment.buildKalComment(indent)
        dec += indent + self.command + self.string
        return dec

    def buildCDeclaration(self, indent="", parent="", **kwargs):
        """Build the 'C' output, one of the only two outputs for this
        symbol along with the Kalimba output."""
        dec  = self.comment.buildCComment(indent)
        dec += indent + self.command + self.string
        return dec


    def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
        """Build the Matlab output for this preprocessor symbol.

        Unlike the symbol class, this assumes that derived classes do not want
        to generate any output. So rather than raise an exception, the base
        class will return an empty string."""
        return ""
    def buildPythonDeclaration(self, indent="", parent="", **kwargs):
        """Build the Python output for this preprocessor symbol.

        Unlike the symbol class, this assumes that derived classes do not want
        to generate any output. So rather than raise an exception, the base
        class will return an empty string."""
        return ""

    def buildSimpleXmlDeclaration(self):
        return ""

    def buildFullXmlDeclaration(self, title=0, offset=0, size=None):
        return ""

    def buildSimpleInfoDeclaration(self):
        """Build the simple information output for this preprocessor symbol.

        Unlike the symbol class, this assumes that derived classes do not want
        to generate any output. So rather than raise an exception, the base
        class will return an empty string."""
        return ""
    def buildFullInfoDeclaration(self, title=0, depth=0):
        """Build the full information output for this preprocessor symbol.

        Unlike the symbol class, this assumes that derived classes do not want
        to generate any output. So rather than raise an exception, the base
        class will return an empty string."""
        return ""



# general class
# class for standard symbols, i.e. not structures or typedefs
class general(symbol):
    """General symbol class, derived from the base symbol class. This is the
    simplest derived class and used by symbols such as 'uint'."""

    tagsUsed = ["subtype"]
    optsUsed = ["size"]

    def __init__(self, node, declare):
        """Instantiate a general symbol object, fairly simple class. The only
        'smarts' are to check if the type is built-in type or one we have
        already declared, e.g. 'uint foo' vs. 'struct myStruct foo'."""
        symbol.__init__(self, node, declare, "general", "General")
        # get the stuff that must be present
        self.subtype = getTag(node, "subtype")
        # check if this is a symbol we already know of
        self.subsym = declare.checkLookup(self.subtype)
        # is there a size
        self.size = getOptionalTag(node, "size", "0")
        self.size = int(self.size)
        if self.size != 0:
            self.subsym = None

    def getSize(self):
        """Return the size of the symbol. If its a built-in symbol
        (i.e. unknown), the size defaults to one. If its a symbol we have
        defined, it's size is returned instead."""
        if self.subsym == None:
            if self.size == 0:
                return 1
            else:
                return self.size
        else:
            if self.size == 0:
                return self.subsym.getSize()
            else:
                return self.size * self.subsym.getSize()

    def buildKalDeclaration(self, indent="", parent="", **kwargs):
        """Build the Kalimba output, if this symbol is infact a symbol already
        defined, we use its buildKalDeclaration method.

        When its a builtin symbol, this function builds the complete Kalimba
        output."""
        # if we don't have a parent we need to die
        if parent == "":
            raise ParentError(self)

        if self.subsym == None:
            # have we got an offset
            offset=0
            if kwargs.has_key("offset"):
                offset = kwargs["offset"]
            # change camel case to underscores
            name = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,self.name).upper()
            # put the field on the end
            name += "_FIELD"
            # print the comment
            dec = self.comment.buildKalComment(indent)
            # this is in a structure, declare with the parent name, change camel
            # case to underscores
            parent = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,parent).lower()
            name = parent+"."+name
            dec += indent+".CONST $%-60s    %d;" % (name, offset)
            return dec
        else:
            return self.subsym.buildKalDeclaration(indent, parent+"."+self.getName(), **kwargs)

    def buildCDeclaration(self, indent="", parent=None, **kwargs):
        """Build the 'C' output, this is fairly simple, as the required output is simple."""
        # if we don't have a parent we need to die
        if parent == None:
            raise ParentError(self)

        dec = self.comment.buildCComment(indent)
        if self.size == 0:
            return dec+indent+"%s %s;" % (self.subtype, self.name)
        else:
            return dec+indent+"%s %s[%d];" % (self.subtype, self.name, self.size)

    def buildMatlabDeclaration(self, indent="", parent=None, **kwargs):
        """Build the Matlab output for this symbol."""
        if parent == None:
            raise ParentError(self)

        # have we got an offset
        offset=0
        if kwargs.has_key("offset"):
            offset = kwargs["offset"]
        # change camel case to underscores
        name   = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,self.name).upper()
        # print the comment
        dec = self.comment.buildMatlabComment(indent)
        # this is in a structure, declare with the parent name
        # change camel case to underscores
        parent = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,parent).lower()
        fullname = parent+"."+name

        dec += indent+"%-60s = %d;\n" % (fullname, offset)
        dec += indent+"%-60s = %d;\n" % (parent+".rev{end+1, 1}", offset)
        dec += indent+"%-60s = '%s';" % (parent+".rev{end,   2}", name)
        return dec

    def buildPythonDeclaration(self, indent="", parent=None, **kwargs):
        """Build the Python output for this symbol."""
        if parent == None:
            raise ParentError(self)

        # have we got an offset
        offset=0
        if kwargs.has_key("offset"):
            offset = kwargs["offset"]
        # change camel case to underscores
        name   = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,self.name).upper()
        # print the comment
        dec = self.comment.buildPythonComment(indent)
        # this is in a structure, declare with the parent name
        # change camel case to underscores
        parent = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,parent).lower()
        dec += "%s%s['%s'] = %d"%(indent, parent, name, offset)
        return dec



# typedef class
# class to hold typedefs
class typedef(symbol):
    """Class representing typedefs."""

    tagsUsed = ["alias", "members"]
    optsUsed = []

    class value:
        """Utility class used by typedef only."""
        tagsUsed = []
        optsUsed = []

        def __init__(self, value):
            """Instantiate a value object, just saves the value of the
            symbol."""
            self.value = value

        def getName(self):
            """Return the name, implemented to look like everything else
            only."""
            return ""

        def getValue(self):
            "Return the value, the only thing we this stores."
            return self.value

        def getSize(self):
            """Return the size, we assume its only 1 word, we can't assume
            anything else."""
            return 1

        def getType(self):
            """Return the type, implemented to look like everything else
            only."""
            return "value"

        def getNumGenerations(self):
            "We have no descendants."
            return 0

        def getNumElements(self):
            """Get the number of elements from here down. This is only used by the
            html generation methods."""
            return 1

        def buildKalDeclaration(self, indent="", parent="", **kwargs):
            """If the typedef is effectively an alias, then Kalimba doesn't care
            about this symbol at all."""
            return ""

        def buildCDeclaration(self, indent="", parent="", **kwargs):
            "Just return the exact string we were given."
            return self.value

        def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
            """If the typedef is effectively an alias, then matlab doesn't care
            about this symbol at all."""
            return ""

        def buildPythonDeclaration(self, indent="", parent="", **kwargs):
            """If the typedef is effectively an alias, then python doesn't care
            about this symbol at all."""
            return ""

    def __init__(self, node, declare):
        """Instantiate a typedef symbol. It differentiates between typedefs of
        predefined or built-in symbols versus anonymous symbols defined on the fly.

        In the latter case it creates the anonymous symbol as well."""
        symbol.__init__(self, node, declare, "typedef")
        # say we don't alias a structure
        self.sym = None
        # now work out what the define is
        define = getOptionalNode(node, "alias")
        syms   = getOptionalNode(node, "members")
        if define is None and syms is None:
            raise TagError(["alias", "members"], node)
        if syms is None:
            # this is just a value
            self.define = self.value(define.childNodes[0].data)
        else:
            s = filter(lambda n: n.nodeType == n.ELEMENT_NODE, syms.childNodes)
            if len(s) != 1:
                raise Exception("Multiple symbols in typedef declaration: %s" % self.name)
            # this is the structure
            self.define = declare(s[0])

    def getPrettyType(self):
        """Utility function used when printing the simple information output."""
        s = self.sym
        if self.sym == None:
            s = self.define
        return "Typedef (alias type: %s)" % s.getType()

    def setStruct(self, sym):
        """As typedef's can alias symbols we have defined, we check these after
        this symbol has been defined and make a reference. This allows outputs
        such as Kalimba, matlab and python, to generate full outputs for this
        typedef."""
        self.sym = sym

    def getDefineValue(self):
        "Return the value of the define (aliased symbol)."
        return self.define.getValue()

    def getSize(self):
        """Get the size of the symbol, calling the getSize method on the define
        or symbol as appropriate."""
        if self.sym != None:
            return self.sym.getSize()
        return self.define.getSize()

    def getAliasType(self):
        """Get the type of the aliased symbol, again the using the type method
        of the define or symbol as appropriate."""
        if self.sym != None:
            return self.sym.getType()
        return self.define.getType()

    def getNumElements(self):
        """Get the number of elements from here down. This is only used by the
        html generation methods."""
        raise Exception("Didn't expect this to get called.")

    def buildKalDeclaration(self, indent="", parent="", **kwargs):
        """Build the Kalimba output for typedefs. This function calls the output
        methods on the children symbol, to build the complete output."""
        dec = self.comment.buildKalComment(indent)
        if self.sym != None:
            # if the symbol is something that just tidies the name up, e.g.
            #     typedef struct foo foo;
            # then do nothing
            child = self.sym.getName().split()
            if child[-1] == self.name:
                return ""
            return dec+self.sym.buildKalDeclaration(indent, parent, typedefName=self.name, **kwargs)
        else:
            if kwargs.has_key("typedefName"):
                kwargs.pop("typedefName")
            return dec+self.define.buildKalDeclaration(indent, parent, typedefName=self.name, **kwargs)

    def buildCDeclaration(self, indent="", parent="", **kwargs):
        """Build the 'C' output for typedefs, again calling the output method on
        the child symbol."""
        dec  = self.comment.buildCComment("")
        dec += "typedef "
        if self.define.getType() == "value":
            dec += self.define.buildCDeclaration(parent="")
        else:
            if self.sym == None:
                dec += self.define.buildCDeclaration(parent="")
            else:
                dec += self.sym.buildCDeclaration(parent="")
        # this is another horrible work around, but replace the last character
        # (a semi-colon) with a space
        if self.define.getType() != "value":
            dec = dec[:-1]
        dec += " "+self.name+";"
        return dec

    def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
        """Build the matlab output, this basically, calls the same method on the
        child symbol, aliasing it to the typedef name."""
        dec = self.comment.buildMatlabComment(indent)
        if self.sym != None:
            return dec+self.sym.buildMatlabDeclaration(indent, "", typedefName=self.name)
        else:
            return dec+self.define.buildMatlabDeclaration(indent, "", typedefName=self.name)

    def buildPythonDeclaration(self, indent="", parent="", **kwargs):
        """Build the python output, this basically, calls the same method on the
        child symbol, aliasing it to the typedef name."""
        dec = self.comment.buildPythonComment(indent)
        if self.sym != None:
            return dec+self.sym.buildPythonDeclaration(indent, "", typedefName=self.name)
        else:
            return dec+self.define.buildPythonDeclaration(indent, "", typedefName=self.name)


    def buildFullXmlDeclaration(self, title, offset=1, size=None):
        """Build the full documentation output for typedefs, calls the same
        method on the child symbol."""

        if title == 0:
            raise Exception("Typedef needs to be a title element")
        if size is None:
            size = self.getNumGenerations() + 1

        dec  = '<Chapter anchor="'+self.link+'.0">\n'
        dec += '  <!--typedef: %s (%d)-->\n' % (self.name, self.getNumGenerations())
        dec += '  <Title><Code>typedef</Code> ' + self.name + '</Title>\n'
        dec += '  <Description anchor="'+self.link+'.1">\n'
        dec += '    <Paragraph>%s</Paragraph>\n' % self.info

        # if this aliases another symbol we know about, just link to that
        if self.sym != None:
            dec += '    <Paragraph>\n'
            # should be able to use the Ref element here, but seems to be
            # broken, so use RefShort to get a URL and then add the name
            # ourselves
            dec += '      Aliases: <RefShort target="%s.0"/> ' % (self.sym.getLink())
            dec += '<Code>%s %s</Code>.\n' % (self.sym.getType(), self.sym.getName())
            dec += '    </Paragraph>\n'
        else:
            # this may be an anonymous symbol, in which case print that out properly
            if self.define.getType() == "value":
                dec += '    <Paragraph>\n'
                dec += '      Aliases: ' + self.define.getValue() + '\n'
                dec += '    </Paragraph>\n'
            elif self.define.getType() in ["struct", "enum", "union"]:
                dec += self.define.buildTypedefXmlDeclaration(offset=offset, size=size)
            else:
                dec += '    <Paragraph>\n'
                dec += '      Aliases: ' + self.define.getType() + " " + self.define.getName() + "\n"
                dec += '    </Paragraph>\n'

        dec += '  </Description>\n'
        dec += '</Chapter>\n\n\n'
        return dec

    def buildFullInfoDeclaration(self, title, depth=0):
        """Build the full information output for typedefs, calls the same method
        on the child symbol."""
        # if this is the title make it look like it
        if title:
            dec  = "<a class=\"anchor\" name=\""+self.link+"\"></a><h2><tt>typedef "+self.name+"</tt></h2>"
        else:
            dec  = "<a class=\"anchor\" name=\""+self.link+"\"></a><p>typedef "+self.name+"</p>\n"
        # iclude any information about this field
        dec += "<p>"+self.info+"</p>\n"
        # if this aliases another symbol we know about, just link to that
        if self.sym != None:
            dec += "<p>Aliases: <a href=\"#"+self.sym.getLink()+"\">"+self.sym.getName()+"</a></p>\n"
        else:
            # this may be an anonymous symbol, in which case print that out properly
            if self.define.getType() == "value":
                dec += "<p>Aliases: "+self.define.getValue()+"</p>\n"
            elif self.define.getType() in ["struct", "enum", "union"]:
                dec += self.define.buildTypedefInfoDeclaration()
            else:
                dec += "<p>Aliases: "+self.define.getType()+" "+self.define.getName()+"</p>\n"
        if title:
            dec += "<hr>\n"
        return dec

# container class
# base class for structures and unions
class container(symbol):
    """Base class for structures and unions, like the symbol and preproc base
    classes this should never be used directly, use one of the derived
    classes."""

    tagsUsed = ["members"]
    optsUsed = ["instance"]

    def __init__(self, node, declare, type, prettyType):
        """Instantiate a container object, descending through each child
        symbol."""
        symbol.__init__(self, node, declare, type, prettyType)
        self.inst = getOptionalTag(node, "instance")
        self.members = []
        # at this point we want the members node, but the function below returns
        # all nodes of type 'members' even if they are not a direct descendant
        # of this node
        #
        # hence we search through looking for the one that has the correct
        # parent
        members = getNode(node, "members")

        for mem in filter(lambda n: n.nodeType == n.ELEMENT_NODE, members.childNodes):
            self.members.append(declare(mem))

    def getSize(self):
        """Overload the base getSize method by summing the size of children to
        determine the overall size of this container object.

        This base class does not implement this method as the two derived
        classes require different implementations, so we just raise an exception
        if they haven't."""
        raise Exception("Unimplemented function in symbol: "+self.name)

    def updateOffset(self):
        """Calculate the new offset after defining an element. This is one area
        where sturctures and unions are different, because unions overlapp
        symbols, the offset does not get updated, unlike structueres where it
        does.

        Like getSize this base class does not implement this method as the two
        derived classes require different implementations, so we just raise an
        exception if they haven't."""
        raise Exception("Unimplemented function in symbol: "+self.name)

    def getNumGenerations(self):
        """Get the number of generations below this container object and add 1
        for ourself."""
        return 1 + max(m.getNumGenerations() for m in self.members)

    def getNumElements(self):
        """Get the number of elements from here down. This is only used by the
        html generation methods.

        For containers, we have to ask all children and sum the results, adding
        one for ourself."""
        return 1 + sum(m.getNumElements() for m in self.members)

    def buildKalDeclaration(self, indent="", parent="", **kwargs):
        """Build the Kalimba output for containers, embedding each child
        member's output, finally adding the 'STRUC_SIZE' field for the overall
        symbol."""
        # should we be using the typedef name of this symbol's name
        if parent == "":
            if self.inst != "":
                raise InstanceError(self.name, self.inst)
            typedefName = self.name
            if kwargs.has_key("typedefName"):
                typedefName = kwargs["typedefName"]
        else:
            typedefName = parent
            if self.inst != "":
                typedefName += "."+self.inst
            elif self.name != "":
                typedefName += "."+self.name
        # make it look pretty
        typedefName = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,typedefName)
        # have we been given an offset
        off = 0
        if kwargs.has_key("offset"):
            off = kwargs["offset"]
        # initialise the output and the offset count
        dec  = indent+"// %s\n" % (typedefName.upper())
        dec += self.comment.buildKalComment(indent)

        for mem in self.members:
            # ask the member to make it's output
            dec += mem.buildKalDeclaration(indent+"  ", typedefName, offset=off)+"\n"
            off  = self.updateOffset(off, mem)

        # put the STRUC_SIZE field in
        if parent == "":
            dec += "%s.CONST %-60s     %d;\n" % (indent, "$"+typedefName.lower()+".STRUC_SIZE", off)
        # again this is a bit naf, but strip off the last new line
        #dec = dec[:-1]
        return dec

    def buildCDeclaration(self, indent="", parent="", **kwargs):
        """Build the 'C' output for containers, embedding each child member's
        output."""
        dec  = self.comment.buildCComment(indent)
        name = " "+self.name
        name = name.rstrip()
        dec += "%s%s%s\n%s{\n" % (indent, self.type, name, indent)
        parentToUse = name
        if name == "":
            parentToUse = self.inst
        for mem in self.members:
            dec += mem.buildCDeclaration(indent+"  ", parent=parentToUse)+"\n"
        dec += indent+"}"
        if len(self.inst) != 0:
            dec += " "+self.inst
        dec += ";"
        return dec

    def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
        """Build the Matlab output for containers. Builds a dictionary of names
        to values (offsets) and a reverse dictionary of offsets to values.

        The variable name defaults to Baton, but can be defined by setting the
        'matlabRoot' attribute of the top level object in the XML document."""
        # is there a root
        try:
            root = kwargs["root"]
        except KeyError:
            root = "BATON"

        # should we be using the typedef name of the symbol's name
        typedefName = self.name
        if self.inst != "":
            typedefName = self.inst
        elif kwargs.has_key("typedefName"):
            typedefName = kwargs["typedefName"]
        # do we have a parent
        if len(parent) != 0:
            typedefName = parent+"."+typedefName
        else:
            typedefName = root + "." + typedefName
        typedefName = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,typedefName)

        # have we been given an offset
        off = 0
        if kwargs.has_key("offset"):
            off = kwargs["offset"]

        # initialise the output and the offset count
        dec  = indent+"%% %s\n" % (typedefName.upper())
        dec += self.comment.buildMatlabComment(indent)
        dec += indent+"%-60s = {};\n" % (typedefName.lower()+".rev")
        for mem in self.members:
            # ask the member to make it's output
            dec += mem.buildMatlabDeclaration(indent+"   ", typedefName, offset=off)+"\n"
            off  = self.updateOffset(off, mem)
        # again this is a bit naf, but strip off the last new line
        dec = dec[:-1]
        return dec

    def buildPythonDeclaration(self, indent="", parent="", **kwargs):
        """Build the python output for containers. Builds a dictionary of names
        to values (offsets). A non-standard dictionary is used that supports
        forward and reverse look ups."""
        # should we be using the typedef name of the container's name
        typedefName = self.name
        if self.inst != "":
            typedefName = self.inst
        elif kwargs.has_key("typedefName"):
            typedefName = kwargs["typedefName"]
        # do we have a parent
        if len(parent) != 0:
            typedefName = parent+"."+typedefName
        typedefName = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,typedefName)

        # have we been given an offset
        off = 0
        if kwargs.has_key("offset"):
            off = kwargs["offset"]

        # initialise the output and the offset count
        dec  = indent+"# %s\n" % (typedefName.upper())
        dec += self.comment.buildPythonComment(indent)
        dec += indent+"%s = biDirDict()\n" % (typedefName.lower())
        for mem in self.members:
            # ask the member to make it's output
            dec += mem.buildPythonDeclaration(indent, typedefName, offset=off)+"\n"
            off  = self.updateOffset(off, mem)
        # again this is a bit naf, but strip off the last new line
        dec = dec[:-1]
        return dec


    def buildTypedefXmlDeclaration(self, offset, size):
        """Special method used when a container is used in a typedef.
        Effectively replaces the full information method for the parent typedef
        symbol."""
        dec  = '<Paragraph>\n'
        if len(self.name) == 0:
            dec += '  Aliases an anonymous %s.\n' % (self.prettyType)
        else:
            dec += '  Aliases a %s, called: "%s".\n' % (self.prettyType, self.name)
        dec += '</Paragraph>\n'
        dec  = '<Paragraph>' + self.info + '</Paragraph>\n'

        dec += xmlTableStart("CommunicationsStructure", size, ['Field', 'Initial Value', 'Description'])

        for mem in self.members:
            dec += mem.buildFullXmlDeclaration(offset=offset, size=size)

        dec += xmlTableEnd(self.name)

        return dec

    def buildTypedefInfoDeclaration(self):
        """Special method used when a container is used in a typedef.
        Effectively replaces the full information method for the parent typedef
        symbol."""
        if len(self.name) == 0:
            dec  = "<p>Aliases an anonymous %s.</p>\n" % (self.prettyType)
        else:
            dec  = "<p>Aliases a %s, called: '%s'.</p>\n" % (self.prettyType, self.name)
        dec += "<p>"+self.info+"</p>\n"
        dec += "<table border=1 cellpadding=5>\n"
        for mem in self.members:
            dec += mem.buildFullInfoDeclaration()
        dec += "</table>\n"
        return dec

    def buildFullXmlDeclaration(self, title=0, offset=0, size=None):
        """Build the full information output for container symbols. Calls the
        same method on each child element."""
        # prepare some variables, what's the depth below us
        if size == None:
            size = self.getNumGenerations() + 1
        # build the name
        if self.name == "":
            name = "anonymous %s" % (self.prettyType)
            if self.inst != "":
                name += " instance %s" % (self.inst)
        else:
            name = "%s %s" % (self.prettyType, self.name)

        # if we're the title, start the chapter
        if title:
            dec  = '<Chapter anchor="'+self.link+'.0">\n'
            dec += '  <!--container: %s (%d)-->\n' % (self.name, self.getNumGenerations())
            dec += '  <Title><Code>%s</Code> %s</Title>\n' % (self.prettyType, self.name)
            dec += '  <Description anchor="%s.1">\n' % self.link
        else:
            dec  = '  <Row>\n'
            for i in range(1, offset):
                dec += '  <Entry colname="col%d" rowsep="0">\n' % i
                dec += '    <Paragraph></Paragraph>\n  </Entry>\n'
            dec += '    <Entry colname="col%d" morerows="%d">\n' % (offset, self.getNumElements()-1)
            dec += '      <Paragraph>\n'
            dec += '        <Emphasis>%s</Emphasis>\n' % name
            dec += '      </Paragraph>\n'
            dec += '    </Entry>\n'
            dec += '    <Entry namest="col%d" nameend="col%d">\n' % (offset+1, size)

        dec += '    <Paragraph>%s</Paragraph>\n' % self.info
        dec += '    <Paragraph>Contains the following members:</Paragraph>\n'

        if title:
            dec += xmlTableStart("CommunicationsStructure.%s" % self.name, size, [])
        else:
            dec += '    </Entry>\n'
            dec += '  </Row>\n'

        for mem in self.members:
            dec += mem.buildFullXmlDeclaration(offset=offset+1, size=size)

        if title:
            dec += xmlTableEnd(name)
            dec += '  </Description>\n'
            dec += '</Chapter>\n'
        return dec

    def buildFullInfoDeclaration(self, title=0, depth=None):
        """Build the full information output for container symbols. Calls the
        same method on each child element."""
        # what's the depth below us
        if depth == None:
            depth = self.getNumGenerations()

        # if we're the title, then make the link a title.
        if title:
            dec  = "<a class=\"anchor\" name=\"%s\"></a><h2><tt>%s %s</tt></h2>" % (self.link, self.type, self.name)
        else:
            # we're not the title, which means we're nested in the parent's
            # table, so make our output fit in with that
            dec  = "<tr>\n<td rowspan=%d valign=top>\n" % self.getNumElements()
            # what is our name? combination of name, instance or nothing
            if self.name == "":
                name = "anonymous %s" % (self.prettyType)
                if self.inst != "":
                    name += "<br>instance %s" % (self.inst)
            else:
                name = "%s %s" % (self.prettyType, self.name)
            dec += "<a class=\"anchor\" name=\"%s\"></a><p><b>%s</b></p>\n" % (self.link, name)
            # the info bumpf needs to span the columns below
            dec += "</td><td colspan=%d>\n" % (depth+1)

        dec += "<p>"+self.info+"</p>\n"
        dec += "<p>Contains the following members:</p>\n"

        if title:
            dec += "<table border=1 cellpadding=5>\n"

        for mem in self.members:
            dec += mem.buildFullInfoDeclaration(depth=depth-1)

        if title:
            dec += "</table>\n"
            dec += "<hr>\n"
        else:
            dec += "</td>\n</tr>\n"
        return dec


# structure class
# class for structures
class structure(container):
    """Class representing structures, most of the functionality is actually
    implemented in the container base class."""

    tagsUsed = []
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate a structure object using the container base class'
        constructor."""
        container.__init__(self, node, declare, "struct", "Structure")

    def getSize(self):
        """Overload the base getSize method by summing the size of children to
        determine the overall size."""
        return sum(m.getSize() for m in self.members)

    def updateOffset(self, off, mem):
        """Update the offset after generating a child symbol. Because structures
        append the children symbols the offset simply increments by the child's
        size."""
        return off + mem.getSize()



# union class
# holds union constructs
class union(container):
    """Class representing unions, most of the functionality is actually
    implemented in the container base class."""

    tagsUsed = []
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate a union object using the container base class'
        constructor."""
        container.__init__(self, node, declare, "union", "Union")

    def getSize(self):
        """Overload the base getSize method by finding the maximum size of all
        the children to determine the overall size."""
        return reduce(max, [m.getSize() for m in self.members])

    def updateOffset(self, off, mem):
        """Update the offset after generating a child symbol. Because unions
        overlay the children symbols the offset does not get updated."""
        return off

# enum entry class
# holds enum entries, should only occur within enum lists
class enumEntry(symbol):
    """Class for enumeration entries. Should only be used as a child of an enum
    object."""
    tagsUsed = ["value"]
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate an enumeration entry object."""
        symbol.__init__(self, node, declare, "enumEntry", "Enumeration Entry")
        self.value   = getTag(node, "value")
        # if there was a value then convert to a number
        if self.value == "":
            self.value = None
        else:
            if self.value[0:2] == "0x":
                self.value = int(self.value, 16)
            else:
                self.value = int(self.value)

    def buildKalDeclaration(self, indent="", parent="", val_req=False, **kwargs):
        """Build the Kalimba output."""
        if not kwargs.has_key("offset"):
            raise Exception("Enum entry display called without offset")
        offset = kwargs["offset"]
        # does this one reset the offset
        if self.value != None:
            offset = self.value
        else:
            if val_req:
                raise Exception("Enum entry value missing for: "+parent+"."+self.name)
        # change camel case to underscores
        parent = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,parent).lower()
        name   = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,self.name).upper()
        name = parent+"."+name
        # comment?
        dec = self.comment.buildKalComment(indent)
        # print out
        return dec+indent+".CONST $%-60s    %d;" %(name, offset)

    def buildCDeclaration(self, indent="", parent="", val_req=False, **kwargs):
        """Build the 'C' output."""
        dec = self.comment.buildCComment(indent)
        name = parent.upper()+"_"+self.name
        if self.value == None:
            if val_req:
                raise Exception("Enum entry value missing for: "+parent+"."+self.name)
            else:
                return dec+indent+name
        else:
            return dec+indent+name+" = "+str(self.value)

    def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
        """Build the Matlab output for enumeration entries. Builds a dictionary
        of names to values (offsets) and a reverse dictionary of offsets to
        values.

        The variable name defaults to Baton, but can be defined by setting the
        'matlabRoot' attribute of the top level object in the XML document."""
        if not kwargs.has_key("offset"):
            raise Exception("Enum entry display called without offset")
        offset = kwargs["offset"]
        # does this one reset the offset
        if self.value is not None:
            # it does use its value
            offset = int(self.value)
        # change camel case to underscores
        parent = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,parent).lower()
        name   = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,self.name).upper()
        fullname = parent+"."+name
        # comment?
        dec = self.comment.buildMatlabComment(indent)
        # print out
        dec += indent+"%-60s = %d;\n" % (fullname, offset)
        dec += indent+"%-60s = %d;\n" % (parent+".rev{end+1, 1}", offset)
        dec += indent+"%-60s = '%s';;" % (parent+".rev{end,   2}", name)
        return dec

    def buildPythonDeclaration(self, indent="", parent="", **kwargs):
        """Build the python output for enumeration entries. Builds a dictionary
        of names to values (offsets). A non-standard dictionary is used that
        supports forward and reverse look ups."""
        if not kwargs.has_key("offset"):
            raise Exception("Enum entry display called without offset")
        offset = kwargs["offset"]
        # does this one reset the offset
        if self.value != None:
            # it does use its value
            offset = int(self.value)
        # check we have a parent
        if parent == "":
            raise Exception()
        # change camel case to underscores
        parent = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,parent).lower()
        name   = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,self.name).upper()
        # comment?
        dec = self.comment.buildPythonComment(indent)
        # print out
        dec += indent+"%s['%s'] = %d" % (parent, name, offset)
        return dec


class enum(symbol):
    """Class for enumeration types.

    Note, this class does have child symbols - the enumeration values - however,
    they are not reported as extra children, hence we can use the base method
    for getNumGenerations."""
    tagsUsed = ["members"]
    optsUsed = ["valuesRequired"]

    def __init__(self, node, declare):
        """Instantiate an enumeration object, including its children (The
        entries)."""
        symbol.__init__(self, node, declare, "enum", "Enumeration")
        # check if values are required, must be true or false
        val_req = getOptionalTag(node, "valuesRequired", "false")
        if val_req == "false":
            self.val_req = False
        elif val_req == "true":
            self.val_req = True
        else:
            err  = "Enumeration field 'valueRequired' must be either 'true' or 'false'.\n"
            err += "Got: %s in node:\n  %s" % (val_req, node.toxml())
            raise Exception(err)

        self.entries = []
        members = getNode(node, "members")
        for entry in filter(lambda n: n.nodeType == n.ELEMENT_NODE, members.childNodes):
            ent = declare( entry )
            if ent.getType() != "enumEntry":
                raise Exception("Incorrect entry '"+ent.getType()+"' found in enumeration:\n"+node.toxml())
            self.entries.append(ent)

    def buildKalDeclaration(self, indent="", parent="", **kwargs):
        """Build the Kalimba output for enumeration lists, invoking the output
        method on the children (the list entries)."""
        name = self.name
        if kwargs.has_key("typedefName"):
            name = kwargs["typedefName"]
        off  = 0
        if len(parent) != 0:
            name = parent+"."+self.name
        dec  = indent+"   // %s\n" % (name)
        dec += self.comment.buildKalComment(indent)
        for entry in self.entries:
            dec += entry.buildKalDeclaration(indent, name, self.val_req, offset=off)+"\n"
            # does this reset the value, either way increment after
            if entry.getValue() != None:
                off = entry.getValue()
            off += 1
        return dec

    def buildCDeclaration(self, indent="", parent="", **kwargs):
        """Build the 'C' output for enumeration lists, invoking the output
        method on the children (the list entries)."""
        dec   = self.comment.buildCComment("")
        dec  += "enum %s\n{\n" % (self.name)
        for entry in self.entries:
            dec += entry.buildCDeclaration("  ",self.name, self.val_req)+",\n"
        # this is a bit horrible but drop the last comma
        dec = dec[:-2]+"\n"
        # finish off
        dec += "};"
        return dec

    def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
        """Build the Matlab output for enumerations. Builds a dictionary of
        names to values (offsets) and a reverse dictionary of offsets to values.

        The variable name defaults to Baton, but can be defined by setting the
        'matlabRoot' attribute of the top level object in the XML document."""
        name = self.name
        if kwargs.has_key("typedefName"):
            name = kwargs["typedefName"]
        if len(parent) != 0:
            name = parent+"."+self.name
        name = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,name).lower()

        # is there a root
        try:
            root = kwargs["root"]
        except KeyError:
            root = "BATON"

        dec  = indent+"   %% %s\n" % (name)
        dec += self.comment.buildMatlabComment(indent+"   ")
        dec += indent+"   %s.%-60s = {};\n" % (root, name+".rev")
        off  = 0
        for entry in self.entries:
            dec += entry.buildMatlabDeclaration(indent+"   %s." % root, name, offset=off)+"\n"
            # does this reset the value, either way increment after
            if entry.getValue() != None:
                off = entry.getValue()
            off += 1
        return dec

    def buildPythonDeclaration(self, indent="", parent="", **kwargs):
        """Build the python output for enumerations. Builds a dictionary of
        names to values (offsets). A non-standard dictionary is used that
        supports forward and reverse look ups."""
        name = self.name
        if kwargs.has_key("typedefName"):
            name = kwargs["typedefName"]
        # do we have a parent
        if len(parent) != 0:
            name = parent+"."+name
        name = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,name)

        dec  = indent+"# %s\n" % (name)
        dec += self.comment.buildPythonComment(indent)
        dec += indent+"%s = biDirDict()\n" % (name)
        off  = 0
        for entry in self.entries:
            dec += entry.buildPythonDeclaration(indent, name, offset=off)+"\n"
            # does this reset the value, either way increment after
            if entry.getValue() != None:
                off = entry.getValue()
            off += 1
        return dec


    def buildTypedefXmlDeclaration(self, title=0, offset=1, size=None):
        """Generates the documentation output for enumerations when
        typedef'd."""
        dec = "<Paragraph>"
        if len(self.name) == 0:
            dec  = "Aliases an anonymous enumeration."
        else:
            dec  = "Aliases an enumeration, called: '"+self.name+"'."
        dec += "</Paragraph>\n"
        dec += "<Paragraph>"+self.info+"</Paragraph>\n"
        dec += "<table border=1 cellpadding=5>\n"
        for entry in self.entries:
            dec += entry.buildFullInfoDeclaration()
        dec += "</table>\n"
        return dec


    def buildTypedefInfoDeclaration(self):
        """Generates the information output for enumerations when typedef'd."""
        if len(self.name) == 0:
            dec  = "<p>Aliases an anonymous enumeration.</p>\n"
        else:
            dec  = "<p>Aliases a enumeration, called: '"+self.name+"'.</p>\n"
        dec += "<p>"+self.info+"</p>\n"
        dec += "<table border=1 cellpadding=5>\n"
        for entry in self.entries:
            dec += entry.buildFullInfoDeclaration()
        dec += "</table>\n"
        return dec


    def buildFullInfoDeclaration(self, title=0, depth=0):
        """Build the full XML output for enumeration symbols. Calls the
        same method on each child element (enumeration value)."""
        if not title:
            raise Exception("Enumerations cannot be nested.")

        raise Exception("XML output not implemented for Enumerations.")


    def buildFullInfoDeclaration(self, title=0, depth=0):
        """Build the full information output for enumeration symbols. Calls the
        same method on each child element (enumeration value)."""
        # if we're the title, then make the link a title.
        if title:
            dec  = "<a class=\"anchor\" name=\"%s\"></a><h2><tt>enum %s</tt></h2>" % (self.link, self.name)
        else:
            # we're not the title, which means we're nested in the parent's
            # table, so make our output fit in with that
            dec  = "<tr>\n<td rowspan=%d>\n" % (len(self.entries))
            if self.name == "":
                dec += "<a class=\"anchor\" name=\"%s\"></a><p><b>anonymous enumeration</b></p>\n" % (self.link)
            else:
                dec += "<a class=\"anchor\" name=\"%s\"></a><p><b>enumeration %s</b></p>\n" % (self.link, self.name)
            if depth < 3:
                dec += "</td><td colspan=%d>\n" % (depth-1)
            else:
                dec += "</td><td>\n"

        dec += "<p>"+self.info+"</p>\n"
        dec += "<p>Contains the following members:</p>\n"

        if title:
            dec += "<table border=1 cellpadding=5>\n"

        for entry in self.entries:
            dec += entry.buildFullInfoDeclaration()

        if title:
            dec += "</table>\n"
            dec += "<hr>\n"
        else:
            dec += "</td>\n</th>\n"

        return dec


class listEntry(symbol):
    """Class for list entries, used within a list define."""

    tagsUsed = ["value"]
    optsUsed = []

    def __init__(self, node, declare):
        "Instantiate a list entry object."
        symbol.__init__(self, node, declare, "listEntry", "List Entry")
        self.value = getTag(node, "value")
        # if there was a value then convert to a number
        if len(self.value) == 0:
            self.value = None
        else:
            if self.value[0:2] == "0x":
                self.value = int(self.value, 16)
            else:
                try:
                    self.value = int(self.value)
                except:
                    pass

    def getSize(self):
        "A list entry doesn't take any memory so has a size of 0."
        return 0

    def buildKalDeclaration(self, indent="", parent="", **kwargs):
        """Build the kalimba output for the list entry symbol, uses the kalasm
        .CONST construct."""
        dec = self.comment.buildKalComment(indent)
        name = self.name
        if len(parent) != 0:
            name = parent+"."+self.name
        return dec+indent+".CONST %-60s 0x%04X;" % ("$"+name, self.value)

    def buildCDeclaration(self, indent="", parent="", **kwargs):
        """Build the 'C' output for the list entry symbol, using the #define
        preprocessor directive."""
        dec = self.comment.buildCComment("")
        name = self.name
        if len(parent) != 0:
            name = parent.upper()+"_"+self.name
            # XAP compiler doesn't like pre-processor commands not left justified so
        # ignore indent
        return dec+"#define %-60s 0x%04X" % (name, self.value)

    def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
        """Build the Matlab output for list entries. Builds a dictionary of
        names to values (offsets) and a reverse dictionary of offsets to values.

        The variable name defaults to Baton, but can be defined by setting the
        'matlabRoot' attribute of the top level object in the XML document."""
        if len(parent) == 0:
            parent = "list"
        fullname = parent+"."+self.name
        dec  = self.comment.buildMatlabComment(indent)
        dec += indent+"%-60s = %d;\n" % (fullname, self.value)
        dec += indent+"%-60s = %d;\n" % (parent+".rev{end+1,1}", self.value)
        dec += indent+"%-60s = '%s';" % (parent+".rev{end,  2}", self.name)
        return dec

    def buildPythonDeclaration(self, indent="", parent="", **kwargs):
        """Build the python output for list entries. Builds a dictionary of
        names to values (offsets). A non-standard dictionary is used that
        supports forward and reverse look ups."""
        if len(parent) == 0:
            parent = "list"
        dec  = self.comment.buildPythonComment(indent)
        dec += "%s%s['%s'] = %d"%(indent, parent, self.name, self.value)
        return dec

    def buildFullXmlDeclaration(self, title=0, offset=1, size=None):
        """Builds the full XML output for list entries. It basically
        builds a row to fit within a table declared by the list."""
        dec  = '<Row>\n'
        dec += '  <Entry>\n'
        dec += '    <Paragraph>'+self.name+'</Paragraph>\n'
        dec += '  </Entry>\n'
        dec += '  <Entry>\n'
        dec += '    <Paragraph>%d (0x%04X)</Paragraph>\n' % (self.value, self.value)
        dec += '  </Entry>\n'
        dec += '  <Entry>\n'
        dec += '    <Paragraph>'+self.info+'</Paragraph>\n'
        dec += '  </Entry>\n'
        dec += '</Row>\n'
        return dec

    def buildFullInfoDeclaration(self, title=0, depth=0):
        """Builds the full information output for list entries. It basically
        builds a row to fit within a table declared by the list."""
        dec  = "  <tr>\n"
        dec += "    <td><tt>%s</tt></td>\n" % (self.name)
        if depth < 3:
            dec += "    <td>%d (0x%04X)</td>\n" % (self.value, self.value)
        else:
            dec += "    <td rowspan=%d>%d (0x%04X)</td>\n" % (depth-1, self.value, self.value)
        dec += "    <td>%s</td>\n" % (self.info)
        dec += "  </tr>\n"
        return dec

class listNumEls(symbol):
    """Class for the 'size of list' symbol.

    This symbol is used to check the last message, e.g. NUM_MESSAGE_IDs.

    Needs to be at the end of the list, but if the list starts at a non-zero
    value, this is taken into account."""

    tagsUsed = []
    optsUsed = []

    def __init__(self, node, declare):
        "Instantiate a size of list element."
        symbol.__init__(self, node, declare, "listNumEls", "Size of list")

    def buildKalDeclaration(self, indent="", parent="", **kwargs):
        """Build the kalimba output for the size of list symbol, uses the kalasm
        .CONST construct."""
        dec = self.comment.buildKalComment(indent)
        name = self.name
        if len(parent) != 0:
            name = parent+"."+self.name
        return dec+indent+".CONST %-60s 0x%04X" % ("$"+name, self.value)

    def buildCDeclaration(self, indent="", parent="", **kwargs):
        """Build the 'C' output for the size of list symbol, using the #define
        preprocessor directive."""
        dec = self.comment.buildCComment()
        name = self.name
        if len(parent) != 0:
            name = parent.upper()+"_"+self.name
        return dec+"#define %-60s 0x%04X" % (name, self.value)

    def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
        """Build the Matlab output for size of list. Builds a dictionary of
        names to values (offsets) and a reverse dictionary of offsets to values.

        The variable name defaults to Baton, but can be defined by setting the
        'matlabRoot' attribute of the top level object in the XML document."""
        if len(parent) == 0:
            parent = "list"
        fullname = parent+"."+self.name
        dec  = self.comment.buildMatlabComment(indent)
        dec += indent+"%-60s = %d;\n" % (fullname, self.value)
        dec += indent+"%-60s = %d;\n" % (parent+".rev{end+1,1}", self.value)
        dec += indent+"%-60s = '%s';" % (parent+".rev{end,  2}", self.name)
        return dec

    def buildPythonDeclaration(self, indent="", parent="", **kwargs):
        """Build the python output for size of list symbol. Builds a dictionary
        of names to values (offsets). A non-standard dictionary is used that
        supports forward and reverse look ups."""
        if len(parent) == 0:
            parent = "list"
        dec  = self.comment.buildPythonComment(indent)
        dec += "%s%s['%s'] = %d"%(indent, parent, self.name, self.value)
        return dec



class listDefine(symbol):
    """Class for list defines, these are similar to enumerations in 'C'.

    Note, this class does have child symbols - the list values - however, they
    are not reported as extra children, hence we can use the base method for
    getNumGenerations."""

    tagsUsed = ["members"]
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate a list define, creating the list entries and list size
        entries as required."""
        symbol.__init__(self, node, declare, "listDefine", "List Define")
        self.entries = []
        self.lookup  = {}
        value = 0
        start = None

        members = getNode( node, "members" )

        for entry in filter(lambda n: n.nodeType == n.ELEMENT_NODE, members.childNodes):
            ent = declare( entry )
            if ent.getType() == "listEntry":
                # does this have a value
                if ent.getValue() == None:
                    ent.setValue(value)
                else:
                    if self.lookup.has_key(ent.getValue()):
                        value = self.lookup[ent.getValue()].getValue()
                        ent.setValue(value)
                    else:
                        value = ent.getValue()
                if start == None:
                    start = value
                value += 1
            elif ent.getType() == "listNumEls":
                ent.setValue(value - start)
            else:
                raise Exception("Incorrect entry '" + ent.getType() +
                                "' found in enumeration:\n" + node.toxml())
            # save the entry
            self.entries.append(ent)
            self.lookup[ent.getName()] = ent


    def buildKalDeclaration(self, indent="", parent="", **kwargs):
        """Build the Kalimba output for list defines, invoking the same method
        on its children (listEntry and listNumEls symbols)."""
        name = self.name
        if len(parent) != 0:
            name = parent+"."+self.name
        name = CAMEL_CASE_SEARCH.sub(CAMEL_CASE_REPLACE_STRING,name).lower()
        dec  = indent+"// %s\n" % (name)
        dec += self.comment.buildKalComment(indent)
        for entry in self.entries:
            dec += entry.buildKalDeclaration(indent, name)+"\n"
        return dec

    def buildCDeclaration(self, indent="", parent="", **kwargs):
        """Build the 'C' output for list defines, invoking the same method on
        its children (listEntry and listNumEls symbols)."""
        name = self.name
        if len(parent) != 0:
            name = parent.upper()+"_"+self.name
        dec  = ("/* %%-%ds */\n" % (DEFAULT_WIDTH-6)) % (name)
        dec += self.comment.buildCComment(indent+"  ")
        for entry in self.entries:
            dec += entry.buildCDeclaration("  ",name)+"\n"
        return dec

    def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
        """Build the Matlab output for list defines, invoking the same method on
        its children (listEntry and listNumEls symbols)."""
        name = self.name
        if len(parent) != 0:
            name = parent+"."+self.name

        # is there a root
        try:
            root = kwargs["root"]
        except KeyError:
            root = "BATON"

        dec  = indent+"   %% %s\n" % (name)
        dec += self.comment.buildMatlabComment(indent+"   ")
        dec += indent+"   %s.%-60s = {};\n" % (root, name+".rev")
        off  = 0
        for entry in self.entries:
            dec += entry.buildMatlabDeclaration(indent+"   %s." % root, name, offset=off)+"\n"
            # does this reset the value, either way increment after
            if entry.getValue() != None:
                off = entry.getValue()
            off += 1
        return dec

    def buildPythonDeclaration(self, indent="", parent="", **kwargs):
        """Build the Python output for list defines, invoking the same method on
        its children (listEntry and listNumEls symbols)."""
        name = self.name
        if len(parent) != 0:
            name = parent+"."+self.name
        dec  = indent+"# %s\n" % (name)
        dec += self.comment.buildPythonComment(indent)
        dec += indent+"%s = biDirDict()\n" % (name)
        off  = 0
        for entry in self.entries:
            dec += entry.buildPythonDeclaration(indent, name, offset=off)+"\n"
            # does this reset the value, either way increment after
            if entry.getValue() != None:
                off = entry.getValue()
            off += 1
        return dec

    def buildFullXmlDeclaration(self, title=0, offset=1, size=None):
        """Build the full XML output for list defines, invoking the same method
        on its children (listEntry and listNumEls symbols)."""
        if not title:
            raise Exception("List defines cannot be nested.")

        dec  = '<Chapter anchor="'+self.link+'.0">\n'
        dec += '<!--List define: %s (%d)-->\n' % (self.name, self.getNumGenerations())
        dec += '  <Title>%s</Title>\n' % self.name
        dec += '  <Description anchor="%s.1">\n' % self.link
        dec += '    <Paragraph>%s</Paragraph>\n' % self.info

        dec += xmlTableStart(self.link, 3, ['Name', 'Value','Description'])

        for entry in self.entries:
            dec += entry.buildFullXmlDeclaration(offset=offset)

        dec += xmlTableEnd(self.name)

        dec += '  </Description>\n'
        dec += '</Chapter>\n\n\n'

        return dec

    def buildFullInfoDeclaration(self, title=0, depth=0):
        """Build the full information output for list defines, invoking the same
        method on its children (listEntry and listNumEls symbols)."""
        if not title:
            raise Exception("List defines cannot be nested.")

        # title and info
        dec  = "<a class=\"anchor\" name=\""+self.link+"\"></a><h2><tt>list "+self.name+"</tt></h2>"
        dec += "<p>%s</p>\n" % self.info
        # start the table and add the header
        dec += "<table border=1 cellpadding=5>\n"
        dec += "  <tr>\n"
        dec += "    <th>Name</th>\n"
        dec += "    <th>Value</th>\n"
        dec += "    <th>Description</th>\n"
        dec += "  </tr>\n"
        # iterate over each element
        for ent in self.entries:
            dec += ent.buildFullInfoDeclaration()
        # finish off the table and close the block (draw a line)
        dec += "</table>"
        dec += "<hr>"

        return dec




# define class
# holds #defines
class define(preproc):
    """Class representing #defines. This is only relevant for Kalimba and 'C'
    output."""

    def __init__(self, node, declare):
        """Instantiate a define object, very simple, basically just uses the
        preproc base class."""
        preproc.__init__(self, node, declare, "define", "#define")
        if len(self.value) == 0:
            self.string = " %s" % (self.name)
        else:
            self.string = " %s %s" % (self.name, self.value)


class ifdef(preproc):
    """Class representing preprocessor '#ifdef's. This is only relevant for
    Kalimba and 'C' output."""

    tagsUsed = []
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate a #ifdef object, very simple, basically just uses the
        preproc base class."""
        preproc.__init__(self, node, declare, "ifdef", "#ifdef")
        self.string = " " + self.test


class ifndef(preproc):
    """Class representing preprocessor '#ifndef's. This is only relevant for
    Kalimba and 'C' output."""

    tagsUsed = []
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate a #ifndef object, very simple, basically just uses the
        preproc base class."""
        preproc.__init__(self, node, declare, "ifndef", "#ifndef")
        self.string = " " + self.test


class hashIf(preproc):
    """Class representing preprocessor '#if's. This is only relevant for Kalimba
    and 'C' output."""

    tagsUsed = []
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate a #if object, very simple, basically just uses the
        preproc base class."""
        preproc.__init__(self, node, declare, "hashIf", "#if")
        self.string = " " + self.test


class hashElse(preproc):
    """Class representing preprocessor '#else's. This is only relevant for
    Kalimba and 'C' output."""

    tagsUsed = []
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate a #else object, very simple, basically just uses the
        preproc base class."""
        preproc.__init__(self, node, declare, "hashElse", "#else")


class hashElif(preproc):
    """Class representing preprocessor '#elif's. This is only relevant for
    Kalimba and 'C' output."""

    tagsUsed = []
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate a '#elif' object, very simple, basically just uses the
        preproc base class."""
        preproc.__init__(self, node, declare, "hashElif", "#elif")
        self.string = " "+self.test


class endif(preproc):
    """Class representing preprocessor '#endif's. This is only relevant for
    Kalimba and 'C' output."""

    tagsUsed = []
    optsUsed = []

    def __init__(self, node, declare):
        """Instantiate a #else object, very simple, basically just uses the
        preproc base class."""
        preproc.__init__(self, node, declare, "endif", "#endif")



class banner:
    """Extended comment class, used to embed large blocks of text in the
    output."""

    tagsUsed = ["text"]
    optsUsed = []

    def __init__(self, node, declare):
        "Instantiate a object."
        self.text = getTag(node, "text")
        self.text = self.text.replace("\n"," ")
        while self.text.find("  ") != -1:
            self.text = self.text.replace("  ", " ")
        self.text = self.text.split(" ")

    def getName(self):
        "Return the name, always nothing."
        return ""

    def getType(self):
        "Our type, which is simply 'banner'."
        return "banner"

    def buildKalDeclaration(self, indent="", parent="", **kwargs):
        """Build the Kalimba output, fairly build the block of text and put a
        banner all around it."""
        width=DEFAULT_WIDTH
        if kwargs.has_key("width"):
            width = kwargs["width"]
        return self.__buildDeclaration(indent, width, "/*", "*/")
    def buildCDeclaration(self, indent="", parent="", **kwargs):
        """Build the 'C' output, fairly build the block of text and put a
        banner all around it."""
        width=DEFAULT_WIDTH
        if kwargs.has_key("width"):
            width = kwargs["width"]
        return self.__buildDeclaration(indent, width, "/*", "*/")
    def buildMatlabDeclaration(self, indent="", parent="", **kwargs):
        """Build the Matlab output, fairly build the block of text and put a
        banner all around it."""
        width=DEFAULT_WIDTH
        if kwargs.has_key("width"):
            width = kwargs["width"]
        return self.__buildDeclaration(indent, width, "%", "")
    def buildPythonDeclaration(self, indent="", parent="", **kwargs):
        """Build the Python output, fairly build the block of text and put a
        banner all around it."""
        width=DEFAULT_WIDTH
        if kwargs.has_key("width"):
            width = kwargs["width"]
        return self.__buildDeclaration(indent, width, "#", "")



    def __buildDeclaration(self, indent, width, comStart, comEnd):
        """Utility function that actually does all the work building the banner
        for the public output methods."""
        # first off put the big banner
        dec  = indent+comStart
        dec += "*" * (width - len(indent) - len(comStart) - len(comEnd))
        dec += comEnd+"\n"
        # build up the rest
        line = indent+comStart
        for t in self.text:
            # work out if this will push us over the limit, remember the space
            if (len(line)+len(t)+len(comEnd)+1) > width:
                line = ("%%-%ds%%s" % (width-len(comEnd))) % (line, comEnd)
                dec += line.rstrip()+"\n"
                line = indent+comStart+" "
            line += " "+t
        line = ("%%-%ds%%s" % (width-len(comEnd))) % (line, comEnd)
        dec += line.rstrip()+"\n"
        dec += indent+comStart
        dec += "*" * (width - len(indent) - len(comStart) - len(comEnd))
        dec += comEnd+"\n"
        return dec


#*******************************************************************************
# documentation helper function
#*******************************************************************************
# utility function returns the list of tags used by the various classes
def buildKeys(classes):
    """Function to build the list of tags used by each function. Returns two
    dictinaries, required tags and optional tags.

    Each dictionary has the tags used as keys and a list of classes that use
    those tags as values."""
    def getClassTags(classType):
        """Utility function to get tags and options for each class and any
        classes it is derived from."""
        try:
            tags = classType.tagsUsed
            opts = classType.optsUsed
        except AttributeError:
            return ([], [])
        for b in classType.__bases__:
            tags.extend(b.tagsUsed)
            opts.extend(b.optsUsed)
        return tags, opts

    # intialise the dictionary of classes against types
    resultTags    = {}
    resultOptions = {}
    for (k,v) in classes.iteritems():
        tags, opts = getClassTags(v)
        for t in tags:
            try:
                if k not in resultTags[t]:
                    resultTags[t].append(k)
                    resultTags[t].sort()
            except KeyError:
                resultTags[t] = [k,]
        for o in opts:
            try:
                if k not in resultOptions[o]:
                    resultOptions[o].append(k)
                    resultOptions[o].sort()
            except KeyError:
                resultOptions[o] = [k,]
    return (resultTags, resultOptions)


#*******************************************************************************
# document class
#*******************************************************************************

# document class
# holds the information used to build the requested document
class document:
    def __buildDocumentClassDocString():
        """Utility function used by the document class to build its document
        string."""

        # build a dictionary of tags and their descriptions, seems a little over
        # the top, but keeps all the information in one place
        tagsStrings = {
            "comment"        : "Define the comment string",
            "define"         : "Define the symbol name for #define's",
            "info"           : "Information string, to end up in the 'info' output",
            "instance"       : "Instance name",
            "matlabRoot"     : "Name of variable used by the matlab output",
            "members"        : "List of symbols, which are going to be children of this symbol",
            "name"           : "Name of this symbol",
            "size"           : "Size of this symbol, i.e. indicate it is an array",
            "subtype"        : "Define the actual type of general symbol",
            "symbol"         : "Define a symbol, either a top level entity a child in a members",
            "test"           : "Define the preprocessor test",
            "text"           : "Text to put into a banner symbol",
            "title"          : "Set the overall document title",
            "value"          : "Define a value for this symbol",
            "valuesRequired" : "Does the enumeration allow automatic value assignment in entries",
            }
        # build the list of classes
        classes = dict(filter(lambda (k,v): type(v) == types.ClassType, globals().iteritems()))
        (tagsUsed, optionsUsed) = buildKeys(classes)

        # build the string we are going to add to the document class
        s  = "Document class that represents the XML document and contains the data.\n\n"
        s += "Available tags:\n"

        for tag in tagsStrings:
            try:
                used = "    Required by : %s\n" % (", ".join(tagsUsed[tag]))
            except KeyError:
                used = ""
            try:
                opts = "    Optional for: %s\n" % (", ".join(optionsUsed[tag]))
            except KeyError:
                opts = ""
            s += "  %s\n  %s\n    %s\n\n%s%s\n" % (tag, "-"*len(tag), tagsStrings[tag], used, opts)

        return s

    __doc__ = __buildDocumentClassDocString()

    tagsUsed = []
    optsUsed = ["title", "info", "matlabRoot"]

    def __init__(self, inputFile, declare):
        """Initialisation method saves some data, parses the XML code and reads
        some top level options from the XML structure.

        Takes two arguments, the input file and an object that builds
        symbols. By default declare an object of type: 'Declare'."""
        self.source       = os.path.join(os.getcwd(), inputFile)
        self.timeStamp    = datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
        self.declare      = declare
        # intialise the sequential data object, used so we print out in the
        # correct order; in "C", etc the order of declaration/definition matters
        self.data         = []

        # parse the XML file
        try:
            self.dom = parse(inputFile)
        except Exception as e:
            err  = "Exception %s, raise when parsing input file%s\n" % (type(e), inputFile)
            err += "Full message:\n  %s" % (e.message)
            raise Exception(err)
        # parse the root object and get the various pieces of information out of
        # it
        self.title = getOptionalTag(self.dom.childNodes[0], "title", "Baton")

        b  = "This document describes the various API messages and "
        b += "structures used by host components and the various "
        b += "onchip components to communicate with each other."
        self.bumfText = getOptionalTag(self.dom.childNodes[0], "info", b)

        self.matlabRoot = getOptionalTag(self.dom.childNodes[0],"matlabRoot","BATON")

        # a look up table to select the correct output format
        self.writeFuncsLut       = {}
        self.writeFuncsLut["-k"] = self.writeKalimbaOutput
        self.writeFuncsLut["-x"] = self.writeCOutput
        self.writeFuncsLut["-m"] = self.writeMatlabOutput
        self.writeFuncsLut["-i"] = self.writeInfoOutput
        self.writeFuncsLut["-p"] = self.writePythonOutput
        self.writeFuncsLut["-d"] = self.writeXmlOutput


    def __call__(self, format, filename):
        """Generates the requested output from the XML data."""
        # turn the filename into something suitable for use in #define's
        prettyname =   filename.replace(".", "_").upper()
        prettyname = prettyname.replace("/", "__")
        prettyname = prettyname.replace(":", "__")
        prettyname = prettyname.replace("-", "__")

        # try and open the file
        with open(filename, "w") as output:
            self.writeFuncsLut[format]( output, prettyname )

    def build(self):
        """Parses the XML data to build the list of symbols."""

        children = filter( lambda n: n.nodeType == n.ELEMENT_NODE, self.dom.childNodes[0].childNodes)
        for node in children:
            try:
                s = self.declare(node)
            except DeclareError as de:
                # is it the document info block
                if de.nodeType != "docInfo":
                    raise de
                # has it got children, this is the only empty info block we allow
                if not node.hasChildNodes():
                    continue
                # does it contain embedded xml, i.e. HTML tags
                if node.getAttribute("type") == "xml":
                    self.bumfText = "".join(map(lambda n: n.toxml(), node.childNodes))
                else:
                    self.bumfText = node.childNodes[0].data
            else:
                self.data.append(s)

        # finally fix the typedefs (a bit like aliasing really)
        self.fixupTypedefs()


    def fixupTypedefs(self):
        """Utility function used by self.build. When the XML is parsed typedef's
        are assumed to alias 'built-in' types. If in fact they alias a structure
        we have already declared, the link is made in this function."""
        import types
        # now iterate over looking to fix up the structure defines
        for sym in self.data:
            # was this a typedef, if so do we need to change the kalimba define
            if sym.getType() == "typedef":
                # did we know what the original is
                if sym.define.getType() == "value":
                    # we didn't recognise it, is it something we actually know
                    # about, first check if its a union or structure
                    if type(sym.getDefineValue()) == types.UnicodeType:
                        name = sym.getDefineValue().split()
                        if len(name) == 1:
                            if self.declare.has_symbol(name[0]):
                                sym.setStruct(self.declare[name[0]])
                            elif self.declare.has_symbol("typedef"+name[0]):
                                sym.setStruct(self.declare["typedef"+name[0]])
                        else:
                            if self.declare.has_symbol(name[0]+name[1]):
                                sym.setStruct(self.declare[name[0]+name[1]])
                    else:
                        if self.declare.has_symbol(sym.getDefineValue()):
                            sym.setStruct(self.declare[sym.getDefineValue()])


    def writeHeaderOutput(self, com, output, prettyname):
        """Utility function that prints out the header information at the top of
        each output file, usual CSR header, date stamp, etc.."""
        # couple of strings we use
        titleString  = "           (c) Cambridge Silicon Radio Limited "
        titleString += datetime.datetime.now().strftime("%Y")
        rightsString = "           All rights reserved and confidential information of CSR"
        revString    = "   REVISION:      $Revision$"
        genString    = "This file was autogenerated from:"
        genString2   = "Using %s version %s, on %s" % (getFile(), getVersion(), self.timeStamp)

        # how long at the comment characters
        lineLen = DEFAULT_WIDTH - len(com[0]) - len(com[1])

        # some lines we use
        starLine = "*" * lineLen
        gapLine  = " " * lineLen

        # bumf required at the head of files that might be released
        out = [starLine, gapLine,
               ("%%-%ds" % lineLen) % titleString,
               gapLine,
               ("%%-%ds" % lineLen) % rightsString,
               gapLine,
               ("%%-%ds" % lineLen) % revString,
               starLine,
               # bumf that says file was autogenerated
               starLine,
               (" %%-%ds"   % (lineLen-1)) % genString,
               ("   %%-%ds" % (lineLen-3)) % self.source,
               (" %%-%ds"   % (lineLen-1)) % genString2,
               starLine]

        output.write( "\n\n"+"".join( "%s%s%s\n" % (com[0], o, com[1]) for o in out) + "\n\n" )

    def writeKalCOutput(self, format, output, prettyname):
        """Utility function used as the base/template for generating the Kalimba
        a 'C' output. This is the basically the function that does all the
        work when generating the outputs."""
        self.writeHeaderOutput(("/*","*/"), output, prettyname)
        # build guard bands
        output.write("#ifndef " + prettyname + "\n")
        output.write("#define " + prettyname + "\n")
        for d in self.data:
            # now just print it out
            if format == "KAL":
                line = d.buildKalDeclaration("", "")
            else:
                line = d.buildCDeclaration("", "")
            if len(line) != 0:
                output.write(line+"\n\n")
        # close the guard band
        output.write("#endif\n")

    def writeKalimbaOutput(self, output, prettyname):
        """Generate the Kalimba output from the XML document, generates the
        Kalimba header."""
        self.writeKalCOutput("KAL", output, prettyname)

    def writeCOutput(self, output, prettyname):
        """Generate the 'C' output from the XML document, generates the 'C'
        header."""
        self.writeKalCOutput("XAP", output, prettyname)

    def writeMatlabOutput(self, output, prettyname):
        """Generate the Matlab output from the XML document, generates the
        Matlab 'header'."""
        self.writeHeaderOutput(("%","%"), output, prettyname)
        for d in self.data:
            # now just print it out
            line = d.buildMatlabDeclaration("", "", root=self.matlabRoot)
            if len(line) != 0:
                output.write(line+"\n\n")

    def writePythonOutput(self, output, prettyname):
        """Generate the Python output from the XML document, generates the
        Python 'header'."""
        self.writeHeaderOutput(("#","#"), output, prettyname)
        # declare the reverse lookup class we use
        lookupClass  = "class biDirDict(dict):\n"
        lookupClass += "    def __init__(self, initial=None):\n"
        lookupClass += "        if not initial == None:\n"
        lookupClass += "            dict.__init__(self, initial)\n"
        lookupClass += "\n"
        lookupClass += "    def __getitem__(self, k):\n"
        lookupClass += "        if dict.has_key(self, k):\n"
        lookupClass += "            return dict.__getitem__(self, k)\n"
        lookupClass += "        try:\n"
        lookupClass += "            return dict.keys(self)[dict.values(self).index(k)]\n"
        lookupClass += "        except ValueError:\n"
        lookupClass += "            raise KeyError(k)\n\n"
        output.write(lookupClass)

        for d in self.data:
            # now just print it out
            line = d.buildPythonDeclaration("", "")
            if len(line) != 0:
                output.write(line+"\n\n")

    def writeInfoOutput(self, output, prettyname):
        """Generate the HTML information output from the XML document."""
        # html gubbins
        output.write("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n")
        output.write("<html>\n")

        # bumf that says file was autogenerated
        self.writeHeaderOutput(("<!",">"), output, prettyname)

        output.write("<head>\n")
        output.write("<title>" + self.title + " documentation</title>\n")
        output.write("<h1>" + self.title + " documentation</h1>\n")
        output.write("</head>\n")
        output.write("<body>\n")
        output.write("<p>" + self.bumfText + "</p>\n")
        output.write("\n<table border=1 cellpadding=5>\n")
        output.write("  <tr>\n")
        output.write("    <th>Symbol</th>\n")
        output.write("    <th>Type</th>\n")
        output.write("    <th>Description</th>\n")
        output.write("  </tr>\n")

        for d in self.data:
            # now just print it out
            line = d.buildSimpleInfoDeclaration()
            if len(line) != 0:
                output.write(line+"\n\n")
        output.write("</table>\n<hr>\n")

        for d in self.data:
            # now just print it out
            line = d.buildFullInfoDeclaration(1)
            if len(line) != 0:
                output.write(line+"\n\n")

        output.write("</body>\n</html>\n")

    def writeXmlOutput(self, output, prettyname):
        """Generate the XML documentation output from the XML document."""

        # make an alias
        w = output.write

        # bumf that says file was autogenerated
        self.writeHeaderOutput(("<!--","-->"), output, prettyname)

        # build the first table
        w('<Chapter anchor="Summary.0">\n')
        w('  <Title>Summary</Title>\n')
        w('  <Description anchor="Summary.1">\n')
        w(xmlTableStart("SymbolList", 3, ["Symbol", "Type", "Description"]))

        for d in self.data:
            # now just print it out
            line = d.buildSimpleXmlDeclaration()
            if len(line) != 0:
                w(line+"\n\n")

        w(xmlTableEnd('Symbol list'))
        w('  </Description>\n')
        w('</Chapter>\n\n\n')

        w('<Chapter anchor="Baton.0">\n')
        w('  <Title>Baton</Title>\n')
        w('  <Description anchor="Baton.1">\n')
        w('  </Description>\n')
        for d in self.data:
            # now just print it out
            line = d.buildFullXmlDeclaration(1)
            if len(line) != 0:
                output.write(line+"\n\n")

        w('</Chapter>\n\n\n')





def getVersion():
    """Utility function that parses the perforce RCS strings to stamp into the
    generated output. This parses the change list."""
    return P4_CHANGE_SEARCH.sub( P4_CHANGE_REPLACE_STRING, "$Change$")


def getFile():
    """Utility function that parses the perforce RCS strings to stamp into the
    generated output. This parses the file name."""
    return P4_FILE_SEARCH.sub( P4_FILE_REPLACE_STRING, "$File$")


if __name__ == "__main__":
    # build a pretty output message, a bit OTT! Starter and some spaces
    starter     = "    %s <input_filename> " % sys.argv[0]
    spaces      = " " * len(starter)
    # this is the actual string
    helpString  = "%s usage:\n" % sys.argv[0]
    helpString += starter + "-k KALIMBA_OUTPUT\n"
    helpString += spaces  + "-x XAP_OUTPUT\n"
    helpString += spaces  + "-m MATLAB_OUTPUT\n"
    helpString += spaces  + "-i INFO_OUTPUT\n"
    helpString += spaces  + "-p PYTHON_OUTPUT\n\n"
    helpString += "Any combination of output files is acceptable\n\n"
    helpString += "For full help use the --help option\n\n"

    # if the user got it wrong tell them
    errorString  = "Wrong number of input arguments.\n\n"
    errorString += helpString

    # we support three input combinations
    #   - the full set as described in helpString
    #   - the "standard" set of help flags: "-h", "--help"
    #   - being called with no inputs at all
    if len(sys.argv) == 1:
        print helpString
        exit(0)
    if len(sys.argv) == 2:
        if sys.argv[1] == "-h":
            print helpString
            exit(0)
        if sys.argv[1] == "--help":
            print helpString
            print document.__doc__
            exit(0)


    # rename the input arguments
    inputFile = sys.argv[1]
    try:
        outputFiles, junk = getopt.getopt(sys.argv[2:], "k:x:m:i:p:d:")
    except getopt.GetoptError as e:
        raise FormatError(e.opt)

    if len(junk) != 0:
        raise Exception(errorString)
    if len(outputFiles) == 0:
        raise Exception(errorString)

    # create the document object
    doc = document(inputFile, Declare())

    # build the data object
    doc.build()

    # work through the list of input files
    for (format, filename) in outputFiles:
        doc(format, filename)






