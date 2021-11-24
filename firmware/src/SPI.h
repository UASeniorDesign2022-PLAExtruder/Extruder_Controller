/*******************************************************************************
 * Extruder_Controller
 * SPI.h
 * Wilson Woods
 * 11.18.2021
 ******************************************************************************/

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/**
 * SPIInit()
 * Initialize the PIC32MX470 SPI1 module 
 */
void SPI_Init(void);

/**
 * SPI_Transfer()
 * Send dummy byte to initiate SPI1 module master read
 */
void SPI_Transfer(char data);

/**
 * SPI_Read_Temp1()
 * Read 2 bytes from SPI slave device connected at SS 1
 */
int SPI_Read_Temp_1(void);

/**
 * SPI_Read_Temp2()
 * Read 2 bytes from SPI slave device connected at SS 2
 */
int SPI_Read_Temp_2(void);

/**
 * SPI_Read_Temp3()
 * Read 2 bytes from SPI slave device connected at SS 3
 */
int SPI_Read_Temp_3(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

