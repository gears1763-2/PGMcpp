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
/// \brief Header file the Controller class.
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
        
        double __getRenewableProduction(int, double, Renewable*, Resources*);
        
        
    public:
        //  1. attributes
        ControlMode control_mode;
        
        std::vector<double> net_load_vec_kW;
        
        
        //  2. methods
        Controller(void);
        
        void init(
            ElectricalLoad*,
            std::vector<Renewable*>*,
            Resources*,
            std::vector<Combustion*>*
        );
        
        void clear(void);
        
        ~Controller(void);
        
};  /* Controller */


#endif  /* CONTROLLER_H */
