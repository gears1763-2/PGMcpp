/*
 *  PGMcpp : PRIMED Grid Resourcesling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file test_Resources.cpp
///
/// \brief Testing suite for Resources class.
///
/// A suite of tests for the Resources class.
///


#include "../utils/testing_utils.h"
#include "../../header/Resources.h"
#include "../../header/ElectricalLoad.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Resources");
    
    srand(time(NULL));
    
    
try {

// ======== CONSTRUCTION ============================================================ //

std::string path_2_electrical_load_time_series = 
    "data/test/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";

ElectricalLoad test_electrical_load(path_2_electrical_load_time_series);

Resources test_resources;

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testFloatEquals(
    test_resources.resource_map_1D.size(),
    0,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_resources.path_map_1D.size(),
    0,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_resources.resource_map_2D.size(),
    0,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_resources.path_map_2D.size(),
    0,
    __FILE__,
    __LINE__
);

// ======== END ATTRIBUTES ========================================================== //


// ======== METHODS ================================================================= //

int solar_resource_key = 0;
std::string path_2_solar_resource_data =
    "data/test/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv";

test_resources.addResource(
    RenewableType::SOLAR,
    path_2_solar_resource_data,
    solar_resource_key,
    &test_electrical_load
);

bool error_flag = true;
try {
    test_resources.addResource(
        RenewableType::SOLAR,
        path_2_solar_resource_data,
        solar_resource_key,
        &test_electrical_load
    );

    error_flag = false;
} catch (...) {
    // Task failed successfully! =P
}
if (not error_flag) {
    expectedErrorNotDetected(__FILE__, __LINE__);
}

//...


int tidal_resource_key = 1;
std::string path_2_tidal_resource_data =
    "data/test/tidal_speed_peak-3ms_1yr_dt-1hr.csv";

test_resources.addResource(
    RenewableType::TIDAL,
    path_2_tidal_resource_data,
    tidal_resource_key,
    &test_electrical_load
);

//...


int wave_resource_key = 2;
std::string path_2_wave_resource_data =
    "data/test/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv";

test_resources.addResource(
    RenewableType::WAVE,
    path_2_wave_resource_data,
    wave_resource_key,
    &test_electrical_load
);

//...


int wind_resource_key = 2;
std::string path_2_wind_resource_data =
    "data/test/wind_speed_peak-25ms_1yr_dt-1hr.csv";

test_resources.addResource(
    RenewableType::WIND,
    path_2_wind_resource_data,
    wind_resource_key,
    &test_electrical_load
);

//...

// ======== END METHODS ============================================================= //

}   /* try */


catch (...) {
    printGold(" .............................. ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
}


printGold(" .............................. ");
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
