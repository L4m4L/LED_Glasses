#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

void system_init_pll_hsi16_53(void);
double system_get_hclk(void);
double system_get_pclk(void);
uint64_t system_sec2tick(double sec);
uint64_t system_get_tick(void);
double system_get_sec(void);
void system_wait_tick(uint64_t tick);
void system_wait_sec(double sec);

#endif // #ifndef SYSTEM_H