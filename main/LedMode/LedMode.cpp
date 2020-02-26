/**
 * XY Code and safety pixel code from
 * https://github.com/FastLED/FastLED/blob/master/examples/XYMatrix/XYMatrix.ino
 */

#include "LedMode.h"
#include "sdkconfig.h"

LedMode::LedMode(CRGB *leds, int ledCount):
    m_leds(leds), m_ledCount(ledCount)
{}

LedMode::~LedMode()
= default;

void LedMode::readOptions(cJSON *root)
{}

bool LedMode::writeOptions(cJSON *root)
{
    return true;
}

uint16_t LedMode::XY(uint8_t x, uint8_t y)
{
    // WARNING: the following code assumes m_leds has a "safety pixel"!!!
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
