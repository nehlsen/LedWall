#ifndef LEDWALL_FDNHEARTC_H
#define LEDWALL_FDNHEARTC_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

class FdNHeartC : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;
};

}

#endif //LEDWALL_FDNHEARTC_H
