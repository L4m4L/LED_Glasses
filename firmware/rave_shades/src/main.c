#include "audio.h"
#include "button.h"
#include "display.h"
#include "system.h"
#include "util.h"

static void button_pressed_callback(context_t* context)
{
    switch(context->display_mode)
    {
    case DISPLAY_MODE_DEFAULT:
        context->display_mode = DISPLAY_MODE_HEART;
        break;
    case DISPLAY_MODE_HEART:
        context->display_mode = DISPLAY_MODE_WATERFALL;
        break;
    case DISPLAY_MODE_WATERFALL:
        context->display_mode = DISPLAY_MODE_LACHLAN;
        break;
	case DISPLAY_MODE_LACHLAN:
		context->display_mode = DISPLAY_MODE_DEFAULT;
		break;
    default:
        context->display_mode = DISPLAY_MODE_DEFAULT;
    }
}

int main(void)
{
    context_t context = {
        .audio_updated = 0,
        .audio_volume  = {0},
        .display_mode  = DISPLAY_MODE_DEFAULT,
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