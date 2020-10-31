#ifndef LEDWALL_FANCYDEMOPART5_H
#define LEDWALL_FANCYDEMOPART5_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with ?, rolling cube
class FancyDemoPart5 : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

    void render(uint16_t relativeFrame) override;

protected:
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALL_FANCYDEMOPART5_H
