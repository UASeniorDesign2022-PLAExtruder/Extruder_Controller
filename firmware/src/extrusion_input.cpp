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
#include "config/default/peripheral/adc/plib_adc.h"

#include "globals.h"
#include "SPI.h"
#include "TempSensor.h"
#include "DataManager.h"

float pressure_input = 0;

#define ADC_VREF                (5.0f)
#define ADC_MAX_COUNT           (1023U)

uint16_t adc_count;

float temp_1_float = 0;
float temp_2_float = 0;
float temp_3_float = 0;

TempSensor tempSensor1( 1 );
TempSensor tempSensor2( 2 );
TempSensor tempSensor3( 3 );

EXTRUSION_INPUT_DATA extrusion_inputData;

void EXTRUSION_INPUT_Initialize( void )
{
    extrusion_inputData.state = EXTRUSION_INPUT_STATE_INIT;
}

void EXTRUSION_INPUT_Tasks( void )
{
    switch( extrusion_inputData.state )
    {
        case EXTRUSION_INPUT_STATE_INIT:
        {
            SPI_init();
            CORETIMER_DelayUs ( 50 );
            SP_TENSION_LED_OutputEnable();
            bool appInitialized = true;

            if( appInitialized )
                extrusion_inputData.state = EXTRUSION_INPUT_STATE_SERVICE_TASKS;
            break;
        }

        case EXTRUSION_INPUT_STATE_SERVICE_TASKS:
        {
            
            ADC_ConversionStart();
            while( !ADC_ResultIsReady() );
            adc_count = ADC_ResultGet( ADC_RESULT_BUFFER_0 );
            globalDataManager.set_spooler_tension( adc_count );
            if ( adc_count > 200 )
            {
                SP_TENSION_LED_Set();
            }
            else
            {
                SP_TENSION_LED_Clear();
            }
                
            temp_1_float = tempSensor1.read_temp();
            globalDataManager.set_numeric_param( ZONE_1_TEMP_INDEX, temp_1_float );
            temp_1_float = 0;
            CORETIMER_DelayUs( 10 );
            
            temp_2_float = tempSensor2.read_temp();
            globalDataManager.set_numeric_param( ZONE_2_TEMP_INDEX, temp_2_float );
            temp_2_float = 0;
            CORETIMER_DelayUs( 10 );
            
            temp_3_float = tempSensor3.read_temp();
            globalDataManager.set_numeric_param( ZONE_3_TEMP_INDEX, temp_3_float );
            temp_3_float = 0;
            CORETIMER_DelayMs( 50 );
            
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
