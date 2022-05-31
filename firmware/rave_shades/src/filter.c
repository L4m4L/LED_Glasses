#include <stdint.h>

#include "filter.h"
#include "fixed.h"
#include "util.h"

void filter_init(filter_instance_t *instance)
{
    for (uint32_t i = 0; i < FILTER_STAGES; i++)
    {
        instance->step[i] = (FILTER_STEPS - (i * FILTER_BYTEWISE_DEC)) % FILTER_STEPS;
        instance->result[i] = 0;
    }
}

#if (FILTER_STAGES == 4U)
uint32_t filter_apply(uint8_t *data_in, uint32_t data_in_len, fixed_t *data_out, uint32_t data_out_len, filter_instance_t *instance)
{
    uint32_t data_in_idx;
    uint32_t data_out_idx;
    uint8_t data0, data1, data2, data3, data4;
    uint32_t step0, step1, step2, step3;
    fixed_t result0, result1, result2, result3;

    data_in_idx = 0;
    data_out_idx = 0;
    step0 = instance->step[0];
    step1 = instance->step[1];
    step2 = instance->step[2];
    step3 = instance->step[3];
    result0 = instance->result[0];
    result1 = instance->result[1];
    result2 = instance->result[2];
    result3 = instance->result[3];

    while ((data_in_idx + 4 < data_in_len) && (data_out_idx < data_out_len))
    {
        data0 = data_in[data_in_idx];
        data1 = data_in[data_in_idx + 1];
        data2 = data_in[data_in_idx + 2];
        data3 = data_in[data_in_idx + 3];
        data4 = data_in[data_in_idx + 4];
        data_in_idx += 5;

        result0 += filter_lookup_table[step0][data0] +
                   filter_lookup_table[step0 + 1][data1] +
                   filter_lookup_table[step0 + 2][data2] +
                   filter_lookup_table[step0 + 3][data3] +
                   filter_lookup_table[step0 + 4][data4];

        result1 += filter_lookup_table[step1][data0] +
                   filter_lookup_table[step1 + 1][data1] +
                   filter_lookup_table[step1 + 2][data2] +
                   filter_lookup_table[step1 + 3][data3] +
                   filter_lookup_table[step1 + 4][data4];

        result2 += filter_lookup_table[step2][data0] +
                   filter_lookup_table[step2 + 1][data1] +
                   filter_lookup_table[step2 + 2][data2] +
                   filter_lookup_table[step2 + 3][data3] +
                   filter_lookup_table[step2 + 4][data4];

        result3 += filter_lookup_table[step3][data0] +
                   filter_lookup_table[step3 + 1][data1] +
                   filter_lookup_table[step3 + 2][data2] +
                   filter_lookup_table[step3 + 3][data3] +
                   filter_lookup_table[step3 + 4][data4];

        step0 += 5;
        step1 += 5;
        step2 += 5;
        step3 += 5;

        if (step0 == FILTER_STEPS)
        {
            data_out[data_out_idx++] = result0;
            step0 = 0;
            result0 = 0;
        }
        if (step1 == FILTER_STEPS)
        {
            data_out[data_out_idx++] = result1;
            step1 = 0;
            result1 = 0;
        }
        if (step2 == FILTER_STEPS)
        {
            data_out[data_out_idx++] = result2;
            step2 = 0;
            result2 = 0;
        }
        if (step3 == FILTER_STEPS)
        {
            data_out[data_out_idx++] = result3;
            step3 = 0;
            result3 = 0;
        }
    }

    instance->step[0] = step0;
    instance->step[1] = step1;
    instance->step[2] = step2;
    instance->step[3] = step3;
    instance->result[0] = result0;
    instance->result[1] = result1;
    instance->result[2] = result2;
    instance->result[3] = result3;

    return data_out_idx;
}
#else
#error No filter_apply function available for the chosen FILTER_TAPS:FILTER_DEC ratio.
#endif /** FILTER_STAGES */