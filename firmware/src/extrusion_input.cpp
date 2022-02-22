/*******************************************************************************
 * Extruder_Controller
 * extrusion_input.cpp
 * Wilson Woods
 * 11.25.2021
 ******************************************************************************/

#include <cstdint>                                              // int types

#include "extrusion_input.h"
#include "config/default/peripheral/gpio/plib_gpio.h"           // tension LED
#include "config/default/peripheral/coretimer/plib_coretimer.h" // timers
#include "config/default/peripheral/adc/plib_adc.h"             // pressure ADC

#include "globals.h"                                            // dataManager
#include "SPI.h"
#include "TempSensor.h"
#include "DataManager.h"

/***************************** Initializations ********************************/

#define ADC_VREF        (5.0f)  
#define ADC_MAX_COUNT   (1023U)


TempSensor zone_1( 1 );                     // temperature sensor objects
TempSensor zone_2( 2 );
TempSensor zone_3( 3 );

// local variables
float z1 = 0;                               // zone temperatures
float z2 = 0;
float z3 = 0;
uint16_t adc_count;                         // pressure sensor reading

EXTRUSION_INPUT_DATA extrusion_inputData;   // hold thread FSM state

/******************************************************************************/

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
            SPI_init();                             // initialize SPI for
            CORETIMER_DelayUs ( 50 );               // temp. sensor reading   
            SP_TENSION_LED_OutputEnable();
            bool appInitialized = true;

            if( appInitialized )
                extrusion_inputData.state = EXTRUSION_INPUT_STATE_SERVICE_TASKS;
            break;
        }

        case EXTRUSION_INPUT_STATE_SERVICE_TASKS:
        {
            
            //ADC_ConversionStart();                  // read pressure ADC to
            //while( !ADC_ResultIsReady() );          // determine spooler tension
            //adc_count = ADC_ResultGet( ADC_RESULT_BUFFER_0 );
            // global_spooler_tension = adc_count;
            //dataManager.set_spooler_tension( adc_count );
            
            z1 = zone_1.read_temp();
            dataManager.set_numeric_param( ZONE_1_TEMP_INDEX, z1 );
            z1 = 0;
            CORETIMER_DelayUs( 10 );
            z2 = zone_2.read_temp();
            dataManager.set_numeric_param( ZONE_2_TEMP_INDEX, z2 );
            z2 = 0;
            CORETIMER_DelayUs( 10 );
            z3 = zone_3.read_temp();
            dataManager.set_numeric_param( ZONE_3_TEMP_INDEX, z3 );
            z3 = 0;
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
