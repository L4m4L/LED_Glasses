/* This module handles system clock configuration and tracks time since start up.
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

/**
 * Initialises the system clocks using the High Speed Internal 16MHz source, routed through the
 * Phase-Locked Loop which brings the frequency up to 53.333MHz. Note this frequency was chosen
 * so the LED interface can derive a 3.333MHz clock from the PCLK.
 */
void system_init_pll_hsi16_53(void);
/** Returns the AHB (read core systems) bus clock in Hz. */
double system_get_hclk(void);
/** Returns the APB (read peripherals) bus clock in Hz. */
double system_get_pclk(void);
/** Returns the timer peripheral clock in Hz. */
double system_get_timpclk(void);
/** Converts seconds to system ticks (~18ns per tick). */
uint64_t system_sec2tick(double sec);
/** Returns ticks since system_init_pll_hsi16_53() was called. */
uint64_t system_get_tick(void);
/** Returns seconds since system_init_pll_hsi16_53() was called. */
double system_get_sec(void);
/** Waits for the requested amount of time. */
void system_wait_tick(uint64_t tick);
/** Ditto. */
void system_wait_sec(double sec);

#endif /** SYSTEM_H */