#ifndef UTIL_H
#define UTIL_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

#include "fixed.h"

/** Pin, peripheral and interrupt assignments. */
#define BUTTON_RCC_GPIO RCC_GPIOB
#define BUTTON_GPIO_PORT GPIOB
#define BUTTON_GPIO_PIN GPIO9
#define BUTTON_RCC_TIMER RCC_TIM6
#define BUTTON_TIMER TIM6
#define BUTTON_TIMER_NVIC NVIC_TIM6_DAC_LPTIM1_IRQ
#define BUTTON_TIMER_INTERRUPT() void tim6_dac_lptim1_isr(void)

#define LED_RCC_GPIO RCC_GPIOB
#define LED_GPIO_PORT GPIOB
#define LED_GPIO_PIN_MOSI GPIO7
#define LED_RCC_SPI RCC_SPI2
#define LED_SPI SPI2
#define LED_RCC_DMA RCC_DMA1
#define LED_DMA DMA1
#define LED_DMA_CHANNEL DMA_CHANNEL2
#define LED_DMA_PRIORITY DMA_CCR_PL_HIGH
#define LED_DMAMUX DMAMUX1
#define LED_DMAMUX_REQUEST DMAMUX_CxCR_DMAREQ_ID_SPI2_TX

#define MIC_RCC_GPIO RCC_GPIOA
#define MIC_GPIO_PORT GPIOA
#define MIC_GPIO_PIN_MISO GPIO5
#define MIC_GPIO_PIN_CLK GPIO6
#define MIC_RCC_SPI RCC_SPI1
#define MIC_SPI SPI1
#define MIC_SPI_NVIC NVIC_SPI1_IRQ
#define MIC_RCC_DMA RCC_DMA1
#define MIC_DMA DMA1
#define MIC_DMA_CHANNEL DMA_CHANNEL1
#define MIC_DMA_PRIORITY DMA_CCR_PL_VERY_HIGH
#define MIC_DMAMUX DMAMUX1
#define MIC_DMAMUX_REQUEST DMAMUX_CxCR_DMAREQ_ID_SPI1_RX
#define MIC_DMA_INTERRUPT() void dma1_channel1_isr(void)

/** Common configuration stuff. */
#define LED_COUNT (66U)
#define LED_ROWS (7U)
#define LED_COLS (10U)

#define FILTER_TAPS (480U)
#define FILTER_DEC (120U)
#define FILTER_STEPS (FILTER_TAPS / 8U)
#define FILTER_BYTEWISE_DEC (FILTER_DEC / 8U)
#define FILTER_STAGES (FILTER_TAPS / FILTER_DEC)

/**
 * At 512 samples with a sample rate of 1.666 MHz / 120 = 13.883 kHz (see
 * audio.h) we are collecting 37 ms of audio giving us a frequency
 * resolution of 27 Hz.
 */
#define AUDIO_BUF_FILT_LEN (512U)
#define AUDIO_BUF_RAW_LEN (AUDIO_BUF_FILT_LEN * FILTER_BYTEWISE_DEC)

/** Common types. */
typedef enum display_mode_e_
{
    DISPLAY_MODE_DEFAULT,
    DISPLAY_MODE_HEART,
    DISPLAY_MODE_WATERFALL,
    DISPLAY_MODE_LACHLAN
} display_mode_e;

typedef struct context_t_
{
    uint32_t audio_updated;
    fixed_t audio_filtered[AUDIO_BUF_FILT_LEN];
    display_mode_e display_mode;
} context_t;

#endif /** UTIL_H */