/*
 * plug_pwm.c
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Administrator
 */

#include "../../pando/pando_object.h"
#include "../../peripheral/peri_jdq.h"
#include "../../peripheral/peri_plug_pwm.h"
#include "c_types.h"
#include "user_interface.h"
#include "plug_pwm.h"


struct plug_pwm {
	uint8 off_on;
};
void ICACHE_FLASH_ATTR
plug_pwm_init() {
	// TODO: add your object init code here.
	peri_jdq_init(14);

	//pwm init
	struct PWM_APP_PARAM pwm_param;
	struct PWM_INIT pwm_init;
	pwm_param.pwm_freq=25000;
	pwm_param.pwm_duty[0]=0;

	pwm_init.io_num=1;
	pwm_init.io_id[0]=15;

	peri_plug_pwm_init(pwm_param, pwm_init);
}
void ICACHE_FLASH_ATTR
plug_pwm_set(struct plug_pwm* value) {
	// TODO: implement object set function here.
	// the set function read value and operate the hardware.
	PRINTF("peri_jdq_set start.\n");
	peri_jdq_set(value->off_on);
	PRINTF("peri_jdq_set over.\n");

	//add plug_pwm action
	//peri_plug_pwm_start(20);
	PRINTF("peri_plug_pwm_start start.:%d\n",value->off_on);
	if(value->off_on == 0)
	{
		peri_plug_pwm_param_timer_set();
	}
	else
	{
		peri_plug_pwm_start(60);
	}
}
void ICACHE_FLASH_ATTR
plug_pwm_get(struct plug_pwm* value) {
	// TODO: implement object get  function here
	// get function retrieve hardware status and assign it to value.
	value->off_on=peri_jdq_get();
}
/*
	auto generated code below!!
	DO NOT edit unless you know how it works.
*/
struct plug_pwm* ICACHE_FLASH_ATTR
create_plug_pwm() {
	struct plug_pwm* plug_pwm = (struct plug_pwm*)os_malloc(sizeof(plug_pwm));
	return plug_pwm;
}
void ICACHE_FLASH_ATTR
delete_plug_pwm(struct plug_pwm* plug_pwm) {
	if(plug_pwm){
		os_free(plug_pwm);
	}
}
void ICACHE_FLASH_ATTR
plug_pwm_object_pack(PARAMS * params) {
	if(NULL == params){
		PRINTF("Create first tlv param failed.\n");
		return;
	}

	struct plug_pwm* plug_pwm = create_plug_pwm();
	plug_pwm_get(plug_pwm);

	if (add_next_uint8(params, plug_pwm->off_on)){
		PRINTF("Add next param failed.\n");
		return;
	}

	delete_plug_pwm(plug_pwm);
}
void ICACHE_FLASH_ATTR
plug_pwm_object_unpack(PARAMS* params) {
	struct plug_pwm* plug_pwm = create_plug_pwm();

	plug_pwm->off_on = get_next_uint8(params);

	plug_pwm_set(plug_pwm);
	delete_plug_pwm(plug_pwm);
}
void ICACHE_FLASH_ATTR
plug_pwm_object_init() {
	plug_pwm_init();
	pando_object plug_pwm_object = {
		1,
		plug_pwm_object_pack,
		plug_pwm_object_unpack,
	};
	register_pando_object(plug_pwm_object);
}


