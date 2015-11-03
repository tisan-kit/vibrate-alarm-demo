/*******************************************************
 * File name: peri_key.h
 * Author: Chongguang Li
 * Versions:1.0
 * Description:This module is the key function module.

 * History:
 *   1.Date:
 *     Author:
 *     Modification:
 *********************************************************/

#ifndef PERI_KEY_H_
#define PERI_KEY_H_


#include "gpio.h"
#include "../../include/os_type.h"
#include "driver/key.h"

#define CONFIG_KEY_0_IO_MUX     PERIPHS_IO_MUX_GPIO4_U


void peri_key_short_press(void);
void peri_key_long_press(void);
void peri_key_init(struct keys_param *keys);


#endif /* APP_INCLUDE_USER_USER_KEY_H_ */
