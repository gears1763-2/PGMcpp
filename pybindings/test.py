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


print()
print()
print("## ================ Testing Python Bindings ================ ##")
print()


if __name__ == "__main__":
    try:
        ## ================ test Controller ================ ##
        print("  Testing Controller ... ", end="", flush=True)
        
        test_controller = PGMcpp.Controller()
        
        test_controller.setControlMode(PGMcpp.ControlMode.LOAD_FOLLOWING);
        
        assert (test_controller.control_mode == PGMcpp.ControlMode.LOAD_FOLLOWING)
        assert (test_controller.control_string == "LOAD_FOLLOWING")
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test ElectricalLoad ================ ##
        print("  Testing ElectricalLoad ... ", end="", flush=True)
        
        path_2_electrical_load_time_series = (
            "../data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv"
        )
        
        test_electrical_load = PGMcpp.ElectricalLoad(path_2_electrical_load_time_series)
        
        assert (
            test_electrical_load.path_2_electrical_load_time_series ==
            path_2_electrical_load_time_series
        )
        
        assert (
            abs(test_electrical_load.min_load_kW - 82.1211213927802) <= FLOAT_TOLERANCE
        )
        
        assert (
            abs(test_electrical_load.mean_load_kW - 258.373472633202) <= FLOAT_TOLERANCE
        )
        
        assert (abs(test_electrical_load.max_load_kW - 500) <= FLOAT_TOLERANCE)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Interpolator ================ ##
        print("  Testing Interpolator ... ", end="", flush=True)
        
        test_interpolator = PGMcpp.Interpolator();
        
        data_key_1D = 1
        path_2_data_1D = "../data/test/interpolation/diesel_fuel_curve.csv"
        
        data_key_2D = 2
        path_2_data_2D = (
            "../data/test/interpolation/wave_energy_converter_normalized_performance_matrix.csv"
        )
        
        test_interpolator.addData1D(1, path_2_data_1D)
        test_interpolator.addData2D(2, path_2_data_2D)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)


    except:
        sys.stdout.write("\x1B[31mFAIL\x1b[0m" + end)
        raise


    finally:
        print()
