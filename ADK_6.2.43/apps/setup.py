# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
import os
import sys
import stat
import shutil
from argparse import ArgumentParser
import glob
import fnmatch
import fileinput
import tempfile

# If another build variant is added to the app_ss builds then
# add the build dir to the key and update the value to the 
# name of the subdir you want to use for this build.
valid_build_dictionary = {"crescendo_p1_d00_lib" :  "csra68100_d00",
                          "crescendo_p1_d01_lib" :  "csra68100_d01",
                          "crescendo_p1_d00" : "csra68100_d00",
                          "crescendo_p1_d01" : "csra68100_d01",
                          "aura_p1_d00_lib" : "qcc3400_d00",
                          "aura_p1_d00" : "qcc3400_d00",
                          "aura_p1_d01_lib" : "qcc3400_d01",
                          "aura_p1_d01" : "qcc3400_d01",
                          "qcc512x_qcc302x_lib" : "qcc512x_qcc302x",
}


def setup_args(custom_args = None):
    parser = ArgumentParser()
    parser.add_argument("-i", "--install_dir", help="Installation Directory",
                        dest="install_dir", default=None)
    parser.add_argument("-a", "--apps_dir", help="Top level firmware directory",
                        dest="apps_dir",default=None)
    parser.add_argument("-b", "--build_to_copy", help="The build to copy, default is all found",
                        dest="build_to_copy",default=None)
    parser.add_argument("--headers_only", action="store_true",
                        help="Only copy headers, not libraries", default=False)

    return parser.parse_args()


def create_trapset_h(path_to_build_defs, path_to_trapset_h):
    """
    Extract the trapset defines from build_defs.h and create a
    file called trapset.h with these defines in it.
    """
    trapset_defines = list()
    trapset_header_directory =  os.path.dirname(path_to_trapset_h)

    try:
        os.remove(os.path.join(path_to_trapset_h))

    except OSError:
        pass


    with open(path_to_build_defs) as build_def_file:
        for line in build_def_file:
            if "TRAPSET_" in line:
                trapset_defines.append(line)

    build_def_file.close()

    if not os.path.exists(trapset_header_directory):
        os.makedirs(trapset_header_directory)

    trapset_header_file = open(path_to_trapset_h, 'w')
    trapset_header_file.write("#ifndef __TRAPSETS_H__\n")
    trapset_header_file.write("#define __TRAPSETS_H__\n")

    trapset_defines.sort()

    for define in trapset_defines:
        trapset_header_file.write(define)

    trapset_header_file.write("#endif /* __TRAPSETS_H__ */\n")
    trapset_header_file.close()



def do_copy(source_file, dest_file):
    """
    Copy a file from source file to dest file. Overwrite dest_file
    if it already exists. Create the target directory if necessary
    """
    print "copying file " + source_file + " to " + dest_file
    target_directory = os.path.dirname(dest_file)

    try:
        os.chmod(dest_file, stat.S_IWRITE)
    except:
        pass

    if not os.path.exists(target_directory):
        os.makedirs(target_directory)

    try:
        shutil.copy2(source_file, dest_file)
    except:
        raise



def file_copy(source_path, dest_dir, recursive = False):

    if recursive:
        filter = os.path.basename(source_path)
        source_dir = os.path.dirname(source_path)

        for dirpath, dirnames, files in os.walk(source_dir):
            subdir = os.path.relpath(dirpath, source_dir)
            for file in fnmatch.filter(files, filter):
                filename = os.path.join(subdir, file)
                destination_file = os.path.join(dest_dir, filename)
                source_file = os.path.join(source_dir, filename)
                do_copy(source_file, destination_file)

    else:
        for file in glob.glob(source_path):
            destination_file = os.path.join(dest_dir, os.path.basename(file))
            do_copy(file, destination_file)


def replace_string_in_file(filename, original_string, new_string):
    """
    Replace original string with new_string in filename
    """
    print "Replacing " + original_string + " with " + new_string + " in " + filename
    for i, line in enumerate(fileinput.input(filename, inplace=1)):
        sys.stdout.write(line.replace(original_string, new_string)) 

    backup_file = filename + ".bak"

    try:
        os.chmod(backup_file, stat.S_IWRITE)
        os.remove(backup_file)
    except:
        pass

def get_build_list(path, build_to_copy, headers_only=False):
    """
    Returns a dictonary containing the valid builds at path
    """
    if build_to_copy:
        builds = [build_to_copy]
    else:
        builds = os.listdir(path)
    build_dirs = dict()
    for build in builds:
        if build in valid_build_dictionary:
            valid_glob = (os.path.join(path, build, "customer","core", 
                                       "trap_api", "api.h") if headers_only else 
                          os.path.join(path, build, "lib*.a"))
            if glob.glob(valid_glob):
                build_dirs[build] = valid_build_dictionary[build]
    return build_dirs

def get_archive_name_for_build(build_to_copy):
    if build_to_copy == "aura_p1_d00_lib" or build_to_copy == "aura_p1_d01_lib":
        return "libaura_app_p1.a"
    elif build_to_copy == "crescendo_p1_d01_lib":
        return "libcrescendo_app_p1.a"
    elif build_to_copy == "qcc512x_qcc302x_lib":
        return "libqcc512x_qcc302x.a"
    else:
        raise RuntimeError("Unsupported build target " + build_to_copy);

def setup_build_firmware(usedDirList, apps_dir, install_dir, build_to_copy, headers_only=False):
    build_list = get_build_list(os.path.join(apps_dir, "fw", "build"), build_to_copy,
                                headers_only=headers_only)

    if len(build_list) == 0:
        raise RuntimeError("Error no OS builds found")

    # Iterate through the valid available builds
    for build_dir in build_list:

        firmware_include_dir = "firmware_" + build_list[build_dir]
        fwinc = os.path.join("include", firmware_include_dir)
        usedDirList.append(fwinc)

        create_trapset_h(
                os.path.join(apps_dir, "fw", "build", build_dir, "build_defs.h"),
                os.path.join(install_dir, fwinc, "trapsets.h"))
        
        file_copy(
            os.path.join(apps_dir, "fw", "build", build_dir,
                                        "customer", "core", "trap_api", "*.h"),
            os.path.join(install_dir, fwinc))

        file_copy(
            os.path.join(apps_dir, "fw", "src", "core", "hydra_log", "hydra_log*.h"),
            os.path.join(install_dir, fwinc),
            )

        file_copy(
            os.path.join(apps_dir, "common", "interface", "app", "*.h"),
            os.path.join(install_dir, fwinc, "app"),
            True)

        file_copy(os.path.join(apps_dir, "fw", "src", "core", "hydra_log", "hydra_log.h"),
                  os.path.join(install_dir, fwinc))

        file_copy(os.path.join(apps_dir, "fw", "src", "core", "hydra", "hydra_macros.h"),
                  os.path.join(install_dir, fwinc))

        file_copy(os.path.join(apps_dir, "fw", "src", "nfc", "nfc_shared", "nfc_tool_stream.h"),
                  os.path.join(install_dir, fwinc))

        replace_string_in_file(
            os.path.join(install_dir, fwinc, "app", "bluestack", "hci.h"),
            "hydra/hydra_types.h",
            "hydra_types.h")

        replace_string_in_file(
            os.path.join(install_dir, fwinc, "hydra_log.h"),
            "hydra_log/hydra_log_firm.h",
            "hydra_log_firm.h")

        replace_string_in_file(
            os.path.join(install_dir, fwinc, "hydra_log.h"),
            "hydra_log/hydra_log_soft.h",
            "hydra_log_soft.h")

        replace_string_in_file(
            os.path.join(install_dir, fwinc, "hydra_log.h"),
            "hydra_log/hydra_log_disabled.h",
            "hydra_log_disabled.h")

        replace_string_in_file(
            os.path.join(install_dir, fwinc, "hydra_log.h"),
            "hydra/hydra_macros.h",
            "hydra_macros.h")

        replace_string_in_file(
            os.path.join(install_dir, fwinc, "hydra_log_firm.h"),
            "hydra/hydra_types.h",
            "hydra_types.h")

        replace_string_in_file(
            os.path.join(install_dir, fwinc, "hydra_log_disabled.h"),
            "hydra_log/hydra_log.h",
            "hydra_log.h")

        replace_string_in_file(
            os.path.join(install_dir, fwinc, "app", "bluestack", "bluetooth.h"),
            "bluestack/types.h",
            "types.h")

        if not headers_only:
            archive_name = get_archive_name_for_build(build_to_copy)
            libos = os.path.join("lib", "os", build_list[build_dir])
            usedDirList.append(libos)
            file_copy(os.path.join(apps_dir, "fw", "build", build_dir, archive_name),
                      os.path.join(install_dir, libos))
            
            do_copy(os.path.join(apps_dir, "fw", "build", build_dir, "customer", "core", "init", "init_usb_early_init.a"),
                      os.path.join(install_dir, libos, "libusb_early_init.a"))

    # These files don't come out of the build directory and aren't grouped with
    # files that are, so will be the same for all flavours of build.
    sysinc = os.path.join("include", "standard")
    usedDirList.append(sysinc)
    file_copy(os.path.join(apps_dir, "fw","src","core","include","macros.h"), 
              os.path.join(install_dir, sysinc))

    file_copy(os.path.join(apps_dir, "fw", "src", "core", "hydra", "hydra_types.h"),
              os.path.join(install_dir, sysinc))

    file_copy(os.path.join(apps_dir, "fw", "src", "customer", "core", "trap_api", "csrtypes.h"),
              os.path.join(install_dir, sysinc))

    file_copy(os.path.join(apps_dir, "fw", "src", "core", "include", "types.h"),
              os.path.join(install_dir, sysinc))


    replace_string_in_file(os.path.join(install_dir, sysinc, "csrtypes.h"),
                           "hydra/hydra_types.h",
                           "hydra_types.h")

    replace_string_in_file(os.path.join(install_dir, sysinc, "macros.h"),
                           "BITFIELD(",
                           "NOT_A_BITFIELD(")

def remove_readonly(func, path, _):
    os.chmod(path, stat.S_IWRITE)
    func(path)

def sync_dirs(current_dir, new_dir, usedDirList):
    # For each directory in usedDirList, remove the existing one and replace it
    # with a fresh one.  This preserves the contents of other directories within
    # installed_libs created by other subsystems.
    for i in usedDirList:
        destDir = os.path.join(current_dir,i)
        if os.path.exists(destDir):
            if os.path.isdir(destDir):
                shutil.rmtree(destDir,onerror=remove_readonly)
            else:
                raise RuntimeError("Expected " + destDir + " to be a directory")
        shutil.move(os.path.join(new_dir,i),destDir)

def doSync(current_dir, new_dir, usedDirList):
    if not os.path.exists(current_dir):
        # current doesn't exist, so the easy answer is just
        # rename the temporary directory.
        os.rename(new_dir,current_dir)
    else:
        # Perform a synchronise and delete the temporary.
        sync_dirs(current_dir,new_dir,usedDirList)
        shutil.rmtree(new_dir,True)

if __name__ == "__main__":

    args = setup_args()
    here = os.path.normpath(os.path.dirname(__file__))
    install_dir = None
    apps_dir = None

    if not args.install_dir:
        install_dir = os.path.join(here, "..", "vm", "installed_libs")
    else:
        install_dir = args.install_dir

    if not args.apps_dir:
        apps_dir = here
    else:
        apps_dir = args.apps_dir

    # Create a temporary directory to create the setup in
    install_dir_new = tempfile.mkdtemp(dir='.')

    # Setup
    usedDirList = []
    setup_build_firmware(usedDirList, apps_dir, install_dir_new, args.build_to_copy,
                         args.headers_only)

    # Sync the new setup with an existing setup
    doSync(install_dir, install_dir_new, usedDirList)
