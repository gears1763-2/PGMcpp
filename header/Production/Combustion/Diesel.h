/*
 *  PGMcpp : PRIMED Grid Modelling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Diesel.h
///
/// \brief Header file the Diesel class.
///


#ifndef DIESEL_H
#define DIESEL_H


#include "Combustion.h"


///
/// \struct DieselInputs
///
/// \brief A structure which bundles the necessary inputs for the Diesel constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates CombustionInputs.
///

struct DieselInputs {
    CombustionInputs combustion_inputs; ///< An encapsulated CombustionInputs instance.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    double fuel_cost_L = 1.70; ///< The cost of fuel [1/L] (undefined currency).
    
    /*
     * ref: https://www.homerenergy.com/products/pro/docs/latest/fuel_curve.html
     * ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_intercept_coefficient.html
     * ref: https://www.homerenergy.com/products/pro/docs/latest/generator_fuel_curve_slope.html
     */
    double linear_fuel_slope_LkWh = -1; ///< The slope [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced. -1 is a sentinel value, which triggers a generic fuel consumption model on construction (in fact, any negative value here will trigger).
    double linear_fuel_intercept_LkWh = -1; ///< The intercept [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced. -1 is a sentinel value, which triggers a generic fuel consumption model on construction (in fact, any negative value here will trigger).
    
    /*
     * ref: docs/refs/diesel_emissions_ref_1.pdf
     * ref: docs/refs/diesel_emissions_ref_2.pdf
     */
    double CO2_emissions_intensity_kgL = 2.7; ///< Carbon dioxide (CO2) emissions intensity [kg/L].
    double CO_emissions_intensity_kgL = 0.0178; ///< Carbon monoxide (CO) emissions intensity [kg/L].
    double NOx_emissions_intensity_kgL = 0.0014; ///< Nitrogen oxide (NOx) emissions intensity [kg/L].
    double SOx_emissions_intensity_kgL = 0.0042; ///< Sulfur oxide (SOx) emissions intensity [kg/L].
    double CH4_emissions_intensity_kgL = 0.0007; ///< Methane (CH4) emissions intensity [kg/L].
    double PM_emissions_intensity_kgL = 0.0001; ///< Particulate Matter (PM) emissions intensity [kg/L].
};


///
/// \class Diesel
///
/// \brief A derived class of the Combustion branch of Production which models
///     production using a diesel generator.
///

class Diesel : public Combustion {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(DieselInputs);
        double __getGenericFuelSlope(void);
        double __getGenericFuelIntercept(void);
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        
    public:
        //  1. attributes
        double fuel_cost_L; ///< The cost of fuel [1/L] (undefined currency).
        
        double CO2_emissions_intensity_kgL; ///< Carbon dioxide (CO2) emissions intensity [kg/L].
        double CO_emissions_intensity_kgL; ///< Carbon monoxide (CO) emissions intensity [kg/L].
        double NOx_emissions_intensity_kgL; ///< Nitrogen oxide (NOx) emissions intensity [kg/L].
        double SOx_emissions_intensity_kgL; ///< Sulfur oxide (SOx) emissions intensity [kg/L].
        double CH4_emissions_intensity_kgL; ///< Methane (CH4) emissions intensity [kg/L].
        double PM_emissions_intensity_kgL; ///< Particulate Matter (PM) emissions intensity [kg/L].
        
        
        //  2. methods
        Diesel(void);
        Diesel (int, DieselInputs);
        
        //...
        
        ~Diesel(void);
        
};  /* Diesel */


#endif  /* DIESEL_H */
