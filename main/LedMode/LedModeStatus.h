#ifndef LEDWALL_LEDMODESTATUS_H
#define LEDWALL_LEDMODESTATUS_H

#include <esp32-hal.h>

#include "LedMode.h"

class LedModeStatus: public LedMode
{
public:
    using LedMode::LedMode;

    void update() override;

protected:
    uint8_t m_lastUpdate = 99;
};

#endif //LEDWALL_LEDMODESTATUS_H
