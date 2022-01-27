
/*******************************************************************************
 * Extruder_Controller 
 * I2C.c
 * Wilson Woods
 * 11.18.2021
 ******************************************************************************/

#include "stdint.h"

#include "config/default/peripheral/i2c/master/plib_i2c1_master.h"
#include "config/default/peripheral/i2c/master/plib_i2c2_master.h"
#include "config/default/peripheral/tmr/plib_tmr2.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "I2C.h"

void I2C_1_init( void )
{
    IEC1CLR = _IEC1_I2C1MIE_MASK;   // disable I2C master interrupt
    IEC1CLR = _IEC1_I2C1BIE_MASK;   // disable I2C collision interrupt
    I2C1CONbits.DISSLW = 1;         // disable slew rate for 100kHz
    I2C1BRG = 235;                  // 1kHz = 235 | 4kHz = 55
    while ( I2C1STATbits.P );
    I2C1CONbits.A10M = 0;           // 7-bit address mode
    I2C1CONbits.I2CEN = 1;          // enable module
}

void I2C_2_init( void )
{
    IEC1CLR = _IEC1_I2C2MIE_MASK;   // disable I2C master interrupt
    IEC1CLR = _IEC1_I2C2BIE_MASK;   // disable I2C collision interrupt
    I2C2CONbits.DISSLW = 1;         // disable slew rate for 100kHz
    I2C2BRG = 235;                  // 1kHz = 235 | 4kHz = 55
    while ( I2C2STATbits.P );
    I2C2CONbits.A10M = 0;           // 7-bit address mode
    I2C2CONbits.I2CEN = 1;          // enable module
}

void I2C_1_wait_for_idle( void )
{
    while ( I2C1CON & 0x1F );
    while ( I2C1STATbits.TRSTAT );
}

void I2C_2_wait_for_idle( void )
{
    while ( I2C2CON & 0x1F );
    while ( I2C2STATbits.TRSTAT );
}

void I2C_1_start( void )
{
    I2C_1_wait_for_idle();
    I2C1CONbits.SEN = 1;            // initiate start condition
    while ( I2C1CONbits.SEN );      // wait for start condition
}

void I2C_2_start( void )
{
    I2C_2_wait_for_idle();
    I2C2CONbits.SEN = 1;            // initiate start condition
    while ( I2C2CONbits.SEN );      // wait for start condition
}

void I2C_1_stop( void )
{
    I2C_1_wait_for_idle();
    I2C1CONbits.PEN = 1;
    while ( I2C1CONbits.PEN );
    CORETIMER_DelayUs( 5 );
}

void I2C_2_stop( void )
{
    I2C_2_wait_for_idle();
    I2C2CONbits.PEN = 1;
    while ( I2C2CONbits.PEN );
    CORETIMER_DelayUs( 5 );
}


bool I2C_1_send_byte( char data )
{
    while ( I2C1STATbits.TBF );     // wait if buffer is full
    I2C_1_wait_for_idle();
    I2C1TRN = data;                 // pass data to transmit register 
    CORETIMER_DelayUs( 5 );
    return ( I2C1STATbits.ACKSTAT == 0 );
}

bool I2C_2_send_byte( char data )
{
    while ( I2C2STATbits.TBF );     // wait if buffer is full
    I2C_2_wait_for_idle();
    I2C2TRN = data;                 // pass data to transmit register 
    CORETIMER_DelayUs( 5 );
    return ( I2C2STATbits.ACKSTAT == 0 );
}

char I2C_1_get_byte( void )
{
    I2C1CONbits.RCEN = 1;           // set RCEN, enable I2C receive mode
    while ( !I2C1STATbits.RBF );    // wait for byte to shift into register
    I2C1CONbits.ACKEN = 1;          // master send acknowledge
    CORETIMER_DelayUs( 5 );
    return ( I2C1RCV );
}

char I2C_2_get_byte( void )
{
    I2C2CONbits.RCEN = 1;           // set RCEN, enable I2C receive mode
    while ( !I2C2STATbits.RBF );    // wait for byte to shift into register
    I2C2CONbits.ACKEN = 1;          // master send acknowledge
    CORETIMER_DelayUs( 5 );
    return ( I2C2RCV );
}
