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
/// \file Combustion.h
///
/// \brief Header file the Combustion class.
///


#ifndef COMBUSTION_H
#define COMBUSTION_H


#include "../Production.h"


///
/// \enum CombustionType
///
/// \brief An enumeration of the types of Combustion asset supported by PGMcpp
///

enum CombustionType {
    DIESEL, ///< A diesel generator.
    N_COMBUSTION_TYPES ///< A simple hack to get the number of elements in CombustionType
};


///
/// \struct CombustionInputs
///
/// \brief A structure which bundles the necessary inputs for the Combustion constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates ProductionInputs.
///

struct CombustionInputs {
    ProductionInputs production_inputs; ///< An encapsulated ProductionInputs instance.
};


///
/// \struct Emissions
///
/// \brief A structure which bundles the emitted masses of various emissions chemistries.
///

struct Emissions {
    double CO2_kg = 0; ///< The mass of carbon dioxide (CO2) emitted [kg].
    double CO_kg = 0; ///< The mass of carbon monoxide (CO) emitted [kg].
    double NOx_kg = 0; ///< The mass of nitrogen oxides (NOx) emitted [kg].
    double SOx_kg = 0; ///< The mass of sulfur oxides (SOx) emitted [kg].
    double CH4_kg = 0; ///< The mass of methane (CH4) emitted [kg].
    double PM_kg = 0; ///< The mass of particulate matter (PM) emitted [kg].
};


///
/// \class Combustion
///
/// \brief The root of the Combustion branch of the Production hierarchy. This branch 
///     contains derived classes which model the production of energy by way of
///     combustibles.
///

class Combustion : public Production {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(CombustionInputs);
        
        
    public:
        //  1. attributes
        CombustionType type; ///< The type (CombustionType) of the asset.
        
        double fuel_cost_L; ///< The cost of fuel [1/L] (undefined currency).
        
        double linear_fuel_slope_LkWh; ///< The slope [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced.
        double linear_fuel_intercept_LkWh; ///< The intercept [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced.
        
        double CO2_emissions_intensity_kgL; ///< Carbon dioxide (CO2) emissions intensity [kg/L].
        double CO_emissions_intensity_kgL; ///< Carbon monoxide (CO) emissions intensity [kg/L].
        double NOx_emissions_intensity_kgL; ///< Nitrogen oxide (NOx) emissions intensity [kg/L].
        double SOx_emissions_intensity_kgL; ///< Sulfur oxide (SOx) emissions intensity [kg/L].
        double CH4_emissions_intensity_kgL; ///< Methane (CH4) emissions intensity [kg/L].
        double PM_emissions_intensity_kgL; ///< Particulate Matter (PM) emissions intensity [kg/L].
        
        double total_fuel_consumed_L; ///< The total fuel consumed [L] over a model run.
        Emissions total_emissions; ///< An Emissions structure for holding total emissions [kg].
    
        std::vector<double> fuel_consumption_vec_L; ///< A vector of fuel consumed [L] over each modelling time step.
        std::vector<double> fuel_cost_vec; ///< A vector of fuel costs (undefined currency) incurred over each modelling time step. These costs are not discounted (i.e., these are nominal costs).
        
        std::vector<double> CO2_emissions_vec_kg; ///< A vector of carbon dioxide (CO2) emitted [kg] over each modelling time step.
        std::vector<double> CO_emissions_vec_kg; ///< A vector of carbon monoxide (CO) emitted [kg] over each modelling time step.
        std::vector<double> NOx_emissions_vec_kg; ///< A vector of nitrogen oxide (NOx) emitted [kg] over each modelling time step.
        std::vector<double> SOx_emissions_vec_kg; ///< A vector of sulfur oxide (SOx) emitted [kg] over each modelling time step.
        std::vector<double> CH4_emissions_vec_kg; ///< A vector of methane (CH4) emitted [kg] over each modelling time step.
        std::vector<double> PM_emissions_vec_kg; ///< A vector of particulate matter (PM) emitted [kg] over each modelling time step.
        
        
        //  2. methods
        Combustion(void);
        Combustion(int, CombustionInputs);
        
        void computeFuelAndEmissions(void);
        void computeEconomics(std::vector<double>*);
        
        virtual double requestProductionkW(int, double, double) {return 0;}
        virtual double commit(int, double, double, double);
        
        double getFuelConsumptionL(double, double);
        Emissions getEmissionskg(double);
        
        virtual ~Combustion(void);
        
};  /* Combustion */


#endif  /* COMBUSTION_H */
