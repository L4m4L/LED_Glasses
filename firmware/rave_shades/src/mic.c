#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/dmamux.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <stdint.h>

#include "mic.h"
#include "util.h"

#define MIC_GPIO_MODE GPIO_MODE_AF
#define MIC_GPIO_PULL GPIO_PUPD_NONE
#define MIC_GPIO_AF GPIO_AF0
#define MIC_GPIO_CLK_TYPE GPIO_OTYPE_PP
#define MIC_GPIO_CLK_SPEED GPIO_OSPEED_50MHZ
#define MIC_SPI_DIV SPI_CR1_BAUDRATE_FPCLK_DIV_32
#define MIC_SPI_POL SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE
#define MIC_SPI_PHA SPI_CR1_CPHA_CLK_TRANSITION_2
#define MIC_SPI_END SPI_CR1_LSBFIRST
#define MIC_SPI_SIZE SPI_CR2_DS_8BIT
#define MIC_DMA_SIZE_PERIPH DMA_CCR_PSIZE_8BIT
#define MIC_DMA_SIZE_MEMORY DMA_CCR_MSIZE_8BIT

void (*mic_dma_complete_callback)(void) = 0;

void mic_init(void (*recording_complete_callback)(void))
{
    mic_dma_complete_callback = recording_complete_callback;

    rcc_periph_clock_enable(MIC_RCC_GPIO);
    gpio_mode_setup(MIC_GPIO_PORT, MIC_GPIO_MODE, MIC_GPIO_PULL, MIC_GPIO_PIN_CLK | MIC_GPIO_PIN_MISO);
    gpio_set_af(MIC_GPIO_PORT, MIC_GPIO_AF, MIC_GPIO_PIN_CLK | MIC_GPIO_PIN_MISO);
    gpio_set_output_options(MIC_GPIO_PORT, MIC_GPIO_CLK_TYPE, MIC_GPIO_CLK_SPEED, MIC_GPIO_PIN_CLK);

    rcc_periph_clock_enable(MIC_RCC_SPI);
    spi_reset(MIC_SPI);
    spi_init_master(MIC_SPI, MIC_SPI_DIV, MIC_SPI_POL, MIC_SPI_PHA, MIC_SPI_END);
    spi_set_data_size(MIC_SPI, MIC_SPI_SIZE);
    spi_set_receive_only_mode(MIC_SPI);
    spi_fifo_reception_threshold_8bit(MIC_SPI);

    rcc_periph_clock_enable(MIC_RCC_DMA);
    dma_channel_reset(MIC_DMA, MIC_DMA_CHANNEL);
    dma_set_peripheral_address(MIC_DMA, MIC_DMA_CHANNEL, (uint32_t)(&SPI_DR(MIC_SPI)));
    dma_set_peripheral_size(MIC_DMA, MIC_DMA_CHANNEL, MIC_DMA_SIZE_PERIPH);
    dma_set_memory_size(MIC_DMA, MIC_DMA_CHANNEL, MIC_DMA_SIZE_MEMORY);
    dma_set_read_from_peripheral(MIC_DMA, MIC_DMA_CHANNEL);
    dma_enable_memory_increment_mode(MIC_DMA, MIC_DMA_CHANNEL);
    dma_set_priority(MIC_DMA, MIC_DMA_CHANNEL, MIC_DMA_PRIORITY);
    dma_enable_transfer_error_interrupt(MIC_DMA, MIC_DMA_CHANNEL);
    dma_enable_transfer_complete_interrupt(MIC_DMA, MIC_DMA_CHANNEL);

    dmamux_reset_dma_channel(MIC_DMAMUX, MIC_DMA_CHANNEL);
    dmamux_set_dma_channel_request(MIC_DMAMUX, MIC_DMA_CHANNEL, MIC_DMAMUX_REQUEST);

    nvic_enable_irq(NVIC_DMA1_CHANNEL1_IRQ);

    spi_enable(MIC_SPI);
}

uint16_t mic_record(uint8_t *buffer, uint16_t transfers)
{
    uint32_t dma_enabled = DMA_CCR(MIC_DMA, MIC_DMA_CHANNEL) & DMA_CCR_EN;
    uint16_t transfers_lost = 0;
    if (dma_enabled)
    {
        transfers_lost = dma_get_number_of_data(MIC_DMA, MIC_DMA_CHANNEL);
    }

    dma_get_number_of_data(MIC_DMA, MIC_DMA_CHANNEL);

    spi_disable_rx_dma(MIC_SPI);
    dma_disable_channel(MIC_DMA, MIC_DMA_CHANNEL);

    dma_set_memory_address(MIC_DMA, MIC_DMA_CHANNEL, (uint32_t)(buffer));
    dma_set_number_of_data(MIC_DMA, MIC_DMA_CHANNEL, transfers);

    dma_enable_channel(MIC_DMA, MIC_DMA_CHANNEL);
    spi_enable_rx_dma(MIC_SPI);

    return transfers_lost;
}

MIC_DMA_INTERRUPT()
{
    /** Check the Transfer Complete Interrupt Flag for channel 1 (TCIF1). */
    if (dma_get_interrupt_flag(MIC_DMA, MIC_DMA_CHANNEL, DMA_ISR_TCIF1))
    {
        if (mic_dma_complete_callback != 0)
        {
            mic_dma_complete_callback();
        }
    }

    /** If we don't clear the flag we'll be stuck in this interrupt forever. */
    dma_clear_interrupt_flags(MIC_DMA, MIC_DMA_CHANNEL, DMA_GIF);
}