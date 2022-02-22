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
#include "config/default/peripheral/gpio/plib_gpio.h"

#include "globals.h"
#include "DataManager.h"
#include "I2C.h"
#include "I2CMotor.h"
#include "pwm.h"

/***************************** Initializations ********************************/

const uint8_t M1 = 0;        // refer to motors on either I2C controller
const uint8_t M2 = 1;

const uint16_t CONTROLLER_1_I2C_ADDRESS = 0x0C; // rollers on controller 1
const uint16_t CONTROLLER_2_I2C_ADDRESS = 0x04; // spooler on controller 2

I2CMotor rollers( CONTROLLER_1_I2C_ADDRESS );   // controller objects
I2CMotor spooler( CONTROLLER_2_I2C_ADDRESS );

uint16_t tension_lower_limit = 100;             // pressure reading limits
uint16_t tension_upper_limit = 750;             // adjust for filament tension

// local variables
float roller_speed = 0;
float spooler_speed = 0;
float zone1 = 0;
float zone2 = 0;
float zone3 = 0;
float current_diameter = 0;

/*
void __ISR(_TIMER_2_VECTOR, IPL7AUTO) T2_IntHandler (void)
{
    IFS0CLR = 0x0100;
}
*/




EXTRUSION_CONTROL_DATA extrusion_controlData;   // hold thread FSM state

/******************************************************************************/

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
            // initialize PWM with period (PR2) = 399
            // initial duty cycle = 200 (50%) since
            // max duty = PR2 + 1 = 400
            // higher pre-scale = lower base frequency for TMR2
            // ( and in turn lower PWM frequency )
            // larger period = lower PWM frequency
            pwm_init(399U, 200U, TMR2_PRESCALE_64);
            PWM_OUT_1_Set();
            // PWM_OUT_2_Set();
                    
            I2C_1_init();
            CORETIMER_DelayMs( 100 );
            rollers.stop(M1);
            rollers.stop(M2);
            spooler.stop(M1);
            CORETIMER_DelayMs( 5000 );

            rollers.set_speed( M1, 150 );
            roller_speed = rollers.set_speed( M2, 150 );
            spooler_speed = spooler.set_speed( M1, 150 );
            dataManager.set_numeric_param( ROLLER_SPEED_INDEX, roller_speed );
            dataManager.set_numeric_param( SPOOLER_SPEED_INDEX, spooler_speed );
            
            bool appInitialized = true;

            if ( appInitialized )
                extrusion_controlData.state = EXTRUSION_CONTROL_STATE_SERVICE_TASKS;

            break;
        }

        case EXTRUSION_CONTROL_STATE_SERVICE_TASKS:
        {
            zone1 = dataManager.get_numeric_param( ZONE_1_TEMP_INDEX );
            zone2 = dataManager.get_numeric_param( ZONE_2_TEMP_INDEX );
            zone3 = dataManager.get_numeric_param( ZONE_3_TEMP_INDEX );
            current_diameter = dataManager.get_numeric_param( DIAMETER_INDEX );
            
            pwm_set_duty_cycle(100);
            CORETIMER_DelayMs(1000);
            
            pwm_set_duty_cycle(50);
            CORETIMER_DelayMs(1000);
            
            pwm_set_duty_cycle(150);
            CORETIMER_DelayMs(1000);
            
            pwm_set_duty_cycle(300);
            CORETIMER_DelayMs(1000);
            
            
            /*
            while(1)
            {
                spooler_speed = spooler.nudge_down(M1, 1);
                rollers.nudge_down(M1, 1);
                roller_speed = rollers.nudge_down(M2, 1);
                dataManager.set_numeric_param( ROLLER_SPEED_INDEX, roller_speed );
                dataManager.set_numeric_param( SPOOLER_SPEED_INDEX, spooler_speed );
                CORETIMER_DelayMs( 750 );
            }
            */
            
            /*
            if ( dataManager.get_spooler_tension() > tension_upper_limit )
            {
                // spooler_speed = spooler.nudge_down( M1, 1 );
                SP_TENSION_LED_Set();
                // dataManager.set_numeric_param( SPOOLER_SPEED_INDEX, spooler_speed );
            }
            */
            /*
            else if ( dataManager.get_spooler_tension() < tension_lower_limit )
            {
                spooler_speed = spooler.nudge_up( M1, 1 );
                SP_TENSION_LED_Set();
                dataManager.set_numeric_param( SPOOLER_SPEED_INDEX, spooler_speed );
            }
            */
            /*
            else
            {
                SP_TENSION_LED_Clear();
            }
            */
            CORETIMER_DelayMs( 500 );
            
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
