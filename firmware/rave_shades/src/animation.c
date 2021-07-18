#include "animation.h"

const colour_t animation_heart[ANIMATION_HEART_LENGTH][LED_COUNT] = {{ // frame 1
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}
    }, { // frame 2
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}
    }, { // frame 3
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0},
        {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0},
        {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}
    }, { // frame 4
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0},
        {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0},
        {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}
    }, { // frame 5
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}
    }, { // frame 6
        {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  0,   0,   0}, {  0,   0,   0}
    }, { // frame 7
        {  0,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  0,   0,   0},
        {  4,   0,   0}, {  0,   0,   0}
    }, { // frame 8
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  0,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0}, {  4,   0,   0},
        {  4,   0,   0}, {  4,   0,   0}
}};