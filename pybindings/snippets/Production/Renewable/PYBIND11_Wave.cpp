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
/// \file PYBIND11_Wave.cpp
///
/// \brief Bindings file for the Wave class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Wave
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<WavePowerProductionModel>(m, "WavePowerProductionModel")
    .value("WAVE_POWER_GAUSSIAN", WavePowerProductionModel::WAVE_POWER_GAUSSIAN)
    .value("WAVE_POWER_PARABOLOID", WavePowerProductionModel::WAVE_POWER_PARABOLOID)
    .value("WAVE_POWER_LOOKUP", WavePowerProductionModel::WAVE_POWER_LOOKUP)
    .value(
        "N_WAVE_POWER_PRODUCTION_MODELS",
        WavePowerProductionModel::N_WAVE_POWER_PRODUCTION_MODELS
    );


pybind11::class_<WaveInputs>(m, "WaveInputs")
    .def_readwrite("renewable_inputs", &WaveInputs::renewable_inputs)
    .def_readwrite("resource_key", &WaveInputs::resource_key)
    .def_readwrite("capital_cost", &WaveInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &WaveInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite(
        "design_significant_wave_height_m",
        &WaveInputs::design_significant_wave_height_m
    )
    .def_readwrite("design_energy_period_s", &WaveInputs::design_energy_period_s)
    .def_readwrite("power_model", &WaveInputs::power_model)
    .def_readwrite(
        "path_2_normalized_performance_matrix",
        &WaveInputs::path_2_normalized_performance_matrix
    )
    
    .def(pybind11::init());


pybind11::class_<Wave>(m, "Wave")
    .def_readwrite(
        "design_significant_wave_height_m",
        &Wave::design_significant_wave_height_m
    )
    .def_readwrite("design_energy_period_s", &Wave::design_energy_period_s)
    .def_readwrite("power_model", &Wave::power_model)
    .def_readwrite("power_model_string", &Wave::power_model_string)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, WaveInputs, std::vector<double>*>())
    .def("handleReplacement", &Wave::handleReplacement)
    .def("computeProductionkW", &Wave::computeProductionkW)
    .def("commit", &Wave::commit);
