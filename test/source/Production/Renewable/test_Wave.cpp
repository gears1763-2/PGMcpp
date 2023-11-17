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
/// \file test_Wave.cpp
///
/// \brief Testing suite for Wave class.
///
/// A suite of tests for the Wave class.
///


#include "../../../utils/testing_utils.h"
#include "../../../../header/Production/Renewable/Wave.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Production <-- Renewable <-- Wave");
    
    srand(time(NULL));
    
    Renewable* test_wave_ptr;
    
try {

// ======== CONSTRUCTION ============================================================ //

bool error_flag = true;

try {
    WaveInputs bad_wave_inputs;
    bad_wave_inputs.design_significant_wave_height_m = -1;
    
    Wave bad_wave(8760, bad_wave_inputs);
    
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

WaveInputs wave_inputs;

test_wave_ptr = new Wave(8760, wave_inputs);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    not wave_inputs.renewable_inputs.production_inputs.print_flag,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_wave_ptr->type,
    RenewableType :: WAVE,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_wave_ptr->capital_cost,
    850831.063539,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_wave_ptr->operation_maintenance_cost_kWh,
    0.069905,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

// test production constraints
testFloatEquals(
    test_wave_ptr->computeProductionkW(0, 1, 0, rand()),
    0,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_wave_ptr->computeProductionkW(0, 1, rand(), 0),
    0,
    __FILE__,
    __LINE__
);

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
double significant_wave_height_m = 0;
double energy_period_s = 0;

for (int i = 0; i < 48; i++) {
    roll = (double)rand() / RAND_MAX;
    
    if (roll <= 0.05) {
        roll = 0;
    }
    
    significant_wave_height_m = roll *
        ((Wave*)test_wave_ptr)->design_significant_wave_height_m;
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll <= 0.05) {
        roll = 0;
    }
    
    energy_period_s = roll * ((Wave*)test_wave_ptr)->design_energy_period_s;
    
    roll = (double)rand() / RAND_MAX;
    
    if (roll >= 0.95) {
        roll = 1.25;
    }
    
    load_vec_kW[i] *= roll * test_wave_ptr->capacity_kW;
    load_kW = load_vec_kW[i];
    
    production_kW = test_wave_ptr->computeProductionkW(
        i,
        dt_vec_hrs[i],
        significant_wave_height_m,
        energy_period_s
    );
    
    load_kW = test_wave_ptr->commit(
        i,
        dt_vec_hrs[i],
        production_kW,
        load_kW
    );
    
    // is running (or not) as expected
    if (production_kW > 0) {
        testTruth(
            test_wave_ptr->is_running,
            __FILE__,
            __LINE__
        );
    }
    
    else {
        testTruth(
            not test_wave_ptr->is_running,
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
        test_wave_ptr->production_vec_kW[i] -
        test_wave_ptr->dispatch_vec_kW[i] -
        test_wave_ptr->storage_vec_kW[i] -
        test_wave_ptr->curtailment_vec_kW[i],
        0,
        __FILE__,
        __LINE__
    );
    
    // resource, O&M > 0 whenever wave is running (i.e., producing)
    if (test_wave_ptr->is_running) {
        testGreaterThan(
            significant_wave_height_m,
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            energy_period_s,
            0,
            __FILE__,
            __LINE__
        );
        
        testGreaterThan(
            test_wave_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
    
    // O&M = 0 whenever wave is not running (i.e., not producing)
    else {
        testFloatEquals(
            test_wave_ptr->operation_maintenance_cost_vec[i],
            0,
            __FILE__,
            __LINE__
        );
    }
}
// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    delete test_wave_ptr;
    
    printGold(" ..... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


delete test_wave_ptr;

printGold(" ..... ");
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
