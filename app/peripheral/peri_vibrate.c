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


void ICACHE_FLASH_ATTR
peri_vibrate_init(uint8 gpio_id)
{

    ETS_GPIO_INTR_ATTACH(gpio_intr_handler, NULL);
    //ETS_GPIO_INTR_DISABLE();
    vibrate_init(gpio_id);

    //ETS_GPIO_INTR_ENABLE();
}

void ICACHE_FLASH_ATTR
peri_vibrate_init_NULL(void)
{
	peri_vibrate_init(VIBRATE_GPIO_ID);
}
