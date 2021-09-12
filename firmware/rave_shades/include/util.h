#ifndef UTIL_H
#define UTIL_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

/******************************************************************************
* GLOBAL DEFINITIONS **********************************************************
*******************************************************************************/
#define BUTTON_RCC_GPIO   RCC_GPIOB
#define BUTTON_GPIO_PORT  GPIOB
#define BUTTON_GPIO_PIN   GPIO9

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

#define AUDIO_FREQUENCY_BINS LED_COLS

/******************************************************************************
* GLOBAL TYPE DEFINITIONS *****************************************************
*******************************************************************************/
typedef enum display_mode_e_
{
    DISPLAY_MODE_DEFAULT,
    DISPLAY_MODE_ANIMATION,
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
    uint32_t         audio_updated;
    float            audio_volume[AUDIO_FREQUENCY_BINS];
    display_mode_e   display_mode;
    uint32_t         display_animation_len;
    const colour_t (*display_animation)[][LED_COUNT]; // A pointer to the current animation.
} context_t;

#endif // #ifndef UTIL_H