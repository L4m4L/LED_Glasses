#ifndef MIC_H
#define MIC_H

#include "stdint.h"

void mic_init(void (*mic_receive_callback)(uint8_t));

#endif // #ifndef MIC_H