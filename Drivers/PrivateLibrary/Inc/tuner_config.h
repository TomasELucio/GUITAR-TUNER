/*
 * tunerconfig.h
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#ifndef PRIVATELIBRARY_INC_TUNER_CONFIG_H_
#define PRIVATELIBRARY_INC_TUNER_CONFIG_H_


/* ── Timer y medición ── */
#define TIMER_FREQ      1000000UL
#define MIN_DT          500UL
#define FREQ_BUF_SIZE   8U
#define SILENCE_MS        2000U   // era 100U — dar tiempo al motor
#define DISPLAY_RESET_MS  5000U   // ms sin señal para volver a "Toque una cuerda"
/* ── Control ── */
#define DEAD_ZONE_HZ    1.0f

/* ── Cuerdas ── */
#define N_STRINGS       6U

typedef struct {
    float       freq;
    const char *name;
} StringConfig_t;

static const StringConfig_t STRINGS[N_STRINGS] = {
    { 329.63f, "Mi"  },
    { 246.94f, "Si"  },
    { 196.00f, "Sol" },
    { 146.83f, "Re"  },
    { 110.00f, "La"  },
    {  82.41f, "Mi"  }
};

#endif /* PRIVATELIBRARY_INC_TUNER_CONFIG_H_ */
