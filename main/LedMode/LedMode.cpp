#include "LedMode.h"

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
