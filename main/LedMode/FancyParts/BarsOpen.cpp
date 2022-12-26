#include "BarsOpen.h"
#include <LedMatrix.h>
#include <Line.h>

namespace LedWall::Mode::FancyParts {

uint16_t BarsOpen::getFrameCount() const
{
    if (isHorizontal()) {
        return m_matrix.getHeight() / 2;
    } else {
        return m_matrix.getWidth() / 2;
    }
}

void BarsOpen::renderImpl(uint16_t relativeFrame)
{
    if (isHorizontal()) {
        uint8_t x0 = 0;
        uint8_t x1 = m_matrix.getWidth() - 1;

        uint8_t y0 = m_matrix.getHeight() / 2 - 1;
        uint8_t y1 = m_matrix.getHeight() / 2;

        y0 -= relativeFrame;
        y1 += relativeFrame;

        Line(x0, y0, x1, y0, CRGB::White).render(m_matrix);
        Line(x0, y1, x1, y1, CRGB::White).render(m_matrix);
    } else {
        uint8_t x0 = m_matrix.getWidth() / 2 - 1;
        uint8_t x1 = m_matrix.getWidth() / 2;

        uint8_t y0 = 0;
        uint8_t y1 = m_matrix.getHeight() - 1;

        x0 -= relativeFrame;
        x1 += relativeFrame;

        Line(x0, y0, x0, y1, CRGB::White).render(m_matrix);
        Line(x1, y0, x1, y1, CRGB::White).render(m_matrix);
    }
}

}
