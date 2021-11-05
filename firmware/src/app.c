/*******************************************************************************
 * 
 * 
 * 
 * 
 * 
 * 
 *******************************************************************************/


#include "app.h"
#include "config/default/peripheral/i2c/master/plib_i2c1_master.h"
#include "config/default/peripheral/tmr/plib_tmr2.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#define APP_AT24MAC_DEVICE_ADDR             0x14
#define APP_TRANSMIT_DATA_LENGTH            2
#define APP_ACK_DATA_LENGTH                 1
#define APP_RECEIVE_DUMMY_WRITE_LENGTH      2
#define APP_RECEIVE_DATA_LENGTH             4
#define APP_TRANSMIT_DATA_ID                3
#define APP_TRANSMIT_DATA_CONTENT           5

uint8_t MOTOR_1                 = 1;
uint8_t MOTOR_2                 = 2;
uint8_t DIRECTION_SET_CODE      = 0xaa;
uint8_t MOTOR_DIR_BOTH_CW       = 0x0a;
uint8_t MOTOR_DIR_BOTH_CCW      = 0x05;
uint8_t MOTOR_DIR_M1CW_M2CCW    = 0x06;
uint8_t MOTOR_DIR_M1CCW_M2CW    = 0x09;
uint8_t MotorSpeedSet           = 0x82;
uint8_t PWMFrequencySet         = 0x84;
uint8_t MotorSetA               = 0xa1;
uint8_t MotorSetB               = 0xa5;
uint8_t NOTHING                 = 0x01;
uint8_t F_31372Hz               = 0x01;
uint8_t F_3921Hz                = 0x02;
uint8_t F_490Hz                 = 0x03;
uint8_t F_122Hz                 = 0x04;
uint8_t F_30Hz                  = 0x05;

// speed 0 to 255
unsigned char SPEED_MOTOR_1 = 0;
unsigned char SPEED_MOTOR_2 = 0;

// clockwise = 1
// counterclockwise = -1
int DIRECTION_MOTOR_1 = 1;
int DIRECTION_MOTOR_2 = 1;

// controller 1: motors 1 & 2
// controller 2: motor 3
const uint16_t CONTROLLER_1_I2C_ADDRESS = 0x0C;
const uint16_t DISPLAY_I2C_ADDRESS = 0x14;

/******************************I2C FUNCTIONS***********************************/

void I2Cinit()
{
    /* Disable the I2C Master interrupt */
    IEC1CLR = _IEC1_I2C1MIE_MASK;
    /* Disable the I2C Bus collision interrupt */
    IEC1CLR = _IEC1_I2C1BIE_MASK;
    I2C1CONbits.DISSLW = 1; // Disable slew rate for 100kHz
    // 4kHz = 55
    // 1kHz = 235
    I2C1BRG = 235;
    while ( I2C1STATbits.P );
    I2C1CONbits.A10M = 0;       // 7-bit address mode
    I2C1CONbits.I2CEN = 1;      // enable module
}

void I2C_wait_for_idle(void)
{
    while(I2C1CON & 0x1F);
    while(I2C1STATbits.TRSTAT);
}

void I2CStart( void )
{
    I2C_wait_for_idle();
    I2C1CONbits.SEN = 1;        // initiate start condition
    while ( I2C1CONbits.SEN );  // wait for start condition
}

void I2CStop( void )
{
    // us_delay( 10 );
    I2C_wait_for_idle();
    I2C1CONbits.PEN = 1;
    while ( I2C1CONbits.PEN );
    //us_delay( 10 );
    CORETIMER_DelayUs(5);
}

bool I2Csendbyte( char data )
{
    while ( I2C1STATbits.TBF );     // wait if buffer is full
    I2C_wait_for_idle();
    I2C1TRN = data;                 // pass data to transmit register 
    //us_delay( 10 );
    CORETIMER_DelayUs(5);
    return (I2C1STATbits.ACKSTAT == 0);
}

char I2Cgetbyte( void )
{
    I2C1CONbits.RCEN = 1;           // set RCEN, enables I2C receive mode
    while ( !I2C1STATbits.RBF );    // wait for byte to shift into register
    I2C1CONbits.ACKEN = 1;          // master sends acknowledge
    //us_delay( 10 );
    CORETIMER_DelayUs(5);
    return ( I2C1RCV );
}

/*****************************MOTOR FUNCTIONS**********************************/
int frequency(unsigned char frequency_setting, uint8_t i2c_address)
{
    if (frequency_setting < F_31372Hz || frequency_setting > F_30Hz)
        return -1;
    I2CStart();
    CORETIMER_DelayUs(5);
    I2Csendbyte((i2c_address << 1));
    CORETIMER_DelayUs(10);
    I2Csendbyte( PWMFrequencySet );
    CORETIMER_DelayUs(10);
    I2Csendbyte( frequency_setting );
    CORETIMER_DelayUs(10);
    I2Csendbyte( NOTHING );
    I2CStop();
    CORETIMER_DelayMs(4);
    return 0;
}

int begin(unsigned char i2c_address)
{
    if (i2c_address > 0x0f)
        return -1;
    CORETIMER_DelayMs(1000);
    // Set default frequence to F_3921Hz
    frequency(F_3921Hz, i2c_address);
    return 0;
}

void direction(uint8_t motor_directions, uint8_t i2c_address)
{
    I2CStart();
    CORETIMER_DelayUs(5);
    I2Csendbyte((i2c_address << 1));
    CORETIMER_DelayUs(10);
    I2Csendbyte( DIRECTION_SET_CODE );
    CORETIMER_DelayUs(10);
    I2Csendbyte( motor_directions );
    CORETIMER_DelayUs(10);
    I2Csendbyte( NOTHING );
    I2CStop();
    CORETIMER_DelayMs(4);
}


void speed(unsigned char motor_id, int new_speed, uint8_t i2c_address)
{
    if (motor_id < MOTOR_1 || motor_id > MOTOR_2)
        return;

    if (motor_id == MOTOR_1)
    {
        if (new_speed >= 0)
        {
            DIRECTION_MOTOR_1 = 1;
            if (new_speed > 255)
                SPEED_MOTOR_1 = 255;
            else
                SPEED_MOTOR_1 = new_speed;
        }
        else if (new_speed < 0)
        {
            DIRECTION_MOTOR_1 = -1;
            if (new_speed < -255)
                SPEED_MOTOR_1 = 255;
            else
                SPEED_MOTOR_1 = -(new_speed);
        }
    }
    else if (motor_id == MOTOR_2)
    {
        if (new_speed >= 0)
        {
            DIRECTION_MOTOR_2 = 1;
            if (new_speed > 255)
                SPEED_MOTOR_2 = 255;
            else
                SPEED_MOTOR_2 = new_speed;
        }
        else if (new_speed < 0)
        {
            DIRECTION_MOTOR_2 = -1;
            if (new_speed < -255)
                SPEED_MOTOR_2 = 255;
            else
                SPEED_MOTOR_2 = -(new_speed);
        }
    }
    // Set the direction
    if (DIRECTION_MOTOR_1 == 1 && DIRECTION_MOTOR_2 == 1)
    {
        direction(MOTOR_DIR_BOTH_CW, i2c_address);
    }
    if (DIRECTION_MOTOR_1 == 1 && DIRECTION_MOTOR_2 == -1)
    {
        direction(MOTOR_DIR_M1CW_M2CCW, i2c_address);
    }
    if (DIRECTION_MOTOR_1 == -1 && DIRECTION_MOTOR_2 == 1)
    {
        direction(MOTOR_DIR_M1CCW_M2CW, i2c_address);
    }
    if (DIRECTION_MOTOR_1 == -1 && DIRECTION_MOTOR_2 == -1)
    {
        direction(MOTOR_DIR_BOTH_CCW, i2c_address);
    }
    // send command
    I2CStart();
    CORETIMER_DelayUs(5);
    I2Csendbyte((i2c_address << 1));
    CORETIMER_DelayUs(10);
    I2Csendbyte( MotorSpeedSet );
    CORETIMER_DelayUs(10);
    I2Csendbyte( SPEED_MOTOR_1 );
    CORETIMER_DelayUs(10);
    I2Csendbyte( SPEED_MOTOR_2 );
    I2CStop();
    CORETIMER_DelayMs(4);
}

    // Stop one motor
    // motor_id: MOTOR1, MOTOR2
int stop(unsigned char motor_id, uint8_t i2c_address)
{
    if (motor_id < MOTOR_1 || motor_id > MOTOR_2)
        return -1;
    speed(motor_id, 0, i2c_address);
    return 0;
}   
/******************************************************************************/

APP_DATA appData;

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        case APP_STATE_INIT:
        {
            I2Cinit();
            frequency(F_3921Hz, CONTROLLER_1_I2C_ADDRESS);
            CORETIMER_DelayMs(500);

            bool appInitialized = true;
            if (appInitialized)
                appData.state = APP_STATE_SERVICE_TASKS;
            break;
        }
        case APP_STATE_SERVICE_TASKS:
        {
                /* display test */
                /*
                while(1)
                {
                    I2CStart();
                    CORETIMER_DelayUs(5);
                    I2Csendbyte(DISPLAY_I2C_ADDRESS << 1);
                    CORETIMER_DelayUs(10);
                    I2Csendbyte( 0x03 );
                    CORETIMER_DelayUs(10);
                    I2Csendbyte( 0x05 );
                    CORETIMER_DelayUs(10);
                    I2CStop();
                    CORETIMER_DelayUs(200);
                }
                */
            
                /* motor control test */

                while(1)
                {
                    CORETIMER_DelayMs(2000);
                    speed(MOTOR_1, 255, CONTROLLER_1_I2C_ADDRESS);
                    speed(MOTOR_2, 255, CONTROLLER_1_I2C_ADDRESS);
                    CORETIMER_DelayMs(2000);
                    stop(MOTOR_1, CONTROLLER_1_I2C_ADDRESS);
                    stop(MOTOR_2, CONTROLLER_1_I2C_ADDRESS);
                    CORETIMER_DelayMs(2000);
                    speed(MOTOR_1, -255, CONTROLLER_1_I2C_ADDRESS);
                    speed(MOTOR_2, -255, CONTROLLER_1_I2C_ADDRESS);
                    CORETIMER_DelayMs(2000);
                    stop(MOTOR_1, CONTROLLER_1_I2C_ADDRESS);
                    stop(MOTOR_2, CONTROLLER_1_I2C_ADDRESS);
                }

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
