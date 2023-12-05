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


// ---------------------------------------------------------------------------------- //

///
/// \fn Renewable* testConstruct_Renewable(void)
///
/// \brief A function to construct a Renewable object and spot check some
///     post-construction attributes.
///
/// \return A pointer to a test Renewable object.
///

Renewable* testConstruct_Renewable(void)
{
    RenewableInputs renewable_inputs;

    Renewable* test_renewable_ptr = new Renewable(8760, 1, renewable_inputs);
    
    testTruth(
        not renewable_inputs.production_inputs.print_flag,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_renewable_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    return test_renewable_ptr;
}   /* testConstruct_Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable");
    
    srand(time(NULL));
    
    
    Renewable* test_renewable_ptr = testConstruct_Renewable();
    
    
    try {
        //...
    }


    catch (...) {
        delete test_renewable_ptr;
        
        printGold(" ................. ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_renewable_ptr;

    printGold(" ................. ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
