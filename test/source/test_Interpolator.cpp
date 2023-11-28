/*
 *  PGMcpp : PRIMED Grid Interpolatorling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file test_Interpolator.cpp
///
/// \brief Testing suite for Interpolator class.
///
/// A suite of tests for the Interpolator class.
///


#include "../utils/testing_utils.h"
#include "../../header/Interpolator.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\n\tTesting Interpolator");
    
    srand(time(NULL));
    
    
try {

// ======== CONSTRUCTION ============================================================ //

//...

// ======== END CONSTRUCTION =========================================================//



// ======== ATTRIBUTES ============================================================== //

//...

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

//...

// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    //...
    
    printGold(" ........................... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


printGold(" ........................... ");
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
