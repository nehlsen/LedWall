#ifndef LEDWALL_FANCYDEMOPART6_H
#define LEDWALL_FANCYDEMOPART6_H

#include <Line.h>
#include <Canvas.h>
#include "FancyDemoPart.h"

namespace LedWall::Mode {

class FdColorRays : public FancyDemoPart
{
public:
    using FancyDemoPart::FancyDemoPart;

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;

    GfxPrimitive m_lines;
    void initLines();
};

}

#endif //LEDWALL_FANCYDEMOPART6_H
