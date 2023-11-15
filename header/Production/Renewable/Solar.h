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
/// \struct SolarInputs
///
/// \brief A structure which bundles the necessary inputs for the Solar constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates RenewableInputs.
///

struct SolarInputs {
    RenewableInputs renewable_inputs; ///< An encapsulated RenewableInputs instance.
};


///
/// \class Solar
///
/// \brief A derived class of the Renewable branch of Production which models solar
///     production.
///

class Solar : public Renewable {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(SolarInputs);
        
        
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Solar(void);
        Solar(int, SolarInputs);
        
        double computeProductionkW(int, double, double);
        double commit(int, double, double, double);
        
        ~Solar(void);
        
};  /* Solar */


#endif  /* SOLAR_H */
