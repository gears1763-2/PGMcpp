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
/// \file test_Controller.cpp
///
/// \brief Testing suite for Controller class.
///
/// A suite of tests for the Controller class.
///


#include "../utils/testing_utils.h"
#include "../../header/Controller.h"


// ---------------------------------------------------------------------------------- //

///
/// \fn Controller* testConstruct_Controller(void)
///
/// \brief A function to construct a Controller object.
///
/// \return A pointer to a test Controller object.
///

Controller* testConstruct_Controller(void)
{
    Controller* test_controller_ptr = new Controller();
    
    return test_controller_ptr;
}   /* testConstruct_Controller() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

int main(int argc, char** argv)
{
    #ifdef _WIN32
        activateVirtualTerminal();
    #endif  /* _WIN32 */
    
    printGold("\tTesting Controller");
    
    srand(time(NULL));


    Controller* test_controller_ptr = testConstruct_Controller();


    try {
        //...
    }


    catch (...) {
        delete test_controller_ptr;
        
        printGold(" ............................... ");
        printRed("FAIL");
        std::cout << std::endl;
        throw;
    }


    delete test_controller_ptr;

    printGold(" ............................... ");
    printGreen("PASS");
    std::cout << std::endl;
    return 0;
}   /* main() */

// ---------------------------------------------------------------------------------- //
