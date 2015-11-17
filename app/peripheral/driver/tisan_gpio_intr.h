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
#include "tisan_vibrate.h"

//void gpio_intr_handler(void *arg);
void gpio_intr_handler(struct base_key_param **keys_param);

#endif /* APP_PERIPHERAL_DRIVER_TISAN_GPIO_INTR_H_ */
