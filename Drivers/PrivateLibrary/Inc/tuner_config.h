/*
 * tunerconfig.h
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#ifndef PRIVATELIBRARY_INC_TUNER_CONFIG_H_
#define PRIVATELIBRARY_INC_TUNER_CONFIG_H_

#define SILENCE_MS   100U        // ms sin flancos = silencio
#define FREQ_BUF_SIZE   8U


typedef struct {
    float       freq;
    const char *name;
} StringConfig_t;

const StringConfig_t STRINGS[] = {
    { 329.63f, "Mi" },
    { 246.94f, "Si" },
    { 196.00f, "Sol" },
    { 146.83f, "Re"  },
    { 110.00f, "La"  },
    {  82.41f, "Mi"  }
};


#endif /* PRIVATELIBRARY_INC_TUNER_CONFIG_H_ */
