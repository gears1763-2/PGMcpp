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
