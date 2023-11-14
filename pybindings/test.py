"""
    PGMcpp : PRIMED Grid Modelling (in C++)
    
    Anthony Truelove MASc, P.Eng.
    email:  gears1763@tutanota.com
    github: gears1763-2
    
    See license terms
    
    A suite of tests for the Python 3 bindings for PGMcpp.
"""


import multiprocessing as mp
import os
import sys

os.system("color")
end = "\n"

import PGMcpp


FLOAT_TOLERANCE = 1e-6


#...


print()
print()
print("## ================ Testing Python Bindings ================ ##")
print()


if __name__ == "__main__":
    try:
        #   1. ElectricalLoad
        print("\tTesting [...] ... ", end="", flush=True)
        
        #...
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)


    except:
        sys.stdout.write("\x1B[31mFAIL\x1b[0m" + end)
        raise


    finally:
        print()
