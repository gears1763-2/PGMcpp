/*
 *  PGMcpp : PRIMED Grid Productionling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Production.cpp
///
/// \brief Implementation file for the Production class.
///
/// The base class of the Production hierarchy. This hierarchy contains derived
/// classes which model the production of energy, be it renewable or otherwise.
///


#include "../../header/Production/Production.h"


// ======== PRIVATE ================================================================= //

double Production :: __computeRealDiscountAnnual(
    double nominal_inflation_annual,
    double nominal_discount_annual
)
{
    /*
     *  Helper method (private) to compute the real, annual discount rate to be used
     *  in computing model economics. This enables application of the discount factor
     *  approach.
     *
     *  ref: https://www.homerenergy.com/products/pro/docs/3.11/real_discount_rate.html
     *  ref: https://www.homerenergy.com/products/pro/docs/3.11/discount_factor.html
     */
    
    double real_discount_annual = nominal_discount_annual - nominal_inflation_annual;
    real_discount_annual /= 1 + nominal_inflation_annual;
    
    return real_discount_annual;
}

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Production :: Production(void)
///
/// \brief Constructor (dummy) for the Production class.
///

Production :: Production(void)
{
    return;
}   /* Production() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Production :: Production(
///     int n_points,
///     ProductionInputs production_inputs
///     )
///
/// \brief Constructor (intended) for the Production class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param production_inputs A structure of constructor inputs.
///

Production :: Production(int n_points, ProductionInputs production_inputs)
{
    //  1. check inputs
    //...
    
    //  2. set attributes
    this->print_flag = production_inputs.print_flag;
    
    this->n_points = n_points;
    
    this->real_discount_annual = this->__computeRealDiscountAnnual(
        production_inputs.nominal_inflation_annual,
        production_inputs.nominal_discount_annual
    );
    this->net_present_cost = 0;
    this->levellized_cost_of_energy_kWh = 0;
    
    this->production_vec_kW.resize(this->n_points, 0);
    this->dispatch_vec_kW.resize(this->n_points, 0);
    this->storage_vec_kW.resize(this->n_points, 0);
    this->curtailment_vec_kW.resize(this->n_points, 0);
    
    this->capital_cost_vec.resize(this->n_points, 0);
    this->operation_maintenance_cost_vec.resize(this->n_points, 0);
    
    //  3. test print
    if (this->print_flag) {
        std::cout << "Production object constructed at " << this << std::endl;
    }
    
    return;
}   /* Production() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

//...

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Production :: ~Production(void)
///
/// \brief Destructor for the Production class.
///

Production :: ~Production(void)
{
    //  1. test print
    if (this->print_flag) {
        std::cout << "Production object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Production() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
