#ifndef LEDWALL_FANCYDEMOPART2_H
#define LEDWALL_FANCYDEMOPART2_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with horizontal bars, open to the top and bottom
class FdBarsOpen : public FancyDemoPart
{
public:
    explicit FdBarsOpen(FancyDemoPart *previousPart, bool growHorizontal);

    void render(uint16_t relativeFrame) override;

protected:
    uint16_t getFrameCount() const override;

    const bool m_growHorizontal;
};

}

#endif //LEDWALL_FANCYDEMOPART2_H
