#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>

#include "mic.h"
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

void (*spi_rxne_callback)(uint8_t);

void mic_init(void (*mic_receive_callback)(uint8_t))
{
    rcc_periph_clock_enable(MIC_RCC_GPIO);
    gpio_mode_setup(MIC_GPIO_PORT, MIC_GPIO_MODE, MIC_GPIO_PULL, MIC_GPIO_PIN_CLK | MIC_GPIO_PIN_MISO);
    gpio_set_af(MIC_GPIO_PORT, MIC_GPIO_AF, MIC_GPIO_PIN_CLK | MIC_GPIO_PIN_MISO);
    gpio_set_output_options(MIC_GPIO_PORT, MIC_GPIO_CLK_TYPE, MIC_GPIO_CLK_SPEED, MIC_GPIO_PIN_CLK);

    rcc_periph_clock_enable(MIC_RCC_SPI);
    spi_reset(MIC_SPI);
    spi_init_master(MIC_SPI, MIC_SPI_DIV, MIC_SPI_POL, MIC_SPI_PHA, MIC_SPI_END);
    spi_set_receive_only_mode(MIC_SPI);

    spi_enable_rx_buffer_not_empty_interrupt(MIC_SPI);
    nvic_enable_irq(NVIC_SPI1_IRQ);

    // In master receive only mode, the SPI clock is always on when enabled.
    // SPH0641LM4H-1 takes 50 ms to power up and reach normal operation.
    spi_enable(MIC_SPI);

    spi_rxne_callback = mic_receive_callback;
}

void spi1_isr(void)
{
    while (SPI_SR(MIC_SPI) & SPI_SR_RXNE)
    {
        spi_rxne_callback(SPI_DR8(MIC_SPI));
    }
}