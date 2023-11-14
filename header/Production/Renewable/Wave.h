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
/// \file Wave.h
///
/// \brief Header file the Wave class.
///


#ifndef WAVE_H
#define WAVE_H


#include "Renewable.h"


///
/// \class Wave
///
/// \brief A derived class of the Renewable branch of Production which models wave
///     production.
///

class Wave : public Renewable {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Wave(void);
        
        //...
        
        ~Wave(void);
        
};  /* Wave */


#endif  /* WAVE_H */
