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
/// \file test_Solar.cpp
///
/// \brief Testing suite for Solar class.
///
/// A suite of tests for the Solar class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Renewable/Solar.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable <-- Solar");
    
    srand(time(NULL));
    
    Renewable* test_solar_ptr;
    
try {

// ======== CONSTRUCTION ============================================================ //

bool error_flag = true;

try {
    SolarInputs bad_solar_inputs;
    bad_solar_inputs.derating = -1;
    
    Solar bad_solar(8760, bad_solar_inputs);
    
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

SolarInputs solar_inputs;

test_solar_ptr = new Solar(8760, solar_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not solar_inputs.renewable_inputs.production_inputs.print_flag,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_solar_ptr->type,
    RenewableType :: SOLAR,
    __FILE__,
    __LINE__
);

testTruth(
    test_solar_ptr->type_str == "SOLAR",
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_solar_ptr->capital_cost,
    350118.723363,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_solar_ptr->operation_maintenance_cost_kWh,
    0.01,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

// test production constraints
testFloatEquals(
    test_solar_ptr->computeProductionkW(0, 1, 2),
    100,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_solar_ptr->computeProductionkW(0, 1, -1),
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
double solar_resource_kWm2 = 0;

for (int i = 0; i < 48; i++) {
    roll = (double)rand() / RAND_MAX;
    
    solar_resource_kWm2 = roll;
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll <= 0.1) {
        solar_resource_kWm2 = 0;
    }
    
    else if (roll >= 0.95) {
        solar_resource_kWm2 = 1.25;
    }
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll >= 0.95) {
        roll = 1.25;
    }
    
    load_vec_kW[i] *= roll * test_solar_ptr->capacity_kW;
    load_kW = load_vec_kW[i];
    
    production_kW = test_solar_ptr->computeProductionkW(
        i,
        dt_vec_hrs[i],
        solar_resource_kWm2
    );
    
    load_kW = test_solar_ptr->commit(
        i,
        dt_vec_hrs[i],
        production_kW,
        load_kW
    );
    
    // is running (or not) as expected
    if (solar_resource_kWm2 > 0) {
        testTruth(
            test_solar_ptr->is_running,
            __FILE__,
            __LINE__
        );
    }
    
    else {
        testTruth(
            not test_solar_ptr->is_running,
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
        test_solar_ptr->production_vec_kW[i] -
        test_solar_ptr->dispatch_vec_kW[i] -
        test_solar_ptr->storage_vec_kW[i] -
        test_solar_ptr->curtailment_vec_kW[i],
        0,
        __FILE__,
        __LINE__
    );
    
    // capacity constraint
    if (solar_resource_kWm2 > 1) {
        testFloatEquals(
            test_solar_ptr->production_vec_kW[i],
            test_solar_ptr->capacity_kW,
            __FILE__,
            __LINE__
        );
    }
    
    // resource, O&M > 0 whenever solar is running (i.e., producing)
    if (test_solar_ptr->is_running) {
        testGreaterThan(
            solar_resource_kWm2,
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_solar_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
    
    // resource, O&M = 0 whenever solar is not running (i.e., not producing)
    else {
        testFloatEquals(
            solar_resource_kWm2,
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_solar_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
}


// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    delete test_solar_ptr;
    
    printGold(" ..... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


delete test_solar_ptr;

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
