#ifndef LEDWALL_FDEXPLODINGLETTERS_H
#define LEDWALL_FDEXPLODINGLETTERS_H

#include <Text.h>
#include "FancyDemoPart.h"

namespace LedWall::Mode {

class FdExplodingLetters : public FancyDemoPart
{
public:
    explicit FdExplodingLetters(uint16_t firstFrame, LedMatrix &matrix, const std::string &letters);
    explicit FdExplodingLetters(FancyDemoPart *previousPart, const std::string &letters);

protected:
    void renderImpl(uint16_t relativeFrame) override;
    uint16_t getFrameCount() const override;

    const Text m_letters;
};

}

#endif //LEDWALL_FDEXPLODINGLETTERS_H
