/*******************************************************************************
 * Extruder_Controller
 * I2C.h
 * Wilson Woods
 * 11.18.2021
 ******************************************************************************/

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * I2C_1_init()
 * Initialize the PIC32MX470 I2C1 module 
 */
void I2C_1_init( void );

/**
 * I2C_2_init()
 * Initialize the PIC32MX470 I2C2 module 
 */
void I2C_2_init( void );

/**
 * I2C_1_wait_for_idle()
 * Wait for I2C1 module to idle before start/stop/send
 */
void I2C_1_wait_for_idle( void );

/**
 * I2C_2_wait_for_idle()
 * Wait for I2C2 module to idle before start/stop/send
 */
void I2C_2_wait_for_idle( void );

/**
 * I2C_1_start()
 * Begin I2C1 module transaction
 */
void I2C_1_start( void );

/**
 * I2C_2_start()
 * Begin I2C2 module transaction
 */
void I2C_2_start( void );


/**
 * I2C_1_stop()
 * End I2C1 module transaction
 */
void I2C_1_stop( void );

/**
 * I2C_2_stop()
 * End I2C2 module transaction
 */
void I2C_2_stop( void );

/**
 * I2C_1_send_byte()
 * Send byte data via I2C1 bus
 */
bool I2C_1_send_byte( char data );

/**
 * I2C_2_send_byte()
 * Send byte data via I2C2 bus
 */
bool I2C_2_send_byte( char data );

/**
 * I2C_1_get_byte()
 * Get byte from I2C1 bus
 */
char I2C_1_get_byte( void );

/**
 * I2C_2_get_byte()
 * Get byte from I2C2 bus
 */
char I2C_2_get_byte( void );

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

