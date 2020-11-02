#ifndef LEDWALL_FDEXPLODINGLETTERS_H
#define LEDWALL_FDEXPLODINGLETTERS_H

#include <Text.h>
#include "FancyDemoPart.h"

namespace LedWall::Mode {

class FdExplodingLetters : public FancyDemoPart
{
public:
    explicit FdExplodingLetters(FancyDemoPart *previousPart, const std::string &letters, PartOptions options = NO_OPS);

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;

    const Text m_letters;
};

}

#endif //LEDWALL_FDEXPLODINGLETTERS_H
