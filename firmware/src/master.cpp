/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    master.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <cstdint>
#include <vector>
#include <tuple>
#include <utility>

#include "config/default/peripheral/i2c/master/plib_i2c1_master.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "master.h"
#include "globals.h"
#include "DataManager.h"
#include "I2C.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the MASTER_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

MASTER_DATA masterData;

typedef union
{
    uint8_t buffer[4];
    float numeric_param_input;
} FloatToBytes;

FloatToBytes converter;

const uint16_t DISPLAY_I2C_ADDRESS = 0x14;

void sendNumericParamI2C(uint8_t data_id, float value)
{
    converter.numeric_param_input = value;
    
    I2C_2_Start();
    CORETIMER_DelayUs(5);
    I2C_2_send_byte(DISPLAY_I2C_ADDRESS << 1);
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( data_id );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( converter.buffer[0] );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( converter.buffer[1] );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( converter.buffer[2] );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( converter.buffer[3] );
    CORETIMER_DelayUs(10);
    I2C_2_Stop();
    converter.numeric_param_input = 0;
    CORETIMER_DelayUs(500);
}

void sendStatusParamI2C(uint8_t data_id, uint8_t status)
{
    I2C_2_Start();
    CORETIMER_DelayUs(5);
    I2C_2_send_byte(DISPLAY_I2C_ADDRESS << 1);
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( data_id );
    CORETIMER_DelayUs(10);
    I2C_2_send_byte( status );
    CORETIMER_DelayUs(10);
    I2C_2_Stop();
    CORETIMER_DelayUs(500);
}

void sendAllFreshNumericParams(std::vector<uint8_t>& fresh_IDs, std::vector<float>& fresh_values)
{
    I2C_2_IS_BUSY = true;
    for (uint8_t index = 0; index < fresh_IDs.size(); index++)
    {
        sendNumericParamI2C(fresh_IDs[index], fresh_values[index]);
    }
    I2C_2_IS_BUSY = false;
}
void sendAllFreshStatusParams(std::vector<uint8_t>& fresh_IDs, std::vector<uint8_t>& fresh_values)
{
    I2C_2_IS_BUSY = true;
    for (uint8_t index = 0; index < fresh_IDs.size(); index++)
    {
        sendStatusParamI2C(fresh_IDs[index], fresh_values[index]);
    }
    I2C_2_IS_BUSY = false;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void MASTER_Initialize ( void )

  Remarks:
    See prototype in master.h.
 */

void MASTER_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    masterData.state = MASTER_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void MASTER_Tasks ( void )

  Remarks:
    See prototype in master.h.
 */

void MASTER_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( masterData.state )
    {
        /* Application's initial state. */
        case MASTER_STATE_INIT:
        {
            
            I2C_2_init();
            bool appInitialized = true;


            if (appInitialized)
            {

                masterData.state = MASTER_STATE_SERVICE_TASKS;
            }
            break;
        }

        case MASTER_STATE_SERVICE_TASKS:
        {
            // while(I2C_IS_BUSY);
            // sendNumericParamI2C(0x05, 29.69);
            
            globalDataManager.pollNumericParams();
            // CORETIMER_DelayUs(500);
            globalDataManager.sendAllFreshNumericParams();
            // std::vector<uint8_t> numeric_ids = globalDataManager.getFreshNumericIDs();
            // std::vector<float> numeric_values = globalDataManager.getFreshNumericValues();
            // while(I2C_2_IS_BUSY);
            // sendAllFreshNumericParams(numeric_ids, numeric_values);
            
            
            /******************************************************************/
            /*
            I2C_IS_BUSY = true;
            for (uint8_t index = 0; index < numeric_ids.size(); index++)
            {
                converter.numeric_param_input = numeric_values[index];
    
                I2C_2_Start();
                CORETIMER_DelayUs(5);
                I2C_2_send_byte(DISPLAY_I2C_ADDRESS << 1);
                CORETIMER_DelayUs(10);
                I2C_2_send_byte( numeric_ids[index] );
                CORETIMER_DelayUs(10);
                I2C_2_send_byte( converter.buffer[0] );
                CORETIMER_DelayUs(10);
                I2C_2_send_byte( converter.buffer[1] );
                CORETIMER_DelayUs(10);
                I2C_2_send_byte( converter.buffer[2] );
                CORETIMER_DelayUs(10);
                I2C_2_send_byte( converter.buffer[3] );
                CORETIMER_DelayUs(10);
                I2C_2_Stop();
                converter.numeric_param_input = 0;
                CORETIMER_DelayUs(500);
            }
            I2C_IS_BUSY = false;
            */        
            
            /******************************************************************/
            
            /*
            fresh_numeric_ data = globalDataManager.pollNumericParams();

            for (uint8_t index = 0; index < fresh_numeric_data.size(); ++index)
            {
                uint8_t data_id = fresh_numeric_data[index].first;
                float value = fresh_numeric_data[index].second;
                while(I2C_2_IS_BUSY);
                I2CSendNumericParam(data_id, value);
                CORETIMER_DelayMs(200);
            }
            fresh_numeric_data.clear();
            /*
            CORETIMER_DelayUs(100);

            // while(I2C_2_IS_BUSY);
            // sendStatusParamI2C(0x20, 0x01);
            /*
            globalDataManager.pollStatusParams();
            std::vector<uint8_t> status_ids = globalDataManager.getFreshStatusIDs();
            std::vector<uint8_t> status_values = globalDataManager.getFreshStatusValues();
            while(I2C_2_IS_BUSY);
            sendAllFreshStatusParams(status_ids, status_values);
            */
            /*
                globalDataManager.sendAllFreshNumericParams();
                globalDataManager.sendAllFreshStatusParams();
            fresh_status_data = globalDataManager.pollStatusParams();

            for (uint8_t index = 0; index < fresh_status_data.size(); ++index)
            {
                uint8_t data_id = fresh_status_data[index].first;
                uint8_t status = fresh_status_data[index].second;
                while(I2C_2_IS_BUSY);
                I2CSendStatusParam(data_id, status);
                CORETIMER_DelayMs(200);
            }
            fresh_status_data.clear();
            */
            CORETIMER_DelayUs(100); 

            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
