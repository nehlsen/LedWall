#ifndef LEDWALL_FDCUBEGROW_H
#define LEDWALL_FDCUBEGROW_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

class FdCubeGrow : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALL_FDCUBEGROW_H
