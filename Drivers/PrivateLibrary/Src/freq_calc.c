/*
 * freqcalc.c
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#include "freq_calc.h"
#include "tuner_config.h"

static FreqCalculator_t FC;


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
    if(FC.timer->Instance != timer_t->Instance) return;

    uint32_t t2 = HAL_TIM_ReadCapturedValue(FC.timer,FC.channel);
    uint32_t dt = (uint32_t)(t2 - FC.t1);

    if(dt > MIN_DT){
        FC.dt_buf[FC.buf_idx] = dt;
        FC.buf_idx = (FC.buf_idx + 1) % FREQ_BUF_SIZE;
        if(FC.buf_idx == 0){
            FC.buf_full = 1;
        }
        FC.t1 = t2;
    }
    FC.last_tick = HAL_GetTick();
}

float get_freq(void) {
    if (FC.buf_full == 0) return 0.0f;

    uint32_t sorted[FREQ_BUF_SIZE];

    // SOLUCIÓN 2: Guardar el estado de las interrupciones y deshabilitarlas
    uint32_t primask_bit = __get_PRIMASK();
    __disable_irq();

    /* Copiar buffer de forma segura e ininterrumpible */
    for (uint8_t i = 0; i < FREQ_BUF_SIZE; i++) {
        sorted[i] = FC.dt_buf[i];
    }

    // Restaurar las interrupciones inmediatamente al terminar la copia
    __set_PRIMASK(primask_bit);

    /* Ordenar con bubble sort */
    for (uint8_t i = 0; i < FREQ_BUF_SIZE - 1; i++) {
        for (uint8_t j = 0; j < FREQ_BUF_SIZE - 1 - i; j++) {
            if (sorted[j] > sorted[j + 1]) {
                uint32_t tmp  = sorted[j];
                sorted[j]     = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }

    /* Tomar los 4 valores centrales */
    uint32_t suma = 0;
    for (uint8_t i = 2; i < FREQ_BUF_SIZE - 2; i++) {
        suma += sorted[i];
    }
    float dt_central = (float)suma / 4.0f;

    return (float)TIMER_FREQ / dt_central;
}
uint8_t freq_is_valid(){
	return (HAL_GetTick() - FC.last_tick) < SILENCE_MS && FC.buf_full == 1;
}
