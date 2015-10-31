/*
 * tisan_vibrate.c
 *
 *  Created on: 2015Äê10ÔÂ29ÈÕ
 *      Author: Administrator
 */

#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "../../pando/pando_subdevice.h"
#include "tisan_vibrate.h"

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
uint32 g_vibrate_high_counter = 0;
uint32 g_vibrate_low_counter = 0;
uint32 vibrate_tm_high_counter = 0;
uint32 vibrate_tm_low_counter = 0;

void peri_vibrate_tim_start(uint16 delay_ms);

void ICACHE_FLASH_ATTR
peri_alarm_init(uint8 gpio_id)
{
	PIN_FUNC_SELECT(tisan_get_gpio_name(gpio_id), tisan_get_gpio_general_func(gpio_id));
	PIN_PULLUP_EN(tisan_get_gpio_name(gpio_id));
}

void ICACHE_FLASH_ATTR
vibrate_init(uint8 gpio_id)
{
    uint32 v_gpio_name=tisan_get_gpio_name(gpio_id);
    uint8 v_gpio_func=tisan_get_gpio_general_func(gpio_id);

    PIN_FUNC_SELECT(v_gpio_name, v_gpio_func);
    PIN_PULLUP_EN(v_gpio_name);
    gpio_output_set(0, 0, 0, GPIO_ID_PIN(gpio_id));   //set as input mode

	gpio_register_set(GPIO_PIN_ADDR(gpio_id), GPIO_PIN_INT_TYPE_SET(GPIO_PIN_INTR_DISABLE)
					  | GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_DISABLE)
					  | GPIO_PIN_SOURCE_SET(GPIO_AS_PIN_SOURCE));

	//clear interrupt status
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(gpio_id));

	//enable interrupt
	gpio_pin_intr_state_set(GPIO_ID_PIN(gpio_id), GPIO_PIN_INTR_NEGEDGE);

	peri_alarm_init(ALARM_GPIO_ID);
	peri_vibrate_tim_start(100);
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
	if(g_vibrate_low_counter)
	{
		PRINTF("vibrate_timer_cb,vibrate_low_counter:%d, tm_counter:%d\n", g_vibrate_low_counter, vibrate_tm_low_counter);

		if(!GPIO_INPUT_GET(GPIO_ID_PIN(VIBRATE_GPIO_ID)))
		{
			g_vibrate_low_counter++;
		}
		vibrate_tm_low_counter++;
	}
	if(vibrate_tm_low_counter >= COUNTER_TO_CLEAR_VIBRATE)
	{
		PRINTF("Clear counter low\n");
		g_vibrate_low_counter = 0;
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

	if((g_vibrate_low_counter >= COUNTER_TO_ALARM) || (g_vibrate_high_counter >= COUNTER_TO_ALARM))
	{
		PRINTF("COUNTER_TO_ALARM, vibrate_low_counter:%u, vibrate_high_counter:%u ", g_vibrate_low_counter, g_vibrate_high_counter);
		g_vibrate_low_counter = 0;
		g_vibrate_high_counter = 0;

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

