/*
 *  PGMcpp : PRIMED Grid Dieselling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Diesel.cpp
///
/// \brief Implementation file for the Diesel class.
///
/// A derived class of the Combustion branch of Production which models production
/// using a diesel generator.
///


#include "../../../header/Production/Combustion/Diesel.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Diesel :: __checkInputs(DieselInputs diesel_inputs)
///
/// \brief Helper method to check inputs to the Diesel constructor.
///
/// \param diesel_inputs A structure of Diesel constructor inputs.
///

void Diesel :: __checkInputs(DieselInputs diesel_inputs)
{
    //  1. check fuel_cost_L
    if (diesel_inputs.fuel_cost_L < 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::fuel_cost_L must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check CO2_emissions_intensity_kgL
    if (diesel_inputs.CO2_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::CO2_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check CO_emissions_intensity_kgL
        if (diesel_inputs.CO_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::CO_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check NOx_emissions_intensity_kgL
    if (diesel_inputs.NOx_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::NOx_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  5. check SOx_emissions_intensity_kgL
    if (diesel_inputs.SOx_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::SOx_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  6. check CH4_emissions_intensity_kgL
    if (diesel_inputs.CH4_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::CH4_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  7. check PM_emissions_intensity_kgL
    if (diesel_inputs.PM_emissions_intensity_kgL < 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::PM_emissions_intensity_kgL must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  8. check minimum_load_ratio
    if (diesel_inputs.minimum_load_ratio < 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::minimum_load_ratio must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  9. check minimum_runtime_hrs
    if (diesel_inputs.minimum_runtime_hrs < 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::minimum_runtime_hrs must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  10. check replace_running_hrs
    if (diesel_inputs.replace_running_hrs <= 0) {
        std::string error_str = "ERROR:  Diesel():  ";
        error_str += "DieselInputs::replace_running_hrs must be > 0";
        
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
/// \fn double Diesel :: __getGenericFuelSlope(void)
///
/// \brief Helper method to generate a generic, linearized fuel consumption
///     slope for a diesel generator.
///
/// This model was obtained by way of surveying an assortment of published diesel
/// generator fuel consumption data, and then constructing a best fit model.
///
/// Ref: \cite HOMER_fuel_curve\n
/// Ref: \cite HOMER_generator_fuel_curve_slope\n
///
/// \return A generic fuel slope for the diesel generator [L/kWh].
///

double Diesel :: __getGenericFuelSlope(void)
{
    double linear_fuel_slope_LkWh = 0.4234 * pow(this->capacity_kW, -0.1012);
    
    return linear_fuel_slope_LkWh;
}   /* __getGenericFuelSlope() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Diesel :: __getGenericFuelIntercept(void)
///
/// \brief Helper method to generate a generic, linearized fuel consumption
///     intercept for a diesel generator.
///
/// This model was obtained by way of surveying an assortment of published diesel
/// generator fuel consumption data, and then constructing a best fit model.
///
/// Ref: \cite HOMER_fuel_curve\n
/// Ref: \cite HOMER_generator_fuel_curve_intercept_coefficient\n
///
/// \return A generic fuel intercept coefficient for the diesel generator [L/kWh].
///

double Diesel :: __getGenericFuelIntercept(void)
{
    double linear_fuel_intercept_LkWh = 0.0940 * pow(this->capacity_kW, -0.2735);
    
    return linear_fuel_intercept_LkWh;
}   /* __getGenericFuelIntercept() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Diesel :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic diesel generator capital cost.
///
/// This model was obtained by way of surveying an assortment of published diesel 
/// generator costs, and then constructing a best fit model. Note that this model
/// expresses cost in terms of Canadian dollars [CAD].
///
/// \return A generic capital cost for the diesel generator [CAD].
///

double Diesel :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kW = 1000 * pow(this->capacity_kW, -0.425) + 800;
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Diesel :: __getGenericOpMaintCost(void)
///
/// \brief Helper method (private) to generate a generic diesel generator operation and
///     maintenance cost. This is a cost incurred per unit energy produced.
///
/// This model was obtained by way of surveying an assortment of published diesel 
/// generator costs, and then constructing a best fit model. Note that this model
/// expresses cost in terms of Canadian dollars per kiloWatt-hour production
/// [CAD/kWh].
///
/// \return A generic operation and maintenance cost, per unit energy produced, for the
///     diesel generator [CAD/kWh].
///

double Diesel :: __getGenericOpMaintCost(void)
{
    double operation_maintenance_cost_kWh = 0.05 * pow(this->capacity_kW, -0.2) + 0.05;
    
    return operation_maintenance_cost_kWh;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Diesel :: __handleStartStop(int timestep, double dt_hrs, double production_kW)
///
/// \brief Helper method (private) to handle the starting/stopping of the diesel
///     generator. The minimum runtime constraint is enforced in this method.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param production_kW The current rate of production [kW] of the generator.
///

void Diesel :: __handleStartStop(int timestep, double dt_hrs, double production_kW)
{
    /*
     *  Helper method (private) to handle the starting/stopping of the diesel 
     *  generator. The minimum runtime constraint is enforced in this method.
     */
    
    if (this->is_running) {
        // handle stopping
        if (
            production_kW <= 0 and
            this->time_since_last_start_hrs >= this->minimum_runtime_hrs
        ) {
            this->is_running = false;
        }
    }
    
    else {
        // handle starting
        if (production_kW > 0) {
            this->is_running = true;
            this->n_starts++;
            this->time_since_last_start_hrs = 0;
        }
    }
    
    return;
}   /* __handleStartStop() */

// ---------------------------------------------------------------------------------- //




// ---------------------------------------------------------------------------------- //

///
/// \fn void Diesel :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for Model.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void Diesel :: __writeSummary(std::string write_path)
{
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write to summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->capacity_kW)));
    ofs << " kW DIESEL Summary Results\n";
    ofs << "\n--------\n\n";
    
    //  2.1. Production attributes
    ofs << "## Production Attributes\n";
    ofs << "\n";
    
    ofs << "Capacity: " << this->capacity_kW << "kW  \n";
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
    
    //  2.2. Combustion attributes
    ofs << "## Combustion Attributes\n";
    ofs << "\n";
    
    ofs << "Fuel Cost: " << this->fuel_cost_L << " per L  \n";
    ofs << "\n";
    
    ofs << "Linear Fuel Slope: " << this->linear_fuel_slope_LkWh << " L/kWh  \n";
    ofs << "Linear Fuel Intercept Coefficient: " << this->linear_fuel_intercept_LkWh
        << " L/kWh  \n";
    ofs << "\n";
    
    ofs << "Carbon Dioxide (CO2) Emissions Intensity: "
        << this->CO2_emissions_intensity_kgL << " kg/L  \n";
    
    ofs << "Carbon Monoxide (CO) Emissions Intensity: "
        << this->CO_emissions_intensity_kgL << " kg/L  \n";
        
    ofs << "Nitrogen Oxides (NOx) Emissions Intensity: "
        << this->NOx_emissions_intensity_kgL << " kg/L  \n";
        
    ofs << "Sulfur Oxides (SOx) Emissions Intensity: "
        << this->SOx_emissions_intensity_kgL << " kg/L  \n";
        
    ofs << "Methane (CH4) Emissions Intensity: "
        << this->CH4_emissions_intensity_kgL << " kg/L  \n";
        
    ofs << "Particulate Matter (PM) Emissions Intensity: "
        << this->PM_emissions_intensity_kgL << " kg/L  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.3. Diesel attributes
    ofs << "## Diesel Attributes\n";
    ofs << "\n";
    
    ofs << "Minimum Load Ratio: " << this->minimum_load_ratio << "  \n";
    ofs << "Minimum Runtime: " << this->minimum_runtime_hrs << " hrs  \n";
    
    ofs << "\n--------\n\n";
    
    //  2.4. Diesel Results
    ofs << "## Results\n";
    ofs << "\n";
    
    ofs << "Net Present Cost: " << this->net_present_cost << "  \n";
    ofs << "\n";
    
    ofs << "Total Dispatch + Discharge: " << this->total_dispatch_kWh
        << " kWh  \n";
        
    ofs << "Levellized Cost of Energy: " << this->levellized_cost_of_energy_kWh
        << " per kWh dispatched/discharged  \n";
    ofs << "\n";
    
    ofs << "Running Hours: " << this->running_hours << "  \n";
    ofs << "Starts: " << this->n_starts << "  \n";
    ofs << "Replacements: " << this->n_replacements << "  \n";
    
    ofs << "Total Fuel Consumed: " << this->total_fuel_consumed_L << " L "
        << "(Annual Average: " << this->total_fuel_consumed_L / this->n_years
        << " L/yr)  \n";
    ofs << "\n";
    
    ofs << "Total Carbon Dioxide (CO2) Emissions: " <<
        this->total_emissions.CO2_kg << " kg "
        << "(Annual Average: " <<  this->total_emissions.CO2_kg / this->n_years
        << " kg/yr)  \n";
        
    ofs << "Total Carbon Monoxide (CO) Emissions: " <<
        this->total_emissions.CO_kg << " kg "
        << "(Annual Average: " <<  this->total_emissions.CO_kg / this->n_years
        << " kg/yr)  \n";
        
    ofs << "Total Nitrogen Oxides (NOx) Emissions: " <<
        this->total_emissions.NOx_kg << " kg "
        << "(Annual Average: " <<  this->total_emissions.NOx_kg / this->n_years
        << " kg/yr)  \n";
        
    ofs << "Total Sulfur Oxides (SOx) Emissions: " <<
        this->total_emissions.SOx_kg << " kg "
        << "(Annual Average: " <<  this->total_emissions.SOx_kg / this->n_years
        << " kg/yr)  \n";
        
    ofs << "Total Methane (CH4) Emissions: " << this->total_emissions.CH4_kg << " kg "
        << "(Annual Average: " <<  this->total_emissions.CH4_kg / this->n_years
        << " kg/yr)  \n";
        
    ofs << "Total Particulate Matter (PM) Emissions: " <<
        this->total_emissions.PM_kg << " kg "
        << "(Annual Average: " <<  this->total_emissions.PM_kg / this->n_years
        << " kg/yr)  \n";
    
    ofs << "\n--------\n\n";

    ofs.close();
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Diesel :: __writeTimeSeries(std::string write_path, int max_lines)
///
/// \brief Helper method to write time series results for Model.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param max_lines The maximum number of lines of output to write. If <0, then all
///     available lines are written.
///

void Diesel :: __writeTimeSeries(std::string write_path, int max_lines)
{
    //  1. handle sentinel
    if (max_lines < 0) {
        max_lines = this->n_points;
    }
    
    //  2. create filestream
    write_path += "time_series_results.csv";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    /*
    //  3. write to time series results
    ofs << "Time (since start of data) [hrs],";
    ofs << "Electrical Load [kW],";
    ofs << "Net Load [kW],";
    ofs << "Missed Load [kW]\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << this->electrical_load.time_vec_hrs[i] << ",";
        ofs << this->electrical_load.load_vec_kW[i] << ",";
        ofs << this->controller.net_load_vec_kW[i] << ",";
        ofs << this->controller.missed_load_vec_kW[i] << "\n";
    }
    */
    return;
}   /* __writeTimeSeries() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Diesel :: Diesel(void)
///
/// \brief Constructor (dummy) for the Diesel class.
///

Diesel :: Diesel(void)
{
    return;
}   /* Diesel() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Diesel :: Diesel(
///         int n_points,
///         double n_years,
///         DieselInputs diesel_inputs
///     )
///
/// \brief Constructor (intended) for the Diesel class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param diesel_inputs A structure of Diesel constructor inputs.
///

Diesel :: Diesel(
    int n_points,
    double n_years,
    DieselInputs diesel_inputs
) :
Combustion(
    n_points,
    n_years,
    diesel_inputs.combustion_inputs
)
{
    //  1. check inputs
    this->__checkInputs(diesel_inputs);
    
    //  2. set attributes
    this->type = CombustionType :: DIESEL;
    this->type_str = "DIESEL";
    
    this->replace_running_hrs = diesel_inputs.replace_running_hrs;
    
    this->fuel_cost_L = diesel_inputs.fuel_cost_L;
    
    this->minimum_load_ratio = diesel_inputs.minimum_load_ratio;
    this->minimum_runtime_hrs = diesel_inputs.minimum_runtime_hrs;
    this->time_since_last_start_hrs = 0;
    
    this->CO2_emissions_intensity_kgL = diesel_inputs.CO2_emissions_intensity_kgL;
    this->CO_emissions_intensity_kgL = diesel_inputs.CO_emissions_intensity_kgL;
    this->NOx_emissions_intensity_kgL = diesel_inputs.NOx_emissions_intensity_kgL;
    this->SOx_emissions_intensity_kgL = diesel_inputs.SOx_emissions_intensity_kgL;
    this->CH4_emissions_intensity_kgL = diesel_inputs.CH4_emissions_intensity_kgL;
    this->PM_emissions_intensity_kgL = diesel_inputs.PM_emissions_intensity_kgL;
    
    if (diesel_inputs.linear_fuel_slope_LkWh < 0) {
        this->linear_fuel_slope_LkWh = this->__getGenericFuelSlope();
    }
    
    if (diesel_inputs.linear_fuel_intercept_LkWh < 0) {
        this->linear_fuel_intercept_LkWh = this->__getGenericFuelIntercept();
    }
    
    if (diesel_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    
    if (diesel_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    
    if (not this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "Diesel object constructed at " << this << std::endl;
    }
    
    return;
}   /* Diesel() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Diesel :: requestProductionkW(
///         int timestep,
///         double dt_hrs,
///         double request_kW
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
/// \return The production [kW] delivered by the diesel generator.
///

double Diesel :: requestProductionkW(
    int timestep,
    double dt_hrs,
    double request_kW
)
{
    //  1. return on request of zero
    if (request_kW <= 0) {
        return 0;
    }
    
    double deliver_kW = request_kW;
    
    //  2. enforce capacity constraint
    if (deliver_kW > this->capacity_kW) {
        deliver_kW = this->capacity_kW;
    }
    
    //  3. enforce minimum load ratio
    if (deliver_kW < this->minimum_load_ratio * this->capacity_kW) {
        deliver_kW = this->minimum_load_ratio * this->capacity_kW;
    }
    
    return deliver_kW;
}   /* requestProductionkW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Diesel :: commit(
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

double Diesel :: commit(
    int timestep,
    double dt_hrs,
    double production_kW,
    double load_kW
)
{
    //  1. handle start/stop, enforce minimum runtime constraint
    this->__handleStartStop(timestep, dt_hrs, production_kW);
    
    //  2. invoke base class method
    load_kW = Combustion :: commit(
        timestep,
        dt_hrs,
        production_kW,
        load_kW
    );
    
    if (this->is_running) {
        //  3. log time since last start
        this->time_since_last_start_hrs += dt_hrs;
        
        //  4. correct operation and maintenance costs (should be non-zero if idling)
        if (production_kW <= 0) {
            double produced_kWh = 0.01 * this->capacity_kW * dt_hrs;
        
            double operation_maintenance_cost =
                this->operation_maintenance_cost_kWh * produced_kWh;
            this->operation_maintenance_cost_vec[timestep] = operation_maintenance_cost;
        }
    }
    
    return load_kW;
}   /* commit() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Diesel :: writeResults(
///         std::string write_path,
///         int combustion_index,
///         int max_lines
///     )
///
/// \brief Method which writes Diesel results to an output directory.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param combustion_index An integer which corresponds to the index of the diesel 
///     generator in the containing Model's combustion pointer vector.
///
/// \param max_lines The maximum number of lines of output to write. If <0, then all
///     available lines are written.
///

void Diesel :: writeResults(
    std::string write_path,
    int combustion_index,
    int max_lines
)
{
    //  1. handle sentinel
    if (max_lines < 0) {
        max_lines = this->n_points;
    }
    
    //  2. create subdirectories
    write_path += "Production/";
    if (not std::filesystem::is_directory(write_path)) {
        std::filesystem::create_directory(write_path);
    }
    
    write_path += "Combustion/";
    if (not std::filesystem::is_directory(write_path)) {
        std::filesystem::create_directory(write_path);
    }
    
    write_path += this->type_str;
    write_path += "_";
    write_path += std::to_string(int(ceil(this->capacity_kW)));
    write_path += "kW_idx";
    write_path += std::to_string(combustion_index);
    write_path += "/";
    std::filesystem::create_directory(write_path);
    
    //  3. write summary
    this->__writeSummary(write_path);
    
    //  4. write time series
    this->__writeTimeSeries(write_path, max_lines);
    
    return;
}   /* writeResults() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Diesel :: ~Diesel(void)
///
/// \brief Destructor for the Diesel class.
///

Diesel :: ~Diesel(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "Diesel object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~Diesel() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
