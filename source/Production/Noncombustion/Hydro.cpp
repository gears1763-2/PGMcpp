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
/// \file Hydro.cpp
///
/// \brief Implementation file for the Hydro class.
///
/// A derived class of the Noncombustion branch of Production which models production
/// using a hydroelectric asset (either with reservoir or not).
///


#include "../../../header/Production/Noncombustion/Hydro.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: __checkInputs(HydroInputs hydro_inputs)
///
/// \brief Helper method to check inputs to the Hydro constructor.
///
/// \param hydro_inputs A structure of Hydro constructor inputs.
///

void Hydro :: __checkInputs(HydroInputs hydro_inputs)
{
    //...
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for Hydro.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void Hydro :: __writeSummary(std::string write_path)
{
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write to summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->capacity_kW)));
    ofs << " kW HYDRO Summary Results\n";
    ofs << "\n--------\n\n";
    
    //  2.1. Production attributes
    ofs << "## Production Attributes\n";
    ofs << "\n";
    
    ofs << "Capacity: " << this->capacity_kW << " kW  \n";
    ofs << "\n";
    
    ofs << "Sunk Cost (N = 0 / Y = 1): " << this->is_sunk << "  \n";
    ofs << "Capital Cost: " << this->capital_cost << "  \n";
    ofs << "Operation and Maintenance Cost: " << this->operation_maintenance_cost_kWh
        << " per kWh produced  \n";
    ofs << "Nominal Inflation Rate (annual): " << this->nominal_inflation_annual
        << "  \n";
    ofs << "Nominal Discount Rate (annual): " << this->nominal_discount_annual
        << "  \n";
    ofs << "Real Discount Rate (annual): " << this->real_discount_annual << "  \n";
    ofs << "\n";
    
    ofs << "Replacement Running Hours: " << this->replace_running_hrs << "  \n";
    ofs << "\n--------\n\n";
    
    //  2.2. Noncombustion attributes
    ofs << "## Noncombustion Attributes\n";
    ofs << "\n";
    
    //...
    
    ofs << "\n--------\n\n";
    
    //  2.3. Hydro attributes
    ofs << "## Hydro Attributes\n";
    ofs << "\n";
    
    //...
    
    ofs << "\n--------\n\n";
    
    //  2.4. Hydro Results
    ofs << "## Results\n";
    ofs << "\n";
    
    ofs << "Net Present Cost: " << this->net_present_cost << "  \n";
    ofs << "\n";
    
    ofs << "Total Dispatch: " << this->total_dispatch_kWh
        << " kWh  \n";
        
    ofs << "Levellized Cost of Energy: " << this->levellized_cost_of_energy_kWh
        << " per kWh dispatched  \n";
    ofs << "\n";
    
    ofs << "Running Hours: " << this->running_hours << "  \n";
    ofs << "Starts: " << this->n_starts << "  \n";
    ofs << "Replacements: " << this->n_replacements << "  \n";
    
    //...
    
    ofs << "\n--------\n\n";

    ofs.close();
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: __writeTimeSeries(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         int max_lines
///     )
///
/// \brief Helper method to write time series results for Hydro.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///
/// \param max_lines The maximum number of lines of output to write.
///

void Hydro :: __writeTimeSeries(
    std::string write_path,
    std::vector<double>* time_vec_hrs_ptr,
    int max_lines
)
{
    //  1. create filestream
    write_path += "time_series_results.csv";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write time series results (comma separated value)
    ofs << "Time (since start of data) [hrs],";
    ofs << "Production [kW],";
    ofs << "Dispatch [kW],";
    ofs << "Storage [kW],";
    ofs << "Curtailment [kW],";
    ofs << "Is Running (N = 0 / Y = 1),";
    //...
    ofs << "Capital Cost (actual),";
    ofs << "Operation and Maintenance Cost (actual),";
    ofs << "\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << time_vec_hrs_ptr->at(i) << ",";
        ofs << this->production_vec_kW[i] << ",";
        ofs << this->dispatch_vec_kW[i] << ",";
        ofs << this->storage_vec_kW[i] << ",";
        ofs << this->curtailment_vec_kW[i] << ",";
        ofs << this->is_running_vec[i] << ",";
        //...
        ofs << this->capital_cost_vec[i] << ",";
        ofs << this->operation_maintenance_cost_vec[i] << ",";
        ofs << "\n";
    }

    ofs.close();
    return;
}   /* __writeTimeSeries() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Hydro :: Hydro(void)
///
/// \brief Constructor (dummy) for the Hydro class.
///

Hydro :: Hydro(void)
{
    return;
}   /* Hydro() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Hydro :: Hydro(
///         int n_points,
///         double n_years,
///         HydroInputs hydro_inputs
///     )
///
/// \brief Constructor (intended) for the Hydro class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param hydro_inputs A structure of Hydro constructor inputs.
///

Hydro :: Hydro(
    int n_points,
    double n_years,
    HydroInputs hydro_inputs
) :
Noncombustion(
    n_points,
    n_years,
    hydro_inputs.noncombustion_inputs
)
{
    //  1. check inputs
    this->__checkInputs(hydro_inputs);
    
    //  2. set attributes
    this->type = NoncombustionType :: HYDRO;
    this->type_str = "HYDRO";
    
    this->resource_key = hydro_inputs.resource_key;
    
    //...
    
    return;
}   /* Hydro() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Hydro :: handleReplacement(int timestep)
{
    //  1. reset attributes
    //...
    
    //  2. invoke base class method
    Noncombustion :: handleReplacement(timestep);
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: requestProductionkW(
///         int timestep,
///         double dt_hrs,
///         double request_kW,
///         double hydro_resource_m3hr
///     )
///
/// \brief Method which takes in production request, and then returns what the asset can
///     deliver (subject to operating constraints, etc.).
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param request_kW The requested production [kW].
///
/// \param hydro_resource_m3hr The currently available hydro flow resource [m3/hr].
///
/// \return The production [kW] delivered by the hydro generator.
///

double Hydro :: requestProductionkW(
    int timestep,
    double dt_hrs,
    double request_kW,
    double hydro_resource_m3hr
)
{
    //  1. return on request of zero
    if (request_kW <= 0) {
        return 0;
    }
    
    //...
    
    return 0;
}   /* requestProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: commit(
///         int timestep,
///         double dt_hrs,
///         double production_kW,
///         double load_kW
///     )
///
/// \brief Method which takes in production and load for the current timestep, computes
///     and records dispatch and curtailment, and then returns remaining load.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param production_kW The production [kW] of the asset in this timestep.
///
/// \param load_kW The load [kW] passed to the asset in this timestep.
///
/// \return The load [kW] remaining after the dispatch is deducted from it.
///

double Hydro :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. invoke base class method
    load_kW = Noncombustion :: commit(
        timestep,
        dt_hrs,
        production_kW,
        load_kW
    );
    
    //...
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Hydro :: ~Hydro(void)
///
/// \brief Destructor for the Hydro class.
///

Hydro :: ~Hydro(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Hydro object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Hydro() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
