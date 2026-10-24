/*
 * tunerctrl.c
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#include "tuner_ctrl.h"

typedef struct {
    float error;       // f_ref - f_medida
    float f_medida;    // la frecuencia que midió en este ciclo
    float f_ref;       // la frecuencia objetivo de la cuerda actual
} TunerControl_t;

static TunerControl_t TC;

void tuner_init(void){
	TC.error = 0.0f;
	TC.f_medida = 0.0f;
	TC.f_ref = string_get_freq();
}

void    tuner_update(void){
	if(!freq_is_valid())return;
	TC.f_ref = string_get_freq();
	TC.f_medida = get_freq();
	TC.error = TC.f_ref - TC.f_medida;
}

float   tuner_get_error(void){
	return TC.error;
}

float   tuner_get_freq(void){
	return TC.f_medida;
}

uint8_t tuner_is_tuned(void){
	if(TC.error >= -DEAD_ZONE_HZ && TC.error <= DEAD_ZONE_HZ) return 1;
	return 0;
}

uint8_t tuner_is_active(void){
	return freq_is_valid();
}
