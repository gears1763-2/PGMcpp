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
/// \file Storage.h
///
/// \brief Header file for the Storage class.
///


#ifndef STORAGE_H
#define STORAGE_H


#include "../std_includes.h"
#include "../../third_party/fast-cpp-csv-parser/csv.h"

#include "../Interpolator.h"


///
/// \enum StorageType
///
/// \brief An enumeration of the types of Storage asset supported by PGMcpp
///

enum StorageType {
    LIION, ///< A system of lithium ion batteries.
    N_STORAGE_TYPES ///< A simple hack to get the number of elements in StorageType
};


///
/// \struct StorageInputs
///
/// \brief A structure which bundles the necessary inputs for the Storage constructor.
///     Provides default values for every necessary input.
///

struct StorageInputs {
    bool print_flag = false; ///< A flag which indicates whether or not object construct/destruction should be verbose.
    bool is_sunk = false; ///< A boolean which indicates whether or not the asset should be considered a sunk cost (i.e., capital cost incurred at the start of the model, or no).
    
    double power_capacity_kW = 100; ///< The rated power capacity [kW] of the asset.
    double energy_capacity_kWh = 1000; ///< The rated energy capacity [kWh] of the asset.
    
    double nominal_inflation_annual = 0.02; ///< The nominal, annual inflation rate to use in computing model economics.
    double nominal_discount_annual = 0.04; ///< The nominal, annual discount rate to use in computing model economics.
};


///
/// \class Storage
///
/// \brief The base class of the Storage hierarchy. This hierarchy contains derived
///     classes which model the storage of energy.
///

class Storage {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(int, double, StorageInputs);
        
        double __computeRealDiscountAnnual(double, double);
        
        virtual void __writeSummary(std::string) {return;}
        virtual void __writeTimeSeries(std::string, std::vector<double>*, int = -1) {return;}
        
        
    public:
        //  1. attributes
        StorageType type; ///< The type (StorageType) of the asset.
        
        Interpolator interpolator; ///< Interpolator component of Storage.
        
        bool print_flag; ///< A flag which indicates whether or not object construct/destruction should be verbose.
        bool is_depleted; ///< A boolean which indicates whether or not the asset is currently considered depleted.
        bool is_sunk; ///< A boolean which indicates whether or not the asset should be considered a sunk cost (i.e., capital cost incurred at the start of the model, or no).
        
        int n_points; ///< The number of points in the modelling time series.
        int n_replacements; ///< The number of times the asset has been replaced.
        
        double n_years; ///< The number of years being modelled.
        
        double power_capacity_kW; ///< The rated power capacity [kW] of the asset.
        double energy_capacity_kWh; ///< The rated energy capacity [kWh] of the asset.
        
        double charge_kWh; ///< The energy [kWh] stored in the asset.
        double power_kW; ///< The power [kW] currently being charged/discharged by the asset.
        
        double nominal_inflation_annual; ///< The nominal, annual inflation rate to use in computing model economics.
        double nominal_discount_annual; ///< The nominal, annual discount rate to use in computing model economics.
        double real_discount_annual; ///< The real, annual discount rate used in computing model economics. Is computed from the given nominal inflation and discount rates.
        double capital_cost; ///< The capital cost of the asset (undefined currency).
        double operation_maintenance_cost_kWh; ///< The operation and maintenance cost of the asset [1/kWh] (undefined currency). This is a cost incurred per unit of energy charged/discharged.
        
        double net_present_cost; ///< The net present cost of this asset.
        double total_discharge_kWh; ///< The total energy discharged [kWh] over the Model run.
        double levellized_cost_of_energy_kWh; ///< The levellized cost of energy [1/kWh] (undefined currency) of this asset. This metric considers only discharge.
        
        std::string type_str; ///< A string describing the type of the asset.
        
        std::vector<double> charge_vec_kWh; ///< A vector of the charge state [kWh] at each point in the modelling time series.
        std::vector<double> charging_power_vec_kW; ///< A vector of the charging power [kW] at each point in the modelling time series.
        std::vector<double> discharging_power_vec_kW; ///< A vector of the discharging power [kW] at each point in the modelling time series.
        
        std::vector<double> capital_cost_vec; ///< A vector of capital costs (undefined currency) incurred over each modelling time step. These costs are not discounted (i.e., these are actual costs).
        std::vector<double> operation_maintenance_cost_vec; ///< A vector of operation and maintenance costs (undefined currency) incurred over each modelling time step. These costs are not discounted (i.e., these are actual costs).
        
        
        //  2. methods
        Storage(void);
        Storage(int, double, StorageInputs);
        virtual void handleReplacement(int);
        
        void computeEconomics(std::vector<double>*);
        
        virtual double getAvailablekW(double) {return 0;}
        virtual double getAcceptablekW(double) {return 0;}
        
        virtual void commitCharge(int, double, double) {return;}
        virtual double commitDischarge(int, double, double, double) {return 0;}
        
        void writeResults(std::string, std::vector<double>*, int, int = -1);
        
        virtual ~Storage(void);
        
};  /* Storage */


#endif  /* STORAGE_H */
