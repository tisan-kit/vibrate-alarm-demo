#ifndef __KEY_H__
#define __KEY_H__

#include "gpio.h"

typedef void (* key_function)(void);

struct single_key_param {
    uint8 key_level;
    uint8 gpio_id;
    uint8 gpio_func;
    uint32 gpio_name;
    os_timer_t key_5s;
    os_timer_t key_50ms;
    uint16 vbr_count;              //vibration times;
    key_function short_press;
    key_function long_press;
    key_function vbr_trigger;		//callback function when vibration triggered;
    key_function vbr_stop;			//callback function when vibration stops;
};

struct keys_param {
    uint8 key_num;
    struct single_key_param **single_key;
};

struct single_key_param *key_init_single(uint8 gpio_id,key_function long_press,
		key_function short_press,key_function vibrate_trigger, key_function vibrate_stop);
void key_init();
void key_5s_cb(struct single_key_param *single_key);
void key_50ms_cb(struct single_key_param *single_key);
#endif
