/*********************************************************************************
 * Extruder_Controller
 * DataManager.cpp
 * Wilson Woods
 * 11.18.2021
 ********************************************************************************/

#include <cstdint>
#include <vector>
#include <tuple>
#include <utility>

#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "DataManager.h"
#include "globals.h"
#include "I2C.h"

DataManager::DataManager()
{
    
}

DataManager::DataManager(const DataManager& orig)
{
    
}

DataManager::~DataManager()
{
    
}

/**
 * DataManager::setNumericParam()
 * 
 * @param index Parameter index in numeric_params[] vector
 * 
 * @param value Parameter value to be added to numeric_params[] vector
 * 
 * Update global DataManager numeric parameter vector
 * Values added to this vector are automatically sent via I2C to the display
 */
void DataManager::setNumericParam(uint8_t index, float value)
{
    numeric_params[index].value = value;
    numeric_params[index].is_current = false;
}

/**
 * DataManager::setStatusParam()
 * 
 * @param index Parameter index in status_params[] vector
 * 
 * @param value Parameter value to be added to status_params[] vector
 * 
 * Update global DataManager status (non-numeric) parameter vector
 * Values added to this vector are automatically sent via I2C to the display
 */
void DataManager::setStatusParam(uint8_t index, uint8_t status)
{
    status_params[index].status = (DataManager::STATUS) status;
    status_params[index].is_current = false;
}

/**
 * DataManager::getNumericParam()
 * 
 * @param index Parameter index in numeric_params[] vector
 * 
 * @return Numeric parameter value stored in numeric_params[] at index
 * 
 * Retrieve the numeric value stored in numeric_params[] at index
 */
float DataManager::getNumericParam(uint8_t index)
{
    return numeric_params[index].value;
}
/**
 * DataManager::getStatusParam()
 * 
 * @param index Parameter index in status_params[] vector
 * 
 * @return Status parameter value stored in status_params[] at index
 * 
 * Retrieve the status (non-numeric) value stored in status_params[] at index
 */
uint8_t DataManager::getStatusParam(uint8_t index)
{
    return status_params[index].status;
}

/**
 * DataManager::clearNumericParamFlag()
 * 
 * @param index Parameter index in numeric_params[] vector
 * 
 * Clear flag associated with the numeric value stored in numeric_params[]
 * at index
 * 
 * When flag is cleared (is_current == true) DataManager will not attempt
 * to publish parameter value to display
 */
void DataManager::clearNumericParamFlag(uint8_t index)
{
    numeric_params[index].is_current = true;
}

/**
 * DataManager::clearStatusParamFlag()
 * 
 * @param index Parameter index in status_params[] vector
 * 
 * Clear flag associated with the status value stored in status_params[]
 * at index
 * 
 * When flag is cleared (is_current == true) DataManager will not attempt
 * to publish parameter value to display
 */
void DataManager::clearStatusParamFlag(uint8_t index)
{
    status_params[index].is_current = true;
}

/**
 * DataManager::pollNumericParams()
 * 
 * Populate fresh_numeric_IDs[] and fresh_numeric_values[] vectors with
 * new numeric parameter ID/value pairs to be sent to display via I2C
 * 
 * Clear numeric parameter is_current flag for every parameter added
 */
void DataManager::pollNumericParams()
{
    for (uint8_t index = 0; index < NUMERIC_PARAM_COUNT; ++index)
    {
        if (numeric_params[index].is_current == false)
        {
            fresh_numeric_IDs.push_back(numeric_params[index].data_id);
            fresh_numeric_values.push_back(numeric_params[index].value);
            numeric_params[index].is_current = true;
        }
    }
}

/**
 * DataManager::pollStatusParams()
 * 
 * Populate fresh_status_IDs[] and fresh_status_values[] vectors with
 * new status parameter ID/value pairs to be sent to display via I2C
 * 
 * Clear numeric parameter is_current flag for every parameter added
 */
void DataManager::pollStatusParams()
{
    for (uint8_t index = 0; index < STATUS_PARAM_COUNT; ++index)
    {
        if (status_params[index].is_current == false)
        {
            fresh_status_IDs.push_back(status_params[index].data_id);
            fresh_status_values.push_back(status_params[index].status);
            status_params[index].is_current = true;
        }
    }
}

/**
 * DataManager::sendNumericParamI2C()
 * 
 * @param data_id Identifier shared by DataManager and Extruder_Display that
 * indicates the source/destination of the parameter
 * 
 * @param value Numeric parameter value to be sent to the display via I2C
 * 
 * Convert float value to 4 integer bytes
 * 
 * Send display I2C address followed by data_id and 4 bytes of numeric data
 */
void DataManager::sendNumericParamI2C(uint8_t data_id, float value)
{
    converter.numeric_param_input = value;
    
    I2C_2_IS_BUSY = true;
    I2C_2_Start();
    CORETIMER_DelayUs(5);
    I2C_2_Send_Byte(DISPLAY_I2C_ADDRESS << 1);
    CORETIMER_DelayUs(10);
    I2C_2_Send_Byte( data_id );
    CORETIMER_DelayUs(10);
    I2C_2_Send_Byte( converter.buffer[0] );
    CORETIMER_DelayUs(10);
    I2C_2_Send_Byte( converter.buffer[1] );
    CORETIMER_DelayUs(10);
    I2C_2_Send_Byte( converter.buffer[2] );
    CORETIMER_DelayUs(10);
    I2C_2_Send_Byte( converter.buffer[3] );
    CORETIMER_DelayUs(10);
    I2C_2_Stop();
    I2C_2_IS_BUSY = false;
    converter.numeric_param_input = 0;
    CORETIMER_DelayUs(100);
}

/**
 * DataManager::sendStatusParamI2C()
 * 
 * @param data_id Identifier shared by DataManager and Extruder_Display that
 * indicates the source/destination of the parameter
 * 
 * @param value Status parameter value to be sent to the display via I2C
 * 
 * Send display I2C address followed by data_id and status which is an integer
 * byte that is translated by the display into a status parameter string value
 */
void DataManager::sendStatusParamI2C(uint8_t data_id, uint8_t status)
{
    I2C_2_IS_BUSY = true;
    I2C_2_Start();
    CORETIMER_DelayUs(5);
    I2C_2_Send_Byte(DISPLAY_I2C_ADDRESS << 1);
    CORETIMER_DelayUs(10);
    I2C_2_Send_Byte( data_id );
    CORETIMER_DelayUs(10);
    I2C_2_Send_Byte( status );
    CORETIMER_DelayUs(10);
    I2C_2_Stop();
    I2C_2_IS_BUSY = false;
    CORETIMER_DelayUs(100);
}

/**
 * DataManager::sendAllFreshNumericParams()
 * 
 * Iterate through the fresh_numeric_IDs[] and fresh_numeric_values[] vectors
 * and send every ID + value pair to the display via I2C
 */
void DataManager::sendAllFreshNumericParams()
{
    for (uint8_t index = 0; index < fresh_numeric_IDs.size(); index++)
    {
        sendNumericParamI2C(fresh_numeric_IDs[index], fresh_numeric_values[index]);
    }
    fresh_numeric_IDs.clear();
    fresh_numeric_values.clear();
}

/**
 * DataManager::sendAllFreshStatusParams()
 * 
 * Iterate through the fresh_status_IDs[] and fresh_status_values[] vectors
 * and send every ID + value pair to the display via I2C
 */
void DataManager::sendAllFreshStatusParams()
{
    for (uint8_t index = 0; index < fresh_status_IDs.size(); index++)
    {
        sendStatusParamI2C(fresh_status_IDs[index], fresh_status_values[index]);
    }
    fresh_status_IDs.clear();
    fresh_status_values.clear();
}


/**
 * DataManager::getFreshNumericIDs()
 * 
 * @return fresh_numeric_IDs vector containing the identifiers of all new 
 * numeric parameter values
 */
std::vector<uint8_t>& DataManager::getFreshNumericIDs()
{
    return fresh_numeric_IDs;
}

/**
 * DataManager::getFreshNumericValues()
 * 
 * @return fresh_numeric_values vector containing every new numeric
 * parameter value 
 */
std::vector<float>& DataManager::getFreshNumericValues()
{
    return fresh_numeric_values;
}

/**
 * DataManager::getFreshStatusIDs()
 * 
 * @return fresh_status_IDs vector containing the identifiers of all new
 * status parameters
 */
std::vector<uint8_t>& DataManager::getFreshStatusIDs()
{
    return fresh_status_IDs;
}

/**
 * DataManager::getFreshStatusValues()
 * 
 * @return fresh_status_values vector containing every new status
 * parameter value
 */
std::vector<uint8_t>& DataManager::getFreshStatusValues()
{
    return fresh_status_values;
}