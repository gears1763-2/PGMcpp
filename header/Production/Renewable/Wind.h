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
/// \file Wind.h
///
/// \brief Header file the Wind class.
///


#ifndef WIND_H
#define WIND_H


#include "Renewable.h"


///
/// \struct WindInputs
///
/// \brief A structure which bundles the necessary inputs for the Wind constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates RenewableInputs.
///

struct WindInputs {
    RenewableInputs renewable_inputs; ///< An encapsulated RenewableInputs instance.
    
    int resource_key = 0; ///< A key used to index into the Resources object, to associate this asset with the appropriate resource time series.
    
    double capital_cost = -1; ///< The capital cost of the asset (undefined currency). -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD].
    double operation_maintenance_cost_kWh = -1; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy produced. -1 is a sentinel value, which triggers a generic cost model on construction (in fact, any negative value here will trigger). Note that the generic cost model is in terms of Canadian dollars [CAD/kWh].
    
    double design_speed_ms = 8; ///< The wind speed [m/s] at which the wind turbine achieves its rated capacity.
};


///
/// \class Wind
///
/// \brief A derived class of the Renewable branch of Production which models wind
///     production.
///

class Wind : public Renewable {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(WindInputs);
        
        double __getGenericCapitalCost(void);
        double __getGenericOpMaintCost(void);
        
        double __computeExponentialProductionkW(int, double, double);
        
        
    public:
        //  1. attributes
        double design_speed_ms; ///< The wind speed [m/s] at which the wind turbine achieves its rated capacity.
        
        
        //  2. methods
        Wind(void);
        Wind(int, WindInputs);
        
        double computeProductionkW(int, double, double);
        double commit(int, double, double, double);
        
        ~Wind(void);
        
};  /* Wind */


#endif  /* WIND_H */
