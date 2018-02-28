############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2014 - 2017 Qualcomm Technologies International, Ltd.
#
############################################################################
from os.path import exists
import sys
import re

# A regular expression that matches a line that defines a constant
# Starts with a '$' then text followed by a ':' a space then some text (hex no)
re_const = re.compile(r'[$][\S]+[:][\s][\S]+')
# A regular expression that matches a line that defines row in the symbol table
re_symbol = re.compile(r'[\S]+[\s]+[\d]+[\s]+[\S]+[\s]+[\S]+[\s]+[\S]+[\s]+[\d]+[\s]+[\S]+')

class sym_table:
    """sym_table - A class that will read in a symbol (.sym) file and provide
    access to the file contents: constants and symbol locations"""
    
    def __init__(self, symbol_file_name):
        "sym_table constructor, loads the symbol file values"
        if not exists(symbol_file_name):
            print "The symbol file", symbol_file_name, "does not exist!  EXITING..."
            sys.exit(-1)
        
        # dictionary of constants defined in the symbol file
        self.consts = {}
        # dictionary of symbols defined in the symbol file
        self.symbols = {}
        # Flag to indicate whether symbols are function calls or really globals
        code_section = False
        
        # .sym file is processed using generator functions as file is large
        fh = open(symbol_file_name, 'r')
        for line in fh.xreadlines():
            # If the line is a const then add the value to the consts dictionary
            const_line = re_const.findall(line)
            if const_line:
                words = const_line[0].split(": ")
                self.consts[words[0]] = (words[1], words[0])
                continue
            # If we find a section header determine if it relates to code/variables
            if "SECTION" in line:
                if "flash.code:" in line or "flash.resetcode" in line:
                    code_section = True
                else:
                    code_section = False
            # If the line wasn't a constant its probably a symbol but check as
            # it could be useless info we can discard
            sym_line = re_symbol.findall(line)
            if sym_line:
                words = sym_line[0].split(" ")
                if words[6] == "NOTYPE":
                    type = "NONE"
                elif words[6] == "FUNC" and code_section:
                    type = "FUNC"
                else:
                    type = "VAR"
                self.symbols[words[-1]] = ("0x" + words[0], type, words[-1])
        #print len(self.consts), "constants and",
        #print len(self.symbols), "symbols read in from", symbol_file_name


    def get_const(self, name):
        "Searches the constant dictionary for the constant 'name'"
        return self.consts[name][0]
    
    def get_symbol(self, name):
        "Searches the symbol dictionary for the constant 'name', fails if not found"
        value = self.search_for_symbol(name)
        if "" != value:
            return value
        else:
            print "Symbol", name, "not found"
            raise StandardError
            
    def search_for_symbol(self, name):
        "Searches the symbol dictionary for the constant 'name'"
        if self.symbols.has_key(name):
            return self.symbols[name][0]
        else:
            return ""             

if __name__ == "__main__":
    symfile = "kymera.sym"
    symbols = sym_table(symfile)
    
    print "$PIO_SELECT9 is", symbols.get_const("$PIO_SELECT9")
    print "$flash.slt.__Base is", symbols.get_symbol("$flash.slt.__Base")   
    