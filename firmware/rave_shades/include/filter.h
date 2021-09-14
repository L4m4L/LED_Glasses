/* This module takes a PDM signal one byte at a time and performs decimation and filtering in
** order to remove noise introduced by the discretisation of the orginal analog signal and bring
** the sample frequency down to something more workable.
**
** Signal decimation by a factor of m usually goes as follows:
**     1. Low-pass filter below half the target sample frequency.
**     2. Discard every m-1 samples.
** Where the low pass filtering is performed by some digital filter (IIR or FIR).
**
** In our particular case, the original sample rate of the microphone is 1.333MHz and if we wanted
** to extract frequency components with, lets say, a 40Hz granularity we'd need an FFT size of at
** least 33000 which is not possible given our processing/memory constraints. Obviously we need to
** downsample our signal, by A LOT! The highest frequency we realistically need to extract for
** creating a music reactive display is around 3kHz. Lets make the filtering a little easier and
** choose a cutoff frequency at around 6.5kHz (0.004*fs) with a new sample frequency of a bit
** over 13kHz (keep only every 130th sample). This is still prohibitively resource intensive,
** requiring a ~500 tap digital filter (think ~500 float multiplications at 1.333MHz) to get even
** close to a 0.004*fs cutoff LPF. However with a few DSP shortcuts we can solve our seemingly
** insurmountable problem:
**     1. [a] explains how we can downsample before we filter effectively reducing our workload
**        by ~130 right off the bat.
**     2. We can exploit the fact that our microphone data encodes only a single bit per sample
**        by splitting our ~500 tap filter into groups of 8. We can then precompute the
**        sum(filter_coef_n * signal_input_n) results for every possible 8bit input. Reducing the
**        FIR filter to a series of table lookups and additions (no expensive multiplications and
**        additions reduced by a factor of 8).
**     3. And lastly we can scale and discretise our filter coefficients so that the results of
**        each group of 8 taps (the values we store in our lookup table) will not exceed what can
**        fit in an 8-bit value and thus reduce our storage requirements for our lookup table!
**
** [a] http://www.ws.binghamton.edu/fowler/fowler%20personal%20page/EE521_files/IV-05%20Polyphase%20FIlters%20Revised.pdf
**     Note that polyphase filters only work for FIR filters, something about FIR filters having a
**     finite history making it easy to skip calculations for filter outputs that were going to be
**     discarded anyways.
**
** Note that the below functions are declared "static inline" which means the compiler will copy
** paste their contents whereever they're called. Meaning that there is no function call overhead
** and the code is executed "inline". This, unsurprisingly, saves quite a bit of time since these
** functions are called so frequency in the microphone interrupt.
**
** TODO: the init function doesn't need to be inline.
** TODO: provide python example for generating LUT.
*/

#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

#define FILTER_TAPS   (480U)
#define FILTER_TAPS8  (FILTER_TAPS / 8)
#define FILTER_DEC    (120U)
#define FILTER_DEC8   (FILTER_DEC / 8)
#define FILTER_STAGES (FILTER_TAPS / FILTER_DEC)

typedef struct filter_instance_t_
{
    uint8_t  warm;
    uint8_t  step[FILTER_STAGES];
    int16_t result[FILTER_STAGES];
} filter_instance_t;

typedef struct filter_result_t_
{
    uint8_t  ready;
    int16_t result;
} filter_result_t;

extern const int8_t filter_lookup_table[FILTER_TAPS8][256];

// Initialises the filter structure.
static inline void filter_init(filter_instance_t* instance)
{
    instance->warm = 0;
    for (uint32_t i = 0; i < FILTER_STAGES; i++)
    {
        instance->step[i]   = (uint8_t)((FILTER_TAPS8 - i * FILTER_DEC8) % FILTER_TAPS8);
        instance->result[i] = 0;
    }
}

// Takes the next 8 samples, performs a table lookup and updates the filter instance.
// \param[in]  instace  The filter instance.
// \param[in]  input    Eight 1-bit microphone samples.
// \return              The ready flag will be set if there is a valid filter result ready.
static inline filter_result_t filter_apply(filter_instance_t* instance, uint8_t input)
{
    filter_result_t retval = {
        .ready  = 0,
        .result = 0
    };

    for (uint32_t i = 0; i < FILTER_STAGES; i++)
    {
        instance->result[i] += (int16_t)(filter_lookup_table[instance->step[i]++][input]);
        if (instance->step[i] == FILTER_TAPS8)
        {
            instance->step[i] = 0;
            if (instance->warm)
            {
                retval.ready  = 1;
                retval.result = instance->result[i];
            }
            else if (i == FILTER_STAGES - 1)
            {
                instance->warm = 1;
            }
            instance->result[i] = 0;
        }
    }

    return retval;
}

#endif // #ifndef FILTER_H