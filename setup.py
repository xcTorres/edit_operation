import os
import sys
import platform
import glob
from sysconfig import get_config_vars
from setuptools import setup, Extension
from pkg_resources import parse_version
from Cython.Build import cythonize


extra_compile_args = []
extra_link_args = []
# Adapted from https://github.com/pandas-dev/pandas/blob/1423ef0f917220682382d478761bf31315a197ef/setup.py#L348
if sys.platform == "darwin":
    if "MACOSX_DEPLOYMENT_TARGET" not in os.environ:
        current_system = platform.mac_ver()[0]
        python_target = get_config_vars().get(
            "MACOSX_DEPLOYMENT_TARGET", current_system
        )
        target_macos_version = "10.9"
        parsed_macos_version = parse_version(target_macos_version)
        if (
            parse_version(str(python_target))
            < parsed_macos_version
            <= parse_version(current_system)
        ):
            os.environ["MACOSX_DEPLOYMENT_TARGET"] = target_macos_version

# Define args by platform.
if platform == "linux" or platform == "linux2":
    # linux
    extra_compile_args = ["-std=c++11"]  # compile with C++11;
    extra_link_args = ["-std=c++11"]
elif platform == "darwin":
    # OS X
    extra_compile_args = ["-std=c++11", '-stdlib=libc++', '-Wno-sign-compare']  # compile with C++17
    extra_link_args = ["-std=c++11", '-stdlib=libc++', ]
else:  # default
    extra_compile_args = ["-std=c++11"]  # compile with C++11;
    extra_link_args = ["-std=c++11"]


included_folders = ["src/edit_operation"]
c_library_extensions = ['cc', 'cpp', 'c', 'cxx']
excluded_cpp_files = ["levenshtein.cpp"]

# We search for all files that within included folders, to get all cpp files.
# Also, if we have identified a source file, we need to include its parent directory as .
source_file_list = []
header_directory = []
for folder_name in included_folders:
    temp_filename_list = glob.glob("./{}/**/*".format(folder_name), recursive=True)
    for filename in temp_filename_list:
        if filename.split('.')[-1] in ['h']:
            header_directory.append(os.path.dirname(filename))
        if (filename.split('.')[-1] in c_library_extensions) and (
                filename.split("/")[-1] not in excluded_cpp_files):
            source_file_list.append(filename)

header_directory = list(set(header_directory))  # Remove duplicate values.
source_file_list += ["src/edit_operation/levenshtein.pyx"]  # Add the original pyx file for coord_list.


ext = Extension(name='edit_operation.levenshtein', 
                sources=source_file_list,
                extra_compile_args=extra_compile_args,
                extra_link_args=extra_link_args,
                include_dirs=header_directory,
                language='c++')

setup(name="edit_operation",
      ext_modules=cythonize(ext),)
