/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <cstdint>
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "config/default/peripheral/tmr/plib_tmr2.h"
#include "config/default/peripheral/tmr/plib_tmr3.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"
#include "config/default/peripheral/icap/plib_icap1.h" 
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/tmr1/plib_tmr1.h"

#include "globals.h"




uint16_t icap1 = 0;
uint8_t icap_count = 0;
//uint16_t icap_pulse_midpoint_us = 0;
//float icap_period_us = 0;
//const uint16_t PULSE_CORRECTION_OFFSET = 500;
//float icap_pulse_width_sum = 0;
//float icap_period_sum = 0;
//uint8_t pulse_measurement_count = 0;
//uint8_t period_measurement_count = 0;
//const uint16_t NUMBER_OF_PULSE_MEASUREMENTS = 200;
//const uint16_t NUMBER_OF_PERIOD_MEASUREMENTS = 50;


bool INPUT_PERIOD_MEASURE = true;
bool INPUT_PULSE_MEASURE = false;



void TIMER2_EventHandler(uint32_t status, uintptr_t context)
{
}\

void ICAP1_EventHandler(uintptr_t context)
{
//    if (period_measurement_count < NUMBER_OF_PERIOD_MEASUREMENTS && INPUT_PERIOD_MEASURE)
//    {
//        if (icap_count == 0)
//        {
//            if (period_measurement_count > 0)
//            {
//                TMR1_Stop();
//                icap_period_sum += ((float)TMR1_CounterGet());
//            }
//            TMR1_Start();
//        }
//        
//        ++period_measurement_count;
//        
//        if (period_measurement_count > NUMBER_OF_PERIOD_MEASUREMENTS - 1)
//        {
//            TMR1_Stop();
//            icap_period_us = (icap_period_sum / (period_measurement_count));
//            INPUT_PERIOD_MEASURE = false;
//            INPUT_PULSE_MEASURE = true;
//        }
//    }
//    
//    if (pulse_measurement_count < NUMBER_OF_PULSE_MEASUREMENTS && INPUT_PULSE_MEASURE)
//    {
//        switch (icap_count)
//        {
//            case 0:
//                TMR1_Start();
//                break;
//            case 1:
//                TMR1_Stop();
//                icap_pulse_width_sum += ((float)TMR1_CounterGet());
//                break;
//            default:
//                break;
//        }
//        
//        ++pulse_measurement_count;
//        
//        if (pulse_measurement_count > NUMBER_OF_PULSE_MEASUREMENTS - 1)
//        {
//            icap_pulse_midpoint_us = (icap_pulse_width_sum / (pulse_measurement_count));
//            INPUT_PULSE_MEASURE = false;
//        }
//        
//    }
    
    if (icap_count == 0)
    {
        // heater_duty_delay = (uint16_t) icap_period_us * heater_duty_cycle;
        // CORETIMER_DelayUs(icap_pulse_midpoint_us);
        CORETIMER_DelayUs(heater_power_control_delay);
        PWM_OUT_1_Set();
        CORETIMER_DelayUs(10);
        PWM_OUT_1_Clear();
    }
    ++icap_count;
    if (icap_count > 1)
        icap_count = 0;
    icap1 = IC1BUF;
    TMR3 = 0;
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    TMR1_Initialize();
    
    TMR2_CallbackRegister(TIMER2_EventHandler,(uintptr_t)NULL);
    ICAP1_CallbackRegister(ICAP1_EventHandler,(uintptr_t)NULL);
    
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************b
 End of File
*/

