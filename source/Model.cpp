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

// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: __checkInputs(ModelInputs)
///
/// \brief Helper method (private) to check inputs to the Model constructor.
///
/// \param model_inputs A structure of Model constructor inputs.
///

void Model :: __checkInputs(ModelInputs)
{
    //...
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //

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
/// \param model_inputs A structure of Model constructor inputs.
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
/// \fn void Model :: addDiesel(DieselInputs diesel_inputs)
///
/// \brief Method to add a Diesel asset to the Model.
///
/// \param diesel_inputs A structure of Diesel constructor inputs.
///

void Model :: addDiesel(DieselInputs diesel_inputs)
{
    Combustion* diesel_ptr = new Diesel(this->electrical_load.n_points, diesel_inputs);
    
    this->combustion_ptr_vec.push_back(diesel_ptr);
    
    return;
}   /* addDiesel() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addResource(
///         RenewableType renewable_type,
///         std::string path_2_resource_data,
///         int resource_key
///     )
///
/// \brief A method to add a renewable resource time series to the Model.
///
/// \param renewable_type The type of renewable resource being added to the Model.
///
/// \param path_2_resource_data A string defining the path (either relative or absolute) to the given resource time series.
///
/// \param resource_key A key used to index into the Resources object, used to associate Renewable assets with the corresponding resource.
///

void Model :: addResource(
    RenewableType renewable_type,
    std::string path_2_resource_data,
    int resource_key
)
{
    resources.addResource(
        renewable_type,
        path_2_resource_data,
        resource_key,
        &(this->electrical_load)
    );
    
    return;
}   /* addResource() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addSolar(SolarInputs solar_inputs)
///
/// \brief Method to add a Solar asset to the Model.
///
/// \param solar_inputs A structure of Solar constructor inputs.
///

void Model :: addSolar(SolarInputs solar_inputs)
{
    Renewable* solar_ptr = new Solar(this->electrical_load.n_points, solar_inputs);
    
    this->renewable_ptr_vec.push_back(solar_ptr);
    
    return;
}   /* addSolar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addTidal(TidalInputs tidal_inputs)
///
/// \brief Method to add a Tidal asset to the Model.
///
/// \param tidal_inputs A structure of Tidal constructor inputs.
///

void Model :: addTidal(TidalInputs tidal_inputs)
{
    Renewable* tidal_ptr = new Tidal(this->electrical_load.n_points, tidal_inputs);
    
    this->renewable_ptr_vec.push_back(tidal_ptr);
    
    return;
}   /* addTidal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addWave(WaveInputs wave_inputs)
///
/// \brief Method to add a Wave asset to the Model.
///
/// \param wave_inputs A structure of Wave constructor inputs.
///

void Model :: addWave(WaveInputs wave_inputs)
{
    Renewable* wave_ptr = new Wave(this->electrical_load.n_points, wave_inputs);
    
    this->renewable_ptr_vec.push_back(wave_ptr);
    
    return;
}   /* addWave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addWind(WindInputs wind_inputs)
///
/// \brief Method to add a Wind asset to the Model.
///
/// \param wind_inputs A structure of Wind constructor inputs.
///

void Model :: addWind(WindInputs wind_inputs)
{
    Renewable* wind_ptr = new Wind(this->electrical_load.n_points, wind_inputs);
    
    this->renewable_ptr_vec.push_back(wind_ptr);
    
    return;
}   /* addWind() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: run(void)
///
/// \brief A method to run the Model.
///

void Model :: run(void)
{
    // 1. init Controller
    this->controller.init(
        &(this->electrical_load),
        &(this->renewable_ptr_vec),
        &(this->resources),
        &(this->combustion_ptr_vec)
    );
    
    //  2. apply dispatch control
    this->controller.applyDispatchControl(
        &(this->electrical_load),
        &(this->combustion_ptr_vec),
        &(this->renewable_ptr_vec),
        &(this->storage_ptr_vec)
    );
    
    return;
}   /* run() */

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
    
    //  2. clear components
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
