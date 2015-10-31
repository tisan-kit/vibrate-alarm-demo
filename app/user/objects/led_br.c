/*
 * led_br.c
 *
 *  Created on: 2015Äê10ÔÂ30ÈÕ
 *      Author: Administrator
 */

#include "../../peripheral/peri_led_br.h"
#include "../../pando/pando_object.h"
#include "c_types.h"
#include "user_interface.h"
#include "led_br.h"
// add your own includes below


struct led_br {
	uint8 degree;
};
void ICACHE_FLASH_ATTR
led_br_init() {
	// TODO: add your object init code here.
	//pwm init
	struct PWM_APP_PARAM pwm_param;
	struct PWM_INIT pwm_init;
	pwm_param.pwm_freq=25000;
	pwm_param.pwm_duty[0]=0;

	pwm_init.io_num=1;
	pwm_init.io_id[0]=15;

	//peri_plug_pwm_init(pwm_param, pwm_init);
	peri_led_br_init(pwm_param, pwm_init);
}
void ICACHE_FLASH_ATTR
led_br_set(struct led_br* value) {
	// TODO: implement object set function here.
	// the set function read value and operate the hardware.
	struct PWM_APP_PARAM led_br_param;
	led_br_param.pwm_freq=25000;
	led_br_param.pwm_duty[0]=value->degree;

	peri_led_br_set(led_br_param);
}
void ICACHE_FLASH_ATTR
led_br_get(struct led_br* value) {
	// TODO: implement object get  function here
	// get function retrieve hardware status and assign it to value.
	struct PWM_APP_PARAM motor_param;
	motor_param=peri_led_br_get();

	value->degree=motor_param.pwm_duty[0];

}
/*
	auto generated code below!!
	DO NOT edit unless you know how it works.
*/
struct led_br* ICACHE_FLASH_ATTR
create_led_br() {
	struct led_br* led_br = (struct led_br*)os_malloc(sizeof(led_br));
	return led_br;
}
void ICACHE_FLASH_ATTR
delete_led_br(struct led_br* led_br) {
	if(led_br){
		os_free(led_br);
	}
}
void ICACHE_FLASH_ATTR
led_br_object_pack(PARAMS * params) {
	if(NULL == params){
		PRINTF("Create first tlv param failed.\n");
		return;
	}

	struct led_br* led_br = create_led_br();
	led_br_get(led_br);

	if (add_next_uint8(params, led_br->degree)){
		PRINTF("Add next param failed.\n");
		return;
	}

	delete_led_br(led_br);
}
void ICACHE_FLASH_ATTR
led_br_object_unpack(PARAMS* params) {
	struct led_br* led_br = create_led_br();

	led_br->degree = get_next_uint8(params);

	led_br_set(led_br);
	delete_led_br(led_br);
}
void ICACHE_FLASH_ATTR
led_br_object_init() {
	led_br_init();
	pando_object led_br_object = {
		1,
		led_br_object_pack,
		led_br_object_unpack,
	};
	register_pando_object(led_br_object);
}




