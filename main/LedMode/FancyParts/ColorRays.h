#ifndef LEDWALL_FANCYSCRIPT_COLORRAYS_H
#define LEDWALL_FANCYSCRIPT_COLORRAYS_H

#include <Line.h>
#include <Canvas.h>
#include "FancyPart.h"

namespace LedWall::Mode::FancyParts {

class ColorRays : public FancyPart
{
public:
    using FancyPart::FancyPart;

    uint16_t getFrameCount() const override;

protected:
    void renderImpl(uint16_t relativeFrame) override;

    GfxPrimitive m_lines;
    void initLines();
};

}

#endif //LEDWALL_FANCYSCRIPT_COLORRAYS_H
