/*
 * tisan_gpio_intr.c
 *
 *  Created on: 2015Äê10ÔÂ28ÈÕ
 *      Author: Administrator
 */

#include "ets_sys.h"
#include "os_type.h"
#include "osapi.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include "../../pando/pando_subdevice.h"
#include "tisan_gpio_intr.h"

#define LONG_PRESS_COUNT 3000
uint8 vibrate_triggered_flag;
void gpio_intr_handler(struct keys_param *keys)
{
    uint8 i;
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);

    PRINTF("Get into gpio_intr_handler, gpio_status:%d\n", gpio_status);
       for (i = 0; i < keys->key_num; i++)
       {
          if (gpio_status & BIT(keys->single_key[i]->gpio_id))
           {
    	      //disable interrupt
    	      gpio_pin_intr_state_set(GPIO_ID_PIN(keys->single_key[i]->gpio_id), GPIO_PIN_INTR_DISABLE);

    	      //clear interrupt status
    	      GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(keys->single_key[i]->gpio_id));

        	   if((keys->single_key[i]->vbr_trigger) != NULL)
        	   {
				   /********************************************************/
				   keys->single_key[i]->vbr_count++;
				   PRINTF("vibrate count:%d\n", keys->single_key[i]->vbr_count);
				   if(((keys->single_key[i]->vbr_count)>100)&&(vibrate_triggered_flag==0))
				   {
					   vibrate_triggered_flag = 1;
					   keys->single_key[i]->vbr_count = 0;
					   PRINTF("vibration triggered\n");
					   if(keys->single_key[i]->vbr_trigger)
					   {
						   keys->single_key[i]->vbr_trigger();
						   vibrate_triggered_flag = 0;
					   }
				   }
				   gpio_pin_intr_state_set(GPIO_ID_PIN(keys->single_key[i]->gpio_id), GPIO_PIN_INTR_NEGEDGE);
				   /*******************************************************/
        	   }

      	       else
        	   {
				   if (keys->single_key[i]->key_level == 1)
				   {
					   // 5s, restart & enter softap mode
					   os_timer_disarm(&keys->single_key[i]->key_5s);
					   os_timer_setfn(&keys->single_key[i]->key_5s, (os_timer_func_t *)key_5s_cb, keys->single_key[i]);
					   os_timer_arm(&keys->single_key[i]->key_5s, LONG_PRESS_COUNT, 0);
					   keys->single_key[i]->key_level = 0;
					   gpio_pin_intr_state_set(GPIO_ID_PIN(keys->single_key[i]->gpio_id), GPIO_PIN_INTR_POSEDGE);
				   }
				   else
				   {
					   // 50ms, check if this is a real key up
					   os_timer_disarm(&keys->single_key[i]->key_50ms);
					   os_timer_setfn(&keys->single_key[i]->key_50ms, (os_timer_func_t *)key_50ms_cb, keys->single_key[i]);
					   os_timer_arm(&keys->single_key[i]->key_50ms, 50, 0);
				   }
        	   }
           }
       }

}


