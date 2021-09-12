#include <math.h>

#include "display.h"
#include "led.h"

#define DISPLAY_HISTORY_LEN (100U)
#define DISPLAY_BASS_BINS (4U)
#define DISPLAY_BASS_RANGE_MIN (0.0015f)
#define DISPLAY_BASS_RATE_LIMIT (0.03f)
#define DISPLAY_WATERFALL_RANGE_MIN (0.0000001f)

static float display_less_than_1 = 0;

static void display_mode_animation(context_t* context);
static void display_mode_waterfall(context_t* context);
static void display_volume_history_update(uint32_t volume_history_len, float volume, float* volume_history, uint32_t* volume_history_idx);
static float display_volume_history_scale(uint32_t volume_history_len, float volume, const float* volume_history, float volume_range_min);

// lachlans functions
static void display_mode_lachlan(context_t* context);
static void plot_4_points(uint32_t x_coord, uint32_t y_coord, uint32_t x, uint32_t y, colour_t c);
static void plot_8_points(uint32_t x_coord, uint32_t y_coord, uint32_t x, uint32_t y, colour_t c);
static void draw_circle(uint32_t x_coord, uint32_t y_coord, uint32_t rad, colour_t c);
// End lachlans functions

void display_init(void)
{
    // Used to map floats to a [0, 1) range which is handy for maping to a range of integers when
    // you don't want to include the last number.
    display_less_than_1 = nextafterf(1, 0);

    led_init();
}

void display_run(context_t* context)
{
    switch (context->display_mode)
    {
    case DISPLAY_MODE_DEFAULT:
        led_clear();
        // for (uint32_t i = 0; i < LED_COUNT; i++)
        // {
        //     led_set(i, (colour_t){i+1, i+1, i+1});
        // }
        // for (uint32_t x = 0; x < 10; x++)
        // {
        //     for (uint32_t y = 0; y < 7; y++)
        //     {
        //         led_set_pixel(x, y, (colour_t){x+1, y+1, 5-(x+y)/3});
        //     }
        // }
        break;
    case DISPLAY_MODE_ANIMATION:
        if (context->audio_updated)
        {
            context->audio_updated = 0;
            display_mode_animation(context);
        }
        break;
    case DISPLAY_MODE_WATERFALL:
        if (context->audio_updated)
        {
            context->audio_updated = 0;
            display_mode_waterfall(context);
        }
        break;
    case DISPLAY_MODE_LACHLAN:
        display_mode_lachlan(context);
        break;
    default:
        led_clear();
    }
    //led_flush();
    led_flush_dma();
}

static void display_mode_animation(context_t* context)
{
    static uint32_t bass_history_idx = 0;
    static float bass_previous = 0;
    static float bass_history[DISPLAY_HISTORY_LEN];
    uint32_t frame;
    float bass, bass_scaled, bass_rate_limited;

    // Calculate bass using the average of the lowest DISPLAY_BASS_BINS. This should correspond to
    // audio energy below ~250Hz.
    bass = 0;
    for (uint32_t bin = 0; bin < DISPLAY_BASS_BINS; bin++)
    {
        bass += context->audio_volume[bin];
    }
    bass /= DISPLAY_BASS_BINS;
    
    // Using recent bass values determine a minimum and maximum range to scale the bass within.
    // This should hopefully keep the display reacting even if the volume is reduced.
    display_volume_history_update(DISPLAY_HISTORY_LEN, bass, bass_history, &bass_history_idx);
    bass_scaled = display_volume_history_scale(DISPLAY_HISTORY_LEN, bass, bass_history, DISPLAY_BASS_RANGE_MIN);

    bass_rate_limited = bass_scaled;
    if (bass_rate_limited < bass_previous - DISPLAY_BASS_RATE_LIMIT)
    {
        bass_rate_limited = bass_previous - DISPLAY_BASS_RATE_LIMIT;
    }

    frame = (uint32_t)(bass_rate_limited * (float)(context->display_animation_len));
    frame = frame < context->display_animation_len ? frame : context->display_animation_len;

    // Display the animation frame corresponding to the scaled bass value.
    for (uint32_t led_idx = 0; led_idx < LED_COUNT; led_idx++) {
        // LM:  (*context->display_animation)[frame][led_idx] gets the colour_t c value from the animation.c file
        //      loops through the whole 66 LEDs as the animation arrays define that 
        led_set(led_idx, (*context->display_animation)[frame][led_idx]);
    }
    
    bass_previous = bass_rate_limited;
}

static void display_mode_waterfall(context_t* context)
{
    static uint32_t volume_history_idx = 0;
    static float volume_history[DISPLAY_HISTORY_LEN] = {0};
    uint32_t volume_row_height = 0;
    float volume_scaled;
    float volume[AUDIO_FREQUENCY_BINS];

    for (uint32_t bin = 0; bin < AUDIO_FREQUENCY_BINS; bin++)
    {
        // Using a log scale for volume, this may or may not work nicely.
        if (context->audio_volume[bin] > 0)
        {
            volume[bin] = log2f(context->audio_volume[bin]);
        }
        else
        {
            volume[bin] = -20;
        }

        display_volume_history_update(DISPLAY_HISTORY_LEN, volume[bin], volume_history, &volume_history_idx);
    }

    led_clear();

    for (uint32_t x = 0; x < AUDIO_FREQUENCY_BINS; x++)
    {
        //volume_scaled = display_volume_history_scale(DISPLAY_HISTORY_LEN, volume[x], volume_history, DISPLAY_WATERFALL_RANGE_MIN);
        volume_scaled = (volume[x] - (-11)) / 2.0;
        volume_scaled = volume_scaled > 0                   ? volume_scaled : 0;
        volume_scaled = volume_scaled < display_less_than_1 ? volume_scaled : display_less_than_1;

        volume_row_height = (uint32_t)((float)(LED_ROWS + 1) * volume_scaled);
        
        for (uint32_t y = 0; y < volume_row_height; y++)
        {
            led_set_pixel(x, LED_ROWS - y - 1, (colour_t){0, 0, 4});
        }
    }

}

static void display_volume_history_update(uint32_t volume_history_len, float volume, float* volume_history, uint32_t* volume_history_idx)
{
    volume_history[(*volume_history_idx)] = volume;
    (*volume_history_idx)++;
    (*volume_history_idx) = (*volume_history_idx) < volume_history_len ? (*volume_history_idx) : 0U;
}

static float display_volume_history_scale(uint32_t volume_history_len, float volume, const float* volume_history, float volume_range_min)
{
    float volume_min = 3e38f;
    float volume_max = -3e38f;
    float volume_range = 1;
    float volume_scaled = 0;

    for (uint32_t i = 0; i < volume_history_len; i++)
    {
        if (volume_history[i] < volume_min)
        {
            volume_min = volume_history[i];
        }

        if (volume_history[i] > volume_max)
        {
            volume_max = volume_history[i];
        }
    }

    volume_range = fabsf(volume_max - volume_min);
    volume_range = volume_range > volume_range_min ? volume_range : volume_range_min;

    // Scale the volume so it fits within the range (0 <= volume < 1).
    volume_scaled = (volume - volume_min) / volume_range;
    volume_scaled = volume_scaled > 0                   ? volume_scaled : 0;
    volume_scaled = volume_scaled < display_less_than_1 ? volume_scaled : display_less_than_1;

    return volume_scaled;
}

// Lachlans Test!
static void display_mode_lachlan(context_t* context){
    led_clear();
    
    // lens 1
    draw_circle(2, 3, 2, (colour_t){10, 0, 0});

    // lens 2
    draw_circle(7, 3, 2, (colour_t){10, 0, 0});
}

// Plot 4 points
static void plot_4_points(uint32_t x_coord, uint32_t y_coord, uint32_t x, uint32_t y, colour_t c){
    led_set_pixel(x_coord+x, y_coord+y, c);

    if (x != 0){
        led_set_pixel(x_coord-x, y_coord+y, c);
        led_set_pixel(x_coord+x, y_coord-y, c);
    }
    
    led_set_pixel(x_coord-x, y_coord-y, c);
}

// Plot 8 points
static void plot_8_points(uint32_t x_coord, uint32_t y_coord, uint32_t x, uint32_t y, colour_t c){
    plot_4_points(x_coord, y_coord, x, y, c);

    if (x != y){
        plot_4_points(x_coord, y_coord, y, x, c);
    }
}

// Draw circle  
static void draw_circle(uint32_t x_coord, uint32_t y_coord, uint32_t rad, colour_t c){
    int32_t error = -rad;
    int32_t x = rad;
    int32_t y = 0;

    while (x >= y){
        plot_8_points(x_coord, y_coord, x, y, c);
        error += y;
        ++y;
        error += y;

        if (error >= 0){
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