/*
 *  PGMcpp : PRIMED Grid Tidalling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Tidal.cpp
///
/// \brief Implementation file for the Tidal class.
///
/// A derived class of the Renewable branch of Production which models tidal
/// production.
///


#include "../../../header/Production/Renewable/Tidal.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

void Tidal :: __checkInputs(TidalInputs tidal_inputs)
{
    /*
     *  Helper method (private) to check inputs to the Tidal constructor.
     */
    
    //  1. check design_speed_ms
    if (tidal_inputs.design_speed_ms <= 0) {
        std::string error_str = "ERROR: Tidal():\t";
        error_str += "TidalInputs::design_speed_ms must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkInputs() */


// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Tidal :: __getGenericCapitalCost(void)
{
    /*
     *  Helper method (private) to generate a generic tidal turbine capital cost.
     *
     *  ref:  Dr. S.L. MacDougall, Commercial Potential of Marine
     *        Renewables in British Columbia, technical report
     *        submitted to Natural Resources Canada, 
     *        S.L. MacDougall Research & Consulting, 2019
     */
    
    double capital_cost_per_kW =
        4497 * exp(0.0002 * log(0.894596397) * this->capacity_kW) + 2000;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Tidal :: __getGenericOpMaintCost(void)
{
    /*
     *  Helper method (private) to generate a generic tidal turbine operation and
     *  maintenance cost. This is a cost incurred per unit energy produced.
     *
     *  ref:  Dr. S.L. MacDougall, Commercial Potential of Marine
     *        Renewables in British Columbia, technical report
     *        submitted to Natural Resources Canada, 
     *        S.L. MacDougall Research & Consulting, 2019
     */
    
    return 0.05;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Tidal :: __computeCubicProductionkW(
    int timestep,
    double dt_hrs,
    double tidal_resource_ms
)
{
    /*
     *  Helper method (private) to compute tidal turbine production under a cubic
     *  production model.
     *
     *  ref:  B. Buckham, C. Crawford, I. Beya Marshall, and
     *        B. Whitby, "Wei Wai Kum Tidal Prefeasibility
     *        Study - Tidal Resource Assessment", PRIMED
     *        technical report, 2023,
     *        P2202E_BRKLYG+WEI WAI KUM_R01_ V20230613v3
     */
    
    double production = 0;
    
    if (
        tidal_resource_ms < 0.15 * this->design_speed_ms or
        tidal_resource_ms > 1.25 * this->design_speed_ms
    ){
        production = 0;
    }
    
    else if (
        0.15 * this->design_speed_ms <= tidal_resource_ms and
        tidal_resource_ms <= this->design_speed_ms
    ) {
        production = 
            (1 / pow(this->design_speed_ms, 3)) * pow(tidal_resource_ms, 3);
    }
    
    else {
        production = 1;
    }
    
    return production * this->capacity_kW;
}   /* __computeCubicProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Tidal :: __computeExponentialProductionkW(
    int timestep,
    double dt_hrs,
    double tidal_resource_ms
)
{
    /*
     *  Helper method (private) to compute tidal turbine production under an exponential
     *  production model.
     *
     *  ref: docs/refs/wind_tidal_wave.pdf
     */
    
    double production = 0;
    
    double turbine_speed =
        (tidal_resource_ms - this->design_speed_ms) / this->design_speed_ms;
        
    if (turbine_speed < -0.71 or turbine_speed > 0.65) {
        production = 0;
    }
    
    else if (turbine_speed >= -0.71 and turbine_speed <= 0) {
        production = 1.69215 * exp(1.25909 * turbine_speed) - 0.69215;
    }
    
    else {
        production = 1;
    }
    
    return production * this->capacity_kW;
}   /* __computeExponentialProductionkW() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Tidal :: Tidal(void)
///
/// \brief Constructor (dummy) for the Tidal class.
///

Tidal :: Tidal(void)
{
    return;
}   /* Tidal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Tidal :: Tidal(void)
///
/// \brief Constructor (intended) for the Tidal class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param tidal_inputs A structure of Tidal constructor inputs.
///

Tidal :: Tidal(int n_points, TidalInputs tidal_inputs) :
Renewable(n_points, tidal_inputs.renewable_inputs)
{
    //  1. check inputs
    this->__checkInputs(tidal_inputs);
    
    //  2. set attributes
    this->type = RenewableType :: TIDAL;
    
    this->resource_key = tidal_inputs.resource_key;
    
    this->design_speed_ms = tidal_inputs.design_speed_ms;
    
    this->power_model = tidal_inputs.power_model;
    
    if (tidal_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    
    if (tidal_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    
    if (this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Tidal object constructed at " << this << std::endl;
    }
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Tidal :: computeProductionkW(
///         int timestep,
///         double dt_hrs,
///         double tidal_resource_ms
///     )
///
/// \brief Method which takes in the tidal resource at a particular point in time, and
///     then returns the tidal turbine production at that point in time.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param tidal_resource_ms Tidal resource (i.e. tidal stream speed) [m/s].
///
/// \return The production [kW] of the tidal turbine.
///

double Tidal :: computeProductionkW(
    int timestep,
    double dt_hrs,
    double tidal_resource_ms
)
{
    // check if no resource
    if (tidal_resource_ms <= 0) {
        return 0;
    }
    
    // compute production
    double production_kW = 0;
    
    switch (this->power_model) {
        case (TidalPowerProductionModel :: EXPONENTIAL): {
            production_kW = this->__computeExponentialProductionkW(
                timestep,
                dt_hrs,
                tidal_resource_ms
            );
            
            break;
        }
        
        default: {   // default to CUBIC
            production_kW = this->__computeCubicProductionkW(
                timestep,
                dt_hrs,
                tidal_resource_ms
            );
            
            break;
        }
    }
    
    return production_kW;
}   /* computeProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Tidal :: commit(
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

double Tidal :: commit(
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
/// \fn Tidal :: ~Tidal(void)
///
/// \brief Destructor for the Tidal class.
///

Tidal :: ~Tidal(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Tidal object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Tidal() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
