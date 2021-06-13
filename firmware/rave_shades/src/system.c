#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/cortex.h>

#include "system.h"

static volatile uint32_t systick_overflow_counter = 0;

static double system_sysclk   = 16e6;
static double system_hclk     = 16e6;
static double system_pclk     = 16e6;
static double system_tick2sec = 1 / 16e6;

void system_init_pll_hsi16_53(void)
{
    int pllm_div = 1;
    int plln_mul = 10;
    int pllp_div = 3;
    int pllq_div = 3;
    int pllr_div = 3;
    int hclk_div = 1;
    int pclk_div = 1;

    system_sysclk   = ((16e6 / pllm_div) * plln_mul) / pllp_div;
    system_hclk     = system_sysclk / hclk_div;
    system_pclk     = system_hclk   / pclk_div;
    system_tick2sec = 1 / system_hclk;

    struct rcc_clock_scale clock_config = {
        .sysclock_source  = RCC_PLL,
        .pll_source       = RCC_PLLCFGR_PLLSRC_HSI16,
        .pll_div          = RCC_PLLCFGR_PLLM_DIV(pllm_div),
        .pll_mul          = RCC_PLLCFGR_PLLN_MUL(plln_mul),
        .pllp_div         = RCC_PLLCFGR_PLLP_DIV(pllp_div),
        .pllq_div         = RCC_PLLCFGR_PLLQ_DIV(pllq_div),
        .pllr_div         = RCC_PLLCFGR_PLLR_DIV(pllr_div),
        .hpre             = RCC_CFGR_HPRE_NODIV,
        .ppre             = RCC_CFGR_PPRE_NODIV,
        .flash_waitstates = FLASH_ACR_LATENCY_2WS,
        .voltage_scale    = PWR_SCALE1,
        .ahb_frequency    = (uint32_t)(system_hclk),
        .apb_frequency    = (uint32_t)(system_pclk),
    };

    rcc_clock_setup(&clock_config);
    // TODO: consider disabling PPL[Q/P]CLK to save power

    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_set_reload(STK_CVR_CURRENT);
    systick_clear();
    systick_interrupt_enable();
    systick_counter_enable();
}

double system_get_hclk(void)
{
    return system_hclk;
}

double system_get_pclk(void)
{
    return system_pclk;
}

uint64_t system_sec2tick(double sec)
{
    return (uint64_t)(sec / system_tick2sec);
}

uint64_t system_get_tick(void)
{
    uint64_t tick;
    CM_ATOMIC_BLOCK()
    {
        // Invert systick because it counts down.
        tick = STK_CVR_CURRENT - systick_get_value();
        tick |= (uint64_t)(systick_overflow_counter) << 24;
    }
    return tick;
}

double system_get_sec(void)
{
    return system_get_tick() * system_tick2sec;
}

void system_wait_tick(uint64_t tick)
{
    uint64_t tick_end = tick + system_get_tick();
    while (system_get_tick() < tick_end);
}

void system_wait_sec(double sec)
{
    double sec_end = sec + system_get_sec();
    while (system_get_sec() < sec_end);
}

void sys_tick_handler(void)
{
    systick_overflow_counter++;
}