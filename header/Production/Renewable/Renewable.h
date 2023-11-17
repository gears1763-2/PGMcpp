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
/// \enum RenewableType
///
/// \brief An enumeration of the types of Renewable asset supported by PGMcpp
///

enum RenewableType {
    SOLAR, ///< A solar photovoltaic (PV) array
    TIDAL, ///< A tidal stream turbine (or tidal energy converter, TEC)
    WAVE, ///< A wave energy converter (WEC)
    WIND, ///< A wind turbine
    N_RENEWABLE_TYPES ///< A simple hack to get the number of elements in RenewableType
};


///
/// \struct RenewableInputs
///
/// \brief A structure which bundles the necessary inputs for the Renewable constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates ProductionInputs.
///

struct RenewableInputs {
    ProductionInputs production_inputs; ///< An encapsulated ProductionInputs instance.
};


///
/// \class Renewable
///
/// \brief The root of the Renewable branch of the Production hierarchy. This branch 
///     contains derived classes which model the renewable production of energy
///

class Renewable : public Production {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(RenewableInputs);
        void __handleStartStop(int, double, double);
        
        
    public:
        //  1. attributes
        RenewableType type; ///< The type (RenewableType) of the asset.
        
        int resource_key; ///< A key used to index into the Resources object, to associate this asset with the appropriate resource time series.
        
        
        //  2. methods
        Renewable(void);
        Renewable(int, RenewableInputs);
        
        virtual double computeProductionkW(int, double, double) {return 0;}
        virtual double computeProductionkW(int, double, double, double) {return 0;}
        virtual double commit(int, double, double, double);
        
        virtual ~Renewable(void);
        
};  /* Renewable */


#endif  /* RENEWABLE_H */
