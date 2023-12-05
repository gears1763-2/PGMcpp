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
/// \file test_Solar.cpp
///
/// \brief Testing suite for Solar class.
///
/// A suite of tests for the Solar class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Renewable/Solar.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Solar* testConstruct_Solar(void)
///
/// \brief A function to construct a Solar object and spot check some
///     post-construction attributes.
///
/// \return A Renewable pointer to a test Solar object.
///

Renewable* testConstruct_Solar(void)
{
    SolarInputs solar_inputs;

    Renewable* test_solar_ptr = new Solar(8760, 1, solar_inputs);
    
    testTruth(
        not solar_inputs.renewable_inputs.production_inputs.print_flag,
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_solar_ptr->n_points,
        8760,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_solar_ptr->type,
        RenewableType :: SOLAR,
        __FILE__,
        __LINE__
    );

    testTruth(
        test_solar_ptr->type_str == "SOLAR",
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_solar_ptr->capital_cost,
        350118.723363,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_solar_ptr->operation_maintenance_cost_kWh,
        0.01,
        __FILE__,
        __LINE__
    );
    
    return test_solar_ptr;
}   /* testConstruct_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testBadConstruct_Solar(void)
///
/// \brief Function to test the trying to construct a Solar object given bad 
///     inputs is being handled as expected.
///

void testBadConstruct_Solar(void)
{
    bool error_flag = true;

    try {
        SolarInputs bad_solar_inputs;
        bad_solar_inputs.derating = -1;
        
        Solar bad_solar(8760, 1, bad_solar_inputs);
        
        error_flag = false;
    } catch (...) {
        // Task failed successfully! =P
    }
    if (not error_flag) {
        expectedErrorNotDetected(__FILE__, __LINE__);
    }
    
    return;
}   /* testBadConstruct_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testProductionConstraint_Solar(Renewable* test_solar_ptr)
///
/// \brief Function to test that the production constraint is active and behaving as
///     expected.
///
/// \param test_solar_ptr A Renewable pointer to the test Solar object.
///

void testProductionConstraint_Solar(Renewable* test_solar_ptr)
{
    testFloatEquals(
        test_solar_ptr->computeProductionkW(0, 1, 2),
        100,
        __FILE__,
        __LINE__
    );

    testFloatEquals(
        test_solar_ptr->computeProductionkW(0, 1, -1),
        0,
        __FILE__,
        __LINE__
    );
    
    return;
}   /* testProductionConstraint_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testCommit_Solar(Renewable* test_solar_ptr)
///
/// \brief Function to test if the commit method is working as expected, by checking
///     some post-call attributes of the test Solar object. Uses a randomized resource
///     input.
///
/// \param test_solar_ptr A Renewable pointer to the test Solar object.
///

void testCommit_Solar(Renewable* test_solar_ptr)
{
    std::vector<double> dt_vec_hrs (48, 1);

    std::vector<double> load_vec_kW = {
        1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
        1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0
    };

    double load_kW = 0;
    double production_kW = 0;
    double roll = 0;
    double solar_resource_kWm2 = 0;

    for (int i = 0; i < 48; i++) {
        roll = (double)rand() / RAND_MAX;
        
        solar_resource_kWm2 = roll;
        
        roll = (double)rand() / RAND_MAX;
        
        if (roll <= 0.1) {
            solar_resource_kWm2 = 0;
        }
        
        else if (roll >= 0.95) {
            solar_resource_kWm2 = 1.25;
        }
        
        roll = (double)rand() / RAND_MAX;
        
        if (roll >= 0.95) {
            roll = 1.25;
        }
        
        load_vec_kW[i] *= roll * test_solar_ptr->capacity_kW;
        load_kW = load_vec_kW[i];
        
        production_kW = test_solar_ptr->computeProductionkW(
            i,
            dt_vec_hrs[i],
            solar_resource_kWm2
        );
        
        load_kW = test_solar_ptr->commit(
            i,
            dt_vec_hrs[i],
            production_kW,
            load_kW
        );
        
        // is running (or not) as expected
        if (solar_resource_kWm2 > 0) {
            testTruth(
                test_solar_ptr->is_running,
                __FILE__,
                __LINE__
            );
        }
        
        else {
            testTruth(
                not test_solar_ptr->is_running,
                __FILE__,
                __LINE__
            );
        }
        
        // load_kW <= load_vec_kW (i.e., after vs before)
        testLessThanOrEqualTo(
            load_kW,
            load_vec_kW[i],
            __FILE__,
            __LINE__
        );
        
        // production = dispatch + storage + curtailment
        testFloatEquals(
            test_solar_ptr->production_vec_kW[i] -
            test_solar_ptr->dispatch_vec_kW[i] -
            test_solar_ptr->storage_vec_kW[i] -
            test_solar_ptr->curtailment_vec_kW[i],
            0,
            __FILE__,
            __LINE__
        );
        
        // capacity constraint
        if (solar_resource_kWm2 > 1) {
            testFloatEquals(
                test_solar_ptr->production_vec_kW[i],
                test_solar_ptr->capacity_kW,
                __FILE__,
                __LINE__
            );
        }
    }
    
    return;
}   /* testCommit_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void testEconomics_Solar(Combustion* test_solar_ptr)
///
/// \brief Function to test that the post-commit model economics for the test Solar
///     object are as expected (> 0 when running, = 0 when not).
///
/// \param test_solar_ptr A Combustion pointer to the test Solar object.
///

void testEconomics_Solar(Renewable* test_solar_ptr)
{
    for (int i = 0; i < 48; i++) {
        // resource, O&M > 0 whenever solar is running (i.e., producing)
        if (test_solar_ptr->is_running_vec[i]) {
            testGreaterThan(
                test_solar_ptr->operation_maintenance_cost_vec[i],
                0,
                __FILE__,
                __LINE__
            );
        }
        
        // resource, O&M = 0 whenever solar is not running (i.e., not producing)
        else {
            testFloatEquals(
                test_solar_ptr->operation_maintenance_cost_vec[i],
                0,
                __FILE__,
                __LINE__
            );
        }
    }
    
    return;
}   /* testEconomics_Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable <-- Solar");
    
    srand(time(NULL));
    
    
    Renewable* test_solar_ptr = testConstruct_Solar();
    
    
    try {
        testBadConstruct_Solar();
        
        testProductionConstraint_Solar(test_solar_ptr);
        
        testCommit_Solar(test_solar_ptr);
        testEconomics_Solar(test_solar_ptr);
    }


    catch (...) {
        delete test_solar_ptr;
        
        printGold(" ....... ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_solar_ptr;

    printGold(" ....... ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;

}   /* main() */

// ---------------------------------------------------------------------------------- //


/*


// ======== METHODS ================================================================= //

// test commit()
std::vector<double> dt_vec_hrs (48, 1);

std::vector<double> load_vec_kW = {
    1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
    1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0
};

double load_kW = 0;
double production_kW = 0;
double roll = 0;
double solar_resource_kWm2 = 0;

for (int i = 0; i < 48; i++) {
    roll = (double)rand() / RAND_MAX;
    
    solar_resource_kWm2 = roll;
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll <= 0.1) {
        solar_resource_kWm2 = 0;
    }
    
    else if (roll >= 0.95) {
        solar_resource_kWm2 = 1.25;
    }
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll >= 0.95) {
        roll = 1.25;
    }
    
    load_vec_kW[i] *= roll * test_solar_ptr->capacity_kW;
    load_kW = load_vec_kW[i];
    
    production_kW = test_solar_ptr->computeProductionkW(
        i,
        dt_vec_hrs[i],
        solar_resource_kWm2
    );
    
    load_kW = test_solar_ptr->commit(
        i,
        dt_vec_hrs[i],
        production_kW,
        load_kW
    );
    
    // is running (or not) as expected
    if (solar_resource_kWm2 > 0) {
        testTruth(
            test_solar_ptr->is_running,
            __FILE__,
            __LINE__
        );
    }
    
    else {
        testTruth(
            not test_solar_ptr->is_running,
            __FILE__,
            __LINE__
        );
    }
    
    // load_kW <= load_vec_kW (i.e., after vs before)
    testLessThanOrEqualTo(
        load_kW,
        load_vec_kW[i],
        __FILE__,
        __LINE__
    );
    
    // production = dispatch + storage + curtailment
    testFloatEquals(
        test_solar_ptr->production_vec_kW[i] -
        test_solar_ptr->dispatch_vec_kW[i] -
        test_solar_ptr->storage_vec_kW[i] -
        test_solar_ptr->curtailment_vec_kW[i],
        0,
        __FILE__,
        __LINE__
    );
    
    // capacity constraint
    if (solar_resource_kWm2 > 1) {
        testFloatEquals(
            test_solar_ptr->production_vec_kW[i],
            test_solar_ptr->capacity_kW,
            __FILE__,
            __LINE__
        );
    }
    
    // resource, O&M > 0 whenever solar is running (i.e., producing)
    if (test_solar_ptr->is_running) {
        testGreaterThan(
            solar_resource_kWm2,
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_solar_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
    
    // resource, O&M = 0 whenever solar is not running (i.e., not producing)
    else {
        testFloatEquals(
            solar_resource_kWm2,
            0,
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_solar_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
}


// ======== END METHODS ============================================================= //
*/
