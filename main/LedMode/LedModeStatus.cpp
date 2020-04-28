#include "LedModeStatus.h"
#include <FastLED.h>

void LedModeStatus::update()
{
    uint8_t currentSeconds = (millis() / 1000) % 60;

    if(m_lastUpdate != currentSeconds) {
        m_lastUpdate = currentSeconds;

        matrix->pixel(0, 0) = currentSeconds % 2 ? CRGB::Red : CRGB::Green;
    }
}