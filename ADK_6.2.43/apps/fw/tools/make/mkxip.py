#!/usr/bin/python
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   Part of 6.2
'''
DESCRIPTION
  Construct a project file for use with xIDE.

  Usage:
      mkxip.py -o <name> [-c <core>] [-s ssid ] <list of source files>

  -o <name>
      The base name of the .xdv, .xpv and .sym files (without any extension).

  -p <path>
       The relative path to the root of the source files.

  -c <core>
      The processor core being targetted (p0 or p1).

  -s <ssid>
      The subsystem id
'''

from optparse import OptionParser
import os

def listfiles(directory, suffixes):
    #directory = os.sep.join(["..", directory])
    filelist = []
    for root, dirs, filenames in os.walk(directory):
        root_dirs = root.split(os.sep)[1:]
        for filename in filenames:
            if suffixes:
                for suffix in suffixes:
                    if filename.endswith(suffix) or not suffix:
                        filelist.append("/".join(root_dirs + [filename]))
                        break
            else:
                filelist.append("/".join(root_dirs + [filename]))
    return filelist

def file_list_to_dict(file_list):
    file_dict = {}
    for filename in file_list:
        bits = filename.split('/')
        name = bits[-1:][0]
        path = "/".join(bits[:-1])
        if path in file_dict:
            file_dict[path].append(name)
        else:
            file_dict[path] = [ name ]
            
    return file_dict

def output_elements(file_handle, source_file_dict, path_to_src):
    for path in source_file_dict:
        # Entries in the base path are excluded since they are not yet properly handled in Heracles
        #if path:
        file_handle.write(' <folder name="%s">\n' % path)
       
        #    for source in source_file_dict[path]:
        #       file_handle.write('  <file path="%s" />\n' % "/".join([path_to_src, path , source]))
        #    file_handle.write(' </folder>\n')
def output_project_header(file_handle, source_file_list, options):
    file_handle.write('<project    buildenvironment="{6907f3f0-0ed4-11d8-be02-0050dadfe87d}"\n') 
    file_handle.write('    buildenvironmentname="kalimba"\n')
    file_handle.write('    executionenvironmentoption="binutils"\n') 
    file_handle.write('    buildenvironmentoption=""\n')
    file_handle.write('    executionenvironmentname="kalimba"\n') 
    file_handle.write('    executionenvironment="{2ca81310-0ecd-11d8-be02-0050dadfe87d}" >\n')
    file_handle.write(' <properties currentconfiguration="Release" >\n')
    file_handle.write('  <configuration name="Release" >\n')
    file_handle.write('   <property key="buildMatch" >(?:ERROR|FATAL|WARNING|MESSAGE):\s+(file)\s+(line):</property>\n')
    file_handle.write('   <property key="hardware" >0</property>\n')
    file_handle.write('   <property key="processorid" >%d</property>\n' % options.core)
    file_handle.write('   <property key="subsysid" >%d</property>\n'  % options.ssid)
    file_handle.write('   <property key="debugtransport" >[SPITRANS=USB SPIMUL=4]</property>\n')
    file_handle.write('   <property key="outputFile">%s</property>\n' % options.ElfFileName)
    file_handle.write('  </configuration>\n')
    file_handle.write(' </properties>\n')
    output_elements(file_handle, source_file_list, options.SourcePath)
    file_handle.write('</project>\n')

if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-o", dest = "OutputFileName", default = "project.xip", 
                        type = "str", help = "Specify output file path and name")
    parser.add_option("-e", dest = "ElfFileName", 
                        type = "str", help = "Specify ELF file name")
    parser.add_option("-p", dest = "SourcePath", default = "", type = "str", 
                        help = "Specify output file path and name")
    parser.add_option("-c", dest = "core", default = "0", type = "int", 
                        help = "Specify processor core being targeted - 0 or 1")
    parser.add_option("-s", dest = "ssid", default = "5", type = "int", 
                        help = "Specify subsystem ID")
    options, args = parser.parse_args()

    try:
        file_handle = file(options.OutputFileName, "wb")
        #src_files =  [f.split("src/")[1] for f in listfiles("src", [ ".c", ".h", ".asm"])]
        src_files = listfiles(options.SourcePath, [ ".c", ".h", ".asm"])
        #config_files = [f for f in listfiles("config", [ ".mk"])]
        #python_files = [f for f in listfiles("tools", [ ".py"])]
        file_dict = file_list_to_dict(src_files)
        output_project_header(file_handle, src_files, options)
    except IOError:
        print "Unable to open output file ", options.OutputFileName
        exit()
