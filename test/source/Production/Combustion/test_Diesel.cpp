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
    
    printGold("\tTesting Production <-- Combustion <-- Diesel ... ");
    
    srand(time(NULL));
    
    
    try {
        //...
    }
    
    catch (...) {
        //...
        
        printRed("\tFAIL");
        std::cout << std::endl;
        throw;
    }
    
    
    printGreen("\tPASS");
    std::cout << std::endl;
    return 0;
}   /* main() */


/*
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
