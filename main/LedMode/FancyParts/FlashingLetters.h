#ifndef LEDWALL_FANCYSCRIPT_FLASHINGLETTERS_H
#define LEDWALL_FANCYSCRIPT_FLASHINGLETTERS_H

#include "FancyPart.h"

namespace LedWall::Mode::FancyParts {

class FlashingLetters : public FancyPart
{
public:
    using FancyPart::FancyPart;

    uint16_t getFrameCount() const override;

protected:
    void renderImpl(uint16_t relativeFrame) override;

    std::string getLetters() const;
    int getFramesPerLetter() const;
    int getBlankFramesPerLetter() const;
};

}

#endif //LEDWALL_FANCYSCRIPT_FLASHINGLETTERS_H
