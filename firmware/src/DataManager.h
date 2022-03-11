/*******************************************************************************
 * Extruder_Controller
 * DataManager.h
 * Wilson Woods
 * 11.18.2021
 * 
 * The dataManager class provides access to all data produced
 * throughout the entire Extruder_Controller system. This includes motor speeds,
 * sensor readings, and stage status values to indicate system progress. The
 * dataManager collects provides access to this data across all threads (via
 * atomic reads), and also sends all new values to the display controller
 * via I2C. Each individual parameter is given a struct that contains its value
 * along with a unique identifier to accompany the value when transmitted via
 * I2C, and an internal flag that indicates whether the value is current or not.
 * 
 * Parameters are divided into 2 groups: Numeric (motor speeds, sensor readings,
 * etc.) and Status (stage statuses and other non-numeric values). Numeric
 * parameter structs are stored in the numeric_params vector and Status
 * parameter structs are stored in the status_params vector.
 * 
 * During operation, any value that is changed is first updated in its
 * respective struct, and its is_current value is set to false. Meanwhile, the
 * dataManager class iterates through each vector (numeric_params and
 * status_params) and automatically sends to the display via I2C any new values,
 * and resets the is_current flag as it does so.
 * 
 ************************** Adding a new parameter *****************************
 * 
 * In order to add a new parameter the following steps must be taken:
 *
 *      1.) Determine whether this is a numeric or status parameter
 * 
 *      2.) In the case of a status parameter, ensure that the STATUS enum
 *          has the required states for this parameter, if not, a new status/
 *          statuses can be added below the last state currently listed
 * 
 *      3.) Increase the size of the appropriate parameter vector (either
 *          NUMERIC_PARAM_COUNT or STATUS_PARAM_COUNT must be incremented to
 *          accommodate the new parameter.
 * 
 *      4.) Create an instance of the appropriate parameter struct (either 
 *          a Numeric_Param or Status_Param, follow the convention below for
 *          naming and initial values)
 * 
 *      5.) Add the newly created struct to the end of appropriate vector
 *          (either numeric_params or status_params)
 * 
 *      6.) The dataManager should now be able to handle the new parameter.
 *          The display code must be updated independently to match the change.
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
        ~DataManager();
        void set_numeric_param( uint8_t index, float param );
        void set_status_param( uint8_t index, uint8_t param );
        void set_spooler_tension( uint16_t tension );
        float get_numeric_param( uint8_t index );
        uint8_t get_status_param( uint8_t index );
        uint16_t get_spooler_tension( void );
        void clear_numeric_param_flag( uint8_t index );
        void clear_status_param_flag( uint8_t index );
        void poll_numeric_params( void );
        void poll_status_params( void );
        void send_numeric_param_I2C( uint8_t data_id, float value );
        void send_status_param_I2C( uint8_t data_id, uint8_t status );
        void send_all_fresh_numeric_params( void );
        void send_all_fresh_status_params( void );
        
        std::vector<uint8_t>& get_fresh_numeric_IDs();
        std::vector<float>&   get_fresh_numeric_values();
        std::vector<uint8_t>& get_fresh_status_IDs();
        std::vector<uint8_t>& get_fresh_status_values();

    private:
        
        uint16_t spooler_tension = 0;
        
        // convert between float and char[4]
        typedef union
        {
            uint8_t buffer[4];
            float numeric_param_input;
        } FloatToBytes;

        // use converter to send float values as integers via I2C
        // convert back to float at the other end
        FloatToBytes converter;         

        // list of possible status values for various process stages
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
