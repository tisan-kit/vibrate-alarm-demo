/*******************************************************
 * File name: peri_vibrate_key.h
 * Author: Chongguang Li
 * Versions:1.0
 * Description:This module is the key function module.

 * History:
 *   1.Date:
 *     Author:
 *     Modification:
 *********************************************************/

#ifndef PERI_VIBRATE_KEY_H_
#define PERI_VIBRATE_KEY_H_


#include "gpio.h"
#include "../../include/os_type.h"
#include "peri_key.h"



struct vibrate_key_param {
    uint8  gpio_id;
    uint16 vibration_times;
    os_timer_t key_1s;
    key_function vibration_triger;
    key_function vibration_stop;
};


void peri_vibrate_key_trigger(void);
void peri_vibrate_key_stop(void);
void peri_vibrate_key_init(uint8 gpio_id,key_function vibrate_trigger, key_function vibrate_stop);


#endif
