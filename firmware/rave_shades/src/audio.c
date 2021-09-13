#include <math.h>

#include "audio.h"
#include "filter.h"
#include "fixed.h"
#include "mic.h"
#include "system.h"
#include "util.h"

// If we were using FFT this would be the length of the FFT. At 512, it collects 37ms worth of
// audio and the Goertzel algorithm returns frequency bins spaced roughly 27Hz apart.
#define AUDIO_BUFFER_LENGTH (512U)

// These frequency bins are spaced logarithmically and were chosen assuming
// AUDIO_FREQUENCY_BIN_COUNT == 10 and AUDIO_BUFFER_LENGTH == 512. If these change the bins will
// need to be updated.
static const uint32_t audio_frequency_bins[AUDIO_FREQUENCY_BIN_COUNT] = {
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

static volatile uint32_t audio_buffer_idx = 0;
static volatile fixed_t audio_buffer[AUDIO_BUFFER_LENGTH];
static filter_instance_t audio_filter_instance;
// Precomputed parts of the Goertzel algorithm.
static fixed_t audio_goertzel_coef_a[AUDIO_FREQUENCY_BIN_COUNT];
static fixed_t audio_goertzel_coef_b[AUDIO_FREQUENCY_BIN_COUNT];
static fixed_t audio_goertzel_coef_c[AUDIO_FREQUENCY_BIN_COUNT];

// Filters and downsamples the audio data. This is passed to the mic module to be called in the
// SPI interrupt.
// \param[in]  audio_data  Raw PDM microphone reading.
static void audio_receive_callback(uint16_t audio_data);
// Precomputes parts of the Goertzel algorithm.
static void audio_goertzel_init(void);
// Runs the actual Goertzel algorithm (https://en.wikipedia.org/wiki/Goertzel_algorithm).
// \param[in]   sample  The data over which to run the algorithm.
// \param[in]   bin     The frequency bin we want.
// \param[out]  real    The real component of the result.
// \param[out]  imag    The imaginary component of the result.
static void audio_goertzel(volatile fixed_t* sample, uint32_t bin, fixed_t* real, fixed_t* imag);

void audio_init(void)
{
    audio_goertzel_init();
    filter_init(&audio_filter_instance);
    mic_init(audio_receive_callback);

    mic_enable();
}

void audio_run(context_t* context)
{
    fixed_t real_fixed, imag_fixed;
    float real, imag;

    if (audio_buffer_idx + 1 >= AUDIO_BUFFER_LENGTH)
    {
        audio_buffer_idx = 0;
        context->audio_updated = 1;
        
        for (uint32_t i = 0; i < AUDIO_FREQUENCY_BIN_COUNT; i++)
        {
            audio_goertzel(audio_buffer, i, &real_fixed, &imag_fixed);

            real = fixed_to_float(real_fixed);
            imag = fixed_to_float(imag_fixed);

            context->audio_volume[i] = sqrtf(real * real + imag * imag);
        }

        // Don't forget to renable the mic interrupt.
        mic_enable();
    }
}

static void audio_receive_callback(uint16_t audio_data)
{
    filter_result_t audio_filter_result;
    
    if (audio_buffer_idx + 1 < AUDIO_BUFFER_LENGTH)
    {
        audio_filter_result = filter_apply(&audio_filter_instance, (uint8_t)(audio_data));
        if (audio_filter_result.ready)
        {
            audio_buffer[audio_buffer_idx++] = audio_filter_result.result;
        }

        audio_filter_result = filter_apply(&audio_filter_instance, (uint8_t)(audio_data >> 8));
        if (audio_filter_result.ready)
        {
            audio_buffer[audio_buffer_idx++] = audio_filter_result.result;
        }
    }
    else
    {
        // Turn off the microphone interrupt, we don't want to keep coming back in here if we're
        // not going to doing anything with the new data.
        mic_disable();
    }
}

static void audio_goertzel_init(void)
{
    float frequency;

    for (uint32_t i = 0; i < AUDIO_FREQUENCY_BIN_COUNT; i++)
    {
        // This frequency is really a ratio of the sample frequency (i.e. frequency = actual_frequency / sample_frequency).
        frequency = (2.0f * (float)(M_PI) * (float)(audio_frequency_bins[i])) / (float)(AUDIO_BUFFER_LENGTH);

        audio_goertzel_coef_a[i] = fixed_from_float(2 * cosf(frequency));
        audio_goertzel_coef_b[i] = fixed_from_float(cosf(frequency));
        audio_goertzel_coef_c[i] = fixed_from_float(-sinf(frequency));
    }
}

static void audio_goertzel(volatile fixed_t* sample, uint32_t bin, fixed_t* real, fixed_t* imag)
{
    fixed_t s0 = 0;
    fixed_t s1 = 0;
    fixed_t s2 = 0;

    for (uint32_t sample_idx = 0; sample_idx < AUDIO_BUFFER_LENGTH; sample_idx++)
    {
        s0 = fixed_mul(s1, audio_goertzel_coef_a[bin]);
        s0 = fixed_sub(s0, s2);
        s0 = fixed_add(s0, sample[sample_idx]);

        s2 = s1;
        s1 = s0;
    }

    s0 = fixed_mul(s1, audio_goertzel_coef_a[bin]);
    s0 = fixed_sub(s0, s2);

    (*real) = fixed_sub(s0, fixed_mul(s1, audio_goertzel_coef_b[bin]));
    (*imag) = fixed_mul(fixed_neg(s1), audio_goertzel_coef_c[bin]);
}