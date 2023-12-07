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
/// \fn Renewable* testConstruct_Renewable(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Renewable object and spot check some
///     post-construction attributes.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A pointer to a test Renewable object.
///

Renewable* testConstruct_Renewable(std::vector<double>* time_vec_hrs_ptr)
{
    RenewableInputs renewable_inputs;

    Renewable* test_renewable_ptr = new Renewable(
        8760,
        1,
        renewable_inputs,
        time_vec_hrs_ptr
    );
    
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
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    Renewable* test_renewable_ptr = testConstruct_Renewable(&time_vec_hrs);
    
    
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
