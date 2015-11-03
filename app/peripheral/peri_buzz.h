/*******************************************************
 * File name: peri_motor.h
 * Author:
 * Versions:1.0
 * Description:This module is driver of the subdevice: motor.
 * History:
 *   1.Date:
 *     Author:
 *     Modification:    
 *********************************************************/
 
#ifndef __PERI_BUZZ_H__
#define __PERI_BUZZ_H__

#include "c_types.h"
#include "driver/pwm.h"
#include "peri_rgb_light.h"

#pragma pack(1)


void peri_buzz_init(struct PWM_APP_PARAM buzz_param,struct PWM_INIT buzz_init);

void peri_buzz_set( struct PWM_APP_PARAM buzz_param);


#endif
