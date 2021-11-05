/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    extrusion.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <cstdint>
#include "extrusion.h"
#include "config/default/peripheral/i2c/master/plib_i2c1_master.h"
#include "config/default/peripheral/tmr/plib_tmr2.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"
#include "I2C.h"
#include "I2CMotor.h"

EXTRUSION_DATA extrusionData;

// uint8_t F_3921Hz    = 0x02;

// controller 1: motors 1 & 2
// controller 2: motor 3
const uint16_t CONTROLLER_1_I2C_ADDRESS = 0x0C;
const uint16_t DISPLAY_I2C_ADDRESS      = 0x14;


I2CMotor controller1;


/*******************************************************************************
  Function:
    void EXTRUSION_Initialize ( void )

  Remarks:
    See prototype in extrusion.h.
 */

void EXTRUSION_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    extrusionData.state = EXTRUSION_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void EXTRUSION_Tasks ( void )

  Remarks:
    See prototype in extrusion.h.
 */

void EXTRUSION_Tasks ( void )
{
    switch ( extrusionData.state )
    {
        case EXTRUSION_STATE_INIT:
        {
            I2Cinit();
            controller1.setPWMFrequency(0x02, CONTROLLER_1_I2C_ADDRESS);
            CORETIMER_DelayMs(500);
            
            bool appInitialized = true;

            if (appInitialized)
            {
                extrusionData.state = EXTRUSION_STATE_SERVICE_TASKS;
            }
            break;
        }

        case EXTRUSION_STATE_SERVICE_TASKS:
        {
            /* display test */
            /*
            while(1)
            {
                I2CStart();
                CORETIMER_DelayUs(5);
                I2Csendbyte(DISPLAY_I2C_ADDRESS << 1);
                CORETIMER_DelayUs(10);
                I2Csendbyte( 0x03 );
                CORETIMER_DelayUs(10);
                I2Csendbyte( 0x05 );
                CORETIMER_DelayUs(10);
                I2CStop();
                CORETIMER_DelayUs(200);
            }
            */
            
            /* motor control test */
           
            while(1)
            {
                CORETIMER_DelayMs(2000);
                controller1.setMotorSpeed(1, 255, CONTROLLER_1_I2C_ADDRESS);
                controller1.setMotorSpeed(2, 255, CONTROLLER_1_I2C_ADDRESS);
                CORETIMER_DelayMs(2000);
                controller1.stopMotor(1, CONTROLLER_1_I2C_ADDRESS);
                controller1.stopMotor(2, CONTROLLER_1_I2C_ADDRESS);
                CORETIMER_DelayMs(2000);
                controller1.setMotorSpeed(1, -255, CONTROLLER_1_I2C_ADDRESS);
                controller1.setMotorSpeed(2, -255, CONTROLLER_1_I2C_ADDRESS);
                CORETIMER_DelayMs(2000);
                controller1.stopMotor(1, CONTROLLER_1_I2C_ADDRESS);
                controller1.stopMotor(2, CONTROLLER_1_I2C_ADDRESS);
            }
            
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
