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
/// \file PYBIND11_PGM.cpp
///
/// \brief Python 3 bindings file for PGMcpp.
///
/// This is a file which defines the Python 3 bindings to be generated for PGMcpp. To
/// generate bindings, use the provided setup.py. \n
/// \n
/// ref: https://pybind11.readthedocs.io/en/stable/


#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../header/Model.h"


PYBIND11_MODULE(PGMcpp, m) {

// ================ Controller ================ //
/*
pybind11::class_<Controller>(m, "Controller")
    .def(pybind11::init());
*/
// ================ END Controller ================ //



// ================ ElectricalLoad ================ //
/*
pybind11::class_<ElectricalLoad>(m, "ElectricalLoad")
    .def_readwrite("n_points", &ElectricalLoad::n_points)
    .def_readwrite("max_load_kW", &ElectricalLoad::max_load_kW)
    .def_readwrite("mean_load_kW", &ElectricalLoad::mean_load_kW)
    .def_readwrite("min_load_kW", &ElectricalLoad::min_load_kW)
    .def_readwrite("dt_vec_hrs", &ElectricalLoad::dt_vec_hrs)
    .def_readwrite("load_vec_kW", &ElectricalLoad::load_vec_kW)
    .def_readwrite("time_vec_hrs", &ElectricalLoad::time_vec_hrs)
    
    .def(pybind11::init<std::string>());
*/
// ================ END ElectricalLoad ================ //



// ================ Model ================ //
/*
pybind11::class_<Model>(m, "Model")
    .def(
        pybind11::init<
            ElectricalLoad*,
            RenewableResources*
        >()
    );
*/
// ================ END Model ================ //



// ================ RenewableResources ================ //
/*
pybind11::class_<RenewableResources>(m, "RenewableResources")
    .def(pybind11::init());
    /*
    .def(pybind11::init<>());
    */
*/
// ================ END RenewableResources ================ //

}   /* PYBIND11_MODULE() */



/*
pybind11::enum_<NondispatchableType>(m, "NondispatchableType")
    .value("SOLAR", NondispatchableType::SOLAR)
    .value("TIDAL", NondispatchableType::TIDAL)
    .value("WAVE", NondispatchableType::WAVE)
    .value("WIND", NondispatchableType::WIND);

pybind11::class_<structNondispatchable>(m, "structNondispatchable")
    .def_readwrite("is_sunk", &structNondispatchable::is_sunk)
    .def_readwrite("print_flag", &structNondispatchable::print_flag)
    .def_readwrite("test_flag", &structNondispatchable::test_flag)
    .def_readwrite("cap_kW", &structNondispatchable::cap_kW)
    .def_readwrite(
        "replace_running_hrs",
        &structNondispatchable::replace_running_hrs
    )
    .def_readwrite(
        "nominal_inflation_rate_annual",
        &structNondispatchable::nominal_inflation_rate_annual
    )
    .def_readwrite(
        "nominal_discount_rate_annual",
        &structNondispatchable::nominal_discount_rate_annual
    )
    .def_readwrite("capital_cost", &structNondispatchable::capital_cost)
    .def_readwrite(
        "op_maint_cost_per_kWh",
        &structNondispatchable::op_maint_cost_per_kWh
    )
    
    .def(pybind11::init());

pybind11::class_<Model>(m, "Model")
    .def_readwrite("struct_model", &Model::struct_model)
    .def_readwrite("n_timesteps", &Model::n_timesteps)
    .def_readwrite("project_life_yrs", &Model::project_life_yrs)
    .def_readwrite("total_load_served_kWh", &Model::total_load_served_kWh)
    .def_readwrite("total_fuel_consumed_L", &Model::total_fuel_consumed_L)
    .def_readwrite("total_CO2_emitted_kg", &Model::total_CO2_emitted_kg)
    .def_readwrite("total_CO_emitted_kg", &Model::total_CO_emitted_kg)
    .def_readwrite("total_NOx_emitted_kg", &Model::total_NOx_emitted_kg)
    .def_readwrite("total_SOx_emitted_kg", &Model::total_SOx_emitted_kg)
    .def_readwrite("total_CH4_emitted_kg", &Model::total_CH4_emitted_kg)
    .def_readwrite("total_PM_emitted_kg", &Model::total_PM_emitted_kg)
    .def_readwrite("real_discount_rate_annual", &Model::real_discount_rate_annual)
    .def_readwrite("net_present_cost", &Model::net_present_cost)
    .def_readwrite(
        "levellized_cost_of_energy_per_kWh",
        &Model::levellized_cost_of_energy_per_kWh
    )
    .def_readwrite("dt_vec_hr", &Model::dt_vec_hr)
    .def_readwrite("load_vec_kW", &Model::load_vec_kW)
    .def_readwrite("net_load_vec_kW", &Model::net_load_vec_kW)
    .def_readwrite("remaining_load_vec_kW", &Model::remaining_load_vec_kW)
    .def_readwrite("time_vec_hr", &Model::time_vec_hr)
    
    .def(pybind11::init<structModel>())
    .def("add1dRenewableResource", &Model::add1dRenewableResource)
    .def("add2dRenewableResource", &Model::add2dRenewableResource)
    .def("addSolar", &Model::addSolar)
    .def("addTidal", &Model::addTidal)
    .def("addWave", &Model::addWave)
    .def("addWind", &Model::addWind)
    .def("addDiesel", &Model::addDiesel)
    .def("addLiIon", &Model::addLiIon)
    .def("run", &Model::run)
    .def("writeResults", &Model::writeResults)
    .def("clearAssets", &Model::clearAssets)
    .def("reset", &Model::reset);
 */
