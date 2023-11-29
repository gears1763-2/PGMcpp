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
/// \file test_LiIon.cpp
///
/// \brief Testing suite for LiIon class.
///
/// A suite of tests for the LiIon class.
///


#include "../../utils/testing_utils.h"
#include "../../../header/Storage/LiIon.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Storage <-- LiIon");
    
    srand(time(NULL));
    
    
try {

// ======== CONSTRUCTION ============================================================ //

bool error_flag = true;

try {
    LiIonInputs bad_liion_inputs;
    bad_liion_inputs.min_SOC = -1;
    
    LiIon bad_liion(8760, 1, bad_liion_inputs);
    
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

LiIonInputs liion_inputs;

LiIon test_liion(8760, 1, liion_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    test_liion.type_str == "LIION",
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.init_SOC,
    0.5,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.min_SOC,
    0.15,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.hysteresis_SOC,
    0.5,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.max_SOC,
    0.9,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.charging_efficiency,
    0.9,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.discharging_efficiency,
    0.9,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.replace_SOH,
    0.8,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.power_kW,
    0,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.SOH_vec.size(),
    8760,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

testFloatEquals(
    test_liion.getAvailablekW(1),
    100,    // hits power capacity constraint
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.getAcceptablekW(1),
    100,    // hits power capacity constraint
    __FILE__,
    __LINE__
);

test_liion.power_kW = 100;

testFloatEquals(
    test_liion.getAvailablekW(1),
    100,    // hits power capacity constraint
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.getAcceptablekW(1),
    100,    // hits power capacity constraint
    __FILE__,
    __LINE__
);

test_liion.power_kW = 1e6;

testFloatEquals(
    test_liion.getAvailablekW(1),
    0,    // is already hitting power capacity constraint
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_liion.getAcceptablekW(1),
    0,    // is already hitting power capacity constraint
    __FILE__,
    __LINE__
);

test_liion.commitCharge(0, 1, 100);

testFloatEquals(
    test_liion.power_kW,
    0,
    __FILE__,
    __LINE__
);

// ======== END METHODS ============================================================= //

} /* try */


catch (...) {
    //...
    
    printGold(" ...................... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


printGold(" ...................... ");
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
