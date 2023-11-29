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
/// \file PYBIND11_ElectricalLoad.cpp
///
/// \brief Bindings file for the ElectricalLoad class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the ElectricalLoad
/// class. Only public attributes/methods are bound!
///


pybind11::class_<ElectricalLoad>(m, "ElectricalLoad")
    .def_readwrite("n_points", &ElectricalLoad::n_points)
    .def_readwrite("n_years", &ElectricalLoad::n_years)
    .def_readwrite("min_load_kW", &ElectricalLoad::min_load_kW)
    .def_readwrite("mean_load_kW", &ElectricalLoad::mean_load_kW)
    .def_readwrite("max_load_kW", &ElectricalLoad::max_load_kW)
    .def_readwrite("path_2_electrical_load_time_series", &ElectricalLoad::path_2_electrical_load_time_series)
    .def_readwrite("time_vec_hrs", &ElectricalLoad::time_vec_hrs)
    .def_readwrite("dt_vec_hrs", &ElectricalLoad::dt_vec_hrs)
    .def_readwrite("load_vec_kW", &ElectricalLoad::load_vec_kW)
    
    .def(pybind11::init<>())
    .def(pybind11::init<std::string>())
    .def("readLoadData", &ElectricalLoad::readLoadData)
    .def("clear", &ElectricalLoad::clear);
