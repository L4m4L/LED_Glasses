#include "animation.h"
#include "audio.h"
#include "button.h"
#include "display.h"
#include "system.h"
#include "util.h"

#define AUDIO_TASK_PERIOD   (0.01)
#define BUTTON_TASK_PERIOD  (0.001)
#define DISPLAY_TASK_PERIOD (0.01)

static void button_pressed_callback(context_t* context)
{
    if (context->mode      == DISPLAY_MODE_DEFAULT)
    {
        context->mode       = DISPLAY_MODE_ANIMATION;
        context->animation  = &animation_heart;
        context->length     = ANIMATION_HEART_LENGTH;
    }
    else if (context->mode == DISPLAY_MODE_ANIMATION)
    {
        context->mode       = DISPLAY_MODE_HEATMAP;
    }
    else if (context->mode == DISPLAY_MODE_HEATMAP)
    {
        context->mode       = DISPLAY_MODE_DEFAULT;
    }
}

int main(void)
{
    uint64_t current_tick;
    uint64_t task_period_tick[3];
    uint64_t task_next_tick[3] = {0};
    void (*task_func[3])(context_t* context);

    context_t context = {
        .bass      = 0.0,
        .mode      = DISPLAY_MODE_DEFAULT,
        .length    = ANIMATION_HEART_LENGTH,
        .animation = &animation_heart
    };

    system_init_pll_hsi16_53();

    audio_init();
    button_init(button_pressed_callback);
    display_init();

    current_tick = system_get_tick();
    task_period_tick[0] = system_sec2tick(AUDIO_TASK_PERIOD);
    task_period_tick[1] = system_sec2tick(BUTTON_TASK_PERIOD);
    task_period_tick[2] = system_sec2tick(DISPLAY_TASK_PERIOD);
    task_next_tick[0] = current_tick;
    task_next_tick[1] = current_tick;
    task_next_tick[2] = current_tick;
    task_func[0] = audio_run;
    task_func[1] = button_run;
    task_func[2] = display_run;

    while(1)
    {
        current_tick = system_get_tick();
        for (uint32_t i = 0; i < 3; i++)
        {
            if (current_tick > task_next_tick[i])
            {
                task_func[i](&context);
                task_next_tick[i] += task_period_tick[i];
            }
        }
    }
}