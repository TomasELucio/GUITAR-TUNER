/*
 * freqcalc.h
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#ifndef PRIVATELIBRARY_INC_FREQ_CALC_H_
#define PRIVATELIBRARY_INC_FREQ_CALC_H_

#include "stm32f4xx_hal.h"
#include "tuner_config.h"

typedef struct {
	TIM_HandleTypeDef 	*timer;
	uint32_t 			channel;
	uint32_t 			t1;
	uint32_t 			last_tick;
    uint32_t           	dt_buf[FREQ_BUF_SIZE];  // buffer circular de períodos
    uint8_t            	buf_idx;                // índice actual del buffer
    uint8_t            	buf_full;               // 1 cuando el buffer se llenó al menos una vez
}FreqCalculator_t;

void freq_calc_init(TIM_HandleTypeDef *timer,uint32_t channel);
void freq_calc_ISR(TIM_HandleTypeDef *timer);
float get_freq(void);
uint8_t freq_is_valid(void);


#endif /* PRIVATELIBRARY_INC_FREQ_CALC_H_ */
