/*
 * peri_vibrate_tim.h
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef APP_PERIPHERAL_PERI_VIBRATE_TIM_H_
#define APP_PERIPHERAL_PERI_VIBRATE_TIM_H_

#include "c_types.h"

#define GPIO_AMOUNT_MAX 5


struct GPIO_INIT
{
	uint8 io_num;               				// io number
	uint8 io_id[GPIO_AMOUNT_MAX];	            //list io id
};

#endif /* APP_PERIPHERAL_PERI_VIBRATE_TIM_H_ */
