/*******************************************************************************
 * Extruder_Controller Refactor
 * globals.h
 * 
 * 
 ******************************************************************************/

#ifndef GLOBALS_H
#define	GLOBALS_H

#include <cstdint>
#include "DataManager.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
extern DataManager globalDataManager;
extern bool I2C_1_IS_BUSY;
extern bool I2C_2_IS_BUSY;


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

extern const uint8_t DIAMETER_INDEX;
extern const uint8_t DIAMETER_ID;

extern const uint8_t EXTRUDED_LENGTH_INDEX;
extern const uint8_t EXTRUDED_LENGTH_ID;

extern const uint8_t PROJECTED_YIELD_INDEX;
extern const uint8_t PROJECTED_YIELD_ID;

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

