#ifndef LEDWALLSTUDIO_FDCIRCLEGROW_H
#define LEDWALLSTUDIO_FDCIRCLEGROW_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

class FdCircleGrow : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALLSTUDIO_FDCIRCLEGROW_H
