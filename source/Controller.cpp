/*
 *  PGMcpp : PRIMED Grid Controllerling (in C++)
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
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resources_ptr->resource_map_1D[renewable_ptr->resource_key][timestep]
            );
            
            break;
        }
        
        case (RenewableType :: TIDAL): {
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resources_ptr->resource_map_1D[renewable_ptr->resource_key][timestep]
            );
            
            break;
        }
        
        case (RenewableType :: WAVE): {
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resources_ptr->resource_map_2D[renewable_ptr->resource_key][timestep][0],
                resources_ptr->resource_map_2D[renewable_ptr->resource_key][timestep][1]
            );
            
            break;
        }
        
        case (RenewableType :: WIND): {
            production_kW = renewable_ptr->computeProductionkW(
                timestep,
                dt_hrs,
                resources_ptr->resource_map_1D[renewable_ptr->resource_key][timestep]
            );
            
            break;
        }
        
        default: {
            // do nothing!
            
            break;
        }
    }
    
    return production_kW;
}   /* __getRenewableProduction() */

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
    
    //...
    
    return;
}   /* init() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Controller :: applyDispatchControl(
///         ElectricalLoad* electrical_load_ptr,
///         std::vector<Combustion*>* combustion_ptr_vec_ptr,
///         std::vector<Renewable*>* renewable_ptr_vec_ptr,
///         std::vector<Storage*>* storage_ptr_vec_ptr
///     )
///
/// \brief Method to apply dispatch control at every point in the modelling time series.
///
/// \param electrical_load_ptr A pointer to the ElectricalLoad component of the Model.
///
/// \param combustion_ptr_vec_ptr A pointer to the Combustion pointer vector of the Model.
///
/// \param renewable_ptr_vec_ptr A pointer to the Renewable pointer vector of the Model.
///
/// \param storage_ptr_vec_ptr A pointer to the Storage pointer vector of the Model.
///

void Controller :: applyDispatchControl(
    ElectricalLoad* electrical_load_ptr,
    std::vector<Combustion*>* combustion_ptr_vec_ptr,
    std::vector<Renewable*>* renewable_ptr_vec_ptr,
    std::vector<Storage*>* storage_ptr_vec_ptr
)
{
    for (int i = 0; i < electrical_load_ptr->n_points; i++) {
        switch (this->control_mode) {
            case (ControlMode :: LOAD_FOLLOWING): {
                //this->__applyLoadFollowingControl();
                
                break;
            }
            
            case (ControlMode :: CYCLE_CHARGING): {
                //this->__applyCycleChargingControl();
                
                break;
            }
            
            default: {
                // do nothing!
                
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
