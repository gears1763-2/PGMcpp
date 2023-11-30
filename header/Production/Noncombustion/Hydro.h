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
/// \file Hydro.h
///
/// \brief Header file for the Hydro class.
///


#ifndef HYDRO_H
#define HYDRO_H


#include "Noncombustion.h"


///
/// \struct HydroInputs
///
/// \brief A structure which bundles the necessary inputs for the Hydro constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates NoncombustionInputs.
///

struct HydroInputs {
    NoncombustionInputs noncombustion_inputs; ///< An encapsulated NoncombustionInputs instance.
};


///
/// \class Hydro
///
/// \brief A derived class of the Noncombustion branch of Production which models
///     production using a hydroelectric asset (either with reservoir or not).
///

class Hydro : public Noncombustion {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(HydroInputs);
        
        //...
        
        void __writeSummary(std::string);
        void __writeTimeSeries(
            std::string,
            std::vector<double>*,
            int = -1
        );
    
    
    public:
        //  1. attributes
        //...
        
        
        //  2. methods
        Hydro(void);
        Hydro (int, double, HydroInputs);
        void handleReplacement(int);
        
        double requestProductionkW(int, double, double);
        double commit(int, double, double, double);
        
        ~Hydro(void);
        
        
};  /* Hydro */


#endif  /* HYDRO_H */
