#ifndef LEDWALL_FANCYDEMOPART2_H
#define LEDWALL_FANCYDEMOPART2_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with horizontal bars, open to the top and bottom
class FdBarsOpen : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;
//    explicit FdBarsOpen(FancyDemoPart *previousPart, bool growHorizontal);

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;

//    const bool m_growHorizontal;
};

}

#endif //LEDWALL_FANCYDEMOPART2_H
