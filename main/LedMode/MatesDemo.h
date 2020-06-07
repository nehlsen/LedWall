#ifndef LEDWALL_MATESDEMO_H
#define LEDWALL_MATESDEMO_H

#include "LedMode.h"

namespace LedWall {
namespace Mode {

class MatesDemo: public LedMode
{
public:
    using LedMode::LedMode;

    bool update() override;

protected:
    uint8_t m_lastUpdate = 99;

    void drawSample(int index);
    void drawSampleLetter(char *c);
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_MATESDEMO_H
