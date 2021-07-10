#include <math.h>
#include <stdint.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>

#include "fft.h"
#include "mic.h"
#include "system.h"
#include "util.h"

#define MIC_GPIO_MODE      GPIO_MODE_AF
#define MIC_GPIO_PULL      GPIO_PUPD_NONE
#define MIC_GPIO_AF        GPIO_AF0
#define MIC_GPIO_CLK_TYPE  GPIO_OTYPE_PP
#define MIC_GPIO_CLK_SPEED GPIO_OSPEED_50MHZ
// sysclk == 53.333MHz, sysclk / 32 == 1.666MHz
#define MIC_SPI_DIV        SPI_CR1_BAUDRATE_FPCLK_DIV_32
#define MIC_SPI_POL        SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE
#define MIC_SPI_PHA        SPI_CR1_CPHA_CLK_TRANSITION_2
#define MIC_SPI_END        SPI_CR1_LSBFIRST
#define MIC_SPI_SIZE       SPI_CR2_DS_16BIT

#define MIC_DECIMATION_RATE       (144U)
#define MIC_FFT_LEN               (256U)
#define MIC_FFT_LOG2LEN           (8U)
#define MIC_DECIMATION_FILTER_LEN (480U)

/*
The FIR filter was designed with the following constraints in mind:
    - each individual element must be able to fit into a uint8_t
    - the sum all the elements must be able to fit into a uint16_t
    - the -20dB frequency should be fairly low (~5kHz)
    - if possible pick filter size and decimation rate that are factors of 16
The resulting filter has 480 taps and a 20dB cutoff frequency of 5.493kHz.
Therefore the target frequency after decimation is 10.99kHz and with a sample
frequency of 1.666MHz results is a decimation rate of 144 which leaves the
actual frequency after decimation at 11.57kHz.

FIR filter can be generated with the following python script:
from scipy import signal
import numpy as np
fs = (16e6 * 10 / 3) / 32
fir = signal.firwin2(480, (0, 200, 1000, fs/2), (1,1,0,0), fs=fs)
mic_dec_filt = np.array(((1<<16) / sum(fir)) * fir, dtype=int)
*/
static const uint8_t mic_dec_filt[MIC_DECIMATION_FILTER_LEN] = {
     20,  20,  20,  20,  20,  20,  20,  20,
     20,  21,  21,  21,  21,  21,  22,  22,
     22,  23,  23,  23,  24,  24,  25,  25,
     25,  26,  26,  27,  28,  28,  29,  29,
     30,  31,  31,  32,  33,  33,  34,  35,
     35,  36,  37,  38,  39,  39,  40,  41,
     42,  43,  44,  45,  46,  47,  48,  49,
     50,  51,  52,  53,  54,  55,  56,  57,
     58,  60,  61,  62,  63,  64,  66,  67,
     68,  69,  70,  72,  73,  74,  76,  77,
     78,  80,  81,  82,  84,  85,  86,  88,
     89,  91,  92,  93,  95,  96,  98,  99,
    101, 102, 103, 105, 106, 108, 109, 111,
    112, 114, 115, 117, 118, 120, 121, 123,
    124, 126, 127, 129, 131, 132, 134, 135,
    137, 138, 140, 141, 143, 144, 146, 147,
    149, 150, 152, 153, 155, 156, 158, 159,
    161, 162, 164, 165, 167, 168, 170, 171,
    173, 174, 176, 177, 178, 180, 181, 183,
    184, 185, 187, 188, 190, 191, 192, 194,
    195, 196, 198, 199, 200, 201, 203, 204,
    205, 206, 208, 209, 210, 211, 212, 214,
    215, 216, 217, 218, 219, 220, 221, 222,
    223, 224, 225, 226, 227, 228, 229, 230,
    231, 232, 233, 233, 234, 235, 236, 237,
    237, 238, 239, 240, 240, 241, 242, 242,
    243, 244, 244, 245, 245, 246, 246, 247,
    247, 248, 248, 249, 249, 249, 250, 250,
    250, 251, 251, 251, 251, 252, 252, 252,
    252, 252, 252, 253, 253, 253, 253, 253,
    253, 253, 253, 253, 253, 252, 252, 252,
    252, 252, 252, 251, 251, 251, 251, 250,
    250, 250, 249, 249, 249, 248, 248, 247,
    247, 246, 246, 245, 245, 244, 244, 243,
    242, 242, 241, 240, 240, 239, 238, 237,
    237, 236, 235, 234, 233, 233, 232, 231,
    230, 229, 228, 227, 226, 225, 224, 223,
    222, 221, 220, 219, 218, 217, 216, 215,
    214, 212, 211, 210, 209, 208, 206, 205,
    204, 203, 201, 200, 199, 198, 196, 195,
    194, 192, 191, 190, 188, 187, 185, 184,
    183, 181, 180, 178, 177, 176, 174, 173,
    171, 170, 168, 167, 165, 164, 162, 161,
    159, 158, 156, 155, 153, 152, 150, 149,
    147, 146, 144, 143, 141, 140, 138, 137,
    135, 134, 132, 131, 129, 127, 126, 124,
    123, 121, 120, 118, 117, 115, 114, 112,
    111, 109, 108, 106, 105, 103, 102, 101,
     99,  98,  96,  95,  93,  92,  91,  89,
     88,  86,  85,  84,  82,  81,  80,  78,
     77,  76,  74,  73,  72,  70,  69,  68,
     67,  66,  64,  63,  62,  61,  60,  58,
     57,  56,  55,  54,  53,  52,  51,  50,
     49,  48,  47,  46,  45,  44,  43,  42,
     41,  40,  39,  39,  38,  37,  36,  35,
     35,  34,  33,  33,  32,  31,  31,  30,
     29,  29,  28,  28,  27,  26,  26,  25,
     25,  25,  24,  24,  23,  23,  23,  22,
     22,  22,  21,  21,  21,  21,  21,  20,
     20,  20,  20,  20,  20,  20,  20,  20
};

// look up table for sum of 1's in uint8_t
static const uint8_t mic_filt_box[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};

static volatile uint32_t mic_buffer_full = 0;
static volatile uint32_t mic_buffer_idx = 0;
static volatile complex_t mic_buffer[MIC_FFT_LEN] = {{0.0, 0.0}};
static enum mic_state_e
{
    MIC_STATE_INIT,
    MIC_STATE_RUN
} mic_state = MIC_STATE_INIT;

static void mic_run(context_t* context);
static void mic_decimate_simple(uint16_t signal);
//static void mic_decimate(uint16_t signal);

void mic_task(context_t* context)
{
    switch (mic_state)
    {
    case MIC_STATE_INIT:
        mic_init();
        break;
    case MIC_STATE_RUN:
        mic_run(context);
        break;
    }
}

void mic_init(void)
{
    rcc_periph_clock_enable(MIC_RCC_GPIO);
    gpio_mode_setup(MIC_GPIO_PORT, MIC_GPIO_MODE, MIC_GPIO_PULL, MIC_GPIO_PIN_CLK | MIC_GPIO_PIN_MISO);
    gpio_set_af(MIC_GPIO_PORT, MIC_GPIO_AF, MIC_GPIO_PIN_CLK | MIC_GPIO_PIN_MISO);
    gpio_set_output_options(MIC_GPIO_PORT, MIC_GPIO_CLK_TYPE, MIC_GPIO_CLK_SPEED, MIC_GPIO_PIN_CLK);

    rcc_periph_clock_enable(MIC_RCC_SPI);
    spi_reset(MIC_SPI);
    spi_init_master(MIC_SPI, MIC_SPI_DIV, MIC_SPI_POL, MIC_SPI_PHA, MIC_SPI_END);
    spi_set_receive_only_mode(MIC_SPI);
    //SPI_CR2(MIC_SPI) |= MIC_SPI_SIZE;
    //spi_enable_rx_buffer_not_empty_interrupt(MIC_SPI);

    // In master receive only mode, the SPI clock is always on when enabled.
    spi_enable(MIC_SPI);

    // SPH0641LM4H-1 takes 50 ms to power up and reach normal operation.
    double mic_warmup_start = system_get_sec();
    while(mic_warmup_start + 0.05 > system_get_sec());

    //nvic_enable_irq(NVIC_SPI1_IRQ);

    mic_state = MIC_STATE_RUN;
}

static void mic_run(context_t* context)
{
    static float max = 1.0;
    float amp = 0.0;

    if (mic_buffer_full)
    {
        fft(mic_buffer, MIC_FFT_LEN, MIC_FFT_LOG2LEN);
        for (uint32_t i = 0; i < LED_COUNT; i++)
        {
            amp = sqrtf(mic_buffer[i].real * mic_buffer[i].real + mic_buffer[i].imag * mic_buffer[i].imag);
            if (amp > max)
            {
                max = amp;
            }
            context->mic_fft[i] = (uint8_t)(255.0 * amp / max);
        }
        for (uint32_t i = 0; i < MIC_FFT_LEN; i++)
        {
            mic_buffer[i].real = 0.0;
            mic_buffer[i].imag = 0.0;
        }
        mic_buffer_full = 0;
        spi_enable_rx_buffer_not_empty_interrupt(MIC_SPI);
    }
}

static void mic_decimate_simple(uint16_t signal)
{
    static uint32_t dec_sum_count = 0;

    while(signal)
    {
        signal &= signal - 1;
        mic_buffer[mic_buffer_idx].real++;
    }
    dec_sum_count++;
    if (dec_sum_count >= MIC_DECIMATION_RATE)
    {
        dec_sum_count = 0;
        mic_buffer_idx++;
        if (mic_buffer_idx >= MIC_FFT_LEN)
        {
            mic_buffer_idx = 0;
            mic_buffer_full = 1;
        }
    }
}

// static void mic_decimate(uint16_t signal)
// {
//     static uint32_t dec_filt_idx[4] = {432, 288, 144, 0};
//     static uint16_t dec_filt_res[4] = {0};

//     uint32_t bit_idx = 0;
//     while(signal)
//     {
//         if (signal & 1)
//         {
//             for (uint32_t j = 0; j < 4; j++)
//             {
//                 dec_filt_idx[j] += bit_idx;
//                 if (dec_filt_idx[j] >= MIC_DECIMATION_FILTER_LEN)
//                 {
//                     dec_filt_idx[j] -= MIC_DECIMATION_FILTER_LEN;
//                     mic_fft_buffer[mic_fft_buffer_idx] = dec_filt_res[j];
//                     mic_fft_buffer_idx++;
//                     dec_filt_res[j] = 0;
//                 }

//                 dec_filt_res[j] += mic_dec_filt[dec_filt_idx[j]];
//             }

//             bit_idx = 0;
//         }

//         signal >>= 1;
//         bit_idx++;
//     }

//     if(bit_idx > 0)
//     {
//         for (uint32_t j = 0; j < 4; j++)
//         {
//             dec_filt_idx[j] += bit_idx;
//             if (dec_filt_idx[j] >= MIC_DECIMATION_FILTER_LEN)
//             {
//                 dec_filt_idx[j] -= MIC_DECIMATION_FILTER_LEN;
//                 mic_fft_buffer[mic_fft_buffer_idx] = dec_filt_res[j];
//                 mic_fft_buffer_idx++;
//             }
//         }
//     }
// }

void mic_test(void)
{
    static uint32_t buffer_idx = 0;
    static volatile uint8_t buffer[33000] = {};

    while (buffer_idx < 33000)
    {
        if (SPI_SR(MIC_SPI) & SPI_SR_RXNE)
        {
            buffer[buffer_idx++] = SPI_DR8(MIC_SPI);
        }
    }
    buffer_idx = 0;
}

void spi1_isr(void)
{
    static uint32_t dec_cnt = 0;

    uint8_t spi_mic_data;

    while (SPI_SR(MIC_SPI) & SPI_SR_RXNE)
    {
        spi_mic_data = SPI_DR8(MIC_SPI);
        mic_buffer[mic_buffer_idx].real += mic_filt_box[spi_mic_data];
        dec_cnt += 8;
        if (dec_cnt >= MIC_DECIMATION_RATE)
        {
            dec_cnt = 0;
            mic_buffer_idx++;
            if (mic_buffer_idx > MIC_FFT_LEN)
            {
                mic_buffer_idx = 0;
                mic_buffer_full = 1;
            }
        }

        //mic_decimate_simple(spi_mic_data);
    }

    // We can turn off the mic interrupt while we're waiting for something to be done with the
    // buffer.
    // TODO: maybe double/triple buffer the data at some stage so the main loop has time to
    //       process the data and we don't lose mic data.
    if (mic_buffer_full)
    {
        spi_disable_rx_buffer_not_empty_interrupt(MIC_SPI);
    }
}



// static uint8_t mic_sum_bits(uint16_t *buffer, uint16_t len)
// {
//     uint8_t sum = 0;
//     uint16_t tmp;
//     for (uint32_t i = 0; i < len; i++)
//     {
//         tmp = buffer[i];
//         for(; tmp; sum++)
//         {
//             tmp &= tmp - 1;
//         }
//     }
//     return sum;
// }

// uint8_t mic_read(uint8_t* min, uint8_t* max)
// {
//     uint16_t decimation_idx = 0;
//     uint16_t decimation_buffer[FILTER_SIZE_16B] = {0};
//     uint16_t sample_count = 0;
//     uint8_t signal_max = 0;
//     uint8_t signal_min = 0xFF;
    
//     if (SPI_SR(MIC_SPI) & SPI_SR_OVR)
//     {
//         spi_read(MIC_SPI);
//     }
    
//     for (decimation_idx = 0; decimation_idx < FILTER_SIZE_16B; decimation_idx++)
//     {
//         decimation_buffer[decimation_idx] = spi_read(MIC_SPI);
//     }
//     decimation_idx = 0;

//     for (uint16_t signal_idx = 0; signal_idx < SIGNAL_SIZE; signal_idx++)
//     {
//         sample_count = 0;
//         while (sample_count < DECIMATION_RATE_16B)
//         {
//             decimation_buffer[decimation_idx] = spi_read(MIC_SPI);
//             decimation_idx++;
//             decimation_idx = decimation_idx < 8 ? decimation_idx : 0;
//             sample_count++;
//         }
        
//         signal[signal_idx] = mic_sum_bits(decimation_buffer, FILTER_SIZE_16B);

//         if (signal[signal_idx] < signal_min)
//         {
//             signal_min = signal[signal_idx];
//         }
//         if (signal[signal_idx] > signal_max)
//         {
//             signal_max = signal[signal_idx];
//         }
//     }
    
//     (*min) = signal_min;
//     (*max) = signal_max;
//     return signal_max - signal_min;
// }