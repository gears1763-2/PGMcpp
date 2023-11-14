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
/// \brief Header file the Storage class.


#ifndef STORAGE_H
#define STORAGE_H


#include "../std_includes.h"
#include "../../third_party/fast-cpp-csv-parser/csv.h"


///
/// \class Storage
///
/// \brief The base class of the Storage hierarchy. This hierarchy contains derived
///     classes which model the storage of energy.

class Storage {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Storage(void);
        
        //...
        
        virtual ~Storage(void);
        
};  /* Storage */


#endif  /* STORAGE_H */
