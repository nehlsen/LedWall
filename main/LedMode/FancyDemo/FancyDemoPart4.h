#ifndef LEDWALL_FANCYDEMOPART4_H
#define LEDWALL_FANCYDEMOPART4_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with ?, shrink cube
class FancyDemoPart4 : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

    void render(uint16_t relativeFrame) override;

protected:
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALL_FANCYDEMOPART4_H
