# PGMcpp : PRIMED Grid Modelling (in C++)

    Anthony Truelove MASc, P.Eng.
    email:   gears1763@tutanota.com
    github:  gears1763-2

***See license terms***


This is a microgrid modelling code, which can be used to assess the economic and
environmental impacts of integrating renewable energy production and storage assets 
into an otherwise isolated microgrid (presumably reliant on diesel, or other
fuel-based, generation to begin with).

The Pacific Regional Institute for Marine Energy Discovery (PRIMED): <https://onlineacademiccommunity.uvic.ca/primed/><br>
The Institute for Integrated Energy Systems (IESVic): <https://www.uvic.ca/research/centres/iesvic/index.php>

--------


## Contents

In the directory for this project, you should find this README, a license file, a
makefile, and the following sub-directories:

    data/           to hold sample input data for testing
    docs/           to hold various documentation
    header/         to hold header files
    projects/       to hold PGMcpp projects, including an example project
    pybindings/     to hold source and extension files for making Python 3 bindings
    source/         to hold source files
    test/           to hold the source files for a suite of tests
    third_party/    to hold third party content used in the development of PGMcpp

--------


## Key Features

  * stuff ...
  * ... and things

--------


## Setup

pybind11 (<https://pybind11.readthedocs.io/en/stable/>), setup.py, etc. ... the idea is
to make and use Python 3 bindings for PGMcpp

[...]

    python(3) setup.py build_ext --inplace

--------


## Documentation

Documentation for this project is auto-generated using Doxygen
(<https://www.doxygen.nl/>). HTML documentation can be found in docs/html, and a 
pre-compiled LaTeX pdf can be bound at docs/PGMcpp_manual.pdf. Additionally, docs/refs
contains a few shareable references that form some of the theoretical/empirical bases
for the implementation of PGMcpp.

--------


## Testing

Tests are automatically run at compile time; one need only "make all"

[...]

The makefile and all source code was successfully tested on the following OS and
architectures:

    Operating System: Linux Mint 21.2
              Kernel: Linux 6.5.4-76060504-generic
        Architecture: x86-64

    Operating System: Windows 11 Home
             Version: 10.0.22621 Build 22621
        Architecture: 64-bit OS, x64-based processor


The following compilers were used in testing:

    g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
    g++ (Rev10, Built by MSYS2 project) 13.2.0


PGMcpp has the following dependencies (by compiler link):

    -lpthread

--------
