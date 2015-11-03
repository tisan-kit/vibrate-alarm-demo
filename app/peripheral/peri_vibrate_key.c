/*******************************************************
 * File name: peri_key.c
 * Author: Chongguang Li
 * Versions:1.0
 * Description:This is the key function module.

 * History:
 *   1.Date:
 *     Author:
 *     Modification:
 *********************************************************/
#include "peri_vibrate_key.h"
#include "user_interface.h"
#include "eagle_soc.h"
#include "driver/key.h"
#include "os_type.h"
#include "mem.h"
#include "driver/tisan_gpio_intr.h"
#include "driver/vibrate_key.h"
#include "driver/pwm.h"
#include "peri_buzz.h"


/******************************************************************************
 * FunctionName : user_plug_short_press
 * Description  : key's short press function, needed to be installed
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
peri_vibrate_key_trigger(void)
{
	  PRINTF("peri_vibrate_key_trigger\n");

	  struct PWM_APP_PARAM buzz_set_param;
	  buzz_set_param.pwm_freq = 25000;
	  buzz_set_param.pwm_duty[0] = 200;
	  peri_buzz_set(buzz_set_param);
}

/******************************************************************************
 * FunctionName : user_plug_long_press
 * Description  : key's long press function, needed to be installed, preserved function.
 * Parameters   : none
 * Returns      : none
*******************************************************************************/

void ICACHE_FLASH_ATTR
peri_vibrate_key_stop(void)
{
	  PRINTF("peri_vibrate_key_stop\n");
}

