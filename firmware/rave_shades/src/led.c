#include <stdint.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

#include "led.h"
#include "util.h"

#define LED_GPIO_PIN_MODE      GPIO_MODE_AF
#define LED_GPIO_PIN_PULL      GPIO_PUPD_NONE
#define LED_GPIO_PIN_AF        GPIO_AF1
#define LED_GPIO_PIN_CLK_TYPE  GPIO_OTYPE_PP
#define LED_GPIO_PIN_CLK_SPEED GPIO_OSPEED_50MHZ
// (sysclk == 53.333MHz) / 16 == 3.333MHz
#define LED_SPI_DIV            SPI_CR1_BAUDRATE_FPCLK_DIV_16
#define LED_SPI_POL            SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE
#define LED_SPI_PHA            SPI_CR1_CPHA_CLK_TRANSITION_1
#define LED_SPI_END            SPI_CR1_MSBFIRST
// 16bit transfers will save some cpu time
#define LED_SPI_SIZE           SPI_CR2_DS_16BIT

#define LED_COLOUR_DEPTH       (8U)
#define LED_COLOUR_BITS_PER_TX (4U)
#define LED_TX_PER_COLOUR      (LED_COLOUR_DEPTH / LED_COLOUR_BITS_PER_TX)
#define LED_COLOUR_MAP_SIZE    (LED_TX_PER_COLOUR * (1U << LED_COLOUR_DEPTH))
#define LED_COLOURS            (3U)
// spi transmissions per led
#define LED_TX_PER             (LED_COLOURS * LED_TX_PER_COLOUR)
#define LED_BUFFER_SIZE        (LED_TX_PER * LED_COUNT)
// reset low pulse is 80us, each spi transfer is 4.8us
// reset required at the end fo each transfer??
#define LED_RESET_TX_COUNT     (30U)//(16U)

#define LED_RESET (0x0000U)
#define LED_0000  (0x8888U)
#define LED_0001  (0x888CU)
#define LED_0010  (0x88C8U)
#define LED_0011  (0x88CCU)
#define LED_0100  (0x8C88U)
#define LED_0101  (0x8C8CU)
#define LED_0110  (0x8CC8U)
#define LED_0111  (0x8CCCU)
#define LED_1000  (0xC888U)
#define LED_1001  (0xC88CU)
#define LED_1010  (0xC8C8U)
#define LED_1011  (0xC8CCU)
#define LED_1100  (0xCC88U)
#define LED_1101  (0xCC8CU)
#define LED_1110  (0xCCC8U)
#define LED_1111  (0xCCCCU)

static const uint16_t led_colour_map[LED_COLOUR_MAP_SIZE] = {
    LED_0000, LED_0000,   LED_0000, LED_0001,   LED_0000, LED_0010,   LED_0000, LED_0011,
    LED_0000, LED_0100,   LED_0000, LED_0101,   LED_0000, LED_0110,   LED_0000, LED_0111,
    LED_0000, LED_1000,   LED_0000, LED_1001,   LED_0000, LED_1010,   LED_0000, LED_1011,
    LED_0000, LED_1100,   LED_0000, LED_1101,   LED_0000, LED_1110,   LED_0000, LED_1111,
    LED_0001, LED_0000,   LED_0001, LED_0001,   LED_0001, LED_0010,   LED_0001, LED_0011,
    LED_0001, LED_0100,   LED_0001, LED_0101,   LED_0001, LED_0110,   LED_0001, LED_0111,
    LED_0001, LED_1000,   LED_0001, LED_1001,   LED_0001, LED_1010,   LED_0001, LED_1011,
    LED_0001, LED_1100,   LED_0001, LED_1101,   LED_0001, LED_1110,   LED_0001, LED_1111,
    LED_0010, LED_0000,   LED_0010, LED_0001,   LED_0010, LED_0010,   LED_0010, LED_0011,
    LED_0010, LED_0100,   LED_0010, LED_0101,   LED_0010, LED_0110,   LED_0010, LED_0111,
    LED_0010, LED_1000,   LED_0010, LED_1001,   LED_0010, LED_1010,   LED_0010, LED_1011,
    LED_0010, LED_1100,   LED_0010, LED_1101,   LED_0010, LED_1110,   LED_0010, LED_1111,
    LED_0011, LED_0000,   LED_0011, LED_0001,   LED_0011, LED_0010,   LED_0011, LED_0011,
    LED_0011, LED_0100,   LED_0011, LED_0101,   LED_0011, LED_0110,   LED_0011, LED_0111,
    LED_0011, LED_1000,   LED_0011, LED_1001,   LED_0011, LED_1010,   LED_0011, LED_1011,
    LED_0011, LED_1100,   LED_0011, LED_1101,   LED_0011, LED_1110,   LED_0011, LED_1111,
    LED_0100, LED_0000,   LED_0100, LED_0001,   LED_0100, LED_0010,   LED_0100, LED_0011,
    LED_0100, LED_0100,   LED_0100, LED_0101,   LED_0100, LED_0110,   LED_0100, LED_0111,
    LED_0100, LED_1000,   LED_0100, LED_1001,   LED_0100, LED_1010,   LED_0100, LED_1011,
    LED_0100, LED_1100,   LED_0100, LED_1101,   LED_0100, LED_1110,   LED_0100, LED_1111,
    LED_0101, LED_0000,   LED_0101, LED_0001,   LED_0101, LED_0010,   LED_0101, LED_0011,
    LED_0101, LED_0100,   LED_0101, LED_0101,   LED_0101, LED_0110,   LED_0101, LED_0111,
    LED_0101, LED_1000,   LED_0101, LED_1001,   LED_0101, LED_1010,   LED_0101, LED_1011,
    LED_0101, LED_1100,   LED_0101, LED_1101,   LED_0101, LED_1110,   LED_0101, LED_1111,
    LED_0110, LED_0000,   LED_0110, LED_0001,   LED_0110, LED_0010,   LED_0110, LED_0011,
    LED_0110, LED_0100,   LED_0110, LED_0101,   LED_0110, LED_0110,   LED_0110, LED_0111,
    LED_0110, LED_1000,   LED_0110, LED_1001,   LED_0110, LED_1010,   LED_0110, LED_1011,
    LED_0110, LED_1100,   LED_0110, LED_1101,   LED_0110, LED_1110,   LED_0110, LED_1111,
    LED_0111, LED_0000,   LED_0111, LED_0001,   LED_0111, LED_0010,   LED_0111, LED_0011,
    LED_0111, LED_0100,   LED_0111, LED_0101,   LED_0111, LED_0110,   LED_0111, LED_0111,
    LED_0111, LED_1000,   LED_0111, LED_1001,   LED_0111, LED_1010,   LED_0111, LED_1011,
    LED_0111, LED_1100,   LED_0111, LED_1101,   LED_0111, LED_1110,   LED_0111, LED_1111,
    LED_1000, LED_0000,   LED_1000, LED_0001,   LED_1000, LED_0010,   LED_1000, LED_0011,
    LED_1000, LED_0100,   LED_1000, LED_0101,   LED_1000, LED_0110,   LED_1000, LED_0111,
    LED_1000, LED_1000,   LED_1000, LED_1001,   LED_1000, LED_1010,   LED_1000, LED_1011,
    LED_1000, LED_1100,   LED_1000, LED_1101,   LED_1000, LED_1110,   LED_1000, LED_1111,
    LED_1001, LED_0000,   LED_1001, LED_0001,   LED_1001, LED_0010,   LED_1001, LED_0011,
    LED_1001, LED_0100,   LED_1001, LED_0101,   LED_1001, LED_0110,   LED_1001, LED_0111,
    LED_1001, LED_1000,   LED_1001, LED_1001,   LED_1001, LED_1010,   LED_1001, LED_1011,
    LED_1001, LED_1100,   LED_1001, LED_1101,   LED_1001, LED_1110,   LED_1001, LED_1111,
    LED_1010, LED_0000,   LED_1010, LED_0001,   LED_1010, LED_0010,   LED_1010, LED_0011,
    LED_1010, LED_0100,   LED_1010, LED_0101,   LED_1010, LED_0110,   LED_1010, LED_0111,
    LED_1010, LED_1000,   LED_1010, LED_1001,   LED_1010, LED_1010,   LED_1010, LED_1011,
    LED_1010, LED_1100,   LED_1010, LED_1101,   LED_1010, LED_1110,   LED_1010, LED_1111,
    LED_1011, LED_0000,   LED_1011, LED_0001,   LED_1011, LED_0010,   LED_1011, LED_0011,
    LED_1011, LED_0100,   LED_1011, LED_0101,   LED_1011, LED_0110,   LED_1011, LED_0111,
    LED_1011, LED_1000,   LED_1011, LED_1001,   LED_1011, LED_1010,   LED_1011, LED_1011,
    LED_1011, LED_1100,   LED_1011, LED_1101,   LED_1011, LED_1110,   LED_1011, LED_1111,
    LED_1100, LED_0000,   LED_1100, LED_0001,   LED_1100, LED_0010,   LED_1100, LED_0011,
    LED_1100, LED_0100,   LED_1100, LED_0101,   LED_1100, LED_0110,   LED_1100, LED_0111,
    LED_1100, LED_1000,   LED_1100, LED_1001,   LED_1100, LED_1010,   LED_1100, LED_1011,
    LED_1100, LED_1100,   LED_1100, LED_1101,   LED_1100, LED_1110,   LED_1100, LED_1111,
    LED_1101, LED_0000,   LED_1101, LED_0001,   LED_1101, LED_0010,   LED_1101, LED_0011,
    LED_1101, LED_0100,   LED_1101, LED_0101,   LED_1101, LED_0110,   LED_1101, LED_0111,
    LED_1101, LED_1000,   LED_1101, LED_1001,   LED_1101, LED_1010,   LED_1101, LED_1011,
    LED_1101, LED_1100,   LED_1101, LED_1101,   LED_1101, LED_1110,   LED_1101, LED_1111,
    LED_1110, LED_0000,   LED_1110, LED_0001,   LED_1110, LED_0010,   LED_1110, LED_0011,
    LED_1110, LED_0100,   LED_1110, LED_0101,   LED_1110, LED_0110,   LED_1110, LED_0111,
    LED_1110, LED_1000,   LED_1110, LED_1001,   LED_1110, LED_1010,   LED_1110, LED_1011,
    LED_1110, LED_1100,   LED_1110, LED_1101,   LED_1110, LED_1110,   LED_1110, LED_1111,
    LED_1111, LED_0000,   LED_1111, LED_0001,   LED_1111, LED_0010,   LED_1111, LED_0011,
    LED_1111, LED_0100,   LED_1111, LED_0101,   LED_1111, LED_0110,   LED_1111, LED_0111,
    LED_1111, LED_1000,   LED_1111, LED_1001,   LED_1111, LED_1010,   LED_1111, LED_1011,
    LED_1111, LED_1100,   LED_1111, LED_1101,   LED_1111, LED_1110,   LED_1111, LED_1111
};

static const uint16_t led_addresses[LED_ROWS][LED_COLS] = {
    {  37,   36,   35,   34,   33,        32,   31,   30,   29,   28},
    {  38,   39,   40,   41,   42,        23,   24,   25,   26,   27},
    {  47,   46,   45,   44,   43,        22,   21,   20,   19,   18},
    {  48,   49,   50,   51,   52,        13,   14,   15,   16,   17},
    {  57,   56,   55,   54,   53,        12,   11,   10,    9,    8},
    {  58,   59,   60,   61,   62,         3,    4,    5,    6,    7},
    {9999,   65,   64,   63, 9999,      9999,    2,    1,    0, 9999},
};

static uint32_t led_buffer_modified = 0;
static uint16_t led_buffer[LED_BUFFER_SIZE];

static inline void led_buffer_modify(uint32_t index, uint16_t value)
{
    led_buffer_modified |= (led_buffer[index] != value);
    led_buffer[index] = value;
}

static inline uint16_t led_colour_lo(uint32_t c)
{
    return led_colour_map[LED_TX_PER_COLOUR * c];
}

static inline uint16_t led_colour_hi(uint32_t c)
{
    return led_colour_map[LED_TX_PER_COLOUR * c + 1];
}

void led_init(void)
{
    for (uint32_t i = 0; i < LED_BUFFER_SIZE; i++)
    {
        led_buffer[i] = LED_0000;
    }

    rcc_periph_clock_enable(LED_RCC_GPIO);
    gpio_mode_setup(LED_GPIO_PORT, LED_GPIO_PIN_MODE, LED_GPIO_PIN_PULL, LED_GPIO_PIN_MOSI);
    gpio_set_af(LED_GPIO_PORT, LED_GPIO_PIN_AF, LED_GPIO_PIN_MOSI);
    gpio_set_output_options(LED_GPIO_PORT, LED_GPIO_PIN_CLK_TYPE, LED_GPIO_PIN_CLK_SPEED, LED_GPIO_PIN_MOSI);

    rcc_periph_clock_enable(LED_RCC_SPI);
    spi_reset(LED_SPI);
    spi_init_master(LED_SPI, LED_SPI_DIV, LED_SPI_POL, LED_SPI_PHA, LED_SPI_END);
    SPI_CR2(LED_SPI) |= LED_SPI_SIZE;
    spi_enable(LED_SPI);

    led_flush();
}

void led_flush(void)
{
    if (led_buffer_modified)
    {
        led_buffer_modified = 0;
        
        for (uint32_t i = 0; i < LED_BUFFER_SIZE; i++)
        {
            spi_send(LED_SPI, led_buffer[i]);
        }

        for (uint32_t i = 0; i < LED_RESET_TX_COUNT; i++)
        {
            spi_send(LED_SPI, LED_RESET);
        }
    }
}

void led_clear(void)
{
    for (uint32_t i = 0; i < LED_BUFFER_SIZE; i++)
    {
        led_buffer_modify(i, LED_0000);
    }
}

void led_set(uint32_t led, colour_t c)
{
    uint32_t base_addr;

    if (led < LED_COUNT)
    {
        base_addr = LED_TX_PER * led;
        led_buffer_modify(base_addr + 0, led_colour_lo(c.g)); // green
        led_buffer_modify(base_addr + 1, led_colour_hi(c.g)); // green
        led_buffer_modify(base_addr + 2, led_colour_lo(c.r)); // red
        led_buffer_modify(base_addr + 3, led_colour_hi(c.r)); // red
        led_buffer_modify(base_addr + 4, led_colour_lo(c.b)); // blue
        led_buffer_modify(base_addr + 5, led_colour_hi(c.b)); // blue
    }
}

void led_set_all(colour_t c)
{
    for (uint32_t led = 0; led < LED_COUNT; led++)
    {
        led_set(led, c);
    }
}

void led_set_pixel(uint32_t x, uint32_t y, colour_t c)
{
    if ((x < LED_COLS) && (y < LED_ROWS))
    {
        led_set(led_addresses[y][x], c);
    }
}