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
/// \file Controller.cpp
///
/// \brief Implementation file for the Controller class.
///
/// A class which contains a various dispatch control logic. Intended to serve as a
/// component class of Controller.
///


#include "../header/Controller.h"



// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __computeRenewableProduction(
///         ElectricalLoad* electrical_load_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         Resources* resources_ptr
///     )
///
/// \brief Helper method to compute and record Renewable production, net load.
///
/// The net load at a given point in time is defined as the load at that point in time,
/// minus the sum of all Renewable production at that point in time. Therefore, a
/// negative net load indicates a surplus of Renewable production, and a positive
/// net load indicates a deficit of Renewable production.
///
/// \param electrical_load_ptr A pointer to the ElectricalLoad component of the Model.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of the Model.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///

void Controller :: __computeRenewableProduction(
    ElectricalLoad* electrical_load_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    Resources* resources_ptr
)
{
    double dt_hrs = 0;
    double load_kW = 0;
    double net_load_kW = 0;
    double production_kW = 0;
    
    Renewable* renewable_ptr;
    
    for (int timestep = 0; timestep < electrical_load_ptr->n_points; timestep++) {
        dt_hrs = electrical_load_ptr->dt_vec_hrs[timestep];
        load_kW = electrical_load_ptr->load_vec_kW[timestep];
        net_load_kW = load_kW;
        
        for (size_t asset = 0; asset < renewable_ptr_vec_ptr->size(); asset++) {
            renewable_ptr = renewable_ptr_vec_ptr->at(asset);
            
            production_kW = this->__getRenewableProduction(
                timestep,
                dt_hrs,
                renewable_ptr,
                resources_ptr
            );
            
            renewable_ptr->production_vec_kW[timestep] = production_kW;
        
            net_load_kW -= production_kW;
        }
        
        this->net_load_vec_kW[timestep] = net_load_kW;
    }
    
    return;
}   /* __computeRenewableProduction() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __constructCombustionMap(
///         std::vector<Combustion*>* combustion_ptr_vec_ptr
///     )
///
/// \brief Helper method to construct a Combustion map, for use in determining
//      the optimal dispatch of Combustion assets in each time step of the Model run.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///

void Controller :: __constructCombustionMap(
    std::vector<Combustion*>* combustion_ptr_vec_ptr
)
{
    std::string print_str = "Controller :: __constructCombustionMap()  ";
    print_str += "constructing combustion map (dispatch)  ";
    
    //  1. get state table dimensions
    unsigned int n_cols = combustion_ptr_vec_ptr->size();
    unsigned long int n_rows = pow(2, n_cols);
    
    //  2. walk through all possible operating states (on/off) and populate combustion
    //     map, keeping only states with minimum number of assets running.
    for (unsigned long int row = 0; row < n_rows; row++) {
        std::vector<bool> state_vec(n_cols, false);
        
        unsigned int asset_count = 0;
        unsigned long int x = row;
        double total_capacity_kW = 0;
        
        for (unsigned int i = 0; i < n_cols; i++) {
            if (x <= 0) {
                break;
            }
            
            if (x % 2 != 0) {
                state_vec[i] = true;
                total_capacity_kW += combustion_ptr_vec_ptr->at(i)->capacity_kW;
                asset_count++;
            }
            
            x /= 2;
        }
        
        if (this->combustion_map.count(total_capacity_kW) == 0) {
            this->combustion_map[total_capacity_kW] = state_vec;
        }
        
        else {
            unsigned int incumbent_asset_count = 0;
            
            for (unsigned int i = 0; i < n_cols; i++) {
                if (this->combustion_map[total_capacity_kW][i]) {
                    incumbent_asset_count++;
                }
            }
            
            if (asset_count < incumbent_asset_count) {
                this->combustion_map[total_capacity_kW] = state_vec;
            }
        }
        
        if (n_cols >= 14) {
            std::cout << print_str << row + 1 << " / " << n_rows << "\r";
        }
    }
    
    if (n_cols >= 14) {
        std::cout << print_str << n_rows << " / " << n_rows << "  DONE" << std::endl;
    }
    
    //  3. sort combustion map by key value (ascending order)
    /*
     *  Not necessary, since std::map is automatically sorted by key value on insertion.
     *  See https://en.cppreference.com/w/cpp/container/map, namely "std::map is a
     *  sorted associative container that contains key-value pairs with unique keys.
     *  Keys are sorted by using the comparison function Compare."
     */
    
    /*
    // ==== TEST PRINT ==== //
    std::cout << std::endl << std::endl;
    
    std::cout << "\t\t";
    for (size_t i = 0; i < combustion_ptr_vec_ptr->size(); i++) {
        std::cout << combustion_ptr_vec_ptr->at(i)->capacity_kW << "\t";
    }
    std::cout << std::endl;
    
    std::map<double, std::vector<bool>>::iterator iter;
    for (
        iter = this->combustion_map.begin();
        iter != this->combustion_map.end();
        iter++
    ) {
        std::cout << iter->first << ":\t{\t";
        
        for (size_t i = 0; i < iter->second.size(); i++) {
            std::cout << iter->second[i] << "\t";
        }
        std::cout << "}" << std::endl;
    }
    
    
    // ==== END TEST PRINT ==== //
    */
    
    return;
}   /* __constructCombustionTable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Controller :: __getRenewableProduction(
///         int timestep,
///         double dt_hrs,
///         Renewable* renewable_ptr,
///         Resources* resources_ptr
///     )
///
/// \brief Helper method to compute the production from the given Renewable asset at
///     the given point in time.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param renewable_ptr A pointer to the Renewable asset.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///
/// \return The production [kW] of the Renewable asset.
///

double Controller :: __getRenewableProduction(
    int timestep,
    double dt_hrs,
    Renewable* renewable_ptr,
    Resources* resources_ptr
)
{
    double production_kW = 0;
    
    switch (renewable_ptr->type) {
        case (RenewableType :: SOLAR): {
            double resource_value = 0;
            
            if (not renewable_ptr->normalized_production_series_given) {
                resource_value =
                    resources_ptr->resource_map_1D[renewable_ptr->resource_key][timestep];
            }
            
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resource_value
            );
            
            break;
        }
        
        case (RenewableType :: TIDAL): {
            double resource_value = 0;
            
            if (not renewable_ptr->normalized_production_series_given) {
                resource_value =
                    resources_ptr->resource_map_1D[renewable_ptr->resource_key][timestep];
            }
            
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resource_value
            );
            
            break;
        }
        
        case (RenewableType :: WAVE): {
            double significant_wave_height_m = 0;
            double energy_period_s = 0;
            
            if (not renewable_ptr->normalized_production_series_given) {
                significant_wave_height_m =
                    resources_ptr->resource_map_2D[renewable_ptr->resource_key][timestep][0];
                
                energy_period_s =
                    resources_ptr->resource_map_2D[renewable_ptr->resource_key][timestep][1];
            }
            
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                significant_wave_height_m,
                energy_period_s
            );
            
            break;
        }
        
        case (RenewableType :: WIND): {
            double resource_value = 0;
            
            if (not renewable_ptr->normalized_production_series_given) {
                resource_value =
                    resources_ptr->resource_map_1D[renewable_ptr->resource_key][timestep];
            }
            
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resource_value
            );
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Controller::__getRenewableProduction():  ";
            error_str += "renewable type ";
            error_str += std::to_string(renewable_ptr->type);
            error_str += " not recognized";
            
            #ifdef _WIN32
                std::cout << error_str << std::endl;
            #endif

            throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return production_kW;
}   /* __getRenewableProduction() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //
/*
// **** DEPRECATED ****
//
// \fn double Controller :: __handleStorageDischarging(
//         int timestep,
//         double dt_hrs,
//         double net_load_kW,
//         std::vector<Storage*>* storage_ptr_vec_ptr
//     )
//
// \brief Helper method to handle the discharging of available Storage assets.
//
// \param timestep The current time step of the Model run.
//
// \param dt_hrs The interval of time [hrs] associated with the action.
//
// \param remaining_load_kW The load remaining [kW] before discharging.
//
// \param storage_ptr_vec_ptr A pointer to a vector of pointers to the Storage assets.
//
// \return The load [kW] remaining after the discharge is deducted from it.
//

double Controller :: __handleStorageDischarging(
    int timestep,
    double dt_hrs,
    double remaining_load_kW,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    //  1. set target discharge
    bool operating_reserve_flag = false;
    double target_discharge_kW = remaining_load_kW;
    
    if (target_discharge_kW < this->required_operating_reserve_kW) {
        target_discharge_kW = this->required_operating_reserve_kW;
        operating_reserve_flag = true;
    }
    
    //  2. immediately return on target_discharge_kW <= 0
    if (target_discharge_kW <= 0) {
        return remaining_load_kW;
    }
    
    //  3. discharge available Storage assets
    double discharging_kW = 0;
    
    Storage* storage_ptr;
    
    for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
        //  3.1. break on vanishing target_discharge_kW
        if (target_discharge_kW <= 0) {
            break;
        }
        
        //  3.2. get pointer to asset
        storage_ptr = storage_ptr_vec_ptr->at(asset);
        
        //  3.3. continue if depleted
        if (storage_ptr->is_depleted) {
            continue;
        }
        
        //  3.4. get available discharging power
        discharging_kW = storage_ptr->getAvailablekW(dt_hrs);
        
        if (discharging_kW > target_discharge_kW) {
            discharging_kW = target_discharge_kW;
        }
        
        //  3.5. commit discharging, log
        target_discharge_kW = storage_ptr->commitDischarge(
            timestep,
            dt_hrs,
            discharging_kW,
            target_discharge_kW
        );
        
        this->storage_discharge_bool_vec[asset] = true;
    }
    
    //  4. log impact of discharge
    if (operating_reserve_flag) {
        remaining_load_kW -= this->required_operating_reserve_kW - target_discharge_kW;
        this->required_operating_reserve_kW = target_discharge_kW;
    }
    
    else {
        this->required_operating_reserve_kW -= remaining_load_kW - target_discharge_kW;
        remaining_load_kW = target_discharge_kW;
    }
    
    return remaining_load_kW;
}   // __handleStorageDischarging()
*/
// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn LoadStruct Controller :: __handleStorageDischarging(
///         int timestep,
///         double dt_hrs,
///         LoadStruct load_struct,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Helper method to handle the discharging of available Storage assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param load_struct A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///
/// \param storage_ptr_vec_ptr A pointer to a vector of pointers to the Storage assets.
///
/// \return A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///

LoadStruct Controller :: __handleStorageDischarging(
    int timestep,
    double dt_hrs,
    LoadStruct load_struct,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    //  1. get total available power
    double total_available_power_kW = 0;
    std::vector<double> available_power_vec_kW(storage_ptr_vec_ptr->size(), 0);
    Storage* storage_ptr;
    
    for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
        //  1.1. get pointer
        storage_ptr = storage_ptr_vec_ptr->at(asset);
        
        //  1.2. check if depleted
        if (storage_ptr->is_depleted) {
            continue;
        }
        
        //  1.3. log available, increment total available
        available_power_vec_kW[asset] = storage_ptr->getAvailablekW(dt_hrs);
        total_available_power_kW += available_power_vec_kW[asset];
    }
    
    //  2. update spinning reserve requirement
    load_struct.required_spinning_reserve_kW -= total_available_power_kW;
    
    if (load_struct.required_spinning_reserve_kW < 0) {
        load_struct.required_spinning_reserve_kW = 0;
    }
    
    //  3. set total discharge power
    double total_discharge_power_kW = 
        load_struct.load_kW - load_struct.total_renewable_production_kW;
    
    if (total_discharge_power_kW < load_struct.required_firm_dispatch_kW) {
        total_discharge_power_kW = load_struct.required_firm_dispatch_kW;
    }
    
    if (total_discharge_power_kW > total_available_power_kW) {
        total_discharge_power_kW = total_available_power_kW;
    }
    
    //  4. update firm dispatch requirement
    load_struct.required_firm_dispatch_kW -= total_discharge_power_kW;
    
    if (load_struct.required_firm_dispatch_kW < 0) {
        load_struct.required_firm_dispatch_kW = 0;
    }
    
    //  5. commit total discharge power
    double asset_discharge_power_kW = 0;
    
    for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
        //  5.1. get pointer
        storage_ptr = storage_ptr_vec_ptr->at(asset);
        
        //  5.2. check if depleted
        if (storage_ptr->is_depleted) {
            continue;
        }
        
        //  5.3. get asset discharge power (proportional to available)
        if (total_available_power_kW <= 0) {
            asset_discharge_power_kW = 0;
        }
        
        else {
            asset_discharge_power_kW =
                (total_discharge_power_kW / total_available_power_kW) *
                available_power_vec_kW[asset];
        }
        
        //  5.4. commit discharging, log
        if (asset_discharge_power_kW > 0) {
            load_struct.load_kW = storage_ptr->commitDischarge(
                timestep,
                dt_hrs,
                asset_discharge_power_kW,
                load_struct.load_kW
            );
        
            this->storage_discharge_bool_vec[asset] = true;
        }
    }
    
    return load_struct;
}   /* __handleStorageDischarging() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //
/*
// **** DEPRECATED ****
//
// \fn double Controller :: __handleNoncombustionDispatch(
//         int timestep,
//         double dt_hrs,
//         double remaining_load_kW,
//         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
//         Resources* resources_ptr
//     )
//
// \brief Helper method to handle the dispatch of Noncombustion assets.
//
// \param timestep The current time step of the Model run.
//
// \param dt_hrs The interval of time [hrs] associated with the action.
//
// \param remaining_load_kW The load remaining [kW] before dispatch.
//
// \param noncombustion_ptr_vec_ptr A pointer to the Noncombustion pointer vector of
//     the Model.
//
// \param resources_ptr A pointer to the Resources component of the Model.
//
// \return The load [kW] remaining after the dispatch is deducted from it.
//

double Controller :: __handleNoncombustionDispatch(
    int timestep,
    double dt_hrs,
    double remaining_load_kW,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    Resources* resources_ptr
)
{
    //  1. set target dispatch
    bool operating_reserve_flag = false;
    double target_dispatch_kW = remaining_load_kW;
    
    if (target_dispatch_kW < this->required_operating_reserve_kW) {
        target_dispatch_kW = this->required_operating_reserve_kW;
        operating_reserve_flag = true;
    }
    
    if (target_dispatch_kW < 0) {
        target_dispatch_kW = 0;
    }
    
    //  2. dispatch Noncombustion assets
    Noncombustion* noncombustion_ptr;
    double production_kW = 0;
    
    for (size_t asset = 0; asset < noncombustion_ptr_vec_ptr->size(); asset++) {
       noncombustion_ptr = noncombustion_ptr_vec_ptr->at(asset);
        
        switch (noncombustion_ptr->type) {
            case (NoncombustionType :: HYDRO): {
                double resource_value = 0;
                
                if (not noncombustion_ptr->normalized_production_series_given) {
                    resource_value =
                        resources_ptr->resource_map_1D[noncombustion_ptr->resource_key][timestep];
                }
                
                production_kW = noncombustion_ptr->requestProductionkW(
                    timestep,
                    dt_hrs,
                    target_dispatch_kW,
                    resource_value
                );
                
                target_dispatch_kW = noncombustion_ptr->commit(
                    timestep,
                    dt_hrs,
                    production_kW,
                    target_dispatch_kW,
                    resource_value
                );
                
                break;
            }
            
            default: {
                production_kW = noncombustion_ptr->requestProductionkW(
                    timestep,
                    dt_hrs,
                    target_dispatch_kW
                );
                
                target_dispatch_kW = noncombustion_ptr->commit(
                    timestep,
                    dt_hrs,
                    production_kW,
                    target_dispatch_kW
                );
                
                break;
            }
        }
    }
    
    //  3. log impact of dispatch
    if (operating_reserve_flag) {
        remaining_load_kW -= this->required_operating_reserve_kW - target_dispatch_kW;
        this->required_operating_reserve_kW = target_dispatch_kW;
    }
    
    else {
        this->required_operating_reserve_kW -= remaining_load_kW - target_dispatch_kW;
        remaining_load_kW = target_dispatch_kW;
    }
    
    return remaining_load_kW;
}   // __handleNoncombustionDispatch()
*/
// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn LoadStruct Controller :: __handleNoncombustionDispatch(
///         int timestep,
///         double dt_hrs,
///         LoadStruct load_struct,
///         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
///         Resources* resources_ptr
///     )
///
/// \brief Helper method to handle the dispatch of Noncombustion assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param load_struct A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///
/// \param noncombustion_ptr_vec_ptr A pointer to the Noncombustion pointer
///     vector of the Model.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///
/// \return A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///

LoadStruct Controller :: __handleNoncombustionDispatch(
    int timestep,
    double dt_hrs,
    LoadStruct load_struct,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    Resources* resources_ptr
)
{
    //  1. get total available production
    double total_available_production_kW = 0;
    std::vector<double> available_production_vec_kW(
        noncombustion_ptr_vec_ptr->size(), 0
    );
    Noncombustion* noncombustion_ptr;
    
    for (
        size_t asset = 0; asset < noncombustion_ptr_vec_ptr->size(); asset++
    ) {
        //  1.1. get pointer
        noncombustion_ptr = noncombustion_ptr_vec_ptr->at(asset);
        
        //  1.2. log available, increment total available
        switch (noncombustion_ptr->type) {
            case (NoncombustionType :: HYDRO): {
                double resource_value = 0;
                
                if (
                    not noncombustion_ptr->normalized_production_series_given
                ) {
                    resource_value =
                        resources_ptr->resource_map_1D[
                            noncombustion_ptr->resource_key
                        ][timestep];
                }
                
                available_production_vec_kW[asset] =
                    noncombustion_ptr->requestProductionkW(
                        timestep,
                        dt_hrs,
                        noncombustion_ptr->capacity_kW,
                        resource_value
                    );
                
                total_available_production_kW +=
                    available_production_vec_kW[asset];
                
                break;
            }
            
            default: {
                available_production_vec_kW[asset] =
                    noncombustion_ptr->requestProductionkW(
                        timestep,
                        dt_hrs,
                        noncombustion_ptr->capacity_kW
                    );
                
                total_available_production_kW +=
                    available_production_vec_kW[asset];
                
                break;
            }
        }
    }
    
    //  2. update spinning reserve requirement
    load_struct.required_spinning_reserve_kW -= total_available_production_kW;
    
    if (load_struct.required_spinning_reserve_kW < 0) {
        load_struct.required_spinning_reserve_kW = 0;
    }
    
    //  3. set total production
    double total_production_kW = 
        load_struct.load_kW - load_struct.total_renewable_production_kW;
    
    if (total_production_kW < load_struct.required_firm_dispatch_kW) {
        total_production_kW = load_struct.required_firm_dispatch_kW;
    }
    
    if (total_production_kW > total_available_production_kW) {
        total_production_kW = total_available_production_kW;
    }
    
    //  4. update firm dispatch requirement
    load_struct.required_firm_dispatch_kW -= total_production_kW;
    
    if (load_struct.required_firm_dispatch_kW < 0) {
        load_struct.required_firm_dispatch_kW = 0;
    }
    
    //  5. commit total production
    double asset_production_kW = 0;
    
    for (
        size_t asset = 0; asset < noncombustion_ptr_vec_ptr->size(); asset++
    ) {
        //  5.1. get pointer
        noncombustion_ptr = noncombustion_ptr_vec_ptr->at(asset);
        
        //  5.2. get asset production (proportional to available)
        if (total_available_production_kW <= 0) {
            asset_production_kW = 0;
        }
        
        else {
            asset_production_kW =
                (total_production_kW / total_available_production_kW) *
                available_production_vec_kW[asset];
        }
        
        //  5.3. commit production, log
        switch (noncombustion_ptr->type) {
            case (NoncombustionType :: HYDRO): {
                double resource_value = 0;
                
                if (
                    not noncombustion_ptr->normalized_production_series_given
                ) {
                    resource_value =
                        resources_ptr->resource_map_1D[
                            noncombustion_ptr->resource_key
                        ][timestep];
                }
                
                load_struct.load_kW = noncombustion_ptr->commit(
                    timestep,
                    dt_hrs,
                    asset_production_kW,
                    load_struct.load_kW,
                    resource_value
                );
                
                break;
            }
            
            default: {
                load_struct.load_kW = noncombustion_ptr->commit(
                    timestep,
                    dt_hrs,
                    asset_production_kW,
                    load_struct.load_kW
                );
                
                break;
            }
        }
    }
    
    return load_struct;
}   /* __handleNoncombustionDispatch() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //
/*
// **** DEPRECATED ****
//
// \fn double Controller :: __handleCombustionDispatch(
//         int timestep,
//         double dt_hrs,
//         double load_kW,
//         double remaining_load_kW,
//         double total_renewable_production_kW,
//         double firm_renewable_production_kW,
//         std::vector<Combustion*>* combustion_ptr_vec_ptr,
//         bool is_cycle_charging
//     )
//
// \brief Helper method to handle the dispatch of Combustion assets.
//
// \param timestep The current time step of the Model run.
//
// \param dt_hrs The interval of time [hrs] associated with the action.
//
// \param load_kW The load [kW] for this timestep.
//
// \param remaining_load_kW The load remaining [kW] before dispatch.
//
// \param total_renewable_production_kW The total production [kW] from all Renewable
//     assets.
//
// \param firm_renewable_production_kW The firm production [kW] from all Renewable 
//     assets.
//
// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of
//     the Model.
//
// \param is_cycle_charging A flag which indicates whether the Combustion assets are 
//     running in cycle charging mode (true) or load following mode (false).
//
// \return The load [kW] remaining after the dispatch is deducted from it.
//

double Controller :: __handleCombustionDispatch(
    int timestep,
    double dt_hrs,
    double load_kW,
    double remaining_load_kW,
    double total_renewable_production_kW,
    double firm_renewable_production_kW,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    bool is_cycle_charging
)
{
    //  1. set target dispatch
    bool operating_reserve_flag = false;
    double target_dispatch_kW = remaining_load_kW;
    
    if (target_dispatch_kW < this->required_operating_reserve_kW) {
        target_dispatch_kW = this->required_operating_reserve_kW;
        operating_reserve_flag = true;
    }
    
    if (target_dispatch_kW < 0) {
        target_dispatch_kW = 0;
    }
    
    //  2. allocate Combustion assets
    double allocated_capacity_kW = 0;
    
    std::map<double, std::vector<bool>>::iterator iter = this->combustion_map.begin();
    
    while (iter != std::prev(this->combustion_map.end(), 1)) {
        if (target_dispatch_kW <= allocated_capacity_kW) {
            break;
        }
        
        iter++;
        allocated_capacity_kW = iter->first;
    }
    
    //  3. dispatch Combustion assets
    //     sharing load proportionally to individual rated capacities
    Combustion* combustion_ptr;
    
    double production_kW = 0;
    double request_kW = 0;
    double target_production_kW = target_dispatch_kW;
    
    for (
        size_t asset = 0;
        asset < this->combustion_map[allocated_capacity_kW].size();
        asset++
    ) {
        combustion_ptr = combustion_ptr_vec_ptr->at(asset);
        
        if (allocated_capacity_kW > 0) {
            request_kW =
                int(this->combustion_map[allocated_capacity_kW][asset]) * 
                target_production_kW *
                (combustion_ptr->capacity_kW / allocated_capacity_kW);
        }
        
        else {
            request_kW = 0;
        }
        
        if (is_cycle_charging and request_kW > 0) {
            if (request_kW < combustion_ptr->cycle_charging_setpoint * combustion_ptr->capacity_kW) {
                request_kW = combustion_ptr->cycle_charging_setpoint * combustion_ptr->capacity_kW;
            }
        }
        
        production_kW = combustion_ptr->requestProductionkW(
            timestep,
            dt_hrs,
            request_kW
        );
        
        target_dispatch_kW = combustion_ptr->commit(
            timestep,
            dt_hrs,
            production_kW,
            target_dispatch_kW
        );
    }
    
    //  4. log impact of dispatch
    if (operating_reserve_flag) {
        remaining_load_kW -= this->required_operating_reserve_kW - target_dispatch_kW;
        this->required_operating_reserve_kW = target_dispatch_kW;
    }
    
    else {
        this->required_operating_reserve_kW -= remaining_load_kW - target_dispatch_kW;
        remaining_load_kW = target_dispatch_kW;
    }
    
    return remaining_load_kW;
}   // __handleCombustionDispatch()
*/
// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn LoadStruct Controller :: __handleCombustionDispatch(
///         int timestep,
///         double dt_hrs,
///         LoadStruct load_struct,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         bool is_cycle_charging
///     )
///
/// \brief Helper method to handle the dispatch of Combustion assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param load_struct A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of
///     the Model.
///
/// \param is_cycle_charging A flag which indicates whether the Combustion assets are 
///     running in cycle charging mode (true) or load following mode (false).
///
/// \return A structure of the load remaining [kW], total renewable 
///     production [kW], required firm dispatch remaining [kW], and required
///     spinning reserve remaining [kW] after discharge.
///

LoadStruct Controller :: __handleCombustionDispatch(
    int timestep,
    double dt_hrs,
    LoadStruct load_struct,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    bool is_cycle_charging
)
{
    //  1. determine allocation
    double allocation_kW = 
        load_struct.load_kW - load_struct.total_renewable_production_kW;
    
    if (allocation_kW < load_struct.required_spinning_reserve_kW) {
        allocation_kW = load_struct.required_spinning_reserve_kW;
    }
    
    if (allocation_kW < load_struct.required_firm_dispatch_kW) {
        allocation_kW = load_struct.required_firm_dispatch_kW;
    }
    
    //  2. allocate Combustion assets
    double allocated_capacity_kW = 0;
    std::map<double, std::vector<bool>>::iterator iter =
        this->combustion_map.begin();
    
    while (iter != std::prev(this->combustion_map.end(), 1)) {
        if (allocation_kW <= allocated_capacity_kW) {
            break;
        }
        
        iter++;
        allocated_capacity_kW = iter->first;
    }
    
    //  3. update spinning reserve requirement
    load_struct.required_spinning_reserve_kW -= allocated_capacity_kW;
    
    if (load_struct.required_spinning_reserve_kW < 0) {
        load_struct.required_spinning_reserve_kW = 0;
    }
    
    //  4. set total production
    double total_production_kW = 
        load_struct.load_kW - load_struct.total_renewable_production_kW;
    
    if (total_production_kW < load_struct.required_firm_dispatch_kW) {
        total_production_kW = load_struct.required_firm_dispatch_kW;
    }
    
    if (total_production_kW > allocated_capacity_kW) {
        total_production_kW = allocated_capacity_kW;
    }
    
    //  5. update firm dispatch requirement
    load_struct.required_firm_dispatch_kW -= total_production_kW;
    
    if (load_struct.required_firm_dispatch_kW < 0) {
        load_struct.required_firm_dispatch_kW = 0;
    }
    
    //  6. commit Combustion assets
    //     sharing load proportionally to individual rated capacities
    double asset_production_kW = 0;
    Combustion* combustion_ptr;
    
    for (
        size_t asset = 0;
        asset < this->combustion_map[allocated_capacity_kW].size();
        asset++
    ) {
        //  6.1. get pointer
        combustion_ptr = combustion_ptr_vec_ptr->at(asset);
        
        //  6.2. get asset production
        if (allocated_capacity_kW <= 0) {
            asset_production_kW = 0;
        }
        
        else {
            asset_production_kW =
                int(this->combustion_map[allocated_capacity_kW][asset]) * 
                (combustion_ptr->capacity_kW / allocated_capacity_kW) *
                total_production_kW;
        }
        
        if (is_cycle_charging and asset_production_kW > 0) {
            if (
                asset_production_kW <
                    combustion_ptr->cycle_charging_setpoint *
                    combustion_ptr->capacity_kW
            ) {
                asset_production_kW =
                    combustion_ptr->cycle_charging_setpoint *
                    combustion_ptr->capacity_kW;
            }
        }
        
        //  6.3. commit production, log
        load_struct.load_kW = combustion_ptr->commit(
            timestep,
            dt_hrs,
            asset_production_kW,
            load_struct.load_kW
        );
    }
    
    return load_struct;
}   /* __handleCombustionDispatch() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Controller :: __handleRenewableDispatch(
///         int timestep,
///         double dt_hrs,
///         double remaining_load_kW,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr
///     )
///
/// \brief Helper method to handle the dispatch of Renewable assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param remaining_load_kW The load remaining [kW] before dispatch.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of
///     the Model.
///
/// \return The net load [kW] remaining after the dispatch is deducted from it.
///

double Controller :: __handleRenewableDispatch(
    int timestep,
    double dt_hrs,
    double remaining_load_kW,
    std::vector<Renewable*>* renewable_ptr_vec_ptr
)
{
    //  1. set target dispatch
    double target_dispatch_kW = remaining_load_kW;
    
    if (target_dispatch_kW < 0) {
        target_dispatch_kW = 0;
    }
    
    //  2. dispatch Renewable assets
    Renewable* renewable_ptr;
    double production_kW = 0;
    
    for (size_t asset = 0; asset < renewable_ptr_vec_ptr->size(); asset++) {
        renewable_ptr = renewable_ptr_vec_ptr->at(asset);
        
        production_kW = renewable_ptr->production_vec_kW[timestep];
        
        target_dispatch_kW = renewable_ptr->commit(
            timestep,
            dt_hrs,
            production_kW,
            target_dispatch_kW
        );
    }
    
    //  3. log impact of dispatch
    remaining_load_kW = target_dispatch_kW;
    
    return remaining_load_kW;
}   /* __handleRenewableDispatch() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __handleStorageCharging(
///         int timestep,
///         double dt_hrs,
///         std::vector<Storage*>* storage_ptr_vec_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr
///     )
///
/// \brief Helper method to handle the charging of available Storage assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param storage_ptr_vec_ptr A pointer to a vector of pointers to the Storage assets
///     that are to be charged.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///
/// \param noncombustion_ptr_vec_ptr A pointer to the Noncombustion pointer vector of
///     the Model.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of the Model.
///

void Controller :: __handleStorageCharging(
    int timestep,
    double dt_hrs,
    std::vector<Storage*>* storage_ptr_vec_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr
)
{
    double acceptable_kW = 0;
    double curtailment_kW = 0;
    
    Storage* storage_ptr;
    Combustion* combustion_ptr;
    Noncombustion* noncombustion_ptr;
    Renewable* renewable_ptr;
    
    for (
        size_t storage_asset = 0;
        storage_asset < storage_ptr_vec_ptr->size();
        storage_asset++
    ) {
        //  1. if already discharged, continue
        if (this->storage_discharge_bool_vec[storage_asset]) {
            continue;
        }
        
        //  2.  get pointer to asset
        storage_ptr = storage_ptr_vec_ptr->at(storage_asset);
        
        //  3. attempt to charge from Combustion curtailment first
        for (size_t asset = 0; asset < combustion_ptr_vec_ptr->size(); asset++) {
            combustion_ptr = combustion_ptr_vec_ptr->at(asset);
            curtailment_kW = combustion_ptr->curtailment_vec_kW[timestep];
            
            if (curtailment_kW <= 0) {
                continue;
            }
            
            acceptable_kW = storage_ptr->getAcceptablekW(dt_hrs);
            
            if (acceptable_kW > curtailment_kW) {
                acceptable_kW = curtailment_kW;
            }
            
            combustion_ptr->curtailment_vec_kW[timestep] -= acceptable_kW;
            combustion_ptr->storage_vec_kW[timestep] += acceptable_kW;
            combustion_ptr->total_stored_kWh += acceptable_kW * dt_hrs;
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  4. attempt to charge from Noncombustion curtailment second
        for (size_t asset = 0; asset < noncombustion_ptr_vec_ptr->size(); asset++) {
            noncombustion_ptr = noncombustion_ptr_vec_ptr->at(asset);
            curtailment_kW = noncombustion_ptr->curtailment_vec_kW[timestep];
            
            if (curtailment_kW <= 0) {
                continue;
            }
            
            acceptable_kW = storage_ptr->getAcceptablekW(dt_hrs);
            
            if (acceptable_kW > curtailment_kW) {
                acceptable_kW = curtailment_kW;
            }
            
            noncombustion_ptr->curtailment_vec_kW[timestep] -= acceptable_kW;
            noncombustion_ptr->storage_vec_kW[timestep] += acceptable_kW;
            noncombustion_ptr->total_stored_kWh += acceptable_kW * dt_hrs;
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  5. attempt to charge from Renewable curtailment third
        for (size_t asset = 0; asset < renewable_ptr_vec_ptr->size(); asset++) {
            renewable_ptr = renewable_ptr_vec_ptr->at(asset);
            curtailment_kW = renewable_ptr->curtailment_vec_kW[timestep];
            
            if (curtailment_kW <= 0) {
                continue;
            }
            
            acceptable_kW = storage_ptr->getAcceptablekW(dt_hrs);
            
            if (acceptable_kW > curtailment_kW) {
                acceptable_kW = curtailment_kW;
            }
            
            renewable_ptr->curtailment_vec_kW[timestep] -= acceptable_kW;
            renewable_ptr->storage_vec_kW[timestep] += acceptable_kW;
            renewable_ptr->total_stored_kWh += acceptable_kW * dt_hrs;
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  6. commit charge
        storage_ptr->commitCharge(
            timestep,
            dt_hrs,
            storage_ptr->power_kW
        );
    }
    
    return;
}   /* __handleStorageCharging() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Controller :: Controller(void)
///
/// \brief Constructor for the Controller class.
///

Controller :: Controller(void)
{
    return;
}   /* Controller() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: setControlMode(ControlMode control_mode)
///
/// \brief Method to set control mode of Controller.
///
/// \param control_mode The ControlMode which is to be active in the Controller.
///

void Controller :: setControlMode(ControlMode control_mode)
{
    this->control_mode = control_mode;
    
    switch(control_mode) {
        case (ControlMode :: LOAD_FOLLOWING): {
            this->control_string = "LOAD_FOLLOWING";
            
            break;
        }
        
        case (ControlMode :: CYCLE_CHARGING): {
            this->control_string = "CYCLE_CHARGING";
            
            break;
        }
        
        default: {
            std::string error_str = "ERROR:  Controller :: setControlMode():  ";
                error_str += "control mode ";
                error_str += std::to_string(control_mode);
                error_str += " not recognized";
                
                #ifdef _WIN32
                    std::cout << error_str << std::endl;
                #endif

                throw std::runtime_error(error_str);
            
            break;
        }
    }
    
    return;
}   /* setControlMode() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //
/*
// **** DEPRECATED ****
//
// \fn void Controller :: setLoadOperatingReserveFactor(double load_operating_reserve_factor)
//
// \brief Method to set Controller load_operating_reserve_factor attribute.
//
// \param load_operating_reserve_factor An operating reserve factor [0, 1] to cover
//     random fluctuations in load.
//

void Controller :: setLoadOperatingReserveFactor(double load_operating_reserve_factor)
{
    this->load_operating_reserve_factor = load_operating_reserve_factor;
    
    return;
}   // setLoadOperatingReserveFactor()
*/
// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //
/*
// **** DEPRECATED ****
//
// \fn void Controller :: setMaxOperatingReserveFactor(double max_operating_reserve_factor)
//
// \brief Method to set Controller max_operating_reserve_factor attribute.
//
// \param max_operating_reserve_factor An operating reserve factor [0, 1] that limits
//     the required overall operating reserve to, at most, factor * load_kW.
//

void Controller :: setMaxOperatingReserveFactor(double max_operating_reserve_factor)
{
    this->max_operating_reserve_factor = max_operating_reserve_factor;
    
    return;
}   // setMaxOperatingReserveFactor()
*/
// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: setFirmDispatchRatio(double max_operating_reserve_factor)
///
/// \brief Method to set Controller firm_dispatch_ratio attribute.
///
/// \param firm_dispatch_ratio The ratio [0, 1] of the load in each time step
///     that must be dispatched from firm assets.
///

void Controller :: setFirmDispatchRatio(double firm_dispatch_ratio)
{
    this->firm_dispatch_ratio = firm_dispatch_ratio;
    
    return;
}   /* setFirmDispatchRatio() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: setLoadReserveRatio(double max_operating_reserve_factor)
///
/// \brief Method to set Controller firm_dispatch_ratio attribute.
///
/// \param load_reserve_ratio The ratio [0, 1] of the load in each time step
///     that must be included in the required spinning reserve.
///

void Controller :: setLoadReserveRatio(double load_reserve_ratio)
{
    this->load_reserve_ratio = load_reserve_ratio;
    
    return;
}   /* setLoadReserveRatio() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
///  \fn void Controller :: init(
///         ElectricalLoad* electrical_load_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         Resources* resources_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr
///     )
///
/// \brief Method to initialize the Controller component of the Model.
///
/// \param electrical_load_ptr A pointer to the ElectricalLoad component of the Model.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of the Model.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///

void Controller :: init(
    ElectricalLoad* electrical_load_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    Resources* resources_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr
)
{
    //  1. init vector attributes
    this->net_load_vec_kW.resize(electrical_load_ptr->n_points, 0);
    this->missed_load_vec_kW.resize(electrical_load_ptr->n_points, 0);
    
    //  2. compute Renewable production
    this->__computeRenewableProduction(
        electrical_load_ptr,
        renewable_ptr_vec_ptr,
        resources_ptr
    );
    
    //  3. construct Combustion table
    this->__constructCombustionMap(combustion_ptr_vec_ptr);
    
    return;
}   /* init() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: applyDispatchControl(
///         ElectricalLoad* electrical_load_ptr,
///         Resources* resources_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Nonombustion*>* noncombustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Method to apply dispatch control at every point in the modelling time series.
///
/// \param electrical_load_ptr A pointer to the ElectricalLoad component of the Model.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///
/// \param noncombustion_ptr_vec_ptr A pointer to the Noncombustion pointer vector of
///     the Model.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of the Model.
///
/// \param storage_ptr_vec_ptr A pointer to the Storage pointer vector of the Model.
///

void Controller :: applyDispatchControl(
    ElectricalLoad* electrical_load_ptr,
    Resources* resources_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    double dt_hrs = 0;
    double load_kW = 0;
    double required_firm_dispatch_kW = 0;
    double total_renewable_production_kW = 0;
    double required_spinning_reserve_kW = 0;
    
    Renewable* renewable_ptr;
    
    LoadStruct load_struct;
    
    this->storage_discharge_bool_vec.clear();
    this->storage_discharge_bool_vec.resize(storage_ptr_vec_ptr->size(), false);
    
    for (
        int timestep = 0; timestep < electrical_load_ptr->n_points; timestep++
    ) {
        //  1. get load and dt_hrs
        load_kW = electrical_load_ptr->load_vec_kW[timestep];
        dt_hrs = electrical_load_ptr->dt_vec_hrs[timestep];
        
        //  2. compute required firm dispatch
        required_firm_dispatch_kW = this->firm_dispatch_ratio * load_kW;
        
        //  3. compute total renewable production and required spinning reserve
        total_renewable_production_kW = 0;
        required_spinning_reserve_kW = this->load_reserve_ratio * load_kW;
        
        for (
            size_t asset = 0; asset < renewable_ptr_vec_ptr->size(); asset++
        ) {
            renewable_ptr = renewable_ptr_vec_ptr->at(asset);
            
            total_renewable_production_kW +=
                renewable_ptr->production_vec_kW[timestep];
            
            required_spinning_reserve_kW += 
                (1 - renewable_ptr->firmness_factor) *
                renewable_ptr->production_vec_kW[timestep];
        }
        
        if (required_spinning_reserve_kW > load_kW) {
            required_spinning_reserve_kW = load_kW;
        }
        
        //  4. init load structure
        load_struct.load_kW = load_kW;
        load_struct.total_renewable_production_kW =
            total_renewable_production_kW;
        load_struct.required_firm_dispatch_kW = required_firm_dispatch_kW;
        load_struct.required_spinning_reserve_kW =
            required_spinning_reserve_kW;
        
        //  5. handle Noncombustion dispatch
        load_struct = this->__handleNoncombustionDispatch(
            timestep,
            dt_hrs,
            load_struct,
            noncombustion_ptr_vec_ptr,
            resources_ptr
        );
        
        //  6. handle Storage discharge
        load_struct = this->__handleStorageDischarging(
            timestep,
            dt_hrs,
            load_struct,
            storage_ptr_vec_ptr
        );
        
        //  7. handle Combustion dispatch
        switch(this->control_mode) {
            case (ControlMode :: LOAD_FOLLOWING): {
                load_struct = this->__handleCombustionDispatch(
                    timestep,
                    dt_hrs,
                    load_struct,
                    combustion_ptr_vec_ptr,
                    false
                );
                
                break;
            }
            
            case (ControlMode :: CYCLE_CHARGING): {
                bool is_cycle_charging = false;
                
                for (
                    size_t asset = 0;
                    asset < storage_ptr_vec_ptr->size();
                    asset++
                ) {
                    if (not this->storage_discharge_bool_vec[asset]) {
                        is_cycle_charging = true;
                        break;
                    }
                }
                
                load_struct = this->__handleCombustionDispatch(
                    timestep,
                    dt_hrs,
                    load_struct,
                    combustion_ptr_vec_ptr,
                    is_cycle_charging
                );
                
                break;
            }
            
            default: {
                std::string error_str = "ERROR:  Controller :: setControlMode():  ";
                    error_str += "control mode ";
                    error_str += std::to_string(control_mode);
                    error_str += " not recognized";
                    
                    #ifdef _WIN32
                        std::cout << error_str << std::endl;
                    #endif

                    throw std::runtime_error(error_str);
                
                break;
            }
        }
        
        //  8. handle Renewable dispatch
        load_struct.load_kW = this->__handleRenewableDispatch(
            timestep,
            dt_hrs,
            load_struct.load_kW,
            renewable_ptr_vec_ptr
        );
        
        //  9. handle Storage charging
        this->__handleStorageCharging(
            timestep,
            dt_hrs,
            storage_ptr_vec_ptr,
            combustion_ptr_vec_ptr,
            noncombustion_ptr_vec_ptr,
            renewable_ptr_vec_ptr
        );
        
        //  10. log missed load, if any
        if (load_struct.load_kW > 1e-6) {
            this->missed_load_vec_kW[timestep] = load_struct.load_kW;
        }
        
        //  11. reset storage_discharge_bool_vec
        for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
            this->storage_discharge_bool_vec[asset] = false;
        }
    }
    
    
    // **** DEPRECATED ****
    /*
    double dt_hrs = 0;
    double load_kW = 0;
    double total_renewable_production_kW = 0;
    double firm_renewable_production_kW = 0;
    double remaining_load_kW = 0;
    
    double required_operating_reserve_before_kW = 0;
    double rem_load_test_0 = 0;
    double rem_load_test_1 = 0;
    double rem_load_test_2 = 0;
    double rem_load_test_3 = 0;
    double rem_load_test_4 = 0;
    
    this->required_operating_reserve_kW = 0;
    this->storage_discharge_bool_vec.clear();
    this->storage_discharge_bool_vec.resize(storage_ptr_vec_ptr->size(), false);
    
    Renewable* renewable_ptr;
    
    for (int timestep = 0; timestep < electrical_load_ptr->n_points; timestep++) {
        //  1. get dt_hrs and load
        dt_hrs = electrical_load_ptr->dt_vec_hrs[timestep];
        load_kW = electrical_load_ptr->load_vec_kW[timestep];
        
        //  2. compute firm and total Renewable productions
        total_renewable_production_kW = 0;
        firm_renewable_production_kW = 0;
        
        for (size_t asset = 0; asset < renewable_ptr_vec_ptr->size(); asset++) {
            renewable_ptr = renewable_ptr_vec_ptr->at(asset);
            
            total_renewable_production_kW += renewable_ptr->production_vec_kW[timestep];
            
            firm_renewable_production_kW +=
                renewable_ptr->firmness_factor * renewable_ptr->production_vec_kW[timestep];
        }
        
        //  3. compute required operating reserve (load + Renewable), enforce max
        this->required_operating_reserve_kW =
            this->load_operating_reserve_factor * load_kW + 
            total_renewable_production_kW - firm_renewable_production_kW;
        
        if (
            this->required_operating_reserve_kW >
            this->max_operating_reserve_factor * load_kW
        ) {
            this->required_operating_reserve_kW =
                this->max_operating_reserve_factor * load_kW;
        }
        
        //required_operating_reserve_before_kW = this->required_operating_reserve_kW;
        
        //  4. init remaining_load_kW
        remaining_load_kW = load_kW - total_renewable_production_kW;
        
        //rem_load_test_0 = remaining_load_kW;
        
        //  5. handle Storage discharging
        remaining_load_kW = this->__handleStorageDischarging(
            timestep,
            dt_hrs,
            remaining_load_kW,
            storage_ptr_vec_ptr
        );
        
        //rem_load_test_1 = remaining_load_kW;
        
        //  6. handle Noncombustion dispatch
        remaining_load_kW = this->__handleNoncombustionDispatch(
            timestep,
            dt_hrs,
            remaining_load_kW,
            noncombustion_ptr_vec_ptr,
            resources_ptr
        );
        
        //rem_load_test_2 = remaining_load_kW;
        
        //  7. handle Combustion dispatch
        switch(control_mode) {
            case (ControlMode :: LOAD_FOLLOWING): {
                remaining_load_kW = this->__handleCombustionDispatch(
                    timestep,
                    dt_hrs,
                    load_kW,
                    remaining_load_kW,
                    total_renewable_production_kW,
                    firm_renewable_production_kW,
                    combustion_ptr_vec_ptr,
                    false
                );
                
                break;
            }
            
            case (ControlMode :: CYCLE_CHARGING): {
                bool is_cycle_charging = false;
                
                for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
                    if (not this->storage_discharge_bool_vec[asset]) {
                        is_cycle_charging = true;
                        break;
                    }
                }
                
                remaining_load_kW = this->__handleCombustionDispatch(
                    timestep,
                    dt_hrs,
                    load_kW,
                    remaining_load_kW,
                    total_renewable_production_kW,
                    firm_renewable_production_kW,
                    combustion_ptr_vec_ptr,
                    is_cycle_charging
                );
                
                break;
            }
            
            default: {
                std::string error_str = "ERROR:  Controller :: setControlMode():  ";
                    error_str += "control mode ";
                    error_str += std::to_string(control_mode);
                    error_str += " not recognized";
                    
                    #ifdef _WIN32
                        std::cout << error_str << std::endl;
                    #endif

                    throw std::runtime_error(error_str);
                
                break;
            }
        }
        
        //rem_load_test_3 = remaining_load_kW;
        
        //  8. handle Renewable dispatch
        remaining_load_kW += total_renewable_production_kW;
        
        //rem_load_test_4 = remaining_load_kW;
        
        remaining_load_kW = this->__handleRenewableDispatch(
            timestep,
            dt_hrs,
            remaining_load_kW,
            renewable_ptr_vec_ptr
        );
        
        //  9. handle Storage charging
        this->__handleStorageCharging(
            timestep,
            dt_hrs,
            storage_ptr_vec_ptr,
            combustion_ptr_vec_ptr,
            noncombustion_ptr_vec_ptr,
            renewable_ptr_vec_ptr
        );
        
        //  10. log missed load, if any
        if (remaining_load_kW > 1e-6) {
            this->missed_load_vec_kW[timestep] = remaining_load_kW;
        }
        
        //  11. reset storage_discharge_bool_vec
        for (size_t asset = 0; asset < storage_ptr_vec_ptr->size(); asset++) {
            this->storage_discharge_bool_vec[asset] = false;
        }
        
        //  12. test print
        if (required_operating_reserve_before_kW < load_kW) {
            std::cout << "Timestep: " << timestep << std::endl;
            std::cout << "Load: " << load_kW << std::endl;
            std::cout << "Req Op Reserve: " << required_operating_reserve_before_kW << std::endl;
            std::cout << "Rem Load (before Storage): " << rem_load_test_0 << std::endl;
            std::cout << "Rem Load (after Storage): " << rem_load_test_1 << std::endl;
            std::cout << "Rem Load (after Noncombustion): " << rem_load_test_2 << std::endl;
            std::cout << "Rem Load (after Combustion): " << rem_load_test_3 << std::endl;
            std::cout << "Rem Load (before Renewable): " << rem_load_test_4 << std::endl;
            std::cout << "Rem Load: " << remaining_load_kW << std::endl;
            std::cout << std::endl;
        }
    */
    
    return;
}   /* applyDispatchControl() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: clear(void)
///
/// \brief Method to clear all attributes of the Controller object.
///

void Controller :: clear(void)
{
    this->net_load_vec_kW.clear();
    this->missed_load_vec_kW.clear();
    this->combustion_map.clear();
    
    return;
}   /* clear() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Controller :: ~Controller(void)
///
/// \brief Destructor for the Controller class.
///

Controller :: ~Controller(void)
{
    this->clear();
    
    return;
}   /* ~Controller() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
