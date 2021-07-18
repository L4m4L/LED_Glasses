#ifndef BUTTON_H
#define BUTTON_H

#include "util.h"

void button_init(void (*button_pressed_callback)(context_t*));
void button_run(context_t* context);

#endif // #ifndef BUTTON_H