/*******************************************************************************
 * Extruder_Controller Refactor
 * globals.h
 * 
 * 
 ******************************************************************************/

#ifndef GLOBALS_H
#define	GLOBALS_H

#include "DataManager.h"
#include "stdbool.h"


#ifdef	__cplusplus
extern "C" {
#endif

extern DataManager dataManager;

extern bool BUTTON_1_PRESSED;
extern bool BUTTON_2_PRESSED;
extern bool BUTTON_3_PRESSED;
extern bool BUTTON_4_PRESSED;

extern bool I2C_1_IS_BUSY;
extern bool I2C_2_IS_BUSY;

extern bool SPI_IS_BUSY;

extern uint16_t global_spooler_tension;

/******************************************************************************/
/***************************** TEMPERATURE CONTROL ****************************/
/******************************************************************************/

extern bool HEATER_CONTROL_ON;

extern bool ZONE_1_ACTIVE;
extern bool ZONE_2_ACTIVE;
extern bool ZONE_3_ACTIVE;

extern const bool TURN_ON_ZONE_1;
extern const bool TURN_ON_ZONE_2;
extern const bool TURN_ON_ZONE_3;

extern const bool TURN_OFF_ZONE_1;
extern const bool TURN_OFF_ZONE_2;
extern const bool TURN_OFF_ZONE_3;

extern float actual_temp_1;
extern float actual_temp_2;
extern float actual_temp_3;

extern const uint16_t target_temp_zone_1;
extern const uint16_t target_temp_zone_2;
extern const uint16_t target_temp_zone_3;

extern const uint16_t pulse_delay;
extern uint16_t variable_delay_zone_1;
extern uint16_t variable_delay_zone_2;
extern uint16_t variable_delay_zone_3;
extern uint16_t current_delay_zone_1;
extern uint16_t current_delay_zone_2;
extern uint16_t current_delay_zone_3;

extern uint16_t first_delay;
extern uint16_t second_delay;
extern uint16_t third_delay;

extern const uint16_t HEATER_CONTROL_BASE_DELAY;
extern const uint16_t HEATER_CONTROL_FULL_DUTY_CYCLE;
extern const uint16_t HEATER_CONTROL_HALF_DUTY_CYCLE;
extern const uint16_t HEATER_CONTROL_FIVE_PERCENT;
extern const uint16_t HEATER_CONTROL_ONE_QUARTER_DUTY_CYCLE;
extern const uint16_t HEATER_CONTROL_THREE_QUARTER_DUTY_CYCLE;

extern uint16_t heater_power_control_delay;
extern uint16_t heater_power_control_delay_Z1;
extern uint16_t heater_power_control_delay_Z2;
extern uint16_t heater_power_control_delay_Z3;



/*************************** Numeric Parameter IDs ****************************/
extern const uint8_t DESIRED_YIELD_INDEX;
extern const uint8_t DESIRED_YIELD_ID;

extern const uint8_t REQUIRED_INPUT_INDEX;
extern const uint8_t REQUIRED_INPUT_ID;

extern const uint8_t GROUND_WEIGHT_INDEX;
extern const uint8_t GROUND_WEIGHT_ID;

extern const uint8_t ZONE_1_TEMP_INDEX;
extern const uint8_t ZONE_1_TEMP_ID;

extern const uint8_t ZONE_2_TEMP_INDEX;
extern const uint8_t ZONE_2_TEMP_ID;

extern const uint8_t ZONE_3_TEMP_INDEX;
extern const uint8_t ZONE_3_TEMP_ID;

extern const uint8_t SCREW_SPEED_INDEX;
extern const uint8_t SCREW_SPEED_ID;

extern const uint8_t ROLLER_SPEED_INDEX;
extern const uint8_t ROLLER_SPEED_ID;

extern const uint8_t SPOOLER_SPEED_INDEX;
extern const uint8_t SPOOLER_SPEED_ID;

extern const uint8_t HEATER_1_INDEX;
extern const uint8_t HEATER_1_ID;

extern const uint8_t HEATER_2_INDEX;
extern const uint8_t HEATER_2_ID;

extern const uint8_t PROJECTED_YIELD_INDEX;
extern const uint8_t PROJECTED_YIELD_ID;

extern const uint8_t HEATER_3_INDEX;
extern const uint8_t HEATER_3_ID;


/**************************** Status Parameter IDs ****************************/
extern const uint8_t HOPPER_LID_STATUS_INDEX;
extern const uint8_t HOPPER_LID_STATUS_ID;

extern const uint8_t GRINDER_ON_OFF_INDEX;
extern const uint8_t GRINDER_ON_OFF_ID;

extern const uint8_t PREPARATION_STATUS_INDEX;
extern const uint8_t PREPARATION_STATUS_ID;

extern const uint8_t EXTRUSION_STATUS_INDEX;
extern const uint8_t EXTRUSION_STATUS_ID;

/******************************************************************************/

#ifdef	__cplusplus
}
#endif

#endif	/* GLOBALS_H */

