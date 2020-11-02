#include "FdExplodingLetters.h"
#include <Canvas.h>
#include "../utilities.h"

namespace LedWall::Mode {

const uint16_t targetZoom = 750;

FdExplodingLetters::FdExplodingLetters(FancyDemoPart *previousPart, const std::string &letters, PartOptions options):
    FancyDemoPart(previousPart, options), m_letters(letters, CRGB::White)
{}

void FdExplodingLetters::renderImpl(uint16_t relativeFrame)
{
    Canvas c(m_letters.pixels());
    c
        .setSubPixelTransformationFactor(2)
        .setZoom(map(relativeFrame, 0, getFrameCount(), 100, targetZoom))
        .applyTransformation()
        .renderCentered(m_matrix)
    ;
}

uint16_t FdExplodingLetters::getFrameCount() const
{
    return 25; // 25 frames = one second
}

}
