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
/// \brief Header file the ElectricalLoad class.
///


#ifndef ELECTRICALLOAD_H
#define ELECTRICALLOAD_H


#include "std_includes.h"


///
/// \class ElectricalLoad
///
/// \brief A class which contains time and electrical load data. Intended to serve as
///     a component class of Model.
///

class ElectricalLoad {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        ElectricalLoad(void);
        
        //...
        
        ~ElectricalLoad(void);
        
};  /* ElectricalLoad */


#endif  /* ELECTRICALLOAD_H */
