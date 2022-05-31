/**
 * This module manages buffering PDM data from the microphone (mic.h), processing that data with
 * the audio filter (filter.h), and making the audio available to the rest of the program via the
 * context structure.
 *
 * It will start collecting audio on initialisation and will require "audio_run" to be called
 * frequently to keep up with the audio stream. Don't worry if it doesn't, the recording will stop
 * if it runs out of buffers but will be restarted on the next call to "audio_run".
 *
 * To manage a continuous stream of data, we use multiple audio buffers. This allows us to save new
 * data in a one buffer and process data from another. These buffers are quite large
 * (512 * 120 / 8 == 7680 bytes), their size being dependant on the decimation rate of the filter
 * and the size of the "audio_filtered" array in the context structure.
 *
 * TODO:
 *  - Implement a flushing mechanism so the audio stream can be paused/resumed without worrying
 *    about stale data.
 *  - Maybe have some sort of overlap option. Some % of the previous processed audio can be
 *    carried over, this should provide a smoother frequency decomposition.
 */

#ifndef AUDIO_H
#define AUDIO_H

#include "util.h"

/** Starts the audio recording. */
void audio_init(void);

/**
 * Filters any available PDM data, storing it in context and setting the "audio_updated" flag.
 * Will also restart recordings if it ran out of available buffers and paused.
 */
void audio_run(context_t *context);

#endif // AUDIO_H