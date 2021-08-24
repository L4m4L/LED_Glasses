#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>

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

void (*spi_rxne_callback)(uint8_t) = 0;

void mic_init(void (*mic_receive_callback)(uint8_t))
{
    spi_rxne_callback = mic_receive_callback;

    rcc_periph_clock_enable(MIC_RCC_GPIO);
    gpio_mode_setup(MIC_GPIO_PORT, MIC_GPIO_MODE, MIC_GPIO_PULL, MIC_GPIO_PIN_CLK | MIC_GPIO_PIN_MISO);
    gpio_set_af(MIC_GPIO_PORT, MIC_GPIO_AF, MIC_GPIO_PIN_CLK | MIC_GPIO_PIN_MISO);
    gpio_set_output_options(MIC_GPIO_PORT, MIC_GPIO_CLK_TYPE, MIC_GPIO_CLK_SPEED, MIC_GPIO_PIN_CLK);

    rcc_periph_clock_enable(MIC_RCC_SPI);
    spi_reset(MIC_SPI);
    spi_init_master(MIC_SPI, MIC_SPI_DIV, MIC_SPI_POL, MIC_SPI_PHA, MIC_SPI_END);
    spi_set_receive_only_mode(MIC_SPI);
    
    nvic_enable_irq(NVIC_SPI1_IRQ);

    // In master receive only mode, the SPI clock is always on when enabled.
    // SPH0641LM4H-1 takes 50 ms to power up and reach normal operation.
    // We enable the clock and leave it enabled so that the mic is always fully operational for
    // when we want to collect data.
    spi_enable(MIC_SPI);
}

double mic_get_sample_rate(void)
{
    uint32_t pclk_div = 1;
    
    switch (MIC_SPI_DIV)
    {
    case SPI_CR1_BAUDRATE_FPCLK_DIV_2:
        pclk_div = 2;
        break;
    case SPI_CR1_BAUDRATE_FPCLK_DIV_4:
        pclk_div = 4;
        break;
    case SPI_CR1_BAUDRATE_FPCLK_DIV_8:
        pclk_div = 8;
        break;
    case SPI_CR1_BAUDRATE_FPCLK_DIV_16:
        pclk_div = 16;
        break;
    case SPI_CR1_BAUDRATE_FPCLK_DIV_32:
        pclk_div = 32;
        break;
    case SPI_CR1_BAUDRATE_FPCLK_DIV_64:
        pclk_div = 64;
        break;
    case SPI_CR1_BAUDRATE_FPCLK_DIV_128:
        pclk_div = 128;
        break;
    case SPI_CR1_BAUDRATE_FPCLK_DIV_256:
        pclk_div = 256;
        break;
    default:
        pclk_div = 1;
        break;
    }

    return system_get_pclk() / pclk_div;
}

void mic_enable(void)
{
    // // Clear any possible pending RXNE interrupts by emptying the RX FIFO then clearing the NVIC
    // // pending interrupt.
    // while (SPI_SR(MIC_SPI) & SPI_SR_RXNE)
    // {
    //     // By casting to void we read the register without actually doing anything with it, and
    //     // the compiler doesn't optimise it away.
    //     (void)SPI_DR8(MIC_SPI);
    // }
    // nvic_clear_pending_irq(NVIC_SPI1_IRQ);
    // nvic_enable_irq(NVIC_SPI1_IRQ);
    spi_enable_rx_buffer_not_empty_interrupt(MIC_SPI);
}

void mic_disable(void)
{
    spi_disable_rx_buffer_not_empty_interrupt(MIC_SPI);
    // nvic_disable_irq(NVIC_SPI1_IRQ);
}

void spi1_isr(void)
{
    while (SPI_SR(MIC_SPI) & SPI_SR_RXNE)
    {
        if (spi_rxne_callback)
        {
            spi_rxne_callback(SPI_DR8(MIC_SPI));
        }
    }
}