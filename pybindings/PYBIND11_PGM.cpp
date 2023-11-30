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
/// \file PYBIND11_PGM.cpp
///
/// \brief Bindings file for PGMcpp.
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for PGMcpp. Only public
/// attributes/methods are bound!
///


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../header/Model.h"


PYBIND11_MODULE(PGMcpp, m) {

    #include "snippets/PYBIND11_Controller.cpp"
    #include "snippets/PYBIND11_ElectricalLoad.cpp"
    #include "snippets/PYBIND11_Interpolator.cpp"
    #include "snippets/PYBIND11_Model.cpp"
    #include "snippets/PYBIND11_Resources.cpp"
    
    #include "snippets/Production/PYBIND11_Production.cpp"
    
    #include "snippets/Production/Noncombustion/PYBIND11_Noncombustion.cpp"
    #include "snippets/Production/Noncombustion/PYBIND11_Hydro.cpp"
    
    #include "snippets/Production/Combustion/PYBIND11_Combustion.cpp"
    #include "snippets/Production/Combustion/PYBIND11_Diesel.cpp"
    
    #include "snippets/Production/Renewable/PYBIND11_Renewable.cpp"
    #include "snippets/Production/Renewable/PYBIND11_Solar.cpp"
    #include "snippets/Production/Renewable/PYBIND11_Tidal.cpp"
    #include "snippets/Production/Renewable/PYBIND11_Wave.cpp"
    #include "snippets/Production/Renewable/PYBIND11_Wind.cpp"
    
    #include "snippets/Storage/PYBIND11_Storage.cpp"
    #include "snippets/Storage/PYBIND11_LiIon.cpp"

}   /* PYBIND11_MODULE() */
