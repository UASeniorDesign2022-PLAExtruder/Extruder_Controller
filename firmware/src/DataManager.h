/*******************************************************************************
 * Extruder_Controller
 * DataManager.h
 * Wilson Woods
 * 11.18.2021
 ******************************************************************************/

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
        
        DataManager();
        DataManager(const DataManager& orig);
        ~DataManager();
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

    private:
        
        typedef union                   // convert between float and char[4]
        {
            uint8_t buffer[4];
            float numeric_param_input;
        } FloatToBytes;

        FloatToBytes converter;

        typedef enum                    // status values
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

        typedef struct                  // numeric parameter object
        {
            const uint8_t data_index;   // index in numeric_params[] vector
            const uint8_t data_id;      // unique identifier, read by display
            float value;                // numeric parameter value
            bool is_current;            // status flag
        } Numeric_Param;

        typedef struct                  // status parameter object
        {
            const uint8_t data_index;   //index in status_params[] vector
            const uint8_t data_id;      // unique identifier, read by display
            STATUS status;              // status parameter value
            bool is_current;            // status flag
        } Status_Param;
        
        const uint8_t NUMERIC_PARAM_COUNT = 12; // length of numeric_params[]
        const uint8_t STATUS_PARAM_COUNT = 4;   // length of status_params[]
        
        // Numeric_Param objects for all numeric values to be tracked
        Numeric_Param desired_yield     = { 0,  0x01, 0.0, false };
        Numeric_Param required_input    = { 1,  0x02, 0.0, false };
        Numeric_Param ground_weight     = { 2,  0x03, 0.0, false };
        Numeric_Param zone_1_temp       = { 3,  0x04, 0.0, false };
        Numeric_Param zone_2_temp       = { 4,  0x05, 0.0, false };
        Numeric_Param zone_3_temp       = { 5,  0x06, 0.0, false };
        Numeric_Param screw_speed       = { 6,  0x07, 0.0, false };
        Numeric_Param roller_speed      = { 7,  0x08, 0.0, false };
        Numeric_Param spooler_speed     = { 8,  0x09, 0.0, false };
        Numeric_Param filament_diameter = { 9,  0x0A, 0.0, false };
        Numeric_Param extruded_length   = { 10, 0x0B, 0.0, false };
        Numeric_Param projected_yield   = { 11, 0x0C, 0.0, false };
        
        // Status_Param objects for all status values to be tracked
        Status_Param hopper_lid_status  = { 0, 0x10, NONE, false };
        Status_Param grinder_status     = { 1, 0x20, NONE, false };
        Status_Param preparation_status = { 2, 0x30, NONE, false };
        Status_Param extrusion_status   = { 3, 0x40, NONE, false };

        // vector of Numeric_Param structs
        std::vector<Numeric_Param> numeric_params = { desired_yield,
            required_input, ground_weight, zone_1_temp, zone_2_temp,
            zone_3_temp, screw_speed, roller_speed, spooler_speed,
            filament_diameter, extruded_length, projected_yield };
        
        // vector of Status_Param structs
        std::vector<Status_Param> status_params = { hopper_lid_status,
            grinder_status, preparation_status, extrusion_status };
        
        std::vector<uint8_t> fresh_numeric_IDs;
        std::vector<float> fresh_numeric_values;
        std::vector<uint8_t> fresh_status_IDs;
        std::vector<uint8_t> fresh_status_values;
};

#endif	/* DATAMANAGER_H */
