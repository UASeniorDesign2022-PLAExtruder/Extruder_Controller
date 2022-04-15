/*********************************************************************************
 * Extruder_Controller
 * I2CMotor.h
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
 ******************************************************************************/

#ifndef I2CMOTOR_H
#define	I2CMOTOR_H

#include <cstdint>
#include <vector>

class I2CMotor
{
    public:
        
        uint8_t I2C_ADDRESS  = 0;
        const char CLOCKWISE = 1;
        const char COUNTERCLOCKWISE = -1;
        
        enum MOTOR_ID
        {
            MOTOR_NULL,
            MOTOR_1,
            MOTOR_2
        };
        
        typedef struct
        {
            MOTOR_ID motor_id;
            unsigned short current_speed;
            short current_direction;
        } Motor_Object;
        
        // motor_1 and motor_2 init speed = 0, direction = clockwise
        Motor_Object motor_1 = { MOTOR_1, 0, 1 };
        Motor_Object motor_2 = { MOTOR_2, 0, 1 };
        
        // store motor objects in a vector in order to access by subscript
        std::vector<Motor_Object> motor_objects = { motor_1, motor_2 };

        // bytes for communicating with I2C motor controller
        uint8_t DIRECTION_SET   = 0xaa;     // indicate next byte is direction
        uint8_t SPEED_SET       = 0x82;     // indicate next byte is speed
        uint8_t FREQ_SET        = 0x84;     // indicate next byte is frequency
        
        // motor directions
        uint8_t BOTH_CW         = 0x0a;     // both clockwise
        uint8_t BOTH_CCW        = 0x05;     // both counter
        uint8_t CW_CCW          = 0x06;     // M1 clockwise, M2 counter
        uint8_t CCW_CW          = 0x09;     // M1 counter, M2 clockwise
        
        uint8_t EMPTY           = 0x01;     // send empty byte
        uint8_t F_3921Hz        = 0x02;     // use this one
        
        // alternative PWM frequencies compatible with I2C motor controller
        uint8_t F_31372Hz       = 0x01;
        uint8_t F_490Hz         = 0x03;
        uint8_t F_122Hz         = 0x04;
        uint8_t F_30Hz          = 0x05;

        // speed -255 to 255 ( 0 = stop )
        unsigned char SPEED_MOTOR_1 = 0;
        unsigned char SPEED_MOTOR_2 = 0;
        
        // clockwise = 1 | counterclockwise = -1, internal to class methods
        // interface allows negative speed values to indicate direction
        // functions interpolate behind the scenes
        int DIRECTION_MOTOR_1 = 1;
        int DIRECTION_MOTOR_2 = 1;
        
        I2CMotor( uint16_t i2c_address );
        virtual ~I2CMotor();
        void send_I2C( uint8_t flag, uint8_t data_byte_1 = 0x01, uint8_t data_byte_2 = 0x01 );
        void set_PWM_frequency( void );
        float get_motor_speed( unsigned char motor_id );
        void set_motor_direction( uint8_t motor_directions );
        float set_speed( unsigned char motor_id, short new_speed );
        float nudge_up( unsigned char motor_id, unsigned char amount );
        float nudge_down( unsigned char motor_id, unsigned char amount );
        float stop( unsigned char motor_id );

    private:

};

#endif	/* I2CMOTOR_H */

