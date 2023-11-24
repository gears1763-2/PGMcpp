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
/// \file test_Model.cpp
///
/// \brief Testing suite for Model class.
///
/// A suite of tests for the Model class.
///


#include "../utils/testing_utils.h"
#include "../../header/Model.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Model");
    
    srand(time(NULL));
    
    
try {

// ======== CONSTRUCTION ============================================================ //

bool error_flag = true;

try {
    ModelInputs bad_model_inputs;
    bad_model_inputs.path_2_electrical_load_time_series =
        "data/test/bad_path_240984069830.csv";
    
    Model bad_model(bad_model_inputs);
    
    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

std::string path_2_electrical_load_time_series =
    "data/test/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";

ModelInputs test_model_inputs;
test_model_inputs.path_2_electrical_load_time_series =
    path_2_electrical_load_time_series;

Model test_model(test_model_inputs);

// ======== END CONSTRUCTION ======================================================== //


// ======== ATTRIBUTES ============================================================== //

testTruth(
    test_model.electrical_load.path_2_electrical_load_time_series ==
    path_2_electrical_load_time_series,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.electrical_load.n_points,
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.electrical_load.n_years,
    0.999886,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.electrical_load.min_load_kW,
    82.1211213927802,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.electrical_load.mean_load_kW,
    258.373472633202,
    __FILE__,
    __LINE__
);


testFloatEquals(
    test_model.electrical_load.max_load_kW,
    500,
    __FILE__,
    __LINE__
);


std::vector<double> expected_dt_vec_hrs (48, 1);

std::vector<double> expected_time_vec_hrs = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11,
    12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47
};

std::vector<double> expected_load_vec_kW = {
    360.253836463674,
    355.171277826775,
    353.776453532298,
    353.75405737934,
    346.592867404975,
    340.132411175118,
    337.354867340578,
    340.644115618736,
    363.639028500678,
    378.787797779238,
    372.215798201712,
    395.093925731298,
    402.325427142659,
    386.907725462306,
    380.709170928091,
    372.062070914977,
    372.328646856954,
    391.841444284136,
    394.029351759596,
    383.369407765254,
    381.093099675206,
    382.604158946193,
    390.744843709034,
    383.13949492437,
    368.150393976985,
    364.629744480226,
    363.572736804082,
    359.854924202248,
    355.207590170267,
    349.094656012401,
    354.365935871597,
    343.380608328546,
    404.673065729266,
    486.296896820126,
    480.225974100847,
    457.318764401085,
    418.177339948609,
    414.399018364126,
    409.678420185754,
    404.768766016563,
    401.699589920585,
    402.44339040654,
    398.138372541906,
    396.010498627646,
    390.165117432277,
    375.850429417013,
    365.567100746484,
    365.429624610923
};

for (int i = 0; i < 48; i++) {
    testFloatEquals(
        test_model.electrical_load.dt_vec_hrs[i],
        expected_dt_vec_hrs[i],
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.electrical_load.time_vec_hrs[i],
        expected_time_vec_hrs[i],
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_model.electrical_load.load_vec_kW[i],
        expected_load_vec_kW[i],
        __FILE__,
        __LINE__
    );
}

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

//  add Solar resource
int solar_resource_key = 0;
std::string path_2_solar_resource_data =
    "data/test/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv";

test_model.addResource(
    RenewableType :: SOLAR,
    path_2_solar_resource_data,
    solar_resource_key
);

std::vector<double> expected_solar_resource_vec_kWm2 = {
    0,
    0,
    0,
    0,
    0,
    0,
    8.51702662684015E-05,
    0.000348341567045,
    0.00213793728593,
    0.004099863613322,
    0.000997135230553,
    0.009534527624657,
    0.022927996790616,
    0.0136071715294,
    0.002535134127751,
    0.005206897515821,
    0.005627658648597,
    0.000701186722215,
    0.00017119827089,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0.000141055102242,
    0.00084525014743,
    0.024893647822702,
    0.091245556190749,
    0.158722176731637,
    0.152859680515876,
    0.149922903895116,
    0.13049996570866,
    0.03081254222795,
    0.001218928911125,
    0.000206092647423,
    0,
    0,
    0,
    0,
    0,
    0
};

for (size_t i = 0; i < expected_solar_resource_vec_kWm2.size(); i++) {
    testFloatEquals(
        test_model.resources.resource_map_1D[solar_resource_key][i],
        expected_solar_resource_vec_kWm2[i],
        __FILE__,
        __LINE__
    );
}


//  add Tidal resource
int tidal_resource_key = 1;
std::string path_2_tidal_resource_data =
    "data/test/tidal_speed_peak-3ms_1yr_dt-1hr.csv";

test_model.addResource(
    RenewableType :: TIDAL,
    path_2_tidal_resource_data,
    tidal_resource_key
);


//  add Wave resource
int wave_resource_key = 2;
std::string path_2_wave_resource_data =
    "data/test/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv";

test_model.addResource(
    RenewableType :: WAVE,
    path_2_wave_resource_data,
    wave_resource_key
);


//  add Wind resource
int wind_resource_key = 3;
std::string path_2_wind_resource_data =
    "data/test/wind_speed_peak-25ms_1yr_dt-1hr.csv";

test_model.addResource(
    RenewableType :: WIND,
    path_2_wind_resource_data,
    wind_resource_key
);


//  add Diesel assets
DieselInputs diesel_inputs;
diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 100;

test_model.addDiesel(diesel_inputs);

testFloatEquals(
    test_model.combustion_ptr_vec.size(),
    1,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.combustion_ptr_vec[0]->type,
    CombustionType :: DIESEL,
    __FILE__,
    __LINE__
);

diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 150;

test_model.addDiesel(diesel_inputs);

diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 250;

test_model.addDiesel(diesel_inputs);

testFloatEquals(
    test_model.combustion_ptr_vec.size(),
    3,
    __FILE__,
    __LINE__
);

std::vector<int> expected_diesel_capacity_vec_kW = {100, 150, 250};

for (int i = 0; i < 3; i++) {
    testFloatEquals(
        test_model.combustion_ptr_vec[i]->capacity_kW,
        expected_diesel_capacity_vec_kW[i],
        __FILE__,
        __LINE__
    );
}

diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 100;

for (int i = 0; i < 2 * ((double)rand() / RAND_MAX); i++) {
    test_model.addDiesel(diesel_inputs);
}


//  add Solar asset
SolarInputs solar_inputs;
solar_inputs.resource_key = solar_resource_key;

test_model.addSolar(solar_inputs);

testFloatEquals(
    test_model.renewable_ptr_vec.size(),
    1,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.renewable_ptr_vec[0]->type,
    RenewableType :: SOLAR,
    __FILE__,
    __LINE__
);


//  add Tidal asset
TidalInputs tidal_inputs;
tidal_inputs.resource_key = tidal_resource_key;

test_model.addTidal(tidal_inputs);

testFloatEquals(
    test_model.renewable_ptr_vec.size(),
    2,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.renewable_ptr_vec[1]->type,
    RenewableType :: TIDAL,
    __FILE__,
    __LINE__
);


//  add Wave asset
WaveInputs wave_inputs;
wave_inputs.resource_key = wave_resource_key;

test_model.addWave(wave_inputs);

testFloatEquals(
    test_model.renewable_ptr_vec.size(),
    3,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.renewable_ptr_vec[2]->type,
    RenewableType :: WAVE,
    __FILE__,
    __LINE__
);


//  add Wind asset
WindInputs wind_inputs;
wind_inputs.resource_key = wind_resource_key;

test_model.addWind(wind_inputs);

testFloatEquals(
    test_model.renewable_ptr_vec.size(),
    4,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.renewable_ptr_vec[3]->type,
    RenewableType :: WIND,
    __FILE__,
    __LINE__
);


//  run
test_model.run();

for (int i = 0; i < test_model.electrical_load.n_points; i++) {
    testLessThanOrEqualTo(
        test_model.controller.net_load_vec_kW[i],
        test_model.electrical_load.max_load_kW,
        __FILE__,
        __LINE__
    );
}

testGreaterThan(
    test_model.net_present_cost,
    0,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_model.total_dispatch_discharge_kWh,
    2263351.62026685,
    __FILE__,
    __LINE__
);

testGreaterThan(
    test_model.levellized_cost_of_energy_kWh,
    0,
    __FILE__,
    __LINE__
);

testGreaterThan(
    test_model.total_fuel_consumed_L,
    0,
    __FILE__,
    __LINE__
);

testGreaterThan(
    test_model.total_emissions.CO2_kg,
    0,
    __FILE__,
    __LINE__
);

testGreaterThan(
    test_model.total_emissions.CO_kg,
    0,
    __FILE__,
    __LINE__
);

testGreaterThan(
    test_model.total_emissions.NOx_kg,
    0,
    __FILE__,
    __LINE__
);

testGreaterThan(
    test_model.total_emissions.SOx_kg,
    0,
    __FILE__,
    __LINE__
);

testGreaterThan(
    test_model.total_emissions.CH4_kg,
    0,
    __FILE__,
    __LINE__
);

testGreaterThan(
    test_model.total_emissions.PM_kg,
    0,
    __FILE__,
    __LINE__
);

// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    //...
    
    printGold(" .................................. ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


printGold(" .................................. ");
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
