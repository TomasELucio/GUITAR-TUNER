/*
 * tunerctrl.h
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#ifndef PRIVATELIBRARY_INC_TUNER_CTRL_H_
#define PRIVATELIBRARY_INC_TUNER_CTRL_H_

#include "freq_calc.h"
#include "string_select.h"

void 	tuner_init(void);
void    tuner_update(void);      // llamar desde el main loop — hace todo el cálculo
float   tuner_get_error(void);   // devuelve el error en Hz
float   tuner_get_freq(void);    // devuelve la frecuencia medida
uint8_t tuner_is_tuned(void);    // 1 si está dentro de la zona muerta
uint8_t tuner_is_active(void);   // 1 si hay señal válida (no silencio)

#endif /* PRIVATELIBRARY_INC_TUNER_CTRL_H_ */
