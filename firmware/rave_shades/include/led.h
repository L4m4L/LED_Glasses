#ifndef LED_H
#define LED_H

typedef struct led_colour_t_ {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_colour_t;

void led_init(void);
void led_flush(void);
void led_reset(void);
uint32_t led_clear(void);
uint32_t led_set(uint32_t led, led_colour_t c);
uint32_t led_set_all(led_colour_t c);
uint32_t led_set_pixel(uint32_t x, uint32_t y, led_colour_t c);

#endif // #ifndef LED_H