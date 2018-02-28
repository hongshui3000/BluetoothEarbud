#!/usr/bin/python
# Copyright (c) 2016-2017 Qualcomm Technologies International, Ltd.
#   Part of 6.2
'''
DESCRIPTION
  Construct an x2p project file for use with QMDE.

  Usage:
      mkx2p.py -o <name> -r <release> -i <includes> -d <defsfile> -l <linkscript>

  -o <name>
      The name of the .x2p file (including the extension).

  -r <release>
      The name of the release makefile. If this isn't specified the script will use
      all project related files from the top level directory for the file list.
      Assuming the script has been launched from the fw/src directory.

  -i <includes>
      A list of include directories.

  -d <defsfile>
      A header file containing a list of defines to set when compiling.

  -l <linkscript>
      The path to the link script.
'''
import os

def indent( depth ):
    return " " * 4 * depth

def xml_file_tree_r( file_node, children, path, depth=1 ):
    '''
    The posix path separator is used regardless of the OS this script run on.
    This keeps the project file the same for either build.
    '''
    qualified_path = '/'.join([path, file_node])

    if not children:
        return indent(depth) + '<file path="' + qualified_path + '"/>\n'

    xml = indent(depth) + '<folder name="' + file_node + '">\n'
    for key in children:
        xml += xml_file_tree_r(key, children[key], qualified_path, depth + 1)
    xml += indent(depth) + '</folder>\n'

    return xml

'''
Returns an xml formatted string representing the directory structure as a tree.
'''
def xml_file_tree( files, path ):
    xml = ""
    for key in files:
        xml += xml_file_tree_r(key, files[key], path)
    return xml

def dbg_core_from_name(name):
    return "app/" + name[-2:] if name.endswith(("_p0", "_p1")) else ""

chip_map = {'crescendo' : 'csra68100',
            'aura' : 'qcc3400'}

def get_chip_and_revision(release_name):
    '''
    Returns a tuple containing the chip name and revision.

    Raises ValueError if the chip or revision weren't found or occurred
    multiple times in release_name.
    '''
    components = release_name.split('_')

    import re
    revision, = [c for c in components if re.match(r'^d\d{2}$', c)]
    chip, = [c for c in components if c in chip_map]

    return (chip, revision)

def release_name_to_chip_type(release_name):
    chip, rev = get_chip_and_revision(release_name)
    return '_'.join([chip_map[chip], rev])

def xml_configuration(build_name, release_name, defs, incpaths, preinclude):
    xml = indent(2) + '<configuration name="makefile_project" options="clean|build|default">\n'
    devkit_groups = {
        'crescendo_p0_d01' : '//{A5701E48-935B-4f43-BFA2-E382247D6B2D}'
    }
    xml += indent(3) + '<devkitConfiguration>makefile_project</devkitConfiguration>\n'
    if build_name in devkit_groups:
        xml += indent(3) + '<devkitGroup>kitres:' + devkit_groups[build_name] + '</devkitGroup>\n'
    xml += indent(3) + '<property name="BUILD_TYPE">MAKEFILE</property>\n'
    xml += indent(3) + '<property name="CLEAN_ARGUMENTS">clean</property>\n'
    xml += indent(3) + '<property name="MAKEFILE_TO_CALL">Makefile</property>\n'
    xml += indent(3) + '<property name="DBG_CORE">' + dbg_core_from_name(build_name) + '</property>\n'
    xml += indent(3) + '<property name="OUTPUT">' + build_name + '</property>\n'
    xml += indent(3) + '<property name="DEFS">' + defs + '</property>\n'
    xml += indent(3) + '<property name="INCPATHS">' + incpaths.replace('\\', '/') + '</property>\n'
    xml += indent(3) + '<property name="PREINCLUDES">' + preinclude.replace('\\', '/') + '</property>\n'
    try:
        chip_type = release_name_to_chip_type(release_name)
        xml += indent(3) + '<property name="CHIP_TYPE">' + chip_type + '</property>\n'
    except ValueError:
        # The release name does not map to a CHIP_TYPE.
        pass
    xml += indent(2) + '</configuration>\n'
    return xml

def xml_configurations(build_name, release_name, defs, incpaths, preinclude):
    xml = indent(1) + '<configurations>\n'
    xml += xml_configuration(build_name, release_name, defs, incpaths, preinclude)
    xml += indent(1) + '</configurations>\n'
    return xml

def file_path_as_list( path ):
    lst = []
    while True:
        path, folder = os.path.split(path)
        if folder != "":
            lst.append(folder)
        else:
            if path != "":
                lst.append(path)
            break
    lst.reverse()
    return lst

def file_list_as_tree( files ):
    root = {}
    for path in files:
        lst = file_path_as_list(path);
        tree = root
        for f in lst:
            if f not in tree:
                tree[f] = {}
            tree = tree[f]
    return root

def files_in_dir(directory, extensions):
    for root, dirnames, filenames in os.walk(directory):
        for filename in filenames:
            if filename.endswith(extensions):
                yield os.path.join(root, filename)

def dep_files_in_dir(directory):
    return files_in_dir(directory, ".d")

def project_files_in_dir(directory):
    endings = (
        # Sources
        ".c", ".h", ".ch", ".s", ".asm",
        # Scripts
        ".py", ".pl", ".ld",
        # Build
        "makefile", ".inc", ".mk", ".rules", ".vcproj", ".sln", ".dist",
        # Documents
        "Doxyfile", "README", ".txt",
        # Config
        ".fsm", ".tpl", ".xml", ".yaml", ".in", ".str", ".xsd", ".cfg")

    return files_in_dir(directory, endings)


def get_files_for_project(directory):
    return [os.path.normpath('fw/src/' + f) for f in project_files_in_dir(directory)]

def dep_file_deps(fp):
    lines = fp.readlines()
    if lines:
        # Strip targets from the first line, they're not dependencies
        deps = lines[0].split(':')[-1].split()

        # Remove new line characters and backslashes
        lines = [l.lstrip().rstrip(' \\\n') for l in deps + lines[1:]]

        # Remove empty strings
        lines = filter(None, lines)
    return lines

def dep_filename_deps(d_filename):
    with open(d_filename) as fp:
        return dep_file_deps(fp)
    return None

def find_sources_for_build(build):
    sources = set([d for f in dep_files_in_dir(build) for d in dep_filename_deps(f)])

    # Remove system header dependencies, these appear as absolute paths.
    return [f for f in sources if not os.path.isabs(f)]

def get_dependency_file_list(release):
    # Scan the build directory for a list of sources in dependency files.
    return find_sources_for_build('../build/' + release)

def get_linkscripts(linkscript):
    linkscripts = [linkscript]
    with open(linkscript) as fp:
        for l in fp:
            if l.startswith("INCLUDE "):
                name, sep, _ = l.split()[1].partition('.ld')
                child = os.path.dirname(linkscript) + "/" + name + sep
                linkscripts += get_linkscripts(child)
    return linkscripts

def relative_to_current_directory(paths):
    return ' '.join([os.path.relpath(p.strip()) for p in paths.split()])

def read_def_string_from_file(defsfile):
    if not defsfile:
        return ""

    defs = []
    with open(defsfile) as fp:
        for l in fp:
            tokens = l.split()
            command, define = tokens[:2]
            if command == "#define":
                try:
                    define += "=" + tokens[2]
                except IndexError:
                    pass
                defs.append(define)

    return ' '.join(defs)

def get_files_for_release(release):
    dependencies = get_dependency_file_list(options.ReleaseName)
    linkscripts = get_linkscripts(options.Linkscript)

    # File paths are currently relative to the 'src' directory.
    # Make them relative to the top of the project and remove extra '.' and '..'.
    files = [os.path.normpath('fw/src/' + f) for f in dependencies + linkscripts]
    return files

def build_xml_project_file(options):
    build_name = options.OutputFileName.split('/')[-1].split('.')[0]

    if options.ReleaseName:
        files = get_files_for_release(options.ReleaseName)
    else:
        files = get_files_for_project('../..')

    file_tree = file_list_as_tree(files)

    relative_includes = relative_to_current_directory(options.Includes)
    relative_defines = relative_to_current_directory(options.Defines)

    defs = read_def_string_from_file(relative_defines)

    xml = '<?xml version="1.0" encoding="UTF-8"?>\n'
    xml += '<project path="">\n'
    # The project path is from the top level directory, but file references
    # should be as if from fw/src, so prefix with '../..'
    xml += xml_file_tree(file_tree, '../..')
    xml += xml_configurations(build_name, options.ReleaseName, defs, relative_includes,
                              relative_defines)
    xml += '</project>\n'
    return xml

from optparse import OptionParser
if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-o", dest = "OutputFileName", default = "project.x2p",
                      type = "str", help = "Specify output file path and name")
    parser.add_option("-r", dest = "ReleaseName", default = "", type = "str",
                      help = ("The name of the release makefile. If this isn't "
                              "specified the script will use all project "
                              "related files from the top level directory for "
                              "the file list. Assuming the script has been "
                              "launched from the fw/src directory."))
    parser.add_option("-i", dest = "Includes", default = "", type = "str",
                      help = "A list of include directories")
    parser.add_option("-d", dest = "Defines", default = "", type = "str",
                      help = "A header file containing a list of defines to set when compiling")
    parser.add_option("-l", dest = "Linkscript", default = "", type = "str",
                      help = "The path to the link script")
    options, _ = parser.parse_args()

    xml = build_xml_project_file(options)

    with open(options.OutputFileName, "wb") as out:
        out.write(xml)
