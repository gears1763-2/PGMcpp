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
/// \file PYBIND11_Noncombustion.cpp
///
/// \brief Bindings file for the Noncombustion class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Noncombustion
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<NoncombustionType>(m, "NoncombustionType")
    .value("HYDRO", NoncombustionType::HYDRO)
    .value("N_NONCOMBUSTION_TYPES", NoncombustionType::N_NONCOMBUSTION_TYPES);


pybind11::class_<NoncombustionInputs>(m, "NoncombustionInputs")
    .def_readwrite("production_inputs", &NoncombustionInputs::production_inputs)
    
    .def(pybind11::init());


pybind11::class_<Noncombustion>(m, "Noncombustion")
    .def_readwrite("type", &Noncombustion::type)
    .def_readwrite("resource_key", &Noncombustion::resource_key)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, NoncombustionInputs, std::vector<double>*>())
    .def("handleReplacement", &Noncombustion::handleReplacement)
    .def("computeEconomics", &Noncombustion::computeEconomics)
    .def(
        "requestProductionkW",
        pybind11::overload_cast<int, double, double>(&Noncombustion::requestProductionkW)
    )
    .def(
        "requestProductionkW",
        pybind11::overload_cast<int, double, double, double>(&Noncombustion::requestProductionkW)
    )
    .def(
        "commit",
        pybind11::overload_cast<int, double, double, double>(&Noncombustion::commit)
    )
    .def(
        "commit",
        pybind11::overload_cast<int, double, double, double, double>(&Noncombustion::commit)
    )
    .def("writeResults", &Noncombustion::writeResults);
