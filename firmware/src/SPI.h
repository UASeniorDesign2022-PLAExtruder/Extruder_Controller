/*******************************************************************************
 * Extruder_Controller
 * SPI.h
 * Wilson Woods
 * 11.18.2021
 ******************************************************************************/

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/**
 * SPI_init()
 * Initialize the PIC32MX470 SPI1 module 
 */
void SPI_init( void );

/**
 * SPI_transfer()
 * Send dummy byte to initiate SPI1 module master read
 */
void SPI_transfer( char data );

/**
 * SPI_read_temp_1()
 * Read 2 bytes from SPI slave device connected at SS 1
 */
int SPI_read_temp_1( void );

/**
 * SPI_read_temp_2()
 * Read 2 bytes from SPI slave device connected at SS 2
 */
int SPI_read_temp_2( void );

/**
 * SPI_read_temp_3()
 * Read 2 bytes from SPI slave device connected at SS 3
 */
int SPI_read_temp_3( void );

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

