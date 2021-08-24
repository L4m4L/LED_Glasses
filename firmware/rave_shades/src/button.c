#include "button.h"

#define BUTTON_GPIO_MODE GPIO_MODE_INPUT
#define BUTTON_GPIO_PULL GPIO_PUPD_PULLUP
#define BUTTON_SHIFT     (9U)
#define BUTTON_CLEAR     (0x00U)
#define BUTTON_SET       (0xFFU)

uint8_t button_history = 0;
uint32_t button_state = 0;
void (*button_pressed)(context_t*);

void button_init(void (*button_pressed_callback)(context_t*))
{
    // TODO: setup a timer interrupt to register button presses and call pending event callbacks
    // on button_run() because running in the mainloop means it's delayed by the audio fft which
    // makes the button unresponsive.
    rcc_periph_clock_enable(BUTTON_RCC_GPIO);
    gpio_mode_setup(BUTTON_GPIO_PORT, BUTTON_GPIO_MODE, BUTTON_GPIO_PULL, BUTTON_GPIO_PIN);

    button_pressed = button_pressed_callback;
}

void button_run(context_t* context)
{
    button_history <<= 1U;
    // Invert the reading because the button signal is active low.
    button_history |= (uint8_t)(~(gpio_port_read(BUTTON_GPIO_PORT) >> BUTTON_SHIFT) & 1);
    
    if (button_state)
    {
        if (button_history == BUTTON_CLEAR)
        {
            button_state = 0;
        }
    }
    else
    {
        if (button_history == BUTTON_SET)
        {
            button_state = 1;
            button_pressed(context);
        }
    }
}