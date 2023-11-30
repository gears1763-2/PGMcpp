"""
    PGMcpp : PRIMED Grid Modelling (in C++)
    
    Anthony Truelove MASc, P.Eng.
    email:  gears1763@tutanota.com
    github: gears1763-2
    
    See license terms
    
    An example PGMcpp project, accessed using the Python bindings for PGMcpp. See the
    data provided in data/test/ for examples of the expected format.
"""

import os
import sys
sys.path.insert(0, "../pybindings")  # <-- just need to point to a directory with the PGMcpp extension

import PGMcpp


#   1. construct Model object
path_2_electrical_load_time_series = (
    "../data/test/electrical_load/electrical_load_generic_peak-500kW_1yr_dt-1hr.csv"
)

model_inputs = PGMcpp.ModelInputs()

model_inputs.path_2_electrical_load_time_series = path_2_electrical_load_time_series
model_inputs.control_mode = PGMcpp.ControlMode.CYCLE_CHARGING

model = PGMcpp.Model(model_inputs)


#   2. add Diesel objects to Model
#      assume diesel generators are sunk assets (no initial capital cost)
diesel_inputs = PGMcpp.DieselInputs()

#   2.1. add 1 x 300 kW diesel generator (since mean load is ~250 kW)
diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 300    #<-- accessing and changing an encapsulated structure attributed
diesel_inputs.combustion_inputs.production_inputs.is_sunk = True

model.addDiesel(diesel_inputs)

#   2.2. add 2 x 150 kW diesel generators (since max load is 500 kW)
diesel_inputs.combustion_inputs.production_inputs.capacity_kW = 150

model.addDiesel(diesel_inputs)
model.addDiesel(diesel_inputs)


#   3. add renewable resources to Model
    
#   3.1. add solar resource time series
solar_resource_key = 0
path_2_solar_resource_data = (
    "../data/test/resources/solar_GHI_peak-1kWm2_1yr_dt-1hr.csv"
)

model.addResource(
    PGMcpp.RenewableType.SOLAR,
    path_2_solar_resource_data,
    solar_resource_key
)

#   3.2. add tidal resource time series
tidal_resource_key = 1
path_2_tidal_resource_data = (
    "../data/test/resources/tidal_speed_peak-3ms_1yr_dt-1hr.csv"
)

model.addResource(
    PGMcpp.RenewableType.TIDAL,
    path_2_tidal_resource_data,
    tidal_resource_key
)

#   3.3. add wave resource time series
wave_resource_key = 2
path_2_wave_resource_data = (
    "../data/test/resources/waves_H_s_peak-8m_T_e_peak-15s_1yr_dt-1hr.csv"
)

model.addResource(
    PGMcpp.RenewableType.WAVE,
    path_2_wave_resource_data,
    wave_resource_key
)

#   3.4. add wind resource time series
wind_resource_key = 3
path_2_wind_resource_data = (
    "../data/test/resources/wind_speed_peak-25ms_1yr_dt-1hr.csv"
)

model.addResource(
    PGMcpp.RenewableType.WIND,
    path_2_wind_resource_data,
    wind_resource_key
)

#   3.5. add hydro resource time series
hydro_resource_key = 4
path_2_hydro_resource_data = (
    "../data/test/resources/hydro_inflow_peak-20000m3hr_1yr_dt-1hr.csv"
)

model.addResource(
    PGMcpp.NoncombustionType.HYDRO,
    path_2_hydro_resource_data,
    hydro_resource_key
)


#   4. add Hydro object to Model
#      assume hydroelectric is a sunk asset (no initial capital cost)

#   4.1. add 1 x 300 kW hydroelectric plant with a 10,000 m3 reservoir
hydro_inputs = PGMcpp.HydroInputs()
hydro_inputs.noncombustion_inputs.production_inputs.capacity_kW = 300
hydro_inputs.reservoir_capacity_m3 = 10000
hydro_inputs.init_reservoir_state = 0.5    #<-- reservoir initially at 50%
hydro_inputs.noncombustion_inputs.production_inputs.is_sunk = True
hydro_inputs.resource_key = hydro_resource_key

model.addHydro(hydro_inputs);


#   5. add Renewable objects to Model

#   5.1. add 1 x 250 kW solar PV array
solar_inputs = PGMcpp.SolarInputs()

solar_inputs.renewable_inputs.production_inputs.capacity_kW = 250
solar_inputs.resource_key = solar_resource_key

model.addSolar(solar_inputs)

#   5.2. add 1 x 120 kW tidal turbine
tidal_inputs = PGMcpp.TidalInputs()

tidal_inputs.renewable_inputs.production_inputs.capacity_kW = 120
tidal_inputs.design_speed_ms = 2.5
tidal_inputs.resource_key = tidal_resource_key

model.addTidal(tidal_inputs)

#   5.3. add 1 x 150 kW wind turbine
wind_inputs = PGMcpp.WindInputs()

wind_inputs.renewable_inputs.production_inputs.capacity_kW = 150
wind_inputs.resource_key = wind_resource_key

model.addWind(wind_inputs)

#   5.4. add 1 x 100 kW wave energy converter
wave_inputs = PGMcpp.WaveInputs()

wave_inputs.renewable_inputs.production_inputs.capacity_kW = 100
wave_inputs.resource_key = wave_resource_key

model.addWave(wave_inputs)


#   6. add LiIon object to Model

#   6.1. add 1 x (500 kW, ) lithium ion battery energy storage system
liion_inputs = PGMcpp.LiIonInputs()

liion_inputs.storage_inputs.power_capacity_kW = 500
liion_inputs.storage_inputs.energy_capacity_kWh = 1050  #<-- about 4 hours of mean load autonomy

model.addLiIon(liion_inputs)


#   7. run and write results
model.run()

model.writeResults("example_py")
