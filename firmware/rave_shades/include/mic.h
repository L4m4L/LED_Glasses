#ifndef MIC_H
#define MIC_H

#include "util.h"

void mic_init(void);
void mic_task(context_t* context);
void mic_test(void);

#endif // #ifndef MIC_H