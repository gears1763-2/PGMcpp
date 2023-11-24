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
/// \file LiIon.h
///
/// \brief Header file the LiIon class.
///


#ifndef LIION_H
#define LIION_H


#include "Storage.h"


///
/// \class LiIon
///
/// \brief A derived class of Storage which models energy storage by way of lithium-ion
///     batteries.
///

class LiIon : public Storage {
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        LiIon(void);
        
        //...
        
        ~LiIon(void);
        
};  /* LiIon */


#endif  /* LIION_H */
