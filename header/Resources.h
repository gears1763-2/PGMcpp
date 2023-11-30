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
/// \brief Header file for the Resources class.
///


#ifndef RESOURCES_H
#define RESOURCES_H


// std and third-party
#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"

// components
#include "ElectricalLoad.h"

// production
#include "Production/Noncombustion/Noncombustion.h"
#include "Production/Renewable/Renewable.h"


///
/// \class Resources
///
/// \brief A class which contains renewable resource data. Intended to serve as a
///     component class of Model.
///

class Resources {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkResourceKey1D(int, RenewableType);
        void __checkResourceKey2D(int, RenewableType);
        
        void __checkResourceKey1D(int, NoncombustionType);
        
        void __checkTimePoint(double, double, std::string, ElectricalLoad*);
        void __throwLengthError(std::string, ElectricalLoad*);
        
        void __readHydroResource(std::string, int, ElectricalLoad*);
        
        void __readSolarResource(std::string, int, ElectricalLoad*);
        void __readTidalResource(std::string, int, ElectricalLoad*);
        void __readWaveResource(std::string, int, ElectricalLoad*);
        void __readWindResource(std::string, int, ElectricalLoad*);
        
        
    public:
        //  1. attributes
        std::map<int, std::vector<double>> resource_map_1D; ///< A map <int, vector<double>> of given 1D renewable resource time series.
        std::map<int, std::string> string_map_1D; ///< A map <int, string> of descriptors for the type of the given 1D renewable resource time series.
        std::map<int, std::string> path_map_1D; ///< A map <int, string> of the paths (either relative or absolute) to given 1D renewable resource time series.
        
        std::map<int, std::vector<std::vector<double>>> resource_map_2D; ///< A map <int, vector<vector<double>>> of given 2D renewable resource time series.
        std::map<int, std::string> string_map_2D; ///< A map <int, string> of descriptors for the type of the given 2D renewable resource time series.
        std::map<int, std::string> path_map_2D; ///< A map <int, string> of the paths (either relative or absolute) to given 2D renewable resource time series.
        
        
        //  2. methods
        Resources(void);
        
        void addResource(NoncombustionType, std::string, int, ElectricalLoad*);
        void addResource(RenewableType, std::string, int, ElectricalLoad*);
        
        void clear(void);
        
        ~Resources(void);
        
};  /* Resources */


#endif  /* RESOURCES_H */
