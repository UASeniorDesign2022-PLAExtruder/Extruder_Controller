/*******************************************************************************
 * Extruder_Controller
 * RTD.cpp
 * Wilson Woods
 * 4.10.2022
 *******************************************************************************/

#include <cstdlib>
#include <cmath>

#include "config/default/peripheral/spi/spi_master/plib_spi1_master.h"
#include "config/default/peripheral/coretimer/plib_coretimer.h"
#include "config/default/peripheral/gpio/plib_gpio.h"

#include "RTD.h"

void SPI_RTD_init_8( void )
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

void SPI_RTD_init_16( void )
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
    SPI1BRG = 23;                           // 4MHz clock
    SPI1STATCLR = _SPI1STAT_SPIROV_MASK;
    SPI1CON = 0x8620;                       // PIC32 master, 16-bit mode
}


bool RTD_begin(max31865_numwires_t wires)
{
    SS_TEMP_1_OutputEnable();
    SS_TEMP_1_Set();
    CORETIMER_DelayUs(100);
    writeRegister8(MAX31865_CONFIG_REG, 0xD3);
    return true;
}

uint8_t readFault(void)
{
  return readRegister8(MAX31865_FAULTSTAT_REG);
}

void clearFault(void)
{
  uint8_t t = readRegister8(MAX31865_CONFIG_REG);
  t &= ~0x2C;
  t |= MAX31865_CONFIG_FAULTSTAT;
  writeRegister8(MAX31865_CONFIG_REG, t);
}

void enableBias(bool b)
{
  uint8_t t = readRegister8(MAX31865_CONFIG_REG);
  if (b)
  {
    t |= MAX31865_CONFIG_BIAS; // enable bias
  }
  else
  {
    t &= ~MAX31865_CONFIG_BIAS; // disable bias
  }
  writeRegister8(MAX31865_CONFIG_REG, t);
}

void autoConvert(bool b)
{
  uint8_t t = readRegister8(MAX31865_CONFIG_REG);
  if (b)
  {
    t |= MAX31865_CONFIG_MODEAUTO; // enable autoconvert
  }
  else
  {
    t &= ~MAX31865_CONFIG_MODEAUTO; // disable autoconvert
  }
  writeRegister8(MAX31865_CONFIG_REG, t);
}

void enable50Hz(bool b)
{
  uint8_t t = readRegister8(MAX31865_CONFIG_REG);
  if (b)
  {
    t |= MAX31865_CONFIG_FILT50HZ;
  }
  else
  {
    t &= ~MAX31865_CONFIG_FILT50HZ;
  }
  writeRegister8(MAX31865_CONFIG_REG, t);
}

void setWires(max31865_numwires_t wires)
{
  uint8_t t = readRegister8(MAX31865_CONFIG_REG);
  if (wires == MAX31865_3WIRE)
  {
    t |= MAX31865_CONFIG_3WIRE;
  }
  else
  {
    // 2 or 4 wire
    t &= ~MAX31865_CONFIG_3WIRE;
  }
  writeRegister8(MAX31865_CONFIG_REG, t);
}

float temperature(float RTDnominal, float refResistor)
{
    float Z1, Z2, Z3, Z4, Rt, temp;

    Rt = readRTD();
    Rt /= 32768;
    Rt *= refResistor;

    Z1 = -RTD_A;
    Z2 = RTD_A * RTD_A - (4 * RTD_B);
    Z3 = (4 * RTD_B) / RTDnominal;
    Z4 = 2 * RTD_B;

    temp = Z2 + (Z3 * Rt);
    temp = (sqrt(temp) + Z1) / Z4;

    if (temp >= 0)
      return temp;

    Rt /= RTDnominal;
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

uint16_t readRTD(void)
{
    uint16_t rtd = readRegister16(MAX31865_RTDMSB_REG, MAX31865_RTDLSB_REG);
    rtd >>= 1;
    return rtd;
}

 uint8_t SPI_transfer_8( uint8_t data )
{
    SPI_RTD_init_8();
    SPI1BUF = data;
    while (SPI1STATbits.SPIRBE);
    return (uint8_t)SPI1BUF;
}

uint16_t SPI_transfer_16( uint8_t data1, uint8_t data2 )
{
    SPI_RTD_init_16();
    uint16_t result = 0;
    uint16_t transmission = 0;
    transmission = data1 << 8;
    transmission |= data2;
    SPI1BUF = transmission;
    while (SPI1STATbits.SPIRBE);
    return SPI1BUF;
}
    
uint8_t readRegister8(uint8_t addr)
{
    addr &= 0x7F;
    uint8_t ret = 0;
    SS_TEMP_1_Clear();
    SPI_transfer_8(addr);
    SS_TEMP_1_Set();
    ret = SPI1BUF;
    return ret;
}

uint16_t readRegister16(uint8_t addr1, uint8_t addr2)
{
    addr1 &= 0x7F;
    addr2 &= 0x7F;
    uint16_t ret = 0;
    
    SS_TEMP_1_Clear();
    CORETIMER_DelayUs(1);
    SPI_transfer_8(addr1);
    ret = SPI_transfer_8(0xFF);
    ret <<= 8;
    SPI_transfer_8(addr2);
    ret |= SPI_transfer_8(0xFF);
    CORETIMER_DelayUs(1);
    SS_TEMP_1_Set();
    return ret;
}


void writeRegister8(uint8_t addr, uint8_t data)
{
    addr |= 0x80;
    SS_TEMP_1_Clear();
    CORETIMER_DelayUs(1);
    SPI_transfer_8(addr);
    SPI_transfer_8(data);
    CORETIMER_DelayUs(1);
    SS_TEMP_1_Set(); 
}

float get_temp()
{
    float temp = 0;
    temp = temperature(RNOMINAL, RREF);
    CORETIMER_DelayMs(17);
    return temp;
}



//float CallendarVanDusen(float rt)
//{
//   float a     = 3.9083E-03;
//   float b     = -5.7750E-07;
//   //float c = -4.18301E-12 for -200C < T < 0C or 0 otherwise
//   float R0    = 100;   //R at T = 0C for PT100
//   // analogous formula for quadratic equation
//   float negb = -R0*a;
//   float bsqrd = ((R0*a)*(R0*a));
//   float fourac = 4*(R0*b*(R0-rt));
//   float twoc = 2*R0*b;
//   float sqroot = sqrt(bsqrd - fourac);   
//   float temp = (negb+sqroot)/twoc;
//   
//   if(rt == 0)
//   {
//       temp = 0;
//   }
//   CORETIMER_DelayUs(1);
//   return (temp);
//}
//
//
//float maxBoardRead()
//{
//   float Rref = 430.0;
//   uint16_t result = 0;
//   float Temperature_VoltRes = 0;
//   // uint16_t temp; 
//   float Resistance;
//   
//   //output_high(MAX31865_CS); //From CS high to cycle complete
//   CORETIMER_DelayMs(100); //Ensure chip has time to startup
//   
//
//    result = readRegister16(MAX31865_RTDMSB_REG, MAX31865_RTDLSB_REG); //Read MSB and LSB from RTD
//    if (result & 1)
//    {
//       //Error bit is set, Read the error status
//       result = readRegister8(MAX31865_FAULTSTAT_REG);
//       //**WANT TO MAYBE ADD DIAGNOSTICS HERE FOR FAULTS
//    }
//    else
//    {
//       result>>=1; //remove low bit
//       if(result == 0x7fff)
//       {
//           result = 0;
//       }
//       Resistance = ((float)result*Rref)/32768;  //32768 = 2^16
//       Temperature_VoltRes = CallendarVanDusen(Resistance);
//       //temp = (int16) Temperature_VoltRes;
//       CORETIMER_DelayMs(17); //minimum 16.5mSec between readings;
//    }
//    return Temperature_VoltRes;
//}