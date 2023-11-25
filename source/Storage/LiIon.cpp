/*
 *  PGMcpp : PRIMED Grid LiIonling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file LiIon.cpp
///
/// \brief Implementation file for the LiIon class.
///
/// A derived class of Storage which models energy storage by way of lithium-ion
/// batteries.
///


#include "../../header/Storage/LiIon.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __checkInputs(LiIonInputs liion_inputs)
///
/// \param liion_inputs A structure of LiIon constructor inputs.
///

void LiIon :: __checkInputs(LiIonInputs liion_inputs)
{
    //...
    
    /*
    this->replace_SOH = liion_inputs.replace_SOH;
    
    this->init_SOC = liion_inputs.init_SOC;

    this->min_SOC = liion_inputs.min_SOC;
    this->hysteresis_SOC = liion_inputs.hysteresis_SOC;
    this->max_SOC = liion_inputs.max_SOC;
    
    this->charging_efficiency = liion_inputs.charging_efficiency;
    this->discharging_efficiency = liion_inputs.discharging_efficiency;
    */
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn LiIon :: LiIon(void)
///
/// \brief Constructor (dummy) for the LiIon class.
///

LiIon :: LiIon(void)
{
    return;
}   /* LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn LiIon :: LiIon(
///         int n_points,
///         double n_years,
///         LiIonInputs liion_inputs
///     )
///
/// \brief Constructor (intended) for the LiIon class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param liion_inputs A structure of LiIon constructor inputs.
///

LiIon :: LiIon(
    int n_points,
    double n_years,
    LiIonInputs liion_inputs
) :
Storage(
    n_points,
    n_years,
    liion_inputs.storage_inputs
)
{
    //  1. check inputs
    this->__checkInputs(liion_inputs);
    
    //  2. set attributes
    this->is_depleted = false;
        
    this->dynamic_capacity_kWh = this->capacity_kWh;
    this->SOH = 1;
    this->replace_SOH = liion_inputs.replace_SOH;
    
    this->init_SOC = liion_inputs.init_SOC;

    this->min_SOC = liion_inputs.min_SOC;
    this->hysteresis_SOC = liion_inputs.hysteresis_SOC;
    this->max_SOC = liion_inputs.max_SOC;
    
    this->charging_efficiency = liion_inputs.charging_efficiency;
    this->discharging_efficiency = liion_inputs.discharging_efficiency;
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "LiIon object constructed at " << this << std::endl;
    }
    
    return;
}   /* LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void LiIon :: handleReplacement(int timestep)
{
    //  1. reset attributes
    this->is_depleted = false;
        
    this->dynamic_capacity_kWh = this->capacity_kWh;
    this->SOH = 1;
    
    /// 2. invoke base class method
    Storage::handleReplacement(timestep);
    
    //  3. correct attributes
    this->charge_kWh = this->init_SOC * this->capacity_kW;
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: getAvailablekW(int timestep)
///
/// \brief Method to get the discharge power currently available from the asset.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \return The discharging power [kW] currently available from the asset.
///

double LiIon :: getAvailablekW(int timestep)
{
    //...
    
    return 0;
}   /* getAvailablekW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: getAcceptablekW(int timestep)
///
/// \brief Method to get the charge power currently acceptable by the asset.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \return The charging power [kW] currently acceptable by the asset.
///

double LiIon :: getAcceptablekW(int timestep)
{
    //...
    
    return 0;
}   /* getAcceptablekW( */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: commitCharge(
///         int timestep,
///         double dt_hrs,
///         double charge_kW
///     )
///
/// \brief Method which takes in the charging power for the current timestep and
///     records.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param charging_kW The charging power [kw] being sent to the asset.
///

void LiIon :: commitCharge(
    int timestep,
    double dt_hrs,
    double charging_kW
)
{
    //...
    
    return;
}   /* commitCharge() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///
/// \brief 
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param discharging_kW The discharging power [kw] being drawn from the asset.
///
/// \param load_kW The load [kW] passed to the asset in this timestep.
///
/// \return The load [kW] remaining after the discharge is deducted from it.
///

double LiIon :: commitDischarge(
    int timestep,
    double dt_hrs,
    double discharging_kW,
    double load_kW
)
{
    //  1. record discharging power
    this->discharging_power_vec_kW[timestep] = discharging_kW;
    
    //  2. update charge and record
    //...
    
    //  3. update load
    load_kW -= discharging_kW;
    
    //...
    
    return load_kW;
}   /* commitDischarge() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn LiIon :: ~LiIon(void)
///
/// \brief Destructor for the LiIon class.
///

LiIon :: ~LiIon(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "LiIon object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~LiIon() */

// ---------------------------------------------------------------------------------- //

// ======== PUBLIC ================================================================== //
