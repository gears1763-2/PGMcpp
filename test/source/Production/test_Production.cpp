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
/// \file test_Production.cpp
///
/// \brief Testing suite for Production class.
///
/// A suite of tests for the Production class.
///


#include "../../utils/testing_utils.h"
#include "../../../header/Production/Production.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\n\tTesting Production");
    
    srand(time(NULL));
    
    
    try {
        //  1. construction
        bool error_flag = true;
        try {
            ProductionInputs production_inputs;
            
            Production bad_production(0, production_inputs);
            
            error_flag = false;
        } catch (...) {
            // Task failed successfully! =P
        }
        if (not error_flag) {
            expectedErrorNotDetected(__FILE__, __LINE__);
        }
        
        
        ProductionInputs production_inputs;
        
        Production test_production(8760, production_inputs);
        
        
        //  2. test structure attributes
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
        
        
        //  3. test post-construction attributes
        testFloatEquals(
            test_production.n_points,
            8760,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_production.capacity_kW,
            100,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_production.real_discount_annual,
            0.0196078431372549,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_production.production_vec_kW.size(),
            8760,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_production.dispatch_vec_kW.size(),
            8760,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_production.storage_vec_kW.size(),
            8760,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_production.curtailment_vec_kW.size(),
            8760,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_production.capital_cost_vec.size(),
            8760,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_production.operation_maintenance_cost_vec.size(),
            8760,
            __FILE__,
            __LINE__
        );
    }
    
    catch (...) {
        //...
        
        printGold(" ............................. ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }
    
    
    printGold(" ............................. ");
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
