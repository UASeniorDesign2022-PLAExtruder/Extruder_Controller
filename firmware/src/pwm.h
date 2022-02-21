/*******************************************************************************
 * Extruder_Controller
 * pwm.h
 * Wilson Woods
 * 2.20.2022
 ******************************************************************************/

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

#define TMR2_RESCALE_256    0x8070
#define TMR2_PRESCALE_64     0x8060
#define TMR2_PRESCALE_32     0x8050
#define TMR2_PRESCALE_16     0x8040
#define TMR2_PRESCALE_8      0x8030
#define TMR2_PRESCALE_4      0x8020
#define TMR2_PRESCALE_2      0x8010
#define TMR2_PRESCALE_1      0x8000
    
void pwm_init(unsigned int period, unsigned int initial_duty_cycle, unsigned int t2conset);
void pwm_set_duty_cycle(unsigned int duty_cycle);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

