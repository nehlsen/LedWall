#include "FdCircleGrow.h"
#include <LedMatrix.h>
#include <Circle.h>

namespace LedWall::Mode {

void FdCircleGrow::renderImpl(uint16_t relativeFrame)
{
    // looks nice!
//    uint8_t x = m_matrix.getWidth() / 2 - 1 - relativeFrame;
//    uint8_t y = m_matrix.getHeight() / 2 - 1 - relativeFrame;
    uint8_t x = m_matrix.getWidth() / 2 - 1;
    uint8_t y = m_matrix.getHeight() / 2 - 1;

    uint8_t r = 1 + 2*relativeFrame;

    Circle(x, y, r, CRGB::White).render(m_matrix);
}

uint16_t FdCircleGrow::getFrameCount() const
{
    return m_matrix.getHeight() / 2;
}

}
