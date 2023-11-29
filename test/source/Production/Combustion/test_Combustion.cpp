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
/// \file test_Combustion.cpp
///
/// \brief Testing suite for Combustion class.
///
/// A suite of tests for the Combustion class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Combustion/Combustion.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Combustion");
    
    srand(time(NULL));
    
    
try {

// ======== CONSTRUCTION ============================================================ //

CombustionInputs combustion_inputs;

Combustion test_combustion(8760, 1, combustion_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not combustion_inputs.production_inputs.print_flag,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_combustion.fuel_consumption_vec_L.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_combustion.fuel_cost_vec.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_combustion.CO2_emissions_vec_kg.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_combustion.CO_emissions_vec_kg.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_combustion.NOx_emissions_vec_kg.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_combustion.SOx_emissions_vec_kg.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_combustion.CH4_emissions_vec_kg.size(),
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_combustion.PM_emissions_vec_kg.size(),
    8760,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //

}   /* try */


catch (...) {
    //...
    
    printGold(" .............. ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


printGold(" .............. ");
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
