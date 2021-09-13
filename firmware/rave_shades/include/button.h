/* This module handles debouncing button readings and calls the button_pressed_callback on the
** unpressed to pressed state change.
** TODO: setup a timer+interrupt to poll the button at regular intervals (say 10ms) and save
**       state changes to be actioned when button_run() is called.
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "util.h"

// Initialises the button input pin and saves the callback function internally.
// \param[in]  button_pressed_callback  Callback function which is executed on the button released
//                                      to button pressed transition.
void button_init(void (*button_pressed_callback)(context_t*));
// Reads the button pin value, debounces it and calls the callback if necessary.
// \param[in]  context  Global context struct, handed to the callback function.
void button_run(context_t* context);

#endif // #ifndef BUTTON_H