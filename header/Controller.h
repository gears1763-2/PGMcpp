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


#include "std_includes.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"


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
    public:
        //  1. attributes
        ControlMode control_mode;
        
        
        //  2. methods
        Controller(void);
        
        void clear(void);
        
        ~Controller(void);
        
};  /* Controller */


#endif  /* CONTROLLER_H */
