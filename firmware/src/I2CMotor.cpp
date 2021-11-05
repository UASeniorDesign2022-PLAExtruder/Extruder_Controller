/* 
 * File:   I2CMotor.cpp
 * Author: wilson
 * 
 * Created on November 5, 2021, 2:03 AM
 */

#include <cstdint>
#include "config/default/peripheral/coretimer/plib_coretimer.h"
#include "I2CMotor.h"
#include "I2C.h"

/**
 * 
 */
I2CMotor::I2CMotor()
{
    
}

/**
 * 
 */
I2CMotor::I2CMotor(const I2CMotor& orig)
{
    
}

/**
 * 
 */
I2CMotor::~I2CMotor()
{
    
}

int I2CMotor::setPWMFrequency(unsigned char frequency_setting, uint8_t i2c_address)
{
    if (frequency_setting < F_31372Hz || frequency_setting > F_30Hz)
        return -1;
    I2CStart();
    CORETIMER_DelayUs(5);
    I2Csendbyte((i2c_address << 1));
    CORETIMER_DelayUs(10);
    I2Csendbyte( PWMFrequencySet );
    CORETIMER_DelayUs(10);
    I2Csendbyte( frequency_setting );
    CORETIMER_DelayUs(10);
    I2Csendbyte( NOTHING );
    I2CStop();
    CORETIMER_DelayMs(4);
    return 0;
}

int I2CMotor::begin(unsigned char i2c_address)
{
    if (i2c_address > 0x0f)
        return -1;
    CORETIMER_DelayMs(1000);
    // Set default frequence to F_3921Hz
    setPWMFrequency(F_3921Hz, i2c_address);
    return 0;
}

void I2CMotor::setMotorDirection(uint8_t motor_directions, uint8_t i2c_address)
{
    I2CStart();
    CORETIMER_DelayUs(5);
    I2Csendbyte((i2c_address << 1));
    CORETIMER_DelayUs(10);
    I2Csendbyte( DIRECTION_SET_CODE );
    CORETIMER_DelayUs(10);
    I2Csendbyte( motor_directions );
    CORETIMER_DelayUs(10);
    I2Csendbyte( NOTHING );
    I2CStop();
    CORETIMER_DelayMs(4);
}

void I2CMotor::setMotorSpeed(unsigned char motor_id, int new_speed, uint8_t i2c_address)
{
    if (motor_id < MOTOR_1 || motor_id > MOTOR_2)
        return;
    if (motor_id == MOTOR_1)
    {
        if (new_speed >= 0)
        {
            DIRECTION_MOTOR_1 = 1;
            if (new_speed > 255)
                SPEED_MOTOR_1 = 255;
            else
                SPEED_MOTOR_1 = new_speed;
        }
        else if (new_speed < 0)
        {
            DIRECTION_MOTOR_1 = -1;
            if (new_speed < -255)
                SPEED_MOTOR_1 = 255;
            else
                SPEED_MOTOR_1 = -(new_speed);
        }
    }
    else if (motor_id == MOTOR_2)
    {
        if (new_speed >= 0)
        {
            DIRECTION_MOTOR_2 = 1;
            if (new_speed > 255)
                SPEED_MOTOR_2 = 255;
            else
                SPEED_MOTOR_2 = new_speed;
        }
        else if (new_speed < 0)
        {
            DIRECTION_MOTOR_2 = -1;
            if (new_speed < -255)
                SPEED_MOTOR_2 = 255;
            else
                SPEED_MOTOR_2 = -(new_speed);
        }
    }
    // Set the direction
    if (DIRECTION_MOTOR_1 == 1 && DIRECTION_MOTOR_2 == 1)
    {
        setMotorDirection(MOTOR_DIR_BOTH_CW, i2c_address);
    }
    if (DIRECTION_MOTOR_1 == 1 && DIRECTION_MOTOR_2 == -1)
    {
        setMotorDirection(MOTOR_DIR_M1CW_M2CCW, i2c_address);
    }
    if (DIRECTION_MOTOR_1 == -1 && DIRECTION_MOTOR_2 == 1)
    {
        setMotorDirection(MOTOR_DIR_M1CCW_M2CW, i2c_address);
    }
    if (DIRECTION_MOTOR_1 == -1 && DIRECTION_MOTOR_2 == -1)
    {
        setMotorDirection(MOTOR_DIR_BOTH_CCW, i2c_address);
    }
    // send command
    I2CStart();
    CORETIMER_DelayUs(5);
    I2Csendbyte((i2c_address << 1));
    CORETIMER_DelayUs(10);
    I2Csendbyte( MotorSpeedSet );
    CORETIMER_DelayUs(10);
    I2Csendbyte( SPEED_MOTOR_1 );
    CORETIMER_DelayUs(10);
    I2Csendbyte( SPEED_MOTOR_2 );
    I2CStop();
    CORETIMER_DelayMs(4);
}

    // Stop one motor
    // motor_id: MOTOR1, MOTOR2
int I2CMotor::stopMotor(unsigned char motor_id, uint8_t i2c_address)
{
    if (motor_id < MOTOR_1 || motor_id > MOTOR_2)
        return -1;
    setMotorSpeed(motor_id, 0, i2c_address);
    return 0;
}
