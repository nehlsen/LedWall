#include "FdExplodingLetters.h"
#include <Canvas.h>
#include "../utilities.h"

namespace LedWall::Mode {

const uint16_t targetZoom = 750;

FdExplodingLetters::FdExplodingLetters(uint16_t firstFrame, LedMatrix &matrix, const std::string &letters):
    FancyDemoPart(firstFrame, matrix), m_letters(letters, CRGB::White)
{}

FdExplodingLetters::FdExplodingLetters(FancyDemoPart *previousPart, const std::string &letters):
    FancyDemoPart(previousPart), m_letters(letters, CRGB::White)
{}

void FdExplodingLetters::renderImpl(uint16_t relativeFrame)
{
    Canvas c(m_letters.pixels());
    c
        .setZoom(map(relativeFrame, 0, 25, 100, targetZoom))
        .applyTransformation()
        .renderCentered(m_matrix)
    ;
}

uint16_t FdExplodingLetters::getFrameCount() const
{
    return 25; // one second
}

}
