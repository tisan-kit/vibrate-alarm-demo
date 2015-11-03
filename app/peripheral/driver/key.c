/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: key.c
 *
 * Description: key driver, now can use different gpio and install different function
 *
 * Modification history:
 *     2014/5/1, v1.0 create this file.
*******************************************************************************/
#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "../peri_key.h"
#include "../../pando/pando_subdevice.h"
#include "key.h"
#include "tisan_gpio_intr.h"




/******************************************************************************
 * FunctionName : key_init_single
 * Description  : init single key's gpio and register function
 * Parameters   : uint8 gpio_id - which gpio to use
 *                uint32 gpio_name - gpio mux name
 *                uint32 gpio_func - gpio function
 *                key_function long_press - long press function, needed to install
 *                key_function short_press - short press function, needed to install
 * Returns      : single_key_param - single key parameter, needed by key init
*******************************************************************************/
struct single_key_param *ICACHE_FLASH_ATTR
key_init_single(uint8 gpio_id,key_function long_press, key_function short_press,key_function vibrate_trigger, key_function vibrate_stop)
{
    struct single_key_param *single_key = (struct single_key_param *)os_zalloc(sizeof(struct single_key_param));


    single_key->gpio_id = gpio_id;
    single_key->gpio_name = tisan_get_gpio_name(gpio_id);
    single_key->gpio_func = tisan_get_gpio_general_func(gpio_id);
    single_key->long_press = long_press;
    single_key->short_press = short_press;
    single_key->vbr_trigger = vibrate_trigger;
    single_key->vbr_stop = vibrate_stop;


    return single_key;
}

/******************************************************************************
 * FunctionName : key_init
 * Description  : init keys
 * Parameters   : key_param *keys - keys parameter, which inited by key_init_single
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
key_init(struct keys_param *keys)
{
	PRINTF("enter into key_init\n");
    uint8 i;
    PRINTF("key address:%d\n", keys);
    ETS_GPIO_INTR_ATTACH(gpio_intr_handler, keys);

    ETS_GPIO_INTR_DISABLE();

    for (i = 0; i < keys->key_num; i++) {

        keys->single_key[i]->key_level = 1;

        PIN_FUNC_SELECT(tisan_get_gpio_name(keys->single_key[i]->gpio_id), tisan_get_gpio_general_func(keys->single_key[i]->gpio_id));
        PIN_PULLUP_EN(tisan_get_gpio_name(keys->single_key[i]->gpio_id));
        gpio_output_set(0, 0, 0, GPIO_ID_PIN(keys->single_key[i]->gpio_id));
        PRINTF("keys->num:%d\n",keys->key_num);
        gpio_register_set(GPIO_PIN_ADDR(keys->single_key[i]->gpio_id), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
                          | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
                          | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

        //clear gpio14 status
        GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(keys->single_key[i]->gpio_id));

        //enable interrupt
        gpio_pin_intr_state_set(GPIO_ID_PIN(keys->single_key[i]->gpio_id), GPIO_PIN_INTR_NEGEDGE);
    }

    ETS_GPIO_INTR_ENABLE();
}

/******************************************************************************
 * FunctionName : key_5s_cb
 * Description  : long press 5s timer callback
 * Parameters   : single_key_param *single_key - single key parameter
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
key_5s_cb(struct single_key_param *single_key)
{
	  // os_timer_disarm(&single_key->key_50ms);
	    os_timer_disarm(&single_key->key_5s);

    if (0 == GPIO_INPUT_GET(GPIO_ID_PIN(single_key->gpio_id))) {
        if (single_key->long_press) {
            single_key->long_press();
        }
    }
}

/******************************************************************************
 * FunctionName : key_50ms_cb
 * Description  : 50ms timer callback to check it's a real key push
 * Parameters   : single_key_param *single_key - single key parameter
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
key_50ms_cb(struct single_key_param *single_key)
{
    os_timer_disarm(&single_key->key_50ms);
    //os_timer_disarm(&single_key->key_5s);
    // high, then key is up
    if (1 == GPIO_INPUT_GET(GPIO_ID_PIN(single_key->gpio_id))) {
        os_timer_disarm(&single_key->key_5s);
        single_key->key_level = 1;
        gpio_pin_intr_state_set(GPIO_ID_PIN(single_key->gpio_id), GPIO_PIN_INTR_NEGEDGE);

        if (single_key->short_press) {
            single_key->short_press();
        }
    } else {
        gpio_pin_intr_state_set(GPIO_ID_PIN(single_key->gpio_id), GPIO_PIN_INTR_POSEDGE);
    }
}


