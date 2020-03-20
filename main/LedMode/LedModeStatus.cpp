#include "LedModeStatus.h"
#include <FastLED.h>

void LedModeStatus::update()
{
    uint8_t currentSeconds = (millis() / 1000) % 60;

    if(m_lastUpdate != currentSeconds) {
        m_lastUpdate = currentSeconds;

        FastLED.leds()[0] = currentSeconds % 2 ? CRGB::Red : CRGB::Green;
    }
}