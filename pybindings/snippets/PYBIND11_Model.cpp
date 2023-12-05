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
/// \file PYBIND11_Model.cpp
///
/// \brief Bindings file for the Model class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Model
/// class. Only public attributes/methods are bound!
///


pybind11::class_<ModelInputs>(m, "ModelInputs")
    .def_readwrite(
        "path_2_electrical_load_time_series",
        &ModelInputs::path_2_electrical_load_time_series
    )
    .def_readwrite("control_mode", &ModelInputs::control_mode)
    
    .def(pybind11::init());


pybind11::class_<Model>(m, "Model")
    .def_readwrite("total_fuel_consumed_L", &Model::total_fuel_consumed_L)
    .def_readwrite("total_emissions", &Model::total_emissions)
    .def_readwrite("net_present_cost", &Model::net_present_cost)
    .def_readwrite("total_renewable_dispatch_kWh", &Model::total_renewable_dispatch_kWh)
    .def_readwrite("total_dispatch_discharge_kWh", &Model::total_dispatch_discharge_kWh)
    .def_readwrite(
        "levellized_cost_of_energy_kWh",
        &Model::levellized_cost_of_energy_kWh
    )
    .def_readwrite("controller", &Model::controller)
    .def_readwrite("electrical_load", &Model::electrical_load)
    .def_readwrite("resources", &Model::resources)
    .def_readwrite("combustion_ptr_vec", &Model::combustion_ptr_vec)
    .def_readwrite("noncombustion_ptr_vec", &Model::noncombustion_ptr_vec)
    .def_readwrite("renewable_ptr_vec", &Model::renewable_ptr_vec)
    .def_readwrite("storage_ptr_vec", &Model::storage_ptr_vec)
    
    .def(pybind11::init<>())
    .def(pybind11::init<ModelInputs>())
    .def("addDiesel", &Model::addDiesel)
    .def(
        "addResource",
        pybind11::overload_cast<NoncombustionType, std::string, int>(&Model::addResource)
    )
    .def(
        "addResource",
        pybind11::overload_cast<RenewableType, std::string, int>(&Model::addResource)
    )
    .def("addHydro", &Model::addHydro)
    .def("addSolar", &Model::addSolar)
    .def("addTidal", &Model::addTidal)
    .def("addWave", &Model::addWave)
    .def("addWind", &Model::addWind)
    .def("addLiIon", &Model::addLiIon)
    .def("run", &Model::run)
    .def("reset", &Model::reset)
    .def("clear", &Model::clear)
    .def(
        "writeResults",
        &Model::writeResults,
        pybind11::arg("write_path"),
        pybind11::arg("max_lines") = -1
    );
