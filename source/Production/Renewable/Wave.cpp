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

void Wave :: __checkInputs(WaveInputs wave_inputs)
{
    /*
     *  Helper method (private) to check inputs to the Wave constructor.
     */
    
    //  1. check design_speed_ms
    //...
    
    return;
}   /* __checkInputs() */


// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Wave :: __getGenericCapitalCost(void)
{
    /*
     *  Helper method (private) to generate a generic wave energy converter capital cost.
     *
     *  ref:  Dr. S.L. MacDougall, Commercial Potential of Marine
     *        Renewables in British Columbia, technical report
     *        submitted to Natural Resources Canada, 
     *        S.L. MacDougall Research & Consulting, 2019
     *
     *  Note that this model expresses cost in terms of Canadian dollars [CAD].
     */
    
    double capital_cost_per_kW = 7000 * pow(this->capacity_kW, -0.15) + 5000;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

double Wave :: __getGenericOpMaintCost(void)
{
    /*
     *  Helper method (private) to generate a generic wave energy converter operation
     *  and maintenance cost. This is a cost incurred per unit energy produced.
     *
     *  ref:  Dr. S.L. MacDougall, Commercial Potential of Marine
     *        Renewables in British Columbia, technical report
     *        submitted to Natural Resources Canada, 
     *        S.L. MacDougall Research & Consulting, 2019
     *
     *  Note that this model expresses cost in terms of Canadian dollars [CAD/kWh].
     */
    
    double operation_maintenance_cost_kWh = 0.05 * pow(this->capacity_kW, -0.2) + 0.05;
    
    return operation_maintenance_cost_kWh;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //


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

//...

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Wave :: ~Wave(void)
///
/// \brief Destructor for the Wave class.
///

Wave :: ~Wave(void)
{
    //...
    
    return;
}   /* ~Wave() */

// ---------------------------------------------------------------------------------- //
