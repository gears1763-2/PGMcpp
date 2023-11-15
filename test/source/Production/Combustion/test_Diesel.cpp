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
    
    Diesel bad_diesel(8760, bad_diesel_inputs);
    
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

DieselInputs diesel_inputs;

test_diesel_ptr =  new Diesel(8760, diesel_inputs);


// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not diesel_inputs.combustion_inputs.production_inputs.print_flag,
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
    67846.467018,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_diesel_ptr->operation_maintenance_cost_kWh,
    0.038027,
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
std::vector<double> time_vec_hrs (48, 0);
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

double roll = 0;
double production_kW = 0;
std::cout << std::endl;
for (int i = 0; i < 48; i++) {
    roll = (double)rand() / RAND_MAX;
    
    if (roll >= 0.95) {
        roll = 1.25;
    }
    
    time_vec_hrs[i] = i;
    load_vec_kW[i] *= roll * test_diesel_ptr->capacity_kW;
    
    production_kW = test_diesel_ptr->requestProductionkW(
        i,
        dt_vec_hrs[i],
        load_vec_kW[i]
    );
    
    test_diesel_ptr->commit(
        i,
        dt_vec_hrs[i],
        production_kW,
        load_vec_kW[i]
    );
    
    // production = dispatch + storage + curtailment, at every point in time
    testFloatEquals(
        test_diesel_ptr->production_vec_kW[i] -
        test_diesel_ptr->dispatch_vec_kW[i] -
        test_diesel_ptr->storage_vec_kW[i] -
        test_diesel_ptr->curtailment_vec_kW[i],
        0,
        __FILE__,
        __LINE__
    );
    
    // fuel consumption and emissions > 0 whenver diesel is running
    if (test_diesel_ptr->is_running) {
        std::cout << test_diesel_ptr->production_vec_kW[i] << "\t" <<
            test_diesel_ptr->fuel_consumption_vec_L[i] << "\t" <<
            ((Diesel*)test_diesel_ptr)->time_since_last_start_hrs << "\t" <<
            test_diesel_ptr->is_running << "\t" <<
            test_diesel_ptr->is_running_vec[i] << std::endl;
    }
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
