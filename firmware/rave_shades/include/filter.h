#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

#define FILTER_TAPS        (480U)
#define FILTER_TAPS8       (FILTER_TAPS / 8)
#define FILTER_DEC         (120U)
#define FILTER_DEC8        (FILTER_DEC / 8)
#define FILTER_STAGES      (FILTER_TAPS / FILTER_DEC)

typedef struct filter_instance_t_
{
    uint8_t  warm;
    uint8_t  step[FILTER_STAGES];
    uint16_t result[FILTER_STAGES];
} filter_instance_t;

typedef struct filter_result_t_
{
    uint8_t  ready;
    uint16_t result;
} filter_result_t;

extern const uint8_t filter_lookup_table[FILTER_TAPS8][256];

static inline void filter_init(filter_instance_t* instance)
{
    instance->warm = 0;
    for (uint32_t i = 0; i < FILTER_STAGES; i++)
    {
        instance->step[i]   = (FILTER_TAPS8 - i * FILTER_DEC8) % FILTER_TAPS8;
        instance->result[i] = 0;
    }
}

static inline filter_result_t filter_apply(filter_instance_t* instance, uint8_t input)
{
    filter_result_t retval = {
        .ready  = 0,
        .result = 0
    };

    for (uint32_t i = 0; i < FILTER_STAGES; i++)
    {
        instance->result[i] += filter_lookup_table[instance->step[i]++][input];
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