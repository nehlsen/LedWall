#ifndef LEDWALL_FANCYSCRIPT_EXPLODINGLETTERS_H
#define LEDWALL_FANCYSCRIPT_EXPLODINGLETTERS_H

#include <Text.h>
#include "FancyPart.h"

namespace LedWall::Mode::FancyParts {

class ExplodingLetters : public FancyPart
{
public:
    ExplodingLetters(LedMatrix &matrix, PartOptions options, const std::string& arguments);

    uint16_t getFrameCount() const override;

    uint8_t getStillFrames() const;
    ExplodingLetters* setStillFrames(uint8_t stillFrames);

protected:
    void renderImpl(uint16_t relativeFrame) override;

    uint8_t m_stillFrames = 0;
    const Text m_letters;
};

}

#endif //LEDWALL_FANCYSCRIPT_EXPLODINGLETTERS_H
