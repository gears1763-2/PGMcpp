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

// ---------------------------------------------------------------------------------- //

void Production :: __checkInputs(int n_points, ProductionInputs production_inputs)
{
    /*
     *  Helper method (private) to check inputs to the Production constructor.
     */
    
    //  1. check n_points
    if (n_points <= 0) {
        std::string error_str = "ERROR: Production():\tn_points must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check capacity_kW
    if (production_inputs.capacity_kW <= 0) {
        std::string error_str = "ERROR: Production():\t";
        error_str += "ProductionInputs::capacity_kW must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check replace_running_hrs
    if (production_inputs.replace_running_hrs <= 0) {
        std::string error_str = "ERROR: Production():\t";
        error_str += "ProductionInputs::replace_running_hrs must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

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
}   /* __computeRealDiscountAnnual() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

void Production :: __handleReplacement(int timestep)
{
    /*
     *  Helper method (private) to handle asset replacement and capital cost incursion,
     *  if applicable.
     */
    
    if (
        this->running_hours >= (this->n_replacements + 1) * this->replace_running_hrs
    ) {
        //  1. log replacement
        this->n_replacements++;
        
        //  2. incur capital cost in timestep
        this->capital_cost_vec[timestep] = this->capital_cost;
    }
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //

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
/// \param production_inputs A structure of Production constructor inputs.
///

Production :: Production(int n_points, ProductionInputs production_inputs)
{
    //  1. check inputs
    this->__checkInputs(n_points, production_inputs);
    
    //  2. set attributes
    this->print_flag = production_inputs.print_flag;
    this->is_running = false;
    
    this->n_points = n_points;
    this->n_starts = 0;
    
    this->running_hours = 0;
    this->replace_running_hrs = production_inputs.replace_running_hrs;
    
    this->capacity_kW = production_inputs.capacity_kW;
    
    this->real_discount_annual = this->__computeRealDiscountAnnual(
        production_inputs.nominal_inflation_annual,
        production_inputs.nominal_discount_annual
    );
    this->capital_cost = 0;
    this->operation_maintenance_cost_kWh = 0;
    this->net_present_cost = 0;
    this->levellized_cost_of_energy_kWh = 0;
    
    this->is_running_vec.resize(this->n_points, 0);
    
    this->production_vec_kW.resize(this->n_points, 0);
    this->dispatch_vec_kW.resize(this->n_points, 0);
    this->storage_vec_kW.resize(this->n_points, 0);
    this->curtailment_vec_kW.resize(this->n_points, 0);
    
    this->capital_cost_vec.resize(this->n_points, 0);
    this->operation_maintenance_cost_vec.resize(this->n_points, 0);
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Production object constructed at " << this << std::endl;
    }
    
    return;
}   /* Production() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Production :: commit(
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

double Production :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. record production
    this->production_vec_kW[timestep] = production_kW;
    
    //  2. compute and record dispatch and curtailment
    double dispatch_kW = 0;
    double curtailment_kW = 0;
    
    if (production_kW > load_kW) {
        dispatch_kW = load_kW;
        curtailment_kW = production_kW - dispatch_kW;
    }
    
    else {
        dispatch_kW = production_kW;
    }
    
    this->dispatch_vec_kW[timestep] = dispatch_kW;
    this->curtailment_vec_kW[timestep] = curtailment_kW;
    
    //  3. update load
    load_kW -= dispatch_kW;
    
    if (this->is_running) {
        //  4. log running state, running hours
        this->is_running_vec[timestep] = this->is_running;
        this->running_hours += dt_hrs;
        
        //  5. incur operation and maintenance costs
        double produced_kWh = production_kW * dt_hrs;
        double operation_maintenance_cost =
            this->operation_maintenance_cost_kWh * produced_kWh;
        this->operation_maintenance_cost_vec[timestep] = operation_maintenance_cost;
        
        //  6. incur capital costs (i.e., handle replacement)
        this->__handleReplacement(timestep);
    }

    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Production :: ~Production(void)
///
/// \brief Destructor for the Production class.
///

Production :: ~Production(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Production object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Production() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
