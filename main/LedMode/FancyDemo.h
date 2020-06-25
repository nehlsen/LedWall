#ifndef LEDWALL_FANCYDEMO_H
#define LEDWALL_FANCYDEMO_H

#include "LedMode.h"

namespace LedWall {
namespace Mode {

class FancyDemo : public LedMode
{
public:
    using LedMode::LedMode;

    bool update() override;
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_FANCYDEMO_H
