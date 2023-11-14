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
/// \file Tidal.h
///
/// \brief Header file the Tidal class.
///


#ifndef TIDAL_H
#define TIDAL_H


#include "Renewable.h"


///
/// \class Tidal
///
/// \brief A derived class of the Renewable branch of Production which models tidal
///     production.
///

class Tidal : public Renewable {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Tidal(void);
        
        //...
        
        ~Tidal(void);
        
};  /* Tidal */


#endif  /* TIDAL_H */
