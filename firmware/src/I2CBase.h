/* 
 * File:   I2CBase.h
 * Author: wilson
 *
 * Created on November 9, 2021, 12:55 PM
 */

#ifndef I2CBASE_H
#define	I2CBASE_H

class I2CBase
{
    public:
        I2CBase();
        I2CBase(const I2CBase& orig);
        virtual ~I2CBase();
        void I2Cinit(void);
        void I2C_wait_for_idle(void);
        void I2CStart(void);
        void I2CStop(void);
        bool I2Csendbyte(char data);
        char I2Cgetbyte(void);
    private:

};

#endif	/* I2CBASE_H */

