/*********************************************************************************
 * Extruder Controller
 * Extrusion.cpp
 * 
 * 
 ********************************************************************************/

#include <cstdint>

#include "config/default/peripheral/i2c/master/plib_i2c1_master.h"
#include "config/default/peripheral/tmr/plib_tmr2.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "extrusion.h"
#include "globals.h"
#include "DataManager.h"
#include "I2C.h"
#include "I2CMotor.h"

EXTRUSION_DATA extrusionData;
float current_motor_speed = 0;

// uint8_t F_3921Hz    = 0x02;

// controller 1: motors 1 & 2
// controller 2: motor 3

const uint8_t MOTOR_1_ID = 0;
const uint8_t MOTOR_2_ID = 1;

const uint16_t CONTROLLER_1_I2C_ADDRESS = 0x0C;

I2CMotor controller1(CONTROLLER_1_I2C_ADDRESS);


void EXTRUSION_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    extrusionData.state = EXTRUSION_STATE_INIT;
}

void EXTRUSION_Tasks ( void )
{
    switch ( extrusionData.state )
    {
        case EXTRUSION_STATE_INIT:
        {
            I2C_1_init();
            controller1.setPWMFrequency(0x02);
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
            /* motor control test */
           

            CORETIMER_DelayMs(2000);
            while(I2C_1_IS_BUSY);
            controller1.setMotorSpeed(MOTOR_1_ID, 255, 1);
            // while(I2C_1_IS_BUSY);
            controller1.setMotorSpeed(MOTOR_2_ID, 255, 1);
            current_motor_speed = controller1.getMotorSpeed(0);
            globalDataManager.setNumericParam(ROLLER_SPEED_INDEX, current_motor_speed);
            
            CORETIMER_DelayMs(2000);
            
            while(I2C_1_IS_BUSY);
            controller1.stopMotor(MOTOR_1_ID);
            // while(I2C_1_IS_BUSY);
            controller1.stopMotor(MOTOR_2_ID);
            current_motor_speed = controller1.getMotorSpeed(0);
            globalDataManager.setNumericParam(ROLLER_SPEED_INDEX, current_motor_speed);
            
            CORETIMER_DelayMs(2000);
            
            while(I2C_1_IS_BUSY);
            controller1.setMotorSpeed(MOTOR_1_ID, 150, -1);
            // while(I2C_1_IS_BUSY);
            controller1.setMotorSpeed(MOTOR_2_ID, 150, -1);
            current_motor_speed = controller1.getMotorSpeed(0);
            globalDataManager.setNumericParam(ROLLER_SPEED_INDEX, current_motor_speed);
            
            CORETIMER_DelayMs(2000);
            
            while(I2C_1_IS_BUSY);
            controller1.stopMotor(MOTOR_1_ID);
            // while(I2C_1_IS_BUSY);
            controller1.stopMotor(MOTOR_2_ID);
            current_motor_speed = controller1.getMotorSpeed(0);
            globalDataManager.setNumericParam(ROLLER_SPEED_INDEX, current_motor_speed);

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
