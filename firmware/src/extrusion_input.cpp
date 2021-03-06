/*******************************************************************************
 * Extruder_Controller
 * extrusion_input.cpp
 * Wilson Woods
 * 11.25.2021
 * 
 * extrusion_input handles input to the extrusion stage, namely temperature
 * readings (for temperature control) and pressure sensor (ADC) readings for
 * dynamic tension control of the spooling system.
 ******************************************************************************/

#include <cstdint>                                              // int types

#include "config/default/peripheral/gpio/plib_gpio.h"           // tension LED
#include "config/default/peripheral/coretimer/plib_coretimer.h" // timers
#include "config/default/peripheral/adc/plib_adc.h"             // pressure ADC

#include "extrusion_input.h"
#include "globals.h"                                            // dataManager
#include "SPI.h"
// #include "TempSensor.h"
#include "DataManager.h"
#include "RTDSensor.h"

/***************************** Initializations ********************************/

#define R_NOMINAL_100       100.0
#define R_REF_430           430.0

#define R_NOMINAL_1000      1000.0
#define R_REF_4300          4300.0



#define ADC_VREF        (5.0f)  
#define ADC_MAX_COUNT   (1023U)



typedef enum max31865_numwires
{
  MAX31865_2WIRE = 0,
  MAX31865_3WIRE = 1,
  MAX31865_4WIRE = 0
} max31865_numwires_t;


RTDSensor RTD_zone_2(R_NOMINAL_100, R_REF_430, SS_TEMP_1_PIN);
// RTDSensor RTD_zone_3(3,  R_NOMINAL_100, R_REF_430);

// TempSensor zone_1( 1 );                     // temperature sensor objects
// TempSensor zone_2( 2 );
// TempSensor zone_3( 3 );

// local variables
float z1 = 0;                               // zone temperatures
//float z2 = 0;
//float z3 = 0;
uint16_t adc;                               // pressure sensor reading

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
//            SS_TEMP_1_OutputEnable();
//            SS_TEMP_1_Set();
            //SPI_RTD_init();
            // SPI_init();
            // RTD_begin(MAX31865_3WIRE);
            // CORETIMER_DelayUs ( 50 );
            /*
            SP_TENSION_LED_OutputEnable();          // LED ON 
            */
            bool appInitialized = true;

            if( appInitialized )
                extrusion_inputData.state = EXTRUSION_INPUT_STATE_SERVICE_TASKS;
            break;
        }

        case EXTRUSION_INPUT_STATE_SERVICE_TASKS:
        {
            
            // setWires(MAX31865_3WIRE);
            // enableBias(false);
//            RTD_begin(MAX31865_3WIRE);
            
            // z1 = temperature(RNOMINAL, RREF);
//            uint16_t raw_res = readRTD();
//            raw_res = raw_res / 2;
//            z1 = (raw_res * 430) / 32768;
            
//              RTD_sensors.get_all_temps();
//              dataManager.set_numeric_param( ZONE_1_TEMP_INDEX, temp_1 );
//              dataManager.set_numeric_param( ZONE_2_TEMP_INDEX, temp_2 );
//              dataManager.set_numeric_param( ZONE_3_TEMP_INDEX, temp_3 );
              
              
            z1 = RTD_zone_2.get_temp();
            dataManager.set_numeric_param( ZONE_1_TEMP_INDEX, z1 );
            CORETIMER_DelayUs(10);
//            z2 = RTD_sensors.get_temp_CS_2();
//            dataManager.set_numeric_param( ZONE_2_TEMP_INDEX, z2 );
//            CORETIMER_DelayUs(100);
//            z3 = RTD_sensors.get_temp_CS_3();
//            dataManager.set_numeric_param( ZONE_3_TEMP_INDEX, z3 );
//            CORETIMER_DelayUs(100);
//            
            
            
//            ADC_ConversionStart();                      // start ADC
//            while( !ADC_ResultIsReady() );              // wait for result
//            adc = ADC_ResultGet( ADC_RESULT_BUFFER_0 ); // store ADC reading
//            dataManager.set_spooler_tension( adc );     // pass to dataManager
            
            
            // read and store zone 1 temperature
//            z1 = zone_1.read_temp();
//            dataManager.set_numeric_param( ZONE_1_TEMP_INDEX, z1 );
//            z1 = 0;
//            CORETIMER_DelayUs( 10 );
//            
            // read and store zone 2 temperature
//            z2 = zone_2.read_temp();
//            dataManager.set_numeric_param( ZONE_2_TEMP_INDEX, z2 );
//            z2 = 0;
//            CORETIMER_DelayUs( 10 );
//            
//            // read and store zone 3 temperature
//            z3 = zone_3.read_temp();
//            dataManager.set_numeric_param( ZONE_3_TEMP_INDEX, z3 );
//            z3 = 0;
//            CORETIMER_DelayUs( 10 );
            
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
