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
/// \brief Header file for the Noncombustion class.
///


#ifndef NONCOMBUSTION_H
#define NONCOMBUSTION_H


#include "../Production.h"


///
/// \enum NoncombustionType
///
/// \brief An enumeration of the types of Noncombustion asset supported by PGMcpp
///

enum NoncombustionType {
    HYDRO, ///< A hydroelectric generator (either with reservoir or not)
    N_NONCOMBUSTION_TYPES ///< A simple hack to get the number of elements in NoncombustionType
};


///
/// \struct NoncombustionInputs
///
/// \brief A structure which bundles the necessary inputs for the Noncombustion constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates ProductionInputs.
///

struct NoncombustionInputs {
    ProductionInputs production_inputs; ///< An encapsulated ProductionInputs instance.
};


///
/// \class Noncombustion
///
/// \brief The root of the Noncombustion branch of the Production hierarchy. This branch 
///     contains derived classes which model controllable production which is not based
///     on combustion.
///

class Noncombustion : public Production {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(NoncombustionInputs);
        void __handleStartStop(int, double, double);
        
        virtual void __writeSummary(std::string) {return;}
        virtual void __writeTimeSeries(
            std::string,
            std::vector<double>*,
            int = -1
        ) {return;}
    
    
    public:
        //  1. attributes
        NoncombustionType type; ///< The type (NoncombustionType) of the asset.
        
        int resource_key; ///< A key used to index into the Resources object, to associate this asset with the appropriate resource time series.
        
        
        //  2. methods
        Noncombustion(void);
        Noncombustion(int, double, NoncombustionInputs, std::vector<double>*);
        virtual void handleReplacement(int);
        
        void computeEconomics(std::vector<double>*);
        
        virtual double requestProductionkW(int, double, double) {return 0;}
        virtual double requestProductionkW(int, double, double, double) {return 0;}
        
        virtual double commit(int, double, double, double);
        virtual double commit(int, double, double, double, double) {return 0;}
        
        void writeResults(
            std::string,
            std::vector<double>*,
            int,
            int = -1
        );
        
        virtual ~Noncombustion(void);
        
};  /* Noncombustion */


#endif  /* NONCOMBUSTION_H */
