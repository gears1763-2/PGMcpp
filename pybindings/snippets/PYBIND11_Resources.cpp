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
/// \file PYBIND11_Resources.cpp
///
/// \brief Bindings file for the Resources class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Resources
/// class. Only public attributes/methods are bound!
///


pybind11::class_<Resources>(m, "Resources")
    .def_readwrite("resource_map_1D", &Resources::resource_map_1D)
    .def_readwrite("string_map_1D", &Resources::string_map_1D)
    .def_readwrite("path_map_1D", &Resources::path_map_1D)
    .def_readwrite("resource_map_2D", &Resources::resource_map_2D)
    .def_readwrite("string_map_2D", &Resources::string_map_2D)
    .def_readwrite("path_map_2D", &Resources::path_map_2D)

    .def(pybind11::init<>())
    .def(
        "addResource",
        pybind11::overload_cast<NoncombustionType, std::string, int, ElectricalLoad*>(&Resources::addResource)
    )
    .def(
        "addResource",
        pybind11::overload_cast<RenewableType, std::string, int, ElectricalLoad*>(&Resources::addResource)
    )
    .def("clear", &Resources::clear);
