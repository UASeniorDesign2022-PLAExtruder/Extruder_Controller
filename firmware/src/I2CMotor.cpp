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
 * I2CMotor()
 * @param i2c_address I2C address of the motor controller
 */
I2CMotor::I2CMotor( uint16_t i2c_address )
{
    I2C_ADDRESS = i2c_address;
    set_PWM_frequency();
}

/**
 * 
 */
I2CMotor::~I2CMotor()
{
    
}

void I2CMotor::send_I2C(uint8_t flag, uint8_t data_byte_1, uint8_t data_byte_2)
{
    I2C_1_IS_BUSY = true;
    I2C_1_start();
    CORETIMER_DelayUs( 5 );
    I2C_1_send_byte( I2C_ADDRESS << 1 );
    CORETIMER_DelayUs( 10 );
    I2C_1_send_byte( flag );
    CORETIMER_DelayUs( 10 );
    I2C_1_send_byte( data_byte_1 );
    CORETIMER_DelayUs( 10 );
    I2C_1_send_byte( data_byte_2 );
    I2C_1_stop();
    I2C_1_IS_BUSY = false;
}
void I2CMotor::set_PWM_frequency()
{
    send_I2C(FREQ_SET, F_3921Hz, EMPTY);
    CORETIMER_DelayUs( 200 );
}

float I2CMotor::get_motor_speed( unsigned char motor_id )
{
    // return ((float)motor_objects[motor_id].current_speed / 255 ) * 100;
    return ((float)motor_objects[motor_id].current_speed);
}

void I2CMotor::set_motor_direction( uint8_t motor_directions )
{
    send_I2C(DIRECTION_SET, motor_directions, EMPTY);
    CORETIMER_DelayUs( 200 );
}

// motor_id = 0 for motor_1 and 1 for motor_2
float I2CMotor::set_speed( unsigned char motor_id, unsigned short new_speed )
{
    char new_direction = 1;
    
    if ( new_speed < 0 )
    {
        new_direction = -1;
        new_speed = new_speed * -1;
    }
    
    motor_objects[motor_id].current_direction = new_direction;
    
    if ( new_speed >= 255 || new_speed <= -255)
    {
        motor_objects[motor_id].current_speed = 255;
    }
    else
    {
        motor_objects[motor_id].current_speed = new_speed;
    }
    
    // Set the direction
    if ( motor_objects[0].current_direction == 1 && motor_objects[1].current_direction == 1 )
    {
        set_motor_direction( BOTH_CW );
    }
    if ( motor_objects[0].current_direction == 1 && motor_objects[1].current_direction == -1 )
    {
        set_motor_direction( CW_CCW );
    }
    if ( motor_objects[0].current_direction == -1 && motor_objects[1].current_direction == 1 )
    {
        set_motor_direction( CCW_CW );
    }
    if ( motor_objects[0].current_direction == -1 && motor_objects[1].current_direction == -1 )
    {
        set_motor_direction( BOTH_CCW );
    }
    
    // send command
    send_I2C(SPEED_SET, motor_objects[0].current_speed, motor_objects[1].current_speed);
    CORETIMER_DelayUs( 200 );
    
    // return ((float)motor_objects[motor_id].current_speed / 255 ) * 100;
    return ((float)motor_objects[motor_id].current_speed);
}

float I2CMotor::nudge_up( unsigned char motor_id, unsigned char amount )
{
    short new_speed = 0;
    
    if ( motor_objects[motor_id].current_speed < 0 )
    {
        new_speed = motor_objects[motor_id].current_speed - amount;
    }
    else
    {
        new_speed = motor_objects[motor_id].current_speed + amount;
    }
    
    return set_speed( motor_id, new_speed );
}
float I2CMotor::nudge_down( unsigned char motor_id, unsigned char amount )
{
    short new_speed = 0;
    
    if ( motor_objects[motor_id].current_speed < 0 )
    {
        new_speed = motor_objects[motor_id].current_speed + amount;
    }
    else
    {
        new_speed = motor_objects[motor_id].current_speed - amount;
    }
    
    return set_speed( motor_id, new_speed );
}

float I2CMotor::stop( unsigned char motor_id )
{
    return set_speed( motor_id, 0 );
}