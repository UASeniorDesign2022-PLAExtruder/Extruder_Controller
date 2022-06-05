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
#include <stdio.h>
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
#include "config/default/peripheral/evic/plib_evic.h"

#include "globals.h"




uint16_t icap1 = 0;
uint8_t icap_count = 0;


bool INPUT_PERIOD_MEASURE = true;
bool INPUT_PULSE_MEASURE = false;



void TIMER2_EventHandler(uint32_t status, uintptr_t context)
{
}\

void BUTTON_1_EventHandler( GPIO_PIN pin, uintptr_t context)
{
    if (BUTTON_1_Get() == 1)
        BUTTON_1_PRESSED = true;
    BUTTON_1_Clear();
}

void BUTTON_2_EventHandler( GPIO_PIN pin, uintptr_t context)
{
    if (BUTTON_2_Get() == 1)
        BUTTON_2_PRESSED = true;
    BUTTON_2_Clear();
}

void BUTTON_3_EventHandler( GPIO_PIN pin, uintptr_t context)
{
    if (BUTTON_3_Get() == 1)
        BUTTON_3_PRESSED = true;
    BUTTON_3_Clear();
}

void BUTTON_4_EventHandler( GPIO_PIN pin, uintptr_t context)
{
    if (BUTTON_4_Get() == 1)
        BUTTON_4_PRESSED = true;
    BUTTON_4_Clear();
}

void ICAP1_EventHandler(uintptr_t context)
{
    if (icap_count == 0)
    {   
        CORETIMER_DelayUs(first_delay);
        HEATER_CONTROL_2_Set();
        CORETIMER_DelayUs(100);
        HEATER_CONTROL_2_Clear();
        
        if (ZONE_1_ACTIVE)
        {
            CORETIMER_DelayUs(first_delay);
            HEATER_CONTROL_1_Set();
            CORETIMER_DelayUs(100);
            HEATER_CONTROL_1_Clear();
        }
        
        if (ZONE_2_ACTIVE)
        {
            CORETIMER_DelayUs(second_delay);
            HEATER_CONTROL_2_Set();
            CORETIMER_DelayUs(100);
            HEATER_CONTROL_2_Clear();
        }
        
        if (ZONE_3_ACTIVE)
        {
            CORETIMER_DelayUs(third_delay);
            HEATER_CONTROL_3_Set();
            CORETIMER_DelayUs(100);
            HEATER_CONTROL_3_Clear();
        } 
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
    
    HEATER_CONTROL_1_OutputEnable();
    HEATER_CONTROL_2_OutputEnable();
    HEATER_CONTROL_3_OutputEnable();
    
    
    // heater_power_control_delay = 2500;
    
    TMR2_CallbackRegister(TIMER2_EventHandler,(uintptr_t)NULL);

    ICAP1_CallbackRegister(ICAP1_EventHandler,(uintptr_t)NULL);
    
    GPIO_PinInterruptDisable(BUTTON_1_PIN);
    GPIO_PinInterruptDisable(BUTTON_2_PIN);
    GPIO_PinInterruptDisable(BUTTON_3_PIN);
    GPIO_PinInterruptDisable(BUTTON_4_PIN);
    GPIO_PinInterruptCallbackRegister(BUTTON_1_PIN, BUTTON_1_EventHandler, (uintptr_t)NULL);
    GPIO_PinInterruptCallbackRegister(BUTTON_3_PIN, BUTTON_3_EventHandler, (uintptr_t)NULL);
    GPIO_PinInterruptCallbackRegister(BUTTON_2_PIN, BUTTON_2_EventHandler, (uintptr_t)NULL);
    GPIO_PinInterruptCallbackRegister(BUTTON_4_PIN, BUTTON_4_EventHandler, (uintptr_t)NULL);
    GPIO_PinInterruptEnable(BUTTON_1_PIN);
    GPIO_PinInterruptEnable(BUTTON_2_PIN);
    GPIO_PinInterruptEnable(BUTTON_3_PIN);
    GPIO_PinInterruptEnable(BUTTON_4_PIN);




    
    
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

