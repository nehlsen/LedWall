#include "CubeGrow.h"
#include <LedMatrix.h>
#include <Rect.h>

namespace LedWall::Mode::FancyParts {

uint16_t CubeGrow::getFrameCount() const
{
    return m_matrix.getHeight() / 2;
}

void CubeGrow::renderImpl(uint16_t relativeFrame)
{
    m_matrix.clear(false);

    uint8_t x = m_matrix.getWidth() / 2 - 1 - relativeFrame;
    uint8_t y = m_matrix.getHeight() / 2 - 1 - relativeFrame;

    uint8_t w = 1 + 2*relativeFrame;
    uint8_t h = 1 + 2*relativeFrame;

    Rect(x, y, w, h, CRGB::White).render(m_matrix);
}

}
