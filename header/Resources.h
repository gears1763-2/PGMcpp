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
#include "ElectricalLoad.h"
#include "Production/Renewable/Renewable.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"


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
        void __checkResourceKey1D(int);
        void __checkResourceKey2D(int);
        
        void __readSolarResource(std::string, int, ElectricalLoad*);
        void __readTidalResource(std::string, int, ElectricalLoad*);
        void __readWaveResource(std::string, int, ElectricalLoad*);
        void __readWindResource(std::string, int, ElectricalLoad*);
        
        
    public:
        //  1. attributes
        std::map<int, std::vector<double>> resource_map_1D;
        std::map<int, std::string> path_map_1D;
        
        std::map<int, std::vector<std::vector<double>>> resource_map_2D;
        std::map<int, std::string> path_map_2D;
        
        
        //  2. methods
        Resources(void);
        
        void addResource(RenewableType, std::string, int, ElectricalLoad*);
        
        void clear(void);
        
        ~Resources(void);
        
};  /* Resources */


#endif  /* RESOURCES_H */
