/*
 * peri_led_br.c
 *
 *  Created on: 2015Äê10ÔÂ30ÈÕ
 *      Author: Administrator
 */

#include "peri_led_br.h"

#include "user_interface.h"
#include "driver/pwm.h"
#include "c_types.h"
#include "spi_flash.h"
#include "../pando/pando_object.h"

extern uint8_t FLASH_SEC_NUM ;

#define MOTOR_FLASH_PRIV_SAVE     FLASH_SEC_NUM

/******************************************************************************
 * FunctionName : peri_motor_get.
 * Description  : get the parameter of the motor status.
 * Parameters   : none
 * Returns      : the parameter of the motor status.
*******************************************************************************/

struct PWM_APP_PARAM ICACHE_FLASH_ATTR
peri_led_br_get(void)
{
    struct PWM_APP_PARAM ret;

    spi_flash_read((PRIV_PARAM_START_SEC + MOTOR_FLASH_PRIV_SAVE) * SPI_FLASH_SEC_SIZE,
    	(uint32 *)&ret, sizeof(struct PWM_APP_PARAM));
    return ret;
}


/******************************************************************************
 * FunctionName : peri_motor_set.
 * Description  : set the parameter of motor status.
 * Parameters   : struct LIGHT_PARAM motor_param.
 * Returns      : none
*******************************************************************************/

void ICACHE_FLASH_ATTR
peri_led_br_set( struct PWM_APP_PARAM led_br_param)
{
    pwm_set_freq(led_br_param.pwm_freq);
    pwm_set_duty(led_br_param.pwm_duty[0], 0);

    pwm_start();

    spi_flash_erase_sector(PRIV_PARAM_START_SEC + MOTOR_FLASH_PRIV_SAVE);
	spi_flash_write((PRIV_PARAM_START_SEC + MOTOR_FLASH_PRIV_SAVE) * SPI_FLASH_SEC_SIZE,
	    (uint32 *)&led_br_param, sizeof(struct PWM_APP_PARAM));
}

/******************************************************************************
 * FunctionName : peri_motor_init
 * Description  : motor initialize , mainly initialize pwm mode
 * Parameters   : struct LIGHT_PARAM motor_param,struct LIGHT_INIT motor_init
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_led_br_init(struct PWM_APP_PARAM led_br_param,struct PWM_INIT led_br_init)
{
    PRINTF("I am the electronic-motor\n");
    PRINTF("pwm_freq: %d, pwm_duty1: %d, pwm_duty2: %d\n",led_br_param.pwm_freq,
        (led_br_param.pwm_duty)[0], (led_br_param.pwm_duty)[1]);
    PRINTF("start flash write \n");

    spi_flash_write((PRIV_PARAM_START_SEC + MOTOR_FLASH_PRIV_SAVE) * SPI_FLASH_SEC_SIZE,
    	    (uint32 *)&led_br_param, sizeof(struct PWM_APP_PARAM));

    PRINTF("finished \n");
    pwm_init(led_br_param, led_br_init);
    pwm_start();

}


