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

//...

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Solar :: Solar(void)
///
/// \brief Constructor for the Solar class.
///
// \param [...]

Solar :: Solar(void) :
Renewable()
{
    //...
    
    return;
}   /* Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

//... INTENDED CONSTRUCTOR HERE

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
    // ref: https://www.homerenergy.com/products/pro/docs/3.11/how_homer_calculates_the_pv_array_power_output.html
    //...
    
    return 0;
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
    //...
    
    return;
}   /* ~Solar() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
