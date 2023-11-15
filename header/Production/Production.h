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
/// \file Production.h
///
/// \brief Header file the Production class.
///


#ifndef PRODUCTION_H
#define PRODUCTION_H


#include "../std_includes.h"
#include "../../third_party/fast-cpp-csv-parser/csv.h"


///
/// \struct ProductionInputs
///
/// \brief A structure which bundles the necessary inputs for the Production constructor.
///     Provides default values for every necessary input.
///

struct ProductionInputs {
    bool print_flag = false; ///< A flag which indicates whether or not object construct/destruction should be verbose.
    
    double nominal_inflation_annual = 0.02; ///< The nominal, annual inflation rate to use in computing model economics.
    double nominal_discount_annual = 0.04; ///< The nominal, annual discount rate to use in computing model economics.
};


///
/// \class Production
///
/// \brief The base class of the Production hierarchy. This hierarchy contains derived
///     classes which model the production of energy, be it renewable or otherwise.
///

class Production {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(ProductionInputs);
        double __computeRealDiscountAnnual(double, double);
        
        
    public:
        //  1. attributes
        bool print_flag; ///< A flag which indicates whether or not object construct/destruction should be verbose.
        
        int n_points; ///< The number of points in the modelling time series.
        
        double real_discount_annual; ///< The real, annual discount rate used in computing model economics. Is computed from the given nominal inflation and discount rates.
        double net_present_cost; ///< The net present cost of this asset.
        double levellized_cost_of_energy_kWh; ///< The levellized cost of energy [1/kWh] (undefined currency) of this asset. This metric considers only dispatched and stored energy.
        
        std::vector<double> production_vec_kW; ///< A vector of production [kW] at each point in the modelling time series.
        std::vector<double> dispatch_vec_kW; ///< A vector of dispatch [kW] at each point in the modelling time series. Dispatch is the amount of production that is sent to the grid to satisfy load.
        std::vector<double> storage_vec_kW; ///< A vector of storage [kW] at each point in the modelling time series. Storage is the amount of production that is sent to storage.
        std::vector<double> curtailment_vec_kW; ///< A vector of curtailment [kW] at each point in the modelling time series. Curtailment is the amount of production that can be neither dispatched nor stored, and is hence curtailed.
        
        std::vector<double> capital_cost_vec; ///< A vector of capital costs (undefined currency) incurred over each modelling time step.
        std::vector<double> operation_maintenance_cost_vec; ///< A vector of operation and maintenance costs (undefined currency) incurred over each modelling time step.
        
        
        //  2. methods
        Production(void);
        Production(int, ProductionInputs);
        
        //...
        
        virtual ~Production(void);
        
};  /* Production */


#endif  /* PRODUCTION_H */
