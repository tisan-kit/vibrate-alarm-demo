/*
 * peri_vibrate.c
 *
 *  Created on: 2015Äê10ÔÂ28ÈÕ
 *      Author: Administrator
 */

#include "peri_vibrate.h"
#include "user_interface.h"
#include "eagle_soc.h"
#include "driver/key.h"
#include "mem.h"
#include "driver/tisan_gpio_intr.h"
#include "driver/tisan_vibrate.h"
#include "driver/key_base.h"

static uint32 vibrate_counter = 0;
static uint32 vibrate_low_counter = 0;


void ICACHE_FLASH_ATTR
peri_vibrate_counter_set(uint32 value_add)
{
	vibrate_counter += value_add;
	  PRINTF("\r\n set vibrate_counter \n");
}

uint32 ICACHE_FLASH_ATTR
peri_vibrate_counter_get(void)
{
	  PRINTF("\r\n get vibrate_counter:%u\n", vibrate_counter);
	  return vibrate_counter;
}


void ICACHE_FLASH_ATTR
peri_vibrate_counter_clear(void)
{

	  PRINTF("\r\n clear vibrate_counter \n");
}

void ICACHE_FLASH_ATTR
peri_vibrate_press(void)
{

	  PRINTF("\r\n peri_vibrate_press \n");
}

void ICACHE_FLASH_ATTR 
peri_vibrate_alarm(void)
{
	vibrate_counter = 0;

	PRINTF("\r\n peri_vibrate_alarm accur \r\n");
}



void ICACHE_FLASH_ATTR
peri_vibrate_init(uint8 gpio_id)
{
	struct base_key_param *single_key = (struct base_key_param *)os_zalloc(sizeof(struct base_key_param));

	single_key->gpio_id = gpio_id;
	single_key->gpio_name = tisan_get_gpio_name(gpio_id);
	single_key->gpio_func = tisan_get_gpio_general_func(gpio_id);
	single_key->level = 1;
	single_key->counter = 0;
	single_key->k_function1 = peri_vibrate_press;
	single_key->k_function2 = peri_vibrate_alarm;

	PRINTF("\r\n single_key->gpio_id:%d\r\n", single_key->gpio_id);
	key_single_init(single_key);

	alarm_init(13);
}

void ICACHE_FLASH_ATTR
peri_vibrate_init_NULL(void)
{
	peri_vibrate_init(VIBRATE_GPIO_ID);
}
