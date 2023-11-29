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
/// \file PYBIND11_Storage.cpp
///
/// \brief Bindings file for the Storage class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Storage
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<StorageType>(m, "StorageType")
    .value("LIION", StorageType::LIION)
    .value("N_STORAGE_TYPES", StorageType::N_STORAGE_TYPES);


pybind11::class_<StorageInputs>(m, "StorageInputs")
    .def_readwrite("print_flag", &StorageInputs::print_flag)
    .def_readwrite("is_sunk", &StorageInputs::is_sunk)
    .def_readwrite("power_capacity_kW", &StorageInputs::power_capacity_kW)
    .def_readwrite("energy_capacity_kWh", &StorageInputs::energy_capacity_kWh)
    .def_readwrite("nominal_inflation_annual", &StorageInputs::nominal_inflation_annual)
    .def_readwrite("nominal_discount_annual", &StorageInputs::nominal_discount_annual)
    
    .def(pybind11::init());


pybind11::class_<Storage>(m, "Storage")
    .def_readwrite("type", &Storage::type)
    .def_readwrite("interpolator", &Storage::interpolator)
    .def_readwrite("print_flag", &Storage::print_flag)
    .def_readwrite("is_depleted", &Storage::is_depleted)
    .def_readwrite("is_sunk", &Storage::is_sunk)
    .def_readwrite("n_points", &Storage::n_points)
    .def_readwrite("n_replacements", &Storage::n_replacements)
    .def_readwrite("n_years", &Storage::n_years)
    .def_readwrite("power_capacity_kW", &Storage::power_capacity_kW)
    .def_readwrite("energy_capacity_kWh", &Storage::energy_capacity_kWh)
    .def_readwrite("charge_kWh", &Storage::charge_kWh)
    .def_readwrite("power_kW", &Storage::power_kW)
    .def_readwrite("nominal_inflation_annual", &Storage::nominal_inflation_annual)
    .def_readwrite("nominal_discount_annual", &Storage::nominal_discount_annual)
    .def_readwrite("real_discount_annual", &Storage::real_discount_annual)
    .def_readwrite("capital_cost", &Storage::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &Storage::operation_maintenance_cost_kWh
    )
    .def_readwrite("net_present_cost", &Storage::net_present_cost)
    .def_readwrite("total_discharge_kWh", &Storage::total_discharge_kWh)
    .def_readwrite(
        "levellized_cost_of_energy_kWh",
        &Storage::levellized_cost_of_energy_kWh
    )
    .def_readwrite("type_str", &Storage::type_str)
    .def_readwrite("charge_vec_kWh", &Storage::charge_vec_kWh)
    .def_readwrite("charging_power_vec_kW", &Storage::charging_power_vec_kW)
    .def_readwrite("discharging_power_vec_kW", &Storage::discharging_power_vec_kW)
    .def_readwrite("capital_cost_vec", &Storage::capital_cost_vec)
    .def_readwrite(
        "operation_maintenance_cost_vec",
        &Storage::operation_maintenance_cost_vec
    )
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, StorageInputs>())
    .def("handleReplacement", &Storage::handleReplacement)
    .def("computeEconomics", &Storage::computeEconomics)
    .def("getAvailablekW", &Storage::getAvailablekW)
    .def("getAcceptablekW", &Storage::getAcceptablekW)
    .def("commitCharge", &Storage::commitCharge)
    .def("commitDischarge", &Storage::commitDischarge)
    .def("writeResults", &Storage::writeResults);
