#ifndef LEDWALL_FANCYDEMOPART2_H
#define LEDWALL_FANCYDEMOPART2_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with horizontal bars, open to the top and bottom
class FancyDemoPart2 : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

    void render(uint16_t relativeFrame) override;

protected:
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALL_FANCYDEMOPART2_H
