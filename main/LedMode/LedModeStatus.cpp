#include "LedModeStatus.h"
#include <FastLED.h>

namespace LedWall {
namespace Mode {

void LedModeStatus::update()
{
    uint8_t currentSeconds = (millis() / 1000) % 60;

    if(m_lastUpdate != currentSeconds) {
        m_lastUpdate = currentSeconds;

        m_matrix.pixel(0, 0) = (currentSeconds % 2) ? CRGB::Red : CRGB::Green;
    }
}

} // namespace Mode
} // namespace LedWall
