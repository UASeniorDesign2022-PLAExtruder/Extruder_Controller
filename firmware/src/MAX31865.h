/* 
 * File:   MAX31865.h
 * Author: wilson
 *
 * Created on March 10, 2022, 3:13 PM
 */

#ifndef MAX31865_H
#define	MAX31865_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <math.h>
#include "config/default/peripheral/coretimer/plib_coretimer.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
/***************************************************************************************/
// SPI chip select pin                          
//D1 is a read data byte 
// #define MAX31865_CS  **NEED SOMETHING HERE      //TQFN 13 SSOP Pin Description Table

    //Table 1 Page 13
#define MAX31856_CONFIG_REG_READ        0x00    //Configuration Read Address     
#define MAX31856_CONFIG_REG_WRITE       0x80    //Configuration Write Address
#define MAX31856_RTDMSB_REG_READ        0x01    //RTD MSBs Read 
#define MAX31856_RTDLSB_REG_READ        0x02    //RTD LSBs Read
#define MAX31856_HIFAULTMSB_REG_READ    0x03    //High Fault Threshold MSB Read
#define MAX31856_HIFAULTMSB_REG_WRITE   0x83    //High Fault Threshold MSB Write    
#define MAX31856_HIFAULTLSB_REG_READ    0x04    //High Fault Threshold LSB Read
#define MAX31856_HIFAULTLSB_REG_WRITE   0x84    //High Fault Threshold LSB Write
#define MAX31856_LOWFAULTMSB_REG_READ   0x05    //Low Fault Threshold MSB Read
#define MAX31856_LOWFAULTMSB_REG_WRITE  0x85    //Low Fault Threshold MSB Write
#define MAX31856_LOWFAULTLSB_REG_READ   0x06    //Low Fault Threshold LSB Read
#define MAX31856_LOWFAULTLSB_REG_WRITE  0x86    //Low Fault Threshold LSB Write
#define MAX31856_FAULTSTATUS_REG_READ   0x07    //Fault Status Read
    //Table 2 Page 13
#define MAX31856_CONFIG_VBIAS           0x80    //D7 VBias ON = 1 
#define MAX31856_CONFIG_CONVMODEAUTO    0x40    //D6 conv mode Auto = 1, Off = 0 
#define MAX31856_CONFIG_CONVMODEOFF     0x00    //D6 conv mode Auto = 1, Off = 0 
#define MAX31856_CONFIG_1SHOT           0x20    //D5 1 = 1-shot autoclear
#define MAX31856_CONFIG_3WIRE           0x10    //D4 3Wire = 1 
#define MAX31856_CONFIG_2OR4WIRE        0x00    //D4 2 or 4 wire  = 0 
#define MAX31856_CONFIG_FILTER50HZ      0x01    //D0 1 = 50Hz  
#define MAX31856_CONFIG_FILTER60HZ      0x00    //D0 0 = 60Hz
    //Table 3 Page 14 and Table 2 Page 13
#define MAX31856_CONFIG_FAULTSTAT       0x02    //D3 = 0 (No write action) D2 = 0 Fault Detection finished D1 = 1 Fault Status auto clear 
    //Table 7 Page 16
#define MAX31865_FAULT_HIGHTHRESH     0x80      //D7 = RTD High Threshold
#define MAX31865_FAULT_LOWTHRESH      0x40      //D6 = RTD Low Threshold
#define MAX31865_FAULT_REFINHIGH      0x20      //D5 = REFIN- > 0.85 X VBias
#define MAX31865_FAULT_REFINLOW       0x10      //D4 = REFIN- < 0.85 X VBias (FORCE = OPEN)
#define MAX31865_FAULT_RTDINLOW       0x08      //D3 = RTDIN- < 0.85 X VBias (FORCE = OPEN)
#define MAX31865_FAULT_VOLTAGE        0x04      //D2 = Over or Under Voltage
                                                //D1 and D0 Dont Matter
/***************************************************************************************/

uint8_t MAX31865_read8(int regno);
uint16_t MAX31865_read16(int regno);
uint16_t MAX31865_read_data();
void MAX31865_write_register(uint8_t reg_address, uint8_t value);
float CallendarVanDusen(float rt);
void MAX31865_init(void);
       
float maxBoardRead();


#ifdef	__cplusplus
}
#endif

#endif	/* MAX31865_H */

