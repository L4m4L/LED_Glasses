#ifndef FIXED_H
#define FIXED_H

#include <stdint.h>

//#define FIXED_CHECK
//#define FIXED_SIZE_16
#define FIXED_SIZE_32

#ifdef FIXED_SIZE_16
    #define FIXED_Q (10)

    typedef int32_t fixed_t;
    typedef int64_t fixed_big_t;

    static const fixed_t fixed_min = -32768;
    static const fixed_t fixed_max = 32767;
#endif // #ifdef FIXED_SIZE_16

#ifdef FIXED_SIZE_32
    #define FIXED_Q (29)

    typedef int32_t fixed_t;
    typedef int64_t fixed_big_t;

    static const fixed_t fixed_min = -2147483648;
    static const fixed_t fixed_max = 2147483647;
#endif // #ifdef FIXED_SIZE_32

static const float fixed_shift = 1 << FIXED_Q;

#ifdef FIXED_CHECK
static fixed_t fixed_clamp(fixed_big_t x)
{
    if (x < fixed_min)
    {
        x = fixed_min;
    }

    if (x > fixed_max)
    {
        x = fixed_max;
    }

    return (fixed_t)(x);
}
#endif // #ifdef FIXED_CHECK

static inline float fixed_to_float(fixed_t x)
{
	return (float)(x) / fixed_shift;
}

static inline fixed_t fixed_from_float(float x)
{
#ifdef FIXED_CHECK
	return fixed_clamp((fixed_big_t)(x * fixed_shift));
#else
    return (fixed_t)(x * fixed_shift);
#endif // #ifdef FIXED_CHECK
}

static inline fixed_t fixed_neg(fixed_t x)
{
    return x == fixed_min ? fixed_max : -x;
}

static inline fixed_t fixed_add(fixed_t x, fixed_t y)
{
#ifdef FIXED_CHECK
	return fixed_clamp((fixed_big_t)(x) + (fixed_big_t)(y));
#else
    return x + y;
#endif // #ifdef FIXED_CHECK
}

static inline fixed_t fixed_sub(fixed_t x, fixed_t y)
{
#ifdef FIXED_CHECK
	return fixed_clamp((fixed_big_t)(x) - (fixed_big_t)(y));
#else
    return x - y;
#endif // #ifdef FIXED_CHECK
}

static inline fixed_t fixed_mul(fixed_t x, fixed_t y)
{
#ifdef FIXED_CHECK
	return fixed_clamp(((fixed_big_t)(x) * (fixed_big_t)(y)) >> FIXED_Q);
#else
    return (fixed_t)(((fixed_big_t)(x) * (fixed_big_t)(y)) >> FIXED_Q);
#endif // #ifdef FIXED_CHECK
}

static inline fixed_t fixed_div(fixed_t x, fixed_t y)
{
#ifdef FIXED_CHECK
	return fixed_clamp(((fixed_big_t)(x) << FIXED_Q) / (fixed_big_t)(y));
#else
    return (fixed_t)(((fixed_big_t)(x) << FIXED_Q) / (fixed_big_t)(y));
#endif // #ifdef FIXED_CHECK
}

#endif // #ifndef FIXED_H