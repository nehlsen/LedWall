#ifndef LEDWALL_LEDMODE_H
#define LEDWALL_LEDMODE_H

#include "FastLED.h"

class cJSON;

class LedMode
{
public:
    static void setup(uint8_t matrixWidth, uint8_t matrixHeight, bool serpentineLayout);

    virtual ~LedMode() = default;

    virtual void update() = 0;

    // read options from mode to JSON object
    virtual void readOptions(cJSON *root);
    // write options from JSON object to mode
    virtual bool writeOptions(cJSON *root);

protected:
    // number of horizontal pixels
    static uint8_t matrixWidth;
    // number of vertical pixels
    static uint8_t matrixHeight;
    // Whether all pixels are laid out all running the same way or back-and-forth (serpentine mode)
    static bool serpentineLayout;

    /**
     * map x/y coordinates to led-string index
     * XY Code and safety pixel code from
     * https://github.com/FastLED/FastLED/blob/master/examples/XYMatrix/XYMatrix.ino
     */
    static uint16_t xyToIndex(uint8_t x, uint8_t y);
};

#endif //LEDWALL_LEDMODE_H
