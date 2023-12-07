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
/// \file PYBIND11_Wind.cpp
///
/// \brief Bindings file for the Wind class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Wind
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<WindPowerProductionModel>(m, "WindPowerProductionModel")
    .value("WIND_POWER_EXPONENTIAL", WindPowerProductionModel::WIND_POWER_EXPONENTIAL)
    .value("WIND_POWER_LOOKUP", WindPowerProductionModel::WIND_POWER_LOOKUP)
    .value(
        "N_WIND_POWER_PRODUCTION_MODELS",
        WindPowerProductionModel::N_WIND_POWER_PRODUCTION_MODELS
    );


pybind11::class_<WindInputs>(m, "WindInputs")
    .def_readwrite("renewable_inputs", &WindInputs::renewable_inputs)
    .def_readwrite("resource_key", &WindInputs::resource_key)
    .def_readwrite("capital_cost", &WindInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &WindInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite("design_speed_ms", &WindInputs::design_speed_ms)
    .def_readwrite("power_model", &WindInputs::power_model)
    
    .def(pybind11::init());


pybind11::class_<Wind>(m, "Wind")
    .def_readwrite("design_speed_ms", &Wind::design_speed_ms)
    .def_readwrite("power_model", &Wind::power_model)
    .def_readwrite("power_model_string", &Wind::power_model_string)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, WindInputs, std::vector<double>*>())
    .def("handleReplacement", &Wind::handleReplacement)
    .def("computeProductionkW", &Wind::computeProductionkW)
    .def("commit", &Wind::commit);
