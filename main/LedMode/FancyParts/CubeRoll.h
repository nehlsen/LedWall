#ifndef LEDWALL_FANCYSCRIPT_CUBEROLL_H
#define LEDWALL_FANCYSCRIPT_CUBEROLL_H

#include "FancyPart.h"

namespace LedWall::Mode::FancyParts {

class CubeRoll : public FancyPart
{
public:
    using FancyPart::FancyPart;

    uint16_t getFrameCount() const override;

protected:
    void renderImpl(uint16_t relativeFrame) override;
};

}

#endif //LEDWALL_FANCYSCRIPT_CUBEROLL_H
