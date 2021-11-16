/*********************************************************************************
 * Extruder Controller
 * DataManager.h
 * 
 * 
 ********************************************************************************/

#ifndef DATAMANAGER_H
#define	DATAMANAGER_H

#include <cstdint>
#include <vector>
#include <utility>
#include <tuple>

      
class DataManager
{
    
    public:
        
        const uint16_t DISPLAY_I2C_ADDRESS = 0x14;
        
        void setNumericParam(uint8_t index, float param);
        void setStatusParam(uint8_t index, uint8_t param);
        
        float   getNumericParam(uint8_t index);
        uint8_t getStatusParam(uint8_t index);
        
        void clearNumericParamFlag(uint8_t index);
        void clearStatusParamFlag(uint8_t index);
        
        void pollNumericParams();
        void pollStatusParams();
        void sendNumericParamI2C(uint8_t data_id, float value);
        void sendStatusParamI2C(uint8_t data_id, uint8_t status);
        void sendAllFreshNumericParams();
        void sendAllFreshStatusParams();
        
        std::vector<uint8_t>& getFreshNumericIDs();
        std::vector<float>&   getFreshNumericValues();
        std::vector<uint8_t>& getFreshStatusIDs();
        std::vector<uint8_t>& getFreshStatusValues();

        DataManager();
        DataManager(const DataManager& orig);
        ~DataManager();
      
    private:
        
        
        
        typedef union
        {
            uint8_t buffer[4];
            float numeric_param_input;
        } FloatToBytes;

        FloatToBytes converter;

        typedef enum
        {
            NONE,
            READY,
            ON,
            OFF,
            OPEN,
            CLOSED,
            IN_PROGRESS,
            COMPLETE
        } STATUS;

        typedef struct
        {
            const uint8_t data_index;
            const uint8_t data_id;
            float value;
            bool is_current;  
        } Numeric_Param;

        typedef struct
        {
            const uint8_t data_index;
            const uint8_t data_id;
            STATUS status;
            bool is_current;   
        } Status_Param;
        
        const uint8_t NUMERIC_PARAM_COUNT = 12;
        const uint8_t STATUS_PARAM_COUNT = 4;
        
        Numeric_Param desired_yield     = { 0,  0x01, 0.0,  false };
        Numeric_Param required_input    = { 1,  0x02, 0.0,  false };
        Numeric_Param ground_weight     = { 2,  0x03, 0.0,  false };
        Numeric_Param zone_1_temp       = { 3,  0x04, 0.0,  false };
        Numeric_Param zone_2_temp       = { 4,  0x05, 0.0,  false };
        Numeric_Param zone_3_temp       = { 5,  0x06, 0.0,  false };
        Numeric_Param screw_speed       = { 6,  0x07, 0.0,  false };
        Numeric_Param roller_speed      = { 7,  0x08, 0.0,  false };
        Numeric_Param spooler_speed     = { 8,  0x09, 0.0,  false };
        Numeric_Param filament_diameter = { 9,  0x0A, 0.0,  false };
        Numeric_Param extruded_length   = { 10, 0x0B, 0.0,  false };
        Numeric_Param projected_yield   = { 11, 0x0C, 0.0,  false };
        Status_Param hopper_lid_status  = { 0, 0x10, NONE, false };
        Status_Param grinder_status     = { 1, 0x20, NONE, false };
        Status_Param preparation_status = { 2, 0x30, NONE, false };
        Status_Param extrusion_status   = { 3, 0x40, NONE, false };

        std::vector<Numeric_Param> numeric_params = { desired_yield, required_input,
            ground_weight, zone_1_temp, zone_2_temp, zone_3_temp, screw_speed, roller_speed, spooler_speed,
            filament_diameter, extruded_length, projected_yield };
        
        std::vector<Status_Param> status_params = { hopper_lid_status, grinder_status,
            preparation_status, extrusion_status };
        
   
        std::vector<uint8_t> fresh_numeric_IDs;
        std::vector<float> fresh_numeric_values;
        std::vector<uint8_t> fresh_status_IDs;
        std::vector<uint8_t> fresh_status_values;
        

        
        /*
        static std::vector<Numeric_Param> numeric_params = { { 0x01, 0.0,  false }, { 0x02, 0.0,  false }, { 0x03, 0.0,  false },
        { 0x04, 0.0,  false }, { 0x05, 0.0,  false }, { 0x06, 0.0,  false }, { 0x07, 0.0,  false }, { 0x08, 0.0,  false }, 
        { 0x09, 0.0,  false }, { 0x0A, 0.0,  false }, { 0x0B, 0.0,  false }, { 0x0C, 0.0,  false } };
        
        static std::vector<Status_Param> status_params = { { 0x10, NONE, false }, { 0x20, NONE, false }, { 0x30, NONE, false }, { 0x40, NONE, false } };
        */
};

#endif	/* DATAMANAGER_H */

