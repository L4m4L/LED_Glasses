#ifndef UTIL_H
#define UTIL_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

/******************************************************************************
* GLOBAL DEFINITIONS **********************************************************
*******************************************************************************/
// Pin, peripheral and interrupt assignments.
#define BUTTON_RCC_GPIO          RCC_GPIOB
#define BUTTON_GPIO_PORT         GPIOB
#define BUTTON_GPIO_PIN          GPIO9
#define BUTTON_RCC_TIMER         RCC_TIM6
#define BUTTON_TIMER             TIM6
#define BUTTON_TIMER_NVIC        NVIC_TIM6_DAC_LPTIM1_IRQ
#define BUTTON_TIMER_INTERRUPT() void tim6_dac_lptim1_isr(void)

#define LED_RCC_GPIO             RCC_GPIOB
#define LED_GPIO_PORT            GPIOB
#define LED_GPIO_PIN_MOSI        GPIO7
#define LED_RCC_SPI              RCC_SPI2
#define LED_SPI                  SPI2
#define LED_RCC_DMA              RCC_DMA1
#define LED_DMA                  DMA1
#define LED_DMA_CHANNEL          DMA_CHANNEL1
#define LED_DMAMUX               DMAMUX1

#define MIC_RCC_GPIO             RCC_GPIOA
#define MIC_GPIO_PORT            GPIOA
#define MIC_GPIO_PIN_MISO        GPIO5
#define MIC_GPIO_PIN_CLK         GPIO6
#define MIC_RCC_SPI              RCC_SPI1
#define MIC_SPI                  SPI1
#define MIC_SPI_NVIC             NVIC_SPI1_IRQ
#define MIC_SPI_INTERRUPT()      void spi1_isr(void)

// Common configuration stuff.
#define LED_COUNT (66U)
#define LED_ROWS  (7U)
#define LED_COLS  (10U)

#define AUDIO_FREQUENCY_BIN_COUNT (10U)

/******************************************************************************
* GLOBAL TYPE DEFINITIONS *****************************************************
*******************************************************************************/
typedef enum display_mode_e_
{
    DISPLAY_MODE_DEFAULT,
    DISPLAY_MODE_HEART,
    DISPLAY_MODE_WATERFALL,
    DISPLAY_MODE_LACHLAN
} display_mode_e;

typedef struct colour_t_ {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} colour_t;

typedef struct context_t_
{
    uint32_t       audio_updated;
    float          audio_volume[AUDIO_FREQUENCY_BIN_COUNT];
    display_mode_e display_mode;
} context_t;

#endif // #ifndef UTIL_H