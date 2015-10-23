/*
 * peri_plug_pwm.c
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Administrator
 */

#include "user_interface.h"
#include "driver/pwm.h"
#include "c_types.h"
#include "spi_flash.h"
#include "peri_plug_pwm.h"

static os_timer_t  plug_pwm_timer;

struct PWM_APP_PARAM plug_pwm_s_param={25000,0,0};


struct PWM_APP_PARAM ICACHE_FLASH_ATTR
peri_plug_pwm_param_get(void)
{
    struct PWM_APP_PARAM ret;

//    spi_flash_read((PRIV_PARAM_START_SEC + PRIV_PARAM_SAVE) * SPI_FLASH_SEC_SIZE,
//    	(uint32 *)&ret, sizeof(struct PWM_APP_PARAM));
    return ret;
}


/******************************************************************************
 * FunctionName : peri_rgb_light_param_set.
 * Description  : set the parameter of the RGB light.
 * Parameters   : light_param-- RGB light parameter.
 * Returns      : none
*******************************************************************************/

void ICACHE_FLASH_ATTR
peri_plug_pwm_param_set( struct PWM_APP_PARAM light_param)
{
    pwm_set_freq(plug_pwm_s_param.pwm_freq);
    pwm_set_duty(plug_pwm_s_param.pwm_duty[0], 0); // single pwm.

    pwm_start();

//  spi_flash_erase_sector(PRIV_PARAM_START_SEC + PRIV_PARAM_SAVE);
//	spi_flash_write((PRIV_PARAM_START_SEC + PRIV_PARAM_SAVE) * SPI_FLASH_SEC_SIZE,
//	    (uint32 *)&light_param, sizeof(struct PWM_APP_PARAM));
}

#define PLUG_PWM_STEP  5
static void ICACHE_FLASH_ATTR
plug_pwm_timer_cb(void* arg)
{
	static uint8 state = 0;
	uint8 step = 5;
	const uint8 max_value_ = 240;
	const uint8 min_value_ = 50;

	if(state == 1)  //increase
	{
		if(plug_pwm_s_param.pwm_duty[0] < max_value_)
		{
			plug_pwm_s_param.pwm_duty[0] += PLUG_PWM_STEP;
		}
		else
		{
			plug_pwm_s_param.pwm_duty[0] = max_value_;
			state = 0;
		}
	}
	else
	{
		if(plug_pwm_s_param.pwm_duty[0] > min_value_)
		{
			plug_pwm_s_param.pwm_duty[0] -= PLUG_PWM_STEP;
		}
		else
		{
			state = 1;
			plug_pwm_s_param.pwm_duty[0] = min_value_;
		}
	}
	peri_plug_pwm_param_set(plug_pwm_s_param);
}

void ICACHE_FLASH_ATTR
peri_plug_pwm_start(uint16 speed)
{
	os_timer_disarm(&plug_pwm_timer);
	os_timer_setfn(&plug_pwm_timer, (os_timer_func_t *)plug_pwm_timer_cb, NULL);
	os_timer_arm(&plug_pwm_timer, speed, 1);
}

void ICACHE_FLASH_ATTR
peri_plug_pwm_set(bool state)
{
	os_timer_disarm(&plug_pwm_timer);
}

/******************************************************************************
 * FunctionName : peri_rgb_light_param_timer_set.
 * Description  : set the parameter of the RGB light by timer.
 * Parameters   : none.
 * Returns      : none.
*******************************************************************************/

void ICACHE_FLASH_ATTR
peri_plug_pwm_param_timer_set(void)
{
	os_timer_disarm(&plug_pwm_timer);
	plug_pwm_s_param.pwm_duty[0] = 255;
	peri_plug_pwm_param_set(plug_pwm_s_param);
}
/******************************************************************************
 * FunctionName : peri_rgb_light_init
 * Description  : light demo initialize, mainly initialize pwm mode
 * Parameters   : struct PWM_APP_PARAM light_param,struct PWM_INIT light_init
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_plug_pwm_init(struct PWM_APP_PARAM plug_pwm_param,struct PWM_INIT plug_pwm_init)
{

    PRINTF("I am the plug_pwm\n");
//    PRINTF("pwm_freq: %d, pwm_duty: %d\n", plug_pwm_s_param.pwm_freq,
//        (plug_pwm_s_param.pwm_duty)[0]);

//    spi_flash_write((PRIV_PARAM_START_SEC + PRIV_PARAM_SAVE) * SPI_FLASH_SEC_SIZE,
//    	    (uint32 *)&light_param, sizeof(struct PWM_APP_PARAM));

    pwm_init(plug_pwm_param,plug_pwm_init);
    pwm_start();

}
