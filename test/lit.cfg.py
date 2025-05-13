# Copyright (c) 2025 Ewan Crawford
import lit.formats
import re
import subprocess
from platform import system

config.name = "clVizulayer Tests"
config.test_format = lit.formats.ShTest("0")
config.suffixes = [".cpp"]
config.excludes = ["common.cpp"]

if system() == "Windows":
    print("WARNING: Windows OS not supported for testing!")
    exit(-1)

config.substitutions.append((re.compile(rf"\bFileCheck\b"), config.filecheck_path))

# Define %build substitution
build_command = config.cxx_compiler + " %s "
build_command += " -I " + config.ocl_headers_path
build_command += " -I " + config.ext_headers_path
build_command += " -DCL_TARGET_OPENCL_VERSION=300 -DCL_USE_DEPRECATED_OPENCL_1_2_APIS"
build_command += " -lOpenCL"
build_command += " -L " + config.ocl_icd_path
config.substitutions.append(("%build", build_command))

# Define %run substitution
icd_lib = os.path.join(config.ocl_icd_path, "libOpenCL.so")
config.environment["LD_PRELOAD"] = icd_lib

viz_lib = os.path.join(config.main_obj_root, "lib")
viz_lib = os.path.join(viz_lib, "libCLVizuLayer.so")
config.environment["OPENCL_LAYERS"] = viz_lib

vendor_icd = os.environ.get("OCL_ICD_VENDORS")
if vendor_icd:
    config.environment["OCL_ICD_VENDORS"] = vendor_icd

# Use clinfo to check for the device capabilities in the first device
# in the first platform, so that tests can be skipped that don't have the
# required optional features.
clinfo_cmd = "{} --raw".format(config.clinfo_path)
clinfo_sp = subprocess.run(clinfo_cmd, text=True, shell=True, capture_output=True)

platform_line = clinfo_sp.stdout.splitlines()[0]
num_platforms = int(platform_line[-1])
if num_platforms == 0:
    print("WARNING: No OpenCL platforms found!")
    exit(-1)
elif num_platforms > 1:
    print("WARNING: More than one platform found, first platform will be used!")

platform_name = ""
for line in clinfo_sp.stdout.splitlines():
    platform = re.search(r"^\[(.+)\/\*\]", line)
    if platform:
        platform_name = platform.group(1)
    else:
        continue

    break

if platform_name == "":
    print("WARNING: No OpenCL platform name found!")
    exit(-1)

# Record the platform name so we can skip tests on a per vendor basis
config.available_features.add(platform_name)

platform_str = r"\[" + platform_name + r"\/\*\]"
device_str = r"\[" + platform_name + r"\/0]"

num_device_str = platform_str + r"\s+\#DEVICES "
for line in clinfo_sp.stdout.splitlines():
    if re.match(num_device_str, line):
        num_devices = int(line[-1])
        if num_devices > 1:
            print(
                "WARNING: More than one device found in platform, first device will be used!"
            )
        break

image_support_str = device_str + r"\s+CL_DEVICE_IMAGE_SUPPORT\s+(.*)$"
for line in clinfo_sp.stdout.splitlines():
    image = re.search(image_support_str, line)
    if image:
        if image.group(1) == "CL_TRUE":
            config.available_features.add("image-support")
        break

device_execution_str = device_str + r"\s+CL_DEVICE_EXECUTION_CAPABILITIES\s+(.*)$"
for line in clinfo_sp.stdout.splitlines():
    device_execution = re.match(device_execution_str, line)
    if device_execution:
        if "CL_EXEC_NATIVE_KERNEL" in device_execution.group(1):
            config.available_features.add("native-kernel-support")
        break

svm_caps_str = device_str + r"\s+CL_DEVICE_SVM_CAPABILITIES\s+(.*)$"
for line in clinfo_sp.stdout.splitlines():
    svm_caps = re.match(svm_caps_str, line)
    if svm_caps:
        if "CL_DEVICE_SVM_COARSE_GRAIN_BUFFER" in svm_caps.group(1):
            config.available_features.add("svm-support")
        break
