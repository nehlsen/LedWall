#ifndef LEDWALL_LEDMODE_H
#define LEDWALL_LEDMODE_H

#include "FastLED.h"
#include <LedMatrix.h>

class cJSON;

class LedMode
{
public:
    static void setup(LedMatrix* matrix);

    virtual ~LedMode() = default;

    virtual void update() = 0;

    // read options from mode to JSON object
    virtual void readOptions(cJSON *root);
    // write options from JSON object to mode
    virtual bool writeOptions(cJSON *root);

protected:
    static LedMatrix* matrix;
};

#endif //LEDWALL_LEDMODE_H
