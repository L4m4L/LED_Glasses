/**
 * In this display mode we're attempting to create a bit of a old-school stereo audio spectrum
 * display, each column representing the loudness of a particular frequency bin. we want to
 * decompose each new audio sample into our target frequencies and map that volume to the rows in
 * that particular column.
 *
 *
 *  . . . . *    . . . . .
 *  . . . . *    . . . . .
 *  . . . . *    . . . * .
 *  * . . * *    . . . * *
 *  * . . * *    * . . * *
 *  * * . * *    * . * * *
 *    * . *        . * *
 */
#include <math.h>

#include "display_spectrum.h"
#include "goertzel.h"
#include "led.h"
#include "util.h"

#define FREQUENCY_BIN_COUNT (10U)
#define RATE_LIMIT (0.055f)

/**
 * These frequency bins are spaced logarithmically and were chosen assuming
 * AUDIO_FREQUENCY_BIN_COUNT == 10 and AUDIO_BUFFER_LENGTH == 512. If these change the bins will
 * need to be updated.
 */
static const uint32_t frequency_bins[] = {
    2,  //   54Hz
    4,  //  108Hz
    7,  //  189Hz
    10, //  271Hz
    15, //  406Hz
    22, //  596Hz
    33, //  895Hz
    49, // 1329Hz
    74, // 2007Hz
    111 // 3010Hz
};
static const float volume_minimum[FREQUENCY_BIN_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static const float volume_scale[FREQUENCY_BIN_COUNT] = {8e-4, 3e-3, 8e-4, 8e-4, 5e-4, 5e-4, 3e-4, 3e-4, 1e-4, 7e-5};
//static const float volume_minimum[FREQUENCY_BIN_COUNT] = {-28, -28, -28, -28, -30, -31, -31, -31, -31, -31};
//static const float volume_scale[FREQUENCY_BIN_COUNT] = {10, 10, 8, 7, 10, 15, 15, 15, 15, 15};

// Precomputed parts of the Goertzel algorithm.
static goertzel_cache_t goertzel_coefs[FREQUENCY_BIN_COUNT];
static float volume_last[FREQUENCY_BIN_COUNT] = {0};
// Used to map floats to a [0, 1) range which is handy for mapping to a range of integers when
// you don't want to include the last number (i.e. when you have the length of an array).
static float display_less_than_1 = 0.99f;

void display_spectrum_init(void)
{
    display_less_than_1 = nextafterf(1, 0);
    goertzel_init(frequency_bins, FREQUENCY_BIN_COUNT, AUDIO_BUF_FILT_LEN, goertzel_coefs);
}

void display_spectrum_run(context_t *context)
{
    if (context->audio_updated > 0)
    {
        context->audio_updated = 0;
        led_clear();

        fixed_t spectrum_real[FREQUENCY_BIN_COUNT];
        fixed_t spectrum_imag[FREQUENCY_BIN_COUNT];

        for (uint32_t i = 0; i < FREQUENCY_BIN_COUNT; i++)
        {
            goertzel_transform(context->audio_filtered, AUDIO_BUF_FILT_LEN, goertzel_coefs[i], &spectrum_real[i], &spectrum_imag[i]);
        }

        for (uint32_t x = 0; x < FREQUENCY_BIN_COUNT; x++)
        {
            float real = fixed_to_float(spectrum_real[x]);
            float imag = fixed_to_float(spectrum_imag[x]);

            float volume = sqrtf(real * real + imag * imag);
            float volume_scaled = (volume / volume_scale[x]) - volume_minimum[x];

            volume_scaled = volume_scaled > 0 ? volume_scaled : 0;
            volume_scaled = volume_scaled < display_less_than_1 ? volume_scaled : display_less_than_1;

            float volume_rate_limited = volume_scaled;
            if (volume_rate_limited < volume_last[x] - RATE_LIMIT)
            {
                volume_rate_limited = volume_last[x] - RATE_LIMIT;
            }
            volume_last[x] = volume_rate_limited;

            uint32_t volume_row_height = (uint32_t)((float)(LED_ROWS + 1) * volume_rate_limited);
            uint32_t brightness = (volume_row_height / 2) + 1;

            for (uint32_t y = 0; y < volume_row_height; y++)
            {
                colour_t c = {0, 0, 0};

                switch (y)
                {
                case 0:
                    c = (colour_t){0, 0, 6};
                    break;
                case 1:
                    c = (colour_t){1, 0, 5};
                    break;
                case 2:
                    c = (colour_t){2, 0, 4};
                    break;
                case 3:
                    c = (colour_t){3, 0, 3};
                    break;
                case 4:
                    c = (colour_t){4, 0, 2};
                    break;
                case 5:
                    c = (colour_t){5, 0, 1};
                    break;
                case 6:
                    c = (colour_t){6, 0, 0};
                    break;
                }

                c.r *= brightness;
                c.g *= brightness;
                c.b *= brightness;

                led_set_pixel(x, LED_ROWS - y - 1, c);
            }
        }
    }
}