/*
 *  PGMcpp : PRIMED Grid Controllerling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file test_Diesel.cpp
///
/// \brief Testing suite for Diesel class.
///
/// A suite of tests for the Diesel class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Combustion/Diesel.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Combustion <-- Diesel");
    
    srand(time(NULL));
    
    
    Combustion* test_diesel_ptr;
    
try {

// ======== CONSTRUCTION ============================================================ //

bool error_flag = true;

try {
    DieselInputs bad_diesel_inputs;
    bad_diesel_inputs.fuel_cost_L = -1;
    
    Diesel bad_diesel(8760, 1, bad_diesel_inputs);
    
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

DieselInputs diesel_inputs;

test_diesel_ptr =  new Diesel(8760, 1, diesel_inputs);


diesel_inputs.combustion_inputs.fuel_mode = FuelMode :: FUEL_MODE_LOOKUP;
diesel_inputs.combustion_inputs.path_2_fuel_interp_data =
    "data/test/interpolation/diesel_fuel_curve.csv";

Diesel test_diesel_lookup(8760, 1, diesel_inputs);


// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not diesel_inputs.combustion_inputs.production_inputs.print_flag,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_diesel_ptr->type,
    CombustionType :: DIESEL,
    __FILE__,
    __LINE__
);

testTruth(
    test_diesel_ptr->type_str == "DIESEL",
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_diesel_ptr->linear_fuel_slope_LkWh,
    0.265675,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_diesel_ptr->linear_fuel_intercept_LkWh,
    0.026676,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_diesel_ptr->capital_cost,
    94125.375446,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_diesel_ptr->operation_maintenance_cost_kWh,
    0.069905,
    __FILE__,
    __LINE__
);

testFloatEquals(
    ((Diesel*)test_diesel_ptr)->minimum_load_ratio,
    0.2,
    __FILE__,
    __LINE__
);

testFloatEquals(
    ((Diesel*)test_diesel_ptr)->minimum_runtime_hrs,
    4,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_diesel_ptr->replace_running_hrs,
    30000,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

//  test capacity constraint
testFloatEquals(
    test_diesel_ptr->requestProductionkW(0, 1, 2 * test_diesel_ptr->capacity_kW),
    test_diesel_ptr->capacity_kW,
    __FILE__,
    __LINE__
);

//  test minimum load ratio constraint
testFloatEquals(
    test_diesel_ptr->requestProductionkW(
        0,
        1, 
        0.5 * ((Diesel*)test_diesel_ptr)->minimum_load_ratio *
            test_diesel_ptr->capacity_kW
    ),
    ((Diesel*)test_diesel_ptr)->minimum_load_ratio * test_diesel_ptr->capacity_kW,
    __FILE__,
    __LINE__
);

//  test commit()
std::vector<double> dt_vec_hrs (48, 1);

std::vector<double> load_vec_kW = {
    1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0
};

std::vector<bool> expected_is_running_vec = {
    1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
    1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0
};

double load_kW = 0;
double production_kW = 0;
double roll = 0;

for (int i = 0; i < 48; i++) {
    roll = (double)rand() / RAND_MAX;
    
    if (roll >= 0.95) {
        roll = 1.25;
    }
    
    load_vec_kW[i] *= roll * test_diesel_ptr->capacity_kW;
    load_kW = load_vec_kW[i];
    
    production_kW = test_diesel_ptr->requestProductionkW(
        i,
        dt_vec_hrs[i],
        load_kW
    );
    
    load_kW = test_diesel_ptr->commit(
        i,
        dt_vec_hrs[i],
        production_kW,
        load_kW
    );
    
    // load_kW <= load_vec_kW (i.e., after vs before)
    testLessThanOrEqualTo(
        load_kW,
        load_vec_kW[i],
        __FILE__,
        __LINE__
    );
    
    // production = dispatch + storage + curtailment
    testFloatEquals(
        test_diesel_ptr->production_vec_kW[i] -
        test_diesel_ptr->dispatch_vec_kW[i] -
        test_diesel_ptr->storage_vec_kW[i] -
        test_diesel_ptr->curtailment_vec_kW[i],
        0,
        __FILE__,
        __LINE__
    );
    
    // capacity constraint
    if (load_vec_kW[i] > test_diesel_ptr->capacity_kW) {
        testFloatEquals(
            test_diesel_ptr->production_vec_kW[i],
            test_diesel_ptr->capacity_kW,
            __FILE__,
            __LINE__
        );
    }
    
    // minimum load ratio constraint
    else if (
        test_diesel_ptr->is_running and
        test_diesel_ptr->production_vec_kW[i] > 0 and
        load_vec_kW[i] <
        ((Diesel*)test_diesel_ptr)->minimum_load_ratio * test_diesel_ptr->capacity_kW
    ) {
        testFloatEquals(
            test_diesel_ptr->production_vec_kW[i],
            ((Diesel*)test_diesel_ptr)->minimum_load_ratio *
                test_diesel_ptr->capacity_kW,
            __FILE__,
            __LINE__
        );
    }
    
    // minimum runtime constraint
    testFloatEquals(
        test_diesel_ptr->is_running_vec[i],
        expected_is_running_vec[i],
        __FILE__,
        __LINE__
    );
    
    // O&M, fuel consumption, and emissions > 0 whenever diesel is running
    if (test_diesel_ptr->is_running) {
        testGreaterThan(
            test_diesel_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_diesel_ptr->fuel_consumption_vec_L[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_diesel_ptr->fuel_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_diesel_ptr->CO2_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_diesel_ptr->CO_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_diesel_ptr->NOx_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_diesel_ptr->SOx_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_diesel_ptr->CH4_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_diesel_ptr->PM_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
    }
    
    // O&M, fuel consumption, and emissions = 0 whenever diesel is not running
    else {
        testFloatEquals(
            test_diesel_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_diesel_ptr->fuel_consumption_vec_L[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_diesel_ptr->fuel_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_diesel_ptr->CO2_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_diesel_ptr->CO_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_diesel_ptr->NOx_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_diesel_ptr->SOx_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_diesel_ptr->CH4_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_diesel_ptr->PM_emissions_vec_kg[i],
            0,
            __FILE__,
            __LINE__
        );
    }
}

std::vector<double> load_ratio_vec = {
    0,
    0.170812859791767,
    0.322739274162545,
    0.369750203682042,
    0.443532869135929,
    0.471567864244626,
    0.536513734479662,
    0.586125806988674,
    0.601101175455075,
    0.658356862575221,
    0.70576929893201,
    0.784069734739331,
    0.805765927542453,
    0.884747873186048,
    0.930870496062112,
    0.979415217694769,
    1
};

std::vector<double> expected_fuel_consumption_vec_L = {
    4.68079520372916,
    8.35159603357656,
    11.7422361561399,
    12.9931187917615,
    14.8786636301325,
    15.5746957307243,
    17.1419229487141,
    18.3041866133728,
    18.6530540913696,
    19.9569217633299,
    21.012354614584,
    22.7142305879957,
    23.1916726441968,
    24.8602332554707,
    25.8172124624032,
    26.8256741279932,
    27.254952
};

for (size_t i = 0; i < load_ratio_vec.size(); i++) {
    testFloatEquals(
        test_diesel_lookup.getFuelConsumptionL(
            1, load_ratio_vec[i] * test_diesel_lookup.capacity_kW
        ),
        expected_fuel_consumption_vec_L[i],
        __FILE__,
        __LINE__
    );
}

// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    delete test_diesel_ptr;
    
    printGold(" ... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


delete test_diesel_ptr;

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
