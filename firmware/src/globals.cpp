
/*********************************************************************************
 * Extruder_Controller Refactor
 * globals.cpp
 * 
 * 
 ********************************************************************************/

#include "globals.h"
#include "DataManager.h"

DataManager dataManager;

bool HEATER_RISING_EDGE_FOUND = false;

// this is the value that controls the power output to the heater
// initialized to 50% power (from above)
uint16_t heater_power_control_delay = HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_HALF_DUTY_CYCLE;

float heater_duty_cycle = 0;

float temp_1 = 0;
float temp_2 = 0;
float temp_3 = 0;


bool I2C_1_IS_BUSY = false;
bool I2C_2_IS_BUSY = false;

bool SPI_IS_BUSY = false;
uint16_t global_spooler_tension = 0;



// base delay 450 is point of zero crossing (half way between rising and falling edges)
const uint16_t HEATER_CONTROL_BASE_DELAY = 450;// half duty is 4155: 50% power (half way between zero crossings)
//
const uint16_t HEATER_CONTROL_FULL_DUTY_CYCLE = 0;
const uint16_t HEATER_CONTROL_HALF_DUTY_CYCLE = 4155;
const uint16_t HEATER_CONTROL_ONE_QUARTER_DUTY_CYCLE = 6233;
const uint16_t HEATER_CONTROL_THREE_QUARTER_DUTY_CYCLE = 2078;// this is the value that controls the power output to the heater
// initialized to 50% power (from above)

uint16_t heater_power_control_delay_Z1 = HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_FULL_DUTY_CYCLE;
uint16_t heater_power_control_delay_Z2 = HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_FULL_DUTY_CYCLE;
uint16_t heater_power_control_delay_Z3 = HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_FULL_DUTY_CYCLE;

//uint16_t heater_power_control_delay_Z1 = HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_FULL_DUTY_CYCLE;
//uint16_t heater_power_control_delay_Z2 = HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_HALF_DUTY_CYCLE;
//uint16_t heater_power_control_delay_Z3 = HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_ONE_QUARTER_DUTY_CYCLE;


/*************************** Numeric Parameter IDs ****************************/

const uint8_t DESIRED_YIELD_INDEX       = 0;
const uint8_t DESIRED_YIELD_ID          = 0x01;

const uint8_t REQUIRED_INPUT_INDEX      = 1;
const uint8_t REQUIRED_INPUT_ID         = 0x02;

const uint8_t GROUND_WEIGHT_INDEX       = 2;
const uint8_t GROUND_WEIGHT_ID          = 0x03;

const uint8_t ZONE_1_TEMP_INDEX         = 3;
const uint8_t ZONE_1_TEMP_ID            = 0x04;

const uint8_t ZONE_2_TEMP_INDEX         = 4;
const uint8_t ZONE_2_TEMP_ID            = 0x05;

const uint8_t ZONE_3_TEMP_INDEX         = 5;
const uint8_t ZONE_3_TEMP_ID            = 0x06;

const uint8_t SCREW_SPEED_INDEX         = 6;
const uint8_t SCREW_SPEED_ID            = 0x07;

const uint8_t ROLLER_SPEED_INDEX        = 7;
const uint8_t ROLLER_SPEED_ID           = 0x08;

const uint8_t SPOOLER_SPEED_INDEX       = 8;
const uint8_t SPOOLER_SPEED_ID          = 0x09;

const uint8_t DIAMETER_INDEX            = 9;
const uint8_t DIAMETER_ID               = 0x0A;

const uint8_t EXTRUDED_LENGTH_INDEX     = 10;
const uint8_t EXTRUDED_LENGTH_ID        = 0x0B;

const uint8_t PROJECTED_YIELD_INDEX     = 11;
const uint8_t PROJECTED_YIELD_ID        = 0x0C;

/**************************** Status Parameter IDs ****************************/
const uint8_t HOPPER_LID_STATUS_INDEX   = 0;
const uint8_t HOPPER_LID_STATUS_ID      = 0x10;

const uint8_t GRINDER_ON_OFF_INDEX      = 1;
const uint8_t GRINDER_ON_OFF_ID         = 0x20;

const uint8_t PREPARATION_STATUS_INDEX  = 2;
const uint8_t PREPARATION_STATUS_ID     = 0x30;

const uint8_t EXTRUSION_STATUS_INDEX    = 3;
const uint8_t EXTRUSION_STATUS_ID       = 0x40;

/******************************************************************************/
