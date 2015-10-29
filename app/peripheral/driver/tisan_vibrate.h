/*
 * tisan_vibrate.h
 *
 *  Created on: 2015Äê10ÔÂ29ÈÕ
 *      Author: Administrator
 */

#ifndef APP_PERIPHERAL_DRIVER_TISAN_VIBRATE_H_
#define APP_PERIPHERAL_DRIVER_TISAN_VIBRATE_H_

#define VIBRATE_GPIO_ID   12

extern uint32 g_vibrate_high_counter;
extern uint32 g_vibrate_low_counter;

void peri_vibrate_tim_start(uint16 delay_ms);

#endif /* APP_PERIPHERAL_DRIVER_TISAN_VIBRATE_H_ */
