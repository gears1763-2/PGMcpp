/*
 *  PGMcpp : PRIMED Grid Modelling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file test_Hydro.cpp
///
/// \brief Testing suite for Hydro class.
///
/// A suite of tests for the Hydro class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Resources.h"
#include "../../../../header/ElectricalLoad.h"
#include "../../../../header/Production/Noncombustion/Hydro.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Noncombustion <-- Hydro");
    
    srand(time(NULL));
    
    
    Noncombustion* test_hydro_ptr;
    
try {

// ======== CONSTRUCTION ============================================================ //

std::string path_2_electrical_load_time_series = 
    "data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";

ElectricalLoad test_electrical_load(path_2_electrical_load_time_series);

Resources test_resources;

HydroInputs hydro_inputs;
int hydro_resource_key = 0;

hydro_inputs.reservoir_capacity_m3 = 10000;
hydro_inputs.resource_key = hydro_resource_key;

test_hydro_ptr =  new Hydro(8760, 1, hydro_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not hydro_inputs.noncombustion_inputs.production_inputs.print_flag,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_hydro_ptr->type,
    NoncombustionType :: HYDRO,
    __FILE__,
    __LINE__
);

testTruth(
    test_hydro_ptr->type_str == "HYDRO",
    __FILE__,
    __LINE__
);

testFloatEquals(
    ((Hydro*)test_hydro_ptr)->turbine_type,
    HydroTurbineType :: HYDRO_TURBINE_PELTON,
    __FILE__,
    __LINE__
);

testFloatEquals(
    ((Hydro*)test_hydro_ptr)->reservoir_capacity_m3,
    10000,
    __FILE__,
    __LINE__
);

std::vector<double> expected_gen_power_ratios = {
    0,   0.1, 0.2,  0.3, 0.4, 0.5,
    0.6, 0.7, 0.75, 0.8, 0.9, 1
};

std::vector<double> expected_gen_efficiencies = {
    0.000, 0.800, 0.900, 0.913,
    0.925, 0.943, 0.947, 0.950,
    0.953, 0.954, 0.956, 0.958
};

double query = 0;
for (size_t i = 0; i < expected_gen_power_ratios.size(); i++) {
    testFloatEquals(
        test_hydro_ptr->interpolator.interp_map_1D[
            HydroInterpKeys :: GENERATOR_EFFICIENCY_INTERP_KEY
        ].x_vec[i],
        expected_gen_power_ratios[i],
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_hydro_ptr->interpolator.interp_map_1D[
            HydroInterpKeys :: GENERATOR_EFFICIENCY_INTERP_KEY
        ].y_vec[i],
        expected_gen_efficiencies[i],
        __FILE__,
        __LINE__
    );
    
    if (i < expected_gen_power_ratios.size() - 1) {
        query = expected_gen_power_ratios[i] + ((double)rand() / RAND_MAX) *
            (expected_gen_power_ratios[i + 1] - expected_gen_power_ratios[i]);
        
        test_hydro_ptr->interpolator.interp1D(
            HydroInterpKeys :: GENERATOR_EFFICIENCY_INTERP_KEY,
            query
        );
    }
}

std::vector<double> expected_turb_power_ratios = {
    0,   0.1, 0.2, 0.3, 0.4,
    0.5, 0.6, 0.7, 0.8, 0.9,
    1
};

std::vector<double> expected_turb_efficiencies = {
    0.000, 0.780, 0.855, 0.875, 0.890,
    0.900, 0.908, 0.913, 0.918, 0.908,
    0.880
};

for (size_t i = 0; i < expected_turb_power_ratios.size(); i++) {
    testFloatEquals(
        test_hydro_ptr->interpolator.interp_map_1D[
            HydroInterpKeys :: TURBINE_EFFICIENCY_INTERP_KEY
        ].x_vec[i],
        expected_turb_power_ratios[i],
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_hydro_ptr->interpolator.interp_map_1D[
            HydroInterpKeys :: TURBINE_EFFICIENCY_INTERP_KEY
        ].y_vec[i],
        expected_turb_efficiencies[i],
        __FILE__,
        __LINE__
    );
    
    if (i < expected_turb_power_ratios.size() - 1) {
        query = expected_turb_power_ratios[i] + ((double)rand() / RAND_MAX) *
            (expected_turb_power_ratios[i + 1] - expected_turb_power_ratios[i]);
        
        test_hydro_ptr->interpolator.interp1D(
            HydroInterpKeys :: TURBINE_EFFICIENCY_INTERP_KEY,
            query
        );
    }
}





// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

std::string path_2_hydro_resource_data =
    "data/test/resources/hydro_inflow_peak-20000m3hr_1yr_dt-1hr.csv";

test_resources.addResource(
    NoncombustionType::HYDRO,
    path_2_hydro_resource_data,
    hydro_resource_key,
    &test_electrical_load
);

double load_kW = 100 * (double)rand() / RAND_MAX;
double production_kW = 0;

for (int i = 0; i < 8760; i++) {
    production_kW = test_hydro_ptr->requestProductionkW(
        i,
        1,
        load_kW,
        test_resources.resource_map_1D[test_hydro_ptr->resource_key][i]
    );
    
    load_kW = test_hydro_ptr->commit(
        i,
        1,
        production_kW,
        load_kW,
        test_resources.resource_map_1D[test_hydro_ptr->resource_key][i]
    );
    
    testGreaterThanOrEqualTo(
        test_hydro_ptr->production_vec_kW[i],
        0,
        __FILE__,
        __LINE__
    );
    
    testLessThanOrEqualTo(
        test_hydro_ptr->production_vec_kW[i],
        test_hydro_ptr->capacity_kW,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_hydro_ptr->production_vec_kW[i] -
        test_hydro_ptr->dispatch_vec_kW[i] -
        test_hydro_ptr->curtailment_vec_kW[i] -
        test_hydro_ptr->storage_vec_kW[i],
        0,
        __FILE__,
        __LINE__
    );
    
    testGreaterThanOrEqualTo(
        ((Hydro*)test_hydro_ptr)->turbine_flow_vec_m3hr[i],
        0,
        __FILE__,
        __LINE__
    );
    
    testLessThanOrEqualTo(
        ((Hydro*)test_hydro_ptr)->turbine_flow_vec_m3hr[i],
        ((Hydro*)test_hydro_ptr)->maximum_flow_m3hr,
        __FILE__,
        __LINE__
    );
    
    testGreaterThanOrEqualTo(
        ((Hydro*)test_hydro_ptr)->stored_volume_vec_m3[i],
        0,
        __FILE__,
        __LINE__
    );
    
    testLessThanOrEqualTo(
        ((Hydro*)test_hydro_ptr)->stored_volume_vec_m3[i],
        ((Hydro*)test_hydro_ptr)->reservoir_capacity_m3,
        __FILE__,
        __LINE__
    );
    
    if (i > 0) {
        /*
        testLessThanOrEqualTo(  //<-- since reservoir has finite capacity
            ((Hydro*)test_hydro_ptr)->stored_volume_vec_m3[i] -
            ((Hydro*)test_hydro_ptr)->stored_volume_vec_m3[i - 1],
            test_resources.resource_map_1D[test_hydro_ptr->resource_key][i] - 
            ((Hydro*)test_hydro_ptr)->turbine_flow_vec_m3hr[i],
            __FILE__,
            __LINE__
        );
        */
        if (((Hydro*)test_hydro_ptr)->stored_volume_vec_m3[i - 1] <= 0) {   //<-- if nothing stored, then only resource available for turbine flow
            testLessThanOrEqualTo(
                ((Hydro*)test_hydro_ptr)->turbine_flow_vec_m3hr[i],
                test_resources.resource_map_1D[test_hydro_ptr->resource_key][i],
                __FILE__,
                __LINE__
            );
        }
    }
}

// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    delete test_hydro_ptr;
    
    printGold(" ... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


delete test_hydro_ptr;

printGold(" ... ");
printGreen("PASS");
std::cout << std::endl;
return 0;

}   /* main() */


/*
bool error_flag = true;

try {
    testTruth(1 == 0, __FILE__, __LINE__);
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}
*/
