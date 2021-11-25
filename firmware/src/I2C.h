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
 * I2C_1_Init()
 * Initialize the PIC32MX470 I2C1 module 
 */
void I2C_1_Init(void);

/**
 * I2C_2_Init()
 * Initialize the PIC32MX470 I2C2 module 
 */
void I2C_2_Init(void);

/**
 * I2C_1_Wait_For_Idle()
 * Wait for I2C1 module to idle before start/stop/send
 */
void I2C_1_Wait_For_Idle(void);

/**
 * I2C_2_Wait_For_Idle()
 * Wait for I2C2 module to idle before start/stop/send
 */
void I2C_2_Wait_For_Idle(void);

/**
 * I2C_1_Start()
 * Begin I2C1 module transaction
 */
void I2C_1_Start(void);

/**
 * I2C_2_Start()
 * Begin I2C2 module transaction
 */
void I2C_2_Start(void);


/**
 * I2C_1_Stop()
 * End I2C1 module transaction
 */
void I2C_1_Stop(void);

/**
 * I2C_2_Stop()
 * End I2C2 module transaction
 */
void I2C_2_Stop(void);

/**
 * I2C_1_Send_Byte()
 * Send byte data via I2C1 bus
 */
bool I2C_1_Send_Byte(char data);

/**
 * I2C_2_Send_Byte()
 * Send byte data via I2C2 bus
 */
bool I2C_2_Send_Byte(char data);

/**
 * I2C_1_Get_Byte()
 * Get byte from I2C1 bus
 */
char I2C_1_Get_Byte(void);

/**
 * I2C_2_Get_Byte()
 * Get byte from I2C2 bus
 */
char I2C_2_Get_Byte(void);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

