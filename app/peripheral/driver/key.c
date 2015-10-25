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

#define LONG_PRESS_COUNT 3000

#define VIBRATE_GPIO_ID   12
#define COUNTER_TO_CLEAR_VIBRATE     20
#define COUNTER_TO_ALARM     10

#define ALARM_GPIO_ID     13
#define COUNTER_TO_CLEAR_ALARM      20
#define ALARM_TIME_HIGH     20
#define ALARM_TIME_LOW      10

static os_timer_t alarm_timer;
uint8 alarm_state = 0;
uint16 alarm_counter = 0;

static os_timer_t  vibrate_timer;
uint8 vibrate_gpio_id = 0;
uint32 vibrate_high_counter = 0;
uint32 vibrate_low_counter = 0;
uint32 vibrate_tm_high_counter = 0;
uint32 vibrate_tm_low_counter = 0;

void ICACHE_FLASH_ATTR
peri_alarm_init(uint8 gpio_id)
{
	PIN_FUNC_SELECT(tisan_get_gpio_name(gpio_id), tisan_get_gpio_general_func(gpio_id));
	PIN_PULLUP_EN(tisan_get_gpio_name(gpio_id));
}

/******************************************************************************
 * FunctionName : key_init
 * Description  : init keys
 * Parameters   : key_param *keys - keys parameter, which inited by key_init_single
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
key_init(uint32 gpio_name,uint8 gpio_id,uint8 gpio_func)
{
    uint32 v_gpio_name=tisan_get_gpio_name(VIBRATE_GPIO_ID);
    uint8 v_gpio_func=tisan_get_gpio_general_func(VIBRATE_GPIO_ID);

    PIN_FUNC_SELECT(v_gpio_name, v_gpio_func);
    PIN_PULLUP_EN(v_gpio_name);
    gpio_output_set(0, 0, 0, GPIO_ID_PIN(VIBRATE_GPIO_ID));   //set as input mode

	PIN_FUNC_SELECT(gpio_name,gpio_func);
	PIN_PULLUP_EN(gpio_name);
	gpio_output_set(0, 0, 0, GPIO_ID_PIN(gpio_id));

	gpio_register_set(GPIO_PIN_ADDR(gpio_id), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
					  | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
					  | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));
	gpio_register_set(GPIO_PIN_ADDR(VIBRATE_GPIO_ID), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
						  | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
						  | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

	//clear interrupt status
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(gpio_id));
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(VIBRATE_GPIO_ID));

	//enable interrupt
	gpio_pin_intr_state_set(GPIO_ID_PIN(gpio_id), GPIO_PIN_INTR_NEGEDGE);
	gpio_pin_intr_state_set(GPIO_ID_PIN(VIBRATE_GPIO_ID), GPIO_PIN_INTR_NEGEDGE);

	peri_alarm_init(ALARM_GPIO_ID);
	peri_vibrate_tim_start(100);
}
/******************************************************************************
 * FunctionName : key_5s_cb
 * Description  : long press 5s timer callback
 * Parameters   : single_key_param *single_key - single key parameter
 * Returns      : none
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
key_5s_cb(struct key_param *single_key)
{
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
LOCAL void ICACHE_FLASH_ATTR
key_50ms_cb(struct key_param *single_key)
{
    os_timer_disarm(&single_key->key_50ms);

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



static void ICACHE_FLASH_ATTR
alarm_timer_cb(void* arg)
{
	if(alarm_counter)
	{
		PRINTF("alarm time, state:%u, alarm_counter:%u\n", alarm_state, alarm_counter);

		GPIO_OUTPUT_SET(ALARM_GPIO_ID, alarm_state);
		if(alarm_state)
		{
			alarm_state = 0;
		}
		else
		{
			alarm_state = 1;
		}
		alarm_counter++;
	}

	if(alarm_counter >= COUNTER_TO_CLEAR_ALARM)
	{
		PRINTF("alarm time end\n");
		alarm_counter = 0;
		alarm_state = 0;
		os_timer_disarm(&alarm_timer);
	}
}

void ICACHE_FLASH_ATTR
peri_alarm_tim_start(uint16 delay_ms)
{
	alarm_counter = 1;
	report_event_alarm();    //report event

	os_timer_disarm(&alarm_timer);
	os_timer_setfn(&alarm_timer, (os_timer_func_t *)alarm_timer_cb, NULL);
	os_timer_arm(&alarm_timer, delay_ms, 1);
}

static void ICACHE_FLASH_ATTR
vibrate_timer_cb(void* arg)
{
	if(vibrate_low_counter)
	{
		PRINTF("vibrate_timer_cb,vibrate_low_counter:%d, tm_counter:%d\n", vibrate_low_counter,vibrate_tm_low_counter);

		if(!GPIO_INPUT_GET(GPIO_ID_PIN(VIBRATE_GPIO_ID)))
		{
			vibrate_low_counter++;
		}
		vibrate_tm_low_counter++;
	}
	if(vibrate_tm_low_counter >= COUNTER_TO_CLEAR_VIBRATE)
	{
		PRINTF("Clear counter low\n");
		vibrate_low_counter = 0;
		vibrate_tm_low_counter = 0;
	}

//	if(vibrate_high_counter)
//	{
//		PRINTF("vibrate_timer_cb,vibrate_high_counter:%d, tm_counter:%d\n", vibrate_high_counter, vibrate_tm_high_counter);
//		vibrate_tm_high_counter++;
//	}
//	if(vibrate_tm_high_counter >= COUNTER_TO_CLEAR)
//	{
//		PRINTF("Clear counter high\n");
//		vibrate_high_counter = 0;
//		vibrate_tm_high_counter = 0;
//	}

	if((vibrate_low_counter >= COUNTER_TO_ALARM) || (vibrate_high_counter >= COUNTER_TO_ALARM))
	{
		PRINTF("COUNTER_TO_ALARM, vibrate_low_counter:%u, vibrate_high_counter:%u ", vibrate_low_counter, vibrate_high_counter);
		vibrate_low_counter = 0;
		vibrate_high_counter = 0;

		vibrate_tm_low_counter = 0;
		vibrate_tm_high_counter = 0;

		peri_alarm_tim_start(200);
	}
}

void ICACHE_FLASH_ATTR
peri_vibrate_tim_start(uint16 delay_ms)
{
	os_timer_disarm(&vibrate_timer);
	os_timer_setfn(&vibrate_timer, (os_timer_func_t *)vibrate_timer_cb, NULL);
	os_timer_arm(&vibrate_timer, delay_ms, 1);
}

/******************************************************************************
 * FunctionName : key_intr_handler
 * Description  : key interrupt handler
 * Parameters   : key_param *keys - keys parameter, which inited by key_init_single
 * Returns      : none
*******************************************************************************/
void key_intr_handler(struct key_param *key)
{
    uint8 i;
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

    PRINTF("Get into vibrate_intr_handler, gpio_status:%u\n", gpio_status);

	if (gpio_status & BIT(key->gpio_id))
	{
		//disable interrupt
		gpio_pin_intr_state_set(GPIO_ID_PIN(key->gpio_id), GPIO_PIN_INTR_DISABLE);
		//clear interrupt status
		GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(key->gpio_id));

		if (key->key_level == 1)
		{
			// 5s, restart & enter softap mode
			os_timer_disarm(&key->key_5s);
			os_timer_setfn(&key->key_5s, (os_timer_func_t *)key_5s_cb, key);
			os_timer_arm(&key->key_5s, LONG_PRESS_COUNT, 0);
			key->key_level = 0;
			gpio_pin_intr_state_set(GPIO_ID_PIN(key->gpio_id), GPIO_PIN_INTR_POSEDGE);
		}
		else
		{
			// 50ms, check if this is a real key up
			os_timer_disarm(&key->key_50ms);
			os_timer_setfn(&key->key_50ms, (os_timer_func_t *)key_50ms_cb, key);
			os_timer_arm(&key->key_50ms, 50, 0);
		}
	}

    if (gpio_status & BIT(VIBRATE_GPIO_ID))  //judge whether interrupt source is gpio12
    {
    	//clear interrupt status
    	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(VIBRATE_GPIO_ID));

		if(GPIO_INPUT_GET(GPIO_ID_PIN(VIBRATE_GPIO_ID)))  //get gpio pin level  (high/low)
		{
			vibrate_high_counter++;
			PRINTF("intr vibrate_high_counter:%d\n", vibrate_high_counter);
		}
		else
		{
			vibrate_low_counter++;
			PRINTF("intr vibrate_low_counter:%d\n", vibrate_low_counter);
		}
    }
}

