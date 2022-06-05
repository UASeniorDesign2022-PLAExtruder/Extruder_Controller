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

#include "RTDSensor.h"

RTDSensor::RTDSensor(float r_nom, float r_ref, GPIO_PIN chip_select)
{
    R_NOM = r_nom;
    R_REF = r_ref;
    RTD_CS = chip_select;
    RTD_begin(MAX31865_3WIRE);
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

    
uint8_t RTDSensor::readRegister8(uint8_t addr)
{
    addr &= 0x7F;
    uint8_t ret = 0;
    GPIO_PinClear(RTD_CS);
    SPI_transfer_8(addr);
    GPIO_PinSet(RTD_CS);
    ret = SPI1BUF;
    return ret;
}

uint16_t RTDSensor::readRegister16(uint8_t addr1, uint8_t addr2)
{
    addr1 &= 0x7F;
    addr2 &= 0x7F;
    uint16_t ret = 0;
    GPIO_PinClear(RTD_CS);
    SPI_transfer_8(0x01);
    ret = SPI_transfer_8(0xFF);
    ret <<= 8;
    SPI_transfer_8(0x02);
    ret |= SPI_transfer_8(0xFF);
    CORETIMER_DelayUs(1);
    GPIO_PinSet(RTD_CS);
    return ret;
}


void RTDSensor::writeRegister8(uint8_t addr, uint8_t data)
{
    addr |= 0x80;
    GPIO_PinClear(RTD_CS);
    CORETIMER_DelayUs(1);
    SPI_transfer_8(addr);
    SPI_transfer_8(data);
    CORETIMER_DelayUs(1);
    GPIO_PinSet(RTD_CS);
}

bool RTDSensor::RTD_begin(max31865_numwires_t wires)
{
    GPIO_PinOutputEnable(RTD_CS);
    GPIO_PinSet(RTD_CS);
    SPI_RTD_init_8();
    CORETIMER_DelayUs(100);
    writeRegister8(MAX31865_CONFIG_REG, 0xD3);
    return true;
}

uint16_t RTDSensor::readRTD(void)
{
    RTD_begin(MAX31865_3WIRE);
    uint16_t rtd = readRegister16(MAX31865_RTDMSB_REG, MAX31865_RTDLSB_REG);
    rtd >>= 1;
    return rtd;
}

float RTDSensor::temperature()
{
    float Z1, Z2, Z3, Z4, Rt, temp;

    Rt = readRTD();
    Rt /= 32768;
    Rt *= R_REF;

    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / R_NOM;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * Rt);
    temp = (sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
      return temp;

    Rt /= R_NOM;
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

float RTDSensor::get_temp()
{
    float temp = 0;
    temp = temperature();
    CORETIMER_DelayMs(17);
    return temp;
}