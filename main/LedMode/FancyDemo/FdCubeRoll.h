#ifndef LEDWALL_FANCYDEMOPART5_H
#define LEDWALL_FANCYDEMOPART5_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

class FdCubeRoll : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALL_FANCYDEMOPART5_H
