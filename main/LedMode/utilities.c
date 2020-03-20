#include "utilities.h"

#include "sdkconfig.h"

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t xyToIndex(uint8_t x, uint8_t y)
{
    // WARNING: the following code assumes LEDs have a "safety pixel"!!!
    if (x >= CONFIG_NUM_LEDS_HORIZONTAL) {
        return -1;
    }

    if (y >= CONFIG_NUM_LEDS_VERTICAL) {
        return -1;
    }

    uint16_t index;

    if (CONFIG_MATRIX_SERPENTINE_LAYOUT == 0) {
        index = (y * CONFIG_NUM_LEDS_HORIZONTAL) + x;
    }

    if (CONFIG_MATRIX_SERPENTINE_LAYOUT != 0) {
        if (y & 0x01) {
            // Odd rows run backwards
            uint8_t reverseX = (CONFIG_NUM_LEDS_HORIZONTAL - 1) - x;
            index = (y * CONFIG_NUM_LEDS_HORIZONTAL) + reverseX;
        } else {
            // Even rows run forwards
            index = (y * CONFIG_NUM_LEDS_HORIZONTAL) + x;
        }
    }

    return index;
}
