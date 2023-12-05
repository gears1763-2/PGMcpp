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
/// \file test_LiIon.cpp
///
/// \brief Testing suite for LiIon class.
///
/// A suite of tests for the LiIon class.
///


#include "../../utils/testing_utils.h"
#include "../../../header/Storage/LiIon.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Storage* testConstruct_LiIon(void)
///
/// \brief A function to construct a LiIon object and spot check some
///     post-construction attributes.
///
/// \return A Storage pointer to a test LiIon object.
///

Storage* testConstruct_LiIon(void)
{
    LiIonInputs liion_inputs;

    Storage* test_liion_ptr = new LiIon(8760, 1, liion_inputs);
    
    testTruth(
        test_liion_ptr->type_str == "LIION",
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((LiIon*)test_liion_ptr)->init_SOC,
        0.5,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((LiIon*)test_liion_ptr)->min_SOC,
        0.15,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((LiIon*)test_liion_ptr)->hysteresis_SOC,
        0.5,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((LiIon*)test_liion_ptr)->max_SOC,
        0.9,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((LiIon*)test_liion_ptr)->charging_efficiency,
        0.9,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((LiIon*)test_liion_ptr)->discharging_efficiency,
        0.9,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((LiIon*)test_liion_ptr)->replace_SOH,
        0.8,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((LiIon*)test_liion_ptr)->power_kW,
        0,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        ((LiIon*)test_liion_ptr)->SOH_vec.size(),
        8760,
        __FILE__,
        __LINE__
    );
    
    return test_liion_ptr;
}   /* testConstruct_LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadConstruct_LiIon(void)
///
/// \brief Function to test the trying to construct a LiIon object given bad 
///     inputs is being handled as expected.
///

void testBadConstruct_LiIon(void)
{
    bool error_flag = true;

    try {
        LiIonInputs bad_liion_inputs;
        bad_liion_inputs.min_SOC = -1;
        
        LiIon bad_liion(8760, 1, bad_liion_inputs);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadConstruct_LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testCommitCharge_LiIon(Storage* test_liion_ptr)
///
/// \brief A function to test commitCharge() and ensure that its impact on acceptable
///     and available power is as expected.
///
/// \param test_liion_ptr A Storage pointer to a test LiIon object.
///

void testCommitCharge_LiIon(Storage* test_liion_ptr)
{
    double dt_hrs = 1;
    
    testFloatEquals(
        test_liion_ptr->getAvailablekW(dt_hrs),
        100,    // hits power capacity constraint
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_liion_ptr->getAcceptablekW(dt_hrs),
        100,    // hits power capacity constraint
        __FILE__,
        __LINE__
    );

    test_liion_ptr->power_kW = 1e6; // as if a massive amount of power is already flowing in

    testFloatEquals(
        test_liion_ptr->getAvailablekW(dt_hrs),
        0,    // is already hitting power capacity constraint
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_liion_ptr->getAcceptablekW(dt_hrs),
        0,    // is already hitting power capacity constraint
        __FILE__,
        __LINE__
    );

    test_liion_ptr->commitCharge(0, dt_hrs, 100);

    testFloatEquals(
        test_liion_ptr->power_kW,
        0,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testCommitCharge_LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testCommitDischarge_LiIon(Storage* test_liion_ptr)
///
/// \brief A function to test commitDischarge() and ensure that its impact on acceptable
///     and available power is as expected.
///
/// \param test_liion_ptr A Storage pointer to a test LiIon object.
///

void testCommitDischarge_LiIon(Storage* test_liion_ptr)
{
    double dt_hrs = 1;
    double load_kW = 100;
    
    testFloatEquals(
        test_liion_ptr->getAvailablekW(dt_hrs),
        100,    // hits power capacity constraint
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_liion_ptr->getAcceptablekW(dt_hrs),
        100,    // hits power capacity constraint
        __FILE__,
        __LINE__
    );

    test_liion_ptr->power_kW = 1e6; // as if a massive amount of power is already flowing out

    testFloatEquals(
        test_liion_ptr->getAvailablekW(dt_hrs),
        0,    // is already hitting power capacity constraint
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_liion_ptr->getAcceptablekW(dt_hrs),
        0,    // is already hitting power capacity constraint
        __FILE__,
        __LINE__
    );

    load_kW = test_liion_ptr->commitDischarge(0, dt_hrs, 100, load_kW);
    
    testFloatEquals(
        load_kW,
        0,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_liion_ptr->power_kW,
        0,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testCommitDischarge_LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Storage <-- LiIon");
    
    srand(time(NULL));
    
    
    Storage* test_liion_ptr = testConstruct_LiIon();
    
    
    try {
        testBadConstruct_LiIon();
        
        testCommitCharge_LiIon(test_liion_ptr);
        testCommitDischarge_LiIon(test_liion_ptr);
    }


    catch (...) {
        delete test_liion_ptr;
        
        printGold(" ........................ ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_liion_ptr;

    printGold(" ........................ ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //
