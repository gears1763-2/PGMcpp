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
/// \file Model.h
///
/// \brief Header file for the Model class.
///


#ifndef MODEL_H
#define MODEL_H


//  components
#include "Controller.h"
#include "ElectricalLoad.h"
#include "Resources.h"

//  production
#include "Production/Combustion/Diesel.h"
#include "Production/Noncombustion/Hydro.h"
#include "Production/Renewable/Solar.h"
#include "Production/Renewable/Tidal.h"
#include "Production/Renewable/Wave.h"
#include "Production/Renewable/Wind.h"

// storage
#include "Storage/LiIon.h"


///
/// \struct ModelInputs
///
/// \brief A structure which bundles the necessary inputs for the Model constructor.
///     Provides default values for every necessary input (except
///     path_2_electrical_load_time_series, for which a valid input must be provided).
///

struct ModelInputs {
    std::string path_2_electrical_load_time_series = ""; ///< A string defining the path (either relative or absolute) to the given electrical load time series.
    
    ControlMode control_mode = ControlMode :: LOAD_FOLLOWING; ///< The control mode to be applied by the Controller object.
};


///
/// \class Model
///
/// \brief A container class which forms the centre of PGMcpp. The Model class is
///     intended to serve as the primary user interface with the functionality of
///     PGMcpp, and as such it contains all other classes.
///

class Model {
    private:
        //  1. attributes
        void __checkInputs(ModelInputs);
        
        void __computeFuelAndEmissions(void);
        void __computeNetPresentCost(void);
        void __computeLevellizedCostOfEnergy(void);
        void __computeEconomics(void);
        
        void __writeSummary(std::string);
        void __writeTimeSeries(std::string, int = -1);
        
        
        //  2. methods
        //...
        
        
    public:
        //  1. attributes
        double total_fuel_consumed_L; ///< The total fuel consumed [L] over a model run.
        Emissions total_emissions; ///< An Emissions structure for holding total emissions [kg].
        
        double net_present_cost; ///< The net present cost of the Model (undefined currency).
        double total_renewable_dispatch_kWh; ///< The total energy dispatched [kWh] by all renewable assets over the Model run.
        double total_dispatch_discharge_kWh; ///< The total energy dispatched/discharged [kWh] over the Model run.
        double levellized_cost_of_energy_kWh; ///< The levellized cost of energy, per unit energy dispatched/discharged, of the Model [1/kWh] (undefined currency).
        
        Controller controller; ///< Controller component of Model
        ElectricalLoad electrical_load; ///< ElectricalLoad component of Model
        Resources resources; ///< Resources component of Model
        
        std::vector<Combustion*> combustion_ptr_vec; ///< A vector of pointers to the various Combustion assets in the Model
        std::vector<Noncombustion*> noncombustion_ptr_vec; ///< A vector of pointers to the various Noncombustion assets in the Model
        std::vector<Renewable*> renewable_ptr_vec;  ///< A vector of pointers to the various Renewable assets in the Model
        std::vector<Storage*> storage_ptr_vec;  ///< A vector of pointers to the various Storage assets in the Model
        
        
        //  2. methods
        Model(void);
        Model(ModelInputs);
        
        void addDiesel(DieselInputs);
        
        void addResource(NoncombustionType, std::string, int);
        void addResource(RenewableType, std::string, int);
        
        void addHydro(HydroInputs);
        
        void addSolar(SolarInputs);
        void addTidal(TidalInputs);
        void addWave(WaveInputs);
        void addWind(WindInputs);
        
        void addLiIon(LiIonInputs);
        
        void run(void);
        
        void reset(void);
        void clear(void);
        
        void writeResults(std::string, int = -1);
        
        ~Model(void);
        
};  /* Model */


#endif  /* Model_H */
