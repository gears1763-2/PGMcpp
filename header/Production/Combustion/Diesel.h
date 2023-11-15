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
    
    double fuel_cost_L = 1.70; ///< The cost of fuel [1/L] (undefined currency).
    
    // ref: docs/refs/diesel_emissions_ref_1.pdf
    // ref: docs/refs/diesel_emissions_ref_2.pdf
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
