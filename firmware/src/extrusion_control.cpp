/*******************************************************************************
 * Extruder_Controller
 * extrusion_control.cpp
 * Wilson Woods
 * 11.25.2021
 ******************************************************************************/

#include <cstdint>

#include "extrusion_control.h"
#include "config/default/peripheral/i2c/master/plib_i2c1_master.h"
#include "config/default/peripheral/tmr/plib_tmr2.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "globals.h"
#include "DataManager.h"
#include "I2C.h"
#include "I2CMotor.h"

const uint16_t CONTROLLER_1_I2C_ADDRESS = 0x0C;
const uint16_t CONTROLLER_2_I2C_ADDRESS = 0x04;

I2CMotor controller1(CONTROLLER_1_I2C_ADDRESS);
I2CMotor controller2(CONTROLLER_2_I2C_ADDRESS);

const uint8_t MOTOR_1_ID = 0;
const uint8_t MOTOR_2_ID = 1;
float current_motor_speed = 0;

float current_temp_1 = 0;
float current_temp_2 = 0;
float current_temp_3 = 0;
float current_diameter = 0;

EXTRUSION_CONTROL_DATA extrusion_controlData;

void EXTRUSION_CONTROL_Initialize ( void )
{
    extrusion_controlData.state = EXTRUSION_CONTROL_STATE_INIT;
}

void EXTRUSION_CONTROL_Tasks ( void )
{
    switch ( extrusion_controlData.state )
    {
        case EXTRUSION_CONTROL_STATE_INIT:
        {
            I2C_1_Init();
            CORETIMER_DelayMs(500);
            
            while(I2C_1_IS_BUSY);
            controller1.setMotorSpeed(MOTOR_1_ID, 100, -1);
            controller1.setMotorSpeed(MOTOR_2_ID, 100, 1);
            current_motor_speed = controller1.getMotorSpeed(0);
            current_motor_speed = (current_motor_speed / 255) * 100;
            globalDataManager.setNumericParam(ROLLER_SPEED_INDEX, current_motor_speed);
            
            while(I2C_1_IS_BUSY);
            controller2.setMotorSpeed(MOTOR_1_ID, 80, -1);
            current_motor_speed = controller2.getMotorSpeed(0);
            current_motor_speed = (current_motor_speed / 255) * 100;
            globalDataManager.setNumericParam(SPOOLER_SPEED_INDEX, current_motor_speed);
            
            bool appInitialized = true;

            if (appInitialized)
            {
                extrusion_controlData.state = EXTRUSION_CONTROL_STATE_SERVICE_TASKS;
            }
            break;
        }

        case EXTRUSION_CONTROL_STATE_SERVICE_TASKS:
        {
            current_temp_1 = globalDataManager.getNumericParam(ZONE_1_TEMP_INDEX);
            current_temp_2 = globalDataManager.getNumericParam(ZONE_2_TEMP_INDEX);
            current_temp_3 = globalDataManager.getNumericParam(ZONE_3_TEMP_INDEX);
            current_diameter = globalDataManager.getNumericParam(DIAMETER_INDEX);
            
            /*  
             * TO DO: Implement feedback control
             */
            
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
