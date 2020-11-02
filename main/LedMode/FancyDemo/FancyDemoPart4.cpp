#include "FancyDemoPart4.h"
#include <LedMatrix.h>
#include <Rect.h>

namespace LedWall::Mode {

void FancyDemoPart4::renderImpl(uint16_t relativeFrame)
{
//    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t x = (m_matrix.getWidth() - m_matrix.getHeight()) / 2 + relativeFrame;
    uint8_t y = relativeFrame;

    uint8_t w = m_matrix.getHeight() - 2*relativeFrame - 1;
    uint8_t h = m_matrix.getHeight() - 2*relativeFrame - 1;

    Rect(x, y, w, h, CRGB::White).render(m_matrix);
}

uint16_t FancyDemoPart4::getFrameCount() const
{
    return (m_matrix.getHeight() - 10) * (2.0/3.0);
}

}
