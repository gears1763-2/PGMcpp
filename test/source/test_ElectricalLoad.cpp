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
/// \file test_ElectricalLoad.cpp
///
/// \brief Testing suite for ElectricalLoad class.
///
/// A suite of tests for the ElectricalLoad class.
///


#include "../utils/testing_utils.h"
#include "../../header/ElectricalLoad.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting ElectricalLoad");
    
    srand(time(NULL));
    
    
try {

// ======== CONSTRUCTION ============================================================ //

std::string path_2_electrical_load_time_series = 
    "data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";

ElectricalLoad test_electrical_load(path_2_electrical_load_time_series);

// ======== END CONSTRUCTION ======================================================== //



// ======== ATTRIBUTES ============================================================== //

testTruth(
    test_electrical_load.path_2_electrical_load_time_series ==
    path_2_electrical_load_time_series,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_electrical_load.n_points,
    8760,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_electrical_load.n_years,
    0.999886,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_electrical_load.min_load_kW,
    82.1211213927802,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_electrical_load.mean_load_kW,
    258.373472633202,
    __FILE__,
    __LINE__
);


testFloatEquals(
    test_electrical_load.max_load_kW,
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
        test_electrical_load.dt_vec_hrs[i],
        expected_dt_vec_hrs[i],
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_electrical_load.time_vec_hrs[i],
        expected_time_vec_hrs[i],
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_electrical_load.load_vec_kW[i],
        expected_load_vec_kW[i],
        __FILE__,
        __LINE__
    );
}

// ======== END ATTRIBUTES ========================================================== //

}   /* try */


catch (...) {
    //...
    
    printGold(" ........................... ");
    printRed("FAIL");
    std::cout << std::endl;
    throw;
} 


printGold(" ........................... ");
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
