#ifndef LEDWALLSTUDIO_NOTHING_H
#define LEDWALLSTUDIO_NOTHING_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

class Nothing : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;
};

}


#endif //LEDWALLSTUDIO_NOTHING_H
