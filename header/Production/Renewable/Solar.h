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
/// \file Solar.h
///
/// \brief Header file for the Solar class.
///


#ifndef SOLAR_H
#define SOLAR_H


#include "Renewable.h"


///
/// \struct SolarInputs
///
/// \brief A structure which bundles the necessary inputs for the Solar constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates RenewableInputs.
///

struct SolarInputs {
    RenewableInputs renewable_inputs; ///< An encapsulated RenewableInputs instance.
    
    int resource_key = 0; ///< A key used to index into the Resources object, to associate this asset with the appropriate resource time series.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    
    double derating = 0.8; ///< The derating of the solar PV array (i.e., shadowing, soiling, etc.).
};


///
/// \class Solar
///
/// \brief A derived class of the Renewable branch of Production which models solar
///     production.
///

class Solar : public Renewable {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(SolarInputs);
        
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        void __writeSummary(std::string);
        void __writeTimeSeries(
            std::string,
            std::vector<double>*,
            std::map<int, std::vector<double>>*,
            std::map<int, std::vector<std::vector<double>>>*,
            int = -1
        );
        
        
    public:
        //  1. attributes
        double derating; ///< The derating of the solar PV array (i.e., shadowing, soiling, etc.).
        
        
        //  2. methods
        Solar(void);
        Solar(int, double, SolarInputs, std::vector<double>*);
        void handleReplacement(int);
        
        double computeProductionkW(int, double, double);
        double commit(int, double, double, double);
        
        ~Solar(void);
        
};  /* Solar */


#endif  /* SOLAR_H */
