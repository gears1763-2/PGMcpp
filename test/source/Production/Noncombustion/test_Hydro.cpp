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

hydro_inputs.reservoir_capacity_m3 = 1000;
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
    ((Hydro*)test_hydro_ptr)->reservoir_capacity_m3,
    1000,
    __FILE__,
    __LINE__
);


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
        testLessThanOrEqualTo(  //<-- since reservoir has finite capacity
            ((Hydro*)test_hydro_ptr)->stored_volume_vec_m3[i] -
            ((Hydro*)test_hydro_ptr)->stored_volume_vec_m3[i - 1],
            test_resources.resource_map_1D[test_hydro_ptr->resource_key][i] - 
            ((Hydro*)test_hydro_ptr)->turbine_flow_vec_m3hr[i],
            __FILE__,
            __LINE__
        );
        
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
