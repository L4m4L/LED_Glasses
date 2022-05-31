/**
 * This module it designed to work with the SK6812MINI smart LED. Each LED is individually
 * addressable, and can display 24 bit colours in full RGB. The data interface uses PWM to
 * represent bits with a period of 1.2us per bit, a pulse width of 0.3us for 0 and 0.6us for 1.
 * The LEDs are connected in serial and each LED along strips the first 24 bits off of the data
 * stream and relays the remainder to the next led. We use the SPI interface to communicate with
 * the LEDs. To achieve this we set the SPI clock rate to be 4x the PWM frequency (3.333MHz) and
 * represent a single PWM cycle or LED bit as 4 SPI bits (0 -> 1000 and 1 -> 1100).
 */

#ifndef LED_H
#define LED_H

#include "util.h"

typedef struct colour_t_
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} colour_t;

/**
 * Clears the display buffer, initialises peripherals, and flushes the display buffer to the LEDs.
 */
void led_init(void);
/** Flushes the display buffer in a loop. */
void led_flush(void);
/** Begins a DMA transfer to flush the display buffer. */
void led_flush_dma(void);
/** Zeros the display buffer. */
void led_clear(void);
/**
 * Sets a particular LED to the specified colour.
 * \param[in]  led  The index of the LED as dictated by the electrical layout.
 * \param[in]  c    The desired RGB colour of the LED.
 */
void led_set(uint32_t led, colour_t c);
/**
 * Sets all LEDs to the desired colour.
 * \param[in]  c  The desired RGB colour.
 */
void led_set_all(colour_t c);
/**
 * Sets the led at location (x, y) to the desired colour.
 * \param[in]  x  The number of columns from the left.
 * \param[in]  y  The number of rows from the top.
 * \param[in]  c  The desired RGB colour.
 */
void led_set_pixel(uint32_t x, uint32_t y, colour_t c);

#endif /** LED_H */