#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

#include "mic.h"
#include "system.h"

#define FILTER_SIZE     (1 << 7)
#define DECIMATION_RATE (32)

#define FILTER_SIZE_16B     (FILTER_SIZE / 16)
#define DECIMATION_RATE_16B (DECIMATION_RATE / 16)

#define SIGNAL_SIZE (512)

static volatile uint8_t signal[SIGNAL_SIZE] = {0};

static uint8_t mic_sum_bits(uint16_t *buffer, uint16_t len);

void mic_init(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO6);
    gpio_set_af(GPIOA, GPIO_AF0, GPIO5 | GPIO6);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO5);

    rcc_periph_clock_enable(RCC_SPI1);
    spi_reset(SPI1);
    spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_16, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE, SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_MSBFIRST);
    spi_set_receive_only_mode(SPI1);
    SPI_CR2(SPI2) |= SPI_CR2_DS_16BIT;

    // In master receive only mode, the SPI clock is always on when enabled.
    spi_enable(SPI1);

    // SPH0641LM4H-1 takes 50 ms to power up and reach normal operation.
    double mic_warmup_start = system_get_sec();
    while(mic_warmup_start + 0.015 > system_get_sec());
}

uint8_t mic_read(uint8_t* min, uint8_t* max)
{
    uint16_t decimation_idx = 0;
    uint16_t decimation_buffer[FILTER_SIZE_16B] = {0};
    uint16_t sample_count = 0;
    uint8_t signal_max = 0;
    uint8_t signal_min = 0xFF;
    
    if (SPI_SR(SPI1) & SPI_SR_OVR)
    {
        spi_read(SPI1);
    }
    
    for (decimation_idx = 0; decimation_idx < FILTER_SIZE_16B; decimation_idx++)
    {
        decimation_buffer[decimation_idx] = spi_read(SPI1);
    }
    decimation_idx = 0;

    for (uint16_t signal_idx = 0; signal_idx < SIGNAL_SIZE; signal_idx++)
    {
        sample_count = 0;
        while (sample_count < DECIMATION_RATE_16B)
        {
            decimation_buffer[decimation_idx] = spi_read(SPI1);
            decimation_idx++;
            decimation_idx = decimation_idx < 8 ? decimation_idx : 0;
            sample_count++;
        }
        
        signal[signal_idx] = mic_sum_bits(decimation_buffer, FILTER_SIZE_16B);

        if (signal[signal_idx] < signal_min)
        {
            signal_min = signal[signal_idx];
        }
        if (signal[signal_idx] > signal_max)
        {
            signal_max = signal[signal_idx];
        }
    }
    
    (*min) = signal_min;
    (*max) = signal_max;
    return signal_max - signal_min;
}

static uint8_t mic_sum_bits(uint16_t *buffer, uint16_t len)
{
    uint8_t sum = 0;
    uint16_t tmp;
    for (uint32_t i = 0; i < len; i++)
    {
        tmp = buffer[i];
        for(; tmp; sum++)
        {
            tmp &= tmp - 1;
        }
    }
    return sum;
}