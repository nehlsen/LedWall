#ifndef LEDWALL_FDCIRCLEGROW_H
#define LEDWALL_FDCIRCLEGROW_H

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

#endif //LEDWALL_FDCIRCLEGROW_H
