/*
 *  PGMcpp : PRIMED Grid Renewableling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Renewable.cpp
///
/// \brief Implementation file for the Renewable class.
///
/// The root of the Renewable branch of the Production hierarchy. This branch 
/// contains derived classes which model the renewable production of energy.
///


#include "../../../header/Production/Renewable/Renewable.h"


// ======== PRIVATE ================================================================= //

//...

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Renewable :: Renewable(void)
///
/// \brief Constructor (dummy) for the Renewable class.
///
// \param [...]

Renewable :: Renewable(void) :
Production()
{
    //...
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Renewable :: commit(
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

double Renewable :: commit(
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
    
    
    //...
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Renewable :: ~Renewable(void)
///
/// \brief Destructor for the Renewable class.
///

Renewable :: ~Renewable(void)
{
    //...
    
    return;
}   /* ~Renewable() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
