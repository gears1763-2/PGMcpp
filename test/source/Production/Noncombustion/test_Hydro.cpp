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
/// \file test_Hydro.cpp
///
/// \brief Testing suite for Hydro class.
///
/// A suite of tests for the Hydro class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Noncombustion/Hydro.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Noncombustion <-- Hydro");
    
    srand(time(NULL));
    
    
    Noncombustion* test_hydro_ptr;
    
try {

// ======== CONSTRUCTION ============================================================ //

HydroInputs hydro_inputs;

test_hydro_ptr =  new Hydro(8760, 1, hydro_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not hydro_inputs.noncombustion_inputs.production_inputs.print_flag,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_hydro_ptr->type,
    NoncombustionType :: HYDRO,
    __FILE__,
    __LINE__
);

testTruth(
    test_hydro_ptr->type_str == "HYDRO",
    __FILE__,
    __LINE__
);


// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

//...

// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    delete test_hydro_ptr;
    
    printGold(" ... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


delete test_hydro_ptr;

printGold(" ... ");
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
