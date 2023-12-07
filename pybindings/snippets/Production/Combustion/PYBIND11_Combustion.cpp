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
/// \file PYBIND11_Combustion.cpp
///
/// \brief Bindings file for the Combustion class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Combustion
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<CombustionType>(m, "CombustionType")
    .value("DIESEL", CombustionType::DIESEL)
    .value("N_COMBUSTION_TYPES", CombustionType::N_COMBUSTION_TYPES);



pybind11::enum_<FuelMode>(m, "FuelMode")
    .value("FUEL_MODE_LINEAR", FuelMode::FUEL_MODE_LINEAR)
    .value("FUEL_MODE_LOOKUP", FuelMode::FUEL_MODE_LOOKUP)
    .value("N_FUEL_MODES", FuelMode::N_FUEL_MODES);


pybind11::class_<CombustionInputs>(m, "CombustionInputs")
    .def_readwrite("production_inputs", &CombustionInputs::production_inputs)
    .def_readwrite("fuel_mode", &CombustionInputs::fuel_mode)
    .def_readwrite(
        "nominal_fuel_escalation_annual",
        &CombustionInputs::nominal_fuel_escalation_annual
    )
    .def_readwrite(
        "path_2_fuel_interp_data",
        &CombustionInputs::path_2_fuel_interp_data
    )
    
    .def(pybind11::init());


pybind11::class_<Emissions>(m, "Emissions")
    .def_readwrite("CO2_kg", &Emissions::CO2_kg)
    .def_readwrite("CO_kg", &Emissions::CO_kg)
    .def_readwrite("NOx_kg", &Emissions::NOx_kg)
    .def_readwrite("SOx_kg", &Emissions::SOx_kg)
    .def_readwrite("CH4_kg", &Emissions::CH4_kg)
    .def_readwrite("PM_kg", &Emissions::PM_kg)
    
    .def(pybind11::init());


pybind11::class_<Combustion>(m, "Combustion")
    .def_readwrite("type", &Combustion::type)
    .def_readwrite("fuel_mode", &Combustion::fuel_mode)
    .def_readwrite("total_emissions", &Combustion::total_emissions)
    .def_readwrite("fuel_cost_L", &Combustion::fuel_cost_L)
    .def_readwrite(
        "nominal_fuel_escalation_annual",
        &Combustion::nominal_fuel_escalation_annual
    )
    .def_readwrite(
        "real_fuel_escalation_annual",
        &Combustion::real_fuel_escalation_annual
    )
    .def_readwrite("linear_fuel_slope_LkWh", &Combustion::linear_fuel_slope_LkWh)
    .def_readwrite(
        "linear_fuel_intercept_LkWh",
        &Combustion::linear_fuel_intercept_LkWh
    )
    .def_readwrite(
        "CO2_emissions_intensity_kgL",
        &Combustion::CO2_emissions_intensity_kgL
    )
    .def_readwrite(
        "CO_emissions_intensity_kgL",
        &Combustion::CO_emissions_intensity_kgL
    )
    .def_readwrite(
        "NOx_emissions_intensity_kgL",
        &Combustion::NOx_emissions_intensity_kgL
    )
    .def_readwrite(
        "SOx_emissions_intensity_kgL",
        &Combustion::SOx_emissions_intensity_kgL
    )
    .def_readwrite(
        "CH4_emissions_intensity_kgL",
        &Combustion::CH4_emissions_intensity_kgL
    )
    .def_readwrite(
        "PM_emissions_intensity_kgL",
        &Combustion::PM_emissions_intensity_kgL
    )
    .def_readwrite("total_fuel_consumed_L", &Combustion::total_fuel_consumed_L)
    .def_readwrite("fuel_mode_str", &Combustion::fuel_mode_str)
    .def_readwrite("fuel_consumption_vec_L", &Combustion::fuel_consumption_vec_L)
    .def_readwrite("fuel_cost_vec", &Combustion::fuel_cost_vec)
    .def_readwrite("CO2_emissions_vec_kg", &Combustion::CO2_emissions_vec_kg)
    .def_readwrite("CO_emissions_vec_kg", &Combustion::CO_emissions_vec_kg)
    .def_readwrite("NOx_emissions_vec_kg", &Combustion::NOx_emissions_vec_kg)
    .def_readwrite("SOx_emissions_vec_kg", &Combustion::SOx_emissions_vec_kg)
    .def_readwrite("CH4_emissions_vec_kg", &Combustion::CH4_emissions_vec_kg)
    .def_readwrite("PM_emissions_vec_kg", &Combustion::PM_emissions_vec_kg)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, CombustionInputs, std::vector<double>*>())
    .def("handleReplacement", &Combustion::handleReplacement)
    .def("computeFuelAndEmissions", &Combustion::computeFuelAndEmissions)
    .def("computeEconomics", &Combustion::computeEconomics)
    .def("requestProductionkW", &Combustion::requestProductionkW)
    .def("commit", &Combustion::commit)
    .def("getFuelConsumptionL", &Combustion::getFuelConsumptionL)
    .def("getEmissionskg", &Combustion::getEmissionskg)
    .def("writeResults", &Combustion::writeResults);
