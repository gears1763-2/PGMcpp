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
/// \file ElectricalLoad.h
///
/// \brief Header file for the ElectricalLoad class.
///


#ifndef ELECTRICALLOAD_H
#define ELECTRICALLOAD_H


// std and third-party
#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"


///
/// \class ElectricalLoad
///
/// \brief A class which contains time and electrical load data. Intended to serve as
///     a component class of Model.
///

class ElectricalLoad {
    public:
        //  1. attributes
        int n_points; ///< The number of points in the modelling time series.
        
        double n_years; ///< The number of years being modelled (inferred from time_vec_hrs).
        
        double min_load_kW; ///< The minimum [kW] of the given electrical load time series.
        double mean_load_kW; ///< The mean, or average, [kW] of the given electrical load time series.
        double max_load_kW; ///< The maximum [kW] of the given electrical load time series.
        
        std::string path_2_electrical_load_time_series; ///< A string defining the path (either relative or absolute) to the given electrical load time series.
        
        std::vector<double> time_vec_hrs; ///< A vector to hold a given sequence of model times [hrs]. This defines the modelling time series.
        std::vector<double> dt_vec_hrs; ///< A vector to hold a sequence of model time deltas [hrs].
        
        std::vector<double> load_vec_kW; ///< A vector to hold a given sequence of electrical load values [kW].
        
        
        //  2. methods
        ElectricalLoad(void);
        ElectricalLoad(std::string);
        
        void readLoadData(std::string);
        void clear(void);
        
        ~ElectricalLoad(void);
        
};  /* ElectricalLoad */


#endif  /* ELECTRICALLOAD_H */
