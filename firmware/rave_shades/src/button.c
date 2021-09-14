#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/timer.h>

#include "button.h"

#define BUTTON_GPIO_MODE               GPIO_MODE_INPUT
// The button doesn't have an external pull up resistor so we use the internal one instead.
#define BUTTON_GPIO_PULL               GPIO_PUPD_PULLUP
// Use the timer update interrupt. This is asserted on counter overflow (and only this if
// timer_update_on_overflow() is called).
#define BUTTON_TIMER_INTERRUPT_ENABLED TIM_DIER_UIE
#define BUTTON_TIMER_INTERRUPT_FLAG    TIM_SR_UIF
// Brute forced this with python, resulting timer interrupt frequency is ~100Hz.
// TODO: maybe calculate these in the init function in case PCLK changes.
#define BUTTON_TIMER_PRESCALER         (240U)
#define BUTTON_TIMER_PERIOD            (2213U)

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
uint32_t button_pressed_count = 0;
void (*button_pressed)(context_t*);

void button_init(void (*button_pressed_callback)(context_t*))
{
    button_pressed = button_pressed_callback;

    rcc_periph_clock_enable(BUTTON_RCC_GPIO);
    rcc_periph_clock_enable(BUTTON_RCC_TIMER);

    gpio_mode_setup(BUTTON_GPIO_PORT, BUTTON_GPIO_MODE, BUTTON_GPIO_PULL, BUTTON_GPIO_PIN);

    timer_continuous_mode(BUTTON_TIMER);
    timer_update_on_overflow(BUTTON_TIMER);
    timer_enable_irq(BUTTON_TIMER, BUTTON_TIMER_INTERRUPT_ENABLED);
    timer_set_prescaler(BUTTON_TIMER, BUTTON_TIMER_PRESCALER);
    timer_set_period(BUTTON_TIMER, BUTTON_TIMER_PERIOD);

    nvic_enable_irq(BUTTON_TIMER_NVIC);

    timer_enable_counter(BUTTON_TIMER);
}

void button_run(context_t* context)
{
    while (button_pressed_count > 0)
    {
        button_pressed_count--;
        if (button_pressed)
        {
            button_pressed(context);
        }
    }
}

BUTTON_TIMER_INTERRUPT()
{
    // Clear this sucker or you won't leave the interupt routine.
    timer_clear_flag(BUTTON_TIMER, BUTTON_TIMER_INTERRUPT_FLAG);

    button_history <<= 1U;

    // Invert the reading because the button signal is active low.
    button_history |= (uint8_t)(~(gpio_port_read(BUTTON_GPIO_PORT) >> BUTTON_SHIFT) & 1);

    switch(button_state)
    {
    case BUTTON_STATE_RELEASED:
        if (button_history == BUTTON_HISTORY_PRESSED)
        {
            button_state = BUTTON_STATE_PRESSED;
            button_pressed_count++;
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