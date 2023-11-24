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
/// \file test_Wind.cpp
///
/// \brief Testing suite for Wind class.
///
/// A suite of tests for the Wind class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Renewable/Wind.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable <-- Wind");
    
    srand(time(NULL));
    
    Renewable* test_wind_ptr;
    
try {

// ======== CONSTRUCTION ============================================================ //

bool error_flag = true;

try {
    WindInputs bad_wind_inputs;
    bad_wind_inputs.design_speed_ms = -1;
    
    Wind bad_wind(8760, 1, bad_wind_inputs);
    
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

WindInputs wind_inputs;

test_wind_ptr = new Wind(8760, 1, wind_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not wind_inputs.renewable_inputs.production_inputs.print_flag,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_wind_ptr->type,
    RenewableType :: WIND,
    __FILE__,
    __LINE__
);

testTruth(
    test_wind_ptr->type_str == "WIND",
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_wind_ptr->capital_cost,
    450356.170088,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_wind_ptr->operation_maintenance_cost_kWh,
    0.034953,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

// test production constraints
testFloatEquals(
    test_wind_ptr->computeProductionkW(0, 1, 1e6),
    0,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_wind_ptr->computeProductionkW(
        0,
        1,
        ((Wind*)test_wind_ptr)->design_speed_ms
    ),
    test_wind_ptr->capacity_kW,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_wind_ptr->computeProductionkW(0, 1, -1),
    0,
    __FILE__,
    __LINE__
);

// test commit()
std::vector<double> dt_vec_hrs (48, 1);

std::vector<double> load_vec_kW = {
    1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0
};

double load_kW = 0;
double production_kW = 0;
double roll = 0;
double wind_resource_ms = 0;

for (int i = 0; i < 48; i++) {
    roll = (double)rand() / RAND_MAX;
    
    wind_resource_ms = roll * ((Wind*)test_wind_ptr)->design_speed_ms;
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll <= 0.1) {
        wind_resource_ms = 0;
    }
    
    else if (roll >= 0.95) {
        wind_resource_ms = 3 * ((Wind*)test_wind_ptr)->design_speed_ms;
    }
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll >= 0.95) {
        roll = 1.25;
    }
    
    load_vec_kW[i] *= roll * test_wind_ptr->capacity_kW;
    load_kW = load_vec_kW[i];
    
    production_kW = test_wind_ptr->computeProductionkW(
        i,
        dt_vec_hrs[i],
        wind_resource_ms
    );
    
    load_kW = test_wind_ptr->commit(
        i,
        dt_vec_hrs[i],
        production_kW,
        load_kW
    );
    
    // is running (or not) as expected
    if (production_kW > 0) {
        testTruth(
            test_wind_ptr->is_running,
            __FILE__,
            __LINE__
        );
    }
    
    else {
        testTruth(
            not test_wind_ptr->is_running,
            __FILE__,
            __LINE__
        );
    }
    
    // load_kW <= load_vec_kW (i.e., after vs before)
    testLessThanOrEqualTo(
        load_kW,
        load_vec_kW[i],
        __FILE__,
        __LINE__
    );
    
    // production = dispatch + storage + curtailment
    testFloatEquals(
        test_wind_ptr->production_vec_kW[i] -
        test_wind_ptr->dispatch_vec_kW[i] -
        test_wind_ptr->storage_vec_kW[i] -
        test_wind_ptr->curtailment_vec_kW[i],
        0,
        __FILE__,
        __LINE__
    );
    
    // resource, O&M > 0 whenever wind is running (i.e., producing)
    if (test_wind_ptr->is_running) {
        testGreaterThan(
            wind_resource_ms,
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_wind_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
    
    // O&M = 0 whenever wind is not running (i.e., not producing)
    else {
        testFloatEquals(
            test_wind_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
}


// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    delete test_wind_ptr;
    
    printGold(" ...... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


delete test_wind_ptr;

printGold(" ...... ");
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
