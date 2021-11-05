/*********************************************************************************
 * Extruder Controller
 * I2C.c
 * 
 * 
 ********************************************************************************/

#include "stdint.h"
#include "config/default/peripheral/i2c/master/plib_i2c1_master.h"
#include "config/default/peripheral/tmr/plib_tmr2.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"
#include "I2C.h"

void I2Cinit()
{
    IEC1CLR = _IEC1_I2C1MIE_MASK;   // disable the I2C master interrupt
    IEC1CLR = _IEC1_I2C1BIE_MASK;   // disable the I2C bus collision interrupt
    I2C1CONbits.DISSLW = 1;         // Disable slew rate for 100kHz
    I2C1BRG = 235;                  // 1kHz = 235 | 4kHz = 55
    while ( I2C1STATbits.P );
    I2C1CONbits.A10M = 0;           // 7-bit address mode
    I2C1CONbits.I2CEN = 1;          // enable module
}

void I2C_wait_for_idle(void)
{
    while(I2C1CON & 0x1F);
    while(I2C1STATbits.TRSTAT);
}

void I2CStart( void )
{
    I2C_wait_for_idle();
    I2C1CONbits.SEN = 1;            // initiate start condition
    while ( I2C1CONbits.SEN );      // wait for start condition
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
    I2C1CONbits.ACKEN = 1;          // master send acknowledge
    //us_delay( 10 );
    CORETIMER_DelayUs(5);
    return ( I2C1RCV );
}

/********************************************************************************/
