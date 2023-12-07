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


// ---------------------------------------------------------------------------------- //

///
/// \fn Combustion* testConstruct_Combustion(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Combustion object and spot check some
///     post-construction attributes.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A pointer to a test Combustion object.
///

Combustion* testConstruct_Combustion(std::vector<double>* time_vec_hrs_ptr)
{
    CombustionInputs combustion_inputs;

    Combustion* test_combustion_ptr = new Combustion(
        8760,
        1,
        combustion_inputs,
        time_vec_hrs_ptr
    );
    
    testTruth(
        not combustion_inputs.production_inputs.print_flag,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->fuel_consumption_vec_L.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->fuel_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->CO2_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->CO_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->NOx_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->SOx_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->CH4_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_combustion_ptr->PM_emissions_vec_kg.size(),
        8760,
        __FILE__,
        __LINE__
    );

    return test_combustion_ptr;
}   /* testConstruct_Combustion() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Combustion");
    
    srand(time(NULL));
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    Combustion* test_combustion_ptr = testConstruct_Combustion(&time_vec_hrs);
    
    
    try {
        //...
    }


    catch (...) {
        delete test_combustion_ptr;
        
        printGold(" ................ ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_combustion_ptr;

    printGold(" ................ ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
