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
/// \fn void Controller :: __computeNetLoad(
///         ElectricalLoad* electrical_load_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         Resources* resources_ptr
///     )
///
/// \brief Helper method to compute and populate the net load vector.
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

void Controller :: __computeNetLoad(
    ElectricalLoad* electrical_load_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    Resources* resources_ptr
)
{
    //  1. init
    this->net_load_vec_kW.resize(electrical_load_ptr->n_points, 0);
    this->missed_load_vec_kW.resize(electrical_load_ptr->n_points, 0);
    
    //  2. populate net load vector
    double dt_hrs = 0;
    double load_kW = 0;
    double net_load_kW = 0;
    double production_kW = 0;
    
    Renewable* renewable_ptr;
    
    for (int i = 0; i < electrical_load_ptr->n_points; i++) {
        dt_hrs = electrical_load_ptr->dt_vec_hrs[i];
        load_kW = electrical_load_ptr->load_vec_kW[i];
        net_load_kW = load_kW;
        
        for (size_t j = 0; j < renewable_ptr_vec_ptr->size(); j++) {
            renewable_ptr = renewable_ptr_vec_ptr->at(j);
            
            production_kW = this->__getRenewableProduction(
                i,
                dt_hrs,
                renewable_ptr,
                resources_ptr
            );
            
            load_kW = renewable_ptr->commit(
                i,
                dt_hrs,
                production_kW,
                load_kW
            );
        
            net_load_kW -= production_kW;
        }
        
        this->net_load_vec_kW[i] = net_load_kW;
    }
    
    return;
}   /* __computeNetLoad() */

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
    //  1. get state table dimensions
    int n_cols = combustion_ptr_vec_ptr->size();
    int n_rows = pow(2, n_cols);
    
    //  2. init state table (all possible on/off combinations)
    std::vector<std::vector<bool>> state_table;
    state_table.resize(n_rows, {});
    
    int x = 0;
    for (int i = 0; i < n_rows; i++) {
        state_table[i].resize(n_cols, false);
        
        x = i;
        for (int j = 0; j < n_cols; j++) {
            if (x % 2 == 0) {
                state_table[i][j] = true;
            }
            x /= 2;
        }
    }
    
    //  3. construct combustion map (handle duplicates by keeping rows with minimum
    //     trues)
    double total_capacity_kW = 0;
    int truth_count = 0;
    int current_truth_count = 0;
    
    for (int i = 0; i < n_rows; i++) {
        total_capacity_kW = 0;
        truth_count = 0;
        current_truth_count = 0;
        
        for (int j = 0; j < n_cols; j++) {
            if (state_table[i][j]) {
                total_capacity_kW += combustion_ptr_vec_ptr->at(j)->capacity_kW;
                truth_count++;
            }
        }
        
        if (this->combustion_map.count(total_capacity_kW) > 0) {
            for (int j = 0; j < n_cols; j++) {
                if (this->combustion_map[total_capacity_kW][j]) {
                    current_truth_count++;
                }
            }
            
            if (truth_count < current_truth_count) {
                this->combustion_map.erase(total_capacity_kW);
            }
        }
        
        this->combustion_map.insert(
            std::pair<double, std::vector<bool>> (
                total_capacity_kW,
                state_table[i]
            )
        );
    }
    
    /*
    // ==== TEST PRINT ==== //
    std::cout << std::endl;
    
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
    //*/
    
    return;
}   /* __constructCombustionTable() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __applyLoadFollowingControl_CHARGING(
///         int timestep,
///         ElectricalLoad* electrical_load_ptr,
///         Resources* resources_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Helper method to apply load following control action for given timestep of
///     the Model run when net load <= 0;
///
/// \param timestep The current time step of the Model run.
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

void Controller :: __applyLoadFollowingControl_CHARGING(
    int timestep,
    ElectricalLoad* electrical_load_ptr,
    Resources* resources_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    //  1. get dt_hrs, set net load
    double dt_hrs = electrical_load_ptr->dt_vec_hrs[timestep];
    double net_load_kW = 0;
    
    //  2. request zero production from all Combustion assets
    this->__handleCombustionDispatch(
        timestep,
        dt_hrs,
        net_load_kW,
        combustion_ptr_vec_ptr,
        false   // is_cycle_charging
    );
    
    //  3. request zero production from all Noncombustion assets
    this->__handleNoncombustionDispatch(
        timestep,
        dt_hrs,
        net_load_kW,
        noncombustion_ptr_vec_ptr,
        resources_ptr
    );
    
    //  4. attempt to charge all Storage assets using any and all available curtailment
    //     charge priority is Combustion, then Renewable
    this->__handleStorageCharging(
        timestep,
        dt_hrs,
        storage_ptr_vec_ptr,
        combustion_ptr_vec_ptr,
        noncombustion_ptr_vec_ptr,
        renewable_ptr_vec_ptr
    );
    
    return;
}   /* __applyLoadFollowingControl_CHARGING() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __applyLoadFollowingControl_DISCHARGING(
///         int timestep,
///         ElectricalLoad* electrical_load_ptr,
///         Resources* resources_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Helper method to apply load following control action for given timestep of
///     the Model run when net load > 0;
///
/// \param timestep The current time step of the Model run.
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

void Controller :: __applyLoadFollowingControl_DISCHARGING(
    int timestep,
    ElectricalLoad* electrical_load_ptr,
    Resources* resources_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    //  1. get dt_hrs, net load
    double dt_hrs = electrical_load_ptr->dt_vec_hrs[timestep];
    double net_load_kW = this->net_load_vec_kW[timestep];
    
    //  2. partition Storage assets into depleted and non-depleted
    std::list<Storage*> depleted_storage_ptr_list;
    std::list<Storage*> nondepleted_storage_ptr_list;
    
    Storage* storage_ptr;
    for (size_t i = 0; i < storage_ptr_vec_ptr->size(); i++) {
        storage_ptr = storage_ptr_vec_ptr->at(i);
        
        if (storage_ptr->is_depleted) {
            depleted_storage_ptr_list.push_back(storage_ptr);
        }
        
        else {
            nondepleted_storage_ptr_list.push_back(storage_ptr);
        }
    }
    
    //  3. discharge non-depleted storage assets
    net_load_kW = this->__handleStorageDischarging(
        timestep,
        dt_hrs,
        net_load_kW,
        nondepleted_storage_ptr_list
    );
    
    //  4. request optimal production from all Noncombustion assets
    net_load_kW = this->__handleNoncombustionDispatch(
        timestep,
        dt_hrs,
        net_load_kW,
        noncombustion_ptr_vec_ptr,
        resources_ptr
    );
    
    //  5. request optimal production from all Combustion assets
    net_load_kW = this->__handleCombustionDispatch(
        timestep,
        dt_hrs,
        net_load_kW,
        combustion_ptr_vec_ptr,
        false   // is_cycle_charging
    );
    
    //  6. attempt to charge depleted Storage assets using any and all available
    ///    curtailment
    //     charge priority is Combustion, then Renewable
    this->__handleStorageCharging(
        timestep,
        dt_hrs,
        depleted_storage_ptr_list,
        combustion_ptr_vec_ptr,
        noncombustion_ptr_vec_ptr,
        renewable_ptr_vec_ptr
    );
    
    //  7. record any missed load
    if (net_load_kW > 1e-6) {
        this->missed_load_vec_kW[timestep] = net_load_kW;
    }
    
    return;
}   /* __applyLoadFollowingControl_DISCHARGING() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __applyCycleChargingControl_CHARGING(
///         int timestep,
///         ElectricalLoad* electrical_load_ptr,
///         Resources* resources_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Helper method to apply cycle charging control action for given timestep of
///     the Model run when net load <= 0. Simply defaults to load following control.
///
/// \param timestep The current time step of the Model run.
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

void Controller :: __applyCycleChargingControl_CHARGING(
    int timestep,
    ElectricalLoad* electrical_load_ptr,
    Resources* resources_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    //  1. default to load following
    this->__applyLoadFollowingControl_CHARGING(
        timestep,
        electrical_load_ptr,
        resources_ptr,
        combustion_ptr_vec_ptr,
        noncombustion_ptr_vec_ptr,
        renewable_ptr_vec_ptr,
        storage_ptr_vec_ptr
    );
    
    return;
}   /* __applyCycleChargingControl_CHARGING() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __applyCycleChargingControl_DISCHARGING(
///         int timestep,
///         ElectricalLoad* electrical_load_ptr,
///         Resources* resources_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Helper method to apply cycle charging control action for given timestep of
///     the Model run when net load > 0. Defaults to load following control if no
///     depleted storage assets.
///
/// \param timestep The current time step of the Model run.
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

void Controller :: __applyCycleChargingControl_DISCHARGING(
    int timestep,
    ElectricalLoad* electrical_load_ptr,
    Resources* resources_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    //  1. get dt_hrs, net load
    double dt_hrs = electrical_load_ptr->dt_vec_hrs[timestep];
    double net_load_kW = this->net_load_vec_kW[timestep];
    
    //  2. partition Storage assets into depleted and non-depleted
    std::list<Storage*> depleted_storage_ptr_list;
    std::list<Storage*> nondepleted_storage_ptr_list;
    
    Storage* storage_ptr;
    for (size_t i = 0; i < storage_ptr_vec_ptr->size(); i++) {
        storage_ptr = storage_ptr_vec_ptr->at(i);
        
        if (storage_ptr->is_depleted) {
            depleted_storage_ptr_list.push_back(storage_ptr);
        }
        
        else {
            nondepleted_storage_ptr_list.push_back(storage_ptr);
        }
    }
    
    //  3. discharge non-depleted storage assets
    net_load_kW = this->__handleStorageDischarging(
        timestep,
        dt_hrs,
        net_load_kW,
        nondepleted_storage_ptr_list
    );
    
    //  4. request optimal production from all Noncombustion assets
    net_load_kW = this->__handleNoncombustionDispatch(
        timestep,
        dt_hrs,
        net_load_kW,
        noncombustion_ptr_vec_ptr,
        resources_ptr
    );
    
    //  5. request optimal production from all Combustion assets
    //     default to load following if no depleted storage
    if (depleted_storage_ptr_list.empty()) {
        net_load_kW = this->__handleCombustionDispatch(
            timestep,
            dt_hrs,
            net_load_kW,
            combustion_ptr_vec_ptr,
            false   // is_cycle_charging
        );
    }
    
    else {
        net_load_kW = this->__handleCombustionDispatch(
            timestep,
            dt_hrs,
            net_load_kW,
            combustion_ptr_vec_ptr,
            true    // is_cycle_charging
        );
    }
    
    //  6. attempt to charge depleted Storage assets using any and all available
    ///    curtailment
    //     charge priority is Combustion, then Renewable
    this->__handleStorageCharging(
        timestep,
        dt_hrs,
        depleted_storage_ptr_list,
        combustion_ptr_vec_ptr,
        noncombustion_ptr_vec_ptr,
        renewable_ptr_vec_ptr
    );
    
    //  7. record any missed load
    if (net_load_kW > 1e-6) {
        this->missed_load_vec_kW[timestep] = net_load_kW;
    }
    
    return;
}   /* __applyCycleChargingControl_DISCHARGING() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: __handleStorageCharging(
///         int timestep,
///         double dt_hrs,
///         std::list<Storage*> storage_ptr_list,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr
///     )
///
/// \brief Helper method to handle the charging of the given Storage assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param storage_ptr_list A list of pointers to the Storage assets that are to be
///     charged.
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
    std::list<Storage*> storage_ptr_list,
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
    
    std::list<Storage*>::iterator iter;
    for (
        iter = storage_ptr_list.begin();
        iter != storage_ptr_list.end();
        iter++
    ){
        storage_ptr = (*iter);
        
        //  1. attempt to charge from Combustion curtailment first
        for (size_t i = 0; i < combustion_ptr_vec_ptr->size(); i++) {
            combustion_ptr = combustion_ptr_vec_ptr->at(i);
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
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  2. attempt to charge from Noncombustion curtailment second
        for (size_t i = 0; i < noncombustion_ptr_vec_ptr->size(); i++) {
            noncombustion_ptr = noncombustion_ptr_vec_ptr->at(i);
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
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  3. attempt to charge from Renewable curtailment third
        for (size_t i = 0; i < renewable_ptr_vec_ptr->size(); i++) {
            renewable_ptr = renewable_ptr_vec_ptr->at(i);
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
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  4. commit charge
        storage_ptr->commitCharge(
            timestep,
            dt_hrs,
            storage_ptr->power_kW
        );
    }
    
    return;
}   /* __handleStorageCharging() */

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
/// \brief Helper method to handle the charging of the given Storage assets.
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
    
    for (size_t j = 0; j < storage_ptr_vec_ptr->size(); j++) {
        storage_ptr = storage_ptr_vec_ptr->at(j);
        
        //  1. attempt to charge from Combustion curtailment first
        for (size_t i = 0; i < combustion_ptr_vec_ptr->size(); i++) {
            combustion_ptr = combustion_ptr_vec_ptr->at(i);
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
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  2. attempt to charge from Noncombustion curtailment second
        for (size_t i = 0; i < noncombustion_ptr_vec_ptr->size(); i++) {
            noncombustion_ptr = noncombustion_ptr_vec_ptr->at(i);
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
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  3. attempt to charge from Renewable curtailment third
        for (size_t i = 0; i < renewable_ptr_vec_ptr->size(); i++) {
            renewable_ptr = renewable_ptr_vec_ptr->at(i);
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
            storage_ptr->power_kW += acceptable_kW;
        }
        
        //  4. commit charge
        storage_ptr->commitCharge(
            timestep,
            dt_hrs,
            storage_ptr->power_kW
        );
    }
    
    return;
}   /* __handleStorageCharging() */

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

///
/// \fn double Controller :: __handleCombustionDispatch(
///         int timestep,
///         double dt_hrs,
///         double net_load_kW,
//          std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         bool is_cycle_charging
///     )
///
/// \brief Helper method to handle the optimal dispatch of Combustion assets. Dispatches
///     for 1.2x the received net load, so as to ensure a "20% spinning reserve".
///     Dispatches a minimum number of Combustion assets, which then share the load
///     proportional to their rated capacities.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param net_load_kW The net load [kW] before the dispatch is deducted from it.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///
/// \param is_cycle_charging A boolean which defines whether to apply cycle charging
///     logic or not.
///
/// \return The net load [kW] remaining after the dispatch is deducted from it.
///

double Controller :: __handleCombustionDispatch(
    int timestep,
    double dt_hrs,
    double net_load_kW,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    bool is_cycle_charging
)
{
    //  1. get minimal Combustion dispatch
    double target_production_kW = 1.2 * net_load_kW;
    double total_capacity_kW = 0;
    
    std::map<double, std::vector<bool>>::iterator iter = this->combustion_map.begin();
    while (iter != std::prev(this->combustion_map.end(), 1)) {
        if (target_production_kW <= total_capacity_kW) {
            break;
        }
        
        iter++;
        total_capacity_kW = iter->first;
    }
    
    //  2. share load proportionally (by rated capacity) over active diesels
    Combustion* combustion_ptr;
    double production_kW = 0;
    double request_kW = 0;
    double _net_load_kW = net_load_kW;
    
    for (size_t i = 0; i < this->combustion_map[total_capacity_kW].size(); i++) {
        combustion_ptr = combustion_ptr_vec_ptr->at(i);
        
        if (total_capacity_kW > 0) {
            request_kW =
                int(this->combustion_map[total_capacity_kW][i]) * 
                net_load_kW *
                (combustion_ptr->capacity_kW / total_capacity_kW);
        }
        
        else {
            request_kW = 0;
        }
        
        if (is_cycle_charging and request_kW > 0) {
            if (request_kW < 0.85 * combustion_ptr->capacity_kW) {
                request_kW = 0.85 * combustion_ptr->capacity_kW;
            }
        }
        
        production_kW = combustion_ptr->requestProductionkW(
            timestep,
            dt_hrs,
            request_kW
        );
        
        _net_load_kW = combustion_ptr->commit(
            timestep,
            dt_hrs,
            production_kW,
            _net_load_kW
        );
    }
    
    return _net_load_kW;
}   /* __handleCombustionDispatch() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //


///
/// \fn double __handleNoncombustionDispatch(
///         int timestep,
///         double dt_hrs,
///         double net_load_kW,
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
/// \param net_load_kW The net load [kW] before the dispatch is deducted from it.
///
/// \param noncombustion_ptr_vec_ptr A pointer to the Noncombustion pointer vector of
///     the Model.
///
/// \param resources_ptr A pointer to the Resources component of the Model.
///
/// \return The net load [kW] remaining after the dispatch is deducted from it.
///

double Controller :: __handleNoncombustionDispatch(
    int timestep,
    double dt_hrs,
    double net_load_kW,
    std::vector<Noncombustion*>* noncombustion_ptr_vec_ptr,
    Resources* resources_ptr
)
{
    Noncombustion* noncombustion_ptr;
    double production_kW = 0;
    
    for (size_t i = 0; i < noncombustion_ptr_vec_ptr->size(); i++) {
        noncombustion_ptr = noncombustion_ptr_vec_ptr->at(i);
        
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
                    net_load_kW,
                    resource_value
                );
                
                net_load_kW = noncombustion_ptr->commit(
                    timestep,
                    dt_hrs,
                    production_kW,
                    net_load_kW,
                    resource_value
                );
                
                break;
            }
            
            default: {
                production_kW = noncombustion_ptr->requestProductionkW(
                    timestep,
                    dt_hrs,
                    net_load_kW
                );
                
                net_load_kW = noncombustion_ptr->commit(
                    timestep,
                    dt_hrs,
                    production_kW,
                    net_load_kW
                );
                
                break;
            }
        }
    }
    
    return net_load_kW;
}   /* __handleNoncombustionDispatch() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn double Controller :: __handleStorageDischarging(
///         int timestep,
///         double dt_hrs,
///         double net_load_kW,
///         std::list<Storage*> storage_ptr_list
///     )
///
/// \brief Helper method to handle the discharging of the given Storage assets.
///
/// \param timestep The current time step of the Model run.
///
/// \param dt_hrs The interval of time [hrs] associated with the action.
///
/// \param storage_ptr_list A list of pointers to the Storage assets that are to be
///     discharged.
///
/// \return The net load [kW] remaining after the discharge is deducted from it.
///

double Controller :: __handleStorageDischarging(
    int timestep,
    double dt_hrs,
    double net_load_kW,
    std::list<Storage*> storage_ptr_list
)
{
    double discharging_kW = 0;
    
    Storage* storage_ptr;
    
    std::list<Storage*>::iterator iter;
    for (
        iter = storage_ptr_list.begin();
        iter != storage_ptr_list.end();
        iter++
    ){
        storage_ptr = (*iter);
        
        discharging_kW = storage_ptr->getAvailablekW(dt_hrs);
        
        if (discharging_kW > net_load_kW) {
            discharging_kW = net_load_kW;
        }
        
        net_load_kW = storage_ptr->commitDischarge(
            timestep,
            dt_hrs,
            discharging_kW,
            net_load_kW
        );
    }
    
    return net_load_kW;
}   /* __handleStorageDischarging() */

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
    //  1. compute net load
    this->__computeNetLoad(electrical_load_ptr, renewable_ptr_vec_ptr, resources_ptr);
    
    //  2. construct Combustion table
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
    for (int i = 0; i < electrical_load_ptr->n_points; i++) {
        switch (this->control_mode) {
            case (ControlMode :: LOAD_FOLLOWING): {
                if (this->net_load_vec_kW[i] <= 0) {
                    this->__applyLoadFollowingControl_CHARGING(
                        i,
                        electrical_load_ptr,
                        resources_ptr,
                        combustion_ptr_vec_ptr,
                        noncombustion_ptr_vec_ptr,
                        renewable_ptr_vec_ptr,
                        storage_ptr_vec_ptr
                    );
                }
                
                else {
                    this->__applyLoadFollowingControl_DISCHARGING(
                        i,
                        electrical_load_ptr,
                        resources_ptr,
                        combustion_ptr_vec_ptr,
                        noncombustion_ptr_vec_ptr,
                        renewable_ptr_vec_ptr,
                        storage_ptr_vec_ptr
                    );
                }
                
                break;
            }
            
            case (ControlMode :: CYCLE_CHARGING): {
                if (this->net_load_vec_kW[i] <= 0) {
                    this->__applyCycleChargingControl_CHARGING(
                        i,
                        electrical_load_ptr,
                        resources_ptr,
                        combustion_ptr_vec_ptr,
                        noncombustion_ptr_vec_ptr,
                        renewable_ptr_vec_ptr,
                        storage_ptr_vec_ptr
                    );
                }
                
                else {
                    this->__applyCycleChargingControl_DISCHARGING(
                        i,
                        electrical_load_ptr,
                        resources_ptr,
                        combustion_ptr_vec_ptr,
                        noncombustion_ptr_vec_ptr,
                        renewable_ptr_vec_ptr,
                        storage_ptr_vec_ptr
                    );
                }
                
                break;
            }
            
            default: {
                std::string error_str = "ERROR:  Controller :: applyDispatchControl():  ";
                error_str += "control mode ";
                error_str += std::to_string(this->control_mode);
                error_str += " not recognized";
                
                #ifdef _WIN32
                    std::cout << error_str << std::endl;
                #endif

                throw std::runtime_error(error_str);
                
                break;
            }
        }
    }
    
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
