/*
 *  PGMcpp : PRIMED Grid Interpolatorling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Interpolator.cpp
///
/// \brief Implementation file for the Interpolator class.
///
/// A class which contains interpolation data and functionality. Intended to serve as a
//      component of the Production and Storage hierarchies.
///


#include "../header/Interpolator.h"



// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __checkDataKey1D(int data_key)
///
/// \brief Helper method to check if given data key (1D) is already in use.
///
/// \param data_key The key associated with the given 1D interpolation data.
///

void Interpolator :: __checkDataKey1D(int data_key)
{
    if (this->interp_map_1D.count(data_key) > 0) {
        std::string error_str = "ERROR:  Interpolator::addData1D()  ";
        error_str += "data key (1D) ";
        error_str += std::to_string(data_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkDataKey1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __checkDataKey2D(int data_key)
///
/// \brief Helper method to check if given data key (2D) is already in use.
///
/// \param data_key The key associated with the given 2D interpolation data.
///

void Interpolator :: __checkDataKey2D(int data_key)
{
    if (this->interp_map_2D.count(data_key) > 0) {
        std::string error_str = "ERROR:  Interpolator::addData2D()  ";
        error_str += "data key (2D) ";
        error_str += std::to_string(data_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkDataKey2D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///

void Interpolator :: __readData1D(int data_key, std::string path_2_data)
{
    //  1. get header row
    std::ifstream ifs;
    ifs.open(path_2_data);
    
    std::string header;
    std::getline(ifs, header);
    std::cout << header << std::endl;
    
    ifs.close();
    
    /*
    
    
    
    std::string line;
    while (true) {
        if (ifs.eof()) {
            break;
        }
        
        ifs >> line;
        std::cout << line << std::endl;
    }
    */
    
    return;
}   /* __readData1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///

void Interpolator :: __readData2D(int data_key, std::string path_2_data)
{
    //...
    
    return;
}   /* __readData2D() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Interpolator :: Interpolator(void)
///
/// \brief Constructor for the Interpolator class.
///

Interpolator :: Interpolator(void)
{
    //...
    
    return;
}   /* Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: addData1D(int data_key, std::string path_2_data)
///
/// \brief Method to add 1D interpolation data to the Interpolator.
///
/// \param data_key A key used to index into the Interpolator.
///
/// \param path_2_data A path (either relative or absolute) to the given 1D
///     interpolation data.
///

void Interpolator :: addData1D(int data_key, std::string path_2_data)
{
    //  1. check key
    this->__checkDataKey1D(data_key);
    
    //  2. read data into map
    this->__readData1D(data_key, path_2_data);
    
    //  3. record path
    this->path_map_1D.insert(std::pair<int, std::string>(data_key, path_2_data));
    
    return;
}   /* addData1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: addData2D(int data_key, std::string path_2_data)
///
/// \brief Method to add 2D interpolation data to the Interpolator.
///
/// \param data_key A key used to index into the Interpolator.
///
/// \param path_2_data A path (either relative or absolute) to the given 2D
///     interpolation data.
///

void Interpolator :: addData2D(int data_key, std::string path_2_data)
{
    //  1. check key
    this->__checkDataKey2D(data_key);
    
    //  2. read data into map
    this->__readData2D(data_key, path_2_data);
    
    //  3. record path
    this->path_map_2D.insert(std::pair<int, std::string>(data_key, path_2_data));
    
    return;
}   /* addData2D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Interpolator :: ~Interpolator(void)
///
/// \brief Destructor for the Interpolator class.
///

Interpolator :: ~Interpolator(void)
{
    //...
    
    return;
}   /* ~Interpolator() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
