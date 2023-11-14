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
/// \file Diesel.h
///
/// \brief Header file the Diesel class.
///


#ifndef DIESEL_H
#define DIESEL_H


#include "Combustion.h"


///
/// \class Diesel
///
/// \brief A derived class of the Combustion branch of Production which models
///     production using a diesel generator.
///

class Diesel : public Combustion {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Diesel(void);
        
        //...
        
        ~Diesel(void);
        
};  /* Diesel */


#endif  /* DIESEL_H */
