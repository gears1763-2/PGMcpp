/*
 *  PGMcpp : PRIMED Grid Combustionling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Combustion.cpp
///
/// \brief Implementation file for the Combustion class.
///
/// The root of the Combustion branch of the Production hierarchy. This branch 
/// contains derived classes which model the production of energy by way of
/// combustibles.
///


#include "../../../header/Production/Combustion/Combustion.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Combustion :: __checkInputs(CombustionInputs combustion_inputs)
///
/// \brief Helper method to check inputs to the Combustion constructor.
///
/// \param combustion_inputs A structure of Combustion constructor inputs.
///

void Combustion :: __checkInputs(CombustionInputs combustion_inputs)
{
    // ...
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Combustion :: Combustion(void)
///
/// \brief Constructor (dummy) for the Combustion class.
///

Combustion :: Combustion(void)
{
    return;
}   /* Combustion() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Combustion :: Combustion(int n_points, CombustionInputs combustion_inputs)
///
/// \brief Constructor (intended) for the Combustion class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param combustion_inputs A structure of Combustion constructor inputs.
///

Combustion :: Combustion(int n_points, CombustionInputs combustion_inputs) :
Production(n_points, combustion_inputs.production_inputs)
{
    //  1. check inputs
    this->__checkInputs(combustion_inputs);
    
    //  2. set attributes
    this->fuel_cost_L = 0;
    
    this->linear_fuel_slope_LkWh = 0;
    this->linear_fuel_intercept_LkWh = 0;
    
    this->CO2_emissions_intensity_kgL = 0;
    this->CO_emissions_intensity_kgL = 0;
    this->NOx_emissions_intensity_kgL = 0;
    this->SOx_emissions_intensity_kgL = 0;
    this->CH4_emissions_intensity_kgL = 0;
    this->PM_emissions_intensity_kgL = 0;
    
    this->fuel_consumption_vec_L.resize(this->n_points, 0);
    this->fuel_cost_vec.resize(this->n_points, 0);
    
    this->CO2_emissions_vec_kg.resize(this->n_points, 0);
    this->CO_emissions_vec_kg.resize(this->n_points, 0);
    this->NOx_emissions_vec_kg.resize(this->n_points, 0);
    this->SOx_emissions_vec_kg.resize(this->n_points, 0);
    this->CH4_emissions_vec_kg.resize(this->n_points, 0);
    this->PM_emissions_vec_kg.resize(this->n_points, 0);
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Combustion object constructed at " << this << std::endl;
    }
    
    return;
}   /* Combustion() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Combustion :: commit(
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

double Combustion :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. invoke base class method
    load_kW = Production :: commit(
        timestep,
        dt_hrs,
        production_kW,
        load_kW
    );
    
    
    if (this->is_running) {
        //  2. compute and record fuel consumption
        double fuel_consumed_L = this->getFuelConsumptionL(dt_hrs, production_kW);
        this->fuel_consumption_vec_L[timestep] = fuel_consumed_L;
        
        //  3. compute and record emissions
        Emissions emissions = this->getEmissionskg(fuel_consumed_L);
        this->CO2_emissions_vec_kg[timestep] = emissions.CO2_kg;
        this->CO_emissions_vec_kg[timestep] = emissions.CO_kg;
        this->NOx_emissions_vec_kg[timestep] = emissions.NOx_kg;
        this->SOx_emissions_vec_kg[timestep] = emissions.SOx_kg;
        this->CH4_emissions_vec_kg[timestep] = emissions.CH4_kg;
        this->PM_emissions_vec_kg[timestep] = emissions.PM_kg;
        
        //  4. incur fuel costs
        this->fuel_cost_vec[timestep] = fuel_consumed_L * this->fuel_cost_L;
    }
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Combustion :: getFuelConsumptionL(double dt_hrs, double production_kW)
///
/// \brief Method which takes in production and returns volume of fuel burned over the
///     given interval of time.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param production_kW The production [kW] of the asset in this timestep.
///
/// \return The volume of fuel consumed [L].
///

double Combustion :: getFuelConsumptionL(double dt_hrs, double production_kW)
{
    double fuel_consumed_L = (
        this->linear_fuel_slope_LkWh * production_kW +
        this->linear_fuel_intercept_LkWh * this->capacity_kW
    ) * dt_hrs;
    
    return fuel_consumed_L;
} /* getFuelConsumptionL() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Emissions Combustion :: getEmissionskg(double fuel_consumed_L)
///
/// \brief Method which takes in volume of fuel consumed and returns mass spectrum of
///     resulting emissions.
///
/// \param fuel_consumed_L The volume of fuel consumed [L].
///
/// \return A structure containing the mass spectrum of resulting emissions.
///

Emissions Combustion :: getEmissionskg(double fuel_consumed_L) {
    Emissions emissions;
    
    emissions.CO2_kg = this->CO2_emissions_intensity_kgL * fuel_consumed_L;
    emissions.CO_kg = this->CO_emissions_intensity_kgL * fuel_consumed_L;
    emissions.NOx_kg = this->NOx_emissions_intensity_kgL * fuel_consumed_L;
    emissions.SOx_kg = this->SOx_emissions_intensity_kgL * fuel_consumed_L;
    emissions.CH4_kg = this->CH4_emissions_intensity_kgL * fuel_consumed_L;
    emissions.PM_kg = this->PM_emissions_intensity_kgL * fuel_consumed_L;
    
    return emissions;
}   /* getEmissionskg() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Combustion :: ~Combustion(void)
///
/// \brief Destructor for the Combustion class.
///

Combustion :: ~Combustion(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Combustion object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Combustion() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
