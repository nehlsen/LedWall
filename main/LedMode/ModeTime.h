#ifndef LEDWALL_MODETIME_H
#define LEDWALL_MODETIME_H

#include "ModeText.h"

namespace LedWall {
namespace Mode {

class ModeTime : public ModeText
{
public:
    explicit ModeTime(LedMatrix& matrix);

    bool update() override;

private:
    int64_t m_lastTimeUpdate = 0;
};

} // namespace Mode
} // namespace LedWall

#endif //LEDWALL_MODETIME_H
