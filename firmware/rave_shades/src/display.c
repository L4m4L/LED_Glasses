#include <math.h>

// #include "animation.h"
#include "display.h"
#include "display_heart.h"
#include "display_spectrum.h"
#include "led.h"
#include "system.h"

// lachlans functions
static void display_mode_lachlan(context_t *context);
static void plot_4_points(uint32_t x_coord, uint32_t y_coord, uint32_t x, uint32_t y, colour_t c);
static void plot_8_points(uint32_t x_coord, uint32_t y_coord, uint32_t x, uint32_t y, colour_t c);
static void draw_circle(uint32_t x_coord, uint32_t y_coord, uint32_t rad, colour_t c);
// End lachlans functions

void display_init(void)
{
    led_init();
    display_heart_init();
    display_spectrum_init();
}

void display_run(context_t *context)
{
    static display_mode_e display_mode_previous = DISPLAY_MODE_DEFAULT;

    if (context->display_mode != display_mode_previous)
    {
        display_mode_previous = context->display_mode;

        led_clear();
    }

    switch (context->display_mode)
    {
    case DISPLAY_MODE_DEFAULT:
        led_clear();
        break;
    case DISPLAY_MODE_HEART:
        display_heart_run(context);
        break;
    case DISPLAY_MODE_WATERFALL:
        display_spectrum_run(context);
        break;
    case DISPLAY_MODE_LACHLAN:
        display_mode_lachlan(context);
        break;
    default:
        led_clear();
    }

    led_flush_dma();
}

// static void display_mode_heart(void)
// {
//     /*static uint32_t frame_idx = 0;*/
//     static double time_last = 0;

//     double time = system_get_sec();

//     if (time > time_last + 0.0)
//     {
//         // time_last = time;

//         // led_clear();

//         // for (uint32_t led_idx = 0; led_idx < LED_COUNT; led_idx++) {
//         //     led_set(led_idx, animation_heart[frame_idx][led_idx]);
//         // }

//         // frame_idx += 1;

//         // if (frame_idx > ANIMATION_HEART_LEN - 1)
//         // {
//         //     frame_idx = 0;
//         // }
//     }

//     // static uint32_t bass_history_idx = 0;
//     // static float bass_previous = 0;
//     // static float bass_history[DISPLAY_HISTORY_LEN];
//     // uint32_t frame;
//     // float bass, bass_scaled, bass_rate_limited;

//     // // See audio.c for which frequencies correspond to which index in the audio_volume array.
//     // bass = context->audio_volume[0];

//     // // Using recent bass values determine a minimum and maximum range to scale the bass within.
//     // // This should hopefully keep the display reacting even if the volume is reduced.
//     // display_volume_history_update(DISPLAY_HISTORY_LEN, bass, bass_history, &bass_history_idx);
//     // bass_scaled = display_volume_history_scale(DISPLAY_HISTORY_LEN, bass, bass_history, DISPLAY_BASS_RANGE_MIN);

//     // bass_rate_limited = bass_scaled;
//     // if (bass_rate_limited < bass_previous - DISPLAY_BASS_RATE_LIMIT)
//     // {
//     //     bass_rate_limited = bass_previous - DISPLAY_BASS_RATE_LIMIT;
//     // }
//     // bass_previous = bass_rate_limited;

//     // frame = (uint32_t)(bass_rate_limited * (float)(ANIMATION_HEART_LEN));
//     // frame = frame < ANIMATION_HEART_LEN ? frame : ANIMATION_HEART_LEN - 1;

//     // // Display the animation frame corresponding to the scaled bass value. Loop through
//     // // animation_heart[][], led by led and get the colour_t value from the animation.c file for
//     // // each 66 LEDs as defined by the animation array.
//     // for (uint32_t led_idx = 0; led_idx < LED_COUNT; led_idx++) {
//     //     led_set(led_idx, animation_heart[frame][led_idx]);
//     // }
// }

// static void display_volume_history_update(uint32_t volume_history_len, float volume, float *volume_history, uint32_t *volume_history_idx)
// {
//     volume_history[(*volume_history_idx)] = volume;
//     (*volume_history_idx)++;
//     (*volume_history_idx) = (*volume_history_idx) < volume_history_len ? (*volume_history_idx) : 0U;
// }

// static float display_volume_history_scale(uint32_t volume_history_len, float volume, const float *volume_history, float volume_range_min)
// {
//     float volume_min = 3e38f;
//     float volume_max = -3e38f;
//     float volume_range = 1;
//     float volume_scaled = 0;

//     for (uint32_t i = 0; i < volume_history_len; i++)
//     {
//         if (volume_history[i] < volume_min)
//         {
//             volume_min = volume_history[i];
//         }

//         if (volume_history[i] > volume_max)
//         {
//             volume_max = volume_history[i];
//         }
//     }

//     volume_range = fabsf(volume_max - volume_min);
//     volume_range = volume_range > volume_range_min ? volume_range : volume_range_min;

//     // Scale the volume so it fits within the range (0 <= volume < 1).
//     volume_scaled = (volume - volume_min) / volume_range;
//     volume_scaled = volume_scaled > 0 ? volume_scaled : 0;
//     volume_scaled = volume_scaled < display_less_than_1 ? volume_scaled : display_less_than_1;

//     return volume_scaled;
// }

// Lachlans Test!
static void display_mode_lachlan(context_t *context)
{
    (void)(context); // Get's rid of "unused variable" warning.
    led_clear();

    // lens 1
    draw_circle(2, 3, 2, (colour_t){10, 0, 0});

    // lens 2
    draw_circle(7, 3, 2, (colour_t){10, 0, 0});
}

// Plot 4 points
static void plot_4_points(uint32_t x_coord, uint32_t y_coord, uint32_t x, uint32_t y, colour_t c)
{
    led_set_pixel(x_coord + x, y_coord + y, c);

    if (x != 0)
    {
        led_set_pixel(x_coord - x, y_coord + y, c);
        led_set_pixel(x_coord + x, y_coord - y, c);
    }

    led_set_pixel(x_coord - x, y_coord - y, c);
}

// Plot 8 points
static void plot_8_points(uint32_t x_coord, uint32_t y_coord, uint32_t x, uint32_t y, colour_t c)
{
    plot_4_points(x_coord, y_coord, x, y, c);

    if (x != y)
    {
        plot_4_points(x_coord, y_coord, y, x, c);
    }
}

// Draw circle
static void draw_circle(uint32_t x_coord, uint32_t y_coord, uint32_t rad, colour_t c)
{
    int32_t error = -rad;
    int32_t x = rad;
    int32_t y = 0;

    while (x >= y)
    {
        // x and y converted to unsigned from signed, might want to consider changing the
        // function arguments so you don't get funniness with negative numbers.
        plot_8_points(x_coord, y_coord, x, y, c);
        error += y;
        ++y;
        error += y;

        if (error >= 0)
        {
            --x;
            error -= x;
            error -= x;
        }
    }
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