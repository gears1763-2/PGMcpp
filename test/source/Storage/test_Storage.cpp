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
/// \file test_Storage.cpp
///
/// \brief Testing suite for Storage class.
///
/// A suite of tests for the Storage class.
///


#include "../../utils/testing_utils.h"
#include "../../../header/Storage/Storage.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Storage* testConstruct_Storage(void)
///
/// \brief A function to construct a Storage object and spot check some
///     post-construction attributes.
///
/// \return A Renewable pointer to a test Storage object.
///

Storage* testConstruct_Storage(void)
{
    StorageInputs storage_inputs;

    Storage* test_storage_ptr = new Storage(8760, 1, storage_inputs);
    
    testFloatEquals(
        test_storage_ptr->power_capacity_kW,
        100,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->energy_capacity_kWh,
        1000,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->charge_vec_kWh.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->charging_power_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->discharging_power_vec_kW.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->capital_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_storage_ptr->operation_maintenance_cost_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );
    
    return test_storage_ptr;
}   /* testConstruct_Storage() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadConstruct_Storage(void)
///
/// \brief Function to test the trying to construct a Storage object given bad 
///     inputs is being handled as expected.
///

void testBadConstruct_Storage(void)
{
    bool error_flag = true;

    try {
        StorageInputs bad_storage_inputs;
        bad_storage_inputs.energy_capacity_kWh = 0;
        
        Storage bad_storage(8760, 1, bad_storage_inputs);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadConstruct_Storage() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Storage");
    
    srand(time(NULL));
    
    
    Storage* test_storage_ptr = testConstruct_Storage();
    
    
    try {
        testBadConstruct_Storage();
    }


    catch (...) {
        delete test_storage_ptr;
        
        printGold(" .................................. ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_storage_ptr;

    printGold(" .................................. ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
