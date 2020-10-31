#include "FancyDemoPart1.h"
#include <LedMatrix.h>
#include <Line.h>

namespace LedWall::Mode {

//FancyDemoPart1::FancyDemoPart1(uint16_t firstFrame, LedMatrix &matrix):
//    FancyDemoPart(firstFrame, matrix)
//{}

void FancyDemoPart1::render(uint16_t relativeFrame)
{
//    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t x0 = std::round(m_matrix.getWidth() / 2.0 - 1.5);
    uint8_t x1 = x0 + 2;

    uint8_t y0 = m_matrix.getHeight() / 2;
    uint8_t y1 = y0 - 1;

    x0 -= relativeFrame;
    x1 += relativeFrame;

    // bottom
    Line(x0, y0, x1, y0, CRGB::White).render(m_matrix);
    // top
    Line(x0, y1, x1, y1, CRGB::White).render(m_matrix);
}

uint16_t FancyDemoPart1::getFrameCount() const
{
    return m_matrix.getWidth() / 2 - 1;
}

}
