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
/// \file test_Storage.cpp
///
/// \brief Testing suite for Storage class.
///
/// A suite of tests for the Storage class.
///


#include "../../utils/testing_utils.h"
#include "../../../header/Storage/Storage.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Storage");
    
    srand(time(NULL));
    
    
try {

// ======== CONSTRUCTION ============================================================ //

bool error_flag = true;

try {
    StorageInputs bad_storage_inputs;
    bad_storage_inputs.capacity_kWh = 0;
    
    Storage bad_storage(8760, 1, bad_storage_inputs);
    
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

StorageInputs storage_inputs;

Storage test_storage(8760, 1, storage_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testFloatEquals(
    test_storage.capacity_kW,
    100,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_storage.capacity_kWh,
    1000,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_storage.charge_vec_kWh.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_storage.charging_power_vec_kW.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_storage.discharging_power_vec_kW.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_storage.capital_cost_vec.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_storage.operation_maintenance_cost_vec.size(),
    8760,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

//...

// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    //...
    
    printGold(" ...................... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


printGold(" ................................ ");
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
