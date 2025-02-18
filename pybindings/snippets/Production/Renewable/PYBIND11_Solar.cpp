/*
 * PGMcpp : PRIMED Grid Modelling (in C++)
 * Copyright 2023 (C)
 * 
 * Anthony Truelove MASc, P.Eng.
 * email:  gears1763@tutanota.com
 * github: gears1763-2
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 * 
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 * 
 *  CONTINUED USE OF THIS SOFTWARE CONSTITUTES ACCEPTANCE OF THESE TERMS.
 *
 */


///
/// \file PYBIND11_Solar.cpp
///
/// \brief Bindings file for the Solar class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Solar
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<SolarPowerProductionModel>(m, "SolarPowerProductionModel")
    .value("SOLAR_POWER_SIMPLE", SolarPowerProductionModel::SOLAR_POWER_SIMPLE)
    .value("SOLAR_POWER_DETAILED", SolarPowerProductionModel::SOLAR_POWER_DETAILED)
    .value(
        "N_SOLAR_POWER_PRODUCTION_MODELS",
        SolarPowerProductionModel::N_SOLAR_POWER_PRODUCTION_MODELS
    );


pybind11::class_<SolarInputs>(m, "SolarInputs")
    .def_readwrite("renewable_inputs", &SolarInputs::renewable_inputs)
    .def_readwrite("resource_key", &SolarInputs::resource_key)
    .def_readwrite("firmness_factor", &SolarInputs::firmness_factor)
    .def_readwrite("capital_cost", &SolarInputs::capital_cost)
    .def_readwrite(
        "operation_maintenance_cost_kWh",
        &SolarInputs::operation_maintenance_cost_kWh
    )
    .def_readwrite("derating", &SolarInputs::derating)
    .def_readwrite("julian_day", &SolarInputs::julian_day)
    .def_readwrite("latitude_deg", &SolarInputs::latitude_deg)
    .def_readwrite("longitude_deg", &SolarInputs::longitude_deg)
    .def_readwrite("panel_azimuth_deg", &SolarInputs::panel_azimuth_deg)
    .def_readwrite("panel_tilt_deg", &SolarInputs::panel_tilt_deg)
    .def_readwrite("albedo_ground_reflectance", &SolarInputs::albedo_ground_reflectance)
    .def_readwrite("power_model", &SolarInputs::power_model)
    
    .def(pybind11::init());


pybind11::class_<Solar>(m, "Solar")
    .def_readwrite("derating", &Solar::derating)
    .def_readwrite("power_model", &Solar::power_model)
    .def_readwrite("power_model_string", &Solar::power_model_string)
    .def_readwrite("total_production_kWh", &Production::total_production_kWh)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, SolarInputs, std::vector<double>*>())
    .def("handleReplacement", &Solar::handleReplacement)
    .def("computeProductionkW", &Solar::computeProductionkW)
    .def("commit", &Solar::commit);
