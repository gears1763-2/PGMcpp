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
/// \file test_Noncombustion.cpp
///
/// \brief Testing suite for Noncombustion class.
///
/// A suite of tests for the Noncombustion class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Noncombustion/Noncombustion.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Noncombustion* testConstruct_Noncombustion(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Noncombustion object and spot check some
///     post-construction attributes.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A pointer to a test Noncombustion object.
///

Noncombustion* testConstruct_Noncombustion(std::vector<double>* time_vec_hrs_ptr)
{
    NoncombustionInputs noncombustion_inputs;

    Noncombustion* test_noncombustion_ptr =
        new Noncombustion(
            8760,
            1,
            noncombustion_inputs,
            time_vec_hrs_ptr
        );
    
    testTruth(
        not noncombustion_inputs.production_inputs.print_flag,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_noncombustion_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    return test_noncombustion_ptr;
}   /* testConstruct_Noncombustion() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Noncombustion");
    
    srand(time(NULL));
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    Noncombustion* test_noncombustion_ptr = testConstruct_Noncombustion(&time_vec_hrs);
    
    
    try {
        //...
    }


    catch (...) {
        delete test_noncombustion_ptr;
        
        printGold(" ............. ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_noncombustion_ptr;

    printGold(" ............. ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
