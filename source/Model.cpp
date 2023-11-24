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
/// \file Model.cpp
///
/// \brief Implementation file for the Model class.
///
/// A container class which forms the centre of PGMcpp. The Model class is
/// intended to serve as the primary user interface with the functionality of PGMcpp,
/// and as such it contains all other classes.
///


#include "../header/Model.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: __checkInputs(ModelInputs)
///
/// \brief Helper method (private) to check inputs to the Model constructor.
///
/// \param model_inputs A structure of Model constructor inputs.
///

void Model :: __checkInputs(ModelInputs)
{
    //...
    
    return;
}   /* __checkInputs() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: __computeFuelAndEmissions(void)
///
/// \brief Helper method to compute the total fuel consumption and emissions over the
///     Model run.
///

void Model :: __computeFuelAndEmissions(void)
{
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        this->combustion_ptr_vec[i]->computeFuelAndEmissions();
        
        this->total_fuel_consumed_L +=
            this->combustion_ptr_vec[i]->total_fuel_consumed_L;
        
        this->total_emissions.CO2_kg += 
            this->combustion_ptr_vec[i]->total_emissions.CO2_kg;
            
        this->total_emissions.CO_kg += 
            this->combustion_ptr_vec[i]->total_emissions.CO_kg;
            
        this->total_emissions.NOx_kg += 
            this->combustion_ptr_vec[i]->total_emissions.NOx_kg;
            
        this->total_emissions.SOx_kg += 
            this->combustion_ptr_vec[i]->total_emissions.SOx_kg;
            
        this->total_emissions.CH4_kg += 
            this->combustion_ptr_vec[i]->total_emissions.CH4_kg;
            
        this->total_emissions.PM_kg += 
            this->combustion_ptr_vec[i]->total_emissions.PM_kg;
    }
    
    return;
}   /* __computeFuelAndEmissions() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: __computeNetPresentCost(void)
///
/// \brief Helper method to compute the overall net present cost, for the Model
///     run, from the asset-wise net present costs.
///

void Model :: __computeNetPresentCost(void)
{
    //  1. account for Combustion economics in net present cost
    //     increment total dispatch
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        this->combustion_ptr_vec[i]->computeEconomics(
            &(this->electrical_load.time_vec_hrs)
        );
        
        this->net_present_cost += this->combustion_ptr_vec[i]->net_present_cost;
        
        this->total_dispatch_discharge_kWh +=
            this->combustion_ptr_vec[i]->total_dispatch_kWh;
    }
    
    //  2. account for Renewable economics in net present cost,
    //     increment total dispatch
    for (size_t i = 0; i < this->renewable_ptr_vec.size(); i++) {
        this->renewable_ptr_vec[i]->computeEconomics(
            &(this->electrical_load.time_vec_hrs)
        );
        
        this->net_present_cost += this->renewable_ptr_vec[i]->net_present_cost;
        
        this->total_dispatch_discharge_kWh +=
            this->renewable_ptr_vec[i]->total_dispatch_kWh;
    }
    
    //  3. account for Storage economics in net present cost
    //     increment total dispatch
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        /*
        this->storage_ptr_vec[i]->computeEconomics(
            &(this->electrical_load.time_vec_hrs)
        );
        
        this->net_present_cost += this->storage_ptr_vec[i]->net_present_cost;
        
        this->total_dispatch_discharge_kWh +=
            this->storage_ptr_vec[i]->total_discharge_kWh;
        */
    }
    
    return;
}   /* __computeNetPresentCost() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: __computeLevellizedCostOfEnergy(void)
///
/// \brief Helper method to compute the overall levellized cost of energy, for the Model
///     run, from the asset-wise levellized costs of energy.
///

void Model :: __computeLevellizedCostOfEnergy(void)
{
    //  1. account for Combustion economics in levellized cost of energy
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        this->levellized_cost_of_energy_kWh += 
            (
                this->combustion_ptr_vec[i]->levellized_cost_of_energy_kWh *
                this->combustion_ptr_vec[i]->total_dispatch_kWh
            ) / this->total_dispatch_discharge_kWh;
    }
    
    //  2. account for Renewable economics in levellized cost of energy
    for (size_t i = 0; i < this->renewable_ptr_vec.size(); i++) {
        this->levellized_cost_of_energy_kWh += 
            (
                this->renewable_ptr_vec[i]->levellized_cost_of_energy_kWh *
                this->renewable_ptr_vec[i]->total_dispatch_kWh
            ) / this->total_dispatch_discharge_kWh;
    }
    
    //  3. account for Storage economics in levellized cost of energy
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        /*
        this->levellized_cost_of_energy_kWh += 
            (
                this->storage_ptr_vec[i]->levellized_cost_of_energy_kWh *
                this->storage_ptr_vec[i]->total_discharge_kWh
            ) / this->total_dispatch_discharge_kWh;
        */
    }
    
    return;
}   /* __computeLevellizedCostOfEnergy() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: __computeEconomics(void)
///
/// \brief Helper method to compute key economic metrics for the Model run.
///

void Model :: __computeEconomics(void)
{
    this->__computeNetPresentCost();
    this->__computeLevellizedCostOfEnergy();
    
    return;
}   /* __computeEconomics() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for Model.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void Model :: __writeSummary(std::string write_path)
{
    //  1. create subdirectory
    write_path += "Model/";
    std::filesystem::create_directory(write_path);
    
    //  2. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  3. write summary results (markdown)
    ofs << "# Model Summary Results\n";
    ofs << "\n--------\n\n";
    
    //  3.1. ElectricalLoad
    ofs << "## Electrical Load\n";
    ofs << "\n";
    ofs << "Path: " <<
        this->electrical_load.path_2_electrical_load_time_series << "  \n";
    ofs << "Data Points: " << this->electrical_load.n_points << "  \n";
    ofs << "Years: " << this->electrical_load.n_years << "  \n";
    ofs << "Min: " << this->electrical_load.min_load_kW << " kW  \n";
    ofs << "Mean: " << this->electrical_load.mean_load_kW << " kW  \n";
    ofs << "Max: " << this->electrical_load.max_load_kW << " kW  \n";
    ofs << "\n--------\n\n";
    
    //  3.2. Controller
    ofs << "## Controller\n";
    ofs << "\n";
    ofs << "Control Mode: " << this->controller.control_string << "  \n";
    ofs << "\n--------\n\n";
    
    //  3.3. Resources (1D)
    ofs << "## 1D Renewable Resources\n";
    ofs << "\n";
    
    std::map<int, std::string>::iterator string_map_1D_iter =
        this->resources.string_map_1D.begin();
    std::map<int, std::string>::iterator path_map_1D_iter =
        this->resources.path_map_1D.begin();
    
    while (
        string_map_1D_iter != this->resources.string_map_1D.end() and
        path_map_1D_iter != this->resources.path_map_1D.end()
    ) {
        ofs << "Resource Key: " << string_map_1D_iter->first << "  \n";
        ofs << "Type: " << string_map_1D_iter->second << "  \n";
        ofs << "Path: " << path_map_1D_iter->second << "  \n";
        ofs << "\n";
        
        string_map_1D_iter++;
        path_map_1D_iter++;
    }
    
    ofs << "\n--------\n\n";
    
    //  3.4. Resources (2D)
    ofs << "## 2D Renewable Resources\n";
    ofs << "\n";
    
    std::map<int, std::string>::iterator string_map_2D_iter =
        this->resources.string_map_2D.begin();
    std::map<int, std::string>::iterator path_map_2D_iter =
        this->resources.path_map_2D.begin();
    
    while (
        string_map_2D_iter != this->resources.string_map_2D.end() and
        path_map_2D_iter != this->resources.path_map_2D.end()
    ) {
        ofs << "Resource Key: " << string_map_2D_iter->first << "  \n";
        ofs << "Type: " << string_map_2D_iter->second << "  \n";
        ofs << "Path: " << path_map_2D_iter->second << "  \n";
        ofs << "\n";
        
        string_map_2D_iter++;
        path_map_2D_iter++;
    }
    
    ofs << "\n--------\n\n";
    
    //  3.5. Combustion
    ofs << "## Combustion Assets\n";
    ofs << "\n";
    
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        ofs << "Asset Index: " << i << "  \n";
        ofs << "Type: " << this->combustion_ptr_vec[i]->type_str << "  \n";
        ofs << "Capacity: " << this->combustion_ptr_vec[i]->capacity_kW << " kW  \n";
        ofs << "\n";
    }
    
    ofs << "\n--------\n\n";
    
    //  3.6. Renewable
    ofs << "## Renewable Assets\n";
    ofs << "\n";
    
    for (size_t i = 0; i < this->renewable_ptr_vec.size(); i++) {
        ofs << "Asset Index: " << i << "  \n";
        ofs << "Type: " << this->renewable_ptr_vec[i]->type_str << "  \n";
        ofs << "Capacity: " << this->renewable_ptr_vec[i]->capacity_kW << " kW  \n";
        ofs << "\n";
    }
    
    ofs << "\n--------\n\n";
    
    //  3.7. Storage
    ofs << "## Storage Assets\n";
    ofs << "\n";
    
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        //...
    }
    
    ofs << "\n--------\n\n";
    
    //  3.8. Model Results
    ofs << "## Results\n";
    ofs << "\n";
    
    ofs << "Net Present Cost: " << this->net_present_cost << "  \n";
    ofs << "\n";
    
    ofs << "Total Dispatch + Discharge: " << this->total_dispatch_discharge_kWh
        << " kWh  \n";
        
    ofs << "Levellized Cost of Energy: " << this->levellized_cost_of_energy_kWh
        << " per kWh dispatched/discharged  \n";
    ofs << "\n";
    
    ofs << "Total Fuel Consumed: " << this->total_fuel_consumed_L << " L "
        << "(Annual Average: " <<
            this->total_fuel_consumed_L / this->electrical_load.n_years
        << " L/yr)  \n";
    ofs << "\n";
    
    ofs << "Total Carbon Dioxide (CO2) Emissions: " <<
        this->total_emissions.CO2_kg << " kg "
        << "(Annual Average: " << 
            this->total_emissions.CO2_kg / this->electrical_load.n_years
        << " kg/yr)  \n";
        
    ofs << "Total Carbon Monoxide (CO) Emissions: " <<
        this->total_emissions.CO_kg << " kg "
        << "(Annual Average: " << 
            this->total_emissions.CO_kg / this->electrical_load.n_years
        << " kg/yr)  \n";
        
    ofs << "Total Nitrogen Oxides (NOx) Emissions: " <<
        this->total_emissions.NOx_kg << " kg "
        << "(Annual Average: " << 
            this->total_emissions.NOx_kg / this->electrical_load.n_years
        << " kg/yr)  \n";
        
    ofs << "Total Sulfur Oxides (SOx) Emissions: " <<
        this->total_emissions.SOx_kg << " kg "
        << "(Annual Average: " << 
            this->total_emissions.SOx_kg / this->electrical_load.n_years
        << " kg/yr)  \n";
        
    ofs << "Total Methane (CH4) Emissions: " << this->total_emissions.CH4_kg << " kg "
        << "(Annual Average: " << 
            this->total_emissions.CH4_kg / this->electrical_load.n_years
        << " kg/yr)  \n";
        
    ofs << "Total Particulate Matter (PM) Emissions: " <<
        this->total_emissions.PM_kg << " kg "
        << "(Annual Average: " << 
            this->total_emissions.PM_kg / this->electrical_load.n_years
        << " kg/yr)  \n";
        
    ofs << "\n--------\n\n";
    
    ofs.close();
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: __writeTimeSeries(std::string write_path, int max_lines)
///
/// \brief Helper method to write time series results for Model.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param max_lines The maximum number of lines of output to write.
///

void Model :: __writeTimeSeries(std::string write_path, int max_lines)
{
    //  1. create filestream
    write_path += "Model/time_series_results.csv";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write time series results (comma separated value)
    ofs << "Time (since start of data) [hrs],";
    ofs << "Electrical Load [kW],";
    ofs << "Net Load [kW],";
    ofs << "Missed Load [kW],";
    ofs << "\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << this->electrical_load.time_vec_hrs[i] << ",";
        ofs << this->electrical_load.load_vec_kW[i] << ",";
        ofs << this->controller.net_load_vec_kW[i] << ",";
        ofs << this->controller.missed_load_vec_kW[i] << ",";
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
/// \fn Model :: Model(void)
///
/// \brief Constructor (dummy) for the Model class.
///

Model :: Model(void)
{
    return;
}   /* Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Model :: Model(ModelInputs model_inputs)
///
/// \brief Constructor (intended) for the Model class.
///
/// \param model_inputs A structure of Model constructor inputs.
///

Model :: Model(ModelInputs model_inputs)
{
    //  1. check inputs
    this->__checkInputs(model_inputs);
    
    //  2. read in electrical load data
    this->electrical_load.readLoadData(model_inputs.path_2_electrical_load_time_series);
    
    //  3. set control mode
    this->controller.setControlMode(model_inputs.control_mode);
    
    //  4. set public attributes
    this->total_fuel_consumed_L = 0;
    this->net_present_cost = 0;
    this->total_dispatch_discharge_kWh = 0;
    this->levellized_cost_of_energy_kWh = 0;
    
    return;
}   /* Model() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addDiesel(DieselInputs diesel_inputs)
///
/// \brief Method to add a Diesel asset to the Model.
///
/// \param diesel_inputs A structure of Diesel constructor inputs.
///

void Model :: addDiesel(DieselInputs diesel_inputs)
{
    Combustion* diesel_ptr = new Diesel(
        this->electrical_load.n_points,
        this->electrical_load.n_years,
        diesel_inputs
    );
    
    this->combustion_ptr_vec.push_back(diesel_ptr);
    
    return;
}   /* addDiesel() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addResource(
///         RenewableType renewable_type,
///         std::string path_2_resource_data,
///         int resource_key
///     )
///
/// \brief A method to add a renewable resource time series to the Model.
///
/// \param renewable_type The type of renewable resource being added to the Model.
///
/// \param path_2_resource_data A string defining the path (either relative or absolute) to the given resource time series.
///
/// \param resource_key A key used to index into the Resources object, used to associate Renewable assets with the corresponding resource.
///

void Model :: addResource(
    RenewableType renewable_type,
    std::string path_2_resource_data,
    int resource_key
)
{
    resources.addResource(
        renewable_type,
        path_2_resource_data,
        resource_key,
        &(this->electrical_load)
    );
    
    return;
}   /* addResource() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addSolar(SolarInputs solar_inputs)
///
/// \brief Method to add a Solar asset to the Model.
///
/// \param solar_inputs A structure of Solar constructor inputs.
///

void Model :: addSolar(SolarInputs solar_inputs)
{
    Renewable* solar_ptr = new Solar(
        this->electrical_load.n_points,
        this->electrical_load.n_years,
        solar_inputs
    );
    
    this->renewable_ptr_vec.push_back(solar_ptr);
    
    return;
}   /* addSolar() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addTidal(TidalInputs tidal_inputs)
///
/// \brief Method to add a Tidal asset to the Model.
///
/// \param tidal_inputs A structure of Tidal constructor inputs.
///

void Model :: addTidal(TidalInputs tidal_inputs)
{
    Renewable* tidal_ptr = new Tidal(
        this->electrical_load.n_points,
        this->electrical_load.n_years,
        tidal_inputs
    );
    
    this->renewable_ptr_vec.push_back(tidal_ptr);
    
    return;
}   /* addTidal() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addWave(WaveInputs wave_inputs)
///
/// \brief Method to add a Wave asset to the Model.
///
/// \param wave_inputs A structure of Wave constructor inputs.
///

void Model :: addWave(WaveInputs wave_inputs)
{
    Renewable* wave_ptr = new Wave(
        this->electrical_load.n_points,
        this->electrical_load.n_years,
        wave_inputs
    );
    
    this->renewable_ptr_vec.push_back(wave_ptr);
    
    return;
}   /* addWave() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: addWind(WindInputs wind_inputs)
///
/// \brief Method to add a Wind asset to the Model.
///
/// \param wind_inputs A structure of Wind constructor inputs.
///

void Model :: addWind(WindInputs wind_inputs)
{
    Renewable* wind_ptr = new Wind(
        this->electrical_load.n_points,
        this->electrical_load.n_years,
        wind_inputs
    );
    
    this->renewable_ptr_vec.push_back(wind_ptr);
    
    return;
}   /* addWind() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: run(void)
///
/// \brief A method to run the Model.
///

void Model :: run(void)
{
    // 1. init Controller
    this->controller.init(
        &(this->electrical_load),
        &(this->renewable_ptr_vec),
        &(this->resources),
        &(this->combustion_ptr_vec)
    );
    
    //  2. apply dispatch control
    this->controller.applyDispatchControl(
        &(this->electrical_load),
        &(this->combustion_ptr_vec),
        &(this->renewable_ptr_vec),
        &(this->storage_ptr_vec)
    );
    
    //  3. compute total fuel consumption and emissions
    this->__computeFuelAndEmissions();
    
    //  4. compute key economic metrics
    this->__computeEconomics();
    
    return;
}   /* run() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: reset(void)
///
/// \brief Method which resets the model for use in assessing a new candidate microgrid
///     design. This method only clears the asset pointer vectors and resets select
///     Model attribues. It leaves the Controller, ElectricalLoad, and Resources
///     objects of the Model alone.
///

void Model :: reset(void)
{
    //  1. clear combustion_ptr_vec
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        delete this->combustion_ptr_vec[i];
    }
    this->combustion_ptr_vec.clear();
    
    //  2. clear renewable_ptr_vec
    for (size_t i = 0; i < this->renewable_ptr_vec.size(); i++) {
        delete this->renewable_ptr_vec[i];
    }
    this->renewable_ptr_vec.clear();
    
    //  3. clear storage_ptr_vec
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        delete this->storage_ptr_vec[i];
    }
    this->storage_ptr_vec.clear();
    
    //  4. reset attributes
    this->total_fuel_consumed_L = 0;
    
    this->total_emissions.CO2_kg = 0;
    this->total_emissions.CO_kg = 0;
    this->total_emissions.NOx_kg = 0;
    this->total_emissions.SOx_kg = 0;
    this->total_emissions.CH4_kg = 0;
    this->total_emissions.PM_kg = 0;
    
    this->net_present_cost = 0;
    this->total_dispatch_discharge_kWh = 0;
    this->levellized_cost_of_energy_kWh = 0;
    
    return;
}   /* reset() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: clear(void)
///
/// \brief Method to clear all attributes of the Model object.
///

void Model :: clear(void)
{
    //  1. reset
    this->reset();
    
    //  2. clear components
    controller.clear();
    electrical_load.clear();
    resources.clear();
    
    return;
}   /* clear() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Model :: writeResults(
///         std::string write_path,
///         int max_lines
///     )
///
/// \brief Method which writes Model results to an output directory. Also calls out to
///     writeResults() for each contained asset.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param max_lines The maximum number of lines of output to write. If <0, then all
///     available lines are written. If =0, then only summary results are written.
///

void Model :: writeResults(
    std::string write_path,
    int max_lines
)
{
    //  1. handle sentinel
    if (max_lines < 0) {
        max_lines = this->electrical_load.n_points;
    }
    
    //  2. check for pre-existing, warn (and remove), then create
    if (write_path.back() != '/') {
        write_path += '/';
    }
    
    if (std::filesystem::is_directory(write_path)) {
        std::string warning_str = "WARNING:  Model::writeResults():  ";
        warning_str += write_path;
        warning_str += " already exists, contents will be overwritten!";
        
        std::cout << warning_str << std::endl;
        
        std::filesystem::remove_all(write_path);
    }
    
    std::filesystem::create_directory(write_path);
    
    //  3. write summary
    this->__writeSummary(write_path);
    
    //  4. write time series
    if (max_lines > this->electrical_load.n_points) {
        max_lines = this->electrical_load.n_points;
    }
    
    if (max_lines > 0) {
        this->__writeTimeSeries(write_path, max_lines);
    }
    
    //  5. call out to Combustion :: writeResults()
    for (size_t i = 0; i < this->combustion_ptr_vec.size(); i++) {
        this->combustion_ptr_vec[i]->writeResults(
            write_path,
            &(this->electrical_load.time_vec_hrs),
            i,
            max_lines
        );
    }
    
    //  6. call out to Renewable :: writeResults()
    for (size_t i = 0; i < this->renewable_ptr_vec.size(); i++) {
        this->renewable_ptr_vec[i]->writeResults(
            write_path,
            &(this->electrical_load.time_vec_hrs),
            &(this->resources.resource_map_1D),
            &(this->resources.resource_map_2D),
            i,
            max_lines
        );
    }
    
    //  7. call out to Storage :: writeResults()
    for (size_t i = 0; i < this->storage_ptr_vec.size(); i++) {
        /*
        this->storage_ptr_vec[i]->writeResults(
            write_path,
            &(this->electrical_load.time_vec_hrs),
            i,
            max_lines
        );
        */
    }
    
    return;
}   /* writeResults() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Model :: ~Model(void)
///
/// \brief Destructor for the Model class.
///

Model :: ~Model(void)
{
    this->clear();
    return;
}   /* ~Model() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
