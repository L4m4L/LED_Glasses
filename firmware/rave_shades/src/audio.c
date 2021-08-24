#include <math.h>

#include "audio.h"
#include "filter.h"
#include "fixed.h"
#include "mic.h"
#include "system.h"
#include "util.h"

#define AUDIO_FREQUENCY_BINS_START (1U)
#define AUDIO_FREQUENCY_BINS_ALL (128U)

static volatile uint32_t audio_buffer_idx = 0;
static volatile fixed_t audio_buffer[AUDIO_FREQUENCY_BINS_ALL];
static filter_instance_t audio_filter_instance;
static fixed_t audio_goertzel_coef_a[AUDIO_FREQUENCY_BINS];
static fixed_t audio_goertzel_coef_b[AUDIO_FREQUENCY_BINS];
static fixed_t audio_goertzel_coef_c[AUDIO_FREQUENCY_BINS];
static float audio_vol_max = 1;

static void audio_receive_callback(uint8_t audio_data);
static void audio_goertzel_init(void);
static void audio_goertzel(volatile fixed_t* sample, uint32_t bin, fixed_t* real, fixed_t* imag);

void audio_init(void)
{
    audio_vol_max = nextafterf(1, 0);

    audio_goertzel_init();
    filter_init(&audio_filter_instance);
    mic_init(audio_receive_callback);

    mic_enable();
}

void audio_run(context_t* context)
{
    fixed_t real_fixed, imag_fixed;
    float real, imag;

    if (audio_buffer_idx == AUDIO_FREQUENCY_BINS_ALL)
    {
        audio_buffer_idx = 0;
        
        for (uint32_t freq_bin = 0; freq_bin < AUDIO_FREQUENCY_BINS; freq_bin++)
        {
            audio_goertzel(audio_buffer, freq_bin, &real_fixed, &imag_fixed);

            real = fixed_to_float(real_fixed);
            imag = fixed_to_float(imag_fixed);

            context->audio_volume[freq_bin] = sqrtf(real * real + imag * imag);
        }

        context->audio_updated = 1;
        mic_enable();
    }
}

static void audio_receive_callback(uint8_t audio_data)
{
    filter_result_t audio_filter_result;
    
    if (audio_buffer_idx < AUDIO_FREQUENCY_BINS_ALL)
    {
        audio_filter_result = filter_apply(&audio_filter_instance, audio_data);
        if (audio_filter_result.ready)
        {
            audio_buffer[audio_buffer_idx++] = audio_filter_result.result << 9;
        }
    }
    else
    {
        mic_disable();
    }
}

static void audio_goertzel_init(void)
{
    float frequency;

    for (uint32_t freq_bin = 0; freq_bin < AUDIO_FREQUENCY_BINS; freq_bin++)
    {
        // We don't actually care about the DC bin so we skip freq_bin == 0.
        frequency = (2.0f * (float)(M_PI) * (float)(freq_bin + AUDIO_FREQUENCY_BINS_START)) / (float)(AUDIO_FREQUENCY_BINS);

        audio_goertzel_coef_a[freq_bin] = fixed_from_float(2 * cosf(frequency));
        audio_goertzel_coef_b[freq_bin] = fixed_from_float(cosf(frequency));
        audio_goertzel_coef_c[freq_bin] = fixed_from_float(-sinf(frequency));
    }
}

static void audio_goertzel(volatile fixed_t* sample, uint32_t bin, fixed_t* real, fixed_t* imag)
{
    fixed_t s0 = 0;
    fixed_t s1 = 0;
    fixed_t s2 = 0;

    for (uint32_t sample_idx = 0; sample_idx < AUDIO_FREQUENCY_BINS_ALL; sample_idx++)
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