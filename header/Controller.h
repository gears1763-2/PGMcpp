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
/// \file Controller.h
///
/// \brief Header file for the Controller class.
///


#ifndef CONTROLLER_H
#define CONTROLLER_H

// std and third-party
#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"

//  components
#include "ElectricalLoad.h"
#include "Resources.h"

// production and storage
#include "Production/Combustion/Combustion.h"
#include "Production/Noncombustion/Noncombustion.h"
#include "Production/Renewable/Renewable.h"
#include "Storage/Storage.h"


///
/// \enum ControlMode
///
/// \brief An enumeration of the types of control modes supported by PGMcpp
///

enum ControlMode {
    LOAD_FOLLOWING, ///< Load following control, with in-order dispatch of non-Combustion assets and optimal dispatch of Combustion assets.
    CYCLE_CHARGING, ///< Cycle charging control, with in-order dispatch of non-Combustion assets and optimal dispatch of Combustion assets.
    N_CONTROL_MODES ///< A simple hack to get the number of elements in ControlMode
};


///
/// \class Controller
///
/// \brief A class which contains a various dispatch control logic. Intended to serve
///     as a component class of Model.
///

class Controller {
    private:
        //  1. attributes
        //...
        
        
        //  2. methods
        void __computeNetLoad(ElectricalLoad*, std::vector<Renewable*>*, Resources*);
        void __constructCombustionMap(std::vector<Combustion*>*);
        
        void __applyLoadFollowingControl_CHARGING(
            int,
            ElectricalLoad*,
            Resources*,
            std::vector<Combustion*>*,
            std::vector<Noncombustion*>*,
            std::vector<Renewable*>*,
            std::vector<Storage*>*
        );
        
        void __applyLoadFollowingControl_DISCHARGING(
            int,
            ElectricalLoad*,
            Resources*,
            std::vector<Combustion*>*,
            std::vector<Noncombustion*>*,
            std::vector<Renewable*>*,
            std::vector<Storage*>*
        );
        
        void __applyCycleChargingControl_CHARGING(
            int,
            ElectricalLoad*,
            Resources*,
            std::vector<Combustion*>*,
            std::vector<Noncombustion*>*,
            std::vector<Renewable*>*,
            std::vector<Storage*>*
        );
        
        void __applyCycleChargingControl_DISCHARGING(
            int,
            ElectricalLoad*,
            Resources*,
            std::vector<Combustion*>*,
            std::vector<Noncombustion*>*,
            std::vector<Renewable*>*,
            std::vector<Storage*>*
        );
        
        void __handleStorageCharging(
            int,
            double,
            std::list<Storage*>,
            std::vector<Combustion*>*,
            std::vector<Noncombustion*>*,
            std::vector<Renewable*>*
        );
        
        void __handleStorageCharging(
            int,
            double,
            std::vector<Storage*>*,
            std::vector<Combustion*>*,
            std::vector<Noncombustion*>*,
            std::vector<Renewable*>*
        );
        
        double __getRenewableProduction(int, double, Renewable*, Resources*);
        
        double __handleCombustionDispatch(
            int,
            double,
            double,
            std::vector<Combustion*>*,
            bool
        );
        
        double __handleNoncombustionDispatch(
            int,
            double,
            double,
            std::vector<Noncombustion*>*,
            Resources*
        );
        
        double __handleStorageDischarging(int, double, double, std::list<Storage*>);
        
        
    public:
        //  1. attributes
        ControlMode control_mode; ///< The ControlMode that is active in the Model.
        std::string control_string; ///< A string describing the active ControlMode.
        
        std::vector<double> net_load_vec_kW; ///< A vector of net load values [kW] at each point in the modelling time series. Net load is defined as load minus all available Renewable production.
        std::vector<double> missed_load_vec_kW; ///< A vector of missed load values [kW] at each point in the modelling time series.
        
        std::map<double, std::vector<bool>> combustion_map; ///< A map of all possible combustion states, for use in determining optimal dispatch.
        
        
        //  2. methods
        Controller(void);
        
        void setControlMode(ControlMode);
        
        void init(
            ElectricalLoad*,
            std::vector<Renewable*>*,
            Resources*,
            std::vector<Combustion*>*
        );
        
        void applyDispatchControl(
            ElectricalLoad*,
            Resources*,
            std::vector<Combustion*>*,
            std::vector<Noncombustion*>*,
            std::vector<Renewable*>*,
            std::vector<Storage*>*
        );
        
        void clear(void);
        
        ~Controller(void);
        
};  /* Controller */


#endif  /* CONTROLLER_H */
