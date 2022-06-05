/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for BUTTON_1 pin ***/
#define BUTTON_1_Set()               (LATESET = (1<<5))
#define BUTTON_1_Clear()             (LATECLR = (1<<5))
#define BUTTON_1_Toggle()            (LATEINV= (1<<5))
#define BUTTON_1_OutputEnable()      (TRISECLR = (1<<5))
#define BUTTON_1_InputEnable()       (TRISESET = (1<<5))
#define BUTTON_1_Get()               ((PORTE >> 5) & 0x1)
#define BUTTON_1_PIN                  GPIO_PIN_RE5
#define BUTTON_1_InterruptEnable()   (CNENESET = (1<<5))
#define BUTTON_1_InterruptDisable()  (CNENECLR = (1<<5))

/*** Macros for HEATER_CONTROL_2 pin ***/
#define HEATER_CONTROL_2_Set()               (LATGSET = (1<<6))
#define HEATER_CONTROL_2_Clear()             (LATGCLR = (1<<6))
#define HEATER_CONTROL_2_Toggle()            (LATGINV= (1<<6))
#define HEATER_CONTROL_2_OutputEnable()      (TRISGCLR = (1<<6))
#define HEATER_CONTROL_2_InputEnable()       (TRISGSET = (1<<6))
#define HEATER_CONTROL_2_Get()               ((PORTG >> 6) & 0x1)
#define HEATER_CONTROL_2_PIN                  GPIO_PIN_RG6

/*** Macros for BUTTON_4 pin ***/
#define BUTTON_4_Set()               (LATGSET = (1<<7))
#define BUTTON_4_Clear()             (LATGCLR = (1<<7))
#define BUTTON_4_Toggle()            (LATGINV= (1<<7))
#define BUTTON_4_OutputEnable()      (TRISGCLR = (1<<7))
#define BUTTON_4_InputEnable()       (TRISGSET = (1<<7))
#define BUTTON_4_Get()               ((PORTG >> 7) & 0x1)
#define BUTTON_4_PIN                  GPIO_PIN_RG7
#define BUTTON_4_InterruptEnable()   (CNENGSET = (1<<7))
#define BUTTON_4_InterruptDisable()  (CNENGCLR = (1<<7))

/*** Macros for BUTTON_2 pin ***/
#define BUTTON_2_Set()               (LATGSET = (1<<8))
#define BUTTON_2_Clear()             (LATGCLR = (1<<8))
#define BUTTON_2_Toggle()            (LATGINV= (1<<8))
#define BUTTON_2_OutputEnable()      (TRISGCLR = (1<<8))
#define BUTTON_2_InputEnable()       (TRISGSET = (1<<8))
#define BUTTON_2_Get()               ((PORTG >> 8) & 0x1)
#define BUTTON_2_PIN                  GPIO_PIN_RG8
#define BUTTON_2_InterruptEnable()   (CNENGSET = (1<<8))
#define BUTTON_2_InterruptDisable()  (CNENGCLR = (1<<8))

/*** Macros for HEATER_CONTROL_1 pin ***/
#define HEATER_CONTROL_1_Set()               (LATGSET = (1<<9))
#define HEATER_CONTROL_1_Clear()             (LATGCLR = (1<<9))
#define HEATER_CONTROL_1_Toggle()            (LATGINV= (1<<9))
#define HEATER_CONTROL_1_OutputEnable()      (TRISGCLR = (1<<9))
#define HEATER_CONTROL_1_InputEnable()       (TRISGSET = (1<<9))
#define HEATER_CONTROL_1_Get()               ((PORTG >> 9) & 0x1)
#define HEATER_CONTROL_1_PIN                  GPIO_PIN_RG9

/*** Macros for HEATER_CONTROL_3 pin ***/
#define HEATER_CONTROL_3_Set()               (LATBSET = (1<<8))
#define HEATER_CONTROL_3_Clear()             (LATBCLR = (1<<8))
#define HEATER_CONTROL_3_Toggle()            (LATBINV= (1<<8))
#define HEATER_CONTROL_3_OutputEnable()      (TRISBCLR = (1<<8))
#define HEATER_CONTROL_3_InputEnable()       (TRISBSET = (1<<8))
#define HEATER_CONTROL_3_Get()               ((PORTB >> 8) & 0x1)
#define HEATER_CONTROL_3_PIN                  GPIO_PIN_RB8

/*** Macros for SP_TENSION_ADC pin ***/
#define SP_TENSION_ADC_Get()               ((PORTB >> 9) & 0x1)
#define SP_TENSION_ADC_PIN                  GPIO_PIN_RB9

/*** Macros for SP_TENSION_LED pin ***/
#define SP_TENSION_LED_Set()               (LATBSET = (1<<11))
#define SP_TENSION_LED_Clear()             (LATBCLR = (1<<11))
#define SP_TENSION_LED_Toggle()            (LATBINV= (1<<11))
#define SP_TENSION_LED_OutputEnable()      (TRISBCLR = (1<<11))
#define SP_TENSION_LED_InputEnable()       (TRISBSET = (1<<11))
#define SP_TENSION_LED_Get()               ((PORTB >> 11) & 0x1)
#define SP_TENSION_LED_PIN                  GPIO_PIN_RB11

/*** Macros for ICAP_1 pin ***/
#define ICAP_1_Get()               ((PORTB >> 14) & 0x1)
#define ICAP_1_PIN                  GPIO_PIN_RB14

/*** Macros for BUTTON_3 pin ***/
#define BUTTON_3_Set()               (LATBSET = (1<<15))
#define BUTTON_3_Clear()             (LATBCLR = (1<<15))
#define BUTTON_3_Toggle()            (LATBINV= (1<<15))
#define BUTTON_3_OutputEnable()      (TRISBCLR = (1<<15))
#define BUTTON_3_InputEnable()       (TRISBSET = (1<<15))
#define BUTTON_3_Get()               ((PORTB >> 15) & 0x1)
#define BUTTON_3_PIN                  GPIO_PIN_RB15
#define BUTTON_3_InterruptEnable()   (CNENBSET = (1<<15))
#define BUTTON_3_InterruptDisable()  (CNENBCLR = (1<<15))

/*** Macros for SDA1 pin ***/
#define SDA1_Get()               ((PORTD >> 9) & 0x1)
#define SDA1_PIN                  GPIO_PIN_RD9

/*** Macros for SCL1 pin ***/
#define SCL1_Get()               ((PORTD >> 10) & 0x1)
#define SCL1_PIN                  GPIO_PIN_RD10

/*** Macros for SCK pin ***/
#define SCK_Get()               ((PORTD >> 2) & 0x1)
#define SCK_PIN                  GPIO_PIN_RD2

/*** Macros for MISO pin ***/
#define MISO_Get()               ((PORTD >> 3) & 0x1)
#define MISO_PIN                  GPIO_PIN_RD3

/*** Macros for SS_TEMP_1 pin ***/
#define SS_TEMP_1_Set()               (LATDSET = (1<<4))
#define SS_TEMP_1_Clear()             (LATDCLR = (1<<4))
#define SS_TEMP_1_Toggle()            (LATDINV= (1<<4))
#define SS_TEMP_1_OutputEnable()      (TRISDCLR = (1<<4))
#define SS_TEMP_1_InputEnable()       (TRISDSET = (1<<4))
#define SS_TEMP_1_Get()               ((PORTD >> 4) & 0x1)
#define SS_TEMP_1_PIN                  GPIO_PIN_RD4

/*** Macros for MOSI pin ***/
#define MOSI_Get()               ((PORTD >> 5) & 0x1)
#define MOSI_PIN                  GPIO_PIN_RD5

/*** Macros for SS_TEMP_2 pin ***/
#define SS_TEMP_2_Set()               (LATFSET = (1<<0))
#define SS_TEMP_2_Clear()             (LATFCLR = (1<<0))
#define SS_TEMP_2_Toggle()            (LATFINV= (1<<0))
#define SS_TEMP_2_OutputEnable()      (TRISFCLR = (1<<0))
#define SS_TEMP_2_InputEnable()       (TRISFSET = (1<<0))
#define SS_TEMP_2_Get()               ((PORTF >> 0) & 0x1)
#define SS_TEMP_2_PIN                  GPIO_PIN_RF0

/*** Macros for SS_TEMP_3 pin ***/
#define SS_TEMP_3_Set()               (LATFSET = (1<<1))
#define SS_TEMP_3_Clear()             (LATFCLR = (1<<1))
#define SS_TEMP_3_Toggle()            (LATFINV= (1<<1))
#define SS_TEMP_3_OutputEnable()      (TRISFCLR = (1<<1))
#define SS_TEMP_3_InputEnable()       (TRISFSET = (1<<1))
#define SS_TEMP_3_Get()               ((PORTF >> 1) & 0x1)
#define SS_TEMP_3_PIN                  GPIO_PIN_RF1


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    GPIO_PORT_B = 0,
    GPIO_PORT_C = 1,
    GPIO_PORT_D = 2,
    GPIO_PORT_E = 3,
    GPIO_PORT_F = 4,
    GPIO_PORT_G = 5,
} GPIO_PORT;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    GPIO_PIN_RB0 = 0,
    GPIO_PIN_RB1 = 1,
    GPIO_PIN_RB2 = 2,
    GPIO_PIN_RB3 = 3,
    GPIO_PIN_RB4 = 4,
    GPIO_PIN_RB5 = 5,
    GPIO_PIN_RB6 = 6,
    GPIO_PIN_RB7 = 7,
    GPIO_PIN_RB8 = 8,
    GPIO_PIN_RB9 = 9,
    GPIO_PIN_RB10 = 10,
    GPIO_PIN_RB11 = 11,
    GPIO_PIN_RB12 = 12,
    GPIO_PIN_RB13 = 13,
    GPIO_PIN_RB14 = 14,
    GPIO_PIN_RB15 = 15,
    GPIO_PIN_RC12 = 28,
    GPIO_PIN_RC13 = 29,
    GPIO_PIN_RC14 = 30,
    GPIO_PIN_RC15 = 31,
    GPIO_PIN_RD0 = 32,
    GPIO_PIN_RD1 = 33,
    GPIO_PIN_RD2 = 34,
    GPIO_PIN_RD3 = 35,
    GPIO_PIN_RD4 = 36,
    GPIO_PIN_RD5 = 37,
    GPIO_PIN_RD6 = 38,
    GPIO_PIN_RD7 = 39,
    GPIO_PIN_RD8 = 40,
    GPIO_PIN_RD9 = 41,
    GPIO_PIN_RD10 = 42,
    GPIO_PIN_RD11 = 43,
    GPIO_PIN_RE0 = 48,
    GPIO_PIN_RE1 = 49,
    GPIO_PIN_RE2 = 50,
    GPIO_PIN_RE3 = 51,
    GPIO_PIN_RE4 = 52,
    GPIO_PIN_RE5 = 53,
    GPIO_PIN_RE6 = 54,
    GPIO_PIN_RE7 = 55,
    GPIO_PIN_RF0 = 64,
    GPIO_PIN_RF1 = 65,
    GPIO_PIN_RF3 = 67,
    GPIO_PIN_RF4 = 68,
    GPIO_PIN_RF5 = 69,
    GPIO_PIN_RG6 = 86,
    GPIO_PIN_RG7 = 87,
    GPIO_PIN_RG8 = 88,
    GPIO_PIN_RG9 = 89,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;

typedef  void (*GPIO_PIN_CALLBACK) ( GPIO_PIN pin, uintptr_t context);

void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptDisable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: Local Data types and Prototypes
// *****************************************************************************
// *****************************************************************************

typedef struct {

    /* target pin */
    GPIO_PIN                 pin;

    /* Callback for event on target pin*/
    GPIO_PIN_CALLBACK        callback;

    /* Callback Context */
    uintptr_t               context;

} GPIO_PIN_CALLBACK_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xF), (uint32_t)(value) << (pin & 0xF));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xF)) & 0x1);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xF)) & 0x1);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInterruptEnable(GPIO_PIN pin)
{
    GPIO_PortInterruptEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInterruptDisable(GPIO_PIN pin)
{
    GPIO_PortInterruptDisable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

bool GPIO_PinInterruptCallbackRegister(
    GPIO_PIN pin,
    const   GPIO_PIN_CALLBACK callBack,
    uintptr_t context
);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
