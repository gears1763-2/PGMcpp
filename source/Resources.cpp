/*
 *  PGMcpp : PRIMED Grid Resourcesling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Resources.cpp
///
/// \brief Implementation file for the Resources class.
///
/// A class which contains renewable resource data. Intended to serve as a component
/// class of Model.
///


#include "../header/Resources.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

void Resources :: __checkResourceKey1D(int resource_key) {
    /*
     *  Helper method (private) to check if given resource key (1D) is already in use.
     */
    
    if (this->resource_map_1D.count(resource_key) > 0) {
        std::string error_str = "ERROR: Resources::__checkResourceKey1D():\t";
        error_str += "resource key ";
        error_str += std::to_string(resource_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /*  __checkResourceKey1D() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

void Resources :: __checkResourceKey2D(int resource_key) {
    /*
     *  Helper method (private) to check if given resource key (2D) is already in use.
     */
    
    if (this->resource_map_2D.count(resource_key) > 0) {
        std::string error_str = "ERROR: Resources::__checkResourceKey2D():\t";
        error_str += "resource key ";
        error_str += std::to_string(resource_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /*  __checkResourceKey2D() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

void Resources :: __checkTimePoint(double time_received_hrs, double time_expected_hrs)
{
    /*
     *  Helper method (private) to check received time point against expected time
     *  point.
     */
    
    if (time_received_hrs != time_expected_hrs) {
        std::string error_str = "ERROR: Resources::__checkTimePoint():\t";
        error_str += "the given resource time series does not align with the ";
        error_str += "previously given electrical load time series";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::runtime_error(error_str);
    }
    
    return;
}

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

void Resources :: __readSolarResource(
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrcial_load_ptr
)
{
    /*
     *  Helper method (private) to handle reading a solar resource time series into
     *  Resources.
     */
    
    //...
    
    return;
}   /* __readSolarResource() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

void Resources :: __readTidalResource(
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrcial_load_ptr
)
{
    /*
     *  Helper method (private) to handle reading a tidal resource time series into
     *  Resources.
     */
    
    //...
    
    return;
}   /* __readTidalResource() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

void Resources :: __readWaveResource(
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrcial_load_ptr
)
{
    /*
     *  Helper method (private) to handle reading a wave resource time series into
     *  Resources.
     */
    
    //...
    
    return;
}   /* __readWaveResource() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

void Resources :: __readWindResource(
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrcial_load_ptr
)
{
    /*
     *  Helper method (private) to handle reading a wind resource time series into
     *  Resources.
     */
    
    //...
    
    return;
}   /* __readWindResource() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //


// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Resources :: Resources(void)
///
/// \brief Constructor for the Resources class.
///

Resources :: Resources(void)
{
    return;
}   /* Resources() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: addResource(
///         RenewableType renewable_type,
///         std::string path_2_resource_data,
///         int resource_key,
///         ElectricalLoad* electrical_load_ptr
///     )
///
/// \brief A method to add a renewable resource time series to Resources. Checks if
///     given resource key is already in use. The associated helper methods also
///     check against ElectricalLoad to ensure that all added time series align with the
///     electrical load time series (both in terms of length and which points in time
///     are included).
///
/// \param renewable_type The type of renewable resource being added to Resources.
///
/// \param path_2_resource_data A string defining the path (either relative or absolute) to the given resource time series.
///
/// \param resource_key A key used to index into the Resources object, used to associate Renewable assets with the corresponding resource.
///
/// \param electrical_load_ptr A pointer to the Model's ElectricalLoad object.
///

void Resources :: addResource(
    RenewableType renewable_type,
    std::string path_2_resource_data,
    int resource_key,
    ElectricalLoad* electrical_load_ptr
)
{
    switch (renewable_type) {
        case (RenewableType :: SOLAR): {
            this->__checkResourceKey1D(resource_key);
            
            this->__readSolarResource(
                path_2_resource_data,
                resource_key,
                electrical_load_ptr
            );
            
            break;
        }
        
        case (RenewableType :: TIDAL): {
            this->__checkResourceKey1D(resource_key);
            
            this->__readTidalResource(
                path_2_resource_data,
                resource_key,
                electrical_load_ptr
            );
            
            break;
        }
        
        case (RenewableType :: WAVE): {
            this->__checkResourceKey2D(resource_key);
            
            this->__readWaveResource(
                path_2_resource_data,
                resource_key,
                electrical_load_ptr
            );
            
            break;
        }
        
        case (RenewableType :: WIND): {
            this->__checkResourceKey1D(resource_key);
            
            this->__readWindResource(
                path_2_resource_data,
                resource_key,
                electrical_load_ptr
            );
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    return;
}   /* addResource() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn void Resources :: clear(void)
///
/// \brief Method to clear all attributes of the Resources object.
///

void Resources :: clear(void)
{
    this->resource_map_1D.clear();
    this->path_map_1D.clear();
    
    this->resource_map_2D.clear();
    this->path_map_2D.clear();
    
    return;
}   /* clear() */

// ---------------------------------------------------------------------------------- //


// ---------------------------------------------------------------------------------- //

///
/// \fn Resources :: ~Resources(void)
///
/// \brief Destructor for the Resources class.
///

Resources :: ~Resources(void)
{
    this->clear();
    return;
}   /* ~Resources() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
