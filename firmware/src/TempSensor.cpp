/*******************************************************************************
 * Extruder_Controller
 * TempSensor.cpp
 * Wilson Woods
 * 11.25.2021
 * 
 * The TempSensor class provides an interface for reading values from the
 * MAX6675 temperature controller using the PIC32MX470F512H:
 * 
 *      https://cdn-shop.adafruit.com/datasheets/MAX6675.pdf
 * 
 * Each MAX6675 is connected to a type-k thermocouple and values from the
 * interface are read via SPI. For each read, 2 bytes are read successively (1 
 * read in 16-bit SPI mode). The 3 LSBs of the reading provide device ID and
 * other state information, and are shifted to give the thermocouple reading
 * in degrees Celsius.
 ******************************************************************************/

#include <cstdint>

#include "config/default/peripheral/gpio/plib_gpio.h"   // GPIO macros

// plib_spi1_master.h used only for bit mask definitions
#include "config/default/peripheral/spi/spi_master/plib_spi1_master.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "TempSensor.h"
#include "globals.h"        // dataManager symbolic constants
#include "SPI.h"            // SPI functions implemented from scratch


/**
 * TempSensor()
 * 
 * @param sensor_id Number 1-3 (or 1-5 for 5-sensor implementation)
 * 
 * Sets the SENSOR ID data member
 * Enables output on the appropriate chip select line and sets the line high
 * 
 */
TempSensor::TempSensor( uint8_t sensor_id )
{
    SENSOR_ID = sensor_id;
    
    // initialize and set chip select line for given temperature sensor
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

/**
 * read_temp()
 * 
 * Lowers the appropriate chip select line
 * Waits for 2 bytes (16us @ 1MHz) and raises chip select line again
 * Reads SPI1BUF and casts result to float
 * 
 * @return Celsius temperature for  zone (if READ_CELSIUS member == true),
 *         Fahrenheit temperature for zone otherwise
 */
float TempSensor::read_temp()
{
    switch( SENSOR_ID )
    {
        case 1:
            SPI1CONbits.DISSDO = 1;
            SS_TEMP_1_Clear();                      // set SS low
            SPI_transfer( 0x00 );                   // send dummy byte
            CORETIMER_DelayUs( 16 );                // wait 16 cycles, 2 bytes
            SPI1CONbits.DISSDO = 0;
            SS_TEMP_1_Set();                        // set SS high
            break;
        case 2:
            SPI1CONbits.DISSDO = 1;
            SS_TEMP_2_Clear();                      // set SS low
            SPI_transfer( 0x00 );                   // send dummy byte
            CORETIMER_DelayUs( 16 );                // wait 16 cycles, 2 bytes
            SPI1CONbits.DISSDO = 0;
            SS_TEMP_2_Set();                        // set SS high
            break;
        case 3:
            SPI1CONbits.DISSDO = 1;
            SS_TEMP_3_Clear();                      // set SS low
            SPI_transfer( 0x00 );                   // send dummy byte
            CORETIMER_DelayUs( 16 );                // wait 16 cycles, 2 bytes
            SPI1CONbits.DISSDO = 0;
            SS_TEMP_3_Set();                        // set SS high
            break;
        default:
            break; 
    }
    
    int raw_temp = SPI1BUF;                 // read incoming data
    raw_temp >>= 3;                         // 3 LSBs are junk (to us anyway)
    float celsius_temp = (float)raw_temp;   // cast to float for dataManager    
    
    /*
    if ( buffer_index < 10 )
    {
        buffer[buffer_index] = celsius_temp;
        buffer_index++;
    }
    else
    {
        uint8_t index = 8;
        // shift samples in FIFO buffer
        while ( index > 0)
        {
            buffer[index + 1] = buffer[index];
            index--;
        }
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
    while ( index < 10 )
    {
        avg += buffer[index];
        index++;
    }
    return (avg / 10.0);
}
*/