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
/// \class Production
///
/// \brief The base class of the Production hierarchy. This hierarchy contains derived
///     classes which model the production of energy, be it renewable or otherwise.
///

class Production {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Production(void);
        
        //...
        
        virtual ~Production(void);
        
};  /* Production */


#endif  /* PRODUCTION_H */
