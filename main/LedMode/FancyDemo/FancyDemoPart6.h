#ifndef LEDWALL_FANCYDEMOPART6_H
#define LEDWALL_FANCYDEMOPART6_H

#include <Line.h>
#include <Canvas.h>
#include "FancyDemoPart.h"

namespace LedWall::Mode {

// start with ?, ?
class FancyDemoPart6 : public FancyDemoPart
{
public:
    explicit FancyDemoPart6(FancyDemoPart *previousPart, bool directionForward);

    void render(uint16_t relativeFrame) override;

protected:
    uint16_t getFrameCount() const override;

    GfxPrimitive m_lines;
    void initLines();

    const bool m_directionForward = true;
};

}

#endif //LEDWALL_FANCYDEMOPART6_H
