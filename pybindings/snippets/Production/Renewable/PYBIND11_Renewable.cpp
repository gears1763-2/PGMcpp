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
/// \file PYBIND11_Renewable.cpp
///
/// \brief Bindings file for the Renewable class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Renewable
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<RenewableType>(m, "RenewableType")
    .value("SOLAR", RenewableType::SOLAR)
    .value("TIDAL", RenewableType::TIDAL)
    .value("WAVE", RenewableType::WAVE)
    .value("WIND", RenewableType::WIND)
    .value("N_RENEWABLE_TYPES", RenewableType::N_RENEWABLE_TYPES);


pybind11::class_<RenewableInputs>(m, "RenewableInputs")
    .def_readwrite("production_inputs", &RenewableInputs::production_inputs)
    
    .def(pybind11::init());


pybind11::class_<Renewable>(m, "Renewable")
    .def_readwrite("type", &Renewable::type)
    .def_readwrite("resource_key", &Renewable::resource_key)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, RenewableInputs, std::vector<double>*>())
    .def("handleReplacement", &Renewable::handleReplacement)
    .def("computeEconomics", &Renewable::computeEconomics)
    .def(
        "computeProductionkW",
        pybind11::overload_cast<int, double, double>(&Renewable::computeProductionkW)
    )
    .def(
        "computeProductionkW",
        pybind11::overload_cast<int, double, double, double>(&Renewable::computeProductionkW)
    )
    .def("commit", &Renewable::commit)
    .def("writeResults", &Renewable::writeResults);
