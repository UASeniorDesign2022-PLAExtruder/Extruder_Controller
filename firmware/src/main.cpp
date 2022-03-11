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

#include "globals.h"

uint16_t icap1 = 0;

void TIMER2_EventHandler(uint32_t status, uintptr_t context)
{
}\

void ICAP1_EventHandler(uintptr_t context)
{

    while (IC1BUF) { icap1 = IC1BUF; }
    CORETIMER_DelayUs(1350);
    PWM_OUT_1_Set();
    CORETIMER_DelayUs(10);
    PWM_OUT_1_Clear();
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
    

    PWM_OUT_1_OutputEnable();
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

