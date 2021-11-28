/*******************************************************************************
 * Extruder_Controller
 * extrusion_input.cpp
 * Wilson Woods
 * 11.25.2021
 ******************************************************************************/

#include <cstdint>

#include "extrusion_input.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "globals.h"
#include "SPI.h"
#include "TempSensor.h"
#include "DataManager.h"

float temp_1_float = 0;
float temp_2_float = 0;
float temp_3_float = 0;

TempSensor tempSensor1(1);
TempSensor tempSensor2(2);
TempSensor tempSensor3(3);

EXTRUSION_INPUT_DATA extrusion_inputData;

void EXTRUSION_INPUT_Initialize ( void )
{
    extrusion_inputData.state = EXTRUSION_INPUT_STATE_INIT;
}

void EXTRUSION_INPUT_Tasks ( void )
{
    switch ( extrusion_inputData.state )
    {
        case EXTRUSION_INPUT_STATE_INIT:
        {
            SPI_Init();
            CORETIMER_DelayUs(50);
            bool appInitialized = true;


            if (appInitialized)
            {
                extrusion_inputData.state = EXTRUSION_INPUT_STATE_SERVICE_TASKS;
            }
            break;
        }

        case EXTRUSION_INPUT_STATE_SERVICE_TASKS:
        {
            temp_1_float = tempSensor1.readTemp();
            globalDataManager.setNumericParam(ZONE_1_TEMP_INDEX, temp_1_float);
            temp_1_float = 0;
            CORETIMER_DelayUs(10);
            
            temp_2_float = tempSensor2.readTemp();
            globalDataManager.setNumericParam(ZONE_2_TEMP_INDEX, temp_2_float);
            temp_2_float = 0;
            CORETIMER_DelayUs(10);
            
            temp_3_float = tempSensor3.readTemp();
            globalDataManager.setNumericParam(ZONE_3_TEMP_INDEX, temp_3_float);
            temp_3_float = 0;
            CORETIMER_DelayMs(500);
            
            break;
        }

        default:
        {
            break;
        }
    }
}

/*******************************************************************************
 End of File
 */
