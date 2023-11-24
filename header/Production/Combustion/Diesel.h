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
/// Ref: \cite HOMER_fuel_curve\n
/// Ref: \cite HOMER_generator_fuel_curve_intercept_coefficient\n
/// Ref: \cite HOMER_generator_fuel_curve_slope\n
/// Ref: docs/refs/diesel_emissions_ref_1.pdf\n
/// Ref: docs/refs/diesel_emissions_ref_2.pdf\n
///

struct DieselInputs {
    CombustionInputs combustion_inputs; ///< An encapsulated CombustionInputs instance.
    
    double replace_running_hrs = 30000; ///< The number of running hours after which the asset must be replaced. Overwrites the ProductionInputs attribute.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    double fuel_cost_L = 1.70; ///< The cost of fuel [1/L] (undefined currency).
    
    double minimum_load_ratio = 0.2; ///< The minimum load ratio of the asset. That is, when the asset is producing, it must produce at least this ratio of its rated capacity.
    double minimum_runtime_hrs = 4; ///< The minimum runtime [hrs] of the asset. This is the minimum time that must elapse between successive starts and stops.
    
    double linear_fuel_slope_LkWh = -1; ///< The slope [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced. -1 is a sentinel value, which triggers a generic fuel consumption model on construction (in fact, any negative value here will trigger).
    double linear_fuel_intercept_LkWh = -1; ///< The intercept [L/kWh] to use in computing linearized fuel consumption. This is fuel consumption per unit energy produced. -1 is a sentinel value, which triggers a generic fuel consumption model on construction (in fact, any negative value here will trigger).
    
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
        void __handleStartStop(int, double, double);
        
        void __writeSummary(std::string);
        void __writeTimeSeries(std::string, int = -1);
        
        double __getGenericFuelSlope(void);
        double __getGenericFuelIntercept(void);
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        
    public:
        //  1. attributes
        double minimum_load_ratio; ///< The minimum load ratio of the asset. That is, when the asset is producing, it must produce at least this ratio of its rated capacity.
        double minimum_runtime_hrs; ///< The minimum runtime [hrs] of the asset. This is the minimum time that must elapse between successive starts and stops.
        double time_since_last_start_hrs; ///< The time that has elapsed [hrs] since the last start of the asset.
        
        
        //  2. methods
        Diesel(void);
        Diesel (int, DieselInputs);
        
        double requestProductionkW(int, double, double);
        double commit(int, double, double, double);
        
        void writeResults(std::string, int, int =- 1);
        
        ~Diesel(void);
        
};  /* Diesel */


#endif  /* DIESEL_H */
