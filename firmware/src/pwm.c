/*******************************************************************************
 * Extruder_Controller
 * pwm.c
 * Wilson Woods
 * 2.20.2022
 ******************************************************************************/

#include "definitions.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/tmr/plib_tmr2.h"

#include "pwm.h"

void pwm_init(unsigned int period, unsigned int initial_duty_cycle, unsigned int t2conset)
{
    OC1CON = 0x0000;                // turn off OC1 module to configure
    T2CONCLR = 0x00008000;          // turn off TMR2 module to configure
    
    OC1R   = initial_duty_cycle;    // initialize read-only compare register
    OC1RS  = initial_duty_cycle;    // initialize writable compare register
    OC1CON = 0x0006;                // configure for PWM mode without fault pin
    
    TMR2 = 0x0;                     // clear TMR2 counter
    PR2  = period;                  // set period
    IFS0CLR = 0x00000100;           // clear the T2 interrupt flag
    IEC0SET = 0x00000100;           // enable T2 interrupt
    IPC2SET = 0x0000001C;           // set T2 interrupt priority to 7
    
    // enable TMR2
    T2CONSET  = t2conset;           // pick TMR2_PRESCALE values from pwm.h        
                                    
    OC1CONSET = 0x8000;             // enable OC1
    
    // maybe unnecessary but no harm in having them
    PWM_OUT_1_OutputEnable();       // direction control line 1
    PWM_OUT_2_OutputEnable();       // direction control line 2
}

void pwm_set_duty_cycle(unsigned int duty_cycle)
{
    OC1RS = duty_cycle;
}