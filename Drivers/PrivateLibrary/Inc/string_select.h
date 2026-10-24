/*
 * stringselect.h
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#ifndef PRIVATELIBRARY_INC_STRING_SELECT_H_
#define PRIVATELIBRARY_INC_STRING_SELECT_H_

#include "string_select.h"
#include "tuner_config.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef struct {
    uint8_t  string_idx;
    uint16_t up_pin;
    uint16_t down_pin;
    uint32_t last_btn_tick;
} StringSelect_t;

void        string_select_init(uint16_t up_pin_t, uint16_t down_pin_t);
void        string_select_isr(uint16_t gpio_pin);

float       string_get_freq(void);
const char* string_get_name(void);
uint8_t     string_get_index(void);


#endif /* PRIVATELIBRARY_INC_STRING_SELECT_H_ */
