#ifndef LEDWALL_LEDMODESTATUS_H
#define LEDWALL_LEDMODESTATUS_H

#include <esp32-hal.h>

#include "LedMode.h"

namespace LedWall {
namespace Mode {

class LedModeStatus: public LedMode
{
public:
    using LedMode::LedMode;

    bool update() override;

protected:
    uint8_t m_lastUpdate = 99;
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_LEDMODESTATUS_H
