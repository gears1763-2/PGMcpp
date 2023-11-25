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
/// \struct LiIonInputs
///
/// \brief A structure which bundles the necessary inputs for the LiIon constructor.
///     Provides default values for every necessary input. Note that this structure
///     encapsulates StorageInputs.
///

struct LiIonInputs {
    StorageInputs storage_inputs; ///< An encapsulated StorageInputs instance.
    
    double init_SOC = 0.5; ///< The initial state of charge of the asset.
    
    double min_SOC = 0.15; ///< The minimum state of charge of the asset. Will toggle is_depleted when reached.
    double hysteresis_SOC = 0.5; ///< The state of charge the asset must achieve to toggle is_depleted.
    double max_SOC = 0.9; ///< The maximum state of charge of the asset.
    
    double charging_efficiency = 0.9; ///< The charging efficiency of the asset.
    double discharging_efficiency = 0.9; ///< The discharging efficiency of the asset.
    
    double replace_SOH = 0.8; ///< The state of health at which the asset is considered "dead" and must be replaced.
};


///
/// \class LiIon
///
/// \brief A derived class of Storage which models energy storage by way of lithium-ion
///     batteries.
///

class LiIon : public Storage {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __checkInputs(LiIonInputs);
        
        
    public:
        //  1. attributes
        bool is_depleted; ///< A boolean which indicates whether or not the asset is currently considered depleted.
        
        double dynamic_capacity_kWh; ///< The dynamic (i.e. degrading) energy capacity [kWh] of the asset.
        double SOH; ///< The state of health of the asset.
        double replace_SOH; ///< The state of health at which the asset is considered "dead" and must be replaced.
        
        double init_SOC; ///< The initial state of charge of the asset.
    
        double min_SOC; ///< The minimum state of charge of the asset. Will toggle is_depleted when reached.
        double hysteresis_SOC; ///< The state of charge the asset must achieve to toggle is_depleted.
        double max_SOC; ///< The maximum state of charge of the asset.
        
        double charging_efficiency; ///< The charging efficiency of the asset.
        double discharging_efficiency; ///< The discharging efficiency of the asset.
        
        //  2. methods
        LiIon(void);
        LiIon(int, double, LiIonInputs);
        void handleReplacement(int);
        
        double getAvailablekW(int);
        double getAcceptablekW(int);
        
        void commitCharge(int, double, double);
        double commitDischarge(int, double, double, double);
        
        ~LiIon(void);
        
};  /* LiIon */


#endif  /* LIION_H */
