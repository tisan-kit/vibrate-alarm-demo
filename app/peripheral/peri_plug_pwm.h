/*
 * peri_plug_pwm.h
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Administrator
 */

#ifndef APP_PERIPHERAL_PERI_PLUG_PWM_H_
#define APP_PERIPHERAL_PERI_PLUG_PWM_H_

#include "c_types.h"
#include "driver/pwm.h"

struct PWM_APP_PARAM peri_plug_pwm_param_get(void);
void peri_plug_pwm_param_set( struct PWM_APP_PARAM plug_pwm_param);
void peri_plug_pwm_param_timer_set(void);
void peri_plug_pwm_start(uint16 speed);

#endif /* APP_PERIPHERAL_PERI_PLUG_PWM_H_ */
