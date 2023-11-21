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
/// \file Resources.h
///
/// \brief Header file the Resources class.
///


#ifndef RESOURCES_H
#define RESOURCES_H


#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"


///
/// \class Resources
///
/// \brief A class which contains renewable resource data. Intended to serve as a
///     component class of Model.
///

class Resources {
    public:
        //  1. attributes
        std::map<int, std::vector<double>> resource_map_1D;
        std::map<int, std::string> path_map_1D;
        
        std::map<int, std::vector<std::vector<double>>> resource_map_2D;
        std::map<int, std::string> path_map_2D;
        
        
        //  2. methods
        Resources(void);
        
        void clear(void);
        
        ~Resources(void);
        
};  /* Resources */


#endif  /* RESOURCES_H */
