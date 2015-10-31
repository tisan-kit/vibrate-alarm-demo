/*
 * peri_led_br.h
 *
 *  Created on: 2015Äê10ÔÂ30ÈÕ
 *      Author: Administrator
 */

#ifndef APP_PERIPHERAL_PERI_LED_BR_H_
#define APP_PERIPHERAL_PERI_LED_BR_H_

#include "c_types.h"
#include "driver/pwm.h"
#include "peri_rgb_light.h"

#pragma pack(1)


void peri_led_br_init(struct PWM_APP_PARAM led_br_param,struct PWM_INIT led_br_init);
struct PWM_APP_PARAM peri_led_br_get(void);
void peri_led_br_set( struct PWM_APP_PARAM led_br_param);


#endif /* APP_PERIPHERAL_PERI_LED_BR_H_ */
