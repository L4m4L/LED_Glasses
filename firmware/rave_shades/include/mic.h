#ifndef MIC_H
#define MIC_H

#include "stdint.h"

void mic_init(void (*mic_receive_callback)(uint8_t));
double mic_get_sample_rate(void);
void mic_enable(void);
void mic_disable(void);

#endif // #ifndef MIC_H