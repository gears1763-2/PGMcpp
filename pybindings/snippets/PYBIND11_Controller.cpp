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
/// \file PYBIND11_Controller.cpp
///
/// \brief Bindings file for the Controller class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Controller
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<ControlMode>(m, "ControlMode")
    .value("LOAD_FOLLOWING", ControlMode::LOAD_FOLLOWING)
    .value("CYCLE_CHARGING", ControlMode::CYCLE_CHARGING)
    .value("N_CONTROL_MODES", ControlMode::N_CONTROL_MODES);


pybind11::class_<Controller>(m, "Controller")
    .def_readwrite("control_mode", &Controller::control_mode)
    .def_readwrite("control_string", &Controller::control_string)
    .def_readwrite("net_load_vec_kW", &Controller::net_load_vec_kW)
    .def_readwrite("missed_load_vec_kW", &Controller::missed_load_vec_kW)
    .def_readwrite("combustion_map", &Controller::combustion_map)
    
    .def(pybind11::init<>())
    .def("setControlMode", &Controller::setControlMode)
    .def("init", &Controller::init)
    .def("applyDispatchControl", &Controller::applyDispatchControl)
    .def("clear", &Controller::clear);
