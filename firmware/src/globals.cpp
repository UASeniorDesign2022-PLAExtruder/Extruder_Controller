/*********************************************************************************
 * Extruder Controller
 * globals.cpp
 * 
 * 
 ********************************************************************************/

#include "globals.h"
#include "DataManager.h"

DataManager globalDataManager;

bool I2C_1_IS_BUSY = false;
bool I2C_2_IS_BUSY = false;


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
