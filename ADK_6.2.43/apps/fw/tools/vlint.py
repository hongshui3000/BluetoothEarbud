# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
# PROGRAM
#   vlint.py  -  support for variable length integers
#
# ORIGIN
#   //depot/dot11/main/mac/mib/vlint.py#2

import sys

# Despite the name, VLINT supports both representing the supplied value as a VLINT
# and representing it as a plain octet.  This is because there's no point writing
# the individual octets in an octet string as VLINTs, since they rather obviously
# have fixed length.
class VLINT:
    def __init__(self, i,plain_octet=False):
        self.set_value(i,plain_octet)
        self.comments = []
        self.label = ""

    def set_value(self, i,plain_octet=False):
        self.val = i
        if plain_octet:
            self.octets = [i]
        else:
            self.octets = self.vlint_octet_list(i)

    def get_octet_list(self):
        return self.octets

    def add_comment(self, comment):
        self.comments = self.comments + [comment]

    def get_comments(self):
        return self.comments

    def set_label(self, label):
        self.label = label

    def get_label(self):
        return self.label

    def octet_length(self):
        return len(self.octets)

    # Return an array holding the octets of a VLINT holding "val".
    #
    def vlint_octet_list(self, val):
        if val == 0:
            return [0x00]
        if val >= -64 and val <= 63:
            return [val & 0x7f]
#       if val >= 0 and val <= 127:
#           return [val & 0x7f]
        # Use vwi re-entrantly to snip off 8 bits of "val" at each call.
        o = []
        o = self.vwi(o, val);
        if val < 0:
            return [0xC0 | len(o)] + o
        else:
            return [0x80 | len(o)] + o

    def vwi(self, o, val):
        if not (val >= -128 and val <= 255):
            o = self.vwi(o, val >> 8)
        return o + [val & 0xff]

# A list of VLINTs.  Eventually, the octets of the sequence of VLINTs will
# be poured into a uint16[], packed two octets per location.  The packing
# uses the lower octet of each uint16 first.  The start of each VLINT will
# be accessible by the firmware using a reference.  This is a bit weird:
# it's the data address of the VLINT as if it had been stored in a uint8[].
# It's the address, not an offset in its array, so that the magic value
# 0x0000 can be used to mean "not a reference".  It's treated as a *uint8
# so that the firmware can work out whether to start from the lower or
# upper octet of the uint16[].  (Incidentally, it will make porting much
# easier if this lot is ever moved to a sane processor, but that's
# irrelevant for now.  The most common use of these references is for
# mibkeys.  A VLINTList stores
# a sequence of VLINTs.  A name "label" can be attached to each VLINT.
# In practice, this will only be used by the first VLINT in each record.
# Subsequently, this label is converted to a reference - e.g., a mibkey.
#
class VLINTList:
    def __init__(self):
        self.vlints = []

    def append_vlint(self, vlint):
        self.vlints = self.vlints + [vlint]

    def append_value(self, val, comment):
        vl = VLINT(val)
        vl.add_comment(comment)
        self.append_vlint(vl)

    # Append a sequence of VLINTs forming a structure.  The first
    # VLINT is formed from the "id" and the length field "vallen"
    # (normally zero or the length of list "val").  To this VLINT
    # is attached "comment".  This is followed by a VLINT holding
    # each number in list "val" in turn.  The "valcomment" is
    # attached to the first VLINT in "val".  Returns the total
    # length of the octets in the VLINTList's notional uint8[]
    # before the structure's VLINTs.
    #
    # Note: for efficiency, there is an option not to use VLINTs
    # for the elements in the list, and just to encode them (if
    # that is the right way to describe leaving them alone) as
    # plain octets.  This is only appropriate if we know that
    # the values in list "val" are octets, which is true iff we
    # are encoding an octet string.  The firmware now expects
    # to encounter octet strings in the ROM in this format.
    def append_struct(self, id, val, vallen, comment, valcomment,
                      plain_octets=False):
        ret = self.octet_length()
        if id < 0 or id > 3:
            croak("unacceptable struct id")
        vl = VLINT((vallen << 2) + id)
        vl.add_comment(comment)
        self.append_vlint(vl)
        for v in val:
            vl = VLINT(v,plain_octets)
            if valcomment:
                vl.add_comment(valcomment)
                valcomment = ""
            self.append_vlint(vl)
        return ret

    def octet_length(self):
        ret = 0
        for v in self.vlints:
            ret = ret + v.octet_length()
        return ret

    # How many VLINTs are stored in the list?
    #
    def get_len(self):
        return len(self.vlints)

    # Find a VLINT in the list, identified by its reference symbol.
    #
    def get_vlint_label(self, label):
        for v in self.vlints:
            if v.get_label() == label:
                return v
        return None

    # Count the number of octets in the VLINTs between that with label
    # "label1" and that with label "label2".  The labels are required
    # to be in list order.  The octet count excludes the octets in the
    # two labelled VLINTs themselves.
    #
    def octets_between_labelled_vlints(self, label1, label2):
        counting = 0
        for v in self.vlints:
            if not counting:
                if v.get_label() == label1:
                    counting = 1
                    noctets = 0
            else:
                if v.get_label() == label2:
                    return noctets
                noctets = noctets + v.octet_length()
        return 0

    # Get the total length of the VLINTs stored in the list up to,
    # and excluding, the VLINT identified by "label".
    #
    def get_offset_label(self, label):
        noctets = 0
        for v in self.vlints:
            if v.get_label() == label:
                return noctets
            noctets = noctets + v.octet_length()
        return None

    # Return the number of VLINTs in self.vlints from the start up to the
    # VLINT labelled "label", excluding the VLINT with "label" itself.
    # (This can then be used with octet_length_range() to count the
    # number of octets in the range.)
    #
    def get_vlintlist_index_label(self, label):
        nvlints = 0
        for v in self.vlints:
            if v.get_label() == label:
                return nvlints
            nvlints = nvlints + 1
        return 0

    # How many octets are there in the set of VLINTs between "start"
    # and "end", inclusive?
    #
    def octet_length_range(self, start, end):
        ret = 0
        for i in range(start, end):
            ret = ret + self.vlints[i].octet_length()

    # Print the contents, packed into uint16s.
    #
    def repr_uint16_array(self, out):
        u16 = 0
        high_next = 0
        high_comments = ""
        low_comments = ""
        comments = ""
        nvlints = len(self.vlints)
        vno = 1
        for vl in self.vlints:
            for c in vl.get_comments():
                if c:
                    if comments:
                        comments = comments + " | "
                    comments = comments + c
            if high_next:
                high_comments = comments
            else:
                low_comments = comments
            comments = ""
            noctets = len(vl.octets)
            ono = 1
            for o in vl.octets:
                if not high_next:
                    u16 = o
                    high_next = 1
                else:
                    u16 = u16 + (o * 256)
                    if vno == nvlints and ono == noctets: # last octet?
                        comma = 0
                    else:
                        comma = 1
                    self.repr_uint16_line(out, u16, comma, low_comments,
                                                high_comments)
                    u16 = 0
                    low_comments, high_comments = "", ""
                    high_next = 0
            vno = vno + 1
        if high_next:
            self.repr_uint16_line(out, u16, 0, low_comments, high_comments)

    def repr_uint16_line(self, out, u16, comma, low_comments, high_comments):
        if low_comments and high_comments:
            comments = high_comments + " ## " + low_comments
        elif low_comments:
            comments = low_comments
        else:
            comments = high_comments
        out.write("0x%04x" % u16)
        if comma:
            out.write(",")
        else:
            out.write(" ")
        if comments:
            out.write("    /* %s */" % comments)
        out.write("\n")


# Test code for this file.
#
if __name__ == '__main__':
    vl = VLINTList()

    v = VLINT(1024)
    v.add_comment("1k")
    v.add_comment("or so")
    vl.append_vlint(v)

    v = VLINT(20)
    v.add_comment("twenty")
    vl.append_vlint(v)

    v = VLINT(30)
    v.add_comment("thirty")
    vl.append_vlint(v)

    v = VLINT(11)
    v.add_comment("eleven")
    vl.append_vlint(v)

    v = VLINT(21)
    v.add_comment("twenty one")
    vl.append_vlint(v)

    v = VLINT(31)
    v.add_comment("thirty one")
    v.add_comment("commented with 'hello'")
    v.set_label("hello")
    vl.append_vlint(v)

    v = VLINT(41)
    v.add_comment("forty one")
    vl.append_vlint(v)

    v = VLINT(51)
    v.add_comment("fifty one")
    vl.append_vlint(v)

    v = vl.get_vlint_label("hello")
    if v:
        v.set_value(4)
        v.add_comment("value changed to 4")
        print "reference: %s" % vl.get_offset_label("hello")

    hv = []
    for c in "hello":
        hv = hv + [ord(c)]
    vl.append_string_default(8, [5,7], hv, "hello struct", "hello")

    vl.repr_uint16_array(sys.stdout)
