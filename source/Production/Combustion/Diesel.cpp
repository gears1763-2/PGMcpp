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
        error_str += "DieselInputs::fuel_cost_L must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check CO2_emissions_intensity_kgL
    if (diesel_inputs.CO2_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR: Diesel():\t";
        error_str += "DieselInputs::CO2_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check CO_emissions_intensity_kgL
        if (diesel_inputs.CO_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR: Diesel():\t";
        error_str += "DieselInputs::CO_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check NOx_emissions_intensity_kgL
    if (diesel_inputs.NOx_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR: Diesel():\t";
        error_str += "DieselInputs::NOx_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  5. check SOx_emissions_intensity_kgL
    if (diesel_inputs.SOx_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR: Diesel():\t";
        error_str += "DieselInputs::SOx_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  6. check CH4_emissions_intensity_kgL
    if (diesel_inputs.CH4_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR: Diesel():\t";
        error_str += "DieselInputs::CH4_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  7. check PM_emissions_intensity_kgL
    if (diesel_inputs.PM_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR: Diesel():\t";
        error_str += "DieselInputs::PM_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkInputs() */


double Diesel :: __getGenericFuelSlope()
{
    /*
     *  Helper method (private) to generate a generic, linearized fuel consumption
     *  slope for a diesel generator.
     *
     * ref: https://www.homerenergy.com/products/pro/docs/latest/fuel_curve.html
     * ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_intercept_coefficient.html
     * ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_slope.html
     *
     *  This model was obtained by way of surveying an assortment of published diesel
     *  generator fuel consumption data, and then constructing a best fit model.
     */
    
    double linear_fuel_slope_LkWh = 0.4234 * pow(this->capacity_kW, -0.1012);
    
    return linear_fuel_slope_LkWh;
}   /* __getGenericFuelSlope() */


double Diesel :: __getGenericFuelIntercept()
{
    /*
     *  Helper method (private) to generate a generic, linearized fuel consumption
     *  intercept for a diesel generator.
     *
     * ref: https://www.homerenergy.com/products/pro/docs/latest/fuel_curve.html
     * ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_intercept_coefficient.html
     * ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_slope.html
     *
     *  This model was obtained by way of surveying an assortment of published diesel
     *  generator fuel consumption data, and then constructing a best fit model.
     */
    
    double linear_fuel_intercept_LkWh = 0.0940 * pow(this->capacity_kW, -0.2735);
    
    return linear_fuel_intercept_LkWh;
}   /* __getGenericFuelIntercept() */


double Diesel :: __getGenericCapitalCost(void)
{
    /*
     *  Helper method (private) to generate a generic diesel generator capital cost.
     *
     *  This model was obtained by way of surveying an assortment of published diesel 
     *  generator costs, and then constructing a best fit model. Note that this model
     *  expresses cost in terms of Canadian dollars [CAD].
     */
    
    double capital_cost = 4831 * pow(this->capacity_kW, 0.5737451);
    
    return capital_cost;
}   /* __getGenericCapitalCost() */


double Diesel :: __getGenericOpMaintCost(void)
{
    /*
     *  Helper method (private) to generate a generic diesel generator operation and
     *  maintenance cost. This is a cost incurred per unit energy produced.
     *
     *  This model was obtained by way of surveying an assortment of published diesel 
     *  generator costs, and then constructing a best fit model. Note that this model
     *  expresses cost in terms of Canadian dollars per kiloWatt-hour production
     *  [CAD/kWh].
     */
    
    double operation_maintenance_cost_kWh =
        0.03 * exp(0.0002 * log(0.03333333) * this->capacity_kW) + 0.01;
    
    return operation_maintenance_cost_kWh;
}   /* __getGenericOpMaintCost() */

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
    
    if (diesel_inputs.linear_fuel_slope_LkWh < 0) {
        this->linear_fuel_slope_LkWh = this->__getGenericFuelSlope();
        
    }
    
    if (diesel_inputs.linear_fuel_intercept_LkWh < 0) {
        this->linear_fuel_intercept_LkWh = this->__getGenericFuelIntercept();
    }
    
    if (diesel_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    
    if (diesel_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    
    if (this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
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
