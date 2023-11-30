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
/// \file test_Renewable.cpp
///
/// \brief Testing suite for Renewable class.
///
/// A suite of tests for the Renewable class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Renewable/Renewable.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable");
    
    srand(time(NULL));
    
    
try {

// ======== CONSTRUCTION ============================================================ //

RenewableInputs renewable_inputs;

Renewable test_renewable(8760, 1, renewable_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not renewable_inputs.production_inputs.print_flag,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //

}   /* try */


catch (...) {
    //...
    
    printGold(" ................. ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


printGold(" ................. ");
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
