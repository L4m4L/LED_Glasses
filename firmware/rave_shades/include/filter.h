/**
 * This module takes PDM data and performs a lookup operation that corrects the DC bias, low pass
 * filters and downsamples it, resulting in audio output that we can actually work with.
 *
 * Digital signal decimation by a factor of m usually goes as follows:
 *     1. Low-pass filter below half the target sample frequency.
 *     2. Discard every m-1 samples.
 * Where the low pass filtering is performed by some digital filter (IIR or FIR).
 *
 * In our particular case, the original sample rate of the microphone is 1.333MHz. If we wanted to
 * extract frequency components with, for example, a 40Hz resolution we'd need an FFT size of at
 * least 33000 which would be unreasonably slow given our processing/memory constraints. Obviously
 * we need to downsample our signal, by A LOT!
 * The highest frequency we realistically need to extract for creating a music reactive display is
 * around 3kHz. Lets make the filtering a little easier and choose a cutoff frequency at around
 * 6.5kHz (0.004*fs) with a new sample frequency of a bit over 13kHz (keep only every 130th
 * sample). This is still prohibitively resource intensive, requiring a ~500 tap digital filter
 * (think ~500 float multiplications at 1.333MHz) to get even close to a 0.004*fs cutoff LPF.
 * However with a few DSP shortcuts we can solve our seemingly insurmountable problem:
 *  1. [a] explains how we can downsample before we filter effectively reducing our workload
 *     by ~130 right off the bat.
 *  2. We can exploit the fact that our microphone data encodes only a single bit per sample
 *     by splitting our ~500 tap filter into groups of 8. We can then precompute the
 *     sum(filter_coef_n * signal_input_n) results for every possible 8bit input. Reducing the
 *     FIR filter to a series of table lookups and additions (no expensive multiplications and
 *     additions reduced by a factor of 8).
 *  3. And lastly we can scale and discretise our filter coefficients so that the results of
 *     each group of 8 taps (the values we store in our lookup table) will not exceed what can
 *     fit in an 8-bit/16-bit value and thus reduce our storage requirements for our lookup table!
 *
 * [a] http://www.ws.binghamton.edu/fowler/fowler%20personal%20page/EE521_files/IV-05%20Polyphase%20FIlters%20Revised.pdf
 *     Note that polyphase filters only work for FIR filters, something about FIR filters having a
 *     finite history making it easy to skip calculations for filter outputs that were going to be
 *     discarded anyways.
 *
 * TODO: Point to python script that generates the lookup table.
 */

#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

#include "fixed.h"
#include "util.h"

/**
 * By using the const keyword we can store the lookup table in flash however this makes reading
 * from the table marginally slower than RAM. If there is an abundance of RAM space we can remove
 * the const keyword and store the LUT in RAM for a little extra speed.
 */
#define FILTER_LUT_STORAGE const /** Use this if you're low on RAM. */
//#define FILTER_LUT_STORAGE /** Use this when you're not. */

/**
 * Allowing 16bit values in the LUT gives a much better SNR to our audio at the cost of flash
 * usage. Keep in mind the audio output values will be different between these two options.
 */
//#define FILTER_LUT_BITDEPTH 8
#define FILTER_LUT_BITDEPTH 16

#if (FILTER_LUT_BITDEPTH == 8)
#define FILTER_LUT_TYPE int8_t
#elif (FILTER_LUT_BITDEPTH == 16)
#define FILTER_LUT_TYPE int16_t
#endif

typedef struct filter_instance_t_
{
    uint32_t step[FILTER_STAGES];
    fixed_t result[FILTER_STAGES];
} filter_instance_t;

extern FILTER_LUT_STORAGE FILTER_LUT_TYPE filter_lookup_table[FILTER_STEPS][256];

/** Populates instance with reasonable starting values. */
void filter_init(filter_instance_t *instance);

/**
 * Passes PDM data through an optimised LUT routine that downsamples, lowpass filters and DC
 * biases audio data. This function benefits from infrequent calls on large chunks of data to keep
 * overhead to a minimum. The filter instance is used to preserve the internal state of the filter
 * between subsequent chunks of audio data.
 * \param[in]     data_in       An array of PDM data to process.
 * \param[in]     data_in_len   The size of data_in.
 * \param[out]    data_out      An array to write outputs into.
 * \param[in]     data_out_len  The size of data_out.
 * \param[inout]  instance      A structure used to carry filter state between calls.
 * \retval  The number of elements put into the data_out array.
 */
uint32_t filter_apply(uint8_t *data_in, uint32_t data_in_len, fixed_t *data_out, uint32_t data_out_len, filter_instance_t *instance);

#endif /** FILTER_H */