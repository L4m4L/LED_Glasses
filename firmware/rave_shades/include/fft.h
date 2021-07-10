#ifndef FFT_H
#define FFT_H

#include <stdint.h>

typedef struct complext_t
{
    float real;
    float imag;
} complex_t;

void fft(volatile complex_t* result, uint32_t len, uint32_t bits);

#endif // #ifndef FFT_H