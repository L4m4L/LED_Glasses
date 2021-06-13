#ifndef LED_H
#define LED_H

#define LED_COUNT (66)
#define LED_ROWS   (7)
#define LED_COLS  (10)

typedef struct _led_colour_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_colour_t;

void led_init(void);
void led_flush(void);
void led_clear(void);
void led_set(uint32_t led, led_colour_t c);
void led_set_all(led_colour_t c);
void led_set_pixel(int x, int y, led_colour_t c);

#endif // #ifndef LED_H