"""
    PGMcpp : PRIMED Grid Modelling (in C++)
    
    Anthony Truelove MASc, P.Eng.
    email:  gears1763@tutanota.com
    github: gears1763-2
    
    See license terms
    
    A suite of tests for the Python 3 bindings for PGMcpp.
    
    Just some simple spot checks. The tests in test/ are much more thorough.
"""


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
        ## ================ test Interpolator ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Interpolator " +
            29 * "." +
            " " +
            "\033[0m"
        )
        
        test_interpolator = PGMcpp.Interpolator();
        
        data_key_1D = 1
        path_2_data_1D = "../data/test/interpolation/diesel_fuel_curve.csv"
        
        data_key_2D = 2
        path_2_data_2D = (
            "../data/test/interpolation/" +
            "wave_energy_converter_normalized_performance_matrix.csv"
        )
        
        test_interpolator.addData1D(1, path_2_data_1D)
        test_interpolator.addData2D(2, path_2_data_2D)
        
        assert (test_interpolator.path_map_1D[data_key_1D] == path_2_data_1D)
        assert (test_interpolator.path_map_2D[data_key_2D] == path_2_data_2D)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Production ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production " +
            31 * "." +
            " " +
            "\033[0m"
        )
        
        test_production_inputs = PGMcpp.ProductionInputs()
        
        test_production = PGMcpp.Production(8760, 1, test_production_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Combustion ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Combustion " +
            16 * "." +
            " " +
            "\033[0m"
        )
        
        test_combustion_inputs = PGMcpp.CombustionInputs()
        
        test_combustion = PGMcpp.Combustion(8760, 1, test_combustion_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Diesel ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Combustion <-- Diesel " +
            5 * "." +
            " " +
            "\033[0m"
        )
        
        test_diesel_inputs = PGMcpp.DieselInputs()
        
        test_diesel = PGMcpp.Diesel(8760, 1, test_diesel_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Noncombustion ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Noncombustion " +
            13 * "." +
            " " +
            "\033[0m"
        )
        
        test_noncombustion_inputs = PGMcpp.NoncombustionInputs()
        
        test_noncombustion = PGMcpp.Noncombustion(8760, 1, test_noncombustion_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Hydro ================ ##
        
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Noncombustion <-- Hydro " +
            3 * "." +
            " " +
            "\033[0m"
        )
        
        test_hydro_inputs = PGMcpp.NoncombustionInputs()
        
        test_hydro = PGMcpp.Noncombustion(8760, 1, test_hydro_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        
        ## ================ test Renewable ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable " +
            17 * "." +
            " " +
            "\033[0m"
        )
        
        test_renewable_inputs = PGMcpp.RenewableInputs()
        
        test_renewable = PGMcpp.Renewable(8760, 1, test_renewable_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Solar ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable <-- Solar " +
            7 * "." +
            " " +
            "\033[0m"
        )
        
        test_solar_inputs = PGMcpp.SolarInputs()
        
        test_solar = PGMcpp.Solar(8760, 1, test_solar_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Tidal ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable <-- Tidal " +
            7 * "." +
            " " +
            "\033[0m"
        )
        
        test_tidal_inputs = PGMcpp.TidalInputs()
        
        test_tidal = PGMcpp.Tidal(8760, 1, test_tidal_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Wave ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable <-- Wave " +
            8 * "." +
            " " +
            "\033[0m"
        )
        
        test_wave_inputs = PGMcpp.WaveInputs()
        
        test_wave = PGMcpp.Wave(8760, 1, test_wave_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Wind ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Production <-- Renewable <-- Wind " +
            8 * "." +
            " " +
            "\033[0m"
        )
        
        test_wind_inputs = PGMcpp.WindInputs()
        
        test_wind = PGMcpp.Wind(8760, 1, test_wind_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Storage ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Storage " +
            34 * "." +
            " " +
            "\033[0m"
        )
        
        test_storage_inputs = PGMcpp.StorageInputs()
        
        test_storage = PGMcpp.Storage(8760, 1, test_storage_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test LiIon ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Storage <-- LiIon " +
            24 * "." +
            " " +
            "\033[0m"
        )
        
        test_liion_inputs = PGMcpp.LiIonInputs()
        
        test_liion = PGMcpp.LiIon(8760, 1, test_liion_inputs)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Controller ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Controller " +
            31 * "." +
            " " +
            "\033[0m"
        )
        
        test_controller = PGMcpp.Controller()
        
        test_controller.setControlMode(PGMcpp.ControlMode.LOAD_FOLLOWING);
        
        assert (test_controller.control_mode == PGMcpp.ControlMode.LOAD_FOLLOWING)
        assert (test_controller.control_string == "LOAD_FOLLOWING")
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test ElectricalLoad ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing ElectricalLoad " +
            27 * "." +
            " " +
            "\033[0m"
        )
        
        path_2_electrical_load_time_series = (
            "../data/test/electrical_load/" +
            "electrical_load_generic_peak-500kW_1yr_dt-1hr.csv"
        )
        
        test_electrical_load = PGMcpp.ElectricalLoad(path_2_electrical_load_time_series)
        
        assert (
            test_electrical_load.path_2_electrical_load_time_series ==
            path_2_electrical_load_time_series
        )
        
        assert (
            abs(test_electrical_load.min_load_kW - 82.1211213927802) <=
            FLOAT_TOLERANCE
        )
        
        assert (
            abs(test_electrical_load.mean_load_kW - 258.373472633202) <=
            FLOAT_TOLERANCE
        )
        
        assert (abs(test_electrical_load.max_load_kW - 500) <= FLOAT_TOLERANCE)
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Resources ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Resources " +
            32 * "." +
            " " +
            "\033[0m"
        )
        
        test_resources = PGMcpp.Resources();
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)
        
        
        ## ================ test Model ================ ##
        sys.stdout.write(
            "\x1B[33m" +
            "  Testing Model " +
            36 * "." +
            " " +
            "\033[0m"
        )
        
        test_model_inputs = PGMcpp.ModelInputs();
        
        path_2_electrical_load_time_series = (
            "../data/test/electrical_load/" +
            "electrical_load_generic_peak-500kW_1yr_dt-1hr.csv"
        )
        
        test_model_inputs.path_2_electrical_load_time_series = (
            path_2_electrical_load_time_series
        )
        
        test_model = PGMcpp.Model(test_model_inputs)
        
        assert (
            test_model.controller.control_mode == PGMcpp.ControlMode.LOAD_FOLLOWING
        )
        
        assert (test_model.controller.control_string == "LOAD_FOLLOWING")
        
        for i in range(0, 3):
            test_model.addDiesel(test_diesel_inputs)
        
        test_model.run()
        
        sys.stdout.write("\x1b[1;32mPASS\x1b[0m" + end)


    except:
        sys.stdout.write("\x1B[31mFAIL\x1b[0m" + end)
        raise


    finally:
        print()
