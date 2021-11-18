/*******************************************************************************
 Extruder_Contoller Refactor
    master.cpp
 *******************************************************************************/

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

MASTER_DATA masterData;

void MASTER_Initialize ( void )
{
    masterData.state = MASTER_STATE_INIT;
}

void MASTER_Tasks ( void )
{
    switch ( masterData.state )
    {
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
            globalDataManager.pollNumericParams();
            globalDataManager.sendAllFreshNumericParams();
            CORETIMER_DelayUs(50); 

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
