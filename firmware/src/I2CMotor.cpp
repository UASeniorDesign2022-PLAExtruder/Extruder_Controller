/*********************************************************************************
 * Extruder_Controller
 * I2CMotor.cpp
 * Wilson Woods
 * 11.19.2021
 * 
 * The I2CMotor class provides an interface for controlling the SeeedStudio
 * I2C motor controller using the PIC32MX470F512H:
 *
 *      https://wiki.seeedstudio.com/Grove-I2C_Motor_Driver_V1.3/
 * 
 * Each controller uses PWM to control up to 2 12V DC motors. PWM frequency,
 * motor speed, and motor direction can be controlled by the PIC32 by sending
 * the appropriate flag bytes (to indicate frequency, speed, or direction)
 * followed by the desired value corresponding to that flag. Each object of
 * this class represents the interface of one controller, and therefore 
 * controls up to 2 motors. The I2C address of controller is customizable
 * using switches on the controller itself (see link above). 
 ********************************************************************************/

#include <cstdint>
#include <vector>
#include "config/default/peripheral/coretimer/plib_coretimer.h"

#include "I2CMotor.h"
#include "globals.h"
#include "I2C.h"

/**
 * I2CMotor()
 * 
 * @param i2c_address I2C address of the motor controller
 */
I2CMotor::I2CMotor( uint16_t i2c_address )
{
    I2C_ADDRESS = i2c_address;
    set_PWM_frequency();
}

/**
 * ~I2CMotor()
 * 
 */
I2CMotor::~I2CMotor()
{
    
}

/**
 * send_I2C()
 * 
 * Helper function to send bytes from master to motor controller via I2C
 * Sends address followed by flag and 2 data bytes
 * 
 * @param flag Tells controller what byte is coming next (frequency, speed, or
 * direction value), see I2CMotor.h for applicable constants
 * 
 * @param data_byte_1 Frequency, speed, or direction value, again see .h file
 * 
 * @param data_byte_2 Frequency, speed, or direction value, both data_byte 1 and
 * 2 are passed the EMPTY byte by default
 */
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

/**
 * set_PWM_frequency()
 * 
 * This function sets operating frequency of the motor controller
 * 
 * Value can be selected from constants in I2CMotor.h
 * 
 * Set value on initialization, see I2CMotor() above
 */
void I2CMotor::set_PWM_frequency()
{
    send_I2C(FREQ_SET, F_3921Hz, EMPTY);
    CORETIMER_DelayUs( 200 );
}


/**
 * get_motor_speed()
 * 
 * @param motor_id Motor of which current speed is desired
 * 
 * @return Speed (-255, 255) of motor specified by motor_id
 */
float I2CMotor::get_motor_speed( unsigned char motor_id )
{
    return ((float)motor_objects[motor_id].current_speed);
}


/**
 * set_motor_directions()
 * 
 * Helper function for set_speed() below
 * 
 * Sets directions of both motors driven by the motor controller
 * 
 * @param motor_directions Selected from symbolic constants in I2CMotor.h,
 * this argument is determined by logic in set_speed()
 * 
 * motor_directions can be any combination of clockwise and counterclockwise
 * for the two motors
 */
void I2CMotor::set_motor_direction( uint8_t motor_directions )
{
    send_I2C(DIRECTION_SET, motor_directions, EMPTY);
    CORETIMER_DelayUs( 200 );
}

/**
 * set_speed()
 * 
 * @param motor_id  Motor to adjust, 0 for motor_1 and 1 for motor_2
 * 
 * @param new_speed Value from -255 to 255 (-255 and 255 being 100% duty cycle
 * in clockwise and counterclockwise respectively)
 * 
 * @return new speed setting for the given motor
 */
float I2CMotor::set_speed( unsigned char motor_id, unsigned short new_speed )
{
    char new_direction = 1;
    
    // handle counterclockwise direction setting
    if ( new_speed < 0 )
    {
        new_direction = -1;
        new_speed = new_speed * -1;
    }
    
    // set new direction for selected motor based on +/- new speed
    motor_objects[motor_id].current_direction = new_direction;
    
    // if new_speed is greater than max, set new speed to max
    if ( new_speed >= 255 || new_speed <= -255)
    {
        motor_objects[motor_id].current_speed = 255;
    }
    else
    {
        motor_objects[motor_id].current_speed = new_speed;
    }
    
    // I2C motor controller requires both motor directions be set at once, so
    // we need to set the new direction for the current motor while maintaining
    // the other motor's direction
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
    
    // return the new speed
    return ((float)motor_objects[motor_id].current_speed);
}

/**
 * nudge_up()
 * 
 * @param motor_id Motor to adjust, 0 for motor_1 and 1 for motor_2
 * 
 * @param amount Value by which to increase the selected motor's speed
 * 
 * @return Call to set_speed() which carries out the increase and returns the
 * newly set speed
 */
float I2CMotor::nudge_up( unsigned char motor_id, unsigned char amount )
{
    short new_speed = 0;
    
    // current speed negative -> subtract to "nudge up"
    if ( motor_objects[motor_id].current_speed < 0 )
    {
        new_speed = motor_objects[motor_id].current_speed - amount;
    }
    // current speed positive -> add to "nudge up"
    else
    {
        new_speed = motor_objects[motor_id].current_speed + amount;
    }
    
    // call set_speed() with new_speed calculated above
    return set_speed( motor_id, new_speed );
}

/**
 * nudge_down()
 * 
 * @param motor_id Motor to adjust, 0 for motor_1 and 1 for motor_2
 * @param amount Value by which to decrease the selected motor's speed
 * @return 
 */
float I2CMotor::nudge_down( unsigned char motor_id, unsigned char amount )
{
    short new_speed = 0;
    
    // current speed negative -> add to "nudge down"
    if ( motor_objects[motor_id].current_speed < 0 )
    {
        new_speed = motor_objects[motor_id].current_speed + amount;
    }
    // current speed positive -> subtract to "nudge down"
    else
    {
        new_speed = motor_objects[motor_id].current_speed - amount;
    }
    
    // call set_speed() with new_speed calculated above
    return set_speed( motor_id, new_speed );
}

/**
 * stop()
 * 
 * @param motor_id Motor to adjust, 0 for motor_1 and 1 for motor_2
 * 
 * @return Call to set_speed() which in turn returns the new speed
 */
float I2CMotor::stop( unsigned char motor_id )
{
    return set_speed( motor_id, 0 );
}