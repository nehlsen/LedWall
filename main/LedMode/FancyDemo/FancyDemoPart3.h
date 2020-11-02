#ifndef LEDWALL_FANCYDEMOPART3_H
#define LEDWALL_FANCYDEMOPART3_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with ?, grow to cube
class FancyDemoPart3 : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALL_FANCYDEMOPART3_H
