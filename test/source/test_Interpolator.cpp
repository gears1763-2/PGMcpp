/*
 *  PGMcpp : PRIMED Grid Interpolatorling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file test_Interpolator.cpp
///
/// \brief Testing suite for Interpolator class.
///
/// A suite of tests for the Interpolator class.
///


#include "../utils/testing_utils.h"
#include "../../header/Interpolator.h"


int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\n\tTesting Interpolator");
    
    srand(time(NULL));
    
    
try {

// ======== CONSTRUCTION ============================================================ //

Interpolator test_interpolator;

// ======== END CONSTRUCTION =========================================================//



// ======== ATTRIBUTES ============================================================== //

//...

// ======== END ATTRIBUTES ========================================================== //



// ======== METHODS ================================================================= //

//  1. 1D interpolation

int data_key = 1;
std::string path_2_data = "data/test/interpolation/diesel_fuel_curve.csv";

test_interpolator.addData1D(data_key, path_2_data);

testTruth(
    test_interpolator.path_map_1D[data_key] == path_2_data,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_interpolator.interp_map_1D[data_key].n_points,
    16,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_interpolator.interp_map_1D[data_key].x_vec.size(),
    16,
    __FILE__,
    __LINE__
);

std::vector<double> expected_x_vec = {
    0,
    0.3,
    0.35,
    0.4,
    0.45,
    0.5,
    0.55,
    0.6,
    0.65,
    0.7,
    0.75,
    0.8,
    0.85,
    0.9,
    0.95,
    1
};

std::vector<double> expected_y_vec = {
    4.68079520372916,
    11.1278522361839,
    12.4787834830748,
    13.7808847600209,
    15.0417468303382,
    16.277263,
    17.4612831516442,
    18.6279054806525,
    19.7698039220515,
    20.8893499214868,
    21.955378,
    23.0690535155297,
    24.1323614374927,
    25.1797231192866,
    26.2122451458747,
    27.254952
};

for (int i = 0; i < test_interpolator.interp_map_1D[data_key].n_points; i++) {
    testFloatEquals(
        test_interpolator.interp_map_1D[data_key].x_vec[i],
        expected_x_vec[i],
        __FILE__,
        __LINE__
    );
    
    testFloatEquals(
        test_interpolator.interp_map_1D[data_key].y_vec[i],
        expected_y_vec[i],
        __FILE__,
        __LINE__
    );
}

//...


//  2. 2D interpolation

data_key = 2;
path_2_data =
    "data/test/interpolation/wave_energy_converter_normalized_performance_matrix.csv";

test_interpolator.addData2D(data_key, path_2_data);

testTruth(
    test_interpolator.path_map_2D[data_key] == path_2_data,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_interpolator.interp_map_2D[data_key].n_rows,
    16,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_interpolator.interp_map_2D[data_key].n_cols,
    16,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_interpolator.interp_map_2D[data_key].x_vec.size(),
    16,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_interpolator.interp_map_2D[data_key].y_vec.size(),
    16,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_interpolator.interp_map_2D[data_key].z_matrix.size(),
    16,
    __FILE__,
    __LINE__
);

testFloatEquals(
    test_interpolator.interp_map_2D[data_key].z_matrix[0].size(),
    16,
    __FILE__,
    __LINE__
);

expected_x_vec = {
    0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25, 4.75, 5.25, 5.75, 6.25, 6.75, 7.25, 7.75
};

expected_y_vec = {
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20
};

for (int i = 0; i < test_interpolator.interp_map_2D[data_key].n_cols; i++) {
    testFloatEquals(
        test_interpolator.interp_map_2D[data_key].x_vec[i],
        expected_x_vec[i],
        __FILE__,
        __LINE__
    );
}

for (int i = 0; i < test_interpolator.interp_map_2D[data_key].n_rows; i++) {
    testFloatEquals(
        test_interpolator.interp_map_2D[data_key].y_vec[i],
        expected_y_vec[i],
        __FILE__,
        __LINE__
    );
}

std::vector<std::vector<double>> expected_z_matrix = {
    {0, 0.129128125, 0.268078125, 0.404253125, 0.537653125, 0.668278125, 0.796128125, 0.921203125, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 0.11160375, 0.24944375, 0.38395375, 0.51513375, 0.64298375, 0.76750375, 0.88869375, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0.094079375, 0.230809375, 0.363654375, 0.492614375, 0.617689375, 0.738879375, 0.856184375, 0.969604375, 1, 1, 1, 1, 1, 1, 1},
    {0, 0.076555, 0.212175, 0.343355, 0.470095, 0.592395, 0.710255, 0.823675, 0.932655, 1, 1, 1, 1, 1, 1, 1},
    {0, 0.059030625, 0.193540625, 0.323055625, 0.447575625, 0.567100625, 0.681630625, 0.791165625, 0.895705625, 0.995250625, 1, 1, 1, 1, 1, 1},
    {0, 0.04150625, 0.17490625, 0.30275625, 0.42505625, 0.54180625, 0.65300625, 0.75865625, 0.85875625, 0.95330625, 1, 1, 1, 1, 1, 1},
    {0, 0.023981875, 0.156271875, 0.282456875, 0.402536875, 0.516511875, 0.624381875, 0.726146875, 0.821806875, 0.911361875, 0.994811875, 1, 1, 1, 1, 1},
    {0, 0.0064575, 0.1376375, 0.2621575, 0.3800175, 0.4912175, 0.5957575, 0.6936375, 0.7848575, 0.8694175, 0.9473175, 1, 1, 1, 1, 1},
    {0, 0, 0.119003125, 0.241858125, 0.357498125, 0.465923125, 0.567133125, 0.661128125, 0.747908125, 0.827473125, 0.899823125, 0.964958125, 1, 1, 1, 1},
    {0, 0, 0.10036875, 0.22155875, 0.33497875, 0.44062875, 0.53850875, 0.62861875, 0.71095875, 0.78552875, 0.85232875, 0.91135875, 0.96261875, 1, 1, 1},
    {0, 0, 0.081734375, 0.201259375, 0.312459375, 0.415334375, 0.509884375, 0.596109375, 0.674009375, 0.743584375, 0.804834375, 0.857759375, 0.902359375, 0.938634375, 0.966584375, 0.986209375},
    {0, 0, 0.0631, 0.18096, 0.28994, 0.39004, 0.48126, 0.5636, 0.63706, 0.70164, 0.75734, 0.80416, 0.8421, 0.87116, 0.89134, 0.90264},
    {0, 0, 0.044465625, 0.160660625, 0.267420625, 0.364745625, 0.452635625, 0.531090625, 0.600110625, 0.659695625, 0.709845625, 0.750560625, 0.781840625, 0.803685624999999, 0.816095625, 0.819070625},
    {0, 0, 0.02583125, 0.14036125, 0.24490125, 0.33945125, 0.42401125, 0.49858125, 0.56316125, 0.61775125, 0.66235125, 0.69696125, 0.72158125, 0.73621125, 0.74085125, 0.73550125},
    {0, 0, 0.007196875, 0.120061875, 0.222381875, 0.314156875, 0.395386875, 0.466071875, 0.526211875, 0.575806875, 0.614856875, 0.643361875, 0.661321875, 0.668736875, 0.665606875, 0.651931875},
    {0, 0, 0, 0.0997625, 0.1998625, 0.2888625, 0.3667625, 0.4335625, 0.4892625, 0.5338625, 0.5673625, 0.5897625, 0.6010625, 0.6012625, 0.5903625, 0.5683625}
};

for (int i = 0; i < test_interpolator.interp_map_2D[data_key].n_rows; i++) {
    for (int j = 0; j < test_interpolator.interp_map_2D[data_key].n_cols; j++) {
        testFloatEquals(
            test_interpolator.interp_map_2D[data_key].z_matrix[i][j],
            expected_z_matrix[i][j],
            __FILE__,
            __LINE__
        );
    }
}

//...

// ======== END METHODS ============================================================= //

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
