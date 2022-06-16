/*******************************************************************************
 * Extruder_Controller
 * extrusion_control.cpp
 * Wilson Woods
 * 11.25.2021
 * 
 * extrusion_control handles the control activities of the extrusion stage.
 * This includes the main (screw) motor controlled directly by PWM,
 * roller and spooling motors controlled by I2C motor controllers (PWM also),
 * and the heating element (in progress).
 ******************************************************************************/

#include <cstdint>                                      // unsigned int types

#include "extrusion_control.h"
#include "config/default/peripheral/tmr/plib_tmr2.h"    // PWM timer
#include "config/default/peripheral/tmr/plib_tmr3.h"    // ICAP timer
#include "config/default/peripheral/coretimer/plib_coretimer.h"
#include "config/default/peripheral/gpio/plib_gpio.h"       // GPIO macros
#include "config/default/peripheral/icap/plib_icap1.h" // capture zero crossings

#include "globals.h"            // parameter indexes, IDs
#include "DataManager.h"
#include "I2C.h"
#include "I2CMotor.h"
//#include "temp_control.h"
#include "TemperatureController.h"
#include "pwm.h"                // screw motor functions


/***************************** Initializations ********************************/

// enumeration of extrusion control sub-states
typedef enum
{
    TEMP_TO_STEADY_STATE=0,
    TEMP_CONTROL_ON,
    TEMP_CONTROL_OFF
} EXTRUSION_CONTROL_SUBSTATES;

// struct to hold extrusion control sub-states
// this is a Harmony convention
typedef struct
{
    EXTRUSION_CONTROL_SUBSTATES substate;
} CONTROL_SUBSTATE_DATA;
            

TemperatureController tempController(true, true, true);

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
float spooler_system_speed = -100;
float screw_speed = 0;
unsigned int screw_period = 7999;
unsigned int screw_duty_cycle = 800;
float current_diameter = 0; 
float zone1 = 0;
float zone2 = 0;
float zone3 = 0;



// variables for temp pulse ICAP1
uint16_t capturedValue[2];
uint16_t capture = 0;
uint16_t pulsePoint;
volatile uint8_t captureIndex = 0;

EXTRUSION_CONTROL_DATA extrusion_controlData;   // hold thread FSM state
CONTROL_SUBSTATE_DATA control_substateData;     // hold control sub-state

/******************************************************************************/

void EXTRUSION_CONTROL_Initialize( void )
{
    extrusion_controlData.state = EXTRUSION_CONTROL_STATE_INIT;
    control_substateData.substate = TEMP_TO_STEADY_STATE;
}

void EXTRUSION_CONTROL_Tasks( void )
{
    switch ( extrusion_controlData.state )
    {
        case EXTRUSION_CONTROL_STATE_INIT:
        {
            //timer and input capture for temp
           
            ICAP1_Enable();
            TMR3_Start();
            //HEATER_CONTROL_1_OutputEnable();
            
            BUTTON_1_InputEnable();
            BUTTON_1_Clear();
            BUTTON_2_InputEnable();
            BUTTON_2_Clear();
            BUTTON_3_InputEnable();
            BUTTON_3_Clear();
            BUTTON_4_InputEnable();
            BUTTON_4_Clear();
            
            tempController.temp_control_turn_on(false, true, false);
            // temp_control_turn_on(TURN_ON_ZONE_1, TURN_ON_ZONE_2, TURN_ON_ZONE_3);
            
            // TO DO: new PWM interface that can be initialized from a frequency
            // in Hz instead of all these numbers
            
            // initialize PWM, start sending 25% duty cycle, ~ 940Hz
           
            // see pwm.h for TMR2_PRESCALE constants
            
            pwm_init(screw_period, screw_duty_cycle, TMR2_PRESCALE_64);
            screw_speed = ((float)screw_duty_cycle / (float)(screw_period + 1)) * 100;
            dataManager.set_numeric_param( SCREW_SPEED_INDEX, screw_speed );
            
            
            I2C_1_init();       // initialize I2C1 module for motor controllers
            CORETIMER_DelayMs( 100 );
            
            
            // all motors off
            rollers.stop( M1 );         // roller 1
            rollers.stop( M2 );         // roller 2
            spooler.stop( M1 );         // spooler
            CORETIMER_DelayMs( 5000 );

            // set initial roller and spoolers to 50% duty cycle
            rollers.set_speed( M1, spooler_system_speed );
            rollers.set_speed( M2, spooler_system_speed );
            spooler.set_speed( M1, spooler_system_speed );
            
            // pass initial roller and spooler speeds to dataManager
            dataManager.set_numeric_param( ROLLER_SPEED_INDEX, spooler_system_speed );
            dataManager.set_numeric_param( SPOOLER_SPEED_INDEX, spooler_system_speed );
            
            bool appInitialized = true;

            if ( appInitialized )
                extrusion_controlData.state = EXTRUSION_CONTROL_STATE_SERVICE_TASKS;

            break;
        }

        case EXTRUSION_CONTROL_STATE_SERVICE_TASKS:
        {
            
            current_delay_zone_2 = tempController.temp_control_compare_actual_to_target(2, current_delay_zone_2, actual_temp_2, target_temp_zone_2);
            first_delay = current_delay_zone_2;
            dataManager.set_numeric_param(HEATER_2_INDEX, (float)second_delay);
            
            
            dataManager.set_numeric_param( SCREW_SPEED_INDEX, screw_speed );
            
            if (BUTTON_1_PRESSED == true)
            {
                spooler_system_speed += (10 * spooler.get_motor_direction(M1));
                if (spooler_system_speed > 100 || spooler_system_speed < -100)
                    spooler_system_speed = 10 * spooler.get_motor_direction(M1);
                
                BUTTON_1_PRESSED = false;
            }
            
            if (BUTTON_2_PRESSED == true)
            {
                screw_duty_cycle += 800;
                if (screw_duty_cycle > (screw_period + 1))
                    screw_duty_cycle = 800;
                BUTTON_2_PRESSED = false;
            }
            
            if (BUTTON_3_PRESSED == true)
            {
                float temp_2_current = dataManager.get_numeric_param( ZONE_2_TEMP_INDEX );
                temp_2_current += 10;
                if (temp_2_current > 300)
                    temp_2_current = 300;
                dataManager.set_numeric_param( ZONE_2_TEMP_INDEX, temp_2_current);
                actual_temp_2 = temp_2_current;
                BUTTON_3_PRESSED = false;
            }
            
            if (BUTTON_4_PRESSED == true)
            {
                float temp_2_current = dataManager.get_numeric_param( ZONE_2_TEMP_INDEX );
                temp_2_current -= 10;
                if (temp_2_current < 0)
                    temp_2_current = 0;
                dataManager.set_numeric_param( ZONE_2_TEMP_INDEX, temp_2_current);
                BUTTON_4_PRESSED = false;
            }
            
            // adjust spooler system
            rollers.set_speed( M2, spooler_system_speed );
            spooler.set_speed( M1, spooler_system_speed );
            dataManager.set_numeric_param( ROLLER_SPEED_INDEX, spooler_system_speed );
            dataManager.set_numeric_param( SPOOLER_SPEED_INDEX, spooler_system_speed );
            // adjust screw
            pwm_set_duty_cycle(screw_duty_cycle);
            screw_speed = ((float)screw_duty_cycle / (float)(screw_period + 1)) * 100;
            dataManager.set_numeric_param( SCREW_SPEED_INDEX, screw_speed );
            
            
            
            /* TO DO: implement sub-state FSM
             * 
             * state machine will hold temperature control logic
             */
            /*
            switch( control_substateData.substate )
            {
                case TEMP_TO_STEADY_STATE:
                {
                    while( dataManager.get_numeric_param( ZONE_1_TEMP_INDEX < 320 ) )
                    {
                        
                    }
                    
                    break;
                }
                case TEMP_CONTROL_ON:
                {
                    break;
                }
                case TEMP_CONTROL_OFF:
                {
                    break;
                }
                default:
                {
                    break;
                }
            }
            */
            /*
            // test receiving temperatures from dataManager
            zone1 = dataManager.get_numeric_param( ZONE_1_TEMP_INDEX );
            zone2 = dataManager.get_numeric_param( ZONE_2_TEMP_INDEX );
            zone3 = dataManager.get_numeric_param( ZONE_3_TEMP_INDEX );
            */

            /********************** test screw motor PWM **********************/
            
            /*
            pwm_set_duty_cycle(1000); // 12.5% duty
            CORETIMER_DelayMs(1000);

            pwm_set_duty_cycle(6000); // 75% duty
            CORETIMER_DelayMs(1000);

            pwm_set_duty_cycle(2000); // 25% duty
            CORETIMER_DelayMs(1000);

            pwm_set_duty_cycle(4000); // 50% duty
            CORETIMER_DelayMs(1000);
            */
            
            /********************** test dynamic tensioning *******************/
            
//            if ( dataManager.get_spooler_tension() > tension_upper_limit )
//            {
//                // spooler_speed = spooler.nudge_down( M1, 1 );
//                SP_TENSION_LED_Set();
//                // dataManager.set_numeric_param( SPOOLER_SPEED_INDEX, spooler_speed );
//            }
//            else if ( dataManager.get_spooler_tension() < tension_lower_limit )
//            {
//                // spooler_speed = spooler.nudge_up( M1, 1 );
//                SP_TENSION_LED_Set();
//                // dataManager.set_numeric_param( SPOOLER_SPEED_INDEX, spooler_speed );
//            }
//            else
//            {
//                SP_TENSION_LED_Clear();
//            }
            
            /******************************************************************/
            
            CORETIMER_DelayMs( 100 );
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
