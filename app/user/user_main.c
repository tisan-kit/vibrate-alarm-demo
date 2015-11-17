/******************************************************************************
 /*******************************************************
 * File name: user_main.c
 * Author:Chongguang Li
 * Versions:1.0
 * Description:entry file of user application.
 *
 * History:
 *   1.Date:
 *     Author:
 *     Modification:
*******************************************************************************/

#include "../peripheral/driver/uart.h"
#include "objects/plug.h"
#include "objects/led.h"
#include "os_type.h"
#include "user_interface.h"
#include "../peripheral/peri_key.h"
#include "wifi_config.h"
#include "../pando/pando_framework.h"
#include "objects/plug_pwm.h"
#include "objects/temperature.h"
#include "objects/led_br.h"
#include "../peripheral/driver/key_base.h"

static void ICACHE_FLASH_ATTR
user_key_long_press_cb()
{
	wifi_config(NULL);
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
	uart_init(115200, 115200); // serial bound rate:11520.

	//long press gpio4, enter into wifi config mode.
	//peri_single_key_init(4, user_key_long_press_cb, NULL);

	peri_config_key_init(4);
	peri_vibrate_init(12);

	base_keys_init();


	pando_framework_init();
}

