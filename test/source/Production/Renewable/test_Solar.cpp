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

testFloatEquals(
    test_solar_ptr->capital_cost,
    3000 * 100,
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

//...

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
