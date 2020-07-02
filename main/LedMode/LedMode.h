#ifndef LEDWALL_LEDMODE_H
#define LEDWALL_LEDMODE_H

#include "FastLED.h"
#include <LedMatrix.h>

class cJSON;

namespace LedWall {
namespace Mode {

class LedMode
{
public:
    explicit LedMode(LedMatrix& matrix);
    virtual ~LedMode() = default;

    // return true if something has changed and needs to be written to physical LEDs, false else
    virtual bool update() = 0;
    virtual int framesPerSecond() const;
    virtual int frameDelay() const;

    // read options from mode to JSON object
    virtual void readOptions(cJSON *root);
    // write options from JSON object to mode
    //   return true if something has changed, false else
    virtual bool writeOptions(cJSON *root);

protected:
    LedMatrix& m_matrix;
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_LEDMODE_H
