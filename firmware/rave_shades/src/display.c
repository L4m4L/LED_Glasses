#include <stdint.h>
#include <stdlib.h>

#include "display.h"
#include "led.h"
#include "mic.h"
#include "system.h"
#include "util.h"

#define DISPLAY_TASK_PERIOD (0.02)
#define DISPLAY_MODE_LINES_PERIOD (1)

static enum display_state_e
{
    DISPLAY_STATE_INIT,
    DISPLAY_STATE_RUN
} display_state = DISPLAY_STATE_INIT;

void display_init(void);
void display_run(context_t* context);
void display_draw_line(int x1, int y1, int x2, int y2, led_colour_t c);
void display_draw_line_rand(void);

void display_task(context_t* context)
{
    static double next = 0;
    if (system_get_sec() > next)
    {
        switch (display_state)
        {
        case DISPLAY_STATE_INIT:
            display_init();
            break;
        case DISPLAY_STATE_RUN:
            display_run(context);
            break;
        }
        next += DISPLAY_TASK_PERIOD;
    }
}

void display_init(void)
{
    led_init();
    mic_init();
    display_state = DISPLAY_STATE_RUN;
}

void display_run(context_t* context)
{
    static double display_mode_lines_next = 0;
    uint8_t display_update = 0;
    uint8_t min, max;
    switch (context->display_mode)
    {
    case DISPLAY_MODE_LINES:
        if (system_get_sec() > display_mode_lines_next)
        {
            led_clear();
            display_draw_line_rand();
            display_update = 1;
            display_mode_lines_next += DISPLAY_MODE_LINES_PERIOD;
        }
        break;
    case DISPLAY_MODE_PK2PK:
        mic_read(&min, &max);
        led_clear();
        min = (uint8_t)((double)(min) * 0.524);
        max *= 0.524;
        for(uint8_t i = 0; i < LED_COUNT; i++)
        {
            if ((i >= min) && (i <= max))
            {
                led_set(i, (led_colour_t){4, 0, 0});
            }
        }
        display_update = 1;
        break;
    case DISPLAY_MODE_SCROLL:
        break;
    }

    if (display_update)
    {
        led_flush();
    }
}

void display_draw_line(int x1, int y1, int x2, int y2, led_colour_t c)
{
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    for (;;){
        led_set_pixel(x1, y1, c);

        if (x1==x2 && y1==y2) break;

        e2 = 2*err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
    // int dx = abs(x2 - x1);
    // int dy = abs(y2 - y1);
    // int sx = x1 < x2 ? 1 : -1;
    // int sy = y1 < y2 ? 1 : -1;
    // int err = dx + dy;
    // int e2;

    // led_set_pixel(x1, y1, c);

    // while((x1 != x2) || (y1 != y2))
    // {
    //     e2 = 2 * err;
    //     if (e2 >= dy)
    //     {
    //         err += dy;
    //         x1 += sx;
    //     }
    //     if (e2 <= dx)
    //     {
    //         err += dx;
    //         y1 += sy;
    //     }

    //     led_set_pixel(x1, y1, c);
    // }
}

void display_draw_line_rand(void)
{
    int y1 = rand() % LED_ROWS;
    int y2 = rand() % LED_ROWS;
    // led_colour_t c = {3,3,4};
    // display_draw_line(0, y1, LED_COLS-1, y2, c);
    static uint32_t i = 0;
    led_colour_t c[] = {
        {4, 0, 0}, // red
        {0, 4, 0}, // green
        {0, 0, 4}, // blue
        {4, 4, 0}, // yellow
        {4, 0, 4}, // magenta
        {0, 4, 4}, // cyan
        {4, 4, 4}
    };
    display_draw_line(0, y1, LED_COLS-1, y2, c[i]);
    i += 1;
    i = i < sizeof(c)/sizeof(c[0]) ? i : 0;
}