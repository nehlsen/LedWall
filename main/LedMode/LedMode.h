#ifndef LEDWALL_LEDMODE_H
#define LEDWALL_LEDMODE_H

#include "FastLED.h"

class cJSON;

class LedMode
{
public:
    LedMode(CRGB *leds, int ledCount);
    virtual ~LedMode();

    virtual void update() = 0;

    // read options from mode to JSON object
    virtual void readOptions(cJSON *root);
    // write options from JSON object to mode
    virtual bool writeOptions(cJSON *root);

protected:
    static uint16_t XY( uint8_t x, uint8_t y);

    CRGB *m_leds;
    int m_ledCount;
};

#endif //LEDWALL_LEDMODE_H
