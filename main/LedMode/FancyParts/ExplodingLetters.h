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

protected:
    void renderImpl(uint16_t relativeFrame) override;

    std::string getLetters() const;
    int getFrameCountExplosion() const;
    int getStillFrameCount() const;
};

}

#endif //LEDWALL_FANCYSCRIPT_EXPLODINGLETTERS_H
