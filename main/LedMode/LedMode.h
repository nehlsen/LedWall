#ifndef LEDWALL_LEDMODE_H
#define LEDWALL_LEDMODE_H

#include "FastLED.h"

class cJSON;

class LedMode
{
public:
    virtual ~LedMode() = default;

    virtual void update() = 0;

    // read options from mode to JSON object
    virtual void readOptions(cJSON *root);
    // write options from JSON object to mode
    virtual bool writeOptions(cJSON *root);
};

#endif //LEDWALL_LEDMODE_H
