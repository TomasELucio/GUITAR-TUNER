/*
 * lcd_display.c
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */


#include "lcd_display.h"
#include "string_select.h"
#include "tuner_ctrl.h"
#include <stdio.h>

typedef struct {
    I2C_LCD_HandleTypeDef hlcd;
    uint16_t              mode_pin;
    uint8_t               mode;             // 0 = Selección, 1 = Gráfico/Afinación
    uint32_t              last_btn_tick;
    uint32_t              last_update_tick;
    uint8_t				  needs_clear;
    uint32_t              last_active_tick;  // ← nuevo: cuándo fue la última señal válida
    float                 last_freq;         // ← nuevo: última frecuencia válida
    float                 last_error;        // ← nuevo: último error válido
} LcdDisplay_t;

static LcdDisplay_t LD;

void lcd_display_init(I2C_HandleTypeDef *hi2c, uint16_t mode_pin) {
    LD.hlcd.hi2c = hi2c;
    LD.hlcd.address = 0x4E; // Dirección típica I2C del PCF8574 (0x27 desplazado 1 bit)

    LD.mode_pin = mode_pin;
    LD.mode = 0;
    LD.last_btn_tick = 0;
    LD.last_update_tick = 0;

    lcd_init(&LD.hlcd);     // Inicializa el display
    lcd_clear(&LD.hlcd);    // Limpia la pantalla
}

void lcd_display_isr(uint16_t gpio_pin) {
    if (gpio_pin != LD.mode_pin) return;
    if (HAL_GetTick() - LD.last_btn_tick < 200) return;
    LD.last_btn_tick = HAL_GetTick();
    LD.mode       = !LD.mode;
    LD.needs_clear = 1;        // ← solo setear flag
}

void lcd_display_update(void) {
    if (LD.needs_clear) {
           lcd_clear(&LD.hlcd);
           LD.needs_clear = 0;
           return;               // esperar al próximo ciclo para dibujar
       }

    // Actualizar pantalla cada 250ms para evitar parpadeos
    if (HAL_GetTick() - LD.last_update_tick < 250) return;
    LD.last_update_tick = HAL_GetTick();


    char buffer[17];

    if (LD.mode == 0) {
        // MODO 0: Selección de cuerda (Muestra nombre y frecuencia de referencia)
        sprintf(buffer, "Cuerda: %-8s", string_get_name());
        lcd_gotoxy(&LD.hlcd, 0, 0);
        lcd_puts(&LD.hlcd, buffer);

        sprintf(buffer, "Ref: %5.1f Hz  ", string_get_freq());
        lcd_gotoxy(&LD.hlcd, 0, 1);
        lcd_puts(&LD.hlcd, buffer);
    }
    else {
    	// MODO 1: Afinación
    	if (tuner_is_active()) {
    	    // Hay señal — actualizar valores retenidos
    	    LD.last_freq       = tuner_get_freq();
    	    LD.last_error      = tuner_get_error();
    	    LD.last_active_tick = HAL_GetTick();
    	}

    	// Decidir qué mostrar
    	uint8_t signal_recently = (HAL_GetTick() - LD.last_active_tick) < DISPLAY_RESET_MS;

    	if (!signal_recently) {
    	    // Pasaron más de 5 segundos sin señal → pedir que toque
    	    lcd_gotoxy(&LD.hlcd, 0, 0);
    	    lcd_puts(&LD.hlcd, "--- Silencio ---");
    	    lcd_gotoxy(&LD.hlcd, 0, 1);
    	    lcd_puts(&LD.hlcd, "Toque una cuerda");
    	} else {
    	    // Hay señal o hubo señal reciente → mostrar último valor conocido
    	    snprintf(buffer, sizeof(buffer), "F:%5.1fHz %-3s",
    	             LD.last_freq, string_get_name());
    	    lcd_gotoxy(&LD.hlcd, 0, 0);
    	    lcd_puts(&LD.hlcd, buffer);

    	    if (tuner_is_tuned()) {
    	        lcd_gotoxy(&LD.hlcd, 0, 1);
    	        lcd_puts(&LD.hlcd, "== AFINADA ==   ");
    	    } else if (LD.last_error > 0) {
    	        snprintf(buffer, sizeof(buffer), "TENSAR %+.1fHz  ", LD.last_error);
    	        lcd_gotoxy(&LD.hlcd, 0, 1);
    	        lcd_puts(&LD.hlcd, buffer);
    	    } else {
    	        snprintf(buffer, sizeof(buffer), "AFLOJAR%+.1fHz  ", LD.last_error);
    	        lcd_gotoxy(&LD.hlcd, 0, 1);
    	        lcd_puts(&LD.hlcd, buffer);
    	    }
    	}
    }
}
