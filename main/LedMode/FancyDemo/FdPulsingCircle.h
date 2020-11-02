#ifndef LEDWALLSTUDIO_FDPULSINGCIRCLE_H
#define LEDWALLSTUDIO_FDPULSINGCIRCLE_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

class FdPulsingCircle : public FancyDemoPart
{
public:
//    explicit FdBarsGrow(uint16_t firstFrame, LedMatrix &matrix, bool growHorizontal);
//    explicit FdBarsGrow(FancyDemoPart *previousPart, bool growHorizontal);

    void render(uint16_t relativeFrame) override;

protected:
    uint16_t getFrameCount() const override;

//    const bool m_growHorizontal;
};

}

#endif //LEDWALLSTUDIO_FDPULSINGCIRCLE_H
