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

// ---------------------------------------------------------------------------------- //

void Renewable :: __checkInputs(RenewableInputs renewable_inputs)
{
    /*
     *  Helper method (private) to check inputs to the Renewable constructor.
     */
    
    //...
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

void Renewable :: __handleStartStop(int timestep, double dt_hrs, double production_kW)
{
    /*
     *  Helper method (private) to handle the starting/stopping of the renewable asset.
     */
    
    if (this->is_running) {
        // handle stopping
        if (production_kW <= 0) {
            this->is_running = false;
        }
    }
    
    else {
        // handle starting
        if (production_kW > 0) {
            this->is_running = true;
            this->n_starts++;
        }
    }
    
    return;
}   /* __handleStartStop() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Renewable :: Renewable(void)
///
/// \brief Constructor (dummy) for the Renewable class.
///

Renewable :: Renewable(void)
{
    //...
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Renewable :: Renewable(void)
///
/// \brief Constructor (intended) for the Renewable class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param renewable_inputs A structure of Renewable constructor inputs.
///

Renewable :: Renewable(int n_points, RenewableInputs renewable_inputs) :
Production(n_points, renewable_inputs.production_inputs)
{
    //  1. check inputs
    this->__checkInputs(renewable_inputs);
    
    //  2. set attributes
    //...
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Renewable object constructed at " << this << std::endl;
    }
    
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
    //  1. handle start/stop
    this->__handleStartStop(timestep, dt_hrs, production_kW);
    
    //  2. invoke base class method
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
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Renewable object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Renewable() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
