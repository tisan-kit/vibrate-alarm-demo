/*
 * peri_vibrate_tim.c
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Administrator
 */

#include "peri_vibrate_tim.h"
#include "user_interface.h"
#include "gpio.h"

static os_timer_t  vibrate_timer;

static uint8_t get_gpio_level(uint8_t gpio_no)
{
	uint8_t level;


	return level;
}


static void ICACHE_FLASH_ATTR
vibrate_timer_cb(void* arg)
{

}

void ICACHE_FLASH_ATTR
peri_vibrate_tim_start(uint16 delay_ms)
{
	os_timer_disarm(&vibrate_timer);
	os_timer_setfn(&vibrate_timer, (os_timer_func_t *)vibrate_timer_cb, NULL);
	os_timer_arm(&vibrate_timer, delay_ms, 1);
}

void ICACHE_FLASH_ATTR
peri_vibrate_timer_set(void)
{
	os_timer_disarm(&vibrate_timer);

}



