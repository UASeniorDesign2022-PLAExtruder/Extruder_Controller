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

uint16_t tension_lower_limit = 200;
uint16_t tension_upper_limit = 800;

I2CMotor rollers( CONTROLLER_1_I2C_ADDRESS );
I2CMotor spooler( CONTROLLER_2_I2C_ADDRESS );

const uint8_t MOTOR_1_ID = 0;
const uint8_t MOTOR_2_ID = 1;
float current_motor_speed = 0;

float current_temp_1 = 0;
float current_temp_2 = 0;
float current_temp_3 = 0;
float current_diameter = 0;

EXTRUSION_CONTROL_DATA extrusion_controlData;

void EXTRUSION_CONTROL_Initialize( void )
{
    extrusion_controlData.state = EXTRUSION_CONTROL_STATE_INIT;
}

void EXTRUSION_CONTROL_Tasks( void )
{
    switch ( extrusion_controlData.state )
    {
        case EXTRUSION_CONTROL_STATE_INIT:
        {
            I2C_1_init();
            CORETIMER_DelayMs( 500 );
            
            while( I2C_1_IS_BUSY );
            rollers.set_motor_speed( MOTOR_1_ID, 80, -1 );
            rollers.set_motor_speed( MOTOR_2_ID, 80, 1 );
            current_motor_speed = rollers.get_motor_speed( 0 );
            current_motor_speed = ( current_motor_speed / 255 ) * 100;
            globalDataManager.set_numeric_param( ROLLER_SPEED_INDEX, current_motor_speed );
            
            while( I2C_1_IS_BUSY );
            spooler.set_motor_speed( MOTOR_1_ID, 80, -1 );
            current_motor_speed = spooler.get_motor_speed( 0 );
            current_motor_speed = ( current_motor_speed / 255 ) * 100;
            globalDataManager.set_numeric_param( SPOOLER_SPEED_INDEX, current_motor_speed );
            
            bool appInitialized = true;

            if ( appInitialized )
                extrusion_controlData.state = EXTRUSION_CONTROL_STATE_SERVICE_TASKS;

            break;
        }

        case EXTRUSION_CONTROL_STATE_SERVICE_TASKS:
        {
            current_temp_1 = globalDataManager.get_numeric_param( ZONE_1_TEMP_INDEX );
            current_temp_2 = globalDataManager.get_numeric_param( ZONE_2_TEMP_INDEX );
            current_temp_3 = globalDataManager.get_numeric_param( ZONE_3_TEMP_INDEX );
            current_diameter = globalDataManager.get_numeric_param( DIAMETER_INDEX );
            
            if ( globalDataManager.get_spooler_tension() > tension_upper_limit )
            {
                spooler.nudge_motor_speed_down( MOTOR_1_ID, 1 );
            }
            
            if ( globalDataManager.get_spooler_tension() < tension_lower_limit )
            {
                spooler.nudge_motor_speed_up( MOTOR_1_ID, 1 );
            }
            
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
