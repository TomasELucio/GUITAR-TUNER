/*
 * freqcalc.c
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#include "freq_calc.h"
#include "tuner_config.h"

void freq_calc_init(TIM_HandleTypeDef *timer_t, uint32_t channel_t) {
    FC.timer   = timer_t;
    FC.channel = channel_t;
    FC.t1      = 0;
    FC.last_tick = 0;
    FC.buf_idx   = 0;
    FC.buf_full  = 0;

    HAL_TIM_IC_Start_IT(FC.timer, FC.channel);
}

void freq_calc_ISR(TIM_HandleTypeDef *timer_t){
	if(FC.timer != timer_t->Instance) return;

	uint32_t t2 = HAL_TIM_ReadCapturedValue(FC.timer,FC.channel);
	uint32_t dt = (uint32_t)(t2 - FC.t1);

	if(dt > MIN_DT){
		FC.dt_buf[FC.buf_idx] = dt;
		FC.buf_idx = (FC.buf_idx + 1) % FREQ_BUF_SIZE;
		if(FC.buf_idx == 0){
			FC.buf_full = 1;
		}
	}
	FC.last_tick = HAL_GetTick();
	FC.t1 = t2;
}

float get_freq(){
	if(FC.buf_full == 0)return 0.0f;

	uint32_t buffer = 0;

	for (int var = 0; var < FREQ_BUF_SIZE; ++var) {
		buffer += FC.dt_buf[var];
	}
	float prom = (float)buffer/(float)FREQ_BUF_SIZE;
	return (float)TIMER_FREQ/prom;
}

uint8_t freq_is_valid(){
	return (HAL_GetTick() - FC.last_tick) < SILENCE_MS && FC.buf_full == 1;
}
