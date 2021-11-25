/*******************************************************************************
 * Extruder_Controller
 * TempSensor.h
 * Wilson Woods
 * 11.25.2021
 ******************************************************************************/

#ifndef TEMPSENSOR_H
#define	TEMPSENSOR_H

#include <cstdint>

class TempSensor
{
    public:
        
        TempSensor(uint8_t sensor_id);
        TempSensor(const TempSensor& orig);
        virtual ~TempSensor();
        float readTemp();
        
    private:
        
        bool SPI_INITIALIZED = false;
        bool READ_CELSIUS = false;
        uint8_t SENSOR_ID = 0;

};

#endif	/* TEMPSENSOR_H */
