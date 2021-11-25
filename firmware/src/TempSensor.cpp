/*******************************************************************************
 * Extruder_Controller
 * TempSensor.cpp
 * Wilson Woods
 * 11.25.2021
 ******************************************************************************/

#include <cstdint>

#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/spi/spi_master/plib_spi1_master.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "TempSensor.h"
#include "globals.h"
#include "SPI.h"

TempSensor::TempSensor(uint8_t sensor_id)
{
    SENSOR_ID = sensor_id;
    switch(SENSOR_ID)
    {
        case 1:
            SS_TEMP_1_OutputEnable();
            SS_TEMP_1_Set();
            break;
        case 2:
            SS_TEMP_2_OutputEnable();
            SS_TEMP_2_Set();
            break;
        case 3:
            SS_TEMP_3_OutputEnable();
            SS_TEMP_3_Set();
            break;
        default:
            break;
    }    
}

TempSensor::TempSensor(const TempSensor& orig)
{
    
}

TempSensor::~TempSensor()
{
    
}

float TempSensor::readTemp()
{
    switch(SENSOR_ID)
    {
        case 1:
            SPI1CONbits.DISSDO = 1;
            SS_TEMP_1_Clear();                      // set ss low
            SPI_Transfer(0x00);                     // send dummy byte
            CORETIMER_DelayUs(16);                  // wait 16 cycles for 2 bytes
            SPI1CONbits.DISSDO = 0;
            SS_TEMP_1_Set();                        // set ss high
            break;
        case 2:
            SPI1CONbits.DISSDO = 1;
            SS_TEMP_2_Clear();                      // set ss low
            SPI_Transfer(0x00);                     // send dummy byte
            CORETIMER_DelayUs(16);                  // wait 16 cycles for 2 bytes
            SPI1CONbits.DISSDO = 0;
            SS_TEMP_2_Set();                        // set ss high
            break;
        case 3:
            SPI1CONbits.DISSDO = 1;
            SS_TEMP_3_Clear();                      // set ss low
            SPI_Transfer(0x00);                     // send dummy byte
            CORETIMER_DelayUs(16);                  // wait 16 cycles for 2 bytes
            SPI1CONbits.DISSDO = 0;
            SS_TEMP_3_Set();                        // set ss high
            break;
        default:
            break; 
    }
    
    int rawTemp = SPI1BUF;                          // read incoming data from buffer
    rawTemp >>= 3;
    float celsiusTemp = (float)rawTemp;
    if (READ_CELSIUS == true)
        return celsiusTemp;
    else
        return ((celsiusTemp * (9 / 5)) + 32);
}
