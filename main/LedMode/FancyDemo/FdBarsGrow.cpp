#include "FdBarsGrow.h"
#include <LedMatrix.h>
#include <Line.h>

namespace LedWall::Mode {

FdBarsGrow::FdBarsGrow(uint16_t firstFrame, LedMatrix &matrix, bool growHorizontal):
    FancyDemoPart(firstFrame, matrix), m_growHorizontal(growHorizontal)
{}

FdBarsGrow::FdBarsGrow(FancyDemoPart *previousPart, bool growHorizontal):
    FancyDemoPart(previousPart), m_growHorizontal(growHorizontal)
{}

void FdBarsGrow::render(uint16_t relativeFrame)
{
    if (m_growHorizontal) {
        uint8_t x0 = std::round(m_matrix.getWidth() / 2.0 - 1.5);
        uint8_t x1 = x0 + 2;

        uint8_t y0 = m_matrix.getHeight() / 2;
        uint8_t y1 = y0 - 1;

        x0 -= relativeFrame;
        x1 += relativeFrame;

        Line(x0, y0, x1, y0, CRGB::White).render(m_matrix);
        Line(x0, y1, x1, y1, CRGB::White).render(m_matrix);
    } else {
        uint8_t x0 = m_matrix.getWidth() / 2;
        uint8_t x1 = x0 - 1;

        uint8_t y0 = std::round(m_matrix.getHeight() / 2.0 - 1.5);
        uint8_t y1 = y0 + 2;

        y0 -= relativeFrame;
        y1 += relativeFrame;

        Line(x0, y0, x0, y1, CRGB::White).render(m_matrix);
        Line(x1, y0, x1, y1, CRGB::White).render(m_matrix);
    }
}

uint16_t FdBarsGrow::getFrameCount() const
{
    if (m_growHorizontal) {
        return m_matrix.getWidth() / 2 - 1;
    } else {
        return m_matrix.getHeight() / 2 - 1;
    }
}

}
