#ifndef LEDWALL_FANCYDEMOPART1_H
#define LEDWALL_FANCYDEMOPART1_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with centered box, grow bars horizontally
class FdBarsGrow : public FancyDemoPart
{
public:
    explicit FdBarsGrow(uint16_t firstFrame, LedMatrix &matrix, bool growHorizontal);
    explicit FdBarsGrow(FancyDemoPart *previousPart, bool growHorizontal);

    void render(uint16_t relativeFrame) override;

protected:
    uint16_t getFrameCount() const override;

    const bool m_growHorizontal;
};

}

#endif //LEDWALL_FANCYDEMOPART1_H
