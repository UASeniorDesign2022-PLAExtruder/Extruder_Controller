/*********************************************************************************
 * Extruder Controller
 * DataManager.cpp
 * 
 * 
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

void DataManager::setNumericParam(uint8_t index, float value)
{
    numeric_params[index].value = value;
    numeric_params[index].is_current = false;
}

void DataManager::setStatusParam(uint8_t index, uint8_t status)
{
    status_params[index].status = (DataManager::STATUS) status;
    status_params[index].is_current = false;
}

float DataManager::getNumericParam(uint8_t index)
{
    return numeric_params[index].value;
}

uint8_t DataManager::getStatusParam(uint8_t index)
{
    return status_params[index].status;
}

void DataManager::clearNumericParamFlag(uint8_t index)
{
    numeric_params[index].is_current = true;
}

void DataManager::clearStatusParamFlag(uint8_t index)
{
    status_params[index].is_current = true;
}

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

void DataManager::sendNumericParamI2C(uint8_t data_id, float value)
{
    converter.numeric_param_input = value;
    
    I2C_2_IS_BUSY = true;
    I2C_2_Start();
    CORETIMER_DelayUs(5);
    I2C_2_send_byte(DISPLAY_I2C_ADDRESS << 1);
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( data_id );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( converter.buffer[0] );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( converter.buffer[1] );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( converter.buffer[2] );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( converter.buffer[3] );
    CORETIMER_DelayUs(10);
    I2C_2_Stop();
    I2C_2_IS_BUSY = false;
    converter.numeric_param_input = 0;
    CORETIMER_DelayUs(200);
}

void DataManager::sendStatusParamI2C(uint8_t data_id, uint8_t status)
{
    I2C_2_IS_BUSY = true;
    I2C_2_Start();
    CORETIMER_DelayUs(5);
    I2C_2_send_byte(DISPLAY_I2C_ADDRESS << 1);
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( data_id );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( status );
    CORETIMER_DelayUs(10);
    I2C_2_Stop();
    I2C_2_IS_BUSY = false;
    CORETIMER_DelayUs(200);
}

void DataManager::sendAllFreshNumericParams()
{
    for (uint8_t index = 0; index < fresh_numeric_IDs.size(); index++)
    {
        sendNumericParamI2C(fresh_numeric_IDs[index], fresh_numeric_values[index]);
    }
    fresh_numeric_IDs.clear();
    fresh_numeric_values.clear();
}
void DataManager::sendAllFreshStatusParams()
{
    for (uint8_t index = 0; index < fresh_status_IDs.size(); index++)
    {
        sendStatusParamI2C(fresh_status_IDs[index], fresh_status_values[index]);
    }
    fresh_status_IDs.clear();
    fresh_status_values.clear();
}

std::vector<uint8_t>& DataManager::getFreshNumericIDs()
{
    return fresh_numeric_IDs;
}
std::vector<float>& DataManager::getFreshNumericValues()
{
    return fresh_numeric_values;
}
std::vector<uint8_t>& DataManager::getFreshStatusIDs()
{
    return fresh_status_IDs;
}
std::vector<uint8_t>& DataManager::getFreshStatusValues()
{
    return fresh_status_values;
}