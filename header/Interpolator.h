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
/// \file Interpolator.h
///
/// \brief Header file for the Interpolator class.
///


#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

// std and third-party
#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"


///
/// \class Interpolator
///
/// \brief A class which contains interpolation data and functionality. Intended to
///     serve as a component of the Production and Storage hierarchies.
///

class Interpolator {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkDataKey1D(int);
        void __checkDataKey2D(int);
        
        void __readData1D(int, std::string);
        void __readData2D(int, std::string);
        
        
    public:
        //  1. attributes
        std::map<int, std::vector<double>> interp_map_1D; ///< A map <int, vector<double>> of given 1D interpolation data.
        std::map<int, std::string> path_map_1D; ///< A map <int, string> of the paths (either relative or absolute) to the given 1D interpolation data.
        
        std::map<int, std::vector<std::vector<double>>> interp_map_2D; ///< A map <int, vector<vector<double>>> of given 2D interpolation data.
        std::map<int, std::string> path_map_2D; ///< A map <int, string> of the paths (either relative or absolute) to the given 2D interpolation data.
        
        
        //  2. methods
        Interpolator(void);
        
        void addData1D(int, std::string);
        void addData2D(int, std::string);
        
        double interp1D(int, double);
        double interp2D(int, double, double);
        
        ~Interpolator(void);
        
};  /* Interpolator */


#endif  /* INTERPOLATOR_H */
