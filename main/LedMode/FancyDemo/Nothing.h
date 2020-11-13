#ifndef LEDWALL_NOTHING_H
#define LEDWALL_NOTHING_H

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


#endif //LEDWALL_NOTHING_H
