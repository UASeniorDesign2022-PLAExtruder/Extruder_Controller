/*******************************************************************************
 * 
 * 
 * 
 * 
 * 
 * 
 ******************************************************************************/

#include "preparation.h"

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
    This structure should be initialized by the PREPARATION_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

PREPARATION_DATA preparationData;

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
    void PREPARATION_Initialize ( void )

  Remarks:
    See prototype in preparation.h.
 */

void PREPARATION_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    preparationData.state = PREPARATION_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void PREPARATION_Tasks ( void )

  Remarks:
    See prototype in preparation.h.
 */

void PREPARATION_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( preparationData.state )
    {
        /* Application's initial state. */
        case PREPARATION_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                preparationData.state = PREPARATION_STATE_SERVICE_TASKS;
            }
            break;
        }

        case PREPARATION_STATE_SERVICE_TASKS:
        {

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
