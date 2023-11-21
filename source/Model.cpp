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
/// \file Model.cpp
///
/// \brief Implementation file for the Model class.
///
/// A container class which forms the centre of PGMcpp. The Model class is
/// intended to serve as the primary user interface with the functionality of PGMcpp,
/// and as such it contains all other classes.
///


#include "../header/Model.h"


// ======== PRIVATE ================================================================= //

void Model :: __checkInputs(ModelInputs)
{
    /*
     *  Helper method (private) to check inputs to the Model constructor.
     */
    
    //...
    
    return;
}   /* __checkInputs() */

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Model :: Model(void)
///
/// \brief Constructor (dummy) for the Model class.
///

Model :: Model(void)
{
    return;
}   /* Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Model :: Model(ModelInputs model_inputs)
///
/// \brief Constructor (intended) for the Model class.
///
/// \param
///

Model :: Model(ModelInputs model_inputs)
{
    //  1. check inputs
    this->__checkInputs(model_inputs);
    
    //  2. read in electrical load data
    this->electrical_load.readLoadData(model_inputs.path_2_electrical_load_time_series);
    
    //  3. set control mode
    this->controller.control_mode = model_inputs.control_mode;
    
    return;
}   /* Model() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: reset(void)
///
/// \brief Method which resets the model for use in assessing a new candidate microgrid
///     design. This method only clears the asset pointer vectors; it leaves the 
///     Controller, ElectricalLoad, and Resources objects of the Model alone.

void Model :: reset(void)
{
    //  1. clear combustion_ptr_vec
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        delete this->combustion_ptr_vec[i];
    }
    this->combustion_ptr_vec.clear();
    
    //  2. clear renewable_ptr_vec
    for (size_t i = 0; i < this->renewable_ptr_vec.size(); i++) {
        delete this->renewable_ptr_vec[i];
    }
    this->renewable_ptr_vec.clear();
    
    //  3. clear storage_ptr_vec
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        delete this->storage_ptr_vec[i];
    }
    this->storage_ptr_vec.clear();
    
    return;
}   /* reset() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: clear(void)
///
/// \brief Method to clear all attributes of the Model object.
///

void Model :: clear(void)
{
    //  1. reset
    this->reset();
    
    //  2. clear remaining attributes
    controller.clear();
    electrical_load.clear();
    resources.clear();
    
    return;
}   /* clear() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn Model :: ~Model(void)
///
/// \brief Destructor for the Model class.
///

Model :: ~Model(void)
{
    this->clear();
    return;
}   /* ~Model() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
