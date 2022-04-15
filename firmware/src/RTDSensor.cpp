/* 
 * File:   RTDSensor.cpp
 * Author: wilson
 * 
 * Created on April 13, 2022, 10:03 PM
 */

#include <cstdlib>
#include <cmath>

#include "config/default/peripheral/spi/spi_master/plib_spi1_master.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"
#include "config/default/peripheral/gpio/plib_gpio.h"

#include "globals.h"
#include "RTDSensor.h"

RTDSensor::RTDSensor()
{
    SPI_RTD_init_8();
    RTD_begin_CS_1(MAX31865_3WIRE);
    RTD_begin_CS_2(MAX31865_3WIRE);
    RTD_begin_CS_3(MAX31865_3WIRE);
}

void RTDSensor::SPI_RTD_init_8( void )
{
    uint32_t rdata = 0U;
    IEC1CLR = 0x8;
    IEC1CLR = 0x10;
    IEC1CLR = 0x20;
    SPI1CON = 0;
    rdata = SPI1BUF;
    rdata = rdata;
    IFS1CLR = 0x8;
    IFS1CLR = 0x10;
    IFS1CLR = 0x20;
    SPI1BRG = 23;
    SPI1STATCLR = _SPI1STAT_SPIROV_MASK;
    SPI1CON = 0x8220;
}

 uint8_t RTDSensor::SPI_transfer_8( uint8_t data )
{
    SPI_RTD_init_8();
    SPI1BUF = data;
    while (SPI1STATbits.SPIRBE);
    return (uint8_t)SPI1BUF;
}

    
uint8_t RTDSensor::readRegister8_CS_1(uint8_t addr)
{
    addr &= 0x7F;
    uint8_t ret = 0;
    SS_TEMP_1_Clear();
    SPI_transfer_8(addr);
    SS_TEMP_1_Set();

    ret = SPI1BUF;
    return ret;
}

uint8_t RTDSensor::readRegister8_CS_2(uint8_t addr)
{
    addr &= 0x7F;
    uint8_t ret = 0;
    SS_TEMP_2_Clear();
    SPI_transfer_8(addr);
    SS_TEMP_2_Set();

    ret = SPI1BUF;
    return ret;
}

uint8_t RTDSensor::readRegister8_CS_3(uint8_t addr)
{
    addr &= 0x7F;
    uint8_t ret = 0;
    SS_TEMP_3_Clear();
    SPI_transfer_8(addr);
    SS_TEMP_3_Set();

    ret = SPI1BUF;
    return ret;
}

uint16_t RTDSensor::readRegister16_CS_1(uint8_t addr1, uint8_t addr2)
{
    while(SPI_IS_BUSY);
    SPI_IS_BUSY = true;
    addr1 &= 0x7F;
    addr2 &= 0x7F;
    uint16_t ret = 0;

    SS_TEMP_1_Clear();
    SPI_transfer_8(addr1);
    ret = SPI_transfer_8(0xFF);
    ret <<= 8;
    SPI_transfer_8(addr2);
    ret |= SPI_transfer_8(0xFF);
    CORETIMER_DelayUs(1);
    SS_TEMP_1_Set();
    SPI_IS_BUSY = false;
    return ret;
}

uint16_t RTDSensor::readRegister16_CS_2(uint8_t addr1, uint8_t addr2)
{
    while(SPI_IS_BUSY);
    SPI_IS_BUSY = true;
    addr1 &= 0x7F;
    addr2 &= 0x7F;
    uint16_t ret = 0;

    SS_TEMP_2_Clear();
    SPI_transfer_8(addr1);
    ret = SPI_transfer_8(0xFF);
    ret <<= 8;
    SPI_transfer_8(addr2);
    ret |= SPI_transfer_8(0xFF);
    CORETIMER_DelayUs(1);
    SS_TEMP_2_Set();
    SPI_IS_BUSY = false;
    return ret;
}

uint16_t RTDSensor::readRegister16_CS_3(uint8_t addr1, uint8_t addr2)
{
    while(SPI_IS_BUSY);
    SPI_IS_BUSY = true;
    addr1 &= 0x7F;
    addr2 &= 0x7F;
    uint16_t ret = 0;

    SS_TEMP_3_Clear();
    SPI_transfer_8(addr1);
    ret = SPI_transfer_8(0xFF);
    ret <<= 8;
    SPI_transfer_8(addr2);
    ret |= SPI_transfer_8(0xFF);
    CORETIMER_DelayUs(1);
    SS_TEMP_3_Set();
    SPI_IS_BUSY = false;
    return ret;
}


void RTDSensor::writeRegister8_CS_1(uint8_t addr, uint8_t data)
{
     while(SPI_IS_BUSY);
    SPI_IS_BUSY = true;
    addr |= 0x80;

    SS_TEMP_1_Clear();
    CORETIMER_DelayUs(1);
    SPI_transfer_8(addr);
    SPI_transfer_8(data);
    CORETIMER_DelayUs(1);
    SS_TEMP_1_Set();

    SPI_IS_BUSY = false;
}

void RTDSensor::writeRegister8_CS_2(uint8_t addr, uint8_t data)
{
     while(SPI_IS_BUSY);
    SPI_IS_BUSY = true;
    addr |= 0x80;

    SS_TEMP_2_Clear();
    CORETIMER_DelayUs(1);
    SPI_transfer_8(addr);
    SPI_transfer_8(data);
    CORETIMER_DelayUs(1);
    SS_TEMP_2_Set();

    SPI_IS_BUSY = false;
}

void RTDSensor::writeRegister8_CS_3(uint8_t addr, uint8_t data)
{
     while(SPI_IS_BUSY);
    SPI_IS_BUSY = true;
    addr |= 0x80;

    SS_TEMP_3_Clear();
    CORETIMER_DelayUs(1);
    SPI_transfer_8(addr);
    SPI_transfer_8(data);
    CORETIMER_DelayUs(1);
    SS_TEMP_3_Set();

    SPI_IS_BUSY = false;
}

bool RTDSensor::RTD_begin_CS_1(max31865_numwires_t wires)
{

    SS_TEMP_1_Set();
    SS_TEMP_1_OutputEnable();

    CORETIMER_DelayMs(1000);
    writeRegister8_CS_1(MAX31865_CONFIG_REG, 0xD3);
    return true;
}

bool RTDSensor::RTD_begin_CS_2(max31865_numwires_t wires)
{

    SS_TEMP_2_Set();
    SS_TEMP_2_OutputEnable();

    CORETIMER_DelayMs(1000);
    writeRegister8_CS_2(MAX31865_CONFIG_REG, 0xD3);
    return true;
}

bool RTDSensor::RTD_begin_CS_3(max31865_numwires_t wires)
{

    SS_TEMP_3_Set();
    SS_TEMP_3_OutputEnable();

    CORETIMER_DelayMs(1000);
    writeRegister8_CS_3(MAX31865_CONFIG_REG, 0xD3);
    return true;
}

uint16_t RTDSensor::readRTD_CS_1(void)
{
//    RTD_begin(MAX31865_3WIRE);
    CORETIMER_DelayMs(65);
    uint16_t rtd = readRegister16_CS_1(MAX31865_RTDMSB_REG, MAX31865_RTDLSB_REG);
    rtd >>= 1;
    return rtd;
}

uint16_t RTDSensor::readRTD_CS_2(void)
{
//    RTD_begin(MAX31865_3WIRE);
    CORETIMER_DelayMs(65);
    uint16_t rtd = readRegister16_CS_2(MAX31865_RTDMSB_REG, MAX31865_RTDLSB_REG);
    rtd >>= 1;
    return rtd;
}

uint16_t RTDSensor::readRTD_CS_3(void)
{
//    RTD_begin(MAX31865_3WIRE);
    CORETIMER_DelayMs(65);
    uint16_t rtd = readRegister16_CS_3(MAX31865_RTDMSB_REG, MAX31865_RTDLSB_REG);
    rtd >>= 1;
    return rtd;
}



float RTDSensor::temperature_CS_1()
{
    float rtd_nominal = 1000.0;
    float rtd_reference = 4300.;
    float Z1, Z2, Z3, Z4, Rt, temp;

    Rt = readRTD_CS_1();
    Rt /= 32768;
    Rt *= rtd_reference;

    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / rtd_nominal;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * Rt);
    temp = (sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
      return temp;

    Rt /= rtd_nominal;
    Rt *= 100; // normalize to 100 ohm

    float rpoly = Rt;

    temp = -242.02;
    temp += 2.2228 * rpoly;
    rpoly *= Rt; // square
    temp += 2.5859e-3 * rpoly;
    rpoly *= Rt; // ^3
    temp -= 4.8260e-6 * rpoly;
    rpoly *= Rt; // ^4
    temp -= 2.8183e-8 * rpoly;
    rpoly *= Rt; // ^5
    temp += 1.5243e-10 * rpoly;

    return temp;
}

float RTDSensor::temperature_CS_2()
{
    float rtd_nominal = 100.0;
    float rtd_reference = 430.;
    float Z1, Z2, Z3, Z4, Rt, temp;

    Rt = readRTD_CS_2();
    Rt /= 32768;
    Rt *= rtd_reference;

    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / rtd_nominal;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * Rt);
    temp = (sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
      return temp;

    Rt /= rtd_nominal;
    Rt *= 100; // normalize to 100 ohm

    float rpoly = Rt;

    temp = -242.02;
    temp += 2.2228 * rpoly;
    rpoly *= Rt; // square
    temp += 2.5859e-3 * rpoly;
    rpoly *= Rt; // ^3
    temp -= 4.8260e-6 * rpoly;
    rpoly *= Rt; // ^4
    temp -= 2.8183e-8 * rpoly;
    rpoly *= Rt; // ^5
    temp += 1.5243e-10 * rpoly;

    return temp;
}

float RTDSensor::temperature_CS_3()
{
    float rtd_nominal = 100.0;
    float rtd_reference = 430.;
    float Z1, Z2, Z3, Z4, Rt, temp;

    Rt = readRTD_CS_3();
    Rt /= 32768;
    Rt *= rtd_reference;

    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / rtd_nominal;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * Rt);
    temp = (sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
      return temp;

    Rt /= rtd_nominal;
    Rt *= 100; // normalize to 100 ohm

    float rpoly = Rt;

    temp = -242.02;
    temp += 2.2228 * rpoly;
    rpoly *= Rt; // square
    temp += 2.5859e-3 * rpoly;
    rpoly *= Rt; // ^3
    temp -= 4.8260e-6 * rpoly;
    rpoly *= Rt; // ^4
    temp -= 2.8183e-8 * rpoly;
    rpoly *= Rt; // ^5
    temp += 1.5243e-10 * rpoly;

    return temp;
}

float RTDSensor::get_temp_CS_1()
{
    RTD_begin_CS_1(MAX31865_3WIRE);
    float temp = 0;
    temp = temperature_CS_1();
    CORETIMER_DelayMs(17);
    return temp;
}

float RTDSensor::get_temp_CS_2()
{
    RTD_begin_CS_2(MAX31865_3WIRE);
    float temp = 0;
    temp = temperature_CS_2();
    CORETIMER_DelayMs(17);
    return temp;
}
float RTDSensor::get_temp_CS_3()
{
    RTD_begin_CS_3(MAX31865_3WIRE);
    float temp = 0;
    temp = temperature_CS_3();
    CORETIMER_DelayMs(17);
    return temp;
}

void RTDSensor::get_all_temps()
{
    temp_1 = get_temp_CS_1();
    CORETIMER_DelayMs(10);
    temp_2 = get_temp_CS_2();
    CORETIMER_DelayMs(10);
    temp_3 = get_temp_CS_3();
    CORETIMER_DelayMs(10);
    
}