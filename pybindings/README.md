# PGMcpp : PRIMED Grid Modelling (in C++)

    Anthony Truelove MASc, P.Eng.
    email:   gears1763@tutanota.com
    github:  gears1763-2

***See license terms***


Python 3 bindings README for PGMcpp.

You can try to make use of the pre-compiled bindings first (provided in 
`precompiled_bindings/`). If one of those works for you, then you need not worry about 
building the bindings. However, if none of the pre-compiled bindings work for you (or 
you make changes to the code and need to build new bindings), then this README is for
you.

--------


## Contents

In this `pybindings/` sub-directory, you should find this README, a pip requirements file,
a setup file, a test file, and various bindings source files organized as follows:

    PYBIND11_PGM.cpp        ties all of the bindings snippets together
    snippets/               to hold the various bindings snippets (has the same file structure as `source/`)
    precompiled_bindings/   to hold some pre-compiled bindings (which may or may not work for your OS/architecture)

Note that each `pybindings/snippets/../PYBIND11_<>.cpp` contains the bindings
definitions corresponding to `source/../<>.cpp`.

--------


## Setup

Note that the provided `setup.py` has the following dependencies (listed in 
`pip_requirements`):

    pybind11==2.11.1
    setuptools==59.6.0

so you may need to `pip install` these before building the bindings. Additionally, you
need to make sure that you are set up with a compatible C++ compiler.

However, if you are all set up, then building the bindings is as simple as

    python(3) setup.py build_ext --inplace

depending on your setup (either `python` or `python3`).

### Linux (Debian/Ubuntu) Notes

You should already be set up with a Python 3 interpreter, so just doing the `pip install`
steps should be sufficient. Additionally, you may also be set up with a compatible
C++ compiler already. However, if this is not the case, then you can simply

    sudo apt-get install build-essential

The provided `setup.py` was successfully invoked on Linux Mint 21.2 using `g++ 11.4.0`.

### Windows Notes

On Windows, there are a few things to set up first before building the Python 3 bindings;
namely

  * You need to be using Python for Windows, as available from <https://www.python.org/downloads/>. Version 3.10+ should do fine.
  * You need to `pip install` the dependencies listed above (and in `pip_requirements`).
  * You need to be using the MSVC compiler (`cl.exe`). This can be obtained by way of installing Visual Studio (as available from <https://visualstudio.microsoft.com/downloads/>; the free community edition is fine) and then selecting the "Desktop development with C++" workload.

The provided `setup.py` was successfully invoked on Windows 11 Home (22H2) using the Microsoft C/C++ Optimizing Compiler Version 19.37.32825.

--------


## Testing

Once the bindings are built, you can run the provided tests by invoking

    python(3) test.py

depending on your setup (either `python` or `python3`). If you see `PASS` across the 
board, then the bindings were successfully built.

--------
