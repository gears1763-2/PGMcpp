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
/// \file Solar.cpp
///
/// \brief Implementation file for the Solar class.
///
/// A derived class of the Renewable branch of Production which models solar
/// production.
///


#include "../../../header/Production/Renewable/Solar.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Solar :: __checkInputs(SolarInputs solar_inputs)
///
/// \brief Helper method to check inputs to the Solar constructor.
///

void Solar :: __checkInputs(SolarInputs solar_inputs)
{
    //  1. check derating
    if (
        solar_inputs.derating < 0 or
        solar_inputs.derating > 1
    ) {
        std::string error_str = "ERROR:  Solar():  ";
        error_str += "SolarInputs::derating must be in the closed interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkInputs() */


// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic solar PV array capital cost.
///
/// This model was obtained by way of surveying an assortment of published solar PV
/// costs, and then constructing a best fit model. Note that this model
/// expresses cost in terms of Canadian dollars [CAD].
///
/// \return A generic capital cost for the solar PV array [CAD].
///

double Solar :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kW = 1000 * pow(this->capacity_kW, -0.15) + 3000;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: __getGenericOpMaintCost(void)
///
/// \brief Helper method to generate a generic solar PV array operation and
///     maintenance cost. This is a cost incurred per unit energy produced.
///
/// This model was obtained by way of surveying an assortment of published solar PV
/// costs, and then constructing a best fit model. Note that this model
/// expresses cost in terms of Canadian dollars [CAD/kWh].
///
/// \return A generic operation and maintenance cost, per unit energy produced, for the
///     solar PV array [CAD/kWh].
///

double Solar :: __getGenericOpMaintCost(void)
{
    return 0.01;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Solar :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for Solar.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void Solar :: __writeSummary(std::string write_path)
{
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->capacity_kW)));
    ofs << " kW SOLAR Summary Results\n";
    ofs << "\n--------\n\n";
    
    //  2.1. Production attributes
    ofs << "## Production Attributes\n";
    ofs << "\n";
    
    ofs << "Capacity: " << this->capacity_kW << " kW  \n";
    ofs << "\n";
    
    ofs << "Production Override: (N = 0 / Y = 1): "
        << this->normalized_production_series_given << "  \n";
    if (this->normalized_production_series_given) {
        ofs << "Path to Normalized Production Time Series: "
            << this->path_2_normalized_production_time_series << "  \n";
    }
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
    
    //  2.2. Renewable attributes
    ofs << "## Renewable Attributes\n";
    ofs << "\n";
    
    ofs << "Resource Key (1D): " << this->resource_key << "  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.3. Solar attributes
    ofs << "## Solar Attributes\n";
    ofs << "\n";
    
    ofs << "Derating Factor: " << this->derating << "  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.4. Solar Results
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
    ofs << "Replacements: " << this->n_replacements << "  \n";
    
    ofs << "\n--------\n\n";
    
    ofs.close();
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Solar :: __writeTimeSeries(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         std::map<int, std::vector<double>>* resource_map_1D_ptr,
///         std::map<int, std::vector<std::vector<double>>>* resource_map_2D_ptr,
///         int max_lines
///     )
///
/// \brief Helper method to write time series results for Solar.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///
/// \param resource_map_1D_ptr A pointer to the 1D map of Resources.
///
/// \param resource_map_2D_ptr A pointer to the 2D map of Resources.
///
/// \param max_lines The maximum number of lines of output to write.
///

void Solar :: __writeTimeSeries(
    std::string write_path,
    std::vector<double>* time_vec_hrs_ptr,
    std::map<int, std::vector<double>>* resource_map_1D_ptr,
    std::map<int, std::vector<std::vector<double>>>* resource_map_2D_ptr,
    int max_lines
)
{
    //  1. create filestream
    write_path += "time_series_results.csv";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write time series results (comma separated value)
    ofs << "Time (since start of data) [hrs],";
    ofs << "Solar Resource [kW/m2],";
    ofs << "Production [kW],";
    ofs << "Dispatch [kW],";
    ofs << "Storage [kW],";
    ofs << "Curtailment [kW],";
    ofs << "Capital Cost (actual),";
    ofs << "Operation and Maintenance Cost (actual),";
    ofs << "\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << time_vec_hrs_ptr->at(i) << ",";
        
        if (not this->normalized_production_series_given) {
            ofs << resource_map_1D_ptr->at(this->resource_key)[i] << ",";
        }
        
        else {
            ofs << "OVERRIDE" << ",";
        }
        
        ofs << this->production_vec_kW[i] << ",";
        ofs << this->dispatch_vec_kW[i] << ",";
        ofs << this->storage_vec_kW[i] << ",";
        ofs << this->curtailment_vec_kW[i] << ",";
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
/// \fn Solar :: Solar(void)
///
/// \brief Constructor (dummy) for the Solar class.
///

Solar :: Solar(void)
{
    //...
    
    return;
}   /* Solar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Solar :: Solar(
///         int n_points,
///         double n_years,
///         SolarInputs solar_inputs,
///         std::vector<double>* time_vec_hrs_ptr
///     )
///
/// \brief Constructor (intended) for the Solar class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param solar_inputs A structure of Solar constructor inputs.
///
/// \param time_vec_hrs_ptr A pointer to the vector containing the modelling time series.
///

Solar :: Solar(
    int n_points,
    double n_years,
    SolarInputs solar_inputs,
    std::vector<double>* time_vec_hrs_ptr
) :
Renewable(
    n_points,
    n_years,
    solar_inputs.renewable_inputs,
    time_vec_hrs_ptr
)
{
    //  1. check inputs
    this->__checkInputs(solar_inputs);
    
    //  2. set attributes
    this->type = RenewableType :: SOLAR;
    this->type_str = "SOLAR";
    
    this->resource_key = solar_inputs.resource_key;
    
    this->derating = solar_inputs.derating;
    
    if (solar_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    else {
        this->capital_cost = solar_inputs.capital_cost;
    }
    
    if (solar_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    else {
        this->operation_maintenance_cost_kWh =
            solar_inputs.operation_maintenance_cost_kWh;
    }
    
    if (not this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Solar object constructed at " << this << std::endl;
    }
    
    return;
}   /* Renewable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Solar :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void Solar :: handleReplacement(int timestep)
{
    //  1. reset attributes
    //...
    
    //  2. invoke base class method
    Renewable :: handleReplacement(timestep);
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: computeProductionkW(
///         int timestep,
///         double dt_hrs,
///         double solar_resource_kWm2
///     )
///
/// \brief Method which takes in the solar resource at a particular point in time, and
///     then returns the solar PV production at that point in time.
///
/// Ref: \cite HOMER_how_homer_calculates_the_pv_array_power_output\n
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param solar_resource_kWm2 Solar resource (i.e. irradiance) [kW/m2].
///
/// \return The production [kW] of the solar PV array.
///

double Solar :: computeProductionkW(
    int timestep,
    double dt_hrs,
    double solar_resource_kWm2
)
{
    //  given production time series override
    if (this->normalized_production_series_given) {
        double production_kW = Production :: getProductionkW(timestep);
        
        return production_kW;
    }
    
    // check if no resource
    if (solar_resource_kWm2 <= 0) {
        return 0;
    }
    
    // compute production
    double production_kW = this->derating * solar_resource_kWm2 * this->capacity_kW;
    
    // cap production at capacity
    if (production_kW > this->capacity_kW) {
        production_kW = this->capacity_kW;
    }
    
    return production_kW;
}   /* computeProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Solar :: commit(
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

double Solar :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. invoke base class method
    load_kW = Renewable :: commit(
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
/// \fn Solar :: ~Solar(void)
///
/// \brief Destructor for the Solar class.
///

Solar :: ~Solar(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Solar object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Solar() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
