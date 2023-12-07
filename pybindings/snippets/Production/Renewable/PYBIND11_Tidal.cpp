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
/// \file PYBIND11_Tidal.cpp
///
/// \brief Bindings file for the Tidal class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Tidal
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<TidalPowerProductionModel>(m, "TidalPowerProductionModel")
    .value("TIDAL_POWER_CUBIC", TidalPowerProductionModel::TIDAL_POWER_CUBIC)
    .value(
        "TIDAL_POWER_EXPONENTIAL",
        TidalPowerProductionModel::TIDAL_POWER_EXPONENTIAL
    )
    .value("TIDAL_POWER_LOOKUP", TidalPowerProductionModel::TIDAL_POWER_LOOKUP)
    .value(
        "N_TIDAL_POWER_PRODUCTION_MODELS",
        TidalPowerProductionModel::N_TIDAL_POWER_PRODUCTION_MODELS
    );


pybind11::class_<TidalInputs>(m, "TidalInputs")
    .def_readwrite("renewable_inputs", &TidalInputs::renewable_inputs)
    .def_readwrite("resource_key", &TidalInputs::resource_key)
    .def_readwrite("capital_cost", &TidalInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &TidalInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite("design_speed_ms", &TidalInputs::design_speed_ms)
    .def_readwrite("power_model", &TidalInputs::power_model)
    
    .def(pybind11::init());


pybind11::class_<Tidal>(m, "Tidal")
    .def_readwrite("design_speed_ms", &Tidal::design_speed_ms)
    .def_readwrite("power_model", &Tidal::power_model)
    .def_readwrite("power_model_string", &Tidal::power_model_string)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, TidalInputs, std::vector<double>*>())
    .def("handleReplacement", &Tidal::handleReplacement)
    .def("computeProductionkW", &Tidal::computeProductionkW)
    .def("commit", &Tidal::commit);
