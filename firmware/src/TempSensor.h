/*******************************************************************************
 * Extruder_Controller
 * TempSensor.h
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

#ifndef TEMPSENSOR_H
#define	TEMPSENSOR_H

#include <cstdint>      // unsigned int types

class TempSensor
{
    public:
        
        TempSensor( uint8_t sensor_id );
        virtual ~TempSensor();
        float read_temp( void );
        // float moving_avg( void );
        
    private:

        // static const uint8_t SIZE = 10;
        // uint8_t buffer_index = 0;
        bool SPI_INITIALIZED = false;
        bool READ_CELSIUS = false;
        uint8_t SENSOR_ID = 0;
        // float buffer[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
};

#endif	/* TEMPSENSOR_H */
 