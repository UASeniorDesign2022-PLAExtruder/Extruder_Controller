/*********************************************************************************
 * Extruder_Controller
 * I2CMotor.h
 * Wilson Woods
 * 11.19.2021
 ********************************************************************************/

#ifndef I2CMOTOR_H
#define	I2CMOTOR_H

#include <cstdint>
#include <vector>

class I2CMotor
{
    public:
        
        uint8_t controller_I2C_address  = 0;
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
        
        std::vector<Motor_Object> motor_objects = { motor_1, motor_2 };

        uint8_t DIRECTION_SET           = 0xaa;
        uint8_t MOTOR_DIR_BOTH_CW       = 0x0a;
        uint8_t MOTOR_DIR_BOTH_CCW      = 0x05;
        uint8_t MOTOR_DIR_M1CW_M2CCW    = 0x06;
        uint8_t MOTOR_DIR_M1CCW_M2CW    = 0x09;
        uint8_t MOTOR_SPEED_SET         = 0x82;
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
        // clockwise = 1 | counterclockwise = -1
        int DIRECTION_MOTOR_1 = 1;
        int DIRECTION_MOTOR_2 = 1;
        
        I2CMotor(uint16_t i2c_address);
        I2CMotor(const I2CMotor& orig);
        virtual ~I2CMotor();
        void setPWMFrequency();
        float getMotorSpeed(unsigned char motor_id);
        void setMotorDirection(uint8_t motor_directions);
        void setMotorSpeed(unsigned char motor_id, unsigned short new_speed, char new_direction);
        void nudgeMotorSpeedUp(unsigned char motor_id, unsigned char amount);
        void nudgeMotorSpeedDown(unsigned char motor_id, unsigned char amount);
        int stopMotor(unsigned char motor_id);

    private:

};

#endif	/* I2CMOTOR_H */

