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
/// \file test_Production.cpp
///
/// \brief Testing suite for Production class.
///
/// A suite of tests for the Production class.
///


#include "../../utils/testing_utils.h"
#include "../../../header/Production/Production.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Production* testConstruct_Production(void)
///
/// \brief A function to construct a Production object and spot check some
///     post-construction attributes.
///
/// \return A pointer to a test Production object.
///

Production* testConstruct_Production(void)
{
    ProductionInputs production_inputs;

    Production* test_production_ptr = new Production(8760, 1, production_inputs);
    
    testTruth(
        not production_inputs.print_flag,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        production_inputs.nominal_inflation_annual,
        0.02,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        production_inputs.nominal_discount_annual,
        0.04,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->capacity_kW,
        100,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->real_discount_annual,
        0.0196078431372549,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->production_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->dispatch_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->storage_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->curtailment_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->capital_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_production_ptr->operation_maintenance_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );

    return test_production_ptr;
}   /* testConstruct_Production() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadConstruct_Production(void)
///
/// \brief Function to test the trying to construct a Production object given bad 
///     inputs is being handled as expected.
///

void testBadConstruct_Production(void)
{
    bool error_flag = true;

    try {
        ProductionInputs production_inputs;
        
        Production bad_production(0, 1, production_inputs);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadConstruct_Production() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production");
    
    srand(time(NULL));
    
    
    Production* test_production_ptr = testConstruct_Production();
    
        
    try {
        testBadConstruct_Production();
    }


    catch (...) {
        delete test_production_ptr;
        
        printGold(" ............................... ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_production_ptr;

    printGold(" ............................... ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
