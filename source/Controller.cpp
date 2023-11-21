/*
 *  PGMcpp : PRIMED Grid Controllerling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Controller.cpp
///
/// \brief Implementation file for the Controller class.
///
/// A class which contains a various dispatch control logic. Intended to serve as a
/// component class of Controller.
///


#include "../header/Controller.h"


// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Controller :: Controller(void)
///
/// \brief Constructor for the Controller class.
///

Controller :: Controller(void)
{
    return;
}   /* Controller() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: clear(void)
///
/// \brief Method to clear all attributes of the Controller object.
///

void Controller :: clear(void)
{
    //...
    
    return;
}   /* clear() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Controller :: ~Controller(void)
///
/// \brief Destructor for the Controller class.
///

Controller :: ~Controller(void)
{
    this->clear();
    
    return;
}   /* ~Controller() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
