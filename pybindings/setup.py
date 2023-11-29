"""
    PGMcpp : PRIMED Grid Modelling (in C++) - Long Term Support (LTS)
    
    Anthony Truelove MASc, P.Eng.
    email:  gears1763@tutanota.com
    github: gears1763-2
    
    See license terms
    
    A setup file for generating Python3 bindings for PGMcpp. To invoke, simply
    
    $   python(3) setup.py build_ext --inplace
    
    depending on your interpreter.
    
    ref: https://pybind11.readthedocs.io/en/stable/compiling.html \n
    ref: https://github.com/pybind/python_example/ \n
    ref: https://github.com/pybind/pybind11/ \n
"""


import os
import shutil
import sys

from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension


# remove any existing extensions
for root, dirs, files in os.walk(os.getcwd()):
    for name in files:
        if "PGMcpp" in name:
            os.remove(name)


# generate list of source files
source_list = ["PYBIND11_PGM.cpp"]
for root, dirs, files in os.walk(".." + os.sep + "source"):
    for name in files:
        path_str = os.path.join(root, name)
        source_list.append(path_str)

for source in source_list:
    print(source)


# generate list of pybind11 extensions
ext_modules = [
    Pybind11Extension(
        "PGMcpp",
        sources=source_list,
        language="c++",
        cxx_std=17
    )
]


# set up extensions
setup(
    name="PGMcpp",
    ext_modules=ext_modules,
    zip_safe=False,
    python_requires=">=3.7",
    install_requires=["setuptools>=42", "pybind11>=2.6.1"]
)


# clean up
for root, dirs, files in os.walk(os.getcwd()):
    for name in dirs:
        if "build" in name:
            shutil.rmtree(name)
