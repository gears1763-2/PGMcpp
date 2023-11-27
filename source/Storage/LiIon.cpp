/*
 *  PGMcpp : PRIMED Grid LiIonling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file LiIon.cpp
///
/// \brief Implementation file for the LiIon class.
///
/// A derived class of Storage which models energy storage by way of lithium-ion
/// batteries.
///


#include "../../header/Storage/LiIon.h"


// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __checkInputs(LiIonInputs liion_inputs)
///
/// \brief Helper method to check inputs to the LiIon constructor.
///
/// \param liion_inputs A structure of LiIon constructor inputs.
///

void LiIon :: __checkInputs(LiIonInputs liion_inputs)
{
    //  1. check replace_SOH
    if (liion_inputs.replace_SOH < 0 or liion_inputs.replace_SOH > 1) {
        std::string error_str = "ERROR:  LiIon():  replace_SOH must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  2. check init_SOC
    if (liion_inputs.init_SOC < 0 or liion_inputs.init_SOC > 1) {
        std::string error_str = "ERROR:  LiIon():  init_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. check min_SOC
    if (liion_inputs.min_SOC < 0 or liion_inputs.min_SOC > 1) {
        std::string error_str = "ERROR:  LiIon():  min_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  4. check hysteresis_SOC
    if (liion_inputs.hysteresis_SOC < 0 or liion_inputs.hysteresis_SOC > 1) {
        std::string error_str = "ERROR:  LiIon():  hysteresis_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  5. check max_SOC
    if (liion_inputs.max_SOC < 0 or liion_inputs.max_SOC > 1) {
        std::string error_str = "ERROR:  LiIon():  max_SOC must be in the closed ";
        error_str += "interval [0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  6. check charging_efficiency
    if (liion_inputs.charging_efficiency <= 0 or liion_inputs.charging_efficiency > 1) {
        std::string error_str = "ERROR:  LiIon():  charging_efficiency must be in the ";
        error_str += "half-open interval (0, 1]";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  7. check discharging_efficiency
    if (liion_inputs.discharging_efficiency <= 0 or liion_inputs.discharging_efficiency > 1) {
        std::string error_str = "ERROR:  LiIon():  discharging_efficiency must be in the ";
        error_str += "half-open interval (0, 1]";
        
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
/// \fn void LiIon :: __toggleDepleted(void)
///
/// \brief Helper method to toggle the is_depleted attribute of LiIon.
///

void LiIon :: __toggleDepleted(void)
{
    if (this->is_depleted) {
        double hysteresis_charge_kWh = this->hysteresis_SOC * this->capacity_kWh;
    
        if (hysteresis_charge_kWh > this->dynamic_capacity_kWh) {
            hysteresis_charge_kWh = this->dynamic_capacity_kWh;
        }
        
        if (this->charge_kWh >= hysteresis_charge_kWh) {
            this->is_depleted = false;
        }
    }
    
    else {
        double min_charge_kWh = this->min_SOC * this->capacity_kWh;
        
        if (this->charge_kWh <= min_charge_kWh) {
            this->is_depleted = true;
        }
    }
    
    return;
}   /* __toggleDepleted() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __writeSummary(std::string write_path)
///
/// \brief Helper method to write summary results for LiIon.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///

void LiIon :: __writeSummary(std::string write_path)
{
    //  1. create filestream
    write_path += "summary_results.md";
    std::ofstream ofs;
    ofs.open(write_path, std::ofstream::out);
    
    //  2. write summary results (markdown)
    ofs << "# ";
    ofs << std::to_string(int(ceil(this->capacity_kW)));
    ofs << " kW ";
    ofs << std::to_string(int(ceil(this->capacity_kWh)));
    ofs << " kWh LIION Summary Results\n";
    ofs << "\n--------\n\n";
    
    //  2.1. Storage attributes
    ofs << "## Storage Attributes\n";
    ofs << "\n";
    //...
    
    ofs << "\n--------\n\n";
    
    //  2.2. LiIon attributes
    ofs << "## LiIon Attributes\n";
    ofs << "\n";
    //...
    
    ofs << "\n--------\n\n";
    
    //  2.3. LiIon Results
    ofs << "## Results\n";
    ofs << "\n";
    //...
    
    ofs << "\n--------\n\n";
    ofs.close();
    return;
}   /* __writeSummary() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: __writeTimeSeries(
///         std::string write_path,
///         std::vector<double>* time_vec_hrs_ptr,
///         int max_lines
///     )
///
/// \brief Helper method to write time series results for LiIon.
///
/// \param write_path A path (either relative or absolute) to the directory location 
///     where results are to be written. If already exists, will overwrite.
///
/// \param time_vec_hrs_ptr A pointer to the time_vec_hrs attribute of the ElectricalLoad.
///
/// \param max_lines The maximum number of lines of output to write.
///

void LiIon :: __writeTimeSeries(
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
    /*
    ofs << "LiIon Resource [kW/m2],";
    ofs << "Production [kW],";
    ofs << "Dispatch [kW],";
    ofs << "Storage [kW],";
    ofs << "Curtailment [kW],";
    ofs << "Capital Cost (actual),";
    ofs << "Operation and Maintenance Cost (actual),";
    */
    ofs << "\n";
    
    for (int i = 0; i < max_lines; i++) {
        ofs << time_vec_hrs_ptr->at(i) << ",";
        /*
        ofs << resource_map_1D_ptr->at(this->resource_key)[i] << ",";
        ofs << this->production_vec_kW[i] << ",";
        ofs << this->dispatch_vec_kW[i] << ",";
        ofs << this->storage_vec_kW[i] << ",";
        ofs << this->curtailment_vec_kW[i] << ",";
        ofs << this->capital_cost_vec[i] << ",";
        ofs << this->operation_maintenance_cost_vec[i] << ",";
        */
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
/// \fn LiIon :: LiIon(void)
///
/// \brief Constructor (dummy) for the LiIon class.
///

LiIon :: LiIon(void)
{
    return;
}   /* LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn LiIon :: LiIon(
///         int n_points,
///         double n_years,
///         LiIonInputs liion_inputs
///     )
///
/// \brief Constructor (intended) for the LiIon class.
///
/// \param n_points The number of points in the modelling time series.
///
/// \param n_years The number of years being modelled.
///
/// \param liion_inputs A structure of LiIon constructor inputs.
///

LiIon :: LiIon(
    int n_points,
    double n_years,
    LiIonInputs liion_inputs
) :
Storage(
    n_points,
    n_years,
    liion_inputs.storage_inputs
)
{
    //  1. check inputs
    this->__checkInputs(liion_inputs);
    
    //  2. set attributes
    this->type = StorageType :: LIION;
    this->type_str = "LiIon";
    
    this->dynamic_capacity_kWh = this->capacity_kWh;
    this->SOH = 1;
    this->replace_SOH = liion_inputs.replace_SOH;
    
    this->init_SOC = liion_inputs.init_SOC;
    this->charge_kWh = this->init_SOC * this->capacity_kWh;

    this->min_SOC = liion_inputs.min_SOC;
    this->hysteresis_SOC = liion_inputs.hysteresis_SOC;
    this->max_SOC = liion_inputs.max_SOC;
    
    this->charging_efficiency = liion_inputs.charging_efficiency;
    this->discharging_efficiency = liion_inputs.discharging_efficiency;
    
    //  3. construction print
    if (this->print_flag) {
        std::cout << "LiIon object constructed at " << this << std::endl;
    }
    
    return;
}   /* LiIon() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: handleReplacement(int timestep)
///
/// \brief Method to handle asset replacement and capital cost incursion,
///     if applicable.
///
/// \param timestep The current time step of the Model run.
///

void LiIon :: handleReplacement(int timestep)
{
    //  1. reset attributes
    this->dynamic_capacity_kWh = this->capacity_kWh;
    this->SOH = 1;
    
    // 2. invoke base class method
    Storage::handleReplacement(timestep);
    
    //  3. correct attributes
    this->charge_kWh = this->init_SOC * this->capacity_kWh;
    this->is_depleted = false;
    
    return;
}   /* __handleReplacement() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: getAvailablekW(double dt_hrs)
///
/// \brief Method to get the discharge power currently available from the asset.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \return The discharging power [kW] currently available from the asset.
///

double LiIon :: getAvailablekW(double dt_hrs)
{
    //  1. get min charge
    double min_charge_kWh = this->min_SOC * this->capacity_kWh;
    
    //  2. compute available power
    //     (accounting for the power currently being charged/discharged by the asset)
    double available_kW =
        ((this->charge_kWh - min_charge_kWh) * this->discharging_efficiency) /
        dt_hrs;
    
    available_kW -= this->power_kW;
    
    if (available_kW <= 0) {
        return 0;
    }
    
    //  3. apply power constraint
    if (available_kW > this->capacity_kW) {
        available_kW = this->capacity_kW;
    }
    
    return available_kW;
}   /* getAvailablekW() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double LiIon :: getAcceptablekW(double dt_hrs)
///
/// \brief Method to get the charge power currently acceptable by the asset.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \return The charging power [kW] currently acceptable by the asset.
///

double LiIon :: getAcceptablekW(double dt_hrs)
{
    //  1. get max charge
    double max_charge_kWh = this->max_SOC * this->capacity_kWh;
    
    if (max_charge_kWh > this->dynamic_capacity_kWh) {
        max_charge_kWh = this->dynamic_capacity_kWh;
    }
    
    //  2. compute acceptable power
    //     (accounting for the power currently being charged/discharged by the asset)
    double acceptable_kW = 
        (max_charge_kWh - this->charge_kWh) /
        (this->charging_efficiency * dt_hrs);
    
    acceptable_kW -= this->power_kW;
    
    if (acceptable_kW <= 0) {
        return 0;
    }
    
    //  3. apply power constraint
    if (acceptable_kW > this->capacity_kW) {
        acceptable_kW = this->capacity_kW;
    }
    
    return acceptable_kW;
}   /* getAcceptablekW( */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void LiIon :: commitCharge(
///         int timestep,
///         double dt_hrs,
///         double charge_kW
///     )
///
/// \brief Method which takes in the charging power for the current timestep and
///     records.
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param charging_kW The charging power [kw] being sent to the asset.
///

void LiIon :: commitCharge(
    int timestep,
    double dt_hrs,
    double charging_kW
)
{
    //  1. record charging power
    this->charging_power_vec_kW[timestep] = charging_kW;
    
    //  2. update charge and record
    this->charge_kWh += this->charging_efficiency * charging_kW * dt_hrs;
    this->charge_vec_kWh[timestep] = this->charge_kWh;
    
    //  3. toggle depleted flag (if applicable)
    this->__toggleDepleted();
    
    //  4. model degradation
    //...
    
    //  5. trigger replacement (if applicable)
    if (this->SOH <= this->replace_SOH) {
        this->handleReplacement(timestep);
    }
    
    this->power_kW= 0;
    return;
}   /* commitCharge() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///
/// \brief 
///
/// \param timestep The timestep (i.e., time series index) for the request.
///
/// \param dt_hrs The interval of time [hrs] associated with the timestep.
///
/// \param discharging_kW The discharging power [kw] being drawn from the asset.
///
/// \param load_kW The load [kW] passed to the asset in this timestep.
///
/// \return The load [kW] remaining after the discharge is deducted from it.
///

double LiIon :: commitDischarge(
    int timestep,
    double dt_hrs,
    double discharging_kW,
    double load_kW
)
{
    //  1. record discharging power
    this->discharging_power_vec_kW[timestep] = discharging_kW;
    
    //  2. update charge and record
    this->charge_kWh -= (discharging_kW * dt_hrs) / this->discharging_efficiency;
    this->charge_vec_kWh[timestep] = this->charge_kWh;
    
    //  3. update load
    load_kW -= discharging_kW;
    
    //  4. toggle depleted flag (if applicable)
    this->__toggleDepleted();
    
    //  5. model degradation
    //...
    
    //  6. trigger replacement (if applicable)
    if (this->SOH <= this->replace_SOH) {
        this->handleReplacement(timestep);
    }
    
    this->power_kW = 0;
    return load_kW;
}   /* commitDischarge() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn LiIon :: ~LiIon(void)
///
/// \brief Destructor for the LiIon class.
///

LiIon :: ~LiIon(void)
{
    //  1. destruction print
    if (this->print_flag) {
        std::cout << "LiIon object at " << this << " destroyed" << std::endl;
    }
    
    return;
}   /* ~LiIon() */

// ---------------------------------------------------------------------------------- //

// ======== PUBLIC ================================================================== //
