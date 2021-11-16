/*********************************************************************************
 * Extruder Controller
 * I2C.h
 * 
 * 
 ********************************************************************************/

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif
    
void I2C_1_init(void);
void I2C_1_wait_for_idle(void);
void I2C_1_Start(void);
void I2C_1_Stop(void);
bool I2C_1_send_byte(char data);
char I2C_1_get_byte(void);

void I2C_2_init(void);
void I2C_2_wait_for_idle(void);
void I2C_2_Start(void);
void I2C_2_Stop(void);
bool I2C_2_send_byte(char data);
char I2C_2_get_byte(void);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

