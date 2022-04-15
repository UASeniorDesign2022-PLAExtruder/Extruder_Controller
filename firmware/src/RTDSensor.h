/* 
 * File:   RTDSensor.h
 * Author: wilson
 *
 * Created on April 13, 2022, 10:03 PM
 */

#ifndef RTDSENSOR_H
#define	RTDSENSOR_H

class RTDSensor {
public:
    #define MAX31865_CONFIG_REG 0x00
    #define MAX31865_CONFIG_BIAS 0x80
    #define MAX31865_CONFIG_MODEAUTO 0x40
    #define MAX31865_CONFIG_MODEOFF 0x00
    #define MAX31865_CONFIG_1SHOT 0x20
    #define MAX31865_CONFIG_3WIRE 0x10
    #define MAX31865_CONFIG_24WIRE 0x00
    #define MAX31865_CONFIG_FAULTSTAT 0x02
    #define MAX31865_CONFIG_FILT50HZ 0x01
    #define MAX31865_CONFIG_FILT60HZ 0x00

    #define MAX31865_RTDMSB_REG 0x01
    #define MAX31865_RTDLSB_REG 0x02
    #define MAX31865_HFAULTMSB_REG 0x03
    #define MAX31865_HFAULTLSB_REG 0x04
    #define MAX31865_LFAULTMSB_REG 0x05
    #define MAX31865_LFAULTLSB_REG 0x06
    #define MAX31865_FAULTSTAT_REG 0x07

    #define MAX31865_FAULT_HIGHTHRESH 0x80
    #define MAX31865_FAULT_LOWTHRESH 0x40
    #define MAX31865_FAULT_REFINLOW 0x20
    #define MAX31865_FAULT_REFINHIGH 0x10
    #define MAX31865_FAULT_RTDINLOW 0x08
    #define MAX31865_FAULT_OVUV 0x04
    #define RTD_A 3.9083e-3
    #define RTD_B -5.775e-7

    #define RNOMINAL    100.0
    #define RREF        430.0

    typedef enum max31865_numwires
    {
        MAX31865_2WIRE = 0,
        MAX31865_3WIRE = 1,
        MAX31865_4WIRE = 0
    } max31865_numwires_t;

    RTDSensor();
    void SPI_RTD_init_8(void);
    uint8_t SPI_transfer_8( uint8_t data );
    uint8_t readRegister8_CS_1(uint8_t addr);
    uint8_t readRegister8_CS_2(uint8_t addr);
    uint8_t readRegister8_CS_3(uint8_t addr);
    uint16_t readRegister16_CS_1(uint8_t addr1, uint8_t addr2);
    uint16_t readRegister16_CS_2(uint8_t addr1, uint8_t addr2);
    uint16_t readRegister16_CS_3(uint8_t addr1, uint8_t addr2);
    void writeRegister8_CS_1(uint8_t addr, uint8_t data);
    void writeRegister8_CS_2(uint8_t addr, uint8_t data);
    void writeRegister8_CS_3(uint8_t addr, uint8_t data);
    bool RTD_begin_CS_1(max31865_numwires_t wires);
    bool RTD_begin_CS_2(max31865_numwires_t wires);
    bool RTD_begin_CS_3(max31865_numwires_t wires);
    uint16_t readRTD_CS_1(void);
    uint16_t readRTD_CS_2(void);
    uint16_t readRTD_CS_3(void);
    float temperature_CS_1();
    float temperature_CS_2();
    float temperature_CS_3();
    float get_temp_CS_1();
    float get_temp_CS_2();
    float get_temp_CS_3();
    
    void get_all_temps();


private:

};

#endif	/* RTDSENSOR_H */
