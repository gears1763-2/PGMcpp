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
/// \file Renewable.h
///
/// \brief Header file the Renewable class.
///


#ifndef RENEWABLE_H
#define RENEWABLE_H


#include "../Production.h"


///
/// \class Renewable
///
/// \brief The root of the Renewable branch of the Production hierarchy. This branch 
///     contains derived classes which model the renewable production of energy
///

class Renewable : public Production {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Renewable(void);
        
        //...
        
        virtual ~Renewable(void);
        
};  /* Renewable */


#endif  /* RENEWABLE_H */
