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

/**
 * pwm_init()
 * 
 * @param period Value assigned to PR2, this is the period of TMR2
 * 
 * @param initial_duty_cycle Any value from 0 to PR2 + 1, actual duty cycle
 * (as a percentage) is given by ( intial_duty_cycle / period )
 * 
 * @param t2conset Value assigned to the T2CONSET register which is in turn
 * written to T2CON. This is the configuration for the timer. In this case,
 * we want 16-bit mode with the timer ON. USER TMR2_PRESCALE constants in
 * pwm.h. They provide different pre-scale values and leave other settings
 * unchanged.
 */
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

/**
 * pwm_set_duty_cycle()
 * 
 * @param duty_cycle A value between 0 and PR2 + 1 where PR2 will be the value
 * passed as period to pwm_init(). duty_cycle is passed to OC1RS (writable
 * output compare register) and will be updated on the next interrupt.
 * 
 * duty_cycle =  PR2 + 1      == 100% duty cycle,
 * duty_cycle = (PR2 + 1) / 2 ==  50% duty cycle,
 * etc. 
 */
void pwm_set_duty_cycle(unsigned int duty_cycle)
{
    OC1RS = duty_cycle;
}