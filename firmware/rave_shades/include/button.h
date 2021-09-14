/* The module handles the user button on the side of the glasses and will call the callback
** function when the button transitions from released to pressed. The button state is debounced
** by tracking the last N readings and only changing the state when all N readings are the same.
** Readings and state changes are done inside a timer interrupt that is set to run every 10ms.
** Button pressed events are tracked in the interrupt and susequently executed on calls to
** button_run().
*/

#ifndef BUTTON_H
#define BUTTON_H

#include "util.h"

// Saves the callback function internally, initialises the button input pin and the timer.
// \param[in]  button_pressed_callback  Callback that is executed on button presses.
void button_init(void (*button_pressed_callback)(context_t*));
// Executes the callback function for each button press detected since button_run() was last
// called.
// \param[in]  context  Global context struct, handed to the callback function.
void button_run(context_t* context);

#endif // #ifndef BUTTON_H