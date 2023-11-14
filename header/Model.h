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
/// \class Model
///
/// \brief A container class which forms the centre of PGMcpp. The Model class is
///     intended to serve as the primary user interface with the functionality of
///     PGMcpp, and as such it contains all other classes.
///

class Model {
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
        
        //...
        
        ~Model(void);
        
};  /* Model */


#endif  /* Model_H */
