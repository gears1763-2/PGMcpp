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
/// \file example_project.cpp
///
/// \brief An example PGMcpp project, accessed natively in C++.
///
/// This is an example PGMcpp project. See the data provided in data/test/ for examples
/// of the expected format.
///


#include "../header/Model.h"


int main(int argc, char** argv)
{
    //  1. construct Model object
    std::string path_2_electrical_load_time_series =
        "data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv";
    
    ModelInputs model_inputs;
    
    model_inputs.path_2_electrical_load_time_series = 
        path_2_electrical_load_time_series;
    
    model_inputs.control_mode = ControlMode :: CYCLE_CHARGING;
    
    Model model(model_inputs);
    
    
    //  2. add Diesel objects to Model
    //     assume diesel generators are sunk assets (no initial capital cost)
    DieselInputs diesel_inputs;
    
    //  2.1. add 1 x 300 kW diesel generator (since mean load is ~250 kW)
    diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 300;    //<-- accessing and changing an encapsulated structure attributed
    diesel_inputs.combustion_inputs.production_inputs.is_sunk = true;
    
    model.addDiesel(diesel_inputs);
    
    //  2.2. add 2 x 150 kW diesel generators (since max load is 500 kW)
    diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 150;
    
    model.addDiesel(diesel_inputs);
    model.addDiesel(diesel_inputs);
    
    
    //  3. add renewable resources to Model
    
    //  3.1. add solar resource time series
    int solar_resource_key = 0;
    std::string path_2_solar_resource_data =
        "data/test/resources/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv";

    model.addResource(
        RenewableType :: SOLAR,
        path_2_solar_resource_data,
        solar_resource_key
    );
    
    //  3.2. add tidal resource time series
    int tidal_resource_key = 1;
    std::string path_2_tidal_resource_data =
        "data/test/resources/tidal_speed_peak-3ms_1yr_dt-1hr.csv";

    model.addResource(
        RenewableType :: TIDAL,
        path_2_tidal_resource_data,
        tidal_resource_key
    );
    
    //  3.3. add wave resource time series
    int wave_resource_key = 2;
    std::string path_2_wave_resource_data =
        "data/test/resources/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv";

    model.addResource(
        RenewableType :: WAVE,
        path_2_wave_resource_data,
        wave_resource_key
    );
    
    //  3.4. add wind resource time series
    int wind_resource_key = 3;
    std::string path_2_wind_resource_data =
        "data/test/resources/wind_speed_peak-25ms_1yr_dt-1hr.csv";

    model.addResource(
        RenewableType :: WIND,
        path_2_wind_resource_data,
        wind_resource_key
    );
    
    
    //  4. add Renewable objects to Model
    
    //  4.1. add 1 x 250 kW solar PV array
    SolarInputs solar_inputs;
    
    solar_inputs.renewable_inputs.production_inputs.capacity_kW = 250;
    solar_inputs.resource_key = solar_resource_key;

    model.addSolar(solar_inputs);
    
    //  4.2. add 1 x 120 kW tidal turbine
    TidalInputs tidal_inputs;
    
    tidal_inputs.renewable_inputs.production_inputs.capacity_kW = 120;
    tidal_inputs.design_speed_ms = 2.5;
    tidal_inputs.resource_key = tidal_resource_key;

    model.addTidal(tidal_inputs);
    
    //  4.3. add 1 x 150 kW wind turbine
    WindInputs wind_inputs;
    
    wind_inputs.renewable_inputs.production_inputs.capacity_kW = 150;
    wind_inputs.resource_key = wind_resource_key;

    model.addWind(wind_inputs);
    
    //  4.4. add 1 x 100 kW wave energy converter
    WaveInputs wave_inputs;
    
    wave_inputs.renewable_inputs.production_inputs.capacity_kW = 100;
    wave_inputs.resource_key = wave_resource_key;

    model.addWave(wave_inputs);
    
    
    //  5. add LiIon object to Model
    
    //  5.1. add 1 x (500 kW, ) lithium ion battery energy storage system
    LiIonInputs liion_inputs;
    
    liion_inputs.storage_inputs.power_capacity_kW = 500;
    liion_inputs.storage_inputs.energy_capacity_kWh = 1050; //<-- about 4 hours of mean load autonomy

    model.addLiIon(liion_inputs);
    
    
    //  6. run and write results
    model.run();
    
    model.writeResults("projects/example_cpp");
    
    return 0;
}   /* main() */
