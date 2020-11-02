#ifndef LEDWALL_FANCYDEMOPART6_H
#define LEDWALL_FANCYDEMOPART6_H

#include <Line.h>
#include <Canvas.h>
#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with ?, ?
class FdColorRays : public FancyDemoPart
{
public:
    explicit FdColorRays(FancyDemoPart *previousPart, bool directionForward);

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;

    GfxPrimitive m_lines;
    void initLines();

    const bool m_directionForward = true;
};

}

#endif //LEDWALL_FANCYDEMOPART6_H
