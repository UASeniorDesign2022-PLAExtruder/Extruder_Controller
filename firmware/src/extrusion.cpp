/*******************************************************************************
 * Extruder_Controller
 * extrusion.cpp
 * Wilson Woods
 * 11.18.2021
 ******************************************************************************/

#include <cstdint>

#include "extrusion.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/spi/spi_master/plib_spi1_master.h"
#include "config/default/peripheral/i2c/master/plib_i2c1_master.h"
#include "config/default/peripheral/tmr/plib_tmr2.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "extrusion.h"
#include "globals.h"
#include "DataManager.h"
#include "SPI.h"
#include "I2C.h"
#include "I2CMotor.h"

const uint16_t CONTROLLER_1_I2C_ADDRESS = 0x0C;
const uint16_t CONTROLLER_2_I2C_ADDRESS = 0x04;

I2CMotor controller1(CONTROLLER_1_I2C_ADDRESS);
I2CMotor controller2(CONTROLLER_2_I2C_ADDRESS);

const uint8_t MOTOR_1_ID = 0;
const uint8_t MOTOR_2_ID = 1;
float current_motor_speed = 0;
uint16_t temp_1 = 0;
uint16_t temp_2 = 0;
uint16_t temp_3 = 0;
float temp_1_float = 0;
float temp_2_float = 0;
float temp_3_float = 0;

EXTRUSION_DATA extrusionData;

void EXTRUSION_Initialize ( void )
{
    extrusionData.state = EXTRUSION_STATE_INIT;

}

void EXTRUSION_Tasks ( void )
{
    switch ( extrusionData.state )
    {
        case EXTRUSION_STATE_INIT:
        {
            I2C_1_Init();
            SPI_Init();
            CORETIMER_DelayMs(500);
            
            TEMP_1_SS_OutputEnable();
            TEMP_1_SS_Set();
            SS_TEMP_2_OutputEnable();
            SS_TEMP_2_Set();
            SS_TEMP_3_OutputEnable();
            SS_TEMP_3_Set();
            
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
                extrusionData.state = EXTRUSION_STATE_SERVICE_TASKS;

            break;
        }

        case EXTRUSION_STATE_SERVICE_TASKS:
        {
            
            temp_1 = SPI_Read_Temp_1();
            temp_1 >>= 3;
            temp_1_float = (float)temp_1;
            temp_1_float = (temp_1_float * (9 / 5)) + 32;
            globalDataManager.setNumericParam(ZONE_1_TEMP_INDEX, temp_1_float);
            temp_1 = 0;
            temp_1_float = 0;
            CORETIMER_DelayUs(10);
            temp_2 = SPI_Read_Temp_2();
            temp_2 >>= 3;
            temp_2_float = (float)temp_2;
            temp_2_float = (temp_2_float * (9 / 5)) + 32;
            globalDataManager.setNumericParam(ZONE_2_TEMP_INDEX, temp_2_float);
            temp_2 = 0;
            temp_2_float = 0;
            CORETIMER_DelayUs(10);
            temp_3 = SPI_Read_Temp_3();
            temp_3 >>= 3;
            temp_3_float = (float)temp_3;
            temp_3_float = (temp_3_float * (9 / 5)) + 32;
            globalDataManager.setNumericParam(ZONE_3_TEMP_INDEX, temp_3_float);
            temp_3 = 0;
            temp_3_float = 0;
            CORETIMER_DelayMs(500);
            
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
