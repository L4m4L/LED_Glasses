/**
 * This module is based on the Goertzel algorithm linked below. It is particularly well suited to
 * applications where input data is real only and the entire frequency spectrum is not required.
 * Additionally it uses significantly less precomputed values and is much easier to implement than
 * an FFT.
 * This algorithm works best if you know what frequencies you're after ahead of time, which lets
 * you cache a number of the required calculations. Next you'll need to calculate what index of an
 * equivalent DFT output array corresponds to your chosen frequencies using the following formula:
 *
 *     index = target_frequency * number_of_audio_samples / sample_frequency
 *
 * For example if you have a sample frequency of 13.883 kHz, collect 512 samples at a time and are
 * interested in 406 Hz you'll want to use index 15 (you'll need to round to the nearest integer)
 * with the functions in this module.
 *
 * https://en.wikipedia.org/wiki/Goertzel_algorithm#Single_DFT_term_with_real-valued_arithmetic
 */

#ifndef GOERTZEL_H
#define GOERTZEL_H

#include <stdint.h>

#include "fixed.h"

typedef struct goertzel_cache_t_
{
    fixed_t a;
    fixed_t b;
    fixed_t c;
} goertzel_cache_t;

/**
 * This function precomputes some elements of the algorithm to speed things up.
 * \param[in]  target_indices      A list of the indices that relate to the target frequencies as
 *                                 explained above.
 * \param[in]  target_indices_len  The length of "target_indices" and "cache_array".
 * \param[in]  transform_len       The size of the audio buffer that you plan to use.
 * \param[out] cache_array         The precomputed values.
 */
void goertzel_init(const uint32_t *target_indices, uint32_t target_indices_len, uint32_t transform_len, goertzel_cache_t *cache_array);

/**
 * Applies the goertzel algorithm to the provided data, extracting the real and imaginary values
 * of the frequency component corresponding to the provided precomputed value.
 * \param[in]  data      An array of data. The length must be the same as used in the
 *                       "transform_len" argument of the "goertzel_init" function.
 * \param[in]  data_len  The length of "data".
 * \param[in]  cache     The precomputed value corresponding to the target frequency.
 * \param[out] real      The real value of the frequency component of the data.
 * \param[out] imag      The imaginary value of the frequency component of the data.
 */
void goertzel_transform(const fixed_t *data, uint32_t data_len, goertzel_cache_t cache, fixed_t *real, fixed_t *imag);

#endif /** GOERTZEL_H */