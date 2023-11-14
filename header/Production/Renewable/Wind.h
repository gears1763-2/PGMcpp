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
/// \file Wind.h
///
/// \brief Header file the Wind class.
///


#ifndef WIND_H
#define WIND_H


#include "Renewable.h"


///
/// \class Wind
///
/// \brief A derived class of the Renewable branch of Production which models wind
///     production.
///

class Wind : public Renewable {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Wind(void);
        
        //...
        
        ~Wind(void);
        
};  /* Wind */


#endif  /* WIND_H */
