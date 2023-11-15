/*
 *  PGMcpp : PRIMED Grid Dieselling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Diesel.cpp
///
/// \brief Implementation file for the Diesel class.
///
/// A derived class of the Combustion branch of Production which models production
/// using a diesel generator.
///


#include "../../../header/Production/Combustion/Diesel.h"


// ======== PRIVATE ================================================================= //

void Diesel :: __checkInputs(DieselInputs diesel_inputs)
{
    /*
     *  Helper method (private) to check inputs to the Diesel constructor.
     */
    
    //  1. check fuel_cost_L
    if (diesel_inputs.fuel_cost_L < 0) {
        std::string error_str = "ERROR: Diesel():\t";
        error_str += "DieselInputs::fuel_cost_L must be >= 0.";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::runtime_error(error_str);
    }
    
    //  2. check CO2_emissions_intensity_kgL
    //...
    
    //  3. check CO_emissions_intensity_kgL
    //...
    
    //  4. check NOx_emissions_intensity_kgL
    //...
    
    //  5. check SOx_emissions_intensity_kgL
    //...
    
    //  6. check CH4_emissions_intensity_kgL
    //...
    
    //  7. check PM_emissions_intensity_kgL
    //...
    
    return;
}   /* __checkInputs() */

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Diesel :: Diesel(void)
///
/// \brief Constructor (dummy) for the Diesel class.
///

Diesel :: Diesel(void)
{
    return;
}   /* Diesel() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Diesel :: Diesel(void)
///
/// \brief Constructor (intended) for the Diesel class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param diesel_inputs A structure of Diesel constructor inputs.
///

Diesel :: Diesel(int n_points, DieselInputs diesel_inputs) :
Combustion(n_points, diesel_inputs.combustion_inputs)
{
    //  1. check inputs
    this->__checkInputs(diesel_inputs);
    
    //  2. set attributes
    this->fuel_cost_L = diesel_inputs.fuel_cost_L;
    
    this->CO2_emissions_intensity_kgL = diesel_inputs.CO2_emissions_intensity_kgL;
    this->CO_emissions_intensity_kgL = diesel_inputs.CO_emissions_intensity_kgL;
    this->NOx_emissions_intensity_kgL = diesel_inputs.NOx_emissions_intensity_kgL;
    this->SOx_emissions_intensity_kgL = diesel_inputs.SOx_emissions_intensity_kgL;
    this->CH4_emissions_intensity_kgL = diesel_inputs.CH4_emissions_intensity_kgL;
    this->PM_emissions_intensity_kgL = diesel_inputs.PM_emissions_intensity_kgL;
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Diesel object constructed at " << this << std::endl;
    }
    
    return;
}   /* Diesel() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

//...

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Diesel :: ~Diesel(void)
///
/// \brief Destructor for the Diesel class.
///

Diesel :: ~Diesel(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Diesel object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Diesel() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
