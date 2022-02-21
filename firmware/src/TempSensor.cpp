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

TempSensor::TempSensor( uint8_t sensor_id )
{
    SENSOR_ID = sensor_id;
    switch( SENSOR_ID )
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

TempSensor::~TempSensor()
{
    
}

float TempSensor::read_temp()
{
    switch( SENSOR_ID )
    {
        case 1:
            SPI1CONbits.DISSDO = 1;
            SS_TEMP_1_Clear();                      // set SS low
            SPI_transfer( 0x00 );                   // send dummy byte
            CORETIMER_DelayUs( 16 );                // wait 16 cycles for 2 bytes
            SPI1CONbits.DISSDO = 0;
            SS_TEMP_1_Set();                        // set SS high
            break;
        case 2:
            SPI1CONbits.DISSDO = 1;
            SS_TEMP_2_Clear();                      // set SS low
            SPI_transfer( 0x00 );                   // send dummy byte
            CORETIMER_DelayUs( 16 );                // wait 16 cycles for 2 bytes
            SPI1CONbits.DISSDO = 0;
            SS_TEMP_2_Set();                        // set SS high
            break;
        case 3:
            SPI1CONbits.DISSDO = 1;
            SS_TEMP_3_Clear();                      // set SS low
            SPI_transfer( 0x00 );                   // send dummy byte
            CORETIMER_DelayUs( 16 );                // wait 16 cycles for 2 bytes
            SPI1CONbits.DISSDO = 0;
            SS_TEMP_3_Set();                        // set SS high
            break;
        default:
            break; 
    }
    
    int raw_temp = SPI1BUF;                          // read incoming data
    raw_temp >>= 3;                                  // 3 LSBs aren't data
    float celsius_temp = (float)raw_temp;
    
    /*
    if ( buffer_index < SIZE )
    {
        buffer[buffer_index] = celsius_temp;
    }
    else
    {
        uint8_t index = 0;
        // shift samples in FIFO buffer
        while ( index < SIZE - 1) { buffer[index + 1] = buffer[index]; }
        // new sample to first position
        buffer[0] = celsius_temp;
    }
    */
    if ( READ_CELSIUS == true )
        return celsius_temp;
    else
        return ( ( celsius_temp * ( 9 / 5 ) ) + 32 );
}

/*
float TempSensor::moving_avg()
{
    float avg = 0;
    uint8_t index = 0;
    while ( index < SIZE ) { avg += buffer[index]; }
    ++index;
    // return (float)(avg / index);
    return 0.0;
}
*/
