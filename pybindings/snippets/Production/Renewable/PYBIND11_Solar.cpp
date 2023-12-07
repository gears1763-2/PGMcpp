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
/// \file PYBIND11_Solar.cpp
///
/// \brief Bindings file for the Solar class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Solar
/// class. Only public attributes/methods are bound!
///


pybind11::class_<SolarInputs>(m, "SolarInputs")
    .def_readwrite("renewable_inputs", &SolarInputs::renewable_inputs)
    .def_readwrite("resource_key", &SolarInputs::resource_key)
    .def_readwrite("capital_cost", &SolarInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &SolarInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite("derating", &SolarInputs::derating)
    
    .def(pybind11::init());


pybind11::class_<Solar>(m, "Solar")
    .def_readwrite("derating", &Solar::derating)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, SolarInputs, std::vector<double>*>())
    .def("handleReplacement", &Solar::handleReplacement)
    .def("computeProductionkW", &Solar::computeProductionkW)
    .def("commit", &Solar::commit);
