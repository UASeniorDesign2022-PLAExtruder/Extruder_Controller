/* 
 * File:   TempControl.h
 * Author: wilson
 *
 * Created on April 18, 2022, 8:00 AM
 */

#ifndef TEMPCONTROL_H
#define	TEMPCONTROL_H

#include <cstdint>
#include <stdbool.h>

void temp_control_turn_on(bool zone_1_active, bool zone_2_active, bool zone_3_active);
void temp_control_turn_off_all(void);
void temp_control_turn_off_zone(uint8_t current_zone);
uint16_t temp_control_compare_actual_to_target(uint8_t current_zone, short current_delay, uint16_t actual_temp, uint16_t target_temp);
void temp_control_calculate_new_delays(void);
void temp_control_compare_new_delay_lengths(void);
void temp_control_calculate_relative_delays(void);
void temp_control_update_delays(void);

#endif	/* TEMPCONTROL_H */

