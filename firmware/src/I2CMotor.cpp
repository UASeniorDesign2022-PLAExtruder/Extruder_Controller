/*********************************************************************************
 * Extruder_Controller
 * I2CMotor.cpp
 * Wilson Woods
 * 11.19.2021
 ********************************************************************************/

#include <cstdint>
#include <vector>
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "I2CMotor.h"
#include "globals.h"
#include "I2C.h"

/**
 * 
 */
I2CMotor::I2CMotor(uint16_t i2c_address)
{
    controller_I2C_address = i2c_address;
    setPWMFrequency();
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

void I2CMotor::setPWMFrequency()
{
    I2C_1_IS_BUSY = true;
    I2C_1_Start();
    CORETIMER_DelayUs(5);
    I2C_1_Send_Byte(controller_I2C_address << 1);
    CORETIMER_DelayUs(10);
    I2C_1_Send_Byte(PWMFrequencySet);
    CORETIMER_DelayUs(10);
    I2C_1_Send_Byte(F_3921Hz);
    CORETIMER_DelayUs(10);
    I2C_1_Send_Byte(NOTHING);
    I2C_1_Stop();
    I2C_1_IS_BUSY = false;
    CORETIMER_DelayMs(4);
}

float I2CMotor::getMotorSpeed(unsigned char motor_id)
{
    return (float)motor_objects[motor_id].current_speed;
}

void I2CMotor::setMotorDirection(uint8_t motor_directions)
{
    I2C_1_IS_BUSY = true;
    I2C_1_Start();
    CORETIMER_DelayUs(5);
    I2C_1_Send_Byte(controller_I2C_address << 1);
    CORETIMER_DelayUs(10);
    I2C_1_Send_Byte(DIRECTION_SET);
    CORETIMER_DelayUs(10);
    I2C_1_Send_Byte(motor_directions);
    CORETIMER_DelayUs(10);
    I2C_1_Send_Byte(NOTHING);
    I2C_1_Stop();
    I2C_1_IS_BUSY = false;
    CORETIMER_DelayUs(200);
}

// motor_id = 0 for motor_1 and 1 for motor_2
void I2CMotor::setMotorSpeed(unsigned char motor_id, unsigned short new_speed, char new_direction)
{
    motor_objects[motor_id].current_direction = new_direction;
    if (new_speed >= 255)
        motor_objects[motor_id].current_speed = 255;
    else
        motor_objects[motor_id].current_speed = new_speed;
    // Set the direction
    if (motor_objects[0].current_direction == 1 && motor_objects[1].current_direction == 1)
        setMotorDirection(MOTOR_DIR_BOTH_CW);
    if (motor_objects[0].current_direction == 1 && motor_objects[1].current_direction == -1)
        setMotorDirection(MOTOR_DIR_M1CW_M2CCW);
    if (motor_objects[0].current_direction == -1 && motor_objects[1].current_direction == 1)
        setMotorDirection(MOTOR_DIR_M1CCW_M2CW);
    if (motor_objects[0].current_direction == -1 && motor_objects[1].current_direction == -1)
        setMotorDirection(MOTOR_DIR_BOTH_CCW);
    // send command
    I2C_1_IS_BUSY = true;
    I2C_1_Start();
    CORETIMER_DelayUs(5);
    I2C_1_Send_Byte(controller_I2C_address << 1);
    CORETIMER_DelayUs(10);
    I2C_1_Send_Byte(MOTOR_SPEED_SET);
    CORETIMER_DelayUs(10);
    I2C_1_Send_Byte(motor_objects[0].current_speed);
    CORETIMER_DelayUs(10);
    I2C_1_Send_Byte(motor_objects[1].current_speed);
    I2C_1_Stop();
    I2C_1_IS_BUSY = false;
    CORETIMER_DelayUs(200);    
}

void I2CMotor::nudgeMotorSpeedUp(unsigned char motor_id, unsigned char amount)
{
    short new_speed = motor_objects[motor_id].current_speed + amount;
    if (new_speed >= 255)
        new_speed = 255;
    setMotorSpeed(motor_id, new_speed, motor_objects[motor_id].current_direction);
}
void I2CMotor::nudgeMotorSpeedDown(unsigned char motor_id, unsigned char amount)
{
    short new_speed = motor_objects[motor_id].current_speed - amount;
    if (new_speed <= 0)
        stopMotor(motor_id);
    else
        setMotorSpeed(motor_id, new_speed, motor_objects[motor_id].current_direction);
}

int I2CMotor::stopMotor(unsigned char motor_id)
{
    setMotorSpeed(motor_id, 0, 1);
    return 0;
}
