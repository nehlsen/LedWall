#ifndef LEDWALL_FANCYSCRIPT_SPRINKLE_H
#define LEDWALL_FANCYSCRIPT_SPRINKLE_H

#include "FancyPart.h"

namespace LedWall::Mode::FancyParts {

class Sprinkle : public FancyPart
{
public:
    using FancyPart::FancyPart;

    uint16_t getFrameCount() const override;

protected:
    void renderImpl(uint16_t relativeFrame) override;
};

}

#endif //LEDWALL_FANCYSCRIPT_SPRINKLE_H
