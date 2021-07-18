#include "display.h"
#include "led.h"

void display_init(void)
{
    led_init();
}

void display_run(context_t* context)
{
    uint32_t frame = 0;
    colour_t c;
    float f;

    switch (context->mode)
    {
    case DISPLAY_MODE_DEFAULT:
        led_clear();
        break;
    case DISPLAY_MODE_ANIMATION:
        frame = context->bass * context->length;
        frame = frame < context->length ? frame : context->length;
        for (uint32_t led_idx = 0; led_idx < LED_COUNT; led_idx++) {
            led_set(led_idx, (*context->animation)[frame][led_idx]);
        }
        break;
    case DISPLAY_MODE_HEATMAP:
        c.r = 255 * context->bass;
        c.g = 0;
        c.b = 255 * (1 - context->bass);
        f = context->bass / 0.4f;
        c.r *= f;
        c.b *= f;
        led_set_all(c);
        break;
    }
    led_flush();
}

// static void display_draw_line(int x1, int y1, int x2, int y2, led_colour_t c)
// {
//     int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
//     int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
//     int err = dx + dy, e2;

//     for (;;){
//         led_set_pixel(x1, y1, c);

//         if (x1==x2 && y1==y2) break;

//         e2 = 2*err;
//         if (e2 >= dy) { err += dy; x1 += sx; }
//         if (e2 <= dx) { err += dx; y1 += sy; }
//     }
//     // int dx = abs(x2 - x1);
//     // int dy = abs(y2 - y1);
//     // int sx = x1 < x2 ? 1 : -1;
//     // int sy = y1 < y2 ? 1 : -1;
//     // int err = dx + dy;
//     // int e2;

//     // led_set_pixel(x1, y1, c);

//     // while((x1 != x2) || (y1 != y2))
//     // {
//     //     e2 = 2 * err;
//     //     if (e2 >= dy)
//     //     {
//     //         err += dy;
//     //         x1 += sx;
//     //     }
//     //     if (e2 <= dx)
//     //     {
//     //         err += dx;
//     //         y1 += sy;
//     //     }

//     //     led_set_pixel(x1, y1, c);
//     // }
// }

// static void display_draw_line_rand(void)
// {
//     int y1 = rand() % LED_ROWS;
//     int y2 = rand() % LED_ROWS;
//     // led_colour_t c = {3,3,4};
//     // display_draw_line(0, y1, LED_COLS-1, y2, c);
//     static uint32_t i = 0;
//     led_colour_t c[] = {
//         {4, 0, 0}, // red
//         {0, 4, 0}, // green
//         {0, 0, 4}, // blue
//         {4, 4, 0}, // yellow
//         {4, 0, 4}, // magenta
//         {0, 4, 4}, // cyan
//         {4, 4, 4}
//     };
//     display_draw_line(0, y1, LED_COLS-1, y2, c[i]);
//     i += 1;
//     i = i < sizeof(c)/sizeof(c[0]) ? i : 0;
// }