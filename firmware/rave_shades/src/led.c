#include <stdint.h>

#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/dmamux.h>
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
// 16bit transfers will (hopefully) save on tx overhead
#define LED_SPI_SIZE           SPI_CR2_DS_16BIT

#define LED_COLOUR_DEPTH       (8U)
#if LED_SPI_SIZE == SPI_CR2_DS_8BIT
    #define LED_COLOUR_BITS_PER_TX (2U)
#elif LED_SPI_SIZE == SPI_CR2_DS_16BIT
    #define LED_COLOUR_BITS_PER_TX (4U)
#endif
#define LED_TX_PER_COLOUR      (LED_COLOUR_DEPTH / LED_COLOUR_BITS_PER_TX)
#define LED_COLOUR_MAP_SIZE    (LED_TX_PER_COLOUR * (1U << LED_COLOUR_DEPTH))
#define LED_COLOURS            (3U)
// Spi transmissions per led.
#define LED_TX_PER             (LED_COLOURS * LED_TX_PER_COLOUR)
#define LED_BUFFER_SIZE        (LED_TX_PER * LED_COUNT)
// Reset low pulse is 80us, each spi transfer is 4.8us.
// Experimentally a reset length of 144us seems to work best.
// TODO: Could possibly be refined further.
#define LED_RESET_TX_COUNT     (30U)

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

static uint32_t led_buffer_modified = 1;
// We can store reset values at the end of the buffer to simplify DMA transfers.
static uint16_t led_buffer[LED_BUFFER_SIZE+LED_RESET_TX_COUNT];

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
    uint32_t i = 0;
    for (; i < LED_BUFFER_SIZE; i++)
    {
        led_buffer[i] = LED_0000;
    }
    for (; i < LED_BUFFER_SIZE + LED_RESET_TX_COUNT; i++)
    {
        led_buffer[i] = LED_RESET;
    }

    rcc_periph_clock_enable(LED_RCC_GPIO);
    gpio_mode_setup(LED_GPIO_PORT, LED_GPIO_PIN_MODE, LED_GPIO_PIN_PULL, LED_GPIO_PIN_MOSI);
    gpio_set_af(LED_GPIO_PORT, LED_GPIO_PIN_AF, LED_GPIO_PIN_MOSI);
    gpio_set_output_options(LED_GPIO_PORT, LED_GPIO_PIN_CLK_TYPE, LED_GPIO_PIN_CLK_SPEED, LED_GPIO_PIN_MOSI);

    rcc_periph_clock_enable(LED_RCC_SPI);
    spi_reset(LED_SPI);
    spi_init_master(LED_SPI, LED_SPI_DIV, LED_SPI_POL, LED_SPI_PHA, LED_SPI_END);
    spi_set_data_size(LED_SPI, LED_SPI_SIZE);
    spi_enable(LED_SPI);

    rcc_periph_clock_enable(RCC_DMA1);
    dma_channel_reset(DMA1, DMA_CHANNEL1);
    dma_set_peripheral_address(DMA1, DMA_CHANNEL1, (uint32_t)(&SPI_DR(LED_SPI)));
    dma_set_peripheral_size(DMA1, DMA_CHANNEL1, DMA_CCR_PSIZE_16BIT);
    dma_set_memory_address(DMA1, DMA_CHANNEL1, (uint32_t)(led_buffer));
    dma_set_memory_size(DMA1, DMA_CHANNEL1, DMA_CCR_MSIZE_16BIT);
    dma_set_read_from_memory(DMA1, DMA_CHANNEL1);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL1);
    dma_set_priority(DMA1, DMA_CHANNEL1, DMA_CCR_PL_VERY_HIGH);

    dmamux_reset_dma_channel(DMAMUX1, DMA_CHANNEL1);
    dmamux_set_dma_channel_request(DMAMUX1, DMA_CHANNEL1, DMAMUX_CxCR_DMAREQ_ID_SPI2_TX);

    led_flush_dma();
}

void led_flush(void)
{
    if (led_buffer_modified)
    {
        led_buffer_modified = 0;
        
        for (uint32_t i = 0; i < LED_BUFFER_SIZE + LED_RESET_TX_COUNT; i++)
        {
            spi_send(LED_SPI, led_buffer[i]);
        }
    }
}

void led_flush_dma(void)
{
    uint16_t dma_tx_remaining = dma_get_number_of_data(DMA1, DMA_CHANNEL1);
    uint32_t dma_enabled = DMA_CCR(DMA1, DMA_CHANNEL1) & DMA_CCR_EN;

    // Make sure the DMA transfer is either complete (number of transfers remaining is 0) or the
    // DMA is disabled (presumably caused by a DMA error) before trying to start a new transfer.
    // TODO: come up with a better solution, this wastes a whole frame worth of processing.
    if ((led_buffer_modified) && ((dma_tx_remaining == 0) || !(dma_enabled)))
    {
        // while ((dma_tx_remaining > 0) && dma_enabled)
        led_buffer_modified = 0;
        
        // DMA_CNDTRx (number of transfers remaining) is decremented until 0 with each successful
        // transfer. To begin another transfer this must be set to the buffer length again. The
        // DMA channel must be disabled to write to DMA_CNDTRx, and the peripherial must be
        // disabled to disable the DMA.
        spi_disable_tx_dma(LED_SPI);
        dma_disable_channel(DMA1, DMA_CHANNEL1);
        dma_set_number_of_data(DMA1, DMA_CHANNEL1, LED_BUFFER_SIZE + LED_RESET_TX_COUNT);
        dma_enable_channel(DMA1, DMA_CHANNEL1);
        spi_enable_tx_dma(LED_SPI);
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