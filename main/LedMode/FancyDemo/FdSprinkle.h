#ifndef LEDWALL_FANCYDEMOPART7_H
#define LEDWALL_FANCYDEMOPART7_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// sprinkle
class FdSprinkle : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALL_FANCYDEMOPART7_H
