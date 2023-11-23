/*
 *  PGMcpp : PRIMED Grid Solarling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Solar.cpp
///
/// \brief Implementation file for the Solar class.
///
/// A derived class of the Renewable branch of Production which models solar
/// production.
///


#include "../../../header/Production/Renewable/Solar.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Solar :: __checkInputs(SolarInputs solar_inputs)
///
/// \brief Helper method to check inputs to the Solar constructor.
///

void Solar :: __checkInputs(SolarInputs solar_inputs)
{
    //  1. check derating
    if (
        solar_inputs.derating < 0 or
        solar_inputs.derating > 1
    ) {
        std::string error_str = "ERROR:  Solar():  ";
        error_str += "SolarInputs::derating must be in the closed interval [0, 1]";
        
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
/// \fn double Solar :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic solar PV array capital cost.
///
/// This model was obtained by way of surveying an assortment of published solar PV
/// costs, and then constructing a best fit model. Note that this model
/// expresses cost in terms of Canadian dollars [CAD].
///
/// \return A generic capital cost for the solar PV array [CAD].
///

double Solar :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kW = 1000 * pow(this->capacity_kW, -0.15) + 3000;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getGenericOpMaintCost(void)
///
/// \brief Helper method to generate a generic solar PV array operation and
///     maintenance cost. This is a cost incurred per unit energy produced.
///
/// This model was obtained by way of surveying an assortment of published solar PV
/// costs, and then constructing a best fit model. Note that this model
/// expresses cost in terms of Canadian dollars [CAD/kWh].
///
/// \return A generic operation and maintenance cost, per unit energy produced, for the
///     solar PV array [CAD/kWh].
///

double Solar :: __getGenericOpMaintCost(void)
{
    return 0.01;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Solar :: Solar(void)
///
/// \brief Constructor (dummy) for the Solar class.
///

Solar :: Solar(void)
{
    //...
    
    return;
}   /* Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Solar :: Solar(void)
///
/// \brief Constructor (intended) for the Solar class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param solar_inputs A structure of Solar constructor inputs.
///

Solar :: Solar(int n_points, SolarInputs solar_inputs) :
Renewable(n_points, solar_inputs.renewable_inputs)
{
    //  1. check inputs
    this->__checkInputs(solar_inputs);
    
    //  2. set attributes
    this->type = RenewableType :: SOLAR;
    this->type_str = "SOLAR";
    
    this->resource_key = solar_inputs.resource_key;
    
    this->derating = solar_inputs.derating;
    
    if (solar_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    
    if (solar_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    
    if (this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Solar object constructed at " << this << std::endl;
    }
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: computeProductionkW(
///         int timestep,
///         double dt_hrs,
///         double solar_resource_kWm2
///     )
///
/// \brief Method which takes in the solar resource at a particular point in time, and
///     then returns the solar PV production at that point in time.
///
/// Ref: \cite HOMER_how_homer_calculates_the_pv_array_power_output\n
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param solar_resource_kWm2 Solar resource (i.e. irradiance) [kW/m2].
///
/// \return The production [kW] of the solar PV array.
///

double Solar :: computeProductionkW(
    int timestep,
    double dt_hrs,
    double solar_resource_kWm2
)
{
    // check if no resource
    if (solar_resource_kWm2 <= 0) {
        return 0;
    }
    
    // compute production
    double production_kW = this->derating * solar_resource_kWm2 * this->capacity_kW;
    
    // cap production at capacity
    if (production_kW > this->capacity_kW) {
        production_kW = this->capacity_kW;
    }
    
    return production_kW;
}   /* computeProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: commit(
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

double Solar :: commit(
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
/// \fn Solar :: ~Solar(void)
///
/// \brief Destructor for the Solar class.
///

Solar :: ~Solar(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Solar object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Solar() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
