#include "FancyDemoPart2.h"
#include <LedMatrix.h>
#include <Line.h>

namespace LedWall::Mode {

void FancyDemoPart2::render(uint16_t relativeFrame)
{
//    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t x0 = 0;
    uint8_t x1 = m_matrix.getWidth() - 1;

    uint8_t y0 = m_matrix.getHeight() / 2 - 1;
    uint8_t y1 = m_matrix.getHeight() / 2;

    y0 -= relativeFrame;
    y1 += relativeFrame;

    // bottom
    Line(x0, y0, x1, y0, CRGB::White).render(m_matrix);
    // top
    Line(x0, y1, x1, y1, CRGB::White).render(m_matrix);
}

uint16_t FancyDemoPart2::getFrameCount() const
{
    return m_matrix.getHeight() / 2;
}

}
