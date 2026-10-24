/*
 * lcd_display.h
 *
 *  Created on: Oct 11, 2026
 *      Author: tomaslucio
 */

#ifndef PRIVATELIBRARY_INC_LCD_DISPLAY_H_
#define PRIVATELIBRARY_INC_LCD_DISPLAY_H_

#include "stm32f4xx_hal.h"
#include "i2c_lcd.h"

// Inicializa el display y configura el botón para cambiar de modo
void lcd_display_init(I2C_HandleTypeDef *hi2c, uint16_t mode_pin);

// Maneja la interrupción del botón de cambio de modo
void lcd_display_isr(uint16_t gpio_pin);

// Actualiza la pantalla (debe llamarse dentro del while(1) principal)
void lcd_display_update(void);

#endif /* PRIVATELIBRARY_INC_LCD_DISPLAY_H_ */
