#include <math.h>

#include "fft.h"

static complex_t cadd(complex_t left, complex_t right)
{
	return (complex_t){left.real + right.real, left.imag + right.imag};
}

static complex_t csub(complex_t left, complex_t right)
{
	return (complex_t){left.real - right.real, left.imag - right.imag};
}

static complex_t cmult(complex_t left, complex_t right)
{
	return (complex_t){left.real * right.real - left.imag * right.imag,
				       left.real * right.imag + left.imag * right.real};
}

static complex_t twiddle(uint32_t k, uint32_t N)
{
	float x = -2 * (float)(M_PI) * k / N;
	return (complex_t){cosf(x), sinf(x)};
}

static uint32_t reverse(uint32_t original, uint32_t bits)
{
    uint32_t reversed = 0;
	while (bits--)
	{
		reversed = (reversed << 1) | (original & 1);
		original >>= 1;
	}
	return reversed;
}

void fft(volatile complex_t* result, uint32_t len, uint32_t bits)
{
    uint32_t rev, m;
    complex_t tmp, even, odd;

    for (uint32_t i = 0; i < len / 2; i++)
    {
        tmp = result[i];
        rev = reverse(i, bits);
        result[i] = result[rev];
        result[rev] = tmp;
    }

    for (uint32_t s = 1; s < bits + 1; s++)
    {
        m = 1 << s;
        for (uint32_t k = 0; k < len; k += m)
        {
            for (uint32_t j = 0; j < m / 2; j++)
            {
                even = result[k + j];
                odd  = cmult(result[k + j + m/2], twiddle(j, m));
				result[k + j]       = cadd(even, odd);
				result[k + j + m/2] = csub(even, odd);
            }
        }
    }
}