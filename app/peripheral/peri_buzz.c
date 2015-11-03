/*******************************************************
 * File name: peri_motor.c
 * Author:
 * Versions:1.0
 * Description:This module is driver of the subdevice: motor.
 * History:
 *   1.Date:
 *     Author:
 *     Modification:    
 *********************************************************/

#include "peri_buzz.h"
#include "peri_rgb_light.h"

#include "user_interface.h"
#include "driver/pwm.h"
#include "c_types.h"
#include "../pando/pando_object.h"


/******************************************************************************
 * FunctionName : peri_buzz_set.
 * Description  : set the parameter of buzz pwm.
 * Parameters   : struct PWM_APP_PARAM motor_param.
 * Returns      : none
*******************************************************************************/

void ICACHE_FLASH_ATTR 
peri_buzz_set( struct PWM_APP_PARAM buzz_param)
{
    pwm_set_freq(buzz_param.pwm_freq);
    pwm_set_duty(buzz_param.pwm_duty[0], 0);
    
    pwm_start();
    
}

/******************************************************************************
 * FunctionName : peri_buzz_init
 * Description  : buzzer initialize , mainly initialize pwm mode
 * Parameters   : struct PWM_APP_PARAM buzz_param,struct PWM_INIT buzz_init
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_buzz_init(struct PWM_APP_PARAM buzz_param,struct PWM_INIT buzz_init)
{
    PRINTF("I am a buzzer\n");
    PRINTF("pwm_freq: %d, pwm_duty1: %d, pwm_duty2: %d\n",buzz_param.pwm_freq,
        (buzz_param.pwm_duty)[0]);
    PRINTF("start flash write \n");

    PRINTF("finished \n");
    pwm_init(buzz_param,buzz_init);
    pwm_start();
    
}



