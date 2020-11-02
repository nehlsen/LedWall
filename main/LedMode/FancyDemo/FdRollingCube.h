#ifndef LEDWALL_FANCYDEMOPART5_H
#define LEDWALL_FANCYDEMOPART5_H

#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with ?, rolling cube
class FdRollingCube : public FancyDemoPart
{
public:
    explicit FdRollingCube(FancyDemoPart *previousPart, bool directionForward);

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;

    const bool m_directionForward = true;
};

}

#endif //LEDWALL_FANCYDEMOPART5_H
