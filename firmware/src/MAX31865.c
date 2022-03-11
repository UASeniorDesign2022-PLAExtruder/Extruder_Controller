

//Need to set as ADC converting
//#device ADC=10
/*
//Window Target Memory View Config Bits
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOXINST
#FUSES NOIESO
*/
//check delay frequency
// #use delay(crystal=8000000)

//#use SPI(SPI1, baud=4000000, MODE=1, bits=32, STREAM=MAX)

#include "config/default/peripheral/coretimer/plib_coretimer.h"
#include "config/default/peripheral/gpio/plib_gpio.h"

#include "math.h"
#include "stdint.h"
#include "MAX31865.h"
#include "SPI.h"

//---------------------------------------
// Call this function to read a 16bit value from a register pair
uint16_t MAX31865_read16(int regno)
{
   uint16_t rtd;    //16bit variable for RTD value
   SS_TEMP_1_Clear();  
   //output_low(MAX31865_CS); //Active-Low Chip Select. Set CS low to enable the serial interface.
   CORETIMER_DelayUs(1);
   SPI_transfer(regno);   //Send the register number
   rtd = SPI_read_temp_1();   // Clock out 16 dummy bits and read the reply
   CORETIMER_DelayUs(1);
   SS_TEMP_1_Set();
   //output_high(MAX31865_CS);    //From CS high to cycle complete
   return(rtd);
}

// Call this function to read an 8bit value from a register
uint8_t MAX31865_read8(int regno)
{
   uint8_t rtd;     //8bit variable for RTD value

   SS_TEMP_1_Clear(); //Active-Low Chip Select. Set CS low to enable the serial interface.
   CORETIMER_DelayUs(1);
   SPI_transfer(regno);   //Send the register number
   rtd = SPI_read_temp_1();     // Clock out 8 dummy bits and read the reply
   CORETIMER_DelayUs(1);
   SS_TEMP_1_Set();   //From CS high to cycle complete
   return(rtd);
}

//Version for data register - Two 8-bit registers, RTD MSBs and RTD LSBs, contain the RTD resistance data and reads MSB first
#define MAX31865_read_data() MAX31865_read16(MAX31856_RTDMSB_REG_READ)

//---------------------------------------
// Call this function to write an 8-bit value to a MAX31865 register.
void MAX31865_write_register(uint8_t reg_address, uint8_t value)
{
   SS_TEMP_1_Clear();
   //output_low(MAX31865_CS); //Active-Low Chip Select. Set CS low to enable the serial interface.
   CORETIMER_DelayUs(1);
   SPI_transfer(reg_address); //Send the register number
   SPI_transfer(value);   //Send value in address
   CORETIMER_DelayUs(1);
   SS_TEMP_1_Set();
   //output_high(MAX31865_CS); //From CS high to cycle complete
}

//Resistance to degrees Celsius (Page 10) Callendar Van Dusen equation for 0<T<661C = Ro(1+aT+BT^2)
float CallendarVanDusen(float rt)
{
   float a     = 3.9083E-03;
   float b     = -5.7750E-07;
   //float c = -4.18301E-12 for -200C < T < 0C or 0 otherwise
   float R0    = 100;   //R at T = 0C for PT100
   // analogous formula for quadratic equation
   float negb = -R0*a;
   float bsqrd = ((R0*a)*(R0*a));
   float fourac = 4*(R0*b*(R0-rt));
   float twoc = 2*R0*b;
   float sqroot = sqrt(bsqrd - fourac);   
   float temp = (negb+sqroot)/twoc;
   
   if(rt == 0)
   {
       temp =0;
   }
   CORETIMER_DelayUs(1);
   return (temp);
}

void MAX31865_init(void)
{
    MAX31865_write_register(MAX31856_CONFIG_REG_WRITE,0xD3);//BIAS, AUTO, 3Wire 50Hz - no 1shot or fault detection cycle control
    CORETIMER_DelayMs(63); //minimum 62.5ms before a result can be read for 50Hz pg 13
}


float maxBoardRead()
{
   float Rref = 470.0;
   uint16_t result = 0;
   float Temperature_VoltRes = 0;
   // uint16_t temp; 
   float Resistance;
   
   SS_TEMP_1_OutputEnable();
   SS_TEMP_1_Set();
   //output_high(MAX31865_CS); //From CS high to cycle complete
   CORETIMER_DelayMs(100); //Ensure chip has time to startup
   MAX31865_init(); //Initialize Chip
   

    result = MAX31865_read_data(); //Read MSB and LSB from RTD
    if (result & 1)
    {
       //Error bit is set, Read the error status
       result = MAX31865_read8(MAX31856_FAULTSTATUS_REG_READ);
       //**WANT TO MAYBE ADD DIAGNOSTICS HERE FOR FAULTS
    }
    else
    {
       result>>=1; //remove low bit
       if(result == 0x7fff)
       {
           result = 0;
       }
       Resistance = ((float)result*Rref)/32768;  //32768 = 2^16
       Temperature_VoltRes = CallendarVanDusen(Resistance);
       //temp = (int16) Temperature_VoltRes;
       CORETIMER_DelayMs(17); //minimum 16.5mSec between readings;
    }
    return Temperature_VoltRes;
}
