/*
 * stringselect.c
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#include "string_select.h"

static StringSelect_t SS;

void string_select_init(uint16_t up_pin_t, uint16_t down_pin_t){
	SS.string_idx = 0;
	SS.last_btn_tick = 0;
	SS.up_pin = up_pin_t;
	SS.down_pin = down_pin_t;
}

void string_select_isr(uint16_t gpio_pin){

    if (HAL_GetTick() - SS.last_btn_tick < 200) return;  // debounce una sola vez
    SS.last_btn_tick = HAL_GetTick();

	if(SS.up_pin == gpio_pin){
			if(SS.string_idx == N_STRINGS - 1){
				SS.string_idx = 0;
			}else{
				SS.string_idx++;
			}
	}
	if(SS.down_pin == gpio_pin){
			if(SS.string_idx == 0){
				SS.string_idx = 5;
			}else{
				SS.string_idx--;
			}
	}
}

float string_get_freq(void){
	return STRINGS[SS.string_idx].freq;
}

const char* string_get_name(void){
	return STRINGS[SS.string_idx].name;
}

uint8_t     string_get_index(void){
	return SS.string_idx;
}
