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
/// \file Combustion.h
///
/// \brief Header file the Combustion class.
///


#ifndef COMBUSTION_H
#define COMBUSTION_H


#include "../Production.h"


///
/// \class Combustion
///
/// \brief The root of the Combustion branch of the Production hierarchy. This branch 
///     contains derived classes which model the production of energy by way of
///     combustibles.
///

class Combustion : public Production {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Combustion(void);
        
        //...
        
        virtual ~Combustion(void);
        
};  /* Combustion */


#endif  /* COMBUSTION_H */
