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
/// \file Solar.h
///
/// \brief Header file the Solar class.
///


#ifndef SOLAR_H
#define SOLAR_H


#include "Renewable.h"


///
/// \class Solar
///
/// \brief A derived class of the Renewable branch of Production which models solar
///     production.
///

class Solar : public Renewable {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Solar(void);
        
        //...
        
        ~Solar(void);
        
};  /* Solar */


#endif  /* SOLAR_H */
