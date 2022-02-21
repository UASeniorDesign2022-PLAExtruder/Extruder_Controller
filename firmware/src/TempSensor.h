/*******************************************************************************
 * Extruder_Controller
 * TempSensor.h
 * Wilson Woods
 * 11.25.2021
 ******************************************************************************/

#ifndef TEMPSENSOR_H
#define	TEMPSENSOR_H

#include <cstdint>
#include <vector>

class TempSensor
{
    public:
        
        TempSensor( uint8_t sensor_id );
        virtual ~TempSensor();
        float read_temp( void );
        // float moving_avg( void );
        
    private:

        // static const uint8_t SIZE = 10;
        uint8_t buffer_index = 0;
        bool SPI_INITIALIZED = false;
        bool READ_CELSIUS = false;
        uint8_t SENSOR_ID = 0;
        // float buffer[SIZE];
};

#endif	/* TEMPSENSOR_H */
