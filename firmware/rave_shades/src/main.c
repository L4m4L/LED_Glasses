#include <stdint.h>

#include "display.h"
#include "system.h"
#include "util.h"

int main(void)
{
    context_t context = {
        .display_text = "HELLO WORLD",
        .mic_pk2pk = 0,
        .display_mode = DISPLAY_MODE_PK2PK
    };

    system_init_pll_hsi16_53();

    while(1)
    {
        display_task(&context);
    }
}