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
/// \file Model.h
///
/// \brief Header file the Model class.
///


#ifndef MODEL_H
#define MODEL_H


//  Components
#include "Controller.h"
#include "ElectricalLoad.h"
#include "Resources.h"

//  Production
#include "Production/Combustion/Diesel.h"
#include "Production/Renewable/Solar.h"
#include "Production/Renewable/Tidal.h"
#include "Production/Renewable/Wave.h"
#include "Production/Renewable/Wind.h"

// Storage
#include "Storage/LiIon.h"


///
/// \struct ModelInputs
///
/// \brief A structure which bundles the necessary inputs for the Model constructor.
///     Provides default values for every necessary input (except
///     path_2_electrical_load_time_series, for which a valid input must be provided).
///

struct ModelInputs {
    std::string path_2_electrical_load_time_series = ""; ///< A string defining the path (either relative or absolute) to the given electrical load time series.
    
    ControlMode control_mode = ControlMode :: LOAD_FOLLOWING; ///< The control mode to be applied by the Controller object.
};


///
/// \class Model
///
/// \brief A container class which forms the centre of PGMcpp. The Model class is
///     intended to serve as the primary user interface with the functionality of
///     PGMcpp, and as such it contains all other classes.
///

class Model {
    private:
        //  1. attributes
        void __checkInputs(ModelInputs);
        
        
        //  2. methods
        //...
        
        
    public:
        //  1. attributes
        Controller controller;  ///< Controller component of Model
        ElectricalLoad electrical_load; ///< ElectricalLoad component of Model
        Resources resources;    ///< Resources component of Model
        
        std::vector<Combustion*> combustion_ptr_vec;    ///< A vector of pointers to the various Combustion assets in the Model
        std::vector<Renewable*> renewable_ptr_vec;  ///< A vector of pointers to the various Renewable assets in the Model
        std::vector<Storage*> storage_ptr_vec;  ///< A vector of pointers to the various Storage assets in the Model
        
        
        //  2. methods
        Model(void);
        Model(ModelInputs);
        
        void addResource(RenewableType, std::string, int);
        
        void reset(void);
        void clear(void);
        
        ~Model(void);
        
};  /* Model */


#endif  /* Model_H */
