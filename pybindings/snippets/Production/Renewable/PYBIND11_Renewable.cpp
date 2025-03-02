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
/// \file PYBIND11_Renewable.cpp
///
/// \brief Bindings file for the Renewable class. Intended to be #include'd in
///     PYBIND11_PGM.cpp
///
/// Ref: \cite pybind11\n
///
/// A file which instructs pybind11 how to build Python bindings for the Renewable
/// class. Only public attributes/methods are bound!
///


pybind11::enum_<RenewableType>(m, "RenewableType")
    .value("SOLAR", RenewableType::SOLAR)
    .value("TIDAL", RenewableType::TIDAL)
    .value("WAVE", RenewableType::WAVE)
    .value("WIND", RenewableType::WIND)
    .value("N_RENEWABLE_TYPES", RenewableType::N_RENEWABLE_TYPES);


pybind11::class_<RenewableInputs>(m, "RenewableInputs")
    .def_readwrite("production_inputs", &RenewableInputs::production_inputs)
    
    .def(pybind11::init());


pybind11::class_<Renewable, Production>(m, "Renewable")
    .def_readwrite("type", &Renewable::type)
    .def_readwrite("resource_key", &Renewable::resource_key)
    .def_readwrite("firmness_factor", &Renewable::firmness_factor)
      
    .def(pybind11::init<>())
    .def(pybind11::init<int, double, RenewableInputs, std::vector<double>*>())
    .def("handleReplacement", &Renewable::handleReplacement)
    .def("computeEconomics", &Renewable::computeEconomics)
    .def(
        "computeProductionkW",
        pybind11::overload_cast<int, double, double>(&Renewable::computeProductionkW)
    )
    .def(
        "computeProductionkW",
        pybind11::overload_cast<int, double, double, double>(&Renewable::computeProductionkW)
    )
    .def("commit", &Renewable::commit)
    .def("writeResults", &Renewable::writeResults);
