///*******************************************************************************
// * Extruder_Controller
// * TempControl.c
// * Wilson Woods
// * 4.7.2022
// ******************************************************************************/
//
//#include <stdbool.h>
//#include <cstdint>
//#include "globals.h"
//#include "temp_control.h"
//
//
//#define DELAY_CHANGE_5      416U
//#define DELAY_CHANGE_10     831U
//#define DELAY_CHANGE_15     1247U
//
//void temp_control_turn_on(bool zone_1_active, bool zone_2_active, bool zone_3_active)
//{
//    if (zone_1_active)
//    {
//        current_delay_zone_1 = HEATER_CONTROL_BASE_DELAY + variable_delay_zone_1;
//        ZONE_1_ACTIVE = true;
//    }
//        
//    if (zone_2_active)
//    {
//        current_delay_zone_2 = HEATER_CONTROL_BASE_DELAY + variable_delay_zone_2;
//        ZONE_2_ACTIVE = true;
//    }
//        
//    if (zone_3_active)
//    {
//        current_delay_zone_3 = HEATER_CONTROL_BASE_DELAY + variable_delay_zone_3;
//        ZONE_3_ACTIVE = true;
//    }
//        
//    HEATER_CONTROL_ON = true;
//    
//}
//
//void temp_control_turn_off_all(void)
//{
//    current_delay_zone_1 = 0U;
//    current_delay_zone_2 = 0U;
//    current_delay_zone_3 = 0U;
//    HEATER_CONTROL_ON = false;
//}
//
//void temp_control_turn_off_zone(uint8_t current_zone)
//{
//    switch(current_zone)
//    {
//        case 1:
//            current_delay_zone_1 = 0U;
//            ZONE_1_ACTIVE = false;
//            break;
//        case 2:
//            current_delay_zone_2 = 0U;
//            ZONE_2_ACTIVE = false;
//            break;
//        case 3:
//            current_delay_zone_3 = 0U;
//            ZONE_3_ACTIVE = false;
//            break;
//        default:
//            break;
//            
//    }
//}
//
//uint16_t temp_control_compare_actual_to_target(uint8_t current_zone, short current_delay, uint16_t actual_temp, uint16_t target_temp)
//{
//    if ((actual_temp < (target_temp + 5)) && (actual_temp > (target_temp - 5)))
//    {
//        return current_delay;
//    }
//    
//    if (actual_temp >= target_temp + 20)
//    {
//        temp_control_turn_off_zone(current_zone);
//    }
//    else if (actual_temp >= target_temp + 15)
//    {
//        current_delay += DELAY_CHANGE_15;
//    }
//    else if (actual_temp >= target_temp + 10)
//    {
//        current_delay += DELAY_CHANGE_10;
//    }
//    else if (actual_temp >= target_temp + 5)
//    {
//        current_delay += DELAY_CHANGE_5;
//    }
//    else if (actual_temp <= target_temp - 20)
//    {
//        current_delay = HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_FULL_DUTY_CYCLE;
//    }
//    else if (actual_temp <= target_temp - 15)
//    {
//        current_delay -= DELAY_CHANGE_15;
//    }
//    else if (actual_temp <= target_temp - 10)
//    {
//        current_delay -= DELAY_CHANGE_10;
//    }
//    else if (actual_temp <= target_temp - 5)
//    {
//        current_delay -= DELAY_CHANGE_5;
//    }
//    
//    if (current_delay > (HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_FULL_DUTY_CYCLE))
//    {
//        current_delay = HEATER_CONTROL_BASE_DELAY + HEATER_CONTROL_FULL_DUTY_CYCLE;
//    }
//    
//    if (current_delay < HEATER_CONTROL_BASE_DELAY)
//    {
//        current_delay = HEATER_CONTROL_BASE_DELAY;
//    }
//    
//    return current_delay;
//}
//
//void temp_control_calculate_new_delays(void)
//{
//    current_delay_zone_1 = temp_control_compare_actual_to_target(1, current_delay_zone_1, actual_temp_1, target_temp_zone_1);
//    current_delay_zone_2 = temp_control_compare_actual_to_target(2, current_delay_zone_2, actual_temp_2, target_temp_zone_2);
//    current_delay_zone_3 = temp_control_compare_actual_to_target(3, current_delay_zone_3, actual_temp_3, target_temp_zone_3);          
//}
//
//void temp_control_compare_new_delay_lengths(void)
//{
//    uint16_t delay_order_array[3] = { current_delay_zone_1, current_delay_zone_2, current_delay_zone_3 };
//    uint16_t temp = 0;
//    if (delay_order_array[0] > delay_order_array[2])
//    {
//        temp = delay_order_array[0];
//        delay_order_array[0] = delay_order_array[2];
//        delay_order_array[2] = temp;
//    }
//    
//    if (delay_order_array[0] > delay_order_array[1])
//    {
//        temp = delay_order_array[0];
//        delay_order_array[0] = delay_order_array[1];
//        delay_order_array[1] = temp;
//    }
//    
//    if (delay_order_array[1] > delay_order_array[2])
//    {
//        temp = delay_order_array[1];
//        delay_order_array[1] = delay_order_array[2];
//        delay_order_array[2] = temp;
//    }
//    
//    first_delay = delay_order_array[0];
//    second_delay = delay_order_array[1];
//    third_delay = delay_order_array[2];
//}
//
//void temp_control_calculate_relative_delays(void)
//{
//    third_delay -= (first_delay + second_delay + (2 * pulse_delay));
//    second_delay -= (first_delay + pulse_delay);
//    
//    if (first_delay < HEATER_CONTROL_BASE_DELAY)
//    {
//        first_delay = HEATER_CONTROL_BASE_DELAY;
//    }
//    if (second_delay < HEATER_CONTROL_BASE_DELAY)
//    {
//        second_delay = HEATER_CONTROL_BASE_DELAY;
//    }
//    if (third_delay < HEATER_CONTROL_BASE_DELAY)
//    {
//        third_delay = HEATER_CONTROL_BASE_DELAY;
//    }
//}
//
//void temp_control_update_delays(void)
//{
//    temp_control_calculate_new_delays();
//    temp_control_compare_new_delay_lengths();
//    temp_control_calculate_relative_delays(); 
//}
