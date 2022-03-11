/*******************************************************************************
 * Extruder_Controller
 * SPI.c
 * Wilson Woods
 * 11.18.2021
 ******************************************************************************/

#include "config/default/peripheral/spi/spi_master/plib_spi1_master.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "SPI.h"

void SPI_init( void )
{
    uint32_t rdata = 0U;
    IEC1CLR = 0x8;
    IEC1CLR = 0x10;
    IEC1CLR = 0x20;
    SPI1CON = 0;
    rdata = SPI1BUF;
    rdata = rdata;
    IFS1CLR = 0x8;
    IFS1CLR = 0x10;
    IFS1CLR = 0x20;
    SPI1BRG = 23;                           // 1MHz clock
    SPI1STATCLR = _SPI1STAT_SPIROV_MASK;
    SPI1CON = 0x8560;                       // PIC32 master, 16-bit mode
}

void SPI_transfer( uint8_t data )
{
    SPI1BUF = ( 0x00FF & data );            // pass data to buffer
    while( !SPI1BUF );                      // wait for data to be sent out
}

int SPI_read_temp_1( void )
{
    SPI1CONbits.DISSDO = 1;
    SS_TEMP_1_Clear();                      // slave select low
    SPI_transfer( 0x00 );                   // send dummy byte
    CORETIMER_DelayUs( 16 );                // wait 16 cycles for 2 bytes
    SPI1CONbits.DISSDO = 0;
    SS_TEMP_1_Set();                        // slave select high
    return ( SPI1BUF );                     // read incoming data from buffer
}

int SPI_read_temp_2( void )
{
    SPI1CONbits.DISSDO = 1;
    SS_TEMP_2_Clear();                      // slave select low
    SPI_transfer( 0x00 );                   // send dummy byte
    CORETIMER_DelayUs( 16 );                // wait 16 cycles for 2 bytes
    SPI1CONbits.DISSDO = 0;
    SS_TEMP_2_Set();                        // slave select high
    return ( SPI1BUF );                     // read incoming data from buffer
}

int SPI_read_temp_3( void )
{
    SPI1CONbits.DISSDO = 1;
    SS_TEMP_3_Clear();                      // slave select low
    SPI_transfer( 0x00 );                   // send dummy byte
    CORETIMER_DelayUs( 16 );                // wait 16 cycles for 2 bytes
    SPI1CONbits.DISSDO = 0;
    SS_TEMP_3_Set();                        // slave select high
    return ( SPI1BUF );                     // read incoming data from buffer
}
