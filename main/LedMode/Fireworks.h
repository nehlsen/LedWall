#ifndef LEDWALL_FIREWORKS_H
#define LEDWALL_FIREWORKS_H

#include "LedMode.h"

namespace LedWall {
namespace Mode {

class Fireworks: public LedMode
{
public:
    using LedMode::LedMode;

    void update() override;

    void readOptions(cJSON *root) override;

    bool writeOptions(cJSON *root) override;

protected:
    uint8_t m_fadeRate = 240; // how fast to fade: 1-fast, 256-slow
    uint16_t m_sparkRate = 240;
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_FIREWORKS_H
