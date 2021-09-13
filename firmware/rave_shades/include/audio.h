/* This module manages reading raw PDM microphone data from the SPH0641LM4H-1, converting the PDM
** data (1s and 0s) to PCM (16bit numbers), lowpass filtering and downsampling the data and finally
** extracting the desired frequency components from the audio.
** The read-filter-downsample process is all handled in the SPI interrupt (don't worry it uses
** lookup tables so it's really fast!), and the frequency component extraction is done in
** audio_run().
** TODO: double buffer audio data
*/

#ifndef AUDIO_H
#define AUDIO_H

#include "util.h"

// Initialises the audio module. This must be called before audio_run().
void audio_init(void);
// Processes collected audio data, saving frequency components into context. Will only run if a
// full buffer of data is available, saving data into audio_volume and setting audio_updated.
// \param[in/out]  context  Global state structure.
void audio_run(context_t* context);

#endif // #ifndef AUDIO_H