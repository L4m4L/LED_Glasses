/******************************************************************************
** This module it designed to work with the SK6812MINI smart LED. Each LED   **
** is individually addressable, and can display 24bit colours in full RGB.   **
** The data interface uses PWM to represent bits with a period of 1.2us per  **
** bit, a pulse width of 0.3us for 0 and 0.6us for 1. The LEDs are connected **
** in serial and each LED along strips the first 24 bits off of the data     **
** stream and relays the remainder to the next led. We use the SPI interface **
** to communicate with the LEDs. To achieve this we set the spi clock rate   **
** to be 4x the PWM frequency (3.333MHz) and represent a single PWM cycle or **
** LED bit as 4 spi bits (0 -> 1000 and 1 -> 1100).                          **
*******************************************************************************/

#ifndef LED_H
#define LED_H

#include "util.h"

void led_init(void);
void led_flush(void);
void led_flush_dma(void);
void led_clear(void);
void led_set(uint32_t led, colour_t c);
void led_set_all(colour_t c);
void led_set_pixel(uint32_t x, uint32_t y, colour_t c);

#endif // #ifndef LED_H