// nextafterf() and sqrtf()
#include <math.h>

// FFT functions
#include "transform_functions.h"

#include "audio.h"
#include "filter.h"
#include "mic.h"

#define AUDIO_FFT_SIZE         (256U)
#define AUDIO_BASS_FFT_BINS    (4U)
#define AUDIO_BASS_WINDOW_SIZE (1U<<5)
#define AUDIO_BASS_RANGE_MIN   (18000U)
//#define AUDIO_BASS_RANGE_MIN   (15U)

static volatile uint32_t                   audio_fft_ready = 0;
static volatile uint32_t                   audio_fft_buffer_idx = 0;
static          float                      audio_fft_buffer[AUDIO_FFT_SIZE];
static          float                      audio_fft_result[AUDIO_FFT_SIZE];
static          arm_rfft_fast_instance_f32 audio_fft_instance;
static          filter_instance_t          audio_filter_instance;
static          uint32_t                   audio_bass_window_idx = 0;
static          float                      audio_bass_window[AUDIO_BASS_WINDOW_SIZE] = {0};
static          float                      audio_bass_max = 0;

static void audio_receive_callback(uint8_t audio_data);

void audio_init(void)
{
    mic_init(audio_receive_callback);
    filter_init(&audio_filter_instance);
    arm_rfft_fast_init_f32(&audio_fft_instance, AUDIO_FFT_SIZE);

    audio_bass_max = nextafterf(1, 0);
}

void audio_run(context_t* context)
{
    float bass, bass_min, bass_max, bass_range;

    if (audio_fft_ready)
    {
        arm_rfft_fast_f32(&audio_fft_instance, audio_fft_buffer, audio_fft_result, 0);

        bass = 0;
        for (uint32_t i = 1; i < AUDIO_BASS_FFT_BINS + 1; i++)
        {
            bass += sqrtf(audio_fft_result[2 * i]     * audio_fft_result[2 * i] +
                          audio_fft_result[2 * i + 1] * audio_fft_result[2 * i + 1]);
            // bass += log10f(audio_fft_result[2 * i]     * audio_fft_result[2 * i] +
            //                audio_fft_result[2 * i + 1] * audio_fft_result[2 * i + 1]);
        }

        audio_bass_window[audio_bass_window_idx++] = bass;
        audio_bass_window_idx &= AUDIO_BASS_WINDOW_SIZE - 1;

        bass_min = FLT_MAX;
        bass_max = 0;
        for (uint32_t i = 0; i < AUDIO_BASS_WINDOW_SIZE; i++)
        {
            if (audio_bass_window[i] < bass_min)
            {
                bass_min = audio_bass_window[i];
            }

            if (audio_bass_window[i] > bass_max)
            {
                bass_max = audio_bass_window[i];
            }
        }

        bass_range = bass_max - bass_min;
        bass_range = bass_range < AUDIO_BASS_RANGE_MIN ? AUDIO_BASS_RANGE_MIN : bass_range;

        context->bass = (bass - bass_min) / bass_range;
        context->bass = context->bass > audio_bass_max ? audio_bass_max : context->bass;

        audio_fft_ready = 0;
    }
}

void audio_receive_callback(uint8_t audio_data)
{
    filter_result_t audio_filter_result;
    
    if (audio_fft_ready == 0)
    {
        audio_filter_result = filter_apply(&audio_filter_instance, audio_data);
        if (audio_filter_result.ready)
        {
            audio_fft_buffer[audio_fft_buffer_idx++] = audio_filter_result.result;

            if (audio_fft_buffer_idx >= AUDIO_FFT_SIZE)
            {
                audio_fft_buffer_idx = 0;
                audio_fft_ready = 1;
            }
        }
    }
}