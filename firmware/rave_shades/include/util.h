#ifndef UTIL_H
#define UTIL_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

/******************************************************************************
* GLOBAL DEFINITIONS **********************************************************
*******************************************************************************/
#define LED_RCC_GPIO      RCC_GPIOB
#define LED_GPIO_PORT     GPIOB
#define LED_GPIO_PIN_MOSI GPIO7
#define LED_RCC_SPI       RCC_SPI2
#define LED_SPI           SPI2

#define LED_COUNT         (66U)
#define LED_ROWS          (7U)
#define LED_COLS          (10U)

#define MIC_RCC_GPIO      RCC_GPIOA
#define MIC_GPIO_PORT     GPIOA
#define MIC_GPIO_PIN_MISO GPIO5
#define MIC_GPIO_PIN_CLK  GPIO6
#define MIC_RCC_SPI       RCC_SPI1
#define MIC_SPI           SPI1

/******************************************************************************
* GLOBAL TYPE DEFINITIONS *****************************************************
*******************************************************************************/
typedef enum display_mode_e_
{
    DISPLAY_MODE_LINES,
    DISPLAY_MODE_PK2PK,
    DISPLAY_MODE_FFT,
    DISPLAY_MODE_SCROLL
} display_mode_e;

typedef struct context_t_
{
    char display_text[50];
    uint8_t mic_pk2pk;
    uint8_t mic_fft[LED_COUNT];
    display_mode_e display_mode;
} context_t;

#endif // #ifndef UTIL_H