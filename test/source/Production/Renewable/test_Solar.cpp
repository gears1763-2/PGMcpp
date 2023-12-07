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
/// \fn Renewable* testConstruct_Solar(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief A function to construct a Solar object and spot check some
///     post-construction attributes.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///
/// \return A Renewable pointer to a test Solar object.
///

Renewable* testConstruct_Solar(std::vector<double>* time_vec_hrs_ptr)
{
    SolarInputs solar_inputs;

    Renewable* test_solar_ptr = new Solar(
        8760,
        1,
        solar_inputs,
        time_vec_hrs_ptr
    );
    
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
/// \fn void testBadConstruct_Solar(std::vector<double>* time_vec_hrs_ptr)
///
/// \brief Function to test the trying to construct a Solar object given bad 
///     inputs is being handled as expected.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

void testBadConstruct_Solar(std::vector<double>* time_vec_hrs_ptr)
{
    bool error_flag = true;

    try {
        SolarInputs bad_solar_inputs;
        bad_solar_inputs.derating = -1;
        
        Solar bad_solar(8760, 1, bad_solar_inputs, time_vec_hrs_ptr);
        
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
/// \fn void testProductionOverride_Solar(
///         std::string path_2_normalized_production_time_series,
///         std::vector<double>* time_vec_hrs_ptr
///     ) 
///
/// \brief Function to test that normalized production data is being read in correctly,
///     and that the associated production override feature is behaving as expected.
///
/// \param path_2_normalized_production_time_series A path (either relative or absolute)
///     to the given normalized production time series data.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

void testProductionOverride_Solar(
    std::string path_2_normalized_production_time_series,
    std::vector<double>* time_vec_hrs_ptr
) 
{
    SolarInputs solar_inputs;
    
    solar_inputs.renewable_inputs.production_inputs.path_2_normalized_production_time_series = 
        path_2_normalized_production_time_series;

    Solar test_solar_override(
        time_vec_hrs_ptr->size(),
        1,
        solar_inputs,
        time_vec_hrs_ptr
    );
    
    
    std::vector<double> expected_normalized_production_vec = {
        0.916955708517556,
        0.90947506148393,
        0.38425267564517,
        0.191510884037643,
        0.803361391862077,
        0.261511294927198,
        0.221944653883198,
        0.858495335855501,
        0.0162863861443092,
        0.774345409915512,
        0.354898664149867,
        0.11158009453439,
        0.191670176408956,
        0.0149072402795702,
        0.30174228469322,
        0.0815062957850151,
        0.776404660266821,
        0.207069187162109,
        0.518926216750454,
        0.148538109788597,
        0.443035200791027,
        0.62119079547209,
        0.270792717524391,
        0.761074879460849,
        0.0545251308358993,
        0.0895417089500092,
        0.21787190761933,
        0.834403724509682,
        0.908807953036246,
        0.815888965292123,
        0.416663215314571,
        0.523649705576525,
        0.490890480401437,
        0.28317138282312,
        0.877382682055847,
        0.14972090597986,
        0.480161632646382,
        0.0655830129932816,
        0.41802666403448,
        0.48692477737368,
        0.275957323208066,
        0.228651250718341,
        0.574371311550247,
        0.251872481275769,
        0.802697508767121,
        0.00130607304363551,
        0.481240172488057,
        0.702527508293784
    };
    
    for (size_t i = 0; i < expected_normalized_production_vec.size(); i++) {
        testFloatEquals(
            test_solar_override.normalized_production_vec[i],
            expected_normalized_production_vec[i],
            __FILE__,
            __LINE__
        );
        
        testFloatEquals(
            test_solar_override.computeProductionkW(i, rand(), rand()),
            test_solar_override.capacity_kW * expected_normalized_production_vec[i],
            __FILE__,
            __LINE__
        );
    }
    
    return;
}   /* testProductionOverride_Solar() */

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
    
    
    std::vector<double> time_vec_hrs (8760, 0);
    for (size_t i = 0; i < time_vec_hrs.size(); i++) {
        time_vec_hrs[i] = i;
    }
    
    Renewable* test_solar_ptr = testConstruct_Solar(&time_vec_hrs);
    
    
    try {
        testBadConstruct_Solar(&time_vec_hrs);
        
        std::string path_2_normalized_production_time_series = 
            "data/test/normalized_production/normalized_solar_production.csv";
        
        testProductionOverride_Solar(
            path_2_normalized_production_time_series,
            &time_vec_hrs
        );
        
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
