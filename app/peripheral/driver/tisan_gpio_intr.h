/*
 * tisan_gpio_intr.h
 *
 *  Created on: 2015Äê10ÔÂ28ÈÕ
 *      Author: Administrator
 */

#ifndef APP_PERIPHERAL_DRIVER_TISAN_GPIO_INTR_H_
#define APP_PERIPHERAL_DRIVER_TISAN_GPIO_INTR_H_

#include "../peri_key.h"
#include "key.h"
#include "vibrate_key.h"


void gpio_intr_handler(struct keys_param *keys);

#endif /* APP_PERIPHERAL_DRIVER_TISAN_GPIO_INTR_H_ */
