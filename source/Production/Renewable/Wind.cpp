/*
 *  PGMcpp : PRIMED Grid Windling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Wind.cpp
///
/// \brief Implementation file for the Wind class.
///
/// A derived class of the Renewable branch of Production which models wind
/// production.
///


#include "../../../header/Production/Renewable/Wind.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

void Wind :: __checkInputs(WindInputs wind_inputs)
{
    /*
     *  Helper method (private) to check inputs to the Wind constructor.
     */
    
    //  1. check design_speed_ms
    if (wind_inputs.design_speed_ms <= 0) {
        std::string error_str = "ERROR:  Wind():  ";
        error_str += "WindInputs::design_speed_ms must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkInputs() */


// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Wind :: __getGenericCapitalCost(void)
{
    /*
     *  Helper method (private) to generate a generic wind turbine capital cost.
     *
     *  This model was obtained by way of surveying an assortment of published wind
     *  turbine costs, and then constructing a best fit model. Note that this model
     *  expresses cost in terms of Canadian dollars [CAD].
     */
    
    double capital_cost_per_kW = 3000 * pow(this->capacity_kW, -0.15) + 3000;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Wind :: __getGenericOpMaintCost(void)
{
    /*
     *  Helper method (private) to generate a generic wind turbine operation and
     *  maintenance cost. This is a cost incurred per unit energy produced.
     *
     *  This model was obtained by way of surveying an assortment of published wind
     *  turbine costs, and then constructing a best fit model. Note that this model
     *  expresses cost in terms of Canadian dollars [CAD/kWh].
     */
    
    double operation_maintenance_cost_kWh = 0.025 * pow(this->capacity_kW, -0.2) + 0.025;
    
    return operation_maintenance_cost_kWh;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Wind :: __computeExponentialProductionkW(
    int timestep,
    double dt_hrs,
    double wind_resource_ms
)
{
    /*
     *  Helper method (private) to compute wind turbine production under an exponential
     *  production model.
     *
     *  ref: docs/refs/wind_tidal_wave.pdf
     */
    
    double production = 0;
    
    double turbine_speed = (wind_resource_ms - this->design_speed_ms) /
        this->design_speed_ms;
        
    if (turbine_speed < -0.76 or turbine_speed > 0.68) {
        production = 0;
    }
    
    else if (turbine_speed >= -0.76 and turbine_speed <= 0) {
        production = 1.03273 * exp(-5.97588 * pow(turbine_speed, 2)) - 0.03273;
    }
    
    else {
        production = 0.16154 * exp(-9.30254 * pow(turbine_speed, 2)) + 0.83846;
    }
    
    return production * this->capacity_kW;
}   /* __computeExponentialProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Wind :: __computeLookupProductionkW(
    int timestep,
    double dt_hrs,
    double wind_resource_ms
)
{
    /*
     *  Helper method (private) to compute tidal turbine production by way of looking up
     *  using given power curve data.
     */
    
    // *** WORK IN PROGRESS *** //
    
    return 0;
}   /* __computeLookupProductionkW() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //


// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Wind :: Wind(void)
///
/// \brief Constructor (dummy) for the Wind class.
///

Wind :: Wind(void)
{
    return;
}   /* Wind() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Wind :: Wind(void)
///
/// \brief Constructor (intended) for the Wind class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param wind_inputs A structure of Wind constructor inputs.
///

Wind :: Wind(int n_points, WindInputs wind_inputs) :
Renewable(n_points, wind_inputs.renewable_inputs)
{
    //  1. check inputs
    this->__checkInputs(wind_inputs);
    
    //  2. set attributes
    this->type = RenewableType :: WIND;
    this->type_str = "WIND";
    
    this->resource_key = wind_inputs.resource_key;
    
    this->design_speed_ms = wind_inputs.design_speed_ms;
    
    this->power_model = wind_inputs.power_model;
    
    if (wind_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    
    if (wind_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    
    if (this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Wind object constructed at " << this << std::endl;
    }
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wind :: computeProductionkW(
///         int timestep,
///         double dt_hrs,
///         double wind_resource_ms
///     )
///
/// \brief Method which takes in the wind resource at a particular point in time, and
///     then returns the wind turbine production at that point in time.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param wind_resource_ms Wind resource (i.e. wind speed) [m/s].
///
/// \return The production [kW] of the wind turbine.
///

double Wind :: computeProductionkW(
    int timestep,
    double dt_hrs,
    double wind_resource_ms
)
{
    // check if no resource
    if (wind_resource_ms <= 0) {
        return 0;
    }
    
    // compute production
    double production_kW = 0;
    
    switch (this->power_model) {
        case (WindPowerProductionModel :: WIND_POWER_LOOKUP): {
            production_kW = this->__computeLookupProductionkW(
                timestep,
                dt_hrs,
                wind_resource_ms
            );
            
            break;
        }
        
        default: {   // default to WindPowerProductionModel :: WIND_POWER_EXPONENTIAL
            production_kW = this->__computeExponentialProductionkW(
                timestep,
                dt_hrs,
                wind_resource_ms
            );
            
            break;
        }
    }
    
    return production_kW;
}   /* computeProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Wind :: commit(
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

double Wind :: commit(
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
/// \fn Wind :: ~Wind(void)
///
/// \brief Destructor for the Wind class.
///

Wind :: ~Wind(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Wind object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Wind() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
