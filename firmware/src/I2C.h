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
    
void I2Cinit(void);
void I2C_wait_for_idle(void);
void I2CStart(void);
void I2CStop(void);
bool I2Csendbyte(char data);
char I2Cgetbyte(void);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

