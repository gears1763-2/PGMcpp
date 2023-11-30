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
    //  1. check fluid_density_kgm3
    if (hydro_inputs.fluid_density_kgm3 <= 0) {
        std::string error_str = "ERROR:  Hydro():  fluid_density_kgm3 must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check net_head_m
    if (hydro_inputs.net_head_m <= 0) {
        std::string error_str = "ERROR:  Hydro():  net_head_m must be > 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check reservoir_capacity_m3
    if (hydro_inputs.reservoir_capacity_m3 < 0) {
        std::string error_str = "ERROR:  Hydro():  reservoir_capacity_m3 must be >= 0";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check init_reservoir_state
    if (
        hydro_inputs.init_reservoir_state < 0 or
        hydro_inputs.init_reservoir_state > 1
    ) {
        std::string error_str = "ERROR:  Hydro():  init_reservoir_state must be in ";
        error_str += "the closed interval [0, 1]";
        
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
/// \fn double Hydro :: __getGenericCapitalCost(void)
///
/// \brief Helper method to generate a generic hydroelectric capital cost.
///
/// This model was obtained by way of ...
///
/// \return A generic capital cost for the hydroelectric asset [CAD].
///

double Hydro :: __getGenericCapitalCost(void)
{
    double capital_cost_per_kW = 0; //<-- WIP: need something better here
    
    return capital_cost_per_kW * this->capacity_kW;
}   /* __getGenericCapitalCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getGenericOpMaintCost(void)
///
/// \brief Helper method (private) to generate a generic hydroelectric operation and
///     maintenance cost. This is a cost incurred per unit energy produced.
///
/// This model was obtained by way of ...
///
/// \return A generic operation and maintenance cost, per unit energy produced, for the
///     hydroelectric asset [CAD/kWh].
///

double Hydro :: __getGenericOpMaintCost(void)
{
    double operation_maintenance_cost_kWh = 0;  //<-- WIP: need something better here
    
    return operation_maintenance_cost_kWh;
}   /* __getGenericOpMaintCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getMinimumFlowm3hr(void)
///
/// \brief Helper method to compute and return the minimum required flow for production,
///     based on turbine type.
///
/// This model was obtained by way of surveying an assortment of published hydroeletric 
/// operational data, and then constructing a best fit model.
///
/// Ref: \cite Marks_2007\n
///
/// \return The minimum required flow [m3/hr] for production.
///

double Hydro :: __getMinimumFlowm3hr(void)
{
    double coefficient = 0;
    
    switch (this->turbine_type) {
        case (HydroTurbineType :: HYDRO_TURBINE_PELTON): {
            coefficient = 0.023529;
            
            break;
        }
        
        case (HydroTurbineType :: HYDRO_TURBINE_FRANCIS): {
            coefficient = 0.2164706;
            
            break;
        }
        
        default: {
            //..
            
            break;
        }
    }
    
    double minimum_flow_m3hr = (1000 * 3600 * coefficient * this->capacity_kW) /
                (this->fluid_density_kgm3 * 9.81 * this->net_head_m);
    
    return minimum_flow_m3hr;
}   /* __getMinimumFlowm3hr() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getMaximumFlowm3hr(void)
///
/// \brief Helper method to compute and return the maximum productive flow, based on
///     turbine type.
///
/// This model was obtained by way of surveying an assortment of published hydroeletric 
/// operational data, and then constructing a best fit model.
///
/// Ref: \cite Marks_2007\n
///
/// \return The maximum productive flow [m3/hr].
///

double Hydro :: __getMaximumFlowm3hr(void)
{
    double coefficient = 0;
    
    switch (this->turbine_type) {
        case (HydroTurbineType :: HYDRO_TURBINE_PELTON): {
            coefficient = 1.166301;
            
            break;
        }
        
        case (HydroTurbineType :: HYDRO_TURBINE_FRANCIS): {
            coefficient = 1.1952933;
            
            break;
        }
        
        default: {
            //..
            
            break;
        }
    }
    
    double maximum_flow_m3hr = (1000 * 3600 * coefficient * this->capacity_kW) /
                (this->fluid_density_kgm3 * 9.81 * this->net_head_m);
    
    return maximum_flow_m3hr;
}   /* __getMaximumFlowm3hr() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __flowToPower(double flow_m3hr)
///
/// \brief Helper method to translate a given flow into a corresponding power output.
///
/// This model was obtained by way of surveying an assortment of published hydroeletric 
/// operational data, and then constructing a best fit model.
///
/// Ref: \cite Marks_2007\n
///
/// \return The power output [kW] corresponding to a given flow [m3/hr].
///

double Hydro :: __flowToPower(double flow_m3hr)
{
    if (flow_m3hr <= 0) {
        return 0;
    }
    
    //  1. compute power ratio
    double power_ratio =
        this->fluid_density_kgm3 * 9.81 * this->net_head_m * (flow_m3hr / 3600);
    
    power_ratio /= 1000 * this->capacity_kW;
    
    //  2. get normalized power
    double normalized_power = 0;
    
    switch (this->turbine_type) {
        case (HydroTurbineType :: HYDRO_TURBINE_PELTON): {
            if (power_ratio <= 0.023529) {
                normalized_power = 0;
            }
            else if (power_ratio >= 1.166301) {
                normalized_power = 1;
            }
            else {
                normalized_power = 0.87448308 * power_ratio - 0.02108607;
            }
            
            break;
        }
        
        case (HydroTurbineType :: HYDRO_TURBINE_FRANCIS): {
            if (power_ratio <= 0.2164706) {
                normalized_power = 0;
            }
            else if (power_ratio >= 1.1952933) {
                normalized_power = 1;
            }
            else {
                normalized_power = (
                    1.61681669 * pow(power_ratio, 0.49508545) - 0.76355563
                );
            }
            
            break;
        }
        
        default: {
            //..
            
            break;
        }
    }
    
    if (normalized_power < 0) {
        normalized_power = 0;
    }
    else if (normalized_power > 1) {
        normalized_power = 1;
    }
    
    return normalized_power * this->capacity_kW;
}   /* __flowToPower() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __powerToFlow(double power_kW)
///
/// \brief Helper method to translate a given power output into a corresponding flow.
///
/// This model was obtained by way of surveying an assortment of published hydroeletric 
/// operational data, and then constructing a best fit model.
///
/// Ref: \cite Marks_2007\n
///
/// \return
///

double Hydro :: __powerToFlow(double power_kW)
{
    if (power_kW <= 0) {
        return 0;
    }
    
    double flow_m3hr = 0;
    
    switch (this->turbine_type) {
        case (HydroTurbineType :: HYDRO_TURBINE_PELTON): {
            flow_m3hr = 3600.0 / 0.87448308;
            flow_m3hr *= (power_kW / this->capacity_kW) + 0.02108607;
            flow_m3hr *= 1000 * this->capacity_kW;
            flow_m3hr /= this->fluid_density_kgm3 * 9.81 * this->net_head_m;
            
            break;
        }
        
        case (HydroTurbineType :: HYDRO_TURBINE_FRANCIS): {
            flow_m3hr = pow(
                (1.0 / 1.61681669) * ((power_kW / this->capacity_kW) + 0.76355563),
                1.0 / 0.49508545
            );
            flow_m3hr *= 3600 * 1000 * this->capacity_kW;
            flow_m3hr /= this->fluid_density_kgm3 * 9.81 * this->net_head_m;
            
            break;
        }
        
        default: {
            //..
            
            break;
        }
    }
    
    return flow_m3hr;
}   /* __powerToFlow() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Hydro :: __getAvailableFlow(double dt_hrs, double hydro_resource_m3hr)
///
/// \brief Helper method to determine what flow is currently available through the
///     turbine.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param hydro_resource_m3hr The currently available hydro flow resource [m3/hr].
///
/// \return The flow [m3/hr] currently available through the turbine.
///

double Hydro :: __getAvailableFlow(double dt_hrs, double hydro_resource_m3hr)
{
    double flow_m3hr = 0;
    
    //  1. add flow available from reservoir
    flow_m3hr += this->stored_volume_m3 / dt_hrs;
    
    //  2. add flow available from resource
    flow_m3hr += hydro_resource_m3hr;
    
    //  3. cap at maximum flow
    if (flow_m3hr > this->maximum_flow_m3hr) {
        flow_m3hr = this->maximum_flow_m3hr;
    }
    
    return flow_m3hr;
}   /* __getAvailableFlow() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Hydro :: __updateState(
///         int timestep,
///         double dt_hrs,
///         double production_kW,
///         double hydro_resource_m3hr
///     )
///
/// \brief Helper method to update and log flow and reservoir state.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param production_kW The production [kW] of the asset in this timestep.
///
/// \param hydro_resource_m3hr The currently available hydro flow resource [m3/hr].
///

void Hydro :: __updateState(
    int timestep,
    double dt_hrs,
    double production_kW,
    double hydro_resource_m3hr
)
{
    //  1. get flow, log
    double flow_m3hr = this->__powerToFlow(production_kW);
    this->turbine_flow_vec_m3hr[timestep] = flow_m3hr;
    
    //  2. update reservoir state, log
    if (this->reservoir_capacity_m3 > 0) {
        this->stored_volume_m3 += hydro_resource_m3hr * dt_hrs;
        this->stored_volume_m3 -= flow_m3hr * dt_hrs;
        
        if (this->stored_volume_m3 < 0) {
            this->stored_volume_m3 = 0;
        }
        
        else if (this->stored_volume_m3 > this->reservoir_capacity_m3) {
            this->stored_volume_m3 = this->reservoir_capacity_m3;
        }
        
        this->stored_volume_vec_m3[timestep] = this->stored_volume_m3;
    }
    
    return;
}   /* __updateState() */

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
    
    ofs << "Fluid Density: " << this->fluid_density_kgm3 << " kg/m3  \n";
    ofs << "Net Head: " << this->net_head_m << " m  \n";
    ofs << "\n";
    
    ofs << "Reservoir Volume: " << this->reservoir_capacity_m3 << " m3  \n";
    ofs << "Reservoir Initial State: " << this->init_reservoir_state << "  \n";
    ofs << "\n";
    
    ofs << "Turbine Type: ";
    switch(this->turbine_type) {
        case(HydroTurbineType :: HYDRO_TURBINE_PELTON): {
            ofs << "PELTON";
            
            break;
        }
        
        case(HydroTurbineType :: HYDRO_TURBINE_FRANCIS): {
            ofs << "FRANCIS";
            
            break;
        }
        
        default: {
            // write nothing!
            
            break;
        }
    }
    ofs << "  \n";
    ofs << "Minimum Flow: " << this->minimum_flow_m3hr << " m3/hr  \n";
    ofs << "Maximum Flow: " << this->maximum_flow_m3hr << " m3/hr  \n";
    ofs << "\n";
    
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
    ofs << "Turbine Flow [m3/hr],";
    ofs << "Stored Volume [m3],";
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
        ofs << this->turbine_flow_vec_m3hr[i] << ",";
        ofs << this->stored_volume_vec_m3[i] << ",";
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
    
    this->turbine_type = hydro_inputs.turbine_type;
    
    this->fluid_density_kgm3 = hydro_inputs.fluid_density_kgm3;
    this->net_head_m = hydro_inputs.net_head_m;
    
    this->reservoir_capacity_m3 = hydro_inputs.reservoir_capacity_m3;
    this->init_reservoir_state = hydro_inputs.init_reservoir_state;
    this->stored_volume_m3 =
        hydro_inputs.init_reservoir_state * hydro_inputs.reservoir_capacity_m3;
    
    this->minimum_flow_m3hr = this->__getMinimumFlowm3hr();
    this->maximum_flow_m3hr = this->__getMaximumFlowm3hr();
    
    this->turbine_flow_vec_m3hr.resize(this->n_points, 0);
    this->stored_volume_vec_m3.resize(this->n_points, 0);
    
    if (hydro_inputs.capital_cost < 0) {
        this->capital_cost = this->__getGenericCapitalCost();
    }
    
    if (hydro_inputs.operation_maintenance_cost_kWh < 0) {
        this->operation_maintenance_cost_kWh = this->__getGenericOpMaintCost();
    }
    
    if (not this->is_sunk) {
        this->capital_cost_vec[0] = this->capital_cost;
    }
    
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
    
    //  2. set flow to available
    double flow_m3hr = this->__getAvailableFlow(dt_hrs, hydro_resource_m3hr);
    
    if (flow_m3hr < this->minimum_flow_m3hr) {
        return 0;
    }
    
    //  3. limit flow to request (and max)
    double request_m3hr = this->__powerToFlow(request_kW);
    
    if (flow_m3hr > request_m3hr) {
        flow_m3hr = request_m3hr;
    }
    
    if (flow_m3hr > this->maximum_flow_m3hr) {
        flow_m3hr = this->maximum_flow_m3hr;
    }
    
    //  4. map flow to production
    double production_kW = this->__flowToPower(flow_m3hr);
    
    //  5. limit production to capacity
    if (production_kW > this->capacity_kW) {
        production_kW = this->capacity_kW;
    }
    
    return production_kW;
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
    double load_kW,
    double hydro_resource_m3hr
)
{
    //  1. invoke base class method
    load_kW = Noncombustion :: commit(
        timestep,
        dt_hrs,
        production_kW,
        load_kW
    );
    
    //  2. update state and record
    this->__updateState(
        timestep,
        dt_hrs,
        production_kW,
        hydro_resource_m3hr
    );
    
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
