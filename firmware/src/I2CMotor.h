/* 
 * File:   I2CMotor.h
 * Author: wilson
 *
 * Created on November 5, 2021, 2:03 AM
 */

#ifndef I2CMOTOR_H
#define	I2CMOTOR_H

#include <cstdint>

class I2CMotor
{
    public:
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
        I2CMotor();
        I2CMotor(const I2CMotor& orig);
        virtual ~I2CMotor();
        int setPWMFrequency(unsigned char frequency_setting, uint8_t i2c_address);
        int begin(unsigned char i2c_address);
        void setMotorDirection(uint8_t motor_directions, uint8_t i2c_address);
        void setMotorSpeed(unsigned char motor_id, int new_speed, uint8_t i2c_address);
        int stopMotor(unsigned char motor_id, uint8_t i2c_address);

    private:

};

#endif	/* I2CMOTOR_H */

