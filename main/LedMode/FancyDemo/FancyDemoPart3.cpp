#include "FancyDemoPart3.h"
#include <LedMatrix.h>
#include <Rect.h>

namespace LedWall::Mode {

void FancyDemoPart3::renderImpl(uint16_t relativeFrame)
{
//    if (frame < 0 || frame >= frameCount) return frameCount;

    uint8_t x = m_matrix.getWidth() / 2 - 1 - relativeFrame;
    uint8_t y = m_matrix.getHeight() / 2 - 1 - relativeFrame;

    uint8_t w = 1 + 2*relativeFrame;
    uint8_t h = 1 + 2*relativeFrame;

    Rect(x, y, w, h, CRGB::White).render(m_matrix);
}

uint16_t FancyDemoPart3::getFrameCount() const
{
    return m_matrix.getHeight() / 2;
}

}
