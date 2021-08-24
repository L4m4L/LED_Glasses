#include "animation.h"
#include "audio.h"
#include "button.h"
#include "display.h"
#include "system.h"
#include "util.h"

static void button_pressed_callback(context_t* context)
{
    if (context->display_mode == DISPLAY_MODE_DEFAULT)
    {
        context->display_mode          = DISPLAY_MODE_ANIMATION;
        context->display_animation_len = ANIMATION_HEART_LEN;
        context->display_animation     = &animation_heart;
    }
    else if (context->display_mode == DISPLAY_MODE_ANIMATION)
    {
        context->display_mode = DISPLAY_MODE_WATERFALL;
    }
    else if (context->display_mode == DISPLAY_MODE_WATERFALL)
    {
        context->display_mode = DISPLAY_MODE_DEFAULT;
    }
}

int main(void)
{
    context_t context = {
        .audio_updated         = 0,
        .audio_volume          = {0},
        .display_mode          = DISPLAY_MODE_WATERFALL,
        .display_animation_len = ANIMATION_HEART_LEN,
        .display_animation     = &animation_heart
    };

    system_init_pll_hsi16_53();

    audio_init();
    button_init(button_pressed_callback);
    display_init();

    while(1)
    {
        audio_run(&context);
        display_run(&context);
        button_run(&context);
    }
}