############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2014 - 2017 Qualcomm Technologies International, Ltd.
#
############################################################################
import imp
import sys
import os
import build_controller
import download_build_info
import capability
import get_python_tools
import make_kdc
import make_kcs
from kcs import KCSFile
import shutil
import glob
import logging
import chip_info
import re
import bundles_for_config

global chip

def get_immediate_subdirectories(a_dir):
    return [name for name in os.listdir(a_dir) if os.path.isdir(os.path.join(a_dir, name))]

def get_obj_files():
    prebuiltlibReStart = re.compile("PREBUILT_LIBS = (.*\.a)")
    o_files = []
    #cap_output_path = os.path.join(KCS_OUTPUT_DIR, "capabilities/"+cap_name+"/debugobj")
    cap_output_path = os.path.join(KCS_OUTPUT_DIR, "capabilities")
    comp_output_path = os.path.join(KCS_OUTPUT_DIR, "components")
    download_output_path = os.path.join(KCS_OUTPUT_DIR, "download")
    lib_output_path = os.path.join(KCS_OUTPUT_DIR, "lib")
    lib_private_output_path = os.path.join(KCS_OUTPUT_DIR, "lib_private")
    lib_prebuilt_libs_path = os.path.join(KYMERA_SRC_PATH, "lib_release", DOWNLOAD_CONFIG)
    cap_names = get_immediate_subdirectories(cap_output_path)
    cfg_mkf = os.path.join(KCS_OUTPUT_DIR, DOWNLOAD_CONFIG + ".mkf")
    prebuilt_lib_names = []
    f = open(cfg_mkf, "r")
    matched = False
    for line in f:
        if matched:
            if line.endswith("\\"):
                prebuilt_lib_names.extend([line.lstrip().rstrip(" \\")])
            else:
                break
        match = prebuiltlibReStart.match(line)
        if match != None:
            prebuilt_lib_names.extend([match.group(1)])
            if line.endswith("\\"):
                matched = True
            else:
                break
    f.close()
    # We might not have any dependency on libraries or components, so catch exceptions if those
    # folders are missing
    try:
        comp_names = get_immediate_subdirectories(comp_output_path)
    except OSError as err:
        # errno 2 means "No such file or directory"
        if err.errno == 2:
            comp_names = []
        else:
            raise(err)
    try:
        lib_names = get_immediate_subdirectories(lib_output_path)
    except OSError as err:
        # errno 2 means "No such file or directory"
        if err.errno == 2:
            lib_names = []
        else:
            raise(err)
    try:
        lib_private_names = get_immediate_subdirectories(lib_private_output_path)
    except OSError as err:
        # errno 2 means "No such file or directory"
        if err.errno == 2:
            lib_private_names = []
        else:
            raise(err)
    logging.info("cap_names" + str(cap_names))
    logging.info("comp_names" + str(comp_names))
    logging.info("lib_names" + str(lib_names))
    logging.info("lib_private_names" + str(lib_private_names))
    logging.info("prebuilt_lib_names" + str(prebuilt_lib_names))
    for cap in cap_names:
        # Exclude processing intermediate dyn object files
        o_files.extend([os.path.join(os.path.join(cap_output_path,cap),"debugbin","lib{}.a".format(cap))])
    for comp in comp_names:
        o_files.extend([os.path.join(os.path.join(comp_output_path,comp),"debugbin","lib{}.a".format(comp))])
    for lib in lib_names:
        o_files.extend([os.path.join(os.path.join(lib_output_path,lib),"debugbin","lib{}.a".format(lib))])
    for lib_private in lib_private_names:
        o_files.extend([os.path.join(os.path.join(lib_private_output_path,lib_private),"debugbin","lib{}.a".format(lib_private))])
    for prebuilt_lib in prebuilt_lib_names:
        o_files.extend([os.path.join(os.path.join(lib_prebuilt_libs_path,prebuilt_lib))])
    # Add C runtime libraries to the list of object files that could be pulled in
    crt_path = get_kcc_version.kcc_version().get_kcc_path(OS_TYPE)
    crt_path = os.path.join(crt_path, "lib")
    crt_path = os.path.join(crt_path, chip.get_crt_lib_name())
    o_files.extend([os.path.join(crt_path,"libc.a")])
    o_files.extend([os.path.join(crt_path,"libcrt.a")])
    o_files.extend([os.path.join(crt_path,"libfle.a")])
    # Add kdc_start object file
    dnld_obj_path = os.path.join(download_output_path,"debugobj")
    o_files.extend([os.path.join(dnld_obj_path,"kdc_start.o")])
    return o_files

def find_build_name(path):
    build_name=""
    # Return first the _external
    list_elfs = filter(lambda x: x.endswith(".elf"), os.listdir(path))
    # remove extension
    list_elfs = map(lambda x: os.path.splitext(x)[0], list_elfs)
    if len(list_elfs) > 2:
        raise Exception("Unexpected amount of ELF ({num_elfs}) files found in {mypath}".format(num_elfs=len(list_elfs), mypath=path))
    elif len(list_elfs) == 0:
        raise Exception("No ELF files found in {mypath}".format(mypath=path))
    # If there are two ELF files we expect one of them to be the external version of the other
    if len(list_elfs) == 2:
        try:
            external_build_name = filter(lambda x: x.endswith("_external"), list_elfs)[0]
        except:
            raise Exception("Couldn't find external ELF")
        try:
            internal_build_name = filter(lambda x: not x.endswith("_external"), list_elfs)[0]
        except:
            raise Exception("Couldn't find internal ELF")
        if internal_build_name != external_build_name.replace("_external",""):
            raise Exception("{first} and {second} names do not match!".format(first=internal_build_name, second=external_build_name))
        else:
            return external_build_name
    return list_elfs[0]

def proc_cmd_line():
    "Processes the command line arguments passed to KCSMaker"
    # Define globals that we might edit
    global DOWNLOAD_CONFIG
    global RELEASE_FLAG
    global BUNDLE_NAME
    global BUILD_DIR
    global BUILD_NAME
    global BUILD_ROOT
    global BUILD_ID
    global KCS_OUTPUT_DIR
    global KCSMAKER_PATH
    global CAPABILITIES_PATH
    global LABEL_NAME
    global EXTRA_INCLUDE
    global KYMERA_SRC_PATH
    global LINKSCRIPT_PATH
    global OS_TYPE
    global MAKE_PATH
    global clean_build

    if 2 > len(sys.argv):
        raise Exception("No enough input arguments. Requires <bundle name>")

    KCSMAKER_PATH = os.path.abspath(os.path.dirname(sys.argv[0]))
    # Set initial and default values
    RELEASE_FLAG = False
    DOWNLOAD_CONFIG = ""
    BUNDLE_NAME = ""
    KCS_OUTPUT_DIR = ""
    BUILD_ID = 0xFFFF
    LABEL_NAME = ""
    BUILD_DIR = ""
    BUILD_ROOT = ""
    BUILD_NAME = ""
    EXTRA_INCLUDE= ""
    KYMERA_SRC_PATH = ""
    LINKSCRIPT_PATH = ""
    MAKE_PATH = "make"
    clean_build = None

    # Flag whether BUILD_DIR has been assigned. It can be assigned by -buildpath
    # and by -buildid. The use of both causes one to override the other and is
    # not permitted.
    build_id_flag = False
    build_config_flag = False
    build_path_flag = False

    for arg in sys.argv[1:]:
        if "RELEASE" in arg:
            val = arg.split("=")[1]
            if ("False" == val) | ("false" == val):
                RELEASE_FLAG = False
            elif ("True" == val) | ("true" == val):
                RELEASE_FLAG = True
            else:
                raise Exception("Unrecognised command line value for RELEASE: " + str(val))
        elif "-ostype" in arg:
            OS_TYPE = arg.split("=")[1]
        elif "-makepath" in arg:
            MAKE_PATH = arg.split("=")[1]
        elif "-noclean" == arg:
            clean_build = False
        elif "-clean" == arg:
            clean_build = True
        elif "-extrainclude" in arg:
            EXTRA_INCLUDE = arg.split("=")[1]
        elif "-bundlename" in arg:
            BUNDLE_NAME = arg.split("=")[1]
        elif "-linkscriptpath" in arg:
            LINKSCRIPT_PATH = arg.split("=")[1]
        elif "-buildpath" in arg:
            if build_id_flag:
                raise Exception("-buildpath and -buildid arguments cannot be used together")
            build_path_flag = True
            BUILD_DIR = os.path.abspath(arg.split("=")[1])
        elif "-buildname" in arg:
            BUILD_NAME = arg.split("=")[1]
        elif "-buildroot" in arg:
            BUILD_ROOT = os.path.abspath(arg.split("=")[1])
        elif "-config" in arg:
            if build_id_flag:
                raise Exception("-config and -buildid arguments cannot be used together")
            build_config_flag = True
            DOWNLOAD_CONFIG = arg.split("=")[1]
        elif "-buildid" in arg:
            if build_path_flag == True or build_config_flag == True:
                raise Exception("-buildpath or -config arguments cannot be used together with -buildid")
            build_dir_or_config_flag = True
            BUILD_ID = arg.split("=")[1]
            import build_finder
            bf = build_finder.build_finder()
            BUILD_DIR = bf.get_build_dir(BUILD_ID)
            BUILD_ROOT = os.path.abspath(os.path.join(BUILD_DIR, "..", "..", "..", "build"))
            BUILD_ID = int(BUILD_ID, 10)
            # Make sure the requested download config matches the target config
            # We get the target config from the BUILD_DIR path
            DOWNLOAD_CONFIG = os.path.basename(os.path.abspath(os.path.join(BUILD_DIR, "..")))
        elif "-outputdir" in arg:
            KCS_OUTPUT_DIR = os.path.abspath(arg.split("=")[1])
        elif "-p4label" in arg:
            LABEL_NAME = arg.split("=")[1]
        elif "-kymerasourcepath" in arg:
            KYMERA_SRC_PATH = arg.split("=")[1]
        else:
            raise Exception("Unrecognised command line argument: " + str(arg))

    if DOWNLOAD_CONFIG == "":
        raise Exception("No -config or -buildid provided. Please provide at least one.")
    # Set a default bundle_name in case none is provided
    if BUNDLE_NAME == "":
        BUNDLE_NAME = DOWNLOAD_CONFIG + "_kcs"
    # If BUILD_DIR is not provided, get it from KCSMAKER_PATH (this is not a release)
    if BUILD_DIR == "":
        BUILD_DIR = os.path.abspath(os.path.join(KCSMAKER_PATH,"..", "..", "output", DOWNLOAD_CONFIG, "build"))
        BUILD_ROOT = os.path.abspath(os.path.join(KCSMAKER_PATH, "..", "..", "build"))
        KYMERA_SRC_PATH = os.path.abspath(os.path.join(KCSMAKER_PATH, "..", ".."))
    else:
        if KYMERA_SRC_PATH == "":
            # Get capabilities src path from the build we are linking against if path hasn't been set previously
            KYMERA_SRC_PATH = os.path.abspath(os.path.join(BUILD_DIR, "..", "..", ".."))
    CAPABILITIES_PATH = os.path.abspath(os.path.join(KYMERA_SRC_PATH,"capabilities"))
    # if BUILD_NAME not provided by command line, resolve it by inspecting output .elf file
    if BUILD_NAME == "":
        BUILD_NAME=find_build_name(os.path.join(BUILD_DIR, "debugbin"))

    if KCS_OUTPUT_DIR == "":
        # For not release builds, use a special folder name, otherwise use KCSMAKER_PATH/output/<BUILD_ID>/<DOWNLOAD_CONFIG>
        if BUILD_ID == 0xFFFF:
            KCS_OUTPUT_DIR = os.path.abspath(os.path.join(KCSMAKER_PATH, "out", DOWNLOAD_CONFIG))
        else:
            KCS_OUTPUT_DIR = os.path.abspath(os.path.join(KCSMAKER_PATH, "out", str(BUILD_ID), DOWNLOAD_CONFIG))

if __name__ == "__main__":
    # This needs at least version 2.6 of Python (needs the as keyword) not checked
    # because Python dies before doing this check
    # Process command line arguments
    logging.basicConfig(filename="KCS_build.log",format='%(asctime)s %(message)s', filemode="w", level=logging.INFO)
    logging.info("Initialising KCSMaker...")
    proc_cmd_line()
    bdl_json_path = os.path.join(KCSMAKER_PATH,"bundle","bundle_list.json")
    if os.path.isfile(bdl_json_path):
        bundle_defs = bundles_for_config.get_bundle_definitions(bdl_json_path)
        if BUNDLE_NAME not in map(lambda x: os.path.splitext(x)[0], bundle_defs):
            raise Exception("Error. Bundle {b} couldn't be found in {p}. Please manually add an entry for this bundle".format(b=BUNDLE_NAME,p=os.path.join(KCSMAKER_PATH,"bundle","bundle_list.json")))
        bundle_list=bundles_for_config.determine_bundles_for_config(DOWNLOAD_CONFIG, bundle_defs)
        if BUNDLE_NAME not in bundle_list:
            raise Exception("Error. The bundle registry {p} explicitly excludes {b} for the configuration {c}. If you don't think this is correct, please fix the registry".format(b=BUNDLE_NAME,p=os.path.join(KCSMAKER_PATH,"bundle","bundle_list.json"),c=DOWNLOAD_CONFIG))
    # Can't import get_kcc_version until this point because we
    # need to get it from the target build path.
    sys.path.append(BUILD_ROOT)
    # Sort out paths for python tools
    sys.path.append(get_python_tools.python_tools_version().get_python_tools_path(OS_TYPE))
    import kalaccess
    import get_kcc_version

    # Read in the existing build info for the release
    logging.info("Loading release build info...")
    ktools_path = os.path.join(get_kcc_version.kcc_version().get_kcc_path(OS_TYPE), "bin")
    rel_binfo = download_build_info.build_info(BUILD_DIR, BUILD_NAME, ktools_path, KCS_OUTPUT_DIR)
    logging.info("DONE")

    # Get the BUILD_ID from Kymera ELF
    BUILD_ID=rel_binfo.get_build_id()

    try:
        mod = imp.load_source("mod", os.path.abspath(os.path.join(KCSMAKER_PATH, "bundle", BUNDLE_NAME + ".bdl")))
    except IOError:
        logging.error("Can't find file %s" % os.path.abspath(os.path.join(KCSMAKER_PATH, "bundle", BUNDLE_NAME + ".bdl")))
        raise
    try:
        if (len(mod.KDCs) >0):
            logging.info("Building KCS %s" % BUNDLE_NAME)
        else:
            logging.error("Error. No KDCs to be built")
            exit()
    except (IOError):
        logging.error("Error. KDCs variable NOT found in %s" % os.path.join(KCSMAKER_PATH, BUNDLE_NAME + ".bdl"))
        raise

    chip = chip_info.chip_info(rel_binfo.get_chipname())
    if chip.is_supported() == False:
        raise Exception("Chip NOT supported: " + str(chip.get_string()))

    if LINKSCRIPT_PATH == "":
        LINKSCRIPT_PATH = os.path.join(KCSMAKER_PATH, "templates", chip.get_linkscript_template_name())

    logging.info("DOWNLOAD_CONFIG = %s" % DOWNLOAD_CONFIG)
    logging.info("RELEASE_FLAG = %s" % RELEASE_FLAG)
    logging.info("CAPABILITIES_PATH = %s" % CAPABILITIES_PATH)
    logging.info("KYMERA_SRC_PATH = %s" % KYMERA_SRC_PATH)
    logging.info("BUNDLE_NAME = %s" % BUNDLE_NAME)
    logging.info("BUILD_DIR = %s" % BUILD_DIR)
    logging.info("BUILD_NAME = %s" % BUILD_NAME)
    logging.info("BUILD_ROOT = %s" % BUILD_ROOT)
    logging.info("BUILD_ID = %s" % BUILD_ID)
    logging.info("KCS_OUTPUT_DIR = %s" % KCS_OUTPUT_DIR)
    logging.info("KCSMAKER_PATH = %s" % KCSMAKER_PATH)
    logging.info("LINKSCRIPT_PATH = %s" % LINKSCRIPT_PATH)
    logging.info("LABEL_NAME = %s" % LABEL_NAME)

    kcsFileT = os.path.join(KCS_OUTPUT_DIR, "download", "debugbin", BUNDLE_NAME + ".kcs")
    kcsFileB = os.path.join(KCS_OUTPUT_DIR, "download", "debugbin", BUNDLE_NAME + ".dkcs")
    kcsFileD = os.path.join(KCS_OUTPUT_DIR, "download", "debugbin", BUNDLE_NAME + ".kcs_DRAM")

    makeKCS = make_kcs.make_kcs(chip, BUILD_ID)

    logging.info("Generating KCS file " + kcsFileT + " ...")

    # Create output file if it doesn't exist
    if not os.path.exists(KCS_OUTPUT_DIR):
            os.makedirs(KCS_OUTPUT_DIR)
    
    # Check if we are building on the same bundle. If not then force a clean build
    if (os.path.exists(os.path.join(KCS_OUTPUT_DIR,"last_bundle.txt"))):
        last_bundle_file = open(os.path.join(KCS_OUTPUT_DIR,"last_bundle.txt"),"r")
        prev_bundle = last_bundle_file.readline()
        if prev_bundle != BUNDLE_NAME:
            clean_build = True
            logging.info("Previous build detected: {prev_build}, now building: {cur_build}. Forcing clean build".format(prev_build=prev_bundle, cur_build=BUNDLE_NAME))
        last_bundle_file.close()
    else:
        clean_build = True

    if clean_build == None:
        clean_build = RELEASE_FLAG

    kdc_count = 1
    for kdc_name in mod.KDCs.keys():
        logging.info("Generating KDC " + kdc_name)
        logging.info("Building " + str(mod.KDCs[kdc_name]) + " capabilities ...")
        cap_list = []
        for cap_tuple in mod.KDCs[kdc_name]:
            cap_list.append(capability.Capability(cap_tuple[0], cap_tuple[1], cap_tuple[2], cap_tuple[3], CAPABILITIES_PATH))
        # Build the elf and object files for each capability
        # Create a Build_Controller object to control the process

        if clean_build:
            # If Make sure the KDC file is created with an empty output directory
            for dir_or_file in os.listdir(KCS_OUTPUT_DIR):
                dir_or_file_abs = os.path.join(KCS_OUTPUT_DIR,dir_or_file)
                if os.path.isdir(dir_or_file_abs):
                    # Don't delete this temp directory used to store kmap files
                    if dir_or_file !=  "kmap_tmp":
                        shutil.rmtree(dir_or_file_abs)
                else:
                    # It's a file, remove it
                    os.remove(dir_or_file_abs)
        
        # Write current bundle into a file
        last_bundle_file = open(os.path.join(KCS_OUTPUT_DIR,"last_bundle.txt"),"w")
        last_bundle_file.write(BUNDLE_NAME)
        last_bundle_file.close()
    
        build = build_controller.Build_Controller(BUILD_ROOT, BUILD_DIR, KCS_OUTPUT_DIR, KCSMAKER_PATH, DOWNLOAD_CONFIG, rel_binfo.get_elf_name(), EXTRA_INCLUDE, BUILD_NAME, KYMERA_SRC_PATH, OS_TYPE)
        build.build(rel_binfo, cap_list, kdc_name, LINKSCRIPT_PATH, MAKE_PATH, clean_build)
        logging.info(str(mod.KDCs[kdc_name]) + " capabilities successfully built.")
        # Copy kmap file to temp folder (it will get deleted on next KDC build)
        if not os.path.exists(os.path.join(KCS_OUTPUT_DIR,"kmap_tmp")):
            os.makedirs(os.path.join(KCS_OUTPUT_DIR,"kmap_tmp"))
        shutil.copy(os.path.join(KCS_OUTPUT_DIR, "download", "debugbin", kdc_name + ".kmap"),os.path.join(KCS_OUTPUT_DIR,"kmap_tmp", kdc_name + "(KDC).kmap"))
        os.chdir(KCSMAKER_PATH)
        objDumpPath = os.path.join(get_kcc_version.kcc_version().get_kcc_path(OS_TYPE), "bin", "kobjdump")
        oFiles = get_obj_files()
        logging.info("oFiles" + str(oFiles))
        dnldElfFile = os.path.join(KCS_OUTPUT_DIR, "download", "debugbin", kdc_name + ".elf")
        mapOutFile = os.path.join(KCS_OUTPUT_DIR, "download", "debugbin", "map_out.txt")
        makeKDC = make_kdc.make_kdc(dnldElfFile, rel_binfo.get_elf_name(), chip, objDumpPath, oFiles, cap_list, OS_TYPE, mapOutFile)
        kdc = makeKDC.generateKDC(kdc_count,rel_binfo.get_rom_kalaccess())
        print("Sizes for KDC {}:".format(dnldElfFile))
        kdc.print_memory_usage()
        makeKCS.addKDC(kdc)
        kdc_count = kdc_count + 1

    # Copy kmap files to the output folder where the KCS files are
    files = glob.iglob(os.path.join(KCS_OUTPUT_DIR, "kmap_tmp", "*.kmap"))
    for file in files:
        if os.path.isfile(file):
            if clean_build:
                shutil.move(file, os.path.join(KCS_OUTPUT_DIR,"download", "debugbin"))
            else:
                shutil.copy(file, os.path.join(KCS_OUTPUT_DIR,"download", "debugbin"))
    if clean_build:
        os.rmdir(os.path.join(KCS_OUTPUT_DIR,"kmap_tmp"))
    kcs = makeKCS.generateKCS()
    kcs.dumpToTextFile(kcsFileT)
    kcs.dumpToBinaryFile(kcsFileB)
    kcs.dumpToDRAMFile(kcsFileD)
    if RELEASE_FLAG:
        import release
        this_rel = release.release(RELEASE_FLAG, KCSMAKER_PATH, BUILD_ID, BUNDLE_NAME, DOWNLOAD_CONFIG)
        # Generate new release ID
        this_rel.generate_new_version_id()
        logging.info("New BUILD ID: " + str(this_rel.get_version_id()))
        this_rel.create_log()
        label = this_rel.label()
        this_rel.write_log("Label for released code " + str(label))
        this_rel.archive_KCS()
    else:
        # Copy binaries to a common folder so they don't get erased when a new build is triggered
        if not os.path.exists(os.path.join(KYMERA_SRC_PATH,"output_bundles", DOWNLOAD_CONFIG, BUNDLE_NAME)):
            os.makedirs(os.path.join(KYMERA_SRC_PATH,"output_bundles",DOWNLOAD_CONFIG, BUNDLE_NAME))
        file_ext_list = [".dkcs",".kcs", ".elf", ".kmap", ".lst", ".sym"]
        for file_ext in file_ext_list:
            shutil.copy(os.path.join(KCS_OUTPUT_DIR,"download", "debugbin",BUNDLE_NAME + file_ext),os.path.join(KYMERA_SRC_PATH,"output_bundles",DOWNLOAD_CONFIG, BUNDLE_NAME,BUNDLE_NAME+file_ext))
        
