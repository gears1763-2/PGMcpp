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
/// \file test_Tidal.cpp
///
/// \brief Testing suite for Tidal class.
///
/// A suite of tests for the Tidal class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Renewable/Tidal.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable <-- Tidal");
    
    srand(time(NULL));
    
    Renewable* test_tidal_ptr;
    
try {

// ======== CONSTRUCTION ============================================================ //

bool error_flag = true;

try {
    TidalInputs bad_tidal_inputs;
    bad_tidal_inputs.design_speed_ms = -1;
    
    Tidal bad_tidal(8760, 1, bad_tidal_inputs);
    
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

TidalInputs tidal_inputs;

test_tidal_ptr = new Tidal(8760, 1, tidal_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not tidal_inputs.renewable_inputs.production_inputs.print_flag,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_tidal_ptr->type,
    RenewableType :: TIDAL,
    __FILE__,
    __LINE__
);

testTruth(
    test_tidal_ptr->type_str == "TIDAL",
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_tidal_ptr->capital_cost,
    500237.446725,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_tidal_ptr->operation_maintenance_cost_kWh,
    0.069905,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

// test production constraints
testFloatEquals(
    test_tidal_ptr->computeProductionkW(0, 1, 1e6),
    0,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_tidal_ptr->computeProductionkW(
        0,
        1,
        ((Tidal*)test_tidal_ptr)->design_speed_ms
    ),
    test_tidal_ptr->capacity_kW,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_tidal_ptr->computeProductionkW(0, 1, -1),
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
double tidal_resource_ms = 0;

for (int i = 0; i < 48; i++) {
    roll = (double)rand() / RAND_MAX;
    
    tidal_resource_ms = roll * ((Tidal*)test_tidal_ptr)->design_speed_ms;
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll <= 0.1) {
        tidal_resource_ms = 0;
    }
    
    else if (roll >= 0.95) {
        tidal_resource_ms = 3 * ((Tidal*)test_tidal_ptr)->design_speed_ms;
    }
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll >= 0.95) {
        roll = 1.25;
    }
    
    load_vec_kW[i] *= roll * test_tidal_ptr->capacity_kW;
    load_kW = load_vec_kW[i];
    
    production_kW = test_tidal_ptr->computeProductionkW(
        i,
        dt_vec_hrs[i],
        tidal_resource_ms
    );
    
    load_kW = test_tidal_ptr->commit(
        i,
        dt_vec_hrs[i],
        production_kW,
        load_kW
    );
    
    // is running (or not) as expected
    if (production_kW > 0) {
        testTruth(
            test_tidal_ptr->is_running,
            __FILE__,
            __LINE__
        );
    }
    
    else {
        testTruth(
            not test_tidal_ptr->is_running,
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
        test_tidal_ptr->production_vec_kW[i] -
        test_tidal_ptr->dispatch_vec_kW[i] -
        test_tidal_ptr->storage_vec_kW[i] -
        test_tidal_ptr->curtailment_vec_kW[i],
        0,
        __FILE__,
        __LINE__
    );
    
    // resource, O&M > 0 whenever tidal is running (i.e., producing)
    if (test_tidal_ptr->is_running) {
        testGreaterThan(
            tidal_resource_ms,
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_tidal_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
    
    // O&M = 0 whenever tidal is not running (i.e., not producing)
    else {
        testFloatEquals(
            test_tidal_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
}


// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    delete test_tidal_ptr;
    
    printGold(" ..... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


delete test_tidal_ptr;

printGold(" ..... ");
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
