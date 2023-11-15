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

void Combustion :: __checkInputs(CombustionInputs combustion_inputs)
{
    /*
     *  Helper method (private) to check inputs to the Combustion constructor.
     */
    
    // ...
    
    return;
}   /* __checkInputs() */

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
    this->linear_fuel_slope_LkWh = 0;
    this->linear_fuel_intercept_LkWh = 0;
    
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

//...

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
