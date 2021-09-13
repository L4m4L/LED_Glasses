#include "button.h"

#define BUTTON_GPIO_MODE GPIO_MODE_INPUT
#define BUTTON_GPIO_PULL GPIO_PUPD_PULLUP

#define BUTTON_SHIFT (9U)

#define BUTTON_HISTORY_RELEASED (0x00U)
#define BUTTON_HISTORY_PRESSED  (0xFFU)

typedef enum button_state_e_
{
    BUTTON_STATE_RELEASED,
    BUTTON_STATE_PRESSED
} button_state_e;

uint8_t button_history = 0;
button_state_e button_state = BUTTON_STATE_RELEASED;
void (*button_pressed)(context_t*);

void button_init(void (*button_pressed_callback)(context_t*))
{
    button_pressed = button_pressed_callback;

    // TODO: setup a timer interrupt to register button presses and call pending event callbacks
    // on button_run() because running in the mainloop means it's delayed by the audio fft which
    // makes the button unresponsive.
    rcc_periph_clock_enable(BUTTON_RCC_GPIO);
    gpio_mode_setup(BUTTON_GPIO_PORT, BUTTON_GPIO_MODE, BUTTON_GPIO_PULL, BUTTON_GPIO_PIN);
}

void button_run(context_t* context)
{
    button_history <<= 1U;
    // Invert the reading because the button signal is active low.
    button_history |= (uint8_t)(~(gpio_port_read(BUTTON_GPIO_PORT) >> BUTTON_SHIFT) & 1);
    
    // A simple state machine for handling button released/pressed transitions.
    switch(button_state)
    {
    case BUTTON_STATE_RELEASED:
        if (button_history == BUTTON_HISTORY_PRESSED)
        {
            button_state = BUTTON_STATE_PRESSED;

            if (button_pressed)
            {
                button_pressed(context);
            }
        }
        break;
    case BUTTON_STATE_PRESSED:
        if (button_history == BUTTON_HISTORY_RELEASED)
        {
            button_state = BUTTON_STATE_RELEASED;
        }
        break;
    default:
        button_state = BUTTON_STATE_RELEASED;
        break;
    }
}