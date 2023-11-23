/*
 *  PGMcpp : PRIMED Grid Waveling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Wave.cpp
///
/// \brief Implementation file for the Wave class.
///
/// A derived class of the Renewable branch of Production which models wave
/// production.
///


#include "../../../header/Production/Renewable/Wave.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Wave :: __checkInputs(WaveInputs wave_inputs)
///
/// \brief Helper method to check inputs to the Wave constructor.
///
/// \param wave_inputs A structure of Wave constructor inputs.
///

void Wave :: __checkInputs(WaveInputs wave_inputs)
{
    //  1. check design_significant_wave_height_m
    if (wave_inputs.design_significant_wave_height_m <= 0) {
        std::string error_str = "ERROR:  Wave():  ";
        error_str += "WaveInputs::design_significant_wave_height_m must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check design_energy_period_s
    if (wave_inputs.design_energy_period_s <= 0) {
        std::string error_str = "ERROR:  Wave():  ";
        error_str += "WaveInputs::design_energy_period_s must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkInputs() */


// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic wave energy converter capital cost.
///
/// Note that this model expresses cost in terms of Canadian dollars [CAD].
///
/// Ref: \cite MacDougall_2019\n
///
/// \return A generic capital cost for the wave energy converter [CAD].
///

double Wave :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kW = 7000 * pow(this->capacity_kW, -0.15) + 5000;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave :: __getGenericOpMaintCost(void)
///
/// \brief Helper method to generate a generic wave energy converter operation and
///     maintenance cost. This is a cost incurred per unit energy produced.
///
/// Note that this model expresses cost in terms of Canadian dollars [CAD/kWh].
///
/// Ref: \cite MacDougall_2019\n
///
/// \return A generic operation and maintenance cost, per unit energy produced, for
///     the wave energy converter [CAD/kWh].
///


double Wave :: __getGenericOpMaintCost(void)
{
    double operation_maintenance_cost_kWh = 0.05 * pow(this->capacity_kW, -0.2) + 0.05;
    
    return operation_maintenance_cost_kWh;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave:: __computeGaussianProductionkW(
///         int timestep,
///         double dt_hrs,
///         double significant_wave_height_m,
///         double energy_period_s
///     )
///
/// \brief Helper method to compute wave energy converter production under a Gaussian
///     production model.
///
/// Ref: docs/refs/wind_tidal_wave.pdf\n
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
/// 
/// \param significant_wave_height_m The significant wave height [m] in the vicinity of
///     the wave energy converter.
///
/// \param energy_period_s The energy period [s] in the vicinity of the wave energy
///     converter
///
/// \return The production [kW] of the wave energy converter, under an exponential
///     model.
///

double Wave:: __computeGaussianProductionkW(
    int timestep,
    double dt_hrs,
    double significant_wave_height_m,
    double energy_period_s
)
{
    double H_s_nondim = 
        (significant_wave_height_m - this->design_significant_wave_height_m) / 
        this->design_significant_wave_height_m;
        
    double T_e_nondim =
        (energy_period_s - this->design_energy_period_s) / 
        this->design_energy_period_s;
        
    double production = exp(
        -2.25119 * pow(T_e_nondim, 2) +
        3.44570 * T_e_nondim * H_s_nondim -
        4.01508 * pow(H_s_nondim, 2)
    );
    
    return production * this->capacity_kW;
}   /* __computeGaussianProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave:: __computeParaboloidProductionkW(
///         int timestep,
///         double dt_hrs,
///         double significant_wave_height_m,
///         double energy_period_s
///     )
///
/// \brief Helper method to compute wave energy converter production under a paraboloid
///     production model.
///
/// Ref: \cite Robertson_2021\n
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
/// 
/// \param significant_wave_height_m The significant wave height [m] in the vicinity of
///     the wave energy converter.
///
/// \param energy_period_s The energy period [s] in the vicinity of the wave energy
///     converter
///
/// \return The production [kW] of the wave energy converter, under a paraboloid
///     model.
///

double Wave:: __computeParaboloidProductionkW(
    int timestep,
    double dt_hrs,
    double significant_wave_height_m,
    double energy_period_s
)
{
    // first, check for idealized wave breaking (deep water)
    if (significant_wave_height_m >= 0.2184 * pow(energy_period_s, 2)) {
        return 0;
    }
    
    // otherwise, apply generic quadratic performance model
    // (with outputs bounded to [0, 1])
    double production = 
        0.289 * significant_wave_height_m - 
        0.00111 * pow(significant_wave_height_m, 2) * energy_period_s - 
        0.0169 * energy_period_s;
    
    if (production < 0) {
        production = 0;
    } 
    
    else if (production > 1) {
        production = 1;
    }
    
    return production * this->capacity_kW;
}   /* __computeParaboloidProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave:: __computeLookupProductionkW(
///         int timestep,
///         double dt_hrs,
///         double significant_wave_height_m,
///         double energy_period_s
///     )
///
/// \brief Helper method to compute wave energy converter production by way of
///     looking up using given performance matrix.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
/// 
/// \param significant_wave_height_m The significant wave height [m] in the vicinity of
///     the wave energy converter.
///
/// \param energy_period_s The energy period [s] in the vicinity of the wave energy
///     converter
///
/// \return The interpolated production [kW] of the wave energy converter.
///

double Wave:: __computeLookupProductionkW(
    int timestep,
    double dt_hrs,
    double significant_wave_height_m,
    double energy_period_s
)
{
    // *** WORK IN PROGRESS *** //
    
    return 0;
}   /* __computeLookupProductionkW() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Wave :: Wave(void)
///
/// \brief Constructor (dummy) for the Wave class.
///
Wave :: Wave(void)
{
    return;
}   /* Wave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Wave :: Wave(void)
///
/// \brief Constructor (intended) for the Wave class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param wave_inputs A structure of Wave constructor inputs.
///

Wave :: Wave(int n_points, WaveInputs wave_inputs) :
Renewable(n_points, wave_inputs.renewable_inputs)
{
    //  1. check inputs
    this->__checkInputs(wave_inputs);
    
    //  2. set attributes
    this->type = RenewableType :: WAVE;
    this->type_str = "WAVE";
    
    this->resource_key = wave_inputs.resource_key;
    
    this->design_significant_wave_height_m =
        wave_inputs.design_significant_wave_height_m;
    this->design_energy_period_s = wave_inputs.design_energy_period_s;
    
    this->power_model = wave_inputs.power_model;
    
    if (wave_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    
    if (wave_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    
    if (this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Wave object constructed at " << this << std::endl;
    }
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave :: computeProductionkW(
///         int timestep,
///         double dt_hrs,
///         double significant_wave_height_m,
///         double energy_period_s
///     )
///
/// \brief Method which takes in the wave resource at a particular point in time, and
///     then returns the wave turbine production at that point in time.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param signficiant_wave_height_m The significant wave height (wave statistic) [m].
///
/// \param energy_period_s The energy period (wave statistic) [s].
///
/// \return The production [kW] of the wave turbine.
///

double Wave :: computeProductionkW(
    int timestep,
    double dt_hrs,
    double significant_wave_height_m,
    double energy_period_s
)
{
    // check if no resource
    if (significant_wave_height_m <= 0 or energy_period_s <= 0) {
        return 0;
    }
    
    // compute production
    double production_kW = 0;
    
    switch (this->power_model) {
        case (WavePowerProductionModel :: WAVE_POWER_PARABOLOID): {
            production_kW = this->__computeParaboloidProductionkW(
                timestep,
                dt_hrs,
                significant_wave_height_m,
                energy_period_s
            );
            
            break;
        }
        
        case (WavePowerProductionModel :: WAVE_POWER_GAUSSIAN): {
            production_kW = this->__computeGaussianProductionkW(
                timestep,
                dt_hrs,
                significant_wave_height_m,
                energy_period_s
            );
            
            break;
        }
        
        case (WavePowerProductionModel :: WAVE_POWER_LOOKUP): {
            production_kW = this->__computeLookupProductionkW(
                timestep,
                dt_hrs,
                significant_wave_height_m,
                energy_period_s
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Wave::computeProductionkW():  ";
            error_str += "power model ";
            error_str += std::to_string(this->power_model);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return production_kW;
}   /* computeProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wave :: commit(
///         int timestep,
///         double dt_hrs,
///         double production_kW,
///         double load_kW
///     )
///
/// \brief Method which takes in production and load for the current timestep, computes
///     and records dispatch and curtailment, and then returns remaining load.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param production_kW The production [kW] of the asset in this timestep.
///
/// \param load_kW The load [kW] passed to the asset in this timestep.
///
/// \return The load [kW] remaining after the dispatch is deducted from it.
///

double Wave :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. invoke base class method
    load_kW = Renewable :: commit(
        timestep,
        dt_hrs,
        production_kW,
        load_kW
    );
    
    
    //...
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Wave :: ~Wave(void)
///
/// \brief Destructor for the Wave class.
///

Wave :: ~Wave(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Wave object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Wave() */

// ---------------------------------------------------------------------------------- //

// ======== PUBLIC ================================================================== //
