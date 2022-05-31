#include <math.h>

#include "fixed.h"
#include "goertzel.h"

void goertzel_init(const uint32_t *target_indices, uint32_t target_indices_len, uint32_t transform_len, goertzel_cache_t *cache_array)
{
    float frequency;

    for (uint32_t i = 0; i < target_indices_len; i++)
    {
        frequency = (2.0f * (float)(M_PI) * (float)(target_indices[i])) / (float)(transform_len);

        cache_array[i].a = fixed_from_float(2 * cosf(frequency));
        cache_array[i].b = fixed_from_float(cosf(frequency));
        cache_array[i].c = fixed_from_float(-sinf(frequency));
    }
}

void goertzel_transform(const fixed_t *data, uint32_t data_len, goertzel_cache_t cache, fixed_t *real, fixed_t *imag)
{
    fixed_t s0 = 0;
    fixed_t s1 = 0;
    fixed_t s2 = 0;

    for (uint32_t i = 0; i < data_len; i++)
    {
        s0 = fixed_mul(s1, cache.a);
        s0 = fixed_sub(s0, s2);
        s0 = fixed_add(s0, data[i]);

        s2 = s1;
        s1 = s0;
    }

    s0 = fixed_mul(s1, cache.a);
    s0 = fixed_sub(s0, s2);

    (*real) = fixed_sub(s0, fixed_mul(s1, cache.b));
    (*imag) = fixed_mul(fixed_neg(s1), cache.c);
}