#ifndef UTIL_H
#define UTIL_H

typedef enum display_mode_e
{
    DISPLAY_MODE_LINES,
    DISPLAY_MODE_PK2PK,
    DISPLAY_MODE_SCROLL
} display_mode_e;

typedef struct context_t
{
    char display_text[50];
    uint8_t mic_pk2pk;
    display_mode_e display_mode;
} context_t;

#endif // #ifndef UTIL_H