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

protected:
    uint16_t m_frame = 0;

    int16_t part1(int16_t frame); // bars grow horizontally
    int16_t part2(int16_t frame); // bars open to top/bottom
    int16_t part3(int16_t frame); // growing cube
    int16_t part4(int16_t frame); // shrinking cube
    int16_t part5(int16_t frame); // rolling cube
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_FANCYDEMO_H
