#include <stdint.h>

#include "audio.h"
#include "filter.h"
#include "mic.h"
#include "util.h"

/** Don't overdo it, these buffers are RAM hungry. Two should suffice. */
#define AUDIO_BUFFER_NUM 2U

typedef enum buffer_state_t_
{
    EMPTY,
    RECORDING,
    FULL,
    PROCESSING
} buffer_state_t;

static filter_instance_t audio_filter_instance;
static uint32_t audio_read = 0;
static volatile uint32_t audio_write = 0;
static volatile buffer_state_t audio_buffer_states[AUDIO_BUFFER_NUM] = {EMPTY};
static uint8_t audio_buffers[AUDIO_BUFFER_NUM][AUDIO_BUF_RAW_LEN] = {0};
static volatile uint32_t audio_overrun = 0;

void audio_recording_complete_callback(void);

void audio_init(void)
{
    filter_init(&audio_filter_instance);
    mic_init(audio_recording_complete_callback);
    audio_buffer_states[audio_write] = RECORDING;
    mic_record(audio_buffers[audio_write], AUDIO_BUF_RAW_LEN);
}

void audio_run(context_t *context)
{
    if (audio_buffer_states[audio_read] == FULL)
    {
        audio_buffer_states[audio_read] = PROCESSING;
        filter_apply(audio_buffers[audio_read], AUDIO_BUF_RAW_LEN, context->audio_filtered, AUDIO_BUF_FILT_LEN, &audio_filter_instance);
        audio_buffer_states[audio_read] = EMPTY;

        /**
         * If we haven't been processing audio data quickly enough it's possible that the
         * recordings have run out of buffers and stopped. We can check for this by looking at the
         * next buffer that needs to be written to and if it's the same as the one we just
         * processed that means it's caught up. In which case we'll want to restart the audio
         * recording and increment the audio overrun counter (this can be read during debug).
         */
        if (audio_write == audio_read)
        {
            audio_buffer_states[audio_write] = RECORDING;
            mic_record(audio_buffers[audio_write], AUDIO_BUF_RAW_LEN);

            audio_overrun++;
        }

        audio_read++;
        audio_read = audio_read < AUDIO_BUFFER_NUM ? audio_read : 0;

        context->audio_updated = 1;
    }
}

void audio_recording_complete_callback(void)
{
    /** A DMA transfer has just completed. The current write buffer is now full. */
    audio_buffer_states[audio_write] = FULL;

    /** Move to the next write buffer and begin the DMA transfer. */
    audio_write++;
    audio_write = audio_write < AUDIO_BUFFER_NUM ? audio_write : 0;

    if (audio_buffer_states[audio_write] == EMPTY)
    {
        audio_buffer_states[audio_write] = RECORDING;
        mic_record(audio_buffers[audio_write], AUDIO_BUF_RAW_LEN);
    }
}